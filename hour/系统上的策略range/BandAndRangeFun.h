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
}mdPrice_range;

typedef struct
{
	char instrumentID[31];
	double ratio;
}HedgeInfo_range;

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

	int cycle;
	int openCount;
	int openTick;
	int runTick;
	int lossTick;
	int profitTick;
	int fastCycle;
	int profitVolume;
	int profitCount;
	int timeGaps;
	double middleLine;
	int barTime;

	int parameter1;
	int parameter2;
	int parameter3;
	int parameter4;
	int parameter5;
	double parameter6;
	double parameter7;
	double parameter8;

	int hedgeInfoSize;
	HedgeInfo_range hedgeInfo[6];

}Param_range;

typedef struct 
{
	int tradingSize;
	int hedgeSize;
	Param_range m_Param[6];
}Parameter_range;

typedef struct
{
	mdPrice_range pre_price;
	mdPrice_range cur_price;

	double exit_quick_line;
	double pre_ema_val_5;

	int now_middle5_bar_tick;

	int multiple;
	double price_tick;
	double max_lastprice;
	double min_lastprice;
	char direction;

	vector<double> lastprice_vector_hour;
	vector<double> lastprice_vector_5minute;

	int open_status;
	int pre_open_status;
	int has_open;
	int current_minute;
	int current_minute_num_hour;
	int current_minute_num_5minute;

	int open_tick_num;
	int triggersize_series;

	double open_price;
	double max_profit;

}VolumeTrendRangeInfo;

typedef struct
{
	bool isTrendOpenTime;
	bool isTrendCloseTime;
}BandAndRangeRetStatus;

bool IsBandOpenTimeSeries(VolumeTrendRangeInfo* info,double lastprice,double middle,double middle_5minute,double open_edge1,double open_edge_increase);
bool IsBandCloseTimeSeries(VolumeTrendRangeInfo* info,double lastprice,double middle
					 ,double loss_band,double profit_band);

bool IsMiddleCrossCloseTime(VolumeTrendRangeInfo *info,double lastprice,double exit_quick_line);
bool IsLimitTimeOpenTimeSeries(Parameter_range *param,VolumeTrendRangeInfo *info,int param_index);
// 根据传入的这个lastprice，计算返回的ema的值。
double GetEMADataSeries(double price,double pre_ema_val,int period);
double GetMAData(double lastprice,vector<double> &lastprice_vector,int period);
bool IsCloseTime(double middle_val,double middle_val_5,Parameter_range *param,VolumeTrendRangeInfo *info,int param_index);
//根据传入的当前的差值和保存差值的数据，以及rsi的周期，来求返回的rsi的值。
double GetRSIDataSeries(double lastprice,vector<double> &lastprice_vector,int period);
void WriteMesgToFile(string path,string mesg);

inline string double2str(const double &int_temp);
inline double str2double(const string &string_temp);

void GetConfigInfoSeries(double &pre_ema_val_60,double &pre_ema_val_5,
				   vector<double> &lastprice_vector,int config_file_path);

void PrintInfo(double &pre_ema_val_60,double &pre_ema_val_5,
			   vector<double> &lastprice_vector,int config_file_path);

void StartAndStopFun(Parameter_range *param,VolumeTrendRangeInfo *info,int param_index);
BandAndRangeRetStatus GetMdData(Parameter_range *param,VolumeTrendRangeInfo *info,int param_index);

bool IsOpenTime(double middle_val,double middle_val_5,Parameter_range *param,VolumeTrendRangeInfo *info,int param_index);
bool IsCloseTime(double middle_val,double middle_val_5,Parameter_range *param,VolumeTrendRangeInfo *info,int param_index);

void GetOpenSignal(VolumeTrendRangeInfo *info);
void GetCloseSignal(VolumeTrendRangeInfo *info);