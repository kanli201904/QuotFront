#include "ChannelSession.h"
#include "MayaCode.h"
#include <fmt/format.h>
#include "ConfigManager.h"

CChannelSession::CChannelSession(tcp::socket socket, CChannelManager& chaManager)
	: socket_(std::move(socket)),
	channelManager(chaManager), m_cirBuff(1000)/*,m_monitor_timer(g_tcp_io_context)*/
{			
	memset(m_szMsgPrex, 0, sizeof(m_szMsgPrex));
	auto r = fmt::format_to_n(m_szPeerAddr, sizeof(m_szPeerAddr) - 1,"{peerip}:{peerport}",
		fmt::arg("peerip", socket_.remote_endpoint().address().to_string()),
		fmt::arg("peerport", socket_.remote_endpoint().port()));
		
	if (r.size > 0)
		m_szPeerAddr[r.size] = 0;

	asio::socket_base::receive_buffer_size size_option(64 * 1024 );
	socket_.set_option(size_option);	

	write_in_progress = false;

    m_cirBuff.clear();
	m_cirbuffmaxsize = GET_CFG->get_value<int>("cirbuffmaxsize");
	m_sUserId = GET_CFG->get_value <std::string > ("loginuser");
}
//
//void CChannelSession::on_monitor_time()
//{
//	g_logger->warn("SendMsg2:PeerAddr={},m_cirBuff.size={}", m_szPeerAddr, m_cirBuff.size());
//	m_monitor_timer.expires_from_now(boost::posix_time::seconds(1));
//	m_monitor_timer.async_wait(boost::bind(&CChannelSession::on_monitor_time, shared_from_this()));
//}

void CChannelSession::start()
{
	channelManager.join(shared_from_this());
	
	g_logger->info("NewConnect:{}", m_szPeerAddr);

	do_read_header();
}

void CChannelSession::deliverNew(std::string sMsg)
{
	if (!socket_.is_open())
	{
		return;
	}

	snprintf(m_szMsgPrex, sizeof(m_szMsgPrex), "{(len=%d)", sMsg.length());
	m_sSendMsg = m_szMsgPrex;
	m_sSendMsg += sMsg.data();
	m_sSendMsg += "}";

	//bool write_in_progress = !m_cirBuff.empty();
	
	if (m_cirBuff.size() > m_cirbuffmaxsize)
	{
		g_logger->error("force close:PeerAddr={}", m_szPeerAddr);
		channelManager.leave(shared_from_this());
		CloseSession();
		return;
	}

	m_cirBuff.push_back(m_sSendMsg);
	if (!write_in_progress)
	{
		do_write();		
		//g_logger->debug("SendMsg:PeerAddr={},m_cirBuff.size={},msg={}", m_szPeerAddr, m_cirBuff.size(), sPtrMsg->data());
	}
	else
	{
		g_logger->warn("SendMsg2:PeerAddr={},m_cirBuff.size={}", m_szPeerAddr, m_cirBuff.size());
	}
	
	//m_monitor_timer.expires_from_now(boost::posix_time::seconds(5));
	//m_monitor_timer.async_wait(boost::bind(&CChannelSession::on_monitor_time, shared_from_this()));
}
void CChannelSession::deliver(/*std::shared_ptr<std::string> sPtrMsg*/char* szMsg)
{
	std::string sMsg(szMsg);	
	g_tcp_io_context.post(std::bind(&CChannelSession::deliverNew, shared_from_this(), sMsg));
	
}

