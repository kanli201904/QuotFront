#include "ChannelManager.h"
#include <boost/thread.hpp>
#include "google/protobuf/arena.h"

#include <fmt/format.h>
#include "ChannelSession.h"
#include "da_marketdata.pb.h"

void CChannelManager::join(channel_ptr channel)
{
	g_CurrentConnect++;
}

void CChannelManager::leave(channel_ptr channel)
{	
	g_CurrentConnect--;

	UnSubAll(channel);
}

void CChannelManager::onTick(DABBMessage* tick, const CPBTransInfo& pbTransInfo)
{		
	//1:½âÎötick	
	const DATopOfBookAndLastTradeMessage& pData = tick->top_message();
	const DATradeStatisticsMessage& pStaticData = tick->statics();
	const DAOrderBookMessage& pDataDepth = tick->order_book();
	const DAStatusMessage& pStatus = tick->status();
	
	//2: Pase and fill the bids/asks
	memset(depthBids, 0, sizeof(depthBids));
	memset(depthAsks, 0, sizeof(depthAsks));

	for (int i = 0; i < 10 && i < pDataDepth.bid_queue_size(); ++i)
	{			
		depthBids[i].Price = pDataDepth.bid_queue(i).price();
		depthBids[i].Qty = pDataDepth.bid_queue(i).quantity();
	}

	for (int i = 0; i < 10 && i < pDataDepth.ask_queue_size(); ++i)
	{
		depthAsks[i].Price = pDataDepth.ask_queue(i).price();
		depthAsks[i].Qty = pDataDepth.ask_queue(i).quantity();
	}
	
	//// 5: Parse the time		
	LocalMarket::getFormatByNanSec((time_t)(pbTransInfo.iNanSec), g_sTime);
	g_sTime[19] = 0;

	auto r = fmt::format_to_n(g_szSanpBuffer, 1024,
		"{marketrspprex}{exchangeSymbol}@{BidPrice:.2f}@{BidQty}@{AskPrice:.2f}@{AskQty}@{LastPrice:.2f}@{LastQty}@{HighPrice:.2f}@{LowPrice:.2f}@{OpenPrice:.2f}@{ClosePrice:.2f}@{Status},{PreoPrice:.2f},{PostPrice:.2f}@{Time}@{TotalQty}@{Turnover}@{BidPrice2:.2f}@{BidPrice3:.2f}@{BidPrice4:.2f}@{BidPrice5:.2f}@{BidQty2}@{BidQty3}@{BidQty4}@{BidQty5}@{AskPrice2:.2f}@{AskPrice3:.2f}@{AskPrice4:.2f}@{AskPrice5:.2f}@{AskQty2}@{AskQty3}@{AskQty4}@{AskQty5}@@@@@Z@@55@@{PreviousClosePrice:.2f}@{ClosePrice:.2f}@Z@{BidPrice6:.2f}@{BidPrice7:.2f}@{BidPrice8:.2f}@{BidPrice9:.2f}@{BidPrice10:.2f}@{BidQty6}@{BidQty7}@{BidQty8}@{BidQty9}@{BidQty10}@{AskPrice6:.2f}@{AskPrice7:.2f}@{AskPrice8:.2f}@{AskPrice9:.2f}@{AskPrice10:.2f}@{AskQty6}@{AskQty7}@{AskQty8}@{AskQty9}@{AskQty10}@Z",
		fmt::arg("marketrspprex", MARKETRSPPREX),
		fmt::arg("exchangeSymbol", pbTransInfo.sInstrument), fmt::arg("BidPrice", depthBids[0].Price), fmt::arg("BidQty", depthBids[0].Qty), fmt::arg("AskPrice", depthAsks[0].Price), fmt::arg("AskQty", depthAsks[0].Qty),
		fmt::arg("LastPrice", pData.last_price()), fmt::arg("LastQty", pData.last_volume()), fmt::arg("HighPrice", pStaticData.day_highest_price()), fmt::arg("LowPrice", pStaticData.day_lowest_price()),
		fmt::arg("OpenPrice", pStaticData.open_price()), fmt::arg("ClosePrice", pStaticData.close_price()),
		fmt::arg("Status", pStatus.status()), fmt::arg("PreoPrice", pData.preo_price()), fmt::arg("PostPrice", pData.post_price()), fmt::arg("Time", g_sTime),
		fmt::arg("TotalQty", pData.volume()), fmt::arg("Turnover", pData.turnover()),
		fmt::arg("BidPrice2", depthBids[1].Price), fmt::arg("BidPrice3", depthBids[2].Price), fmt::arg("BidPrice4", depthBids[3].Price), fmt::arg("BidPrice5", depthBids[4].Price),
		fmt::arg("BidQty2", depthBids[1].Qty), fmt::arg("BidQty3", depthBids[2].Qty), fmt::arg("BidQty4", depthBids[3].Qty), fmt::arg("BidQty5", depthBids[4].Qty),
		fmt::arg("AskPrice2", depthAsks[1].Price), fmt::arg("AskPrice3", depthAsks[2].Price), fmt::arg("AskPrice4", depthAsks[3].Price), fmt::arg("AskPrice5", depthAsks[4].Price),
		fmt::arg("AskQty2", depthAsks[1].Qty), fmt::arg("AskQty3", depthAsks[2].Qty), fmt::arg("AskQty4", depthAsks[3].Qty), fmt::arg("AskQty5", depthAsks[4].Qty),
		fmt::arg("PreviousClosePrice", pStaticData.pre_close_price()),
		fmt::arg("BidPrice6", depthBids[5].Price), fmt::arg("BidPrice7", depthBids[6].Price), fmt::arg("BidPrice8", depthBids[7].Price), fmt::arg("BidPrice9", depthBids[8].Price), fmt::arg("BidPrice10", depthBids[9].Price),
		fmt::arg("BidQty6", depthBids[5].Qty), fmt::arg("BidQty7", depthBids[6].Qty), fmt::arg("BidQty8", depthBids[7].Qty), fmt::arg("BidQty9", depthBids[8].Qty), fmt::arg("BidQty10", depthBids[9].Qty),
		fmt::arg("AskPrice6", depthAsks[5].Price), fmt::arg("AskPrice7", depthAsks[6].Price), fmt::arg("AskPrice8", depthAsks[7].Price), fmt::arg("AskPrice9", depthAsks[8].Price), fmt::arg("AskPrice10", depthAsks[9].Price),
		fmt::arg("AskQty6", depthAsks[5].Qty), fmt::arg("AskQty7", depthAsks[6].Qty), fmt::arg("AskQty8", depthAsks[7].Qty), fmt::arg("AskQty9", depthAsks[8].Qty), fmt::arg("AskQty10", depthAsks[9].Qty));

	if (r.size < 0)
		return;

	g_szSanpBuffer[r.size] = 0;

	std::unordered_map<std::string, std::list<channel_ptr>>::iterator itr = g_subContractMapList.find(pbTransInfo.sInstrument);
	for (auto channel : (itr->second))
	{
		if (channel != nullptr)
		{
			channel->deliver(g_szSanpBuffer);
			//g_tcp_io_context.post(std::bind(&CChannelSession::deliver, (CChannelSession*)channel.get(), sSnapBuf));
		}
	}		

}

