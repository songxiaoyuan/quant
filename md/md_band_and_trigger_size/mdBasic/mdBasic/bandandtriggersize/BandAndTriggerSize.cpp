#include "BandAndTriggerSize.h"


BandAndTriggerSize::BandAndTriggerSize(void)
{
	compXaveNum_ =10;
	movingTheo_ = "EMA";
	band_open_edge_ = 0.5;
    band_loss_close_edge_= 1;
	band_profit_close_edge_ =3;
	direction_ = "LONG";
	position_ =0;
	price_spread_edge_ = 0;
	volume_edge_ =900 ;
	openinterest_edge_ = 0;

	now_rsi_bar_tick_ = 0;
	pre_rsi_lastprice_ = 0;
	rsi_val_ = 0;
	limit_rsi_val_ =80;
	pre_ema_val_=0;

	string path = "bandandtriggersizeconfig.txt";
	vector<string> ret =  GetConfigInfo(path);
	rsi_bar_period_ = atoi(ret[0].c_str());
	rsi_period_ = atoi(ret[1].c_str());
	limit_rsi_val_ = atoi(ret[2].c_str());

	//cout<<rsi_bar_period_<<endl;
	//cout<<rsi_period_<<endl;
	//cout<<limit_rsi_val_<<endl;
}


BandAndTriggerSize::~BandAndTriggerSize(void)
{
	vector<double>().swap(vector_prices_);
}

bool BandAndTriggerSize::IsTrendOpenTime(){
	if (direction_ =="LONG")
	{
		bool is_band_open = IsBandOpenTime('l',cur_lastprice_,cur_middle_value_,cur_sd_val_,band_open_edge_);
		if(is_band_open ==false){
		  //return false;
		}
		bool is_trigger_open = IsTriggerSizeOpenTime('l',&cur_price_,&pre_price_,volume_edge_,openinterest_edge_,price_spread_edge_);
		return is_trigger_open;
	}
	else if (direction_ =="SHORT")
	{
		bool is_band_open = IsBandOpenTime('s',cur_lastprice_,cur_middle_value_,cur_sd_val_,band_open_edge_);
		if(is_band_open ==false){
		  return false;
		}
		bool is_trigger_open = IsTriggerSizeOpenTime('s',&cur_price_,&pre_price_,volume_edge_,openinterest_edge_,price_spread_edge_);
		return is_trigger_open;
	}
	else{
	  return false;
	}
}

bool BandAndTriggerSize::IsTrendCloseTime(){
	if(direction_ =="LONG"){
	  return IsBandCloseTime('l',cur_lastprice_,cur_middle_value_,cur_sd_val_,band_loss_close_edge_,band_profit_close_edge_,rsi_val_,limit_rsi_val_);
	}
	else if (direction_ =="SHORT"){
	  return IsBandCloseTime('s',cur_lastprice_,cur_middle_value_,cur_sd_val_,band_loss_close_edge_,band_profit_close_edge_,rsi_val_,limit_rsi_val_);
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
	cur_price_.multiple = 10;
	//cout<<cur_price_.BidPrice1<<endl;
	//cout<<pre_price_.LastPrice<<endl;

	if (pre_price_.LastPrice ==0)
	{
		pre_price_ = cur_price_;
		pre_rsi_lastprice_ = pre_price_.LastPrice;
		return;
	}
	if (pre_rsi_lastprice_ ==0)
	{
		pre_rsi_lastprice_ = cur_lastprice_;
	}
	if (now_rsi_bar_tick_ >= rsi_bar_period_)
	{
		double tmpdiff = cur_lastprice_ - pre_rsi_lastprice_;
		pre_rsi_lastprice_ = cur_lastprice_;
		now_rsi_bar_tick_ =1;
		rsi_val_ = GetRSIData(tmpdiff,rsi_vector_,rsi_period_);
		rsi_vector_.push_back(tmpdiff);
	}
	else{
		now_rsi_bar_tick_ +=1;
		double tmpdiff = cur_lastprice_ - pre_rsi_lastprice_;
		rsi_val_ = GetRSIData(tmpdiff,rsi_vector_,rsi_period_);
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
	//cout<<date<<endl;
	string insert = id +  ","+date+","+time+","+to_string(cur_lastprice_)
		+","+to_string(cur_middle_value_)+"," + to_string(cur_sd_val_)+","+ to_string(rsi_val_);
	//cout<<insert<<endl;
	WriteMesgToFile(path,insert);
	bool band_open_signal = IsTrendOpenTime();
	if (band_open_signal){
		cout<<"this is band open signal"<<endl;
	}
	bool band_close_signal =  IsTrendCloseTime();
	if(band_close_signal){
		cout<<"this is band close signal"<<endl;
	}
	pre_price_ = cur_price_;
}