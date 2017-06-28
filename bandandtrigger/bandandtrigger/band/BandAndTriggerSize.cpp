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
	compXaveNum_ =3600;
	movingTheo_ = "EMA";
	band_open_edge_ = 0.5;
    band_loss_close_edge_= 1;
	band_profit_close_edge_ =3;
	direction_ = "LONG";
	position_ =0;
	price_spread_edge_ = 100;
	volume_edge_ =900 ;
	openinterest_edge_ = 0;

	open_price_ = 0;
	max_profit_ = 0;
	limit_max_draw_down_ = 0;

	now_interest_ =0;
	limit_interest_ =1;
	multiple_val_ =10;

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
	time_ = "0";
}


BandAndTriggerSize::~BandAndTriggerSize(void)
{
	vector<double>().swap(vector_prices_);
}

bool BandAndTriggerSize::IsTrendOpenTime(){
	if (direction_ =="LONG")
	{
		bool is_band_open = IsBandOpenTime('l',cur_lastprice_,cur_middle_value_,cur_sd_val_,band_open_edge_);
		if(is_band_open == false){
		  return false;
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
	if (now_interest_ <=0)
	{
		return false;
	}
	if(direction_ =="LONG"){
		bool tmp_draw_down = IsMaxDrawDown('l',cur_lastprice_,open_price_,multiple_val_,max_profit_,limit_max_draw_down_);
		//cout<<max_profit_<<endl;
		if (tmp_draw_down ==true)
		{
			return true;
		}
	  return IsBandCloseTime('l',cur_lastprice_,cur_middle_value_,cur_sd_val_,band_loss_close_edge_,band_profit_close_edge_,rsi_val_,limit_rsi_val_);
	}
	else if (direction_ =="SHORT"){
		bool tmp_draw_down = IsMaxDrawDown('s',cur_lastprice_,open_price_,multiple_val_,max_profit_,limit_max_draw_down_);
		if (tmp_draw_down ==true)
		{
			return true;
		}
	  return IsBandCloseTime('s',cur_lastprice_,cur_middle_value_,cur_sd_val_,band_loss_close_edge_,band_profit_close_edge_,rsi_val_,limit_rsi_val_);
	}
	return false;
	
}

void BandAndTriggerSize::getPrices(vector<string> &line_data){
	cur_lastprice_ = atof(line_data[LASTPRICE].c_str());
	cur_price_.Volume = atof(line_data[VOLUME].c_str());
	cur_price_.LastPrice = atof(line_data[LASTPRICE].c_str());
	cur_price_.OpenInterest =atof(line_data[OPENINTEREST].c_str());
	cur_price_.AskPrice1 =atof(line_data[ASKPRICE1].c_str());
	cur_price_.BidPrice1 = atof(line_data[BIDPRICE1].c_str());
	cur_price_.multiple = multiple_val_;
	cur_price_.Turnover = atof(line_data[TURNONER].c_str());
	//cout<<cur_price_.BidPrice1<<endl;
	//cout<<pre_price_.LastPrice<<endl;
	time_ = line_data[UpdateTime];
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

	//cout<<line_data[UpdateTime]<<endl;
	string path=line_data[InstrumentID]+"_"+line_data[TradingDay];

	
	string insert = line_data[UpdateTime]+ ","+to_string(cur_lastprice_)
		+","+to_string(cur_middle_value_)+"," + to_string(cur_sd_val_)+","+ to_string(rsi_val_);

	//WriteMesgToFile(path,insert);
	bool band_open_signal = IsTrendOpenTime();
	bool band_close_signal =  IsTrendCloseTime();


	if (band_open_signal && now_interest_ < limit_interest_){
		cout<<"this is band open signal "<<line_data[UpdateTime] <<" the price "<<line_data[LASTPRICE]<<endl;
		now_interest_ +=1;
		open_price_ = atof(line_data[LASTPRICE].c_str());
	}
	else if(band_close_signal && now_interest_ >0){
		cout<<"this is band close signal "<<line_data[UpdateTime]<<" the price "<<line_data[LASTPRICE]<<endl;
		now_interest_ -=1;
		open_price_ =0;
		max_profit_ = 0;
	}
	pre_price_ = cur_price_;
}