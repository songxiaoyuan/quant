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
	char instrumentID[31];
	double ratio;
}HedgeInfo;

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
	HedgeInfo hedgeInfo[6];

}Param;

typedef struct 
{
	int tradingSize;
	int hedgeSize;
	Param m_Param[6];
}Parameter;

typedef struct
{
	mdPrice pre_price;
	mdPrice cur_price;
	queue<double> prices_queue;
	map<double,int> prices_map;

	double pre_ema_val;

	int multiple;
	char direction;
	double max_profit;
	double open_price;

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
bool IsBandOpenTime(char direction,double lastprice,double middle,double sd,double openval,double limit_sd,double limit_sd_open_edge);
bool IsBandCloseTime(char direction,double lastprice,double middle,double sd
					 ,double loss_band,double profit_band,double rsival,double limit_rsi
					 ,double limit_sd,double limit_sd_loss_close_edge);

bool IsTriggerSizeOpenTime(char direction,mdPrice *now_price,mdPrice *pre_price,int multiple
				,double volume_edge,double openinterest_edge,double spread );
bool IsTriggerSizeCloseTime(char direction,mdPrice *now_price,mdPrice *pre_price,int multiple
				,double volume_edge,double openinterest_edge,int spread);

bool IsDownTime(mdPrice *now_price,mdPrice *pre_price,int spread,int multiple);
bool IsUpTime(mdPrice *now_price,mdPrice *pre_price,int spread,int multiple);
//根据现在的price的一个列表，计算列表里面的ma数据。
double GetMAData(vector<double> &vector_prices,int period);
// 根据现在的price的一个列表，计算列表里面的标准差
double GetSDData(vector<double> &vector_prices,int period);
double GetSDDataByMap(map<double,int> &map_prices,int period);
// 根据传入的这个lastprice，计算返回的ema的值。
double GetEMAData(double price,double pre_ema_val,int period);
//根据传入的当前的差值和保存差值的数据，以及rsi的周期，来求返回的rsi的值。
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
bool IsMaxDrawDown(char direction,double cur_lastprice,double open_price,int multiple,
				   double &max_profit,double limit_max_drawdown);

void StartAndStopFun(Parameter *param,VolumeTrendOther3Info *info,int param_index);
BandAndTriggerSizeRetStatus GetMdData(Parameter *param,VolumeTrendOther3Info *info,int param_index);

bool isOpenTime(double middle_val,double sd_val,Parameter *param,VolumeTrendOther3Info *info,int param_index);
bool isCloseTime(double middle_val,double sd_val,double rsi_val,Parameter *param,VolumeTrendOther3Info *info,int param_index);