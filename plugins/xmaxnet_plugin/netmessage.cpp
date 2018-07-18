#include "netmessage.hpp"

#include <google/protobuf/stubs/common.h>
#include "../../network/protos_cpp/netmessage.pb.h"

using namespace google::protobuf;

namespace xmax
{
void NetMessasgeHandler::HandleMessage(const char* pMsg, const MsgHeader& header)
{
	MsgId id = (MsgId)header.id;
	if (header.id == MSG_TEST)
	{
		HelloMsg msg;
		msg.ParseFromArray(pMsg, header.msgLength);
	}
}


}