
#pragma once

namespace ApiType
{
	const char  SdkApi[] = "A";											// API标识
	const char  Client[] = "C";											// 客户端
	const char  Dealer[] = "D";											// 合作伙伴
}

namespace VerfityStep
{
	const char  Over[] = "0";											// 已经校验
	const char  Must[] = "1";											// 必须验证
}

namespace SetAnswerStep
{
	const char  None[] = "0";											// 已经校验
	const char  Have[] = "1";											// 必须验证
}

namespace MayaKind
{
	const char  Market[] = "";											// 行情
	const char  Future[] = "0";											// 期货
	const char  HKStock[] = "1";										// 港股
	const char  ContinentStock[] = "2";									// 大陆股票
	const char  AmericaStock[] = "3";									// 美股
}

namespace TransMode
{
	const char  CompressEnglish[] = "1";								// 压缩,不带汉字
	const char  CompressChina[] = "2";									// 压缩,带汉字
	const char  NonCompress[] = "";										// 不压缩
}

namespace MayaCode
{
	const char  HEARTBIT[] = "TEST0001";								// 测试保持连接
	const char  HEARTBITAPI[] = "TEST001";								// 测试保持连接
	const char  LOGIN[] = "LOGIN001";									// 用户登录请求代码
	const char  UNLOGIN[] = "UNLOGIN1";									// 用户退出请求代码
	const char  LOGINSAVE[] = "LOGINSV1";								// 用户登录成功信息保存的指令代码
	const char  LOGINRID[] = "LOGIN002";								// 用户多处登录踢掉前一登陆的代码
	const char  LOGINMULTI[] = "LOGIN003";								// 提示用户前面已经有人在别处登陆过的代码
	const char  ORDER[] = "ORDER001";									// 定单请求代码,及返回信息代码
	const char  ORDER002[] = "ORDER002";								// 强平下单请求代码
	const char  ORDER003[] = "ORDER003";								// 柜台改单请求代码
	const char  ORDER004[] = "ORDER004";								// 盈损单下单请求代码
	const char  OrderMarker[] = "ORDER005";								// 做市商下单请求代码,及返回信息代码
	const char  ORDERSTATUS[] = "ORDERSTA";								// 定单状态请求代码
	const char  ORDERSEARCH[] = "ORDERLST";								// 定单查询请求代码
	const char  MODIFY[] = "MODIFY01";									// 客户端改单请求代码,及返回信息代码
	const char  CHANGEDATE[] = "CHANGEDT";								// 调期请求代码,及返回信息代码
	const char  CHANGELIST[] = "CHANGELS";								// 调期明细列表请求代码,及返回信息代码
	const char  CANCEL[] = "CANCEL01";									// 撤单请求代码
	const char  CANCELCAST[] = "CANCST01";								// 撤单广播返回信息代码
	const char  FILLEDCAST[] = "FILCST01";								// 成交广播返回信息代码
	const char  FILLEDSEARCH[] = "FILLLIST";							// 成交查询请求代码
	const char  OPENSEARCH[] = "OPENLIST";								// 持仓查询请求代码
	const char  OPENDETAIL[] = "OPENDETL";								// 持仓明细查询请求代码（包括昨仓和今仓的分笔持仓明细）
	const char  YHOLDSEARCH[] = "YHLDLIST";								// 昨日持仓请求代码
	const char  HOLDSTATUS[] = "HOLDSTAT";								// 持仓状态请求代码
	const char  ACCOUNTSEARCH[] = "DEPOSTAT";							// 账户资金查询请求代码
	const char  ACCOUNTLAST[] = "ACNTLAST";								// 资金推送
	const char  USERACCOUNTINFOLIST[] = "USERLIST";						// 用户信息取得请求代码
	const char  SYSTEMNOGET[] = "SYSTEMNO";								// 系统编号取得请求代码
	const char  GETTCONTRACT[] = "CONTRACT";							// 用户请求所有可用的合约
	const char  GETEXCHANGE[] = "EXCHANGE";								// 用户请求所有可用的交易所信息
	const char  GETDEFEEDEPO[] = "DEFEEDEP";							// 请求读用户缺省的手续费、保证金
	const char  GETTDFEE[] = "TRADEFEE";								// 请求读用户交易手续费
	const char  GETTDDEPO[] = "TRADEDEP";								// 请求用户交易保证金
	const char  CLIENTUPPER[] = "CLNTUPPR";								// 请求客户上手信息
	const char  TESTINFO[] = "TESTINFO";								// 请求客户上手信息
	const char  GETSYSNO[] = "GETSYSNO";								// 请求客户的最大系统号
	const char  MARKET01[] = "MARKET01";								// 请求行情
	const char  MARKET02[] = "M2";										// 推送交易日
	const char  MODIFYPW[] = "MODIFYPW";								// 修改用户密码
	const char  FILLEDINFO[] = "FILLINFO";								// 成交情况
	const char  WAITINFO[] = "WAITINFO";								// 排队情况
	const char  SETTLEPRICE[] = "STLPRICE";								// 上日结算价
	const char  TCASH[] = "TCASH001";									// 出入金
	const char  EnFrozenLogin[] = "FroLogin";							// 冻结账户
	const char  UnFrozenLogin[] = "FreLogin";							// 解冻账户
	const char  delOrder[] = "DelOrder";								// 删除下单
	const char  GuitaiPws[] = "GuiTmpws";								// 柜台生成新的密码
	const char  CURRENCYLIST[] = "CRNYLIST";							// 币种信息列表请求代码
	const char  FROZENUSER[] = "FROZUSER";								// 柜台冻结客户不让继续交易
	const char  UNFROZENUSER[] = "UNFRUSER";							// 柜台解冻客户
	const char  FROZENUSERMONEY[] = "FRUSERMN";							// 柜台冻结客户部分资金
	const char  UNFROZENUSERMONEY[] = "UNFRUSMN";						// 柜台解冻客户被冻结的资金
	const char  GETDELIVERYDATE[] = "DELIVERY";							// 请求当天的LME合约的交割日
	const char  UPDATESTATUS[] = "UPSTATUS";							// 更新定单状态的请求
	const char  GETFILLEDNO[] = "FILLEDNO";								// 请求生成成交编号
	const char  SETYINGSUN[] = "YSSET001";								// 止损止盈设置请求
	const char  DELYINGSUN[] = "YSDEL001";								// 止损止盈设置删除请求
	const char  GETYINGSUNLIST[] = "YSLIST01";							// 止损止盈设置列表数据取得请求
	const char  LOGINYINGSUN[] = "LOGINYSQ";							// 前置登录止损止盈服务器的请求，将前置的ID放到NetInfo的accountNo中发送
	const char  SENDMSG[] = "SNDMSG01";									// 发送消息
	const char  GETVERSION[] = "GETVER01";								// 取得最新版本号
	const char  GETVERSIONCOMU[] = "GETVERCM";							// 取得最新底层通信版本号
	const char  GETSTRATEGY[] = "GETSTRLS";								// 取得现有策略列表
	const char  GETSTRATEGYDETAIL[] = "GETSTRDL";						// 取得现有策略明细列表
	const char  ReportDaily[] = "REPORTDA";								// 请求客户历史成交数据
	const char  ReportSettleDaily[] = "REPORTSD";						// 请求客户日交易结算报表数据
	const char  ReportMonthly[] = "REPORTMO";							// 请求客户月交易结算报表
	const char  GetMoneyApplyList[] = "APPMLIST";						// 请求客户出入金申请数据列表
	const char  MoneyApply[] = "MONEYAPP";								// 客户出入金申请
	const char  GetBankInfoList[] = "BANKLIST";							// 请求客户银行卡数据列表
	const char  SaveBankInfo[] = "SAVEBANK";							// 请求保存客户银行卡模版数据
	const char  DelBankInfo[] = "DELEBANK";								// 请求删除客户银行卡模版数据
	const char  GetGoodsApplyList[] = "APPGLIST";						// 请求客户提存货申请数据列表
	const char  GoodsApply[] = "GOODSAPP";								// 客户提存货申请
	const char  GetConditionList[] = "TJLIST01";						// 请求客户条件单设置数据列表
	const char  ConditionSet[] = "TJSETREQ";							// 客户条件单设置请求（有条件单编号是修改，没有是新增）
	const char  ConditionDel[] = "TJDELREQ";							// 客户条件单设置删除
	const char  UpdateStrategyHold[] = "UPDATESTRATEGYHOLD";			// 更新策略持仓
	const char  SearchStrategyHold[] = "SEARCHSTRATEGYHOLD";			// 检索策略持仓
	const char  GETTTIME[] = "GETTTIME";								// 取得当前交易日的开盘和收盘时间
	const char  GetSettlePrice[] = "GETPRICE";							// 取得最新的昨结算价
	const char  SearchGuaDan[] = "SEARCHGD";							// 检索挂单数据
	const char  SearchHoldTotal[] = "SEARCHHT";							// 检索持仓合计数据
	const char  SearchHistoryAccount[] = "SEARCHHA";					// 检索历史结算后资金数据
	const char  CMEMarketRight[] = "CMEMarketRight";					// 客户端请求取得CME行情的权限
	const char  OptionExercise[] = "OptionExercise";					// 期权行权
	const char  BALCANCEL[] = "BALCANCEL";								// 差额被取消
	const char  LOGINHK[] = "LOGINHK1";									// 股票用户登录请求代码
	const char  GetExchangeStock[] = "ExchStLs";						// 取得股票交易所列表数据
	const char  GetContractStock[] = "ContStLs";						// 取得股票合约列表数据
	const char  UpdateContractStock[] = "UpCoStLs";						// 取得盘中更新的股票合约数据列表
	const char  GetOrderListStock[] = "OrdeStLs";						// 取得股票委托列表数据
	const char  GetFilledListStock[] = "FillStLs";						// 取得股票成交列表数据
	const char  GetHoldListStock[] = "HoldStLs";						// 取得股票持仓列表数据
	const char  GetAccountListStock[] = "AccoStLs";						// 取得股票资金列表数据
	const char  GetContractStockUpperTick[] = "UptiStLs";				// 取得股票合约最小变动单位列表数据
	const char  OrderStockHK[] = "OrdeStHK";							// 港股下单及返回指令
	const char  OrderStockHK02[] = "OrdeSt02";							// 股票柜台强平及返回指令
	const char  OrderStockHK03[] = "OrdeSt03";							// 股票柜台下单及返回指令
	const char  SystemNoStockHK[] = "SystStHK";							// 港股系统号返回指令
	const char  ModifyStockHK[] = "ModiStHK";							// 港股改单及返回指令
	const char  CancelStockHK[] = "CancStHK";							// 港股撤单及返回指令
	const char  CancelStockHKResponse[] = "CancStRe";					// 柜台股票撤单返回指令
	const char  FilledStockHK[] = "FillStHK";							// 港股成交返回指令
	const char  AccountStockHK[] = "AccoStHK";							// 港股最新资金返回指令
	const char  StatusStockHK[] = "StatStHK";							// 港股最新订单状态返回指令
	const char  HoldStockHK[] = "HoldStHK";								// 港股最新持仓返回指令
	const char  UpdateClient[] = "UpdtClet";							// 更新客户端版本指令
	const char  TransferMoney[] = "TranMony";							// 股票期货之间资金调拨指令
	const char  GetLoginHistoryList[] = "LogHisLs";						// 取得用户上次登录和同时在线登录的Ip地址列表
	const char  LogException[] = "LogExcep";							// 记录异常log
	const char  GetServerInfoList[] = "ServList";						// 取得服务器信息列表
	const char  GetMarketFilledInfoList[] = "GET_TRADE_LIST";			// 取得合约行情最新成交信息列表
	const char  SearchMSg[] = "SearchMSg";								// 查询消息
	const char  GetPlateList[] = "GetPlatL";							// 取得期货板块数据列表
	const char  GetPlateListStockHK[] = "GetPlSHL";						// 取得港股板块数据列表
	const char  GetPlateListStockAM[] = "GetPlSAL";						// 取得港美股板块数据列表
	const char  GetPlateListStockCN[] = "GetPlSCL";						// 取得国内股票板块数据列表
	const char  GetActiveOptionList[] = "GetAcOpL";						// 取得活跃期权数据列表
	const char  GetStrategyList[] = "GetSratL";							// 取得策略数据列表
	const char  GetStrategyDetailList[] = "GetSrDtL";					// 取得策略明细数据列表
	const char  GetStrategyOrderSeqList[] = "GetSrOSL";					// 取得策略单明细下单顺序数据列表
	const char  setStrategy[] = "SetSrate";								// 设置策略
	const char  setStrategyDetail[] = "SetSraDt";						// 设置策略明细
	const char  delStrategy[] = "DelSrate";								// 删除策略
	const char  CheckStrategyOrder[] = "CheSraOd";						// 校对策略订单数据
	const char  StrategyOrderFillLeg[] = "FillLeg1";					// 策略订单补腿请求
	const char  StrategyTriggerInfo[] = "StraTrig";						// 策略触发消息返回
	const char  GetCommondityInfoList[] = "GetComLs";					// 取得商品信息列表
	const char  GetUserCommondityFeeList[] = "GetUsCoL";				// 取得用户商品跳点盈利信息列表
	const char  GetStockHKBrokerList[] = "GetHKBrL";					// 取得港股经纪商信息列表
	const char  GetMainContractList[] = "GetMConL";						// 取得期货主力合约信息列表
	const char  AskMarketBrokerList[] = "BROKER";						// 请求港股行情经纪商牌号列表
	const char  GetExchangeTime[] = "GETETIME";							// 取得交易所的开盘和收盘时间  2016-12-05
	const char  GetCommonTime[] = "GETCTIME";							// 取产品的开收盘时间
	const char  getSettlementInfo[] = "GetSettl";						// 取得结算单数据
	const char  getSettlementConfirm[] = "GetSeCon";					// 取得结算确认信息
	const char  CreateAccount[] = "CratAcct";							// 开户指令
	const char  UpdateClientContractData[] = "UpClCoDa";				// 盘中实时更新指定用户的合约数据（更新股票的持仓限额）
	const char  GetQuestions[] = "VfQuList";							// 认证问题列表数据请求
	const char  SafeVerify[] = "SfVerify";								// 安全认证请求
	const char  SetVerifyQA[] = "SetVeriQA";							// 密保问题答案设置请求
	const char  VerifyCode[] = "RequVfCd";								// 手机验证码发送请求
	const char  ReqPublicKey[] = "REQ_PUBLIC_KEY";						// 获取RSA公钥
	const char  ReplyPublicKey[] = "REPLY_PUBLIC_KEY";					// 返回RSA公钥
	const char  SendKey[] = "SEND_KEY";									// 发送对称公钥
	const char  ReplySendKey[] = "REPLY_SEND_KEY";						// 回应对称公钥
	const char  GetBroker[] = "BROKER";									// 获取经济商
	const char  ReplyBroker[] = "B";									// 获取经济商
	const char  GetComdy[] = "GetComdy";								// 获取商品列表
	const char  QuerySettleInfoConfirm[] = "QUERYSETTLEINFOCONFIRM";	// 获取结算单
	const char  NetCheck[] = "NCV_netcheck";							// 心跳消息
	const char  PasswdShow[] = "******";								// 密码字段在log文件显示内容
};

