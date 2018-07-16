#pragma once

#include <memory>
#include <string>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/host_name.hpp>

namespace xmax
{
	using boost::asio::ip::tcp;
	using boost::asio::ip::address_v4;
	using boost::asio::ip::host_name;


	class XMX_Connection : public std::enable_shared_from_this<XMX_Connection>
{
public:

	 XMX_Connection(const std::string& endpoint);

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

protected:

private:

	std::shared_ptr<tcp::socket>		socket_;
	std::string							peerAddr_;
};

inline std::shared_ptr<tcp::socket> XMX_Connection::GetSocket() const
{
	return socket_;
}

inline const std::string& XMX_Connection::GetPeerAddress() const
{
	return peerAddr_;
}

}