void CChannelSession::do_read_header()
{
	if (!socket_.is_open())
	{
		return;
	}

	// need to do : 
	auto self(shared_from_this());
	asio::async_read(socket_,
		asio::buffer(read_msg_.data(), CMessage::header_length),
		[this, self](std::error_code  ec, std::size_t /*length*/)
	{
		if (!ec)
		{			
			// 解析长度
			m_sPackHead = read_msg_.data();
			
			int iMsgHeadPrefixLen = strlen(CLIENT_MSG_HEADER);
			
			if (m_sPackHead.find(CLIENT_MSG_HEADER) != string::npos)
			{
						
				int iPosBeforeMayaCode = -1;
				
				// 解析)符号
				for (int index = iMsgHeadPrefixLen; index < CMessage::header_length; index++)
				{
					if (m_sPackHead.at(index) == ')')
					{
						iPosBeforeMayaCode = index;
						break;
					}
				}
				if (iPosBeforeMayaCode != -1)
				{
					
					int iPackBodyLen = 0;
					m_sPackBodyLen = m_sPackHead.substr(iMsgHeadPrefixLen, (iPosBeforeMayaCode - iMsgHeadPrefixLen));
					iPackBodyLen = atoi(m_sPackBodyLen.c_str());

					read_msg_.iPackBodyLen = iPackBodyLen;
					read_msg_.iPackTotalLen_ = iMsgHeadPrefixLen + m_sPackBodyLen.length() + 1 + iPackBodyLen + 1;
					read_msg_.iPosBeforeMayaCode_ = iPosBeforeMayaCode;
					
					do_read_body();
				}
				else
				{
					// 此处已经串包，有很大的风险，因为socket里面可能还有待收数据
					g_logger->error("Parse PackHead failed:{}-{}", m_szPeerAddr, m_sPackHead);
					channelManager.leave(self);
					CloseSession();
				}				
			}
			else
			{				
				do_read_header();
			}
		}
		else
		{
			g_logger->error("do_read_header-Disconnect:PeerAddr={},Errcode={}",m_szPeerAddr,ec.value());
			channelManager.leave(self);
			CloseSession();
		}
	});
}

void CChannelSession::do_read_body()
{
	auto self(shared_from_this());
	asio::async_read(socket_,
		asio::buffer(read_msg_.body(), read_msg_.iPackTotalLen_ - CMessage::header_length),
		[this, self](std::error_code ec, std::size_t /*length*/)
	{
		if (!ec)
		{
			// 接收到完整的buf
			HandleClientMsg();
			do_read_header();
		}
		else
		{
			g_logger->error("do_read_body-Disconnect:PeerAddr={},Errcode={}", m_szPeerAddr, ec.value());
			channelManager.leave(self);
			CloseSession();
		}
	});
}

void CChannelSession::do_write()
{
	if (!socket_.is_open())
	{
		return;
	}

	auto self(shared_from_this());
	
	write_in_progress = true;

	asio::async_write(socket_,
		asio::buffer(m_cirBuff.front().data(),
			m_cirBuff.front().length()),
		[this, self](std::error_code ec, std::size_t length)
	{
		if (!ec)
		{						
			m_cirBuff.pop_front();
			if (!m_cirBuff.empty())
			{				
				do_write();				
			}
			else
			{								
				write_in_progress = false;
			}
		}
		else
		{		
			/*if (ec == boost::asio::error::operation_aborted)
				return;*/
			g_logger->error("do_write Disconnect:PeerAddr={},Errcode={}", m_szPeerAddr, ec.value());						
			channelManager.leave(self);
			CloseSession();
		}
	});	
}

void CChannelSession::CloseSession()
{
	if (socket_.is_open())
	{
		socket_.close();
	}
}

