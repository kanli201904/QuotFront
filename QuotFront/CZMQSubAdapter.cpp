#include "CZMQSubAdapter.h"
//#include "Common.h"
#include "zmq.hpp"
#include "zmq_addon.hpp"
#include "ConfigManager.h"
#include "log.h"

CZMQSubAdapter::CZMQSubAdapter(char* szAddr)
{
	subscriber = NULL;

	zmq_address = GET_CFG->get_value<std::string>("zmq_address");
	zmq_rate = GET_CFG->get_value<int>("zmq_rate");
	zmq_rcvbuf = GET_CFG->get_value<int>("zmq_rcvbuf");
	zmq_rcvhwm = GET_CFG->get_value<int>("zmq_rcvhwm");
	zmq_recover = GET_CFG->get_value<int>("zmq_recover");

}

CZMQSubAdapter::~CZMQSubAdapter()
{
	if (subscriber != NULL)
	{
		zmq_close(subscriber);
		zmq_ctx_destroy(context);
	}
}

int CZMQSubAdapter::EPGMSubInit() 
{
	int iRet = -1;
	context = zmq_ctx_new();                       //创建一个新的上下文
	subscriber = zmq_socket(context, ZMQ_SUB);     //创建一个新的zmq的socket 类型为sub
	zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);    //sub类型的socket要设置过滤器，第三项为数据开头的报文，第四项为第三项的数据大小，若设置为""和0即收取所有
	zmq_setsockopt(subscriber, ZMQ_RATE, &zmq_rate, sizeof(zmq_rate)); //设置socket的接收速率，单位为kilobits per second
	zmq_setsockopt(subscriber, ZMQ_RCVBUF, &zmq_rcvbuf, sizeof(zmq_rcvbuf));//设置socket的接受区buffer, 单位为bytes
	zmq_setsockopt(subscriber, ZMQ_RCVHWM, &zmq_rcvhwm, sizeof(zmq_rcvhwm)); //设置socket的接收区高水位，如果管道内堆积的数据超过水位，新进来的数据将直接被管道drop
	//zmq_setsockopt(subscriber, ZMQ_RECOVERY_IVL, &zmq_recover, sizeof(zmq_recover)); //设置socket的能从多播组缺席的最长时间 单位为ms,这段时间发送的数据会被保存在内存里
	iRet = zmq_connect(subscriber, zmq_address.c_str());   //sub端用connect连接到多播组
	//int rc = zmq_bind(subscriber, zmq_address.c_str());   //sub端用connect连接到多播组
	std::cout << "zmq_address: " << zmq_address << "\n";
	assert(iRet == 0);
	return iRet;
}

// 
int  CZMQSubAdapter::EPGMSubRecv(char* szRecvBuf)
{		
	int iRecvLen = -1;
	uint64_t init_time_stamp = xdk_nanosecond_timestamp();
	while (iRecvLen <= 0)
	{		
		iRecvLen = zmq_recv(subscriber, szRecvBuf,768, ZMQ_DONTWAIT);
		if (iRecvLen < 0 &&
			xdk_nanosecond_timestamp() - init_time_stamp > RECONNECT_TIME_OUT)
		{
			// disconnect			
			if (zmq_disconnect(subscriber, zmq_address.c_str()) == 0)
			{
				int flag_con = -1;
				int iReconCount = 0;
				// 重连一定次数
				while (flag_con == -1 )
				{					
					if (iReconCount > RECONNECT_TIMES)
					{
						std::cout << "Exceed the reconnect times,reset counter \n";
						iReconCount = 0;
						//return -1;													
					}

					flag_con = zmq_connect(subscriber, zmq_address.c_str());

					iReconCount++;
					
					xdk_microsecond_delay(DELAY_TIME);

				}
			}
		}
	}	
	
	return iRecvLen;
}

void CZMQSubAdapter::EPGMCleanUp()
{
	if (subscriber != NULL)
	{
		zmq_close(subscriber);
		zmq_ctx_destroy(context);
	}
}


