#include "BandAndTriggerSizeFun.h"


double GetSDDataByMap(map<double,int> &map_prices,int period){
	int size =0;
	if (period ==0)
	{
		return 0;
	}
	double sum =0;
	map<double,int>::iterator iter;
	for (iter = map_prices.begin();iter != map_prices.end();++iter) 
	{
		sum = sum+(iter->first*iter->second);
		size +=iter->second;
	}
	if (size != period)
	{
		cout<<"the size is not the period "<<size<<" : "<<period<<endl;
		//exit(1);
	}
	double avg = sum/period;
	sum=0;
	for (iter = map_prices.begin();iter != map_prices.end();++iter) 
	{
		sum += (iter->first - avg)*(iter->first - avg)*iter->second;
	}
	return sqrt(sum/period);
}

double GetEMAData(double price,double pre_ema_val,int period){
	if (period <=1)
	{
		return price;
	}
	double ret = ((period - 1)*pre_ema_val + 2*price)/(period + 1);
	return ret;
}

double GetRSIData(double tmpdiff,vector<double> &rsi_vector,int period){
	if (period == 0){
		return 0;
	}
	period = period -1;
	double rise = 0;
	double total = 0;
	if (tmpdiff >0) {
		rise +=tmpdiff;
		total +=tmpdiff;
	}
	else{
		total -=tmpdiff;
	}
	int size = rsi_vector.size();
	for (int i = size - 1; i>=0 && i>=size - period; i--)
	{
		double tmp = rsi_vector[i];
		if (tmp >0)
		{
			rise +=tmp;
			total +=tmp;
		}
		else{
			total -=tmp;
		}
	}
	if(rise ==0 || total ==0){
	  return 0;
	}
	return 100*rise/total;
}

bool IsBandOpenTime(char direction,double lastprice,double middle,double sd
					,double open_edge_begin,double open_edge_end ,double limit_sd,double limit_sd_open_edge){
	if (sd < limit_sd)
	{
		open_edge_end = limit_sd_open_edge;
	}
	if (direction =='l')
	{
		double startval = middle + sd*open_edge_begin;
		double upval = middle + sd * open_edge_end;
		if (lastprice > startval && lastprice < upval)
		{
			return true;
		}
	}
	else if (direction	==	's')
	{
		double startval = middle - sd*open_edge_begin;
		double downval = middle - sd * open_edge_end;
		if (lastprice > downval && lastprice < startval)
		{
			return true;
		}
	}
	return false;
}

bool IsBandCloseTime(char direction,double lastprice,double middle,double sd
					 ,double loss_band,double profit_band,double rsival,double limit_rsi
					 ,double limit_sd,double limit_sd_loss_band){
	 if (sd < limit_sd){
		loss_band = limit_sd_loss_band;
	}
	if (direction	==	'l')
	{
		double profitval = middle + sd * profit_band;
		double lossval = middle - sd * loss_band;
		if ( lastprice < lossval)
		{
			return true;
		}
		if (lastprice > profitval)
		{
			if (rsival >limit_rsi)
			{
				return true;
			}
		}
	}
	else if (direction	==	's')
	{
		double profitval = middle - sd * profit_band;
		double lossval = middle + sd * loss_band;
		if (lastprice > lossval)
		{
			return true;
		}
		double tmp_rsi = 100 - rsival;
		if (lastprice < profitval && tmp_rsi > limit_rsi)
		{
			if (tmp_rsi > limit_rsi)
			{
				return true;
			}
		}
	}
	return false;
}

bool IsTriggerSizeOpenTime(char direction,double limit_diff_volume,double limit_diff_openinterest,double limit_spread
				,mdPrice *now_price,mdPrice *pre_price,int multiple){

	if (now_price->Volume - pre_price->Volume < limit_diff_volume )
	 {
		return false;
	}
	if (now_price->OpenInterest - pre_price->OpenInterest < limit_diff_openinterest)
	{
		return false;
	}
	if (direction =='l')
	{
		return IsUpTime(now_price,pre_price,limit_spread,multiple);
	}
	else if (direction =='s')
	{
		return IsDownTime(now_price,pre_price,limit_spread,multiple);
	}
	return false;
}

