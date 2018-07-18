#include "netmessage_pool.hpp"

namespace xmax
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

void MessagePoolBuffer::IncrementWriteIndex(uint32_t bytes)
{
	_IncrementIndexImpl(writeIndex_, bytes);

	size_t nBuf = writeIndex_.bufferId - msgBuffers_.size();
	if (nBuf > 0)
	{
		for (size_t i = 0; i < nBuf; ++i)
		{
			auto pBuf = objectPool_.malloc();
			msgBuffers_.push_back(pBuf);
		}
	}
}

bool MessagePoolBuffer::TryGetData(void* pData, uint32_t nBytes, bufferIndex readPtr)
{
	bool ret = false;
	if (CanReadBytes() >= nBytes)
	{
		char* pBuffer = &msgBuffers_[readPtr.bufferId]->at(readPtr.bufferPtr);
		if (readPtr.bufferPtr + nBytes <= bufferSize_)
		{
			memcpy(pData, pBuffer, nBytes);
			ret = true;
		}
		else
		{
			uint32_t remainBytes = bufferSize_ - readPtr.bufferPtr;
			memcpy(pData, pBuffer, remainBytes);
			_IncrementIndexImpl(readPtr, nBytes);
			ret |= TryGetData((char*)pData + remainBytes, nBytes - remainBytes, readPtr);

		}
		return ret;
	}
	else
	{
		return false;
	}
}

void MessagePoolBuffer::Allocate(uint32_t nBytes)
{
	uint32_t nBuffers = nBytes / bufferSize_ + 1;
	for (size_t i = 0; i < nBuffers; ++i)
	{
		auto buffer = objectPool_.malloc();
		msgBuffers_.push_back(buffer);
	}
}

void MessagePoolBuffer::IncrementReadIndex(uint32_t bytes)
{
	_IncrementIndexImpl(readIndex_, bytes);
	
	// delete previous buffers
	size_t nDelete = 0;
	for (size_t i = 0; i < readIndex_.bufferId; ++i)
	{
		objectPool_.destroy(msgBuffers_.front());
		msgBuffers_.pop_front();
		nDelete++;
	}
	readIndex_.bufferId  -= nDelete;
	writeIndex_.bufferId -= nDelete;
}

}