#include "netmessage_pool.hpp"

namespace xmax
{
MessagePoolBuffer::MessagePoolBuffer()
{
	msg_buffers_.push_back(objectPool_.malloc());
}

MessagePoolBuffer::~MessagePoolBuffer()
{
	for (size_t i = 0; i < msg_buffers_.size(); ++i)
	{
		objectPool_.destroy(msg_buffers_[i]);
	}
	msg_buffers_.clear();
}

std::vector<boost::asio::mutable_buffer> MessagePoolBuffer::GetAvailableBufferFromPool()
{
	//construct current available byte in the buffers
	std::vector<boost::asio::mutable_buffer> mbuffers;
	size_t currBufferId       = writeIndex_.bufferId;
	size_t currRestBufferSize = bufferSize_ - writeIndex_.bufferPtr;

	auto currBuffer = boost::asio::buffer(&msg_buffers_[currBufferId]->at(writeIndex_.bufferPtr),
						currRestBufferSize);

	mbuffers.push_back(currBuffer);

	//push rest buffers into
	for (size_t i = currBufferId + 1; i < msg_buffers_.size(); ++i)
	{
		auto mbuf = boost::asio::buffer(&msg_buffers_[i]->begin(), bufferSize_);
		mbuffers.push_back(mbuf);
	}

	return mbuffers;
}

void MessagePoolBuffer::IncrementWriteIndex(uint32_t bytes)
{
	_IncrementIndexImpl(writeIndex_, bytes);

	int nBuf = writeIndex_.bufferId + 1 - msg_buffers_.size();
	if (nBuf > 0)
	{
		for (size_t i = 0; i < nBuf; ++i)
		{
			auto pBuf = objectPool_.malloc();
			msg_buffers_.push_back(pBuf);
		}
	}
}

bool MessagePoolBuffer::TryGetData(void* data, uint32_t bytes, bufferIndex read_ptr)
{
	bool ret = false;
	if (CanReadBytes() >= bytes)
	{
		char* pBuffer = &msg_buffers_[read_ptr.bufferId]->at(read_ptr.bufferPtr);
		if (read_ptr.bufferPtr + bytes <= bufferSize_)
		{
			memcpy(data, pBuffer, bytes);
			ret = true;
		}
		else
		{
			uint32_t remainBytes = bufferSize_ - read_ptr.bufferPtr;
			memcpy(data, pBuffer, remainBytes);
			_IncrementIndexImpl(read_ptr, bytes);
			ret |= TryGetData((char*)data + remainBytes, bytes - remainBytes, read_ptr);

		}
		return ret;
	}
	else
	{
		return false;
	}
}

bool MessagePoolBuffer::GetData(void* pData, uint32_t nBytes)
{
	bool ret = false;
	if (CanReadBytes() >= nBytes)
	{
		char* pBuffer = &msg_buffers_[readIndex_.bufferId]->at(readIndex_.bufferPtr);
		if (readIndex_.bufferPtr + nBytes <= bufferSize_)
		{
			memcpy(pData, pBuffer, nBytes);
			IncrementReadIndex(nBytes);
			ret = true;
		}
		else
		{
			uint32_t remainBytes = bufferSize_ - readIndex_.bufferPtr;
			memcpy(pData, pBuffer, remainBytes);
			IncrementReadIndex(remainBytes);
			ret |= GetData((char*)pData + remainBytes, nBytes - remainBytes);
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
		msg_buffers_.push_back(buffer);
	}
}

void MessagePoolBuffer::IncrementReadIndex(uint32_t bytes)
{
	_IncrementIndexImpl(readIndex_, bytes);
	
	// delete previous buffers
	size_t nDelete = 0;
	for (size_t i = 0; i < readIndex_.bufferId; ++i)
	{
		objectPool_.destroy(msg_buffers_.front());
		msg_buffers_.pop_front();
		nDelete++;
	}
	readIndex_.bufferId  -= nDelete;
	writeIndex_.bufferId -= nDelete;
}

}