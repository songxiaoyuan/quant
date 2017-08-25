#include "BandAndTriggerSizeFade.h"
int LASTPRICE = 4;
int VOLUME = 11;
int OPENINTEREST = 13;
int TURNONER = 12;
int BIDPRICE1 = 22;
int ASKPRICE1 =24;
int InstrumentID =1;
int TradingDay =0;
int UpdateTime =20;

BandAndTriggerSizeFade::BandAndTriggerSizeFade(void)
{
	now_interest_ = 0;
	limit_interest_ =3;
	clearVector();
}


BandAndTriggerSizeFade::~BandAndTriggerSizeFade(void)
{
	clearVector();
}

void BandAndTriggerSizeFade::clearVector(){
	Parameter_Fade *param = &interface_param_;
	VolumeTrendOtherFadeInfo *info = &TriggerSizeFade_info_;

	memset(&info->pre_price,0,sizeof(mdPrice_Fade));
	memset(&info->cur_price,0,sizeof(mdPrice_Fade));

	//init the info 
	info->multiple = 10;
	info->direction = 's';
	info->now_rsi_bar_tick =0;
	info->max_profit =0;
	info->open_price = 0;
	info->now_interest = 0;

	//init the param 
	int param_index =0;
	//spread
	param->m_Param[param_index].spread = 30;
	//diff_volume
	param->m_Param[param_index].openEdge = 1;
	//open interest
	param->m_Param[param_index].closeEdge = 2;
	//period
	param->m_Param[param_index].compXave = 7200;
	
	//rsi_bar_period
	param->m_Param[param_index].PositionAdj = 120;
	//rsi_period
	param->m_Param[param_index].AdjEmaFast = 14;
	//limit_rsi_data
	param->m_Param[param_index].AdjEmaSlow =80;


	//limit_max_draw_down
	param->m_Param[param_index+1].maxDrawDown = 200;

	param->m_Param[param_index].arbitrageTypeID = 320;

	StartAndStopFun(param,info,param_index);
}


void BandAndTriggerSizeFade::getPrices(vector<string> &line_data){

	Parameter_Fade *param = &interface_param_;
	VolumeTrendOtherFadeInfo *info = &TriggerSizeFade_info_;

	//init the pre price and cur price
	info->cur_price.LastPrice = atof(line_data[LASTPRICE].c_str());
	info->cur_price.Volume = atof(line_data[VOLUME].c_str());
	info->cur_price.OpenInterest = atof(line_data[OPENINTEREST].c_str());
	info->cur_price.AskPrice1 = atof(line_data[ASKPRICE1].c_str());
	info->cur_price.BidPrice1 = atof(line_data[BIDPRICE1].c_str());
	info->cur_price.Turnover = atof(line_data[TURNONER].c_str());
	sprintf(info->cur_price.updateTime,"%s",line_data[UpdateTime].c_str());
	

	//is the first data in the server this is not needed
	if (info->pre_price.LastPrice ==0)
	{
		info->pre_price = info->cur_price;
		info->pre_rsi_lastprice = info->cur_price.LastPrice;
		return;
	}

	int param_index = 0;
	BandAndTriggerSizeFadeRetStatus ret_status = GetMdData(param,info,param_index);

	if (ret_status.isTrendOpenTime && now_interest_ < limit_interest_)
	{
		cout<<info->cur_price.updateTime<<" open the once"<<endl;
		now_interest_ +=1;
		GetOpenSignal(info);
	}
	if (ret_status.isTrendCloseTime && now_interest_ >0 )
	{
		cout<<info->cur_price.updateTime<<" close once"<<endl;
		now_interest_ =0;
		GetCloseSignal(info);
	}
	info->pre_price = info->cur_price;
}