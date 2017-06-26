#include "arbitrageVolumeTrendOther3.h"

void CHyArbitrageVolumeTrendOther3::clearVector()
{
	memset(&new_Price,0,sizeof(mdPrice));
	memset(&last_Price,0,sizeof(mdPrice));
	isTrendOpenTime=false;
	isTrendCloseTime=false;
	cur_spread_price_val_=0;

	string path = "bandandtriggersizeconfig.txt";
	unordered_map<string,string> ret =  GetConfigInfo(path);
	rsi_bar_period_ = atoi(ret["rsi_bar_period"].c_str());
	rsi_period_ = atoi(ret["rsi_period"].c_str());
	limit_rsi_data_ = atoi(ret["limit_rsi_data"].c_str());
}

bool CHyArbitrageVolumeTrendOther3::get_fv_less(double &fv)
{
	// 获取最新的价格。问题：这个mdprice对象每一个参数的作用？
	new_Price=getNewPrice(g_arrChannel,less_md_index);

	last_Price=getLastPrice(g_arrChannel,less_md_index);


	if (new_Price.Volume	==0	||	new_Price.OpenInterest	==	0	||	new_Price.Turnover	==0||	new_Price.LastPrice	==	0
	  ||last_Price.Volume	==0	||	last_Price.OpenInterest	==	0	||	last_Price.Turnover	==0||	last_Price.LastPrice	==	0)
	{
		return false;
	}

	//通过 ShortCompXave 来设置band的开仓边界，一般都是0.5,这边除以了10，所以设置的时候，应该在10倍。
	band_open_edge_ = ((double)param->less.PositionAdj)/10;
	band_loss_close_edge_ = ((double)param->less.AdjEmaFast)/10;
	// 通过设置 LongCompXave 来设置band的平仓边界，一般都是3
	band_profit_close_edge_ = ((double)param->less.AdjEmaSlow)/10;

	// 此部分代码主要是用来保存计算cur_middle_value_和sd的price。
	cur_lastprice_ = new_Price.LastPrice;


	if (now_rsi_bar_tick_ >= rsi_bar_period_)
	{
		double tmpdiff = cur_lastprice_ - last_Price.LastPrice;
		pre_rsi_lastprice_ = cur_lastprice_;
		now_rsi_bar_tick_ =1;
		rsi_data_ = GetRSIData(tmpdiff,rsi_vector_,rsi_period_);
		rsi_vector_.push_back(tmpdiff);
	}
	else{
		now_rsi_bar_tick_ +=1;
		double tmpdiff = cur_lastprice_ - pre_price_.LastPrice;
		rsi_data_ = GetRSIData(tmpdiff,rsi_vector_,rsi_period_);
	}


	compXaveNum_ = param->less.compXave;
	if (vector_prices_.size() < param->less.compXave)
	{
		vector_prices_.push_back(cur_lastprice_);
		pre_ema_val_ = GetEMAData(cur_lastprice_,pre_ema_val_,vector_prices_.size());
		pre_price_ = cur_price_;
		return false;
	}
	else{
		vector_prices_.push_back(cur_lastprice_);
		cur_middle_value_ = GetEMAData(cur_lastprice_,pre_ema_val_,compXaveNum_);
		pre_ema_val_ = cur_middle_value_;
		// vector<double>::iterator it = vector_prices_.begin();
		// it = vector_prices_.erase(it);
	}
	//已经达到了周期，开始计算middle data和sd的data
	/*
	if (param->less.PositionAdj == 0){
		cur_middle_value_ = GetEMAData(cur_lastprice_);
	}
	else
	{
		cur_middle_value_ = GetMAData(vector_prices_);
	}*/
	cur_sd_val_ = GetSDData(vector_prices_,compXaveNum_);

	// 根据最新的价格判断已经下的单子是不是需要撤单
	// 问题：我需要一个参数，现在来决定到了这个边界就开始撤单，那么我怎么调用。达到布林带的某一个标准的时候。
	STraderChannel *pTraderInfo_open=&g_arrTraderChannel[less_open_order_index];
	cancelNotMatchOrder(pTraderInfo_open);

	STraderChannel *pTraderInfo_close=&g_arrTraderChannel[less_close_order_index];
	cancelNotMatchOrder(pTraderInfo_close);

	
	// 计算开仓和平仓的时机，通过这个tick的数据，来判断是不是需要进行开仓或者平仓。
	isTrendOpenTime		=	isOpenTrendTime();
	isTrendCloseTime	=	isCloseTrendTime();


	char path[256]={0};
	sprintf(path,"%d_%s",param->less.arbitrageTypeID,param->less.instrumentTypeID);



	char temp[1024]={0};
	sprintf(temp,"%s,%.2f,%.2f,%.2f,%d,%.2f,%.2f,%.2f,%d,%d,%.2f,%.2f,%.2f",new_Price.updateTime,
		new_Price.LastPrice,cur_middle_value_,cur_sd_val_,new_Price.Volume	-last_Price.Volume,
		new_Price.OpenInterest	-	last_Price.OpenInterest,cur_spread_price_val_,
		param->less.openEdge,param->less.index,param->less.spread,band_open_edge_,band_loss_close_edge_,band_profit_close_edge_);

	WriteMesgToFile((string)path,(string)temp);

	// 如果达到开仓的时机，那么就进行开仓的操作，如果达到平仓的时机，那么就进行平仓的操作？
	//如果到了开仓的时机，但是已经进行开仓了，还会继续开仓吗，还是这个函数会自己判断。
	// 如果已经平仓了也是同样的道理，判断是不是已经平仓了吗。
	if (isTrendOpenTime)
	{
		STraderChannel *pTraderInfo=&g_arrTraderChannel[less_open_order_index];
		less_process_Lock(pTraderInfo);
	}
	else if (isTrendCloseTime)
	{
		STraderChannel *pTraderInfo=&g_arrTraderChannel[less_close_order_index];
		less_process_Lock(pTraderInfo);

	}
	return true;
}
double CHyArbitrageVolumeTrendOther3::calculateEmaFvAdj()
{
	return 0;
}