void CChannelManager::dispatchMsgNew(const std::string& sRecvMsg/*MyBuff* myBufPtr*/)
{
	// ½â°ü	
	m_packet.Clear(); 
	if (m_packet.ParseFromArray(sRecvMsg.data(), sRecvMsg.length()) == false)
		return;

	// HeartBeat
	if (m_packet.topic() == 0)
	{
		return;
	}

	for (auto iter : m_packet.messages())
	{	
		// NanSeconds
		m_pbTransInfo.iNanSec = m_packet.timestamp();

		// Instrument
		m_pbTransInfo.sInstrument = iter.market() + "@" + iter.instrument() + ".US";
		auto& data = iter.data();						

		std::unordered_map<std::string, std::list<channel_ptr>>::iterator itr = g_subContractMapList.find(m_pbTransInfo.sInstrument);
		if (itr != g_subContractMapList.end() && (itr->second).size() > 0)
		{
			m_dispatcher.onMessage(&data, m_pbTransInfo);
		}		
	}		 	
}

void CChannelManager::UnSubAll(channel_ptr channel)
{
	//loop and delete the m_subContractMapList 
	subscribeQueue.push([channel]
	{
		std::unordered_map<std::string, std::list<channel_ptr>>::iterator itr = g_subContractMapList.begin();
		for (; itr != g_subContractMapList.end(); itr++)
		{
			std::list<channel_ptr> *channelList = &(itr->second);
			channelList->remove(channel);
		}
	}
	);
}

