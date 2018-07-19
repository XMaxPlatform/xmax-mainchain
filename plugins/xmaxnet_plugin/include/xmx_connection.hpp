#pragma once

#include <memory>
#include <string>
#include <queue>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/host_name.hpp>

#include "netmessage.hpp"

namespace xmax
{
	using boost::asio::ip::tcp;
	using boost::asio::ip::address_v4;
	using boost::asio::ip::host_name;

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

	 XMX_Connection(const std::string& endpoint, const std::shared_ptr<tcp::socket>& s);

	 XMX_Connection(const std::shared_ptr<tcp::socket>& s);
	 /**
	 * is network connectd and not connecting
	 */
	 bool Connected();
	 /**
	 * is network connectd and not syncing
	 */
	 bool Current();
	 /**
	 * reset state
	 */
	 void Reset();
	 /**
	 * close connection
	 */
	 void Close();
	 /**
	 * send handshake msg to sync states between peers
	 */
	 void SendHandShake();
	 /**
	 * send a block to sync the peer
	 */
	 void SendSignedBlock();
	 /**
	 * send blocks to sync the peer
	 */
	 void SendSignedBlockList();


	 std::shared_ptr<tcp::socket> GetSocket() const;

	 const std::string& GetPeerAddress() const;

	 void	SetConStatus(ConnectionStatus cs);
	 ConnectionStatus GetConStatus() const;

	 MessagePoolBuffer*		GetMsgBuffer() const;

	 void PushMsg(const NetMessage& msg);

protected:

	void _AsyncSend();

private:

	std::shared_ptr<tcp::socket>				socket_;
	std::string									peerAddr_;
	ConnectionStatus							conStatus_;

	MessagePoolBuffer*							pMsgBuffer_;
	std::queue< std::pair<char*, size_t> >		messeageQueue_;
};

inline std::shared_ptr<tcp::socket> XMX_Connection::GetSocket() const
{
	return socket_;
}

inline const std::string& XMX_Connection::GetPeerAddress() const
{
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

}
