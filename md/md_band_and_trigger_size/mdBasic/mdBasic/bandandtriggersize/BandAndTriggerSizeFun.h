#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <string.h>
#include <math.h>
#include <fstream>
using namespace std;

typedef struct
{
	double Volume;
	double OpenInterest;
	double Turnover;
	double AskPrice1;
	double LastPrice;
	double BidPrice1;
	int multiple;
}BandAndTriggerSizePriceInfo;

	// �ж��ǲ��Ǵﵽ�˲��ִ��Ŀ��ֺ�ƽ������
bool IsBandOpenTime(char direction,double lastprice,double middle,double sd,double openval);
bool IsBandCloseTime(char direction,double lastprice,double middle,double sd,double loss_band,double profit_band,double rsival,double limit_rsi);

bool IsTriggerSizeOpenTime(char direction,BandAndTriggerSizePriceInfo *now_price,BandAndTriggerSizePriceInfo *pre_price
				,double volume_edge,double openinterest_edge,int spread );
bool IsTriggerSizeCloseTime(char direction,BandAndTriggerSizePriceInfo *now_price,BandAndTriggerSizePriceInfo *pre_price
				,double volume_edge,double openinterest_edge,int spread);

bool IsDownTime(BandAndTriggerSizePriceInfo *now_price,BandAndTriggerSizePriceInfo *pre_price,int spread);
bool IsUpTime(BandAndTriggerSizePriceInfo *now_price,BandAndTriggerSizePriceInfo *pre_price,int spread);
//�������ڵ�price��һ���б��������б������ma���ݡ�
double GetMAData(vector<double> &vector_prices,int period);
// �������ڵ�price��һ���б��������б�����ı�׼��
double GetSDData(vector<double> &vector_prices,int period);
// ���ݴ�������lastprice�����㷵�ص�ema��ֵ��
double GetEMAData(double price,double pre_ema_val,int period);
//���ݴ���ĵ�ǰ�Ĳ�ֵ�ͱ����ֵ�����ݣ��Լ�rsi�����ڣ����󷵻ص�rsi��ֵ��
double GetRSIData(double tmpdiff,vector<double> &rsi_vector,int period);
void WriteMesgToFile12(string path,string mesg);