#pragma once

#include "pro/utils/singleton.hpp"
#include "netmessage.hpp"
#include "netmessage.pb.h"

namespace xmax
{
	class XMX_Connection;
	using namespace google::protobuf;

	class NetMsgProcessing : public pro::Singleton<NetMsgProcessing>
{
	public:

		void	ParseMsg(const char* pMsg, const MsgHeader& header, std::shared_ptr<XMX_Connection> pConnect);

	protected:

		void _OnHandleMsg(std::shared_ptr<XMX_Connection> pConnect, const HelloMsg& msg);
		void _OnHandleMsg(std::shared_ptr<XMX_Connection> pConnect, const VersionMsg& msg);
		void _OnHandleMsg(std::shared_ptr<XMX_Connection> pConnect, const VerAckMsg& msg);
		void _OnHandleMsg(std::shared_ptr<XMX_Connection> pConnect, const AddrMsg& msg);
		void _OnHandleMsg(std::shared_ptr<XMX_Connection> pConnect, const GetAddrMsg& msg);

	private:
};


}