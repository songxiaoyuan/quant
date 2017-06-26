#include "BandAndTriggerSize.h"


BandAndTriggerSize::BandAndTriggerSize(void)
{
	compXaveNum_ =1000;
	movingTheo_ = "EMA";
	band_open_edge_ = 1;
	band_close_edge_ = 2;
	direction_ = "LONG";
	position_ =0;
	price_spread_edge_ = 0;
	volume_edge_ =0 ;
	openinterest_edge_ = 0;
	multiple_val_ = 5;
	now_rsi_bar_tick_ = 0;
	rsi_bar_period_ =100;
	rsi_period_ = 10;
	pre_rsi_lastprice_ = 0;
	rsi_data_ = 0;
	pre_ema_val_=0;
	pre_price_ = CThostFtdcDepthMarketDataField();
	cur_price_ = CThostFtdcDepthMarketDataField();
}


BandAndTriggerSize::~BandAndTriggerSize(void)
{
	vector<double>().swap(vector_prices_);
}


bool BandAndTriggerSize::IsBandOpenTime(){
	if (direction_	==	"LONG")
	{
		double upval = cur_middle_value_ + cur_sd_val_ * band_open_edge_;
		if (cur_lastprice_ > cur_middle_value_ && cur_lastprice_ < upval)
		{
			return true;
		}
	}
	else if (direction_	==	"SHORT")
	{
		double downval = cur_middle_value_ - cur_sd_val_ * band_open_edge_;
		if (cur_lastprice_ > downval && cur_lastprice_ < cur_middle_value_)
		{
			return true;
		}
	}
	return false;
}

bool BandAndTriggerSize::IsBandCloseTime(){

	if (direction_	==	"LONG")
	{
		double profitval = cur_middle_value_ + cur_sd_val_ * band_close_edge_;
		double lossval = cur_middle_value_ - cur_sd_val_ * band_open_edge_;
		if (cur_lastprice_ > profitval || cur_lastprice_ < lossval)
		{
			return true;
		}
	}
	else if (direction_	==	"SHORT")
	{
		double profitval = cur_middle_value_ - cur_sd_val_ * band_close_edge_;
		double lossval = cur_middle_value_ + cur_sd_val_ * band_open_edge_;
		if (cur_lastprice_ > lossval || cur_lastprice_ < profitval)
		{
			return true;
		}
	}
	return false;
}

bool BandAndTriggerSize::IsTriggerSizeOpenTime(){
	if (cur_price_.Volume - pre_price_.Volume < volume_edge_ )
	 {
		return false;
	}
	if (cur_price_.OpenInterest - pre_price_.OpenInterest <= openinterest_edge_)
	{
		return false;
	}
	if (direction_ =="LONG")
	{
		return IsUpTime(price_spread_edge_);
		
	}
	else if (direction_ =="SHORT")
	{
		return IsDownTime(price_spread_edge_);
	}
	return false;
}

bool BandAndTriggerSize::IsTriggerSizeCloseTime(){
	if (cur_price_.Volume - pre_price_.Volume < volume_edge_ )
	 {
		return false;
	}
	if (cur_price_.OpenInterest - pre_price_.OpenInterest >= - openinterest_edge_)
	{
		return false;
	}
	bool ret = false;
	if (direction_ =="LONG")
	{
		return IsDownTime(price_spread_edge_);
	}
	else if (direction_ =="SHORT")
	{
		return IsUpTime(price_spread_edge_);
	}
	return false;
}

bool BandAndTriggerSize::IsDownTime(double edge)
{
	int multiple=multiple_val_;    
	int diffVolume	=	cur_price_.Volume	-	pre_price_.Volume;   
	double diffTurnover	=	cur_price_.Turnover	-	pre_price_.Turnover;  

	if (diffVolume	==	0	||	diffTurnover	==	0	||	multiple	==	0)
	{
		return false;
	}

	double avePrice	=	diffTurnover/diffVolume/multiple;

	double temp	=	100*(pre_price_.AskPrice1	-	avePrice)/(pre_price_.AskPrice1-pre_price_.BidPrice1);
	if (temp >=	edge)
	{
		return true;

	}
	return false;

}

