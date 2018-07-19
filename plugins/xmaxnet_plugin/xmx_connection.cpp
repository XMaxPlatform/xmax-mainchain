#include "xmx_connection.hpp"
#include "netmessage_pool.hpp"

#include <boost/asio/write.hpp>
#include <pro/log/log.hpp>

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

void XMX_Connection::PushMsg(const NetMessage& msg)
{
	char* pData = msg.ToBinary();

	if (pData != nullptr)
	{
		std::pair<char*, size_t> msgInfo;
		msgInfo.first = pData;
		msgInfo.second = msg.TotalLength();
		messeageQueue_.push(msgInfo);
	}
	else
	{
		return;
	}

	
	if (messeageQueue_.size() == 1)
	{
		_AsyncSend();
	}

}

void XMX_Connection::_AsyncSend()
{
	if (messeageQueue_.empty())
	{
		return;
	}

	auto onWriteFinish = [&](boost::system::error_code ec, size_t bytes_transferred)
	{
		if (!ec)
		{
			const std::pair<char*, size_t>& msgInfo = messeageQueue_.front();
			delete[] msgInfo.first;
			messeageQueue_.pop();
			_AsyncSend();
		}
		else
		{
			ErrorSprintf("async_wrtie to peer(%s) error : %s", peerAddr_.c_str(), ec.message());
		}
	};

	const std::pair<char*, size_t>& toSendMsg = messeageQueue_.front();
	boost::asio::async_write(*socket_, boost::asio::buffer(toSendMsg.first, toSendMsg.second), onWriteFinish);
}


}