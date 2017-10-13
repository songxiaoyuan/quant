#include "arbitrageVolumeTrendOtherFade.h"

void CHyArbitrageVolumeTrendOtherFade::clearVector()
{
	for (int i=0;i<MAX_PARAM_LENGTH;i++)
	{
		VolumeTrendOtherFadeInfo * info	=	&m_volumeTrendOtherFadeInfo[i];
		memset(&info->pre_price,0,sizeof(mdPrice_Fade));
		memset(&info->cur_price,0,sizeof(mdPrice_Fade));

		info->pre_ema_val	=	0;

		info->multiple	=	0;
		info->direction	=	'n';
		info->max_profit = 0;
		info->open_price = 0;

		info->pre_rsi_lastprice	=	0;
		info->now_rsi_bar_tick	=	0;
	}

	VolumeTrendOtherFadeInfo * info	=	&m_volumeTrendOtherFadeInfo[0];
	Parameter_Fade param_trigger;
	memcpy(&param_trigger,param,sizeof(Parameter_Fade));
	StartAndStopFun(&param_trigger,info,0);
}

double CHyArbitrageVolumeTrendOtherFade::calculateLessPrice(SThreadChannel *threadChannel,char OffsetFlag,char Direction,double fv,int perside)
{
	double price=0;
	if (Direction	==	THOST_FTDC_D_Buy)				//Âò
	{
		price	=	0;
	}
	else if(Direction	==	THOST_FTDC_D_Sell)			//Âô
	{
		price	=	MaxPrice;
	}
	return price;
}

bool CHyArbitrageVolumeTrendOtherFade::get_fv(SThreadChannel *threadChannel,double &fv)
{
	int md_index	=	threadChannel->md_index;
	int param_index	=	threadChannel->param_index;
	int trader_open_index	=	threadChannel->Trader_index_1;
	int trader_close_index	=	threadChannel->Trader_index_2;

	VolumeTrendOtherFadeInfo *volumeTrendInfo	=	&m_volumeTrendOtherFadeInfo[param_index];

    mdPrice cur_price;
    mdPrice pre_price;
	getNewPrice(g_arrChannel,md_index,cur_price);
	getLastPrice(g_arrChannel,md_index,pre_price);

	memcpy(&volumeTrendInfo->cur_price,&cur_price,sizeof(mdPrice_Fade));
	memcpy(&volumeTrendInfo->pre_price,&pre_price,sizeof(mdPrice_Fade));

	Parameter_Fade param_trigger;
	memcpy(&param_trigger,param,sizeof(Parameter_Fade));

	if (volumeTrendInfo->cur_price.Volume	==0	||	volumeTrendInfo->cur_price.OpenInterest	==0	
		||	volumeTrendInfo->cur_price.Turnover	==0||	volumeTrendInfo->cur_price.LastPrice	==	0
		||volumeTrendInfo->pre_price.Volume	==0	||	volumeTrendInfo->pre_price.OpenInterest	==0	
		||	volumeTrendInfo->pre_price.Turnover	==0||	volumeTrendInfo->pre_price.LastPrice	==	0)
	{
		return false;
	}

	volumeTrendInfo->multiple = getVolumeMultiple(g_arrChannel,md_index);


	STraderChannel *pTraderInfo_open=&g_arrTraderChannel[trader_open_index];
	cancelNotMatchOrder(threadChannel,pTraderInfo_open);

	STraderChannel *pTraderInfo_close=&g_arrTraderChannel[trader_close_index];
	cancelNotMatchOrder(threadChannel,pTraderInfo_close);

	if (status	==	STATUS_Exit	||	status	==	STATUS_Pause)
	{
		return false;
	}

	if (arbitrageDirection	==	Direction_long)
	{
		volumeTrendInfo->direction = 'l';
	}
	else if (arbitrageDirection	==	Direction_short)
	{
		volumeTrendInfo->direction = 's';
	}
	else{
	  return false;
	}

	BandAndTriggerSizeFadeRetStatus ret_status = GetMdData(&param_trigger,volumeTrendInfo,param_index);

	threadChannel->isTrendOpenTime	=	ret_status.isTrendOpenTime;
	threadChannel->isTrendCloseTime	=	ret_status.isTrendCloseTime;


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


void CHyArbitrageVolumeTrendOtherFade::cancelNotMatchOrder(SThreadChannel *threadChannel,STraderChannel* pTraderInfo)
{
	pthread_rwlock_wrlock(&pTraderInfo->cs_trader_order);
	for (unsigned int i=0;i<pTraderInfo->trader_order.size();i++)
	{
		orderMsg *ordermsg=&pTraderInfo->trader_order[i];
		if (ordermsg->remainVolume	==	0)
		{
			continue;
		}
		if (ordermsg->status	==	1) //¹Òµ¥×´Ì¬£¬¿¼ÂÇÊÇ·ñ³·µ¥
		{
			tradingAction_action_Lock(threadChannel,ordermsg);
		}
	}
	pthread_rwlock_unlock(&pTraderInfo->cs_trader_order);
}

void CHyArbitrageVolumeTrendOtherFade::tradingOpenTraded(char *instrumentID)
{
	int thread_index	=	getThreadChannelIndex_trading(instrumentID);
	if (thread_index	==	-1)
	{
		return;
	}
	SThreadChannel * threadChannel	=	&m_arrThreadChannel[thread_index];
	int param_index	=	threadChannel->param_index;
	VolumeTrendOtherFadeInfo *volumeTrendInfo	=	&m_volumeTrendOtherFadeInfo[param_index];
	
	GetOpenSignal(volumeTrendInfo);
	threadChannel->isTrendOpenTime	=	false;
}

void CHyArbitrageVolumeTrendOtherFade::tradingCloseTraded(char *instrumentID)
{
	int thread_index	=	getThreadChannelIndex_trading(instrumentID);
	if (thread_index	==	-1)
	{
		return;
	}
	SThreadChannel * threadChannel	=	&m_arrThreadChannel[thread_index];
	int param_index	=	threadChannel->param_index;
	VolumeTrendOtherFadeInfo *volumeTrendInfo	=	&m_volumeTrendOtherFadeInfo[param_index];
   
   	GetCloseSignal(volumeTrendInfo);
}



