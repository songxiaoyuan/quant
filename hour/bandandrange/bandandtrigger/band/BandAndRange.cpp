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
	param->m_Param[param_index].spread = 10;
	//limit large period
	param->m_Param[param_index].openEdge = 3;
	param->m_Param[param_index].closeEdge = 2;
	
	//rsi_bar_period
	param->m_Param[param_index].PositionAdj = 20;
	//rsi_period
	param->m_Param[param_index].AdjEmaFast = 14;
	//limit_rsi_data
	param->m_Param[param_index].AdjEmaSlow =70;

	
	//limit multiple bigger
	param->m_Param[param_index+1].spread =2;
	//band open egde start the bigger 10 beishu,base the tick
	param->m_Param[param_index+1].openEdge=3;
	//band open edge start2
	param->m_Param[param_index+1].closeEdge=2;
	//band loss edge close
	param->m_Param[param_index+1].EdgeAdj=0;
	//band profit close edge close
	param->m_Param[param_index+1].cancelEdge = 25;
	//small the limit sd
	param->m_Param[param_index+1].edgebWork = 60;
	param->m_Param[param_index+1].compXave = 1000;

	param->m_Param[param_index+1].PositionAdj = 60;


	//limit_max_draw_down
	param->m_Param[param_index+1].maxDrawDown = 0;

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