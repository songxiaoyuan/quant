#pragma once
#include "../md/mdspi.h"
#include "../basicFun/basicFun.h"
#include <iostream>
#include <Windows.h>

class application {
 public:
  application();
  //Ӧ�õ�������Ϣ�����ò���ǰ�õ�ַ���û����Ȼ�����Ϣ��
  void ConfigFun();
  // Ӧ�ó�ʼ������
  void Init();
  // Ӧ�����к���
  void Run();
  // �����û����е�¼
  void MdUserLogin(TThostFtdcBrokerIDType appId, TThostFtdcUserIDType userId,
                   TThostFtdcPasswordType passwd);
  // ���Ĺ��ĵĺ�Լ����
  void SubscribeMarketData(char* instIdList[], int len);

  virtual ~application();

 protected:
 private:
  int requestid_;
   //�û���¼��Ҫ��ǰ�õ�ַ
	char* MDFRONT;

//�û���¼��Ҫ����Ϣ��
	TThostFtdcBrokerIDType APPID;
	TThostFtdcUserIDType USERID;
	TThostFtdcPasswordType PASSWD;

//���ĵ������б�
	char** PPINSTRUMENTID;
	int NUMOFINSTRUMENTID;
  // �����������ʱ��md��spi��
   CtpMdSpi* pMdSpi;

  //�����������ʱ��md api
  CThostFtdcMdApi* pMdApi;
};

