#pragma once

#include "pro/utils/singleton.hpp"
#include "netmessage.hpp"
#include "netmessage.pb.h"

namespace xmax
{
	class XMX_Connection;
	class XmaxNetPluginImpl;
	using namespace google::protobuf;
	// process all  net messages
	class NetMsgProcessing : public pro::Singleton<NetMsgProcessing>
{
	public:

		void	ParseMsg(const char* pMsg, const MsgHeader& header, std::shared_ptr<XMX_Connection> pConnect);
		void    SetNetImpl(const std::shared_ptr<XmaxNetPluginImpl>& impl);
	protected:
		// test msg
		void _OnHandleMsg(std::shared_ptr<XMX_Connection> pConnect, const HelloMsg& msg);
		// version msg
		void _OnHandleMsg(std::shared_ptr<XMX_Connection> pConnect, const VersionMsg& msg);
		// version ack msg
		void _OnHandleMsg(std::shared_ptr<XMX_Connection> pConnect, const VerAckMsg& msg);
		//send addr to peer msg
		void _OnHandleMsg(std::shared_ptr<XMX_Connection> pConnect, const AddrMsg& msg);
		// request addr from peer msg
		void _OnHandleMsg(std::shared_ptr<XMX_Connection> pConnect, const GetAddrMsg& msg);

	private:

		std::shared_ptr<XmaxNetPluginImpl>	pNetImpl_;
};

	inline void NetMsgProcessing::SetNetImpl(const std::shared_ptr<XmaxNetPluginImpl>& impl)
	{
		pNetImpl_ = impl;
	}

}