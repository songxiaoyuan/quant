#pragma once
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
	void getPrices(vector<string> &line_data);
private:
	bool IsTrendCloseTime();
	bool IsTrendOpenTime();

private:
	vector<double> vector_prices_;
	double cur_lastprice_;
	double pre_ema_val_;
	int current_ema_tick_num_;
	double band_open_edge_;
	double band_profit_close_edge_;
	double band_loss_close_edge_;
	double cur_middle_value_;
	double cur_sd_val_;
	double price_spread_edge_;
	int position_;
	int compXaveNum_;
	int volume_edge_;
	int openinterest_edge_;
	int multiple_val_;

	double limit_rsi_val_;
	double rsi_val_;
	double pre_rsi_lastprice_;
	int now_rsi_bar_tick_;
	vector<double> rsi_vector_;
	int rsi_bar_period_;
	int rsi_period_;

	int now_interest_;
	int limit_interest_;

	double max_profit_;
	double open_price_;
	double limit_max_draw_down_;


	BandAndTriggerSizePriceInfo pre_price_;
	BandAndTriggerSizePriceInfo cur_price_;
	string direction_;
	string movingTheo_;
	string time_;
};

