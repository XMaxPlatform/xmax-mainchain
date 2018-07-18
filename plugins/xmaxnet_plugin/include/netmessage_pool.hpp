#pragma once

#include <boost/pool/object_pool.hpp>
#include <boost/asio/buffer.hpp>
#include <deque>

namespace xmax
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

	class MessagePoolBuffer
{
public:

	MessagePoolBuffer();

	~MessagePoolBuffer();

public:

	std::vector<boost::asio::mutable_buffer> GetAvailableBufferFromPool();

	uint32_t	AvailableBytes() const;
	uint32_t	CanReadBytes() const;
	void		IncrementWriteIndex(uint32_t bytes);
	void		IncrementReadIndex(uint32_t bytes);
	bool		TryGetData(void* pData, uint32_t nBytes, bufferIndex readPtr);
	
	bufferIndex GetReadIndex() const;
	void        Allocate(uint32_t nBytes);

protected:

	uint32_t	_TotalBytes() const;
	void		_IncrementIndexImpl(bufferIndex& bi, uint32_t bytes);

private:

	const static uint32_t								bufferSize_ = 1024 * 1024;
	boost::object_pool<std::array<char, bufferSize_> >	objectPool_;
	bufferIndex											writeIndex_;
	bufferIndex											readIndex_;

	std::deque< std::array<char, bufferSize_>* >		msgBuffers_;

};

inline uint32_t MessagePoolBuffer::AvailableBytes() const
{
	size_t bytes = _TotalBytes() - (writeIndex_.bufferId * bufferSize_ + writeIndex_.bufferPtr);
	return bytes;
}

inline uint32_t MessagePoolBuffer::_TotalBytes() const
{
	return msgBuffers_.size() * bufferSize_;
}

inline void MessagePoolBuffer::_IncrementIndexImpl(bufferIndex& bi, uint32_t bytes)
{
	bi.bufferId  += (bi.bufferPtr + bytes) / bufferSize_;
	bi.bufferPtr = (bi.bufferPtr + bytes) % bufferSize_;
}

inline uint32_t MessagePoolBuffer::CanReadBytes() const
{
	uint32_t nBytes = bufferSize_ - readIndex_.bufferPtr + writeIndex_.bufferPtr +
		(writeIndex_.bufferId - readIndex_.bufferId - 1) * bufferSize_;
	return nBytes;
}

inline bufferIndex MessagePoolBuffer::GetReadIndex() const
{
	return readIndex_;
}

}