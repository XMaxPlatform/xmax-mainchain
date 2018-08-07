#include "netmsg_processing.h"
#include "xmaxnet_pluginimpl.hpp"
#include "xmx_connection.hpp"
#include <pro/log/log.hpp>

namespace xmax
{

void NetMsgProcessing::ParseMsg(const char* pMsg, const MsgHeader& header, std::shared_ptr<XMX_Connection> pConnect)
{
	//Test msg
	MsgId id = (MsgId)header.id;
	if (header.id == MSG_TEST)
	{
		HelloMsg msg;
		msg.ParseFromArray(pMsg, header.msgLength);
		_OnHandleMsg(pConnect, msg);
	}
	else if (header.id == MSG_VER)
	{
		VersionMsg msg;
		msg.ParseFromArray(pMsg, header.msgLength);
		_OnHandleMsg(pConnect, msg);
	}
	else if (header.id == MSG_VERACK)
	{
		VerAckMsg msg;
		msg.ParseFromArray(pMsg, header.msgLength);
		_OnHandleMsg(pConnect, msg);
	}
}

void NetMsgProcessing::_OnHandleMsg(std::shared_ptr<XMX_Connection> pConnect, const HelloMsg& msg)
{
	LogSprintf("recv hellomsg from peer(%s), content is %s", pConnect->GetPeerAddress().c_str(), msg.msg().c_str());
}

void NetMsgProcessing::_OnHandleMsg(std::shared_ptr<XMX_Connection> pConnect, const VersionMsg& msg)
{
	LogSprintf("recv versionmsg from peer(%s)\n", pConnect->GetPeerAddress().c_str());


	pConnect->SendVerAckMsg();


	if (!pConnect->IsInBound())
	{
		pConnect->SendGetAddrMsg();		
	}
}

void NetMsgProcessing::_OnHandleMsg(std::shared_ptr<XMX_Connection> pConnect, const VerAckMsg& msg)
{
	LogSprintf("recv verackmsg from peer(%s)\n", pConnect->GetPeerAddress().c_str());


}

void NetMsgProcessing::_OnHandleMsg(std::shared_ptr<XMX_Connection> pConnect, const AddrMsg& msg)
{
	LogSprintf("recv addrmsg from peer(%s)\n", pConnect->GetPeerAddress().c_str());

	for (std::string addr : msg.addrlist())
	{
		pNetImpl_->ConnectImpl(addr);
	}

}

void NetMsgProcessing::_OnHandleMsg(std::shared_ptr<XMX_Connection> pConnect, const GetAddrMsg& msg)
{
	LogSprintf("recv getaddrmsg from peer(%s)\n", pConnect->GetPeerAddress().c_str());

	if (pConnect->IsInBound())
	{
		const auto& conList = pNetImpl_->GetAllConnections();
		for (auto con : conList)
		{
			if (con->IsInBound() && pConnect != con)
			{
				const std::string& peerAddr = pConnect->GetPeerAddress();
				pConnect->AddAddrToSend(peerAddr);
			}
		}
	}
}

}