bool BandAndTriggerSize::IsUpTime(double edge)
{
	int multiple=multiple_val_;   //返回合约乘数
	int diffVolume	=	cur_price_.Volume	-	pre_price_.Volume;  //返回持仓量的变化
	double diffTurnover	=	cur_price_.Turnover	-	pre_price_.Turnover;  //返回成交金额的变化

	if (diffVolume	==	0	||	diffTurnover	==	0	||	multiple	==	0)
	{
		return false;
	}
	double avePrice	=	diffTurnover/diffVolume/multiple;

	double temp	=	100*(avePrice	-	pre_price_.BidPrice1)/(pre_price_.AskPrice1-pre_price_.BidPrice1);

	if (temp >=	edge)
	{
		return true;

	}
	return false;

}

void BandAndTriggerSize::getPrices(CThostFtdcDepthMarketDataField *pDepthMarketData){
	cur_lastprice_ = pDepthMarketData->LastPrice;
	cur_price_.Volume = pDepthMarketData->Volume;
	cur_price_.LastPrice = pDepthMarketData->LastPrice;
	cur_price_.OpenInterest = pDepthMarketData->OpenInterest;
	cur_price_.AskPrice1 = pDepthMarketData->AskPrice1;
	cur_price_.BidPrice1 = pDepthMarketData->BidPrice1;
	//cout<<cur_price_.BidPrice1<<endl;
	//cout<<pre_price_.LastPrice<<endl;

	if (pre_price_.LastPrice ==0)
	{
		pre_price_ = cur_price_;
		pre_rsi_lastprice_ = pre_price_.LastPrice;
		return;
	}
	if (now_rsi_bar_tick_ >= rsi_bar_period_)
	{
		double tmpdiff = cur_lastprice_ - pre_price_.LastPrice;
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

	if (vector_prices_.size() < compXaveNum_)
	{
		vector_prices_.push_back(cur_lastprice_);
		pre_ema_val_ = GetEMAData(cur_lastprice_,pre_ema_val_,vector_prices_.size());
		pre_price_ = cur_price_;
		return;
	}
	else{
		vector_prices_.push_back(cur_lastprice_);
	}
	if (movingTheo_ == "MA"){
		cur_middle_value_ = GetMAData(vector_prices_,compXaveNum_);
	}
	else
	{
		cur_middle_value_ = GetEMAData(cur_lastprice_,pre_ema_val_,compXaveNum_);
		pre_ema_val_ = cur_middle_value_;
	}
	cur_sd_val_ = GetSDData(vector_prices_,compXaveNum_);

	string id = pDepthMarketData->InstrumentID;
	string date = pDepthMarketData->TradingDay;
	string time =pDepthMarketData->UpdateTime;
	/*cout<<cur_lastprice_<<endl;
	cout<<cur_middle_value_<<endl;
	cout<<cur_sd_val_<<endl;
	*/
	//cout<<id<<endl;
	string path = id+"_"+date;
	string insert = id +  ","+date+","+time+","+to_string(cur_lastprice_)
		+","+to_string(cur_middle_value_)+"," + to_string(cur_sd_val_);
	//cout<<insert<<endl;
	WriteMesgToFile12(path,insert);
	bool band_open_signal = IsBandOpenTime();
	bool trigger_size_open_signal = IsTriggerSizeOpenTime();	
	if (band_open_signal){
		cout<<"this is band open signal"<<endl;
	}
	if (trigger_size_open_signal)
	{
		cout<<"this is the trigger signal"<<endl;
	}
	bool band_close_signal = IsBandCloseTime();
	if(band_close_signal){
		cout<<"this is band close signal"<<endl;
	}

	pre_price_ = cur_price_;
}