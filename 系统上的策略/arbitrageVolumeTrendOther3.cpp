#include "arbitrageVolumeTrendOther3.h"

void CHyArbitrageVolumeTrendOther3::clearVector()
{
	for (int i=0;i<MAX_PARAM_LENGTH;i++)
	{
		VolumeTrendOther3Info * info	=	&m_volumeTrendOther3Info[i];
		memset(&info->new_Price,0,sizeof(mdPrice));
		memset(&info->last_Price,0,sizeof(mdPrice));

		memset(&info->last_Price,0,sizeof(pre_price_));
		memset(&info->last_Price,0,sizeof(cur_price_));

		info->vector_prices_.clear();
		info->rsi_vector_.clear();
		info->cur_lastprice_	==	0;
		info->pre_ema_val_	==	0;
		info->band_open_edge_	==	0;
		info->band_profit_close_edge_	==	0;
		info->band_loss_close_edge_	==	0;
		info->cur_middle_value_	==	0;
		info->cur_sd_val_	==	0;
		info->cur_spread_price_val_	==	0;
		info->rsi_data_	==	0;
		info->pre_rsi_lastprice_	==	0;
		info->now_rsi_bar_tick_	==	0;

		string path = "bandandtriggersizeconfig.txt";
		vector<string> ret =  GetConfigInfo(path);
	
		info->rsi_bar_period_ = atoi(ret[0].c_str());
		info->rsi_period_ = atoi(ret[1].c_str());
		info->limit_rsi_data_ = atoi(ret[2].c_str());
	}
}
double CHyArbitrageVolumeTrendOther3::calculateLessPrice(SThreadChannel *threadChannel,char OffsetFlag,char Direction,double fv,int perside)
{
	double price=0;
	if (Direction	==	THOST_FTDC_D_Buy)				//买
	{
		price	=	0;
	}
	else if(Direction	==	THOST_FTDC_D_Sell)			//卖
	{
		price	=	MaxPrice;
	}
	return price;


}

