#pragma once

namespace google
{
	namespace protobuf
	{
		class Message;
	}
}

namespace xmax
{
	enum MsgId
	{
		MSG_TEST = 0,
	};

	struct MsgHeader
	{
		unsigned char	id;
		unsigned int	msgLength;

		MsgHeader()
			: id(0),
			  msgLength(0)
		{

		}
	};

	struct NetMessage
	{
		MsgHeader header;
		google::protobuf::Message *pMsg;
	};
}