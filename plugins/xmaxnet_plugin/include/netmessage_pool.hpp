#pragma once

#include "pro/utils/singleton.hpp"
#include <boost/pool/object_pool.hpp>
#include <boost/asio/buffer.hpp>
#include <deque>

namespace xmx
{
	struct bufferIndex
	{
		uint32_t bufferId;
		uint32_t bufferPtr;

		bufferIndex()
			: bufferId(0),
			  bufferPtr(0)
		{}
	};

	class MessagePoolBuffer : public pro::Singleton<MessagePoolBuffer>
{
public:

	MessagePoolBuffer();

	~MessagePoolBuffer();

public:

	std::vector<boost::asio::mutable_buffer> GetAvailableBufferFromPool();

protected:
private:

	const static uint32_t								bufferSize_ = 1024 * 1024;
	boost::object_pool<std::array<char, bufferSize_> >	objectPool_;
	bufferIndex											writeIndex_;
	bufferIndex											readIndex_;

	std::deque< std::array<char, bufferSize_>* >		msgBuffers_;

};

}