bool CHyArbitrageVolumeTrendOther3::get_fv(SThreadChannel *threadChannel,double &fv)
{
	int md_index	=	threadChannel->md_index;
	int param_index	=	threadChannel->param_index;
	int trader_open_index	=	threadChannel->Trader_index_1;
	int trader_close_index	=	threadChannel->Trader_index_2;

	VolumeTrendOther3Info *volumeTrendInfo	=	&m_volumeTrendOther3Info[param_index];


	getNewPrice(g_arrChannel,md_index,volumeTrendInfo->new_Price);
	getLastPrice(g_arrChannel,md_index,volumeTrendInfo->last_Price);

	if (volumeTrendInfo->new_Price.Volume	==0	||	volumeTrendInfo->new_Price.OpenInterest	==0	
		||	volumeTrendInfo->new_Price.Turnover	==0||	volumeTrendInfo->new_Price.LastPrice	==	0
		||volumeTrendInfo->last_Price.Volume	==0	||	volumeTrendInfo->last_Price.OpenInterest	==0	
		||	volumeTrendInfo->last_Price.Turnover	==0||	volumeTrendInfo->last_Price.LastPrice	==	0)
	{
		return false;
	}

	cur_price_.Volume = volumeTrendInfo->new_Price.Volume;
	cur_price_.OpenInterest = volumeTrendInfo->new_Price.OpenInterest;
	cur_price_.Turnover = volumeTrendInfo->new_Price.Turnover;
	cur_price_.LastPrice = volumeTrendInfo->new_Price.LastPrice;
	cur_price_.AskPrice1 = volumeTrendInfo->new_Price.AskPrice1;
	cur_price_.BidPrice1 = volumeTrendInfo->new_Price.BidPrice1;
	cur_price_.multiple = getVolumeMultiple(g_arrChannel,md_index);

	pre_price_.Volume = volumeTrendInfo->last_Price.Volume;
	pre_price_.OpenInterest = volumeTrendInfo->last_Price.OpenInterest;
	pre_price_.Turnover = volumeTrendInfo->last_Price.Turnover;
	pre_price_.LastPrice = volumeTrendInfo->last_Price.LastPrice;
	pre_price_.AskPrice1 = volumeTrendInfo->last_Price.AskPrice1;
	pre_price_.BidPrice1 = volumeTrendInfo->last_Price.BidPrice1;
	pre_price_.multiple = getVolumeMultiple(g_arrChannel,md_index);


	volumeTrendInfo->band_open_edge_ = ((double)param->m_Param[param_index].PositionAdj)/10;
	volumeTrendInfo->band_loss_close_edge_ = ((double)param->m_Param[param_index].AdjEmaFast)/10;
	volumeTrendInfo->band_profit_close_edge_ = ((double)param->m_Param[param_index].AdjEmaSlow)/10;

	// 此部分代码主要是用来保存计算cur_middle_value_和sd的price。
	volumeTrendInfo->cur_lastprice_ = volumeTrendInfo->new_Price.LastPrice;

	if (volumeTrendInfo->pre_rsi_lastprice_ ==0)
	{
		volumeTrendInfo->pre_rsi_lastprice_ =volumeTrendInfo->cur_lastprice_;
	}
	if (volumeTrendInfo->now_rsi_bar_tick_ >= volumeTrendInfo->rsi_bar_period_)
	{
		double tmpdiff = volumeTrendInfo->cur_lastprice_ - volumeTrendInfo->pre_rsi_lastprice_;
		volumeTrendInfo->pre_rsi_lastprice_ = volumeTrendInfo->cur_lastprice_;
		volumeTrendInfo->now_rsi_bar_tick_ =1;
		volumeTrendInfo->rsi_data_ = GetRSIData(tmpdiff,volumeTrendInfo->rsi_vector_,volumeTrendInfo->rsi_period_);
		volumeTrendInfo->rsi_vector_.push_back(tmpdiff);
	}
	else{
		volumeTrendInfo->now_rsi_bar_tick_ +=1;
		double tmpdiff = volumeTrendInfo->cur_lastprice_ - volumeTrendInfo->pre_rsi_lastprice_;
		volumeTrendInfo->rsi_data_ = GetRSIData(tmpdiff,volumeTrendInfo->rsi_vector_,volumeTrendInfo->rsi_period_);
	}

	if (volumeTrendInfo->vector_prices_.size() < param->m_Param[param_index].compXave)
	{
		volumeTrendInfo->vector_prices_.push_back(volumeTrendInfo->cur_lastprice_);
		//double tmp = GetEMAData(param_index,volumeTrendInfo->cur_lastprice_);
		volumeTrendInfo->pre_ema_val_ = GetEMAData(volumeTrendInfo->cur_lastprice_,volumeTrendInfo->pre_ema_val_,volumeTrendInfo->vector_prices_.size());
		return false;
	}
	else{
		volumeTrendInfo->vector_prices_.push_back(volumeTrendInfo->cur_lastprice_);
		volumeTrendInfo->cur_middle_value_ = GetEMAData(volumeTrendInfo->cur_lastprice_,volumeTrendInfo->pre_ema_val_,param->m_Param[param_index].compXave);
		volumeTrendInfo->pre_ema_val_ = volumeTrendInfo->cur_middle_value_;
		// vector<double>::iterator it = vector_prices_.begin();
		// it = vector_prices_.erase(it);
	}
	//已经达到了周期，开始计算middle data和sd的data
	/*
	if (param->m_Param[param_index].PositionAdj == 0){
		volumeTrendInfo->cur_middle_value_ = GetEMAData(param_index,volumeTrendInfo->cur_lastprice_);
	}
	else
	{
		volumeTrendInfo->cur_middle_value_ = GetMAData(param_index);
	}
	*/
	//volumeTrendInfo->cur_sd_val_ = GetSDData(param_index);
	volumeTrendInfo->cur_sd_val_ = GetSDData(volumeTrendInfo->vector_prices_,param->m_Param[param_index].compXave);


	STraderChannel *pTraderInfo_open=&g_arrTraderChannel[trader_open_index];
	cancelNotMatchOrder(threadChannel,pTraderInfo_open);

	STraderChannel *pTraderInfo_close=&g_arrTraderChannel[trader_close_index];
	cancelNotMatchOrder(threadChannel,pTraderInfo_close);

	threadChannel->isTrendOpenTime	=	isOpenTrendTime(threadChannel);
	threadChannel->isTrendCloseTime	=	isCloseTrendTime(threadChannel);

	char path[256]={0};
	sprintf(path,"%d_%s",param->m_Param[param_index].arbitrageTypeID,param->m_Param[param_index].instrumentTypeID);

	char temp[1024]={0};
	sprintf(temp,"%s,%.2f,%.2f,%.2f,%d,%.2f,%.2f,%d,%d,%.2f",volumeTrendInfo->new_Price.updateTime
		,volumeTrendInfo->new_Price.LastPrice,volumeTrendInfo->cur_middle_value_,volumeTrendInfo->cur_sd_val_
		,volumeTrendInfo->new_Price.Volume	-	volumeTrendInfo->last_Price.Volume
		,volumeTrendInfo->new_Price.OpenInterest	-	volumeTrendInfo->last_Price.OpenInterest
		,param->m_Param[param_index].openEdge,param->m_Param[param_index].index
		,param->m_Param[param_index].spread,volumeTrendInfo->rsi_data_);

	WriteMesgToFile((string)path,(string)temp);

	if (threadChannel->isTrendOpenTime)
	{
		STraderChannel *pTraderInfo=&g_arrTraderChannel[trader_open_index];
		trading_process_Lock(threadChannel,pTraderInfo);
	}
	else if (threadChannel->isTrendCloseTime)
	{
		STraderChannel *pTraderInfo=&g_arrTraderChannel[trader_close_index];
		trading_process_Lock(threadChannel,pTraderInfo);
	}
	return true;
}

