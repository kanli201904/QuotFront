#pragma once
#include "TcpMessage.h"
#include "Common.h"
#include "log.h"
#include"boost/thread.hpp"
#include "da_marketdata.pb.h"
#include <boost/lockfree/spsc_queue.hpp>

using namespace std;
using namespace google::protobuf;
using namespace da::marketdata::v1;

extern int g_CurrentConnect;
extern char g_szSanpBuffer[1024];
extern char g_sTime[20];

extern LocalMarket::DepthData depthBids[10];
extern LocalMarket::DepthData depthAsks[10];
extern LOGGER g_logger;
extern asio::io_context g_tcp_io_context;
extern boost::lockfree::spsc_queue<std::string, boost::lockfree::capacity<1024 * 10>, boost::lockfree::fixed_sized<true>> msgDataQueue;

class CChannelInterface
{
public:
	virtual ~CChannelInterface() {}
	virtual void deliver(/*std::shared_ptr<std::string>*/char* sPtrMsg) = 0;

public:
	// 订阅列表
	unordered_map<std::string, bool> m_subContractMap; //key：合约关键信息，value: 是否订阅true已订阅，false被取消 
};

typedef std::shared_ptr<CChannelInterface> channel_ptr;
extern unordered_map<std::string, std::list<channel_ptr>> g_subContractMapList; //key：合约关键信息，value: set<>列表，存放订阅的channel ptr

class CPBTransInfo
{
public: 
	CPBTransInfo()
	{
		iNanSec = 0;
		sInstrument = "";
	}

	uint64 iNanSec;
	std::string sInstrument;

};

class Callback : boost::noncopyable
{
public:
	virtual ~Callback() {};
	virtual void onMessage(google::protobuf::Message* message, const CPBTransInfo& pbTransInfo)   = 0;
};

template <typename T>
class CallbackT : public Callback
{
public:
	typedef boost::function<void(T* message, const CPBTransInfo& pbTransInfo)> ProtobufMessageCallback;

	CallbackT(const ProtobufMessageCallback& callback)
		: callback_(callback)
	{
	}

	virtual void onMessage(google::protobuf::Message* message, const CPBTransInfo& pbTransInfo) 
	{
		T* t = dynamic_cast<T*>(message);
		assert(t != NULL);
		callback_(t, pbTransInfo);
	}

private:
	ProtobufMessageCallback callback_;
};

class ProtobufDispatcher
{

public:

	void static discardProtobufMessage(google::protobuf::Message* message, const CPBTransInfo& pbTransInfo)
	{
		cout << "Discarding " << message->GetTypeName() << endl;
	}
	~ProtobufDispatcher()
	{
	}
	ProtobufDispatcher()
		: defaultCallback_(ProtobufDispatcher::discardProtobufMessage)
	{
	}

	void onMessage(const google::protobuf::Any* message, const CPBTransInfo& pbTransInfo) 
	{
		sTypeName.assign(strchr(message->type_url().c_str(), '/') + 1);
	
		auto descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(sTypeName);
		auto prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
				
		//instance = prototype->New();
		instance = const_cast<Message*>(prototype);
		message->UnpackTo(instance);

		CallbackMap::const_iterator it = callbacks_.find(descriptor);
		if (it != callbacks_.end())
		{
			it->second->onMessage(instance, pbTransInfo);
		}
		else
		{
			defaultCallback_(instance, pbTransInfo);
		}

		// 是否有方法清空内存，而不是delete
		instance->Clear();
	}

	template<typename T>
	void registerMessageCallback(const typename CallbackT<T>::ProtobufMessageCallback& callback)
	{
		boost::shared_ptr<CallbackT<T> > pd(new CallbackT<T>(callback));
		callbacks_[T::descriptor()] = pd;
	}

	typedef std::map<const google::protobuf::Descriptor*, boost::shared_ptr<Callback> > CallbackMap;
	CallbackMap callbacks_;
	boost::function<void(google::protobuf::Message* message, const CPBTransInfo& pbTransInfo)> defaultCallback_;

	std::string sTypeName;
	Message* instance;
};

class CChannelManager
{
public:
	CChannelManager()
	{				
		//初始化pb相关信息
		m_dispatcher.registerMessageCallback<DABBMessage>(CChannelManager::onTick);		
	}

	~CChannelManager()
	{			
		google::protobuf::ShutdownProtobufLibrary();
	}

	void run();
	void join(channel_ptr channel);
	void leave(channel_ptr channel);

	void dispatchMsgNew(const std::string& sRecvMsg/*MyBuff* myBufPtr*/);
	void SubContracts(const string& sSubDetailInfo, bool isSub, channel_ptr channel);
	void UnSubAll(channel_ptr channel);

	void static onTick(DABBMessage* tick, const CPBTransInfo& pbTransInfo);

private:

	ProtobufDispatcher m_dispatcher;
	DAMarketDataPacket m_packet;
	DATopOfBookAndLastTradeMessage pData;
	DATradeStatisticsMessage* pStaticData;
	DAOrderBookMessage* pDataDepth;
	DAStatusMessage* pStatus ;
	
	 CPBTransInfo m_pbTransInfo;

	boost::lockfree::spsc_queue<std::function<void(void)>, boost::lockfree::capacity<8000>, boost::lockfree::fixed_sized<true>> subscribeQueue;
};

