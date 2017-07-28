﻿#include "BandAndTriggerSize.h"
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

	//init the param 
	int param_index =0;
	//spread
	param->m_Param[param_index].spread = 100;
	//diff_volume
	param->m_Param[param_index].openEdge = 100;
	//open interest
	param->m_Param[param_index].index = 0;
	//period
	param->m_Param[param_index].compXave = 7200;

	//band_open_noraml
	param->m_Param[param_index].PositionAdj = 5;
	//band_loss_close_edge
	param->m_Param[param_index].AdjEmaFast = 10;
	//band_profit_close_edge
	param->m_Param[param_index].AdjEmaSlow = 50;

	//rsi_bar_period
	param->m_Param[param_index+1].PositionAdj = 100;
	//rsi_period
	param->m_Param[param_index+1].AdjEmaFast = 10;
	//limit_rsi_data
	param->m_Param[param_index+1].AdjEmaSlow =80;

	//limit_sd
	param->m_Param[param_index+1].spread =4;
	//limit_sd_open_edge
	param->m_Param[param_index+1].openEdge=10;
	//limit_sd_loss_close_edge
	param->m_Param[param_index+1].closeEdge=20;

	//limit_max_draw_down
	param->m_Param[param_index+1].maxDrawDown = 100;

	param->m_Param[param_index].arbitrageTypeID = 320;

	//init the info 
	info->multiple = 10;
	info->direction = 'l';

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
	sprintf(info->cur_price.updateTime,"%s",line_data[UpdateTime]);
	info->multiple = 10;

	//is the first data in the server this is not needed
	if (info->pre_price.LastPrice ==0)
	{
		info->pre_price = info->cur_price;
		info->pre_rsi_lastprice = info->cur_price.LastPrice;
		return;
	}

	int param_index = 0;
	BandAndTriggerSizeRetStatus ret_status = GetMdData(param,info,param_index);
}