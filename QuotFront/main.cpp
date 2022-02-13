// Synchronous echo server.

#include <array>
#include <iostream>
#include <string>
#include "asio.hpp"
#include "TcpServer.h"
#include "CZMQSubAdapter.h"
#include "ChannelManager.h"
#include "ConfigManager.h"

#include<boost/date_time/posix_time/posix_time.hpp>
#include <boost/atomic.hpp>

#ifdef _WIN32
#include <process.h>
#include <conio.h>

char g_ExeFile[MAX_PATH] = { 0 };		//执行程序
char g_BinPath[MAX_PATH] = { 0 };     //运行路径
SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

#endif
#define SVC_NAME	"QuotFront"

bool g_MainClose;			//关闭程序
int g_CurrentConnect = 0;
char g_szSanpBuffer[1024] = { 0 };
char g_sTime[20] = { 0 };

asio::io_context g_tcp_io_context;
CChannelManager g_channelManager;

LocalMarket::DepthData depthBids[10];
LocalMarket::DepthData depthAsks[10];

LOGGER g_logger = LoggerUtils::get_mutable_instance().getLogger("QuoteFront");
//boost::shared_mutex  g_session_lock;

unordered_map<std::string, std::list<channel_ptr>> g_subContractMapList; //key：合约关键信息，value: set<>列表，存放订阅的channel ptr
boost::lockfree::spsc_queue</*MyBuff**/std::string, boost::lockfree::capacity<1024*10>, boost::lockfree::fixed_sized<true>> msgDataQueue;

