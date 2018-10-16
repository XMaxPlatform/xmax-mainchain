#include "netmessage.hpp"
#include "netmessage.pb.h"
#include <pro/log/log.hpp>

namespace xmax
{
NetMessage::NetMessage(const google::protobuf::Message& proMsg)
	: msg_(proMsg)
{

}

char* NetMessage::ToBinary() const
{
	size_t header_sz = sizeof(MsgHeader);
	size_t msg_sz    = msg_.ByteSize();
	size_t data_bytes  = header_sz + msg_sz;

	char* data = new char[data_bytes];
	memcpy(data, &header_, header_sz);

	bool ret = msg_.SerializeToArray(data + header_sz, msg_sz);

	if (!ret)
	{
		delete[] data;
		ErrorSprintf("serialize msg error, id is: %d", header_.id);
		return nullptr;
	}
	else
	{
		return data;
	}

}

size_t NetMessage::TotalLength() const
{
	size_t nHeader = sizeof(MsgHeader);
	size_t nMsg = msg_.ByteSize();
	size_t nBytes = nHeader + nMsg;
	return nBytes;
}

}