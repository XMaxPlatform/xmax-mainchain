#include "xmx_connection.hpp"
#include "netmessage_pool.hpp"

namespace xmax
{
XMX_Connection::XMX_Connection(const std::string& endpoint, const std::shared_ptr<tcp::socket>& s)
	: peerAddr_(endpoint),
	  socket_(s),
	  conStatus_(CS_DISCONNECTED)
{
	
}

XMX_Connection::XMX_Connection(const std::shared_ptr<tcp::socket>& s)
	: socket_(s),
	  conStatus_(CS_DISCONNECTED)
{

}

bool XMX_Connection::Connected()
{
	return false;
}

bool XMX_Connection::Current()
{
	return false;
}

void XMX_Connection::Reset()
{

}

void XMX_Connection::Close()
{
	conStatus_ = CS_DISCONNECTED;
	if (socket_ != nullptr)
	{
		socket_->close();
	}
}

void XMX_Connection::SendHandShake()
{

}

void XMX_Connection::SendSignedBlock()
{

}

void XMX_Connection::SendSignedBlockList()
{

}

}