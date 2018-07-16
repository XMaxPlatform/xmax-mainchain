#include <xmaxnet_plugin.hpp>
#include <google/protobuf/stubs/common.h>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/host_name.hpp>
#include <pro/log/log.hpp>
#include "xmx_connection.hpp"

#include <functional>

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
		/**
		* set endpoint (using boost params)
		* @param[in]	std::string		endpoint of network
		*/
		void SetEndpoint(const std::string& endpoint);

		void StartUpImpl();

		void ConnectImpl(const std::string& host);

		void Connect(std::shared_ptr<XMX_Connection> pConnect, tcp::resolver::iterator endpointItr);
		
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
			Logf("start to listen incoming peers");
		}
	}

	void XmaxNetPluginImpl::SetEndpoint(const std::string& endpoint)
	{
		
		
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
				}
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

	}

	void XmaxNetPluginImpl::ConnectImpl(const std::string& host)
	{
		if (_IsConnectd(host))
		{
			Warnf("duplicated connection\n");
			return;
		}

		std::shared_ptr<XMX_Connection> pConnect = std::make_shared<XMX_Connection>(host);
		connections_.push_back(pConnect);
	}

	void XmaxNetPluginImpl::Connect(std::shared_ptr<XMX_Connection> pConnect, tcp::resolver::iterator endpointItr)
	{

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
