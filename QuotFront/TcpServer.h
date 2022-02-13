#pragma once
#include "ChannelManager.h"
#include "ChannelSession.h"
#include "log.h"
class CTcpServer
{
public:
	CTcpServer(asio::io_context& io_context,const tcp::endpoint& endpoint, 
		       CChannelManager& chaManager, int iMaxConnect);

private:
	void do_accept();

	tcp::acceptor acceptor_;
	CChannelManager& m_channelManager;

	int m_MaxConnect;
};

extern int g_CurrentConnect;
extern LOGGER g_logger;