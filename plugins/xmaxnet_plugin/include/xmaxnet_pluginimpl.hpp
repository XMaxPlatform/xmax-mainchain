#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/host_name.hpp>
#include <boost/asio/steady_timer.hpp>

#include "app_types.hpp"
#include "pro/types/generictypes.hpp"

#include <memory>

namespace xmax
{
	using boost::asio::ip::tcp;
	using xmaxapp::OptionsDesc;
	using xmaxapp::VarsMap;
	using namespace pro;
	class XMX_Connection;

	class XmaxNetPluginImpl
{
public:

	XmaxNetPluginImpl(const boost::asio::io_service& io);

	~XmaxNetPluginImpl();
public:
	/**
	* Init p2p network params
	*/
	void Init(const VarsMap& options);
	/**
	* start p2p network
	*/
	void StartupImpl();
	/**
	* delay start
	*/
	void Startup();

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

	const std::vector<std::shared_ptr<XMX_Connection> >& GetAllConnections() const;

	bool HasAddress(const std::string& addr) const;

	//send addr to peers
	void BroadCastAddr(const std::string& addr);

	void StartPendingTimer();

public:

	static const char* s_ServerAddress;
	static const char* s_PeerAddress;
	static const char* s_UPNP;

protected:

	bool _IsConnectd(const std::string& host);

	void _ConnectionTimer();

	void _CheckConnection();

	void _SendAddrsTimer();

	void _SendAddrs();

	void _Disconnect(std::shared_ptr<XMX_Connection> pc);

	void _DetectExternalAddr();

	void _MapPort();

private:

	std::unique_ptr<tcp::acceptor>							acceptor_;
	std::unique_ptr<tcp::resolver>							resolver_;
	tcp::endpoint											endpoint_;
	std::vector<std::shared_ptr<XMX_Connection>>			connections_;
	uint32													nMaxClients_;
	uint32													nCurrClients_;
	std::string												seedServer_;
	std::vector<std::string>								peerAddressList_;
	std::string												listeningPort_;
	std::string												localAddr_;
	std::string												externalAddr_;
	bool													bUpnp_;

	const boost::asio::io_service&							ioService_;
	boost::asio::deadline_timer								connectionTimer_;
	boost::asio::deadline_timer								sendAddrsTimer_;
	boost::asio::deadline_timer								delayNetStartTimer_;
	std::unique_ptr<boost::asio::steady_timer>				pendingBlocksTimer_;
	boost::asio::steady_timer::duration						sendPengdingBlocksPeriod_;
};


inline const std::vector<std::shared_ptr<XMX_Connection> >& XmaxNetPluginImpl::GetAllConnections() const
{
	return connections_;
}

}