void CChannelSession::HandleClientMsg()
{	
	int iPosMayaCode = read_msg_.iPosBeforeMayaCode_ + 1;
	string sPackBody(read_msg_.data(), iPosMayaCode,read_msg_.iPackTotalLen_ - iPosMayaCode);
	int iPackBodyLen = sPackBody.length();

	g_logger->info("ClientMsg:{}-{}", m_szPeerAddr,sPackBody);
	
	int iPosAfterMayaCode = sPackBody.find("@");
	string sMayaCode = sPackBody.substr(0, iPosAfterMayaCode);

	
	if (sMayaCode == MayaCode::ReqPublicKey)
	{				
		m_sDeliverMsg = "REPLY_PUBLIC_KEY@y8RV+DZEIbKMdHUvXNyUNAhvmP87D2TZ1gNcdyCb9HlyCuhUByh2oyNqEWqDPA6teSmfcKs8SFkheybPdFQkjCdzDikFY+pCxosMBkZWEIA5FhKpQlaQl5g+GnG48+xMQ1vZ9fPefWpm0vQhPY2JaYXX8uelFlwM5M9nOOhypuc=@AQAB@A@@@@@@@0@0&";
		g_tcp_io_context.post(std::bind(&CChannelSession::deliverNew, shared_from_this(), /*std::make_shared<std::string>(std::move(m_sDeliverMsg)*/m_sDeliverMsg));
	}
	else if (sMayaCode == MayaCode::SendKey)
	{		
		m_sDeliverMsg = "REPLY_SEND_KEY@Y/pX2ZtIFkvnx+PLAzrdlH1qhTHE8X3bzcQ5d6rOwqs=@CkgqNYUPM06dXrltwOs5dQ==@@00000@@@@@@0@0&";
		g_tcp_io_context.post(std::bind(&CChannelSession::deliverNew, shared_from_this(), /*std::make_shared<std::string>(std::move(m_sDeliverMsg)*/m_sDeliverMsg));

	}
	else if (sMayaCode == MayaCode::LOGINHK)
	{		
		////
		//if (sPackBody.find(m_sUserId) != string::npos)
		//{
			m_sDeliverMsg = "LOGINHK1@1@2@A@00000@@@@@0@0@0&stock|;future|";
		//}
		//else
		//{
		//	m_sDeliverMsg = "LOGINHK1@1@2@A@00000@@@@@0@0@0&stock|BAD_USER;future|";
		//	// 断开连接

		//	g_logger->error("do_write Disconnect:PeerAddr={},Errcode={}", m_szPeerAddr, ec.value());
		//	channelManager.leave(shared_from_this());
		//	CloseSession();
		//}

		g_tcp_io_context.post(std::bind(&CChannelSession::deliverNew, shared_from_this(), /*std::make_shared<std::string>(std::move(m_sDeliverMsg)*/m_sDeliverMsg));
	}
	else if (sMayaCode == MayaCode::HEARTBIT)
	{		
		m_sDeliverMsg = "TEST0001@@@@@@@@@@&";
		g_tcp_io_context.post(std::bind(&CChannelSession::deliverNew, shared_from_this(),/*std::make_shared<std::string>(std::move(m_sDeliverMsg)*/m_sDeliverMsg));

	}
	else if (sMayaCode == MayaCode::HEARTBITAPI )
	{
		m_sDeliverMsg = "TEST001@@@@@@@@@@&";
		g_tcp_io_context.post(std::bind(&CChannelSession::deliverNew, shared_from_this(), /*std::make_shared<std::string>(std::move(m_sDeliverMsg)*/m_sDeliverMsg));
		
	}
	else if (sMayaCode == MayaCode::MARKET01)
	{
		//;MARKET01@@D@+@@@&CME,OG_C2104 1050;CME,HXE_C2105 415;CME,OZC_P
		// 解析订阅信息
		
		int iPosAnd = sPackBody.find("&");
		if (iPosAnd == string::npos)
		{
			return;
		}

		string subDetailInfo = sPackBody.substr(iPosAnd + 1, iPackBodyLen - iPosAnd - 1 - 1);

		if (sPackBody.find("+") != string::npos)
		{
			subDetailInfo += ";";
			//HP::MayaSplit(subDetailInfo, m_subContractMap, true, ';');
			channelManager.SubContracts(subDetailInfo, true, shared_from_this());

		}
		else if (sPackBody.find("--") != string::npos)
		{
			//m_subContractMap.clear();
			channelManager.UnSubAll(shared_from_this());
		}
		else if (sPackBody.find("-") != string::npos)
		{		
			subDetailInfo += ";";
			//HP::MayaSplit(subDetailInfo, m_subContractMap, false,';');
			channelManager.SubContracts(subDetailInfo, false, shared_from_this());
		}
	}
}