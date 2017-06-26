#ifndef ARBITRAGEVOLUMETRENDOTHER3_H
#define ARBITRAGEVOLUMETRENDOTHER3_H

#include "hyArbitrage_Interface.h"
#include "func.h"
#include "BandAndTriggerSizeFun.h"
using namespace std;

#define MaxPrice 100000
class CHyArbitrageVolumeTrendOther3:public CHyArbitrageBase
{
public:
	CHyArbitrageVolumeTrendOther3(Parameter* Param):CHyArbitrageBase(Param){}
	~CHyArbitrageVolumeTrendOther3(){};
public:
	virtual bool get_fv_less(double &fv);
	virtual void clearVector();
	virtual double calculateEmaFvAdj();
	virtual double calculateLessPrice(char OffsetFlag,char Direction,double fv,int perside);
	virtual void   closeTraded(char direction,double price);
	virtual void   openTraded(char direction,double price);

private:

	void cancelNotMatchOrder(STraderChannel* pTraderInfo);

	bool isOpenTrendTime();
	bool isCloseTrendTime();

	bool isUpTime(double edge);
	bool isDownTime(double edge);


	// 判断是不是达到了布林带的开仓和平仓条件
	bool IsBandCloseTime();
	bool IsBandOpenTime();

private:

	mdPrice new_Price;
	mdPrice last_Price;
	vector<double> vector_prices_;
	double cur_lastprice_;
	double last_ema_val_;
	double cur_middle_value_;
	double cur_sd_val_;

	double band_open_edge_;
	double band_profit_close_edge_;
	double band_loss_close_edge_;


	double limit_rsi_data_;
	double rsi_data_;
	double pre_rsi_lastprice_;
	int now_rsi_bar_tick_;
	vector<double> rsi_vector_;
	int rsi_bar_period_;
	int rsi_period_;

};


#endif