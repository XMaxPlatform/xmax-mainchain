#pragma once

namespace google
{
	namespace protobuf
	{
		class Message;
	}
}

namespace xmx
{
	enum MsgId
	{
		MSG_TEST = 0,
	};

	struct MsgHeader
	{
		unsigned char	id;
		unsigned int	msgLength;
	};

	struct NetMessage
	{
		MsgHeader header;
		google::protobuf::Message *pMsg;
	};
}