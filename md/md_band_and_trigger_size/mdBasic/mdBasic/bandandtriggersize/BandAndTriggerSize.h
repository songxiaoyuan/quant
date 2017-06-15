#pragma once
#include "../api/ThostFtdcMdApi.h"
#include "../basicFun/basicFun.h"
#include <vector>
#include <math.h>
#include <string>
#include <string.h>
#include <iostream>
using namespace std;
class BandAndTriggerSize
{
public:
	BandAndTriggerSize(void);
	~BandAndTriggerSize(void);
	void getPrices(CThostFtdcDepthMarketDataField *pDepthMarketData);
private:
	// 判断是不是达到了布林带的开仓和平仓条件
	bool IsBandCloseTime();
	bool IsBandOpenTime();
	//根据现在的price的一个列表，计算列表里面的ma数据。
	double GetMAData(vector<double> &prices);
	// 根据现在的price的一个列表，计算列表里面的标准差
	double GetSDData(vector<double> &prices);
	// 根据传入的这个lastprice，计算返回的ema的值。
	double GetEMAData(double price);

	bool IsDownTime(double edge);
	bool IsUpTime(double edge);

	bool IsTriggerSizeOpenTime();
	bool IsTriggerSizeCloseTime();

private:
	vector<double> vector_prices_;
	double cur_lastprice_;
	double last_mea_val_;
	int current_ema_tick_num_;
	double band_open_edge_;
	double band_close_edge_;
	double cur_middle_value_;
	double cur_sd_val_;
	double price_spread_edge_;
	int position_;
	int compXaveNum_;
	int volume_edge_;
	int openinterest_edge_;
	int multiple_val_;
	CThostFtdcDepthMarketDataField pre_price_;
	CThostFtdcDepthMarketDataField cur_price_;
	string direction_;
	string movingTheo_;
};

