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
	char updateTime[9];
	double	LastPrice;
	int  Volume;
	int  TotalVolume;
	double	BidPrice1;
	int	BidVolume1;
	double	AskPrice1;
	int	AskVolume1;
	double	BidPrice2;
	int	BidVolume2;
	double	AskPrice2;
	int	AskVolume2;
	double	BidPrice3;
	int	BidVolume3;
	double	AskPrice3;
	int	AskVolume3;
	double	BidPrice4;
	int	BidVolume4;
	double	AskPrice4;
	int	AskVolume4;
	double	BidPrice5;
	int	BidVolume5;
	double	AskPrice5;
	int	AskVolume5;
	double Turnover;
	double OpenInterest;
}mdPrice;


typedef struct  
{

	//trigger size param
	double		diff_volume;
	double 	spread;
	double 		openInterest;


	//bolling param
	int		period;
	double		openEdgeBegin;
	double		openEdgeEnd;
	double 	lossCloseEdge;
	double 	profitCloseEdge;
	double limitSD;
	double limitSDOpenEdgeEnd;
	double limitSDLossCloseEdge;
	int smaWeight;

	//RSI param
	double 	limitRSI;
	int 	RSIBarPeriod;
	int RSIPeriod;

}Param;


typedef struct
{
	mdPrice pre_price;
	mdPrice cur_price;
	queue<double> prices_queue;
	map<double,int> prices_map;

	double pre_sma_val;

	int multiple;
	char direction;

	double pre_rsi_lastprice;
	int now_rsi_bar_tick;
	vector<double> rsi_vector;

}VolumeTrendOther3Info;

typedef struct
{
	bool isTrendOpenTime;
	bool isTrendCloseTime;
}BandAndTriggerSizeRetStatus;

	// 判断是不是达到了布林带的开仓和平仓条件
bool IsBandOpenTime(char direction,double lastprice,double middle,double sd
					,double openval1,double openval2,double limit_sd,double limit_sd_open_edge);
bool IsBandCloseTime(char direction,double lastprice,double middle,double sd
					 ,double loss_band,double profit_band,double rsival,double limit_rsi
					 ,double limit_sd,double limit_sd_loss_close_edge);

bool IsTriggerSizeOpenTime(char direction,double limit_diff_volume,double limit_diff_openinterest,double limit_spread
				,mdPrice *now_price,mdPrice *pre_price,int multiple);
bool IsTriggerSizeCloseTime(char direction,mdPrice *now_price,mdPrice *pre_price,int multiple
				,double volume_edge,double openinterest_edge,double spread);

bool IsDownTime(mdPrice *now_price,mdPrice *pre_price,int spread,int multiple);
bool IsUpTime(mdPrice *now_price,mdPrice *pre_price,int spread,int multiple);

double GetSDDataByMap(map<double,int> &map_prices,int period);
// 根据传入的这个lastprice，计算返回的ema的值。
double GetSMAData(double price,double pre_ema_val,int period,int weight);
//根据传入的当前的差值和保存差值的数据，以及rsi的周期，来求返回的rsi的值。
double GetRSIData(double tmpdiff,vector<double> &rsi_vector,int period);


void StartAndStopFun(Param *param,VolumeTrendOther3Info *info);
BandAndTriggerSizeRetStatus GetMdData(Param *param,VolumeTrendOther3Info *info);

bool IsOpenTime(double middle_val,double sd_val,Param *param,VolumeTrendOther3Info *info);
bool IsCloseTime(double middle_val,double sd_val,double rsi_val,Param *param,
				 VolumeTrendOther3Info *info);

void GetOpenSignal(VolumeTrendOther3Info *info);
void GetCloseSignal(VolumeTrendOther3Info *info);