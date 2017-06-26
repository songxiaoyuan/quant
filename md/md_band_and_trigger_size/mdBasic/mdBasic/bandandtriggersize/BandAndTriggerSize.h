#pragma once
#include "../api/ThostFtdcMdApi.h"
#include "../basicFun/basicFun.h"
#include "BandAndTriggerSizeFun.h"
#include <vector>
#include <math.h>
#include <string>
#include <string.h>
#include <iostream>
#include <unordered_map>
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

	double limit_rsi_data_;
	double rsi_data_;
	double pre_rsi_lastprice_;
	int now_rsi_bar_tick_;
	vector<double> rsi_vector_;
	int rsi_bar_period_;
	int rsi_period_;

	double pre_ema_val_;
	CThostFtdcDepthMarketDataField pre_price_;
	CThostFtdcDepthMarketDataField cur_price_;
	string direction_;
	string movingTheo_;
};

