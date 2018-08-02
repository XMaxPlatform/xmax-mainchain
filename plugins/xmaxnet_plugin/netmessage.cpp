#include "netmessage.hpp"
#include "netmessage.pb.h"
#include <pro/log/log.hpp>

namespace xmax
{
NetMessage::NetMessage(const google::protobuf::Message& proMsg)
	: pMsg(&proMsg)
{

}

char* NetMessage::ToBinary() const
{
	size_t header_sz = sizeof(MsgHeader);
	size_t msg_sz    = pMsg->ByteSize();
	size_t data_bytes  = header_sz + msg_sz;

	char* data = new char[data_bytes];
	memcpy(data, &header, header_sz);

	bool ret = pMsg->SerializeToArray(data + header_sz, msg_sz);

	if (!ret)
	{
		delete[] data;
		ErrorSprintf("serialize msg error, id is: %d", header.id);
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
	size_t nMsg = pMsg->ByteSize();
	size_t nBytes = nHeader + nMsg;
	return nBytes;
}

}