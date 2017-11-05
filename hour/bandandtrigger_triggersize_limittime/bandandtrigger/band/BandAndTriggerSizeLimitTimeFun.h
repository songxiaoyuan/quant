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
}mdPrice_limittime;

typedef struct
{
	char instrumentID[31];
	double ratio;
}HedgeInfo_limittime;

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
	HedgeInfo_limittime hedgeInfo[6];

}Param_limittime;

typedef struct 
{
	int tradingSize;
	int hedgeSize;
	Param_limittime m_Param[6];
}Parameter_limittime;

typedef struct
{
	mdPrice_limittime pre_price;
	mdPrice_limittime cur_price;

	double pre_ema_val_60;
	double pre_ema_val_5;

	int now_middle5_bar_tick;

	int multiple;
	char direction;
	int open_status;

	vector<double> lastprice_vector;

	int has_open;
	int current_hour_open;
	int current_hour_line;

}VolumeTrendLimitTimeInfo;

typedef struct
{
	bool isTrendOpenTime;
	bool isTrendCloseTime;
}BandAndTriggerSizeLimitTimeRetStatus;

	// 判断是不是达到了布林带的开仓和平仓条件
bool IsLimitTimeOpenTime(Parameter_limittime *param,VolumeTrendLimitTimeInfo *info,int parm_index);
bool IsMiddleCrossCloseTime(VolumeTrendLimitTimeInfo *info,double lastprice,double middle_val_5,double cross_middle_edge);
bool IsBandOpenTime(char direction,double lastprice,double middle,double sd
					,double openval1,double openval2);
bool IsBandCloseTime(char direction,double lastprice,double middle,double sd
					 ,double loss_band,double profit_band,double rsi_data,double limit_rsi_data);

bool IsTriggerSizeOpenTime(char direction,mdPrice_limittime *now_price,mdPrice_limittime *pre_price,int multiple
				,double volume_edge,double openinterest_edge,double spread);
bool IsTriggerSizeCloseTime(char direction,mdPrice_limittime *now_price,mdPrice_limittime *pre_price,int multiple
				,double volume_edge,double openinterest_edge,int spread);

bool IsDownTime(mdPrice_limittime *now_price,mdPrice_limittime *pre_price,int spread,int multiple);
bool IsUpTime(mdPrice_limittime *now_price,mdPrice_limittime *pre_price,int spread,int multiple);
// 根据现在的price的一个列表，计算列表里面的标准差
double GetSDData(vector<double> &vector_prices,int period);
// 根据传入的这个lastprice，计算返回的ema的值。
double GetEMAData(double price,double pre_ema_val,int period);
//根据传入的当前的差值和保存差值的数据，以及rsi的周期，来求返回的rsi的值。
double GetRSIData(double tmpdiff,vector<double> &rsi_vector,int period);
void WriteMesgToFile(string path,string mesg);

inline string double2str(const double &int_temp);
inline double str2double(const string &string_temp);

void GetConfigInfo(double &pre_ema_val_60,double &pre_ema_val_5,
				   vector<double> &lastprice_vector,int config_file_path);
void WriteConfigInfo(double &pre_ema_val,queue<double> &lastprice_queue,vector<double> &ris_vector,
					 double rsi_period,int config_arbit_id);

void PrintInfo(double &pre_ema_val_60,double &pre_ema_val_5,
			   vector<double> &lastprice_vector,int config_file_path);

void StartAndStopFun(Parameter_limittime *param,VolumeTrendLimitTimeInfo *info,int param_index);
BandAndTriggerSizeLimitTimeRetStatus GetMdData(Parameter_limittime *param,VolumeTrendLimitTimeInfo *info,int param_index);

bool IsOpenTime(double middle_val_60,double middle_val_5,Parameter_limittime *param,VolumeTrendLimitTimeInfo *info,int param_index);
bool IsCloseTime(double middle_val,double sd_val,double rsi_val,double middle_val_5,
				 Parameter_limittime *param,VolumeTrendLimitTimeInfo *info,int param_index);

void GetOpenSignal(VolumeTrendLimitTimeInfo *info);
void GetCloseSignal(VolumeTrendLimitTimeInfo *info);