bool CHyArbitrageVolumeTrendOther3::isOpenTrendTime(SThreadChannel *threadChannel)
{
	if (status	==	STATUS_Exit	||	status	==	STATUS_Pause)
	{
		return false;
	}

	int param_index	=	threadChannel->param_index;
	double	openEdge		=	param->m_Param[param_index].openEdge;
	int index	=	param->m_Param[param_index].index;
	int	spread		=	param->m_Param[param_index].spread;
	VolumeTrendOther3Info *volumeTrendInfo	=	&m_volumeTrendOther3Info[param_index];

	if (arbitrageDirection	==	Direction_long)
	{
		bool is_band_open = IsBandOpenTime('l',volumeTrendInfo->cur_lastprice_,volumeTrendInfo->cur_middle_value_
							,volumeTrendInfo->cur_sd_val_,volumeTrendInfo->band_open_edge_);
		if(is_band_open ==false){
		  return false;
		}
		bool is_trigger_open = IsTriggerSizeOpenTime('l',&(volumeTrendInfo->cur_price_),&(volumeTrendInfo->pre_price_)
					,openEdge,index,spread);
		return is_trigger_open;
	}
	else if (arbitrageDirection	==	Direction_short)
	{
		bool is_band_open = IsBandOpenTime('s',volumeTrendInfo->cur_lastprice_,volumeTrendInfo->cur_middle_value_
							,volumeTrendInfo->cur_sd_val_,volumeTrendInfo->band_open_edge_);
		if(is_band_open ==false){
		  return false;
		}
		bool is_trigger_open = IsTriggerSizeOpenTime('s',&(volumeTrendInfo->cur_price_),&(volumeTrendInfo->pre_price_)
					,openEdge,index,spread);
		return is_trigger_open;
	}
	else{
	  return false;
	}
}

bool CHyArbitrageVolumeTrendOther3::isCloseTrendTime(SThreadChannel *threadChannel)
{
	if (status	==	STATUS_Pause)
	{
		return false;
	}

	
	int param_index	=	threadChannel->param_index;

	if (arbitrageDirection	==	Direction_long)
	{
		return IsBandCloseTime('l',volumeTrendInfo->cur_lastprice_,volumeTrendInfo->cur_middle_value_,volumeTrendInfo->cur_sd_val_
					,volumeTrendInfo->band_loss_close_edge_,volumeTrendInfo->band_profit_close_edge_
					,volumeTrendInfo->rsi_data_,volumeTrendInfo->limit_rsi_data_);
	}
	else if (arbitrageDirection	==	Direction_short)
	{
		return IsBandCloseTime('s',volumeTrendInfo->cur_lastprice_,volumeTrendInfo->cur_middle_value_,volumeTrendInfo->cur_sd_val_
					,volumeTrendInfo->band_loss_close_edge_,volumeTrendInfo->band_profit_close_edge_
					,volumeTrendInfo->rsi_data_,volumeTrendInfo->limit_rsi_data_);
	}
	return false;
}


void CHyArbitrageVolumeTrendOther3::cancelNotMatchOrder(SThreadChannel *threadChannel,STraderChannel* pTraderInfo)
{
	pthread_rwlock_wrlock(&pTraderInfo->cs_trader_order);
	for (unsigned int i=0;i<pTraderInfo->trader_order.size();i++)
	{
		orderMsg *ordermsg=&pTraderInfo->trader_order[i];
		if (ordermsg->remainVolume	==	0)
		{
			continue;
		}
		if (ordermsg->status	==	1) //挂单状态，考虑是否撤单
		{
			tradingAction_action_Lock(threadChannel,ordermsg);
		}
	}
	pthread_rwlock_unlock(&pTraderInfo->cs_trader_order);
}



