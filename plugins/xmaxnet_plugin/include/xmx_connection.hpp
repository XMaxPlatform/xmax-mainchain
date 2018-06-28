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

protected:
private:

	std::shared_ptr<tcp::socket>		socket_;
};

}
