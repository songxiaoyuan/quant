#include "application.h"


application::application() {
  // ctor
	requestid_ = 1;
}

void application::ConfigFun() {
    //此函数主要是用来获取一些配置信息包括前置地址等。通过读取config.txt来获得信息。
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

  //初始化MDApi
  pMdApi = CThostFtdcMdApi::CreateFtdcMdApi();
  pMdSpi = new CtpMdSpi();  //创建回调处理类对象MdSpi
  pMdApi->RegisterSpi(pMdSpi);        // 回调对象注入接口类
  pMdApi->RegisterFront(MDFRONT);     // 注册行情前置地址
  pMdApi->Init();                     //接口线程启动, 开始工作
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
        //用户登录信息，包括md登录和trader登录
        // MD用户开始登录
        MdUserLogin(APPID, USERID, PASSWD);
        break;
      }
      case 2: {
        // 用户订阅信息，此时线程会根据传回来的数据处理是不是乌龙指，
        // 如果发现是乌龙指会自动下单，同时在乌龙指结束的时候自动锁仓。
        SubscribeMarketData(PPINSTRUMENTID, NUMOFINSTRUMENTID);
        break;
      }
      case 3: {
        // 程序退出，或者可以做别的操作。
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

