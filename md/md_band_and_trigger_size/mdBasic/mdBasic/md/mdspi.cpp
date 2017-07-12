#include "mdspi.h"
using namespace std;


void CtpMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                          bool bIsLast) {
  IsErrorRspInfo(pRspInfo);
}

void CtpMdSpi::OnFrontDisconnected(int nReason) {
  cerr << " return | break..."
       << " reason=" << nReason << endl;
}

void CtpMdSpi::OnHeartBeatWarning(int nTimeLapse) {
  cerr << " return | heart warming..."
       << " TimerLapse = " << nTimeLapse << endl;
}

void CtpMdSpi::OnFrontConnected() {
  cerr << " connect md ...success" << endl;
}


void CtpMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
                              CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                              bool bIsLast) {
  if (!IsErrorRspInfo(pRspInfo) && pRspUserLogin) {
    cerr << " ��Ӧ | ��¼�ɹ�...��ǰ������:" << pRspUserLogin->TradingDay
         << endl;
	band_and_trigger_size_ = BandAndTriggerSize();
	cerr <<"has init the band and trigger size"<<endl;
  }
}

void CtpMdSpi::OnRspSubMarketData(
    CThostFtdcSpecificInstrumentField *pSpecificInstrument,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  cerr << " ��Ӧ |  ���鶩��...�ɹ�" << endl;
}

void CtpMdSpi::OnRspUnSubMarketData(
    CThostFtdcSpecificInstrumentField *pSpecificInstrument,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  cerr << " ��Ӧ |  ����ȡ������...�ɹ�" << endl;
}

void CtpMdSpi::OnRtnDepthMarketData(
    CThostFtdcDepthMarketDataField *pDepthMarketData) {
  string InstrumentID = pDepthMarketData->InstrumentID;
  cout<<InstrumentID+","<<endl;
  //strToFile(InstrumentID);
 // double price = pDepthMarketData->LastPrice;
  //band_and_trigger_size_.getPrices(pDepthMarketData);

/*
  cout << pDepthMarketData->InstrumentID << endl;
  cout << pDepthMarketData->TradingDay << endl;
  cout << pDepthMarketData->LastPrice << endl;
  cout << pDepthMarketData->Volume << endl;
  cout << pDepthMarketData->BidPrice1 << endl;
  cout << pDepthMarketData->AskPrice1 << endl;
  */
}

bool CtpMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
  bool ret = ((pRspInfo) && (pRspInfo->ErrorID != 0));
  if (ret){
    cerr<<" ��Ӧ | "<<pRspInfo->ErrorMsg<<endl;
  }
  return ret;
}
