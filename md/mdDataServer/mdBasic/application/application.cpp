#include "application.h"


application::application() {
  // ctor
	requestid_ = 1;
}

void application::ConfigFun() {
    //�˺�����Ҫ��������ȡһЩ������Ϣ����ǰ�õ�ַ�ȡ�ͨ����ȡconfig.txt�������Ϣ��
	string path = "config.txt";
	unordered_map<string,string> ret =  GetConfigInfo(path);
	MDFRONT=ChangeStrToChar(ret["MDFRONT"]);
	strcpy(APPID,ChangeStrToChar(ret["APPID"]));
	strcpy(USERID,ChangeStrToChar(ret["USERID"]));
	strcpy(PASSWD,ChangeStrToChar(ret["PASSWD"]));
	vector<string> instruments = split(ret["INSTRUMENT"],",");
	NUMOFINSTRUMENTID = instruments.size();
	//cout<<NUMOFINSTRUMENTID<<endl;
	PPINSTRUMENTID = new char* [NUMOFINSTRUMENTID];
	for(unsigned int   i=0;i<NUMOFINSTRUMENTID;i++){
		PPINSTRUMENTID[i] = ChangeStrToChar(instruments[i]);
	}
	
}

void application::Init() {

  //��ʼ��MDApi
  pMdApi = CThostFtdcMdApi::CreateFtdcMdApi();
  pMdSpi = new CtpMdSpi();  //�����ص����������MdSpi
  pMdApi->RegisterSpi(pMdSpi);        // �ص�����ע��ӿ���
  pMdApi->RegisterFront(MDFRONT);     // ע������ǰ�õ�ַ
  pMdApi->Init();                     //�ӿ��߳�����, ��ʼ����
}

void application::Run() {
 /*
	int i;
  cerr << "-----------------------------------------------" << endl;
  cerr << "[1] userLogin     "<<endl;
  cerr << "[2] userSubscribe "<< endl;
  cerr << "[3] Exit    " <<endl;
  cerr << "----------------------------------------------" << endl;
  */

  Sleep(1000);
  MdUserLogin(APPID, USERID, PASSWD);
  Sleep(1000);
  SubscribeMarketData(PPINSTRUMENTID, NUMOFINSTRUMENTID);
  /*
  while (1) {
    //cin >> i;
	  Sleep(1000);
    switch (i) {
      case 1: {
        //�û���¼��Ϣ������md��¼��trader��¼
        // MD�û���ʼ��¼
        MdUserLogin(APPID, USERID, PASSWD);
        break;
      }
      case 2: {
        // �û�������Ϣ����ʱ�̻߳���ݴ����������ݴ����ǲ�������ָ��
        // �������������ָ���Զ��µ���ͬʱ������ָ������ʱ���Զ����֡�
        SubscribeMarketData(PPINSTRUMENTID, NUMOFINSTRUMENTID);
        break;
      }
      case 3: {
        // �����˳������߿�������Ĳ�����
       // exit(0);
        break;
      }
    }
  }*/
  pMdApi->Join();
}

void application::MdUserLogin(TThostFtdcBrokerIDType appId,
                              TThostFtdcUserIDType userId,
                              TThostFtdcPasswordType passwd) {
  CThostFtdcReqUserLoginField req;
  memset(&req, 0, sizeof(req));
  strcpy(req.BrokerID, APPID);
  strcpy(req.UserID, userId);
  strcpy(req.Password, passwd);
  int ret = pMdApi->ReqUserLogin(&req, ++requestid_);
  //cerr << " req | send login..." << ((ret == 0) ? "success" : "fail") << endl;
}

void application::SubscribeMarketData(char* instIdList[], int len) {
  int ret = pMdApi->SubscribeMarketData(instIdList, len);
 // cerr << " req | send market md... " << ((ret == 0) ? "success" : "fail") << endl;
}


application::~application()
{
    //dtor
}

