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
#include <stdlib.h>
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
}mdPrice_series;

typedef struct
{
	char instrumentID[31];
	double ratio;
}HedgeInfo_series;

typedef struct  
{
	int		apiType;
	char	investorId[32];
	int		arbitrageMode;
	int		arbitrageType;
	int		traderMode;
	int		arbitrageTypeID;
	char	userName[32];
	int 	HedgeFlag;
	bool	isTrader;
	char	instrumentTypeID[31];
	int bCloseYestoday;
	char	arbitrageTypeName[31];
	int groupID;
	bool bClose;

	int spread;
	double openEdge;
	double closeEdge;
	double EdgeAdj;
	int cancelEdge;
	int edgebWork;
	int orderDelay;
	int edgePrice;

	int compTheo;
	int compXave;
	int Theo;
	int index;
	int marketImprovement;
	int marketPriceAdj;

	int orderPerside;
	int orderSpacingOpen;
	int orderSpacingClose;
	int orderVolume;
	int orderMaxVolume;
	int orderLegMaxVolume;
	int PositionNormal;
	int volMultiplier;

	int maxDrawDown;
	int maxLoss;
	int lockLoss;
	int closeLoss;


	int profitStart;
	int profitStop;
	int lockAdj;

	int closeStart;
	int closeStop;
	int closeAdj;

	int PositionAdj;
	int AdjEmaFast;
	int AdjEmaSlow;

	int dayStopTime;
	int midStopTime;

	int hedgeInfoSize;
	HedgeInfo_series hedgeInfo[6];

}Param_series;

typedef struct 
{
	int tradingSize;
	int hedgeSize;
	Param_series m_Param[6];
}Parameter_series;

typedef struct
{
	mdPrice_series pre_price;
	mdPrice_series cur_price;

	double pre_ema_val_60;
	double pre_ema_val_5;
	double pre_ema_val_1;

	int now_middle5_bar_tick;
	int now_middle1_bar_tick;

	int multiple;
	char direction;
	double max_profit;
	double open_price;

	vector<double> lastprice_vector_hour;
	vector<int> diff_volume_vector;
	vector<double> lastprice_vector;

	int tmp_sum_diff_volume;
	int lastprice_bar_tick;

	int has_open;
	int current_hour_open;
	int current_hour_line;

}VolumeTrendSeriesInfo;

typedef struct
{
	bool isTrendOpenTime;
	bool isTrendCloseTime;
}BandAndSeriesRetStatus;

	// 判断是不是达到了布林带的开仓和平仓条件
bool IsDiffVolumeOpenTime(int tmp_sum_diff_volume,vector<int> &diff_volume_vector,int limit_multiple,int limit_large_period);
bool IsLastpriceOpenTime(char direction,double lastprice,vector<double> &lastprice_vector,int limit_large_period);
bool IsBandOpenTime(char direction,double lastprice,double middle,double open_edge1,double open_edge2);
bool IsBandCloseTime(char direction,double lastprice,double middle,double sd
					 ,double loss_band,double profit_band,double rsi_data,double limit_rsi_data);

double GetSDData(double lastprice,vector<double> &vector_prices,int period);
// 根据传入的这个lastprice，计算返回的ema的值。
double GetEMAData(double price,double pre_ema_val,int period);
//根据传入的当前的差值和保存差值的数据，以及rsi的周期，来求返回的rsi的值。
double GetRSIData(double lastprice,vector<double> &lastprice_vector,int period);
void WriteMesgToFile(string path,string mesg);

inline string double2str(const double &int_temp);
inline double str2double(const string &string_temp);

void GetConfigInfo(double &pre_ema_val_60,double &pre_ema_val_5,double &pre_ema_val_1,
				   vector<double> &lastprice_vector,int config_file_path);

void PrintInfo(double &pre_ema_val_60,double &pre_ema_val_5,double &pre_ema_val_1,
			   vector<double> &lastprice_vector,int config_file_path);
bool IsMaxDrawDown(char direction,double cur_lastprice,double open_price,int multiple,
				   double &max_profit,double limit_max_drawdown);

void StartAndStopFun(Parameter_series *param,VolumeTrendSeriesInfo *info,int param_index);
BandAndSeriesRetStatus GetMdData(Parameter_series *param,VolumeTrendSeriesInfo *info,int param_index);

bool IsOpenTime(double middle_val,Parameter_series *param,VolumeTrendSeriesInfo *info,int param_index);
bool IsCloseTime(double middle_val,double sd_val,double rsi_val,double middle_val_5,double middle_val_1,Parameter_series *param,VolumeTrendSeriesInfo *info,int param_index);

void GetOpenSignal(VolumeTrendSeriesInfo *info);
void GetCloseSignal(VolumeTrendSeriesInfo *info);