double CHyArbitrageVolumeTrendOther3::calculateLessPrice(char OffsetFlag,char Direction,double fv,int perside)
{
	
	// 这个最后的挂单价根本就没有计算啊，还是这个maxprice已经改变了。
	// tirgger 感觉根本就不需要计算挂单价，直接达到下单条件之后，根据选择的下单方式直接下单就好了
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

bool CHyArbitrageVolumeTrendOther3::isOpenTrendTime()
{
	if (status	==	STATUS_Exit	||	status	==	STATUS_Pause)
	{
		return false;
	}

	// 表示现在交易量之间的差值必须达到一定的数值。
	int diffVolume	=	new_Price.Volume	-	last_Price.Volume;
	if (diffVolume	<	param->less.openEdge)
	{
		return false;
	}

	// 表示现在持仓量之间的差值需要达到一定的数值。
	int diffOpenInterest	=	new_Price.OpenInterest	-	last_Price.OpenInterest;

	if (diffOpenInterest	<=	param->less.index)
	{
		return false;
	}

	// 获取band的信号，判断根据布林带是不是已经达到开仓条件。
	// 如果没有达到的话，直接返回false, 达到的话判断trigger size是不是达到.
	bool bandSignal = IsBandOpenTime();
	if (bandSignal ==false)
	{
		return false;
	}
	double edge =param->less.spread;

	if (arbitrageDirection	==	Direction_long)
	{
		return isUpTime(edge);
	}
	else if (arbitrageDirection	==	Direction_short)
	{
		return isDownTime(edge);
	}
	return false;
}

bool CHyArbitrageVolumeTrendOther3::isCloseTrendTime()
{

	if (status	==	STATUS_Pause)
	{
		return false;
	}

	//现在根据袁总的要求，在判断平仓的时候，只是根据band的平仓条件。
	// 所以trigger size 的平仓条件先忽略。
	return IsBandCloseTime();

	int diffVolume	=	new_Price.Volume	-	last_Price.Volume;
	if (diffVolume	<	param->less.closeEdge)
	{
		return false;
	}

	int diffOpenInterest	=	new_Price.OpenInterest	-	last_Price.OpenInterest;

	if (diffOpenInterest	>=	-	param->less.index)
	{
		return false;
	}


	double edge =param->less.spread;


	if (arbitrageDirection	==	Direction_long)
	{
		return isDownTime(edge);
	}
	else if (arbitrageDirection	==	Direction_short)
	{
		return isUpTime(edge);
	}
	return false;
}

bool CHyArbitrageVolumeTrendOther3::isUpTime(double edge)
{
	int multiple=getVolumeMultiple(g_arrChannel,less_md_index);   //返回合约乘数
	int diffVolume	=	new_Price.Volume	-	last_Price.Volume;  //返回持仓量的变化
	double diffTurnover	=	new_Price.Turnover	-	last_Price.Turnover;  //返回成交金额的变化

	if (diffVolume	==	0	||	diffTurnover	==	0	||	multiple	==	0)
	{
		return false;
	}
	double avePrice	=	diffTurnover/diffVolume/multiple;

	double temp	=	100*(avePrice	-	last_Price.BidPrice1)/(last_Price.AskPrice1-last_Price.BidPrice1);
	cur_spread_price_val_ = temp;
	if (temp >=	edge)
	{
		return true;

	}
	return false;

}

bool CHyArbitrageVolumeTrendOther3::isDownTime(double edge)
{
	int multiple=getVolumeMultiple(g_arrChannel,less_md_index);    
	int diffVolume	=	new_Price.Volume	-	last_Price.Volume;   
	double diffTurnover	=	new_Price.Turnover	-	last_Price.Turnover;  

	if (diffVolume	==	0	||	diffTurnover	==	0	||	multiple	==	0)
	{
		return false;
	}

	double avePrice	=	diffTurnover/diffVolume/multiple;

	double temp	=	100*(last_Price.AskPrice1	-	avePrice)/(last_Price.AskPrice1-last_Price.BidPrice1);
	cur_spread_price_val_ = temp;
	if (temp >=	edge)
	{
		return true;

	}
	return false;

}
void CHyArbitrageVolumeTrendOther3::cancelNotMatchOrder(STraderChannel* pTraderInfo)
{
	pthread_mutex_lock(&pTraderInfo->cs_trader_order);
	for (unsigned int i=0;i<pTraderInfo->trader_order.size();i++)
	{
		orderMsg *ordermsg=&pTraderInfo->trader_order[i];
		if (ordermsg->remainVolume	==	0)
		{
			continue;
		}

		if (ordermsg->status	==	1) //挂单状态，考虑是否撤单
		{
			lessAction_action_Lock(ordermsg);

		}
	}
	pthread_mutex_unlock(&pTraderInfo->cs_trader_order);
}


bool CHyArbitrageVolumeTrendOther3::IsBandOpenTime(){
	if (arbitrageDirection	==	Direction_long)
	{
		double upval = cur_middle_value_ + cur_sd_val_ * band_open_edge_;
		if (cur_lastprice_ > cur_middle_value_ && cur_lastprice_ < upval)
		{
			return true;
		}
	}
	else if (arbitrageDirection	==	Direction_short)
	{
		double downval = cur_middle_value_ - cur_sd_val_ * band_open_edge_;
		if (cur_lastprice_ > downval && cur_lastprice_ < cur_middle_value_)
		{
			return true;
		}
	}
	return false;
}

bool CHyArbitrageVolumeTrendOther3::IsBandCloseTime(){

	if (arbitrageDirection	==	Direction_long)
	{
		double profitval = cur_middle_value_ + cur_sd_val_ * band_profit_close_edge_;
		double lossval = cur_middle_value_ - cur_sd_val_ * band_loss_close_edge_;
		if ( band_loss_close_edge_ !=0  && cur_lastprice_ < lossval)
		{
			return true;
		}
		if (cur_lastprice_ > profitval && rsi_data_ > limit_rsi_data_ )
		{
			return true;
		}
	}
	else if (arbitrageDirection	==	Direction_short)
	{
		double profitval = cur_middle_value_ - cur_sd_val_ * band_profit_close_edge_;
		double lossval = cur_middle_value_ + cur_sd_val_ * band_loss_close_edge_;
		if (band_loss_close_edge_ !=0 && cur_lastprice_ > lossval)
		{
			return true;
		}
		double tmp_rsi = 100 - rsi_data_;
		if (cur_lastprice_ < profitval && tmp_rsi > limit_rsi_data_)
		{
			return true;
		}
	}
	return false;
}

void CHyArbitrageVolumeTrendOther3::closeTraded(char direction,double price)
{


}

void CHyArbitrageVolumeTrendOther3::openTraded(char direction,double price)
{

}
