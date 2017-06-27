#ifndef ARBITRAGEVOLUMETRENDOTHER3_H
#define ARBITRAGEVOLUMETRENDOTHER3_H
#include "hyArbitrage_Interface.h"
#include "func.h"
#include "band/BandAndTriggerSizeFun.h"
using namespace std;

#define MaxPrice 100000
typedef struct
{
	mdPrice new_Price;
	mdPrice last_Price;
	vector<double> vector_prices_;
	double cur_lastprice_;
	double pre_ema_val_;
	double cur_middle_value_;
	double cur_sd_val_;
	double band_open_edge_;
	double band_profit_close_edge_;
	double band_loss_close_edge_;

	double cur_spread_price_val_;
	double limit_rsi_data_;
	double rsi_data_;
	double pre_rsi_lastprice_;
	int now_rsi_bar_tick_;
	vector<double> rsi_vector_;
	int rsi_bar_period_;
	int rsi_period_;
	BandAndTriggerSizePriceInfo pre_price_;
	BandAndTriggerSizePriceInfo cur_price_;

}VolumeTrendOther3Info;

class CHyArbitrageVolumeTrendOther3:public CHyArbitrageBase
{
public:
	CHyArbitrageVolumeTrendOther3(Parameter* Param):CHyArbitrageBase(Param){}
	~CHyArbitrageVolumeTrendOther3(){};
public:
	virtual bool get_fv(SThreadChannel *threadChannel,double &fv);
	virtual void clearVector();
	virtual double calculateLessPrice(SThreadChannel *threadChannel,char OffsetFlag,char Direction,double fv,int perside);

private:

	void cancelNotMatchOrder(SThreadChannel *threadChannel,STraderChannel* pTraderInfo);

	bool isOpenTrendTime(SThreadChannel *threadChannel);
	bool isCloseTrendTime(SThreadChannel *threadChannel);

private:
	VolumeTrendOther3Info m_volumeTrendOther3Info[MAX_PARAM_LENGTH];
};


#endif