bool IsTriggerSizeCloseTime(char direction,mdPrice *now_price,mdPrice *pre_price,int multiple
				,double volume_edge,double openinterest_edge,double spread){
	if (now_price->Volume - pre_price->Volume < volume_edge )
	 {
		return false;
	}
	if (now_price->OpenInterest - pre_price->OpenInterest >= - openinterest_edge)
	{
		return false;
	}
	if (direction =='l')
	{
		return IsDownTime(now_price,pre_price,spread,multiple);
	}
	else if (direction =='s')
	{
		return IsUpTime(now_price,pre_price,spread,multiple);
	}
	return false;
}

bool IsDownTime(mdPrice *now_price,mdPrice *pre_price,int spread,int multiple){
	int diffVolume	=	now_price->Volume	-	pre_price->Volume;   
	double diffTurnover	=	now_price->Turnover	-	pre_price->Turnover;  
	if (diffVolume	==	0	||	diffTurnover	==	0	||	multiple	==	0)
	{
		return false;
	}

	double avePrice	=	diffTurnover/diffVolume/multiple;

	double temp	=	100*(pre_price->AskPrice1	-	avePrice)/(pre_price->AskPrice1 - pre_price->BidPrice1);
	if (temp >=	spread)
	{
		return true;
	}
	return false;
}

bool IsUpTime(mdPrice *now_price,mdPrice *pre_price,int spread,int multiple)
{
	int diffVolume	=	now_price->Volume	-	pre_price->Volume;  //返回持仓量的变化
	double diffTurnover	=	now_price->Turnover	-	pre_price->Turnover;  //返回成交金额的变化

	if (diffVolume	==	0	||	diffTurnover	==	0	||	multiple	==	0)
	{
		return false;
	}
	double avePrice	=	diffTurnover/diffVolume/multiple;

	double temp	=	100*(avePrice	-	pre_price->BidPrice1)/(pre_price->AskPrice1 - pre_price->BidPrice1);


	if (temp >=	spread)
	{
		return true;

	}
	return false;
}


void StartAndStopFun(Param *param,VolumeTrendOther3Info *info){

	memset(&info->pre_price,0,sizeof(mdPrice));
	memset(&info->cur_price,0,sizeof(mdPrice));

	//init the info 
	info->multiple = 10;
	info->direction = 's';
	info->now_rsi_bar_tick =0;


	//set the trigger size param
	param->diff_volume =500;
	param->openInterest = 0;
	param->spread = 100;

	//set the bolling param
	param->openEdgeBegin = 0;
	param->openEdgeEnd = 0.5;
	param->lossCloseEdge = 0.5;
	param->profitCloseEdge = 5;
	param->limitSD = 10;
	param->limitSDOpenEdgeEnd = 1;
	param->limitSDLossCloseEdge = 1;
	param->period = 7200;

	//set the RSI param
	param->limitRSI = 90;
	param->RSIBarPeriod = 120;
	param->RSIPeriod = 14;
}

