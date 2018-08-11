#pragma once

#include <memory>
#include <string>
#include <queue>
#include <set>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/host_name.hpp>

#include "netmessage.hpp"
#include "pro/utils/bloomfilter.hpp"

namespace xmax
{
	using boost::asio::ip::tcp;
	using boost::asio::ip::address_v4;
	using boost::asio::ip::host_name;
	using namespace pro;

	enum ConnectionStatus
	{
		CS_DISCONNECTED = 0,
		CS_CONNECTING,
		CS_CONNECTED,
	};

	class MessagePoolBuffer;

	class XMX_Connection : public std::enable_shared_from_this<XMX_Connection>
{
public:

	 XMX_Connection(const std::string& endpoint, const std::shared_ptr<tcp::socket>& s, const bloom_parameters& param);

	 XMX_Connection(const std::shared_ptr<tcp::socket>& s, const bloom_parameters& param);

	 ~XMX_Connection();

	 void Close();

	 std::shared_ptr<tcp::socket> GetSocket() const;

	 const std::string& GetPeerAddress() const;

	 void	SetConStatus(ConnectionStatus cs);
	 ConnectionStatus GetConStatus() const;

	 /* Inbound means recv a connection from a peer,
	  * Outbound means connecting to a peer
	 */
	 void   SetInBound(bool b);
	 bool   IsInBound() const;

	 MessagePoolBuffer*		GetMsgBuffer() const;

	 //send msg to peer
	 void PushMsg(const NetMessage& msg);

	 void AddAddrToSend(const std::string& addr);
	 const std::set<std::string>& GetAddrToSendList() const;
	 void SendAddrsToPeer();

	 void SendVersionMsg();
	 void SendVerAckMsg();
	 void SendGetAddrMsg();

protected:

	void _AsyncSend();

private:

	std::shared_ptr<tcp::socket>				socket_;
	mutable std::string							peerAddr_;
	ConnectionStatus							conStatus_;

	MessagePoolBuffer*							pMsgBuffer_;
	std::queue< std::pair<char*, size_t> >		messeageQueue_;
	std::set<std::string>						addrToSendList_;
	bool										bInBound_;

	bloom_filter								bloomFilter_;
};

inline std::shared_ptr<tcp::socket> XMX_Connection::GetSocket() const
{
	return socket_;
}

inline const std::string& XMX_Connection::GetPeerAddress() const
{
	if (peerAddr_.empty())
	{
		peerAddr_ = socket_->remote_endpoint().address().to_string() + ":" +
			std::to_string(socket_->remote_endpoint().port());
	}
	return peerAddr_;
}

inline void XMX_Connection::SetConStatus(ConnectionStatus cs)
{
	conStatus_ = cs;
}

inline ConnectionStatus XMX_Connection::GetConStatus() const
{
	return conStatus_;
}

inline MessagePoolBuffer* XMX_Connection::GetMsgBuffer() const
{
	return pMsgBuffer_;
}

inline void XMX_Connection::SetInBound(bool b)
{
	bInBound_ = b;
}

inline bool XMX_Connection::IsInBound() const
{
	return bInBound_;
}

inline void XMX_Connection::AddAddrToSend(const std::string& addr)
{
	if (!bloomFilter_.contains(addr))
	{
		bloomFilter_.insert(addr);
		addrToSendList_.insert(addr);
	}
}

inline const std::set<std::string>& XMX_Connection::GetAddrToSendList() const
{
	return addrToSendList_;
}

}
