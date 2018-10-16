#pragma once

#include "pro/utils/singleton.hpp"

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
		MSG_VER,
		MSG_VERACK,
		MSG_ADDR,
		MSG_GETADDR,
	};

	struct MsgHeader
	{
		unsigned char	id;
		unsigned int	version;
		unsigned int	msgLength;

		MsgHeader()
			: id(0),
			  msgLength(0),
			  version(0)
		{

		}
	};

	class NetMessage
	{
public:
		NetMessage(const google::protobuf::Message& proMsg);

		char*	ToBinary() const;
		size_t	TotalLength() const;
	
		MsgHeader header_;
		const google::protobuf::Message& msg_;
	};


}