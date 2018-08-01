#include <xmaxnet_plugin.hpp>
#include <google/protobuf/stubs/common.h>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/host_name.hpp>
#include <pro/log/log.hpp>
#include <functional>
#include "netmessage_pool.hpp"
#include "xmx_connection.hpp"
#include "netmessage.hpp"
#include "netmessage.pb.h"
#include "pro/types/time.hpp"
#include "netutl.hpp"


#ifdef USE_UPNP
#include "miniwget.h"
#include "miniupnpc.h"
#include "upnpcommands.h"
#include "portlistingparse.h"
#include "upnperrors.h"
#include "miniupnpcstrings.h"
#endif


using namespace google::protobuf;

namespace xmax {
	using namespace xmaxapp;
	using namespace pro;
	using namespace google::protobuf;
	using boost::asio::ip::tcp;
	using boost::asio::ip::address_v4;
	using boost::asio::ip::host_name;
	using boost::asio::ip::address;

	const char* s_ServerAddress = "server-address";
	const char* s_PeerAddress   = "peer-address";
	const char* s_UPNP = "upnp";

	/**
	*  Implementation details of the xmax net plugin
	*
	*/
	class XmaxNetPluginImpl {	
	public:
		XmaxNetPluginImpl(const boost::asio::io_service& io);
		~XmaxNetPluginImpl();

	public:
		/**
		* Init p2p newwork params
		*/
		void Init(const VarsMap& options);

		void StartupImpl();

		void ConnectImpl(const std::string& host);

		void StartResolve(std::shared_ptr<XMX_Connection> pConnect);

		void StartConnect(std::shared_ptr<XMX_Connection> pConnect, tcp::resolver::iterator itr);
		
		/**
		*  start listening loop
		*/
		void StartListen();
		/**
		*  async read a message
		*/
		void StartRecvMsg(std::shared_ptr<XMX_Connection> pConnect);
		
	protected:

		bool _IsConnectd(const std::string& host);

		void _ParseMsg(const char* pMsg, const MsgHeader& header, std::shared_ptr<XMX_Connection> pConnect);

		void _OnHandleMsg(std::shared_ptr<XMX_Connection> pConnect, const HelloMsg& msg);

		void _ConnectionTimer();

		void _CheckConnection();

		void _Disconnect(std::shared_ptr<XMX_Connection> pc);

		void _DetectExternalAddr();

		void _MapPort();

	private:

		std::unique_ptr<tcp::acceptor>							acceptor_;
		std::unique_ptr<tcp::resolver>							resolver_;
		tcp::endpoint											endpoint_;
		std::vector<std::shared_ptr<XMX_Connection>>			connections_;
		uint													nMaxClients_;
		uint													nCurrClients_;
		std::string												seedServer_;
		std::vector<std::string>								peerAddressList_;
		std::string												listeningPort_;
		std::string												localAddr_;
		std::string												externalAddr_;
		bool													bUpnp_;

		const boost::asio::io_service&							ioService_;
		boost::asio::deadline_timer								connectionTimer_;
	};

	/**
	*  Implementations of XmaxNetPluginImpl interfaces
	*
	*/

	XmaxNetPluginImpl::XmaxNetPluginImpl(const boost::asio::io_service& io)
		: nMaxClients_(30),
		  nCurrClients_(0),
		  connectionTimer_(const_cast<boost::asio::io_service&>(io)),
		  ioService_(io),
		  bUpnp_(false)
	{

	}


	XmaxNetPluginImpl::~XmaxNetPluginImpl()
	{
		ShutdownProtobufLibrary();

		if (acceptor_ != nullptr)
		{
			acceptor_->close();
			acceptor_.reset(nullptr);
			Logf(" acceptor is closed");
		}

		for (auto pConnect : connections_)
		{
			pConnect->Close();
		}
	}
	/**
	*  Initialize network
	*
	*/
	void XmaxNetPluginImpl::Init(const VarsMap& options)
	{
		bUpnp_ = options.at(s_UPNP).as<bool>();

		resolver_ = std::make_unique<tcp::resolver>( const_cast<boost::asio::io_service&>(ioService_) );
		acceptor_.reset(new tcp::acceptor(const_cast<boost::asio::io_service&>(ioService_)));

		if (options.count(s_ServerAddress))
		{
			seedServer_		 = options.at(s_ServerAddress).as<std::string>();
			size_t spos		 = seedServer_.find(':');
			std::string addr = seedServer_.substr(0, spos);
			std::string port = seedServer_.substr(spos + 1, seedServer_.size());

			listeningPort_	 = port;
			localAddr_		 = addr;

			tcp::resolver::query query(tcp::v4(), addr.c_str(), port.c_str());
			endpoint_ = *resolver_->resolve(query);
			acceptor_.reset(new tcp::acceptor(const_cast<boost::asio::io_service&>(ioService_)));
		}

		if (options.count(s_PeerAddress))
		{
			peerAddressList_ = options.at(s_PeerAddress).as<std::vector<std::string>>();
		}
	}