// async 
void StartTcpServer()
{
	try
	{	
		int iPort = GET_CFG->get_value<int>("port");
		int iMaxConnect = GET_CFG->get_value<int>("maxconnect");
		tcp::endpoint endpoint(tcp::v4(), iPort);
		CTcpServer servers(g_tcp_io_context, endpoint, g_channelManager,iMaxConnect);

		g_logger->info("StartTcpServer port:{}", iPort);
		
		g_tcp_io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}


void EPGMSubscriberThread()
{	
	try
	{
		int iRecvLen = 0;
		char szAddr[128] = { 0 };

		CZMQSubAdapter ZMQSubAdapter(szAddr);
		int iEPGMRet = ZMQSubAdapter.EPGMSubInit();
		if (iEPGMRet != 0)
		{
			g_logger->debug("EGPM failed {}", iEPGMRet);
			return;
		}
	
		char szRecvMsg[769] = { 0 };		
		std::string sRecvMsg;
		while (!g_MainClose)
		{			
			iRecvLen = ZMQSubAdapter.EPGMSubRecv(szRecvMsg);
			if (iRecvLen < 0)
				continue;
						
			sRecvMsg.assign(szRecvMsg, iRecvLen);
			if (msgDataQueue.push(sRecvMsg) == false)
			{
				g_logger->warn("msgDataQueue push failed,QueueCount={}", msgDataQueue.read_available());
			}			
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}


/********************************************************************
 *	函数名:
 *	编写者:
 *	输入参数:
 *  输出参数：
 *	功能描述:
 ********************************************************************/
int Startup(void)
{	
	//start consumer
	std::thread thread_consumer([] {
		g_channelManager.run();
	});
	thread_consumer.detach();

	std::this_thread::sleep_for(std::chrono::microseconds(1));
	
	// Start up the epgm receive thread 
	std::thread epgmThread([&]() {
		EPGMSubscriberThread();
	});
	epgmThread.detach();
	
	// Start up the tcp server
	StartTcpServer();
	
	return 0;
}

/********************************************************************
 *	函数名:
 *	编写者:
 *	输入参数:
 *  输出参数：
 *	功能描述:
 ********************************************************************/
void Cleanup()
{
	//
	g_tcp_io_context.stop();

}

//控制台入口
void ConsoleMain()
{
	if (Startup() == 0)
	{
		bool b1 = false;
		while (!g_MainClose)
		{
			if (kbhit() != 0)
			{
				int c = getch();
				printf("%c", c);
				switch (c) {
				case 'q':
				case 'Q':
					b1 = true;
					break;
				case 13:
					printf("\n");
					if (b1) g_MainClose = true;
				default:
					b1 = false;
				}
			}

			Sleep(50);
		}//end while
	}
	Cleanup();
}

#ifdef _WIN32

//安装服务
int InstService()
{
	int ret = 0;
	SC_HANDLE svc = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (svc == 0)
	{
		ret = GetLastError();
	}
	else
	{
		char svcfile[MAX_PATH];
		sprintf(svcfile, """%s"" -Service", g_ExeFile);
		//printf("%s\n", svcfile);
		SC_HANDLE hnd = CreateService(svc, SVC_NAME, SVC_NAME,
			SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START,
			SERVICE_ERROR_NORMAL, svcfile, "", NULL, "", NULL, NULL);
		if (hnd == 0)
		{
			ret = GetLastError();
		}
		else
		{
			CloseServiceHandle(hnd);
		}
		CloseServiceHandle(svc);
	}
	return ret;
}

//卸载服务
int UninstService()
{
	int ret = 0;
	SC_HANDLE svc = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (svc == 0)
	{
		ret = GetLastError();
	}
	else
	{
		SC_HANDLE hnd = OpenService(svc, SVC_NAME, SERVICE_ALL_ACCESS);
		if (hnd == 0)
		{
			ret = GetLastError();
		}
		else
		{
			if (!DeleteService(hnd))
			{
				ret = GetLastError();
			}
			CloseServiceHandle(hnd);
		}
		CloseServiceHandle(svc);
	}
	return ret;
}

//运行服务
int RunService()
{
	int ret = 0;
	SC_HANDLE svc = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (svc == 0)
	{
		ret = GetLastError();
	}
	else
	{
		SC_HANDLE hnd = OpenService(svc, SVC_NAME, SERVICE_ALL_ACCESS);
		if (hnd == 0)
		{
			ret = GetLastError();
		}
		else
		{
			if (!StartService(hnd, 0, NULL))
			{
				ret = GetLastError();
			}
			CloseServiceHandle(hnd);
		}
		CloseServiceHandle(svc);
	}
	return ret;
}

//停止服务
int StopService()
{
	int ret = 0;
	SC_HANDLE svc = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (svc == 0)
	{
		ret = GetLastError();
	}
	else
	{
		SC_HANDLE hnd = OpenService(svc, SVC_NAME, SERVICE_ALL_ACCESS);
		if (hnd == 0)
		{
			ret = GetLastError();
		}
		else
		{
			SERVICE_STATUS svcstatus;
			if (!ControlService(hnd, SERVICE_CONTROL_STOP, &svcstatus))
			{
				ret = GetLastError();
			}
			CloseServiceHandle(hnd);
		}
		CloseServiceHandle(svc);
	}
	return ret;
}
//检查服务状态
int CheckService()
{
	int ret = 0;
	SC_HANDLE svc = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (svc == 0)
	{
		ret = GetLastError();
	}
	else
	{
		SC_HANDLE hnd = OpenService(svc, SVC_NAME, SERVICE_ALL_ACCESS);
		if (hnd == 0)
		{
			ret = GetLastError();
		}
		else
		{
			SERVICE_STATUS svcstatus;
			if (QueryServiceStatus(hnd, &svcstatus))
			{
				if (svcstatus.dwCurrentState == SERVICE_STOPPED)
				{
					printf("Service %s is stopped.\n", SVC_NAME);
				}
				else
				{
					printf("Service %s is running.\n", SVC_NAME);
				}
			}
			else
			{
				ret = GetLastError();
			}
			CloseServiceHandle(hnd);
		}
		CloseServiceHandle(svc);
	}
	return ret;
}

//外部控制
void WINAPI ControlHandler(DWORD request)
{
	switch (request)
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(hStatus, &ServiceStatus);
		g_MainClose = false;			//关闭程序

		Cleanup();
		exit(0);


	default:
		break;
	}
}

//服务入口
void WINAPI ServiceMain(int argc, char** argv)
{
	ServiceStatus.dwServiceType = SERVICE_WIN32;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	ServiceStatus.dwWin32ExitCode = 0;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;
	hStatus = RegisterServiceCtrlHandler(SVC_NAME, (LPHANDLER_FUNCTION)ControlHandler);
	if (hStatus == 0)
	{
		return;
	}

	int iStartupRet = Startup();
	if (iStartupRet == 0)
	{

		//服务循环
		while (!g_MainClose)
		{
			Sleep(50);
		}
	}
	Cleanup();

	//终止服务
	ServiceStatus.dwWin32ExitCode = iStartupRet;
	ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(hStatus, &ServiceStatus);

}

//启动服务
void InitService()
{
	SERVICE_TABLE_ENTRY ServiceTable[2];
	ServiceTable[0].lpServiceName =(LPSTR)SVC_NAME;
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;
	//启动服务的控制分派机线程
	StartServiceCtrlDispatcher(ServiceTable);
}

void PrintErrorMsg(int ErrNo)
{
	char *pMsg;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		ErrNo, 0, (LPSTR)&pMsg, 0, NULL);
	printf("%s\n", pMsg);
	LocalFree(pMsg);
}

#endif
//______________________________________________________________________________
//******************************************************************************
int main(int argc, char *argv[])
{
#ifndef _WIN32
	ConsoleMain();
	return 0;
#else
	_fullpath(g_ExeFile, argv[0], MAX_PATH);
	if (argc == 1)
	{
		ConsoleMain();
		return 0;
	}
	int swArg = 0;
	for (int i = 1; i < argc; i++)
	{
		char *swStr = strupr(argv[i]);
		if (*swStr == '-' || *swStr == '/')
		{
			swStr++;
			if (strcmp(swStr, "INSTALL") == 0)
			{
				swArg = 1;
			}
			else if (strcmp(swStr, "UNINSTALL") == 0)
			{
				swArg = 2;
			}
			else if (strcmp(swStr, "RUN") == 0)
			{
				swArg = 3;
			}
			else if (strcmp(swStr, "STOP") == 0)
			{
				swArg = 4;
			}
			else if (strcmp(swStr, "STATUS") == 0)
			{
				swArg = 5;
			}
			else if (strcmp(swStr, "SERVICE") == 0)
			{
				swArg = 6;
			}
		}
	}

	int ErrNo = 0;
	switch (swArg)
	{
	case 1:
		ErrNo = InstService();
		if (ErrNo == 0)
		{
			printf("Install Ok.\n");
		}
		break;
	case 2:
		ErrNo = UninstService();
		if (ErrNo == 0)
		{
			printf("Uninstall Ok.\n");
		}
		break;
	case 3:
		ErrNo = RunService();
		if (ErrNo == 0)
		{
			printf("Run Ok.\n");
		}
		break;
	case 4:
		ErrNo = StopService();
		if (ErrNo == 0)
		{
			printf("Stop Ok.\n");
		}
		break;
	case 5:
		ErrNo = CheckService();
		break;
	case 6:
		InitService();
		break;
	default:
		printf(" strategyTrade using:\n");
		printf("  run it on console    : strategyTrade\n");
		printf("  install the service  : strategyTrade -install\n");
		printf("  uninstall the service: strategyTrade -uninstall\n");
		printf("  run the service      : strategyTrade -run\n");
		printf("  stop the service     : strategyTrade -stop\n");
		printf("  check the service    : strategyTrade -status\n");
		printf("  this help text       : strategyTrade -help\n");
	}

	if (ErrNo != 0)
	{
		PrintErrorMsg(ErrNo);
	}
	return ErrNo;
#endif
}