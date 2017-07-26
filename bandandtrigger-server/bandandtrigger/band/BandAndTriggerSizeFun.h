#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <math.h>
#include <fstream>
#include <queue>
#include <map>
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
bool IsBandOpenTime(char direction,double lastprice,double middle,double sd,double openval,double limit_sd,double limit_sd_open_edge);
bool IsBandCloseTime(char direction,double lastprice,double middle,double sd
					 ,double loss_band,double profit_band,double rsival,double limit_rsi
					 ,double limit_sd,double limit_sd_loss_close_edge);

bool IsTriggerSizeOpenTime(char direction,BandAndTriggerSizePriceInfo *now_price,BandAndTriggerSizePriceInfo *pre_price
				,double volume_edge,double openinterest_edge,int spread );
bool IsTriggerSizeCloseTime(char direction,BandAndTriggerSizePriceInfo *now_price,BandAndTriggerSizePriceInfo *pre_price
				,double volume_edge,double openinterest_edge,int spread);

bool IsDownTime(BandAndTriggerSizePriceInfo *now_price,BandAndTriggerSizePriceInfo *pre_price,int spread);
bool IsUpTime(BandAndTriggerSizePriceInfo *now_price,BandAndTriggerSizePriceInfo *pre_price,int spread);
//�������ڵ�price��һ���б������б������ma���ݡ�
double GetMAData(vector<double> &vector_prices,int period);
// �������ڵ�price��һ���б������б�����ı�׼��
double GetSDData(vector<double> &vector_prices,int period);
double GetSDDataByMap(map<double,int> &map_prices,int period);
// ���ݴ�������lastprice�����㷵�ص�ema��ֵ��
double GetEMAData(double price,double pre_ema_val,int period);
//���ݴ���ĵ�ǰ�Ĳ�ֵ�ͱ����ֵ�����ݣ��Լ�rsi�����ڣ����󷵻ص�rsi��ֵ��
double GetRSIData(double tmpdiff,vector<double> &rsi_vector,int period);
void WriteMesgToFile(string path,string mesg);
void WriteMesgToFileSO(string path,string mesg);

inline string double2str(const double &int_temp);
inline double str2double(const string &string_temp);

void GetConfigInfo(double &pre_ema_val,queue<double> &lastprice_queue,map<double,int> &lastprice_map,
				   vector<double> &rsi_vector,double &pre_rsi,int config_arbit_id);
void WriteConfigInfo(double &pre_ema_val,queue<double> &lastprice_queue,vector<double> &ris_vector,
					 double rsi_period,double pre_rsi,int config_arbit_id);

void printInfo(double &pre_ema_val,queue<double> &lastprice_queue,map<double,int> &lastprice_map,
			   vector<double> &rsi_vector,double &pre_rsi,int config_file_path);
bool IsMaxDrawDown(char direction,double cur_lastprice,double open_price,int multiple,double &max_profit,double limit_max_drawdown);