	void XmaxNetPluginImpl::StartupImpl()
	{
		if (acceptor_ != nullptr)
		{
			acceptor_->open(endpoint_.protocol());
			acceptor_->set_option(tcp::acceptor::reuse_address(true));
			acceptor_->bind(endpoint_);
			acceptor_->listen();
			StartListen();
			Logf("start to listen incoming peers");

			_DetectExternalAddr();
		}

		for (const std::string& peer : peerAddressList_)
		{
			ConnectImpl(peer);
		}

	}

	void XmaxNetPluginImpl::StartListen()
	{
		std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(const_cast<boost::asio::io_service&>(ioService_));
		
		auto onAccept = [socket, this](boost::system::error_code ec)
		{
			if ( !ec )
			{
				if (nCurrClients_ < nMaxClients_)
				{
					std::shared_ptr<XMX_Connection> pConnect = std::make_shared<XMX_Connection>(socket);
					connections_.push_back(pConnect);
					tcp::no_delay nd(true);
					pConnect->GetSocket()->set_option(nd);
					pConnect->SetInitiative(false);
					StartRecvMsg(pConnect);
					nMaxClients_++;
				}
				else
				{
					Warnf("MaxClinet exceeded!!!!\n");
					socket->close();
				}

				StartListen();
			}
			else
			{
				ErrorSprintf("accept error: %s", ec.message());
			}
		};
		
		acceptor_->async_accept(*socket, onAccept);
	}

	void XmaxNetPluginImpl::StartRecvMsg(std::shared_ptr<XMX_Connection> connection)
	{
		if (connection->GetSocket() == nullptr)
		{
			return;
		}

		try
		{
			auto onReadFunc = [connection, this](boost::system::error_code ec, std::size_t bytes_read)
			{
				if (ec)
				{
					if ((boost::asio::error::eof == ec) ||
						(boost::asio::error::connection_reset == ec))
					{			
						LogSprintf("peer disconnected: %s", connection->GetPeerAddress());
					}
					else
					{
						WarnSprintf("read msg from %s error : %s", connection->GetPeerAddress().c_str(), ec.message().c_str());
					}
					_Disconnect(connection);
					return;
				}
				else
				{
					MessagePoolBuffer* msg_pool_buf = connection->GetMsgBuffer();
					size_t nCanWrite = msg_pool_buf->AvailableBytes();
					if (bytes_read > nCanWrite)
					{
						ErrorSprintf("read msg bytes exceeded msg buffer size\n");
						return;
					}

					msg_pool_buf->IncrementWriteIndex(bytes_read);
					uint32_t nCanReadBytes = msg_pool_buf->CanReadBytes();
					while (nCanReadBytes > 0)
					{
						if (nCanReadBytes < sizeof(MsgHeader)  )
						{
							break;
						}

						MsgHeader msgHeader;
						bufferIndex readIndex = msg_pool_buf->GetReadIndex();
						bool bGet             = msg_pool_buf->TryGetData(&msgHeader, sizeof(MsgHeader), readIndex);
						if (!bGet)
						{
							break;
						}

						uint32_t msgSize = msgHeader.msgLength + sizeof(MsgHeader);
						if (nCanReadBytes >= msgSize)
						{
							msg_pool_buf->IncrementReadIndex(sizeof(MsgHeader));
							char* pMsgData = new char[msgHeader.msgLength];
							bool bret = msg_pool_buf->GetData(pMsgData, msgHeader.msgLength);

							if (!bret)
							{
								delete[] pMsgData;
								break;
							}	

							_ParseMsg(pMsgData, msgHeader, connection);
							delete[] pMsgData;

							nCanReadBytes = msg_pool_buf->CanReadBytes();
						}
						else
						{
							uint32_t remainMsgLength = msgSize - nCanReadBytes;
							uint32_t canWriteLength  = msg_pool_buf->AvailableBytes();

							if (remainMsgLength > canWriteLength)
							{
								msg_pool_buf->Allocate(remainMsgLength - canWriteLength);
							}

							break;
						}
					}

					StartRecvMsg(connection);
				}
			};
			std::vector<boost::asio::mutable_buffer> mbBuffers = connection->GetMsgBuffer()->GetAvailableBufferFromPool();
			connection->GetSocket()->async_read_some(mbBuffers, onReadFunc);


		}
		catch (...)
		{

		}
	}

