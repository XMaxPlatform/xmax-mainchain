#include "xmx_connection.hpp"

namespace xmax
{
XMX_Connection::XMX_Connection(const std::string& endpoint)
{

}

XMX_Connection::XMX_Connection(const std::shared_ptr<tcp::socket>& s)
	: socket_(s)
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