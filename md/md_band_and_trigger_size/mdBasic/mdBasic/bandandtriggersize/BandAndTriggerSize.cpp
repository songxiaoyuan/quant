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
	pre_price_ = CThostFtdcDepthMarketDataField();
	cur_price_ = CThostFtdcDepthMarketDataField();
}


BandAndTriggerSize::~BandAndTriggerSize(void)
{
	vector<double>().swap(vector_prices_);
}

double BandAndTriggerSize::GetMAData(vector<double> &vector_prices_){
	double sum =0;
	if (vector_prices_.size()==0)
	{
		return 0;
	}
	for (int i = vector_prices_.size()-1; i >=0 && i >= vector_prices_.size() - compXaveNum_; i--)
	{
		sum +=vector_prices_[i];
	}
	return sum/compXaveNum_;
}

double BandAndTriggerSize::GetSDData(vector<double> &vector_prices_){
	int size = vector_prices_.size();
	if (size ==0)
	{
		return 0;
	}
	double sum = 0;
	for (int i = size-1; i >=0 && i >= size - compXaveNum_; i--)
	{
		sum +=vector_prices_[i];
	}
	double avg = sum/compXaveNum_;
	sum = 0;
	for (int i = size-1; i >=0 && i >= size - compXaveNum_; i--)
	{
		sum += (vector_prices_[i]-avg)*(vector_prices_[i]-avg);
	}
	return sqrt(sum/compXaveNum_);
}

double BandAndTriggerSize::GetEMAData(double price){
	if (vector_prices_.size()==1)
	{
		current_ema_tick_num_ =1;
		last_mea_val_ = price;
		return last_mea_val_;
	}
	if (current_ema_tick_num_ < compXaveNum_)
	{
		current_ema_tick_num_ +=1;
	}
	double ret = ((current_ema_tick_num_ - 1)*last_mea_val_ + 2*price)/(current_ema_tick_num_ + 1);
	last_mea_val_ = ret;
	return ret;
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
	if (vector_prices_.size() < compXaveNum_-1)
	{
		vector_prices_.push_back(cur_lastprice_);
		double tmp = GetEMAData(cur_lastprice_);
		pre_price_ = cur_price_;
		return;
	}
	else{
		vector_prices_.push_back(cur_lastprice_);
	}
	if (movingTheo_ == "MA"){
		cur_middle_value_ = GetMAData(vector_prices_);
	}
	else
	{
		cur_middle_value_ = GetEMAData(cur_lastprice_);
	}
	cur_sd_val_ = GetSDData(vector_prices_);

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
	WriteMesgToFile(path,insert);
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