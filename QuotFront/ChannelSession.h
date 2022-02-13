#pragma once
#include "ChannelManager.h"
#include "log.h"
#include "asio.hpp"
#include <boost/circular_buffer.hpp>
#include <boost/bind.hpp>

extern asio::io_context g_tcp_io_context;
extern LOGGER g_logger;

class CChannelSession
	: public CChannelInterface,
	public std::enable_shared_from_this<CChannelSession>
{
public:
	CChannelSession(tcp::socket socket, CChannelManager& chaManager);

	void start();
	void deliver(char* szMsg);
	void deliverNew(std::string sMsg);

	// 主动删除通道
	void CloseSession();
	//void on_monitor_time();
private:

	tcp::socket socket_;
	CChannelManager& channelManager;
	CMessage read_msg_;
	
	char m_szPeerAddr[128];
	char m_szMsgPrex[16];	
	string m_sSendMsg;
	string m_sPackHead;
	string m_sPackBodyLen;
	string m_sDeliverMsg;

	boost::circular_buffer<std::string> m_cirBuff;
	
	bool write_in_progress;
	int m_cirbuffmaxsize;
	string m_sUserId;
	//asio::deadline_timer m_monitor_timer;
private:
	void do_read_header();
	void do_read_body();
	void do_write();
	void HandleClientMsg();

};