BandAndTriggerSizeRetStatus GetMdData(Param *param,VolumeTrendOther3Info *info){
	BandAndTriggerSizeRetStatus ret;
	ret.isTrendCloseTime = false;
	ret.isTrendOpenTime = false;
	double rsi_data;
	double middle_val;
	double sd_val;

	double lastprice = info->cur_price.LastPrice;
	int period = param->period;
	int rsi_period = param->RSIPeriod;
	int rsi_bar_limit_tick =param->RSIBarPeriod;

	if (info->pre_rsi_lastprice ==0)
	{
		info->pre_rsi_lastprice = lastprice;
	}
	if(info->now_rsi_bar_tick >= rsi_bar_limit_tick){
		//cout<<"the bar tick is enough"<<endl;
		double tmpdiff = lastprice - info->pre_rsi_lastprice;
		info->pre_rsi_lastprice = lastprice;
		info->now_rsi_bar_tick = 1;
		rsi_data = GetRSIData(tmpdiff,info->rsi_vector,rsi_period);
		info->rsi_vector.push_back(tmpdiff);
	}
	else{
		info->now_rsi_bar_tick +=1;
		double tmpdiff = lastprice - info->pre_rsi_lastprice;
		rsi_data = GetRSIData(tmpdiff,info->rsi_vector,rsi_period);
	}

	if (info->prices_queue.size() < period )
	{
		info->prices_queue.push(lastprice);
		info->pre_ema_val = GetEMAData(lastprice,info->pre_ema_val,info->prices_queue.size());
		map<double,int>::iterator iter = info->prices_map.find(lastprice);
		if (iter ==  info->prices_map.end())
		{
			 info->prices_map[lastprice] = 1;
		}
		else{
			iter->second +=1;
		}
		return ret;
	}
	else{
		info->prices_queue.push(lastprice);
		double front_prices_ = info->prices_queue.front();
		info->prices_queue.pop();
		if (lastprice != front_prices_)
		{
			map<double,int>::iterator iter = info->prices_map.find(lastprice);
		    if (iter ==  info->prices_map.end())
			{
				 info->prices_map[lastprice] = 1;
			}
			else{
				iter->second +=1;
			}
			 info->prices_map[front_prices_] -=1;
		}
	}

	middle_val =  GetEMAData(lastprice,info->pre_ema_val,period);
	info->pre_ema_val = middle_val;
	sd_val = GetSDDataByMap(info->prices_map,period);

	ret.isTrendOpenTime = IsOpenTime(middle_val,sd_val,param,info);
	ret.isTrendCloseTime = IsCloseTime(middle_val,sd_val,rsi_data,param,info);

	return ret;
}

bool IsOpenTime(double middle_val,double sd_val,Param *param,VolumeTrendOther3Info *info){
	//band_open_noraml
	double band_open_edge_begin = param->openEdgeBegin;
	double band_open_edge_end =  param->openEdgeEnd;

	//limit_sd
	double limit_sd = param->limitSD;
	//limit_sd_open_edge
	double limit_sd_open_edge =param->limitSDOpenEdgeEnd;


	double spread = param->spread;
	//diff_volume
	double diff_volume = param->diff_volume;
	//open interest
	double diff_openinterest = param->openInterest;
	//tick num
	

	double lastprice = info->cur_price.LastPrice;

	bool is_band_open = IsBandOpenTime(info->direction,lastprice,middle_val,sd_val,
		band_open_edge_begin,band_open_edge_end,limit_sd,limit_sd_open_edge);
	if (is_band_open ==false)
	{
		return false;
	}

	bool is_trigger_open = IsTriggerSizeOpenTime(info->direction,diff_volume,diff_openinterest,spread,
		&info->cur_price,&info->pre_price,info->multiple);

	return is_trigger_open;
}

bool IsCloseTime(double middle_val,double sd_val,double rsi_val,Param *param,VolumeTrendOther3Info *info){
	//band_loss_close_edge
	double band_loss_close_edge = param->lossCloseEdge;
	//band_profit_close_edge
	double band_profit_close_edge =param->profitCloseEdge;

	//limit_rsi
	double limit_rsi = param->limitRSI;

	//limit_sd
	double limit_sd =param->limitSD;
	//limit_sd_loss_close_edge
	double limit_sd_close_edge = param->limitSDLossCloseEdge;

	double lastprice = info->cur_price.LastPrice;

	return IsBandCloseTime(info->direction,lastprice,middle_val,sd_val,
		band_loss_close_edge,band_profit_close_edge,rsi_val,limit_rsi,limit_sd,limit_sd_close_edge);
}


void GetOpenSignal(VolumeTrendOther3Info *info){
	//this function is called when the open signal is received
}
void GetCloseSignal(VolumeTrendOther3Info *info){
	//this function is called when the close signal is received
}