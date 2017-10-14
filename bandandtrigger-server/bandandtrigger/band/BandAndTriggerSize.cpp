#include "BandAndTriggerSize.h"
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
	Parameter *param = &interface_param_;
	VolumeTrendOther3Info *info = &TriggerSize3_info_;

	memset(&info->pre_price,0,sizeof(mdPrice));
	memset(&info->cur_price,0,sizeof(mdPrice));

	//init the info 
	info->multiple = 10;
	info->direction = 's';
	info->now_rsi_bar_tick =0;
	info->max_profit =0;
	info->open_price = 0;

	//init the param 
	int param_index =0;
	//spread
	param->m_Param[param_index].spread = 100;
	//diff_volume
	param->m_Param[param_index].openEdge = 500;
	//open interest
	param->m_Param[param_index].index = 0;
	//period
	param->m_Param[param_index].compXave = 7200;
	
	//rsi_bar_period
	param->m_Param[param_index].PositionAdj = 120;
	//rsi_period
	param->m_Param[param_index].AdjEmaFast = 14;
	//limit_rsi_data
	param->m_Param[param_index].AdjEmaSlow =80;

	//tick num
	param->m_Param[param_index+1].spread =1;
	//band open edge start
	param->m_Param[param_index+1].openEdge=0;
	//band open edge close
	param->m_Param[param_index+1].closeEdge=5;
	//band loss close edge close
	param->m_Param[param_index+1].EdgeAdj=5;
	//band profit close edge
	param->m_Param[param_index+1].cancelEdge = 50;

	//small the limit sd
	param->m_Param[param_index+1].edgebWork = 100;
	//band_open bigger edge
	param->m_Param[param_index+1].orderDelay = 10;
	//band_close bigger edge
	param->m_Param[param_index+1].edgePrice = 10;


	//limit_max_draw_down
	param->m_Param[param_index+1].maxDrawDown = 0;

	param->m_Param[param_index].arbitrageTypeID = 320;

	StartAndStopFun(param,info,param_index);
}


void BandAndTriggerSize::getPrices(vector<string> &line_data){

	Parameter *param = &interface_param_;
	VolumeTrendOther3Info *info = &TriggerSize3_info_;

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
	BandAndTriggerSizeRetStatus ret_status = GetMdData(param,info,param_index);

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