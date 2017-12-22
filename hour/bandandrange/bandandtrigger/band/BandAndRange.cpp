#include "BandAndRange.h"
int LASTPRICE = 4;
int VOLUME = 11;
int OPENINTEREST = 13;
int TURNONER = 12;
int BIDPRICE1 = 22;
int ASKPRICE1 =24;
int InstrumentID =1;
int TradingDay =0;
int UpdateTime =20;

BandAndTriggerSize::BandAndTriggerSize(void)
{
	now_interest_ = 0;
	limit_interest_ =1;
	clearVector();
}


BandAndTriggerSize::~BandAndTriggerSize(void)
{
	clearVector();
}

void BandAndTriggerSize::clearVector(){
	Parameter_range *param = &interface_param_;
	VolumeTrendRangeInfo *info = &Series_info_;

	memset(&info->pre_price,0,sizeof(mdPrice_range));
	memset(&info->cur_price,0,sizeof(mdPrice_range));

	//init the info 
	info->multiple = 10;
	info->direction = 'l';
	info->price_tick = 1;

	//init the param 
	int param_index =0;
	//bar tick
	param->m_Param[param_index].cycle= 60;
	//limit large period
	param->m_Param[param_index].openCount = 2;
	param->m_Param[param_index].openTick = 3;
	param->m_Param[param_index].runTick = 2;
	param->m_Param[param_index].lossTick = 2;
	param->m_Param[param_index].profitTick = 20;
	param->m_Param[param_index].fastCycle = 30;
	param->m_Param[param_index].profitVolume = 1000;
	param->m_Param[param_index].profitCount = 2;
	param->m_Param[param_index].timeGaps = 60;
	param->m_Param[param_index].middleLine = 0;
	param->m_Param[param_index].barTime = 5;
	param->m_Param[param_index].parameter1 = 20;
	param->m_Param[param_index].parameter2 = 0;
	param->m_Param[param_index].parameter3 = 20;

	param->m_Param[param_index].arbitrageTypeID = 320;

	StartAndStopFun(param,info,param_index);
}


void BandAndTriggerSize::getPrices(vector<string> &line_data){

	Parameter_range *param = &interface_param_;
	VolumeTrendRangeInfo *info = &Series_info_;

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
		return;
	}

	int param_index = 0;
	BandAndRangeRetStatus ret_status = GetMdData(param,info,param_index);

	if (ret_status.isTrendOpenTime && now_interest_ < limit_interest_)
	{
		cout<<info->cur_price.updateTime<<" open the once"<<endl;
		now_interest_ +=1;
		GetOpenSignal(info);
	}
	if (ret_status.isTrendCloseTime && now_interest_ >0 )
	{
		cout<<info->cur_price.updateTime<<" close once"<<endl;
		now_interest_ -=1;
		GetCloseSignal(info);
	}
	info->pre_price = info->cur_price;
}