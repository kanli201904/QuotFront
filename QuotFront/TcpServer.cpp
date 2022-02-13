#include "TcpServer.h"
#include "log.h"

CTcpServer::CTcpServer(asio::io_context& io_context,
		const tcp::endpoint& endpoint,
	   CChannelManager& chaManager,int iMaxConnect)
		: acceptor_(io_context, endpoint), m_channelManager(chaManager), m_MaxConnect(iMaxConnect)
	{		
		do_accept();
	}

void CTcpServer::do_accept()
{
	acceptor_.async_accept(
		[this](std::error_code ec, tcp::socket socket)
	{
		if (g_CurrentConnect >= m_MaxConnect)
		{
			g_logger->warn("Exceded the max connect num");
		}
		else
		{
			if (!ec)
			{
				std::make_shared<CChannelSession>(std::move(socket), m_channelManager)->start();
			}
		}

		do_accept();
	});
}