
#pragma once

namespace ApiType
{
	const char  SdkApi[] = "A";											// API��ʶ
	const char  Client[] = "C";											// �ͻ���
	const char  Dealer[] = "D";											// �������
}

namespace VerfityStep
{
	const char  Over[] = "0";											// �Ѿ�У��
	const char  Must[] = "1";											// ������֤
}

namespace SetAnswerStep
{
	const char  None[] = "0";											// �Ѿ�У��
	const char  Have[] = "1";											// ������֤
}

namespace MayaKind
{
	const char  Market[] = "";											// ����
	const char  Future[] = "0";											// �ڻ�
	const char  HKStock[] = "1";										// �۹�
	const char  ContinentStock[] = "2";									// ��½��Ʊ
	const char  AmericaStock[] = "3";									// ����
}

namespace TransMode
{
	const char  CompressEnglish[] = "1";								// ѹ��,��������
	const char  CompressChina[] = "2";									// ѹ��,������
	const char  NonCompress[] = "";										// ��ѹ��
}

namespace MayaCode
{
	const char  HEARTBIT[] = "TEST0001";								// ���Ա�������
	const char  HEARTBITAPI[] = "TEST001";								// ���Ա�������
	const char  LOGIN[] = "LOGIN001";									// �û���¼�������
	const char  UNLOGIN[] = "UNLOGIN1";									// �û��˳��������
	const char  LOGINSAVE[] = "LOGINSV1";								// �û���¼�ɹ���Ϣ�����ָ�����
	const char  LOGINRID[] = "LOGIN002";								// �û��ദ��¼�ߵ�ǰһ��½�Ĵ���
	const char  LOGINMULTI[] = "LOGIN003";								// ��ʾ�û�ǰ���Ѿ������ڱ𴦵�½���Ĵ���
	const char  ORDER[] = "ORDER001";									// �����������,��������Ϣ����
	const char  ORDER002[] = "ORDER002";								// ǿƽ�µ��������
	const char  ORDER003[] = "ORDER003";								// ��̨�ĵ��������
	const char  ORDER004[] = "ORDER004";								// ӯ���µ��������
	const char  OrderMarker[] = "ORDER005";								// �������µ��������,��������Ϣ����
	const char  ORDERSTATUS[] = "ORDERSTA";								// ����״̬�������
	const char  ORDERSEARCH[] = "ORDERLST";								// ������ѯ�������
	const char  MODIFY[] = "MODIFY01";									// �ͻ��˸ĵ��������,��������Ϣ����
	const char  CHANGEDATE[] = "CHANGEDT";								// �����������,��������Ϣ����
	const char  CHANGELIST[] = "CHANGELS";								// ������ϸ�б��������,��������Ϣ����
	const char  CANCEL[] = "CANCEL01";									// �����������
	const char  CANCELCAST[] = "CANCST01";								// �����㲥������Ϣ����
	const char  FILLEDCAST[] = "FILCST01";								// �ɽ��㲥������Ϣ����
	const char  FILLEDSEARCH[] = "FILLLIST";							// �ɽ���ѯ�������
	const char  OPENSEARCH[] = "OPENLIST";								// �ֲֲ�ѯ�������
	const char  OPENDETAIL[] = "OPENDETL";								// �ֲ���ϸ��ѯ������루������ֺͽ�ֵķֱʳֲ���ϸ��
	const char  YHOLDSEARCH[] = "YHLDLIST";								// ���ճֲ��������
	const char  HOLDSTATUS[] = "HOLDSTAT";								// �ֲ�״̬�������
	const char  ACCOUNTSEARCH[] = "DEPOSTAT";							// �˻��ʽ��ѯ�������
	const char  ACCOUNTLAST[] = "ACNTLAST";								// �ʽ�����
	const char  USERACCOUNTINFOLIST[] = "USERLIST";						// �û���Ϣȡ���������
	const char  SYSTEMNOGET[] = "SYSTEMNO";								// ϵͳ���ȡ���������
	const char  GETTCONTRACT[] = "CONTRACT";							// �û��������п��õĺ�Լ
	const char  GETEXCHANGE[] = "EXCHANGE";								// �û��������п��õĽ�������Ϣ
	const char  GETDEFEEDEPO[] = "DEFEEDEP";							// ������û�ȱʡ�������ѡ���֤��
	const char  GETTDFEE[] = "TRADEFEE";								// ������û�����������
	const char  GETTDDEPO[] = "TRADEDEP";								// �����û����ױ�֤��
	const char  CLIENTUPPER[] = "CLNTUPPR";								// ����ͻ�������Ϣ
	const char  TESTINFO[] = "TESTINFO";								// ����ͻ�������Ϣ
	const char  GETSYSNO[] = "GETSYSNO";								// ����ͻ������ϵͳ��
	const char  MARKET01[] = "MARKET01";								// ��������
	const char  MARKET02[] = "M2";										// ���ͽ�����
	const char  MODIFYPW[] = "MODIFYPW";								// �޸��û�����
	const char  FILLEDINFO[] = "FILLINFO";								// �ɽ����
	const char  WAITINFO[] = "WAITINFO";								// �Ŷ����
	const char  SETTLEPRICE[] = "STLPRICE";								// ���ս����
	const char  TCASH[] = "TCASH001";									// �����
	const char  EnFrozenLogin[] = "FroLogin";							// �����˻�
	const char  UnFrozenLogin[] = "FreLogin";							// �ⶳ�˻�
	const char  delOrder[] = "DelOrder";								// ɾ���µ�
	const char  GuitaiPws[] = "GuiTmpws";								// ��̨�����µ�����
	const char  CURRENCYLIST[] = "CRNYLIST";							// ������Ϣ�б��������
	const char  FROZENUSER[] = "FROZUSER";								// ��̨����ͻ����ü�������
	const char  UNFROZENUSER[] = "UNFRUSER";							// ��̨�ⶳ�ͻ�
	const char  FROZENUSERMONEY[] = "FRUSERMN";							// ��̨����ͻ������ʽ�
	const char  UNFROZENUSERMONEY[] = "UNFRUSMN";						// ��̨�ⶳ�ͻ���������ʽ�
	const char  GETDELIVERYDATE[] = "DELIVERY";							// �������LME��Լ�Ľ�����
	const char  UPDATESTATUS[] = "UPSTATUS";							// ���¶���״̬������
	const char  GETFILLEDNO[] = "FILLEDNO";								// �������ɳɽ����
	const char  SETYINGSUN[] = "YSSET001";								// ֹ��ֹӯ��������
	const char  DELYINGSUN[] = "YSDEL001";								// ֹ��ֹӯ����ɾ������
	const char  GETYINGSUNLIST[] = "YSLIST01";							// ֹ��ֹӯ�����б�����ȡ������
	const char  LOGINYINGSUN[] = "LOGINYSQ";							// ǰ�õ�¼ֹ��ֹӯ�����������󣬽�ǰ�õ�ID�ŵ�NetInfo��accountNo�з���
	const char  SENDMSG[] = "SNDMSG01";									// ������Ϣ
	const char  GETVERSION[] = "GETVER01";								// ȡ�����°汾��
	const char  GETVERSIONCOMU[] = "GETVERCM";							// ȡ�����µײ�ͨ�Ű汾��
	const char  GETSTRATEGY[] = "GETSTRLS";								// ȡ�����в����б�
	const char  GETSTRATEGYDETAIL[] = "GETSTRDL";						// ȡ�����в�����ϸ�б�
	const char  ReportDaily[] = "REPORTDA";								// ����ͻ���ʷ�ɽ�����
	const char  ReportSettleDaily[] = "REPORTSD";						// ����ͻ��ս��׽��㱨������
	const char  ReportMonthly[] = "REPORTMO";							// ����ͻ��½��׽��㱨��
	const char  GetMoneyApplyList[] = "APPMLIST";						// ����ͻ���������������б�
	const char  MoneyApply[] = "MONEYAPP";								// �ͻ����������
	const char  GetBankInfoList[] = "BANKLIST";							// ����ͻ����п������б�
	const char  SaveBankInfo[] = "SAVEBANK";							// ���󱣴�ͻ����п�ģ������
	const char  DelBankInfo[] = "DELEBANK";								// ����ɾ���ͻ����п�ģ������
	const char  GetGoodsApplyList[] = "APPGLIST";						// ����ͻ��������������б�
	const char  GoodsApply[] = "GOODSAPP";								// �ͻ���������
	const char  GetConditionList[] = "TJLIST01";						// ����ͻ����������������б�
	const char  ConditionSet[] = "TJSETREQ";							// �ͻ�����������������������������޸ģ�û����������
	const char  ConditionDel[] = "TJDELREQ";							// �ͻ�����������ɾ��
	const char  UpdateStrategyHold[] = "UPDATESTRATEGYHOLD";			// ���²��Գֲ�
	const char  SearchStrategyHold[] = "SEARCHSTRATEGYHOLD";			// �������Գֲ�
	const char  GETTTIME[] = "GETTTIME";								// ȡ�õ�ǰ�����յĿ��̺�����ʱ��
	const char  GetSettlePrice[] = "GETPRICE";							// ȡ�����µ�������
	const char  SearchGuaDan[] = "SEARCHGD";							// �����ҵ�����
	const char  SearchHoldTotal[] = "SEARCHHT";							// �����ֲֺϼ�����
	const char  SearchHistoryAccount[] = "SEARCHHA";					// ������ʷ������ʽ�����
	const char  CMEMarketRight[] = "CMEMarketRight";					// �ͻ�������ȡ��CME�����Ȩ��
	const char  OptionExercise[] = "OptionExercise";					// ��Ȩ��Ȩ
	const char  BALCANCEL[] = "BALCANCEL";								// ��ȡ��
	const char  LOGINHK[] = "LOGINHK1";									// ��Ʊ�û���¼�������
	const char  GetExchangeStock[] = "ExchStLs";						// ȡ�ù�Ʊ�������б�����
	const char  GetContractStock[] = "ContStLs";						// ȡ�ù�Ʊ��Լ�б�����
	const char  UpdateContractStock[] = "UpCoStLs";						// ȡ�����и��µĹ�Ʊ��Լ�����б�
	const char  GetOrderListStock[] = "OrdeStLs";						// ȡ�ù�Ʊί���б�����
	const char  GetFilledListStock[] = "FillStLs";						// ȡ�ù�Ʊ�ɽ��б�����
	const char  GetHoldListStock[] = "HoldStLs";						// ȡ�ù�Ʊ�ֲ��б�����
	const char  GetAccountListStock[] = "AccoStLs";						// ȡ�ù�Ʊ�ʽ��б�����
	const char  GetContractStockUpperTick[] = "UptiStLs";				// ȡ�ù�Ʊ��Լ��С�䶯��λ�б�����
	const char  OrderStockHK[] = "OrdeStHK";							// �۹��µ�������ָ��
	const char  OrderStockHK02[] = "OrdeSt02";							// ��Ʊ��̨ǿƽ������ָ��
	const char  OrderStockHK03[] = "OrdeSt03";							// ��Ʊ��̨�µ�������ָ��
	const char  SystemNoStockHK[] = "SystStHK";							// �۹�ϵͳ�ŷ���ָ��
	const char  ModifyStockHK[] = "ModiStHK";							// �۹ɸĵ�������ָ��
	const char  CancelStockHK[] = "CancStHK";							// �۹ɳ���������ָ��
	const char  CancelStockHKResponse[] = "CancStRe";					// ��̨��Ʊ��������ָ��
	const char  FilledStockHK[] = "FillStHK";							// �۹ɳɽ�����ָ��
	const char  AccountStockHK[] = "AccoStHK";							// �۹������ʽ𷵻�ָ��
	const char  StatusStockHK[] = "StatStHK";							// �۹����¶���״̬����ָ��
	const char  HoldStockHK[] = "HoldStHK";								// �۹����³ֲַ���ָ��
	const char  UpdateClient[] = "UpdtClet";							// ���¿ͻ��˰汾ָ��
	const char  TransferMoney[] = "TranMony";							// ��Ʊ�ڻ�֮���ʽ����ָ��
	const char  GetLoginHistoryList[] = "LogHisLs";						// ȡ���û��ϴε�¼��ͬʱ���ߵ�¼��Ip��ַ�б�
	const char  LogException[] = "LogExcep";							// ��¼�쳣log
	const char  GetServerInfoList[] = "ServList";						// ȡ�÷�������Ϣ�б�
	const char  GetMarketFilledInfoList[] = "GET_TRADE_LIST";			// ȡ�ú�Լ�������³ɽ���Ϣ�б�
	const char  SearchMSg[] = "SearchMSg";								// ��ѯ��Ϣ
	const char  GetPlateList[] = "GetPlatL";							// ȡ���ڻ���������б�
	const char  GetPlateListStockHK[] = "GetPlSHL";						// ȡ�ø۹ɰ�������б�
	const char  GetPlateListStockAM[] = "GetPlSAL";						// ȡ�ø����ɰ�������б�
	const char  GetPlateListStockCN[] = "GetPlSCL";						// ȡ�ù��ڹ�Ʊ��������б�
	const char  GetActiveOptionList[] = "GetAcOpL";						// ȡ�û�Ծ��Ȩ�����б�
	const char  GetStrategyList[] = "GetSratL";							// ȡ�ò��������б�
	const char  GetStrategyDetailList[] = "GetSrDtL";					// ȡ�ò�����ϸ�����б�
	const char  GetStrategyOrderSeqList[] = "GetSrOSL";					// ȡ�ò��Ե���ϸ�µ�˳�������б�
	const char  setStrategy[] = "SetSrate";								// ���ò���
	const char  setStrategyDetail[] = "SetSraDt";						// ���ò�����ϸ
	const char  delStrategy[] = "DelSrate";								// ɾ������
	const char  CheckStrategyOrder[] = "CheSraOd";						// У�Բ��Զ�������
	const char  StrategyOrderFillLeg[] = "FillLeg1";					// ���Զ�����������
	const char  StrategyTriggerInfo[] = "StraTrig";						// ���Դ�����Ϣ����
	const char  GetCommondityInfoList[] = "GetComLs";					// ȡ����Ʒ��Ϣ�б�
	const char  GetUserCommondityFeeList[] = "GetUsCoL";				// ȡ���û���Ʒ����ӯ����Ϣ�б�
	const char  GetStockHKBrokerList[] = "GetHKBrL";					// ȡ�ø۹ɾ�������Ϣ�б�
	const char  GetMainContractList[] = "GetMConL";						// ȡ���ڻ�������Լ��Ϣ�б�
	const char  AskMarketBrokerList[] = "BROKER";						// ����۹����龭�����ƺ��б�
	const char  GetExchangeTime[] = "GETETIME";							// ȡ�ý������Ŀ��̺�����ʱ��  2016-12-05
	const char  GetCommonTime[] = "GETCTIME";							// ȡ��Ʒ�Ŀ�����ʱ��
	const char  getSettlementInfo[] = "GetSettl";						// ȡ�ý��㵥����
	const char  getSettlementConfirm[] = "GetSeCon";					// ȡ�ý���ȷ����Ϣ
	const char  CreateAccount[] = "CratAcct";							// ����ָ��
	const char  UpdateClientContractData[] = "UpClCoDa";				// ����ʵʱ����ָ���û��ĺ�Լ���ݣ����¹�Ʊ�ĳֲ��޶
	const char  GetQuestions[] = "VfQuList";							// ��֤�����б���������
	const char  SafeVerify[] = "SfVerify";								// ��ȫ��֤����
	const char  SetVerifyQA[] = "SetVeriQA";							// �ܱ��������������
	const char  VerifyCode[] = "RequVfCd";								// �ֻ���֤�뷢������
	const char  ReqPublicKey[] = "REQ_PUBLIC_KEY";						// ��ȡRSA��Կ
	const char  ReplyPublicKey[] = "REPLY_PUBLIC_KEY";					// ����RSA��Կ
	const char  SendKey[] = "SEND_KEY";									// ���ͶԳƹ�Կ
	const char  ReplySendKey[] = "REPLY_SEND_KEY";						// ��Ӧ�Գƹ�Կ
	const char  GetBroker[] = "BROKER";									// ��ȡ������
	const char  ReplyBroker[] = "B";									// ��ȡ������
	const char  GetComdy[] = "GetComdy";								// ��ȡ��Ʒ�б�
	const char  QuerySettleInfoConfirm[] = "QUERYSETTLEINFOCONFIRM";	// ��ȡ���㵥
	const char  NetCheck[] = "NCV_netcheck";							// ������Ϣ
	const char  PasswdShow[] = "******";								// �����ֶ���log�ļ���ʾ����
};

