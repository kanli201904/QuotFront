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
	context = zmq_ctx_new();                       //����һ���µ�������
	subscriber = zmq_socket(context, ZMQ_SUB);     //����һ���µ�zmq��socket ����Ϊsub
	zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);    //sub���͵�socketҪ���ù�������������Ϊ���ݿ�ͷ�ı��ģ�������Ϊ����������ݴ�С��������Ϊ""��0����ȡ����
	zmq_setsockopt(subscriber, ZMQ_RATE, &zmq_rate, sizeof(zmq_rate)); //����socket�Ľ������ʣ���λΪkilobits per second
	zmq_setsockopt(subscriber, ZMQ_RCVBUF, &zmq_rcvbuf, sizeof(zmq_rcvbuf));//����socket�Ľ�����buffer, ��λΪbytes
	zmq_setsockopt(subscriber, ZMQ_RCVHWM, &zmq_rcvhwm, sizeof(zmq_rcvhwm)); //����socket�Ľ�������ˮλ������ܵ��ڶѻ������ݳ���ˮλ���½��������ݽ�ֱ�ӱ��ܵ�drop
	//zmq_setsockopt(subscriber, ZMQ_RECOVERY_IVL, &zmq_recover, sizeof(zmq_recover)); //����socket���ܴӶಥ��ȱϯ���ʱ�� ��λΪms,���ʱ�䷢�͵����ݻᱻ�������ڴ���
	iRet = zmq_connect(subscriber, zmq_address.c_str());   //sub����connect���ӵ��ಥ��
	//int rc = zmq_bind(subscriber, zmq_address.c_str());   //sub����connect���ӵ��ಥ��
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
				// ����һ������
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


