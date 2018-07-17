#include "netmessage_pool.h"

namespace xmx
{
MessagePoolBuffer::MessagePoolBuffer()
{

}

MessagePoolBuffer::~MessagePoolBuffer()
{

}

std::vector<boost::asio::mutable_buffer> MessagePoolBuffer::GetAvailableBufferFromPool()
{
	//construct current available byte in the buffers
	std::vector<boost::asio::mutable_buffer> mbuffers;
	size_t currBufferId       = writeIndex_.bufferId;
	size_t currRestBufferSize = bufferSize_ - writeIndex_.bufferPtr;

	auto currBuffer = boost::asio::buffer(&msgBuffers_[currBufferId]->at(writeIndex_.bufferPtr),
						currRestBufferSize);

	mbuffers.push_back(currBuffer);

	//push rest buffers into
	for (size_t i = currBufferId + 1; i < msgBuffers_.size(); ++i)
	{
		auto mbuf = boost::asio::buffer(&msgBuffers_[i]->begin(), bufferSize_);
		mbuffers.push_back(mbuf);
	}

	return mbuffers;
}

}