void CChannelManager::SubContracts(const string& sSubDetailInfo, bool isSub, channel_ptr channel)
{	
	// sub contracts
	string sKeyContract;
	string sKeyReplaceContract;	

	size_t count = sSubDetailInfo.length();
	int iPos = -1;
	for (size_t i = 0; i < count; i++)
	{
		if (sSubDetailInfo[i] == ';')
		{
			// replace the "," to "@"
			iPos = sKeyContract.find(',');
			if (iPos == string::npos)
				continue;
			sKeyReplaceContract = sKeyContract.replace(iPos, 1, "@");
			
			auto iter = GET_INSTRUMENT_CFG->instrument_map.find(sKeyReplaceContract);			
			if (iter == GET_INSTRUMENT_CFG->instrument_map.end())
			{			
				// No map
				g_logger->error(" cant find the exchange symbol string {}", sKeyReplaceContract);
				sKeyContract.clear();
				sKeyReplaceContract.clear();
				continue;
			}
			
			// +
			if (isSub)
			{
				subscribeQueue.push([sKeyReplaceContract,channel]
				{
					std::unordered_map<std::string, std::list<channel_ptr>>::iterator itr = g_subContractMapList.find(sKeyReplaceContract);
					if (itr != g_subContractMapList.end())
					{
						std::list<channel_ptr> *findChannelList = &((itr->second));
						bool bChannelFund = false;
						for (auto channelPtr : (*findChannelList))
						{
							if (channelPtr == channel)
							{
								bChannelFund = true;
								break;
							}
						}

						if (bChannelFund == false)
						{
							findChannelList->emplace(findChannelList->end(), channel);
						}

					}
					else
					{
						g_subContractMapList[sKeyReplaceContract].push_back(channel);
					}
				});
				
			}
			//-
			else
			{
				subscribeQueue.push([sKeyReplaceContract, channel]
				{
					std::unordered_map<std::string, std::list<channel_ptr>>::iterator itr = g_subContractMapList.find(sKeyReplaceContract);
					if (itr != g_subContractMapList.end())
					{
						std::list<channel_ptr> *findChannelList = &(itr->second);
						findChannelList->remove(channel);
					}
				});
			}


			sKeyContract.clear();
			sKeyReplaceContract.clear();
		}
		else
		{
			sKeyContract += sSubDetailInfo[i];
		}
	}
}

void CChannelManager::run()
{	
	std::function<void(void)> func = nullptr;
	std::string sRecvMsg;

	bool bSubQueueEmpty = false;
	bool bDataQueueEmpty = false;
	while (true)
	{
		bSubQueueEmpty = false;
		bDataQueueEmpty = false;
		
		// Sub/unsub
		if (subscribeQueue.pop(func))
		{
			bSubQueueEmpty = true;
			func();
		}

		if (msgDataQueue.pop(sRecvMsg))
		{			
			dispatchMsgNew(sRecvMsg);
		}
		else if (bSubQueueEmpty == false)  // two queue are empty,sleep
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
		
	}
}
