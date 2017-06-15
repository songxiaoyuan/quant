#include "band.h"


band::band(void)
{
	compXaveNum_ =2;
	movingTheo_ = "MA";
	band_open_edge_ = 0.5;
	band_close_edge_ = 2;
	direction_ = "LONG";
}


band::~band(void)
{
	vector<double>().swap(vector_prices_);
}

double band::GetMAData(vector<double> &vector_prices_){
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

double band::GetSDData(vector<double> &vector_prices_){
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
	return sqrt(sum);
}

double band::GetEMAData(double price){
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



bool band::IsBandOpenTime(){
	if (direction_	==	"LONG")
	{
		double upval = cur_middle_value_ + cur_sd_val_ * band_open_edge_;
		if (cur_lastprice_ > cur_middle_value_ && cur_lastprice_ < upval)
		{
		 cout<<"this is open signal"<<endl;
	             cout<<"the price is "<<cur_lastprice_<<endl;
				cout<<"the middle val is "<<cur_middle_value_<<endl;
				   cout<<"the cur sd  is "<<cur_sd_val_<<endl;
	             cout<<"this middle val is  "<<cur_middle_value_<<endl;
                 cout<<"this up val is "<<upval<<endl;
			return true;
		}
	}
	else if (direction_	==	"SHORT")
	{
		double downval = cur_middle_value_ - cur_sd_val_ * band_open_edge_;
		if (cur_lastprice_ > downval && cur_lastprice_ < cur_middle_value_)
		{
			cout<<"this is open signal"<<endl;
	         cout<<"the price is "<<cur_lastprice_<<endl;
			  cout<<"the middle val is "<<cur_middle_value_<<endl;
				   cout<<"the cur sd  is "<<cur_sd_val_<<endl;
	          cout<<"this middle val is  "<<cur_middle_value_<<endl;
			  cout<<"this downval val is "<< downval<<endl;
			return true;
		}
	}
	return false;
}

bool band::IsBandCloseTime(){

	if (direction_	==	"LONG")
	{
		double profitval = cur_middle_value_ + cur_sd_val_ * band_close_edge_;
		double lossval = cur_middle_value_ - cur_sd_val_ * band_open_edge_;
		if (cur_lastprice_ > profitval || cur_lastprice_ < lossval)
		{
			/*
				  cout<<"this is close signal"<<endl;
	               cout<<"the price is "<<cur_lastprice_<<endl;
				   cout<<"the middle val is "<<cur_middle_value_<<endl;
				   cout<<"the cur sd  is "<<cur_sd_val_<<endl;
	               cout<<"this profitval val is  "<<profitval<<endl;
	               cout<<"this lossval val is "<<lossval<<endl; */
			return true;
		}
	}
	else if (direction_	==	"SHORT")
	{
		double profitval = cur_middle_value_ - cur_sd_val_ * band_close_edge_;
		double lossval = cur_middle_value_ + cur_sd_val_ * band_open_edge_;
		if (cur_lastprice_ > lossval || cur_lastprice_ < profitval)
		{
				/*  cout<<"this is close signal"<<endl;
	              cout<<"the price is "<<cur_lastprice_<<endl;
				  	cout<<"the middle val is "<<cur_middle_value_<<endl;
				   cout<<"the cur sd  is "<<cur_sd_val_<<endl;
	              cout<<"this profitval val is  "<<profitval<<endl;
	                cout<<"this lossval val is "<<lossval<<endl;  */
			return true;
		}
	}
	return false;
}


void band::getPrices(double price){
	cur_lastprice_ = price;
	if (vector_prices_.size() < compXaveNum_-1)
	{
		vector_prices_.push_back(price);
		double tmp = GetEMAData(price);
		return;
	}
	else{
		vector_prices_.push_back(price);
	}
	if (movingTheo_ == "MA"){
		cur_middle_value_ = GetMAData(vector_prices_);
	}
	else
	{
		cur_middle_value_ = GetEMAData(cur_lastprice_);
	}
	cur_sd_val_ = GetSDData(vector_prices_);

		string id = "pb1706";
	string time ="12:13:14";
	/*cout<<cur_lastprice_<<endl;
	cout<<cur_middle_value_<<endl;
	cout<<cur_sd_val_<<endl;
	*/
	string insert = id +  "," +time+","+to_string(cur_lastprice_)
		+","+to_string(cur_middle_value_)+"," + to_string(cur_sd_val_);
	cout<<insert<<endl;
	strToFile("test.txt",insert);
	
	bool band_open_signal = IsBandOpenTime();
	/*
	if (band_open_signal){

	}*/
	bool band_close_signal = IsBandCloseTime();
	/*
	if(band_close_signal){

	}*/
}

 void band::strToFile(string path,string error) {
	// char *filePath =path.data;
  ofstream file_in;
  file_in.open(path, std::ios::out | std::ios::app);
  if (! file_in.is_open())
  {
	  return;
  }
  file_in <<error<<endl;
  file_in.close();
}
