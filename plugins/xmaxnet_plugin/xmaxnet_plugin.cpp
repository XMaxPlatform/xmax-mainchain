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

using namespace google::protobuf;

namespace xmax {
	using namespace xmaxapp;
	using namespace google::protobuf;
	using boost::asio::ip::tcp;
	using boost::asio::ip::address_v4;
	using boost::asio::ip::host_name;

	const char* s_ServerAddress = "server-address";
	const char* s_PeerAddress   = "peer-address";

	/**
	*  Implementation details of the xmax net plugin
	*
	*/
	class XmaxNetPluginImpl {	
	public:
		XmaxNetPluginImpl();
		~XmaxNetPluginImpl();

	public:
		/**
		* Init p2p newwork params
		*/
		void Init(boost::asio::io_service& io, const VarsMap& options);

		void StartUpImpl();

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


	private:

		std::unique_ptr<tcp::acceptor>							acceptor_;
		std::unique_ptr<tcp::resolver>							resolver_;
		tcp::endpoint											endpoint_;
		std::vector<std::shared_ptr<XMX_Connection>>			connections_;
		uint													nMaxClients_;
		uint													nCurrClients_;
		std::string												seedServer_;
		std::vector<std::string>								peerAddressList_;

		boost::asio::io_service*								pIoService_;
	};

	/**
	*  Implementations of XmaxNetPluginImpl interfaces
	*
	*/

	XmaxNetPluginImpl::XmaxNetPluginImpl()
		: nMaxClients_(0),
		  nCurrClients_(0)
	{

	}


	XmaxNetPluginImpl::~XmaxNetPluginImpl()
	{
		ShutdownProtobufLibrary();
	}
	/**
	*  Initialize network
	*
	*/
	void XmaxNetPluginImpl::Init(boost::asio::io_service& io, const VarsMap& options)
	{
		pIoService_ = &io;

		resolver_ = std::make_unique<tcp::resolver>( io );
		acceptor_.reset( new tcp::acceptor(io) );

		if (options.count(s_ServerAddress))
		{
			seedServer_		 = options.at(s_ServerAddress).as<std::string>();
			size_t spos		 = seedServer_.find(':');
			std::string addr = seedServer_.substr(0, spos);
			std::string port = seedServer_.substr(spos + 1, seedServer_.size());

			tcp::resolver::query query(tcp::v4(), addr.c_str(), port.c_str());
			endpoint_ = *resolver_->resolve(query);
			acceptor_.reset(new tcp::acceptor(io));
		}

		if (options.count(s_PeerAddress))
		{
			peerAddressList_ = options.at(s_PeerAddress).as<std::vector<std::string>>();
		}
	}

	void XmaxNetPluginImpl::StartUpImpl()
	{
		if (acceptor_ != nullptr)
		{
			acceptor_->open(endpoint_.protocol());
			acceptor_->set_option(tcp::acceptor::reuse_address(true));
			acceptor_->bind(endpoint_);
			acceptor_->listen();
			StartListen();
			Logf("start to listen incoming peers");
		}

		for (const std::string& peer : peerAddressList_)
		{
			ConnectImpl(peer);
		}
	}

	void XmaxNetPluginImpl::StartListen()
	{
		std::shared_ptr<tcp::socket> pSocket = std::make_shared<tcp::socket>(*pIoService_);
		
		auto onAccept = [pSocket, this](boost::system::error_code ec)
		{
			if ( !ec )
			{
				if (nCurrClients_ < nMaxClients_)
				{
					std::shared_ptr<XMX_Connection> pConnect = std::make_shared<XMX_Connection>(pSocket);
					connections_.push_back(pConnect);
					tcp::no_delay nd(true);
					pConnect->GetSocket()->set_option(nd);
					StartRecvMsg(pConnect);
					nMaxClients_++;
				}
				else
				{
					Warnf("MaxClinet exceeded!!!!\n");
					pSocket->close();
				}

				StartListen();
			}
			else
			{
				ErrorSprintf("accept error: %s", ec.message());
			}
		};
		
		acceptor_->async_accept(*pSocket, onAccept);
	}

	void XmaxNetPluginImpl::StartRecvMsg(std::shared_ptr<XMX_Connection> pConnect)
	{
		if (pConnect->GetSocket() == nullptr)
		{
			return;
		}

		try
		{
			MessagePoolBuffer* pMsgPoolBuf = pConnect->GetMsgBuffer();
			auto onReadFunc = [&](boost::system::error_code ec, std::size_t bytesRead)
			{
				if (ec)
				{
					ErrorSprintf("read msg from %s error : %s", pConnect->GetPeerAddress().c_str(), ec.message().c_str());
					return;
				}
				else
				{
					size_t nCanWrite = pMsgPoolBuf->AvailableBytes();
					if (bytesRead > nCanWrite)
					{
						ErrorSprintf("read msg bytes exceeded msg buffer size\n");
						return;
					}

					pMsgPoolBuf->IncrementWriteIndex(bytesRead);
					uint32_t nCanReadBytes = pMsgPoolBuf->CanReadBytes();
					while (nCanReadBytes > 0)
					{
						if (nCanReadBytes < sizeof(MsgHeader)  )
						{
							break;
						}

						MsgHeader msgHeader;
						bufferIndex readIndex = pMsgPoolBuf->GetReadIndex();
						bool bGet             = pMsgPoolBuf->TryGetData(&msgHeader, sizeof(MsgHeader), readIndex);
						if (!bGet)
						{
							break;
						}

						uint32_t msgSize = msgHeader.msgLength + sizeof(MsgHeader);
						if (nCanReadBytes >= msgSize)
						{
							pMsgPoolBuf->IncrementReadIndex(sizeof(MsgHeader));
							char* pMsgData = new char[msgHeader.msgLength];
							bool bret = pMsgPoolBuf->GetData(pMsgData, msgHeader.msgLength);

							if (!bret)
							{
								delete[] pMsgData;
								break;
							}	

							_ParseMsg(pMsgData, msgHeader, pConnect);
							pMsgPoolBuf->IncrementReadIndex(msgHeader.msgLength);
							delete[] pMsgData;
						}
						else
						{
							uint32_t remainMsgLength = msgSize - nCanReadBytes;
							uint32_t canWriteLength  = pMsgPoolBuf->AvailableBytes();

							if (remainMsgLength > canWriteLength)
							{
								pMsgPoolBuf->Allocate(remainMsgLength - canWriteLength);
							}

							break;
						}
					}

					StartRecvMsg(pConnect);
				}
			};

			pConnect->GetSocket()->async_read_some(pMsgPoolBuf->GetAvailableBufferFromPool(), onReadFunc);


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

		std::shared_ptr<tcp::socket> s = std::make_shared<tcp::socket>(*pIoService_);

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
		auto onResolve = [&](const boost::system::error_code& ec, tcp::resolver::iterator itr)
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

		auto onConnect = [&](const boost::system::error_code& ec)
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
					currItr++;
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

		impl_.reset(new XmaxNetPluginImpl());
		impl_->Init(*GetApp()->GetService(), options);
	}

	//--------------------------------------------------
	void XmaxNetPlugin::Startup() 
	{	
		PluginFace::Startup();
		impl_->StartUpImpl();
	}


	//--------------------------------------------------
	void XmaxNetPlugin::Shutdown() 
	{	
		impl_.reset();


		PluginFace::Shutdown();
	}



}