	void XmaxNetPluginImpl::ConnectImpl(const std::string& host)
	{
		if (_IsConnectd(host))
		{
			Warnf("duplicated connection\n");
			return;
		}

		size_t pos = host.find(':');

		if (pos == std::string::npos)
		{
			Warnf("invalid peer address format\n");
			return;
		}

		std::shared_ptr<tcp::socket> s = std::make_shared<tcp::socket>(const_cast<boost::asio::io_service&>(ioService_));

		std::shared_ptr<XMX_Connection> pConnect = std::make_shared<XMX_Connection>(host, s);
		connections_.push_back(pConnect);

		StartResolve(pConnect);
	}

	void XmaxNetPluginImpl::StartResolve(std::shared_ptr<XMX_Connection> pConnect)
	{
		const std::string host = pConnect->GetPeerAddress();
		size_t pos = host.find(':');

		std::string addr = host.substr(0, pos);
		std::string port = host.substr(pos + 1);

		tcp::resolver::query query(tcp::v4(), addr.c_str(), port.c_str());
		auto onResolve = [pConnect, host, this](const boost::system::error_code& ec, tcp::resolver::iterator itr)
		{
			if (!ec)
			{
				StartConnect(pConnect, itr);
			}
			else
			{
				WarnSprintf("resolve %s error : %s", host.c_str(), ec.message().c_str());
				connections_.pop_back();
			}
		};

		resolver_->async_resolve(query, onResolve);
	}

	void XmaxNetPluginImpl::StartConnect(std::shared_ptr<XMX_Connection> pConnect, tcp::resolver::iterator itr)
	{
		pConnect->SetConStatus(CS_CONNECTING);
		tcp::resolver::iterator currItr = itr;
		currItr++;

		auto onConnect = [pConnect, currItr, this](const boost::system::error_code& ec)
		{
			if (!ec)
			{
				pConnect->SetConStatus(CS_CONNECTED);
				StartRecvMsg(pConnect);
			} 
			else
			{
				if (currItr != tcp::resolver::iterator())
				{
					pConnect->Close();
					StartConnect(pConnect, currItr);
				}
				else
				{
					pConnect->Close();
					WarnSprintf("failed to connect to %s, error is %s", pConnect->GetPeerAddress().c_str(), ec.message().c_str());
				}
			}
		};

		pConnect->GetSocket()->async_connect(*itr, onConnect);
	}

	bool XmaxNetPluginImpl::_IsConnectd(const std::string& host)
	{
		for (const auto& pConnect : connections_)
		{
			if (pConnect->GetPeerAddress() == host)
			{
				return true;
			}
		}

		return false;
	}

	void XmaxNetPluginImpl::_ParseMsg(const char* pMsg, const MsgHeader& header, std::shared_ptr<XMX_Connection> pConnect)
	{
		//Test msg
		MsgId id = (MsgId)header.id;
		if (header.id == MSG_TEST)
		{
			HelloMsg msg;
			msg.ParseFromArray(pMsg, header.msgLength);
			_OnHandleMsg(pConnect, msg);
		}
	}

	void XmaxNetPluginImpl::_OnHandleMsg(std::shared_ptr<XMX_Connection> pConnect, const HelloMsg& msg)
	{
		LogSprintf("recv hellomsg from peer(%s), content is %s", pConnect->GetPeerAddress().c_str(), msg.msg().c_str());
	}

	void XmaxNetPluginImpl::_ConnectionTimer()
	{
		TimeMicroseconds time(2000000);
		connectionTimer_.expires_from_now(boost::posix_time::microseconds(time.GetValue()));
		connectionTimer_.async_wait(std::bind(&XmaxNetPluginImpl::_CheckConnection, this));
	}

	void XmaxNetPluginImpl::_CheckConnection()
	{
		for (auto pc : connections_)
		{
			if (pc->GetConStatus() == CS_DISCONNECTED || !pc->GetSocket()->is_open())
			{
				if (pc->IsInitiative())
				{
					ConnectImpl(pc->GetPeerAddress());
				}
				
			}
			
		}

		_ConnectionTimer();
	}

