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
	size_t nHeader = sizeof(MsgHeader);
	size_t nMsg    = pMsg->ByteSize();
	size_t nBytes  = nHeader + nMsg;

	char* pData = new char[nBytes];
	memcpy(pData, &header, nHeader);

	bool ret = pMsg->SerializeToArray(pData + nHeader, nMsg);

	if (!ret)
	{
		delete[] pData;
		ErrorSprintf("serialize msg error, id is: %d", header.id);
		return nullptr;
	}
	else
	{
		return pData;
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