	void XmaxNetPluginImpl::_Disconnect(std::shared_ptr<XMX_Connection> pc)
	{
		for (auto itr = connections_.begin(); itr != connections_.end(); ++itr)
		{
			if (*itr == pc)
			{
				pc->Close();
				connections_.erase(itr);
				break;
			}
		}
	}

	void XmaxNetPluginImpl::_DetectExternalAddr()
	{
		std::vector<std::string> publicAddrList = GetExternalAddress();
		auto listenAddr = address::from_string(localAddr_);
		bool bListenAddrSet = !listenAddr.is_unspecified();
		bool listenIsPublic = bListenAddrSet && IsPublicAddress(localAddr_);

		if (bUpnp_ && listenIsPublic)
		{
			LogSprintf("Listening Address is public : %s", localAddr_.c_str());
			externalAddr_ = localAddr_;
		}
		else if (bUpnp_)
		{
			_MapPort();
		}
		else
		{
			Logf("can not find external addresses, upnp not supported\n");
		}
	}

	void XmaxNetPluginImpl::_MapPort()
	{
#ifdef USE_UPNP 
		const char * multicastif = 0;
		const char * minissdpdpath = 0;
		struct UPNPDev * devlist = 0;
		struct UPNPDev* dev;
		int error = 0;
		char lanaddr[64];

		devlist = upnpDiscover(2000, multicastif, minissdpdpath, 0, 0, 2, &error);
		struct UPNPUrls urls;
		struct IGDdatas data;
		int r;

		char* descXML;
		int descXMLsize = 0;
		int upnperror = 0;

		r = UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr));

		if (r == 1)
		{
			char externalIPAddress[40];
			r = UPNP_GetExternalIPAddress(urls.controlURL, data.first.servicetype, externalIPAddress);
			if (r != UPNPCOMMAND_SUCCESS)
			{
				LogSprintf("UPnP: GetExternalIPAddress() returned %d\n", r);
			}
			else
			{
				if (externalIPAddress[0])
				{
					externalAddr_ = lanaddr;
				}
				else
				{
					LogSprintf("UPnP: GetExternalIPAddress failed.\n");
				}
			}

			r = UPNP_AddPortMapping(urls.controlURL, data.first.servicetype,
				listeningPort_.c_str(), listeningPort_.c_str(), lanaddr, "xmax", "TCP", 0, "0");

			if (r != UPNPCOMMAND_SUCCESS)
			{
				LogSprintf("AddPortMapping(%s, %s, %s) failed with code %d (%s)\n",
					listeningPort_, listeningPort_, lanaddr, r, strupnperror(r));
			}					
			else
			{
				LogSprintf("UPnP Port Mapping successful.\n");
			}
				
		}
		else if (r == 2)
		{
			Logf("A valid IGD has been found but it reported as not connected");
		}
		else if (r == 3)
		{
			Logf("an UPnP device has been found but was not recognized as an IGD");
		}
#endif
	}

	/**
	*  Implementations of XmaxNetPlugin interfaces
	*
	*/
	//--------------------------------------------------
	XmaxNetPlugin::XmaxNetPlugin() {	

	}

	//--------------------------------------------------
	XmaxNetPlugin::~XmaxNetPlugin() {	

	}

	//--------------------------------------------------
	void XmaxNetPlugin::Initialize(const xmaxapp::VarsMap& options) {
		
		PluginFace::Initialize(options);

		impl_.reset(new XmaxNetPluginImpl(*GetApp()->GetService()));
		impl_->Init(options);
	}

	//--------------------------------------------------
	void XmaxNetPlugin::Startup() 
	{	
		PluginFace::Startup();
		impl_->StartupImpl();
	}


	//--------------------------------------------------
	void XmaxNetPlugin::Shutdown() 
	{	
		impl_.reset();


		PluginFace::Shutdown();
	}

	void XmaxNetPlugin::InitOptions(OptionsDesc& cli, OptionsDesc& cfg)
	{
		cfg.add_options()
			(s_ServerAddress, xmaxapp::options::value<std::string>()->default_value("127.0.0.1:8080"), "default server address(ip:port)");

		cfg.add_options()
			(s_PeerAddress, xmaxapp::options::value<std::vector<std::string>>()->composing(), "list of peer address");

		cfg.add_options()
			(s_UPNP, xmaxapp::options::value<bool>()->default_value(false), "turn on/off upnp\n");
	}

}
