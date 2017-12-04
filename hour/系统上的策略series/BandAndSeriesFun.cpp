#include "BandAndSeriesFun.h"


double GetSDDataSeries(double lastprice,vector<double> &vector_prices,int period){
	int size = vector_prices.size();
	if (size ==0 || period ==0)
	{
		return 0;
	}
	double sum = 0;
	for (int i = size-1; i >=0 && i > size - period; i--)
	{
		sum +=vector_prices[i];
	}
	sum += lastprice;
	double avg = sum/period;
	sum = 0;
	for (int i = size-1; i >=0 && i > size - period; i--)
	{
		sum += (vector_prices[i]-avg)*(vector_prices[i]-avg);
	}
	sum += (lastprice - avg)*(lastprice - avg);
	if (vector_prices.size() < period)
	{
		return sqrt(sum/vector_prices.size());
	}
	return sqrt(sum/(period-1));
}

double GetEMADataSeries(double price,double pre_ema_val,int period){
	if (period <=1)
	{
		return price;
	}
	double ret = ((period - 1)*pre_ema_val + 2*price)/(period + 1);
	return ret;
}

double GetRSIDataSeries(double lastprice,vector<double> &lastprice_vector,int period){
	if (period == 0 || lastprice_vector.size() ==0){
		return 50;
	}
	double rise = 0;
	double total = 0;
	double tmp = lastprice - lastprice_vector[lastprice_vector.size()-1];
	if (tmp >0) {
		rise +=tmp;
		total +=tmp;
	}
	else{
		total -=tmp;
	}
	int size = lastprice_vector.size();
	for (int i = size - 2; i>=0 && i>=size - period; i--)
	{
		double tmp = lastprice_vector[i+1] - lastprice_vector[i];
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

bool IsBandOpenTimeSeries(char direction,double lastprice,double middle,double open_edge1,double open_edge2){
	if (direction =='l')
	{
		double startval = middle + open_edge1;
		double upval = middle + open_edge2;
		if (lastprice >= startval && lastprice <= upval)
		{
			return true;
		}
	}
	else if (direction	==	's')
	{
		double startval = middle - open_edge1;
		double downval = middle - open_edge2;
		if (lastprice >= downval && lastprice <= startval)
		{
			return true;
		}
	}
	return false;
}

bool IsBandCloseTimeSeries(char direction,double lastprice,double middle,double sd
					 ,double loss_band,double profit_band,double rsi_data,double limit_rsi_data){
	/*cout<<"the limit sd is"<<limit_sd<<endl;
	cout<<"the loss band is :"<<loss_band<<endl;
	cout<<"the profit_band band is :"<<profit_band<<endl;
	cout<<"the limit_sd_loss_band band is :"<<limit_sd_loss_band<<endl;*/
	if (direction	==	'l')
	{
		double profitval = middle + sd * profit_band;
		double lossval = middle - loss_band;
		if ( lastprice < lossval)
		{
			return true;
		}
		if (lastprice > profitval && rsi_data > limit_rsi_data)
		{
			return true;
		}
	}
	else if (direction	==	's')
	{
		double profitval = middle - sd * profit_band;
		double lossval = middle + loss_band;
		rsi_data = 100 - rsi_data;
		if (lastprice > lossval)
		{
			return true;
		}
		if (lastprice < profitval && rsi_data > limit_rsi_data)
		{
			return true;
		}
	}
	return false;
}

bool IsMiddleCrossCloseTime(VolumeTrendSeriesInfo *info,double lastprice,double middle_val_5,double cross_middle_edge){
	if (info->direction	==	'l')
	{
		if ( lastprice <  middle_val_5 - cross_middle_edge)
		{
			if (info->open_status ==1)
			{
				return false;
			}
			else{
				return true;
			}
		}
		if (lastprice > middle_val_5 + cross_middle_edge)
		{
			info->open_status = 0;
		}
	}
	else if (info->direction ==	's')
	{
		if ( lastprice >  middle_val_5 + cross_middle_edge)
		{
			if (info->open_status ==1)
			{
				return false;
			}
			else{
				return true;
			}
		}
		if (lastprice < middle_val_5 - cross_middle_edge)
		{
			info->open_status = 0;
		}
	}
	return false;
}

void WriteMesgToFile(string path,string mesg){
  //return;
  FILE *file_fd = fopen((char*)path.c_str(),"a");
  char tmp[1024] = {0};
  sprintf(tmp,"%s\n",mesg.c_str());
  int write_len = fwrite(tmp,1,strlen(tmp),file_fd);
  fclose(file_fd);
}

void PrintInfo(double &pre_ema_val_60,double &pre_ema_val_5,
			   vector<double> &lastprice_vector,int config_file_path){
    cout<<"the pre ema val 60 is: "<<pre_ema_val_60<<endl;
	cout<<"the pre ema val 5 is: "<<pre_ema_val_5<<endl;
	cout<<"the size of lastprice prices is : " <<lastprice_vector.size()<<endl;
	cout<<"the path is  : " <<config_file_path<<endl;
}


string double2str(const double &int_temp)
{
        stringstream stream;
        stream<<int_temp;
        return stream.str();
}
 
double str2double(const string &string_temp)
{
	stringstream stream(string_temp);
	double double_temp;
	stream>>double_temp;
	return double_temp;
}

void split(std::string& s, std::string& delim,std::vector<double> &ret)  
{  
	//"INFO:: remove the first one and return the double"
    size_t last = 0;  
    size_t index=s.find_first_of(delim,last);  
    while (index!=std::string::npos)  
    {  
		string tmp = (s.substr(last,index-last));
		ret.push_back(str2double(tmp));  
        last=index+1;  
        index=s.find_first_of(delim,last);  
    }  
    if (index-last>0)  
	{ 
		string tmp = s.substr(last,index-last);
        ret.push_back(str2double(tmp));  
    }  
} 

void GetConfigInfoSeries(double &pre_ema_val_60,double &pre_ema_val_5,
				   vector<double> &lastprice_vector,int config_file_path){
	char path[256]={0};
	sprintf(path,"band_and_triggersize_config/%d",config_file_path);
	FILE *file_fd = fopen(path,"r");
	if (NULL ==file_fd)
	{
		cout<<"cant find the config file"<<endl;
		pre_ema_val_60 = 0;
		pre_ema_val_5 = 0;
		lastprice_vector.clear();
		return;
	}
	char tmp[1024*100] = {0};
	while(!feof(file_fd)){
		memset(tmp, 0, sizeof(tmp));
		fgets(tmp, sizeof(tmp) - 1, file_fd);
		//cout<<tmp<<endl;
		size_t index = ((string)tmp).find_first_of(",",0);
		string front = ((string)tmp).substr(0,index);
		string content = ((string)tmp).substr(index+1,((string)tmp).size());
		vector<double> tmp_vector;
		string tmp = ",";
		split(content,tmp,tmp_vector);
		cout<<front<<endl;
		if (front.compare("pre_ema_val_60:")==0)
		{
			cout<<"in the pre_ema_val_60"<<endl;
			//cout<<front<<endl;
			if (tmp_vector.size() >0)
			{
				pre_ema_val_60 = tmp_vector[0];
			}
			else{
			  cout<<"the pre_ema_val_60 is wrong"<<endl;
			}
		}
		else if (front.compare("pre_ema_val_5:")==0)
		{
			cout<<"in the pre_ema_val_5"<<endl;
			//cout<<front<<endl;
			if (tmp_vector.size() >0)
			{
				pre_ema_val_5 = tmp_vector[0];
			}
			else{
			  cout<<"the pre_ema_val_5 is wrong"<<endl;
			}
		}
		else if (front.compare("lastprice_array:")==0)
		{
			cout<<"in the lastprice_array"<<endl;
			//cout<<front<<endl;
			cout<<"the size of lastprice_array is: "<<tmp_vector.size()<<endl;
			if (tmp_vector.size() >0)
			{
				for (int i = 0; i < tmp_vector.size(); i++)
				{
					lastprice_vector.push_back(tmp_vector[i]);
				}
			}
			else{
			  cout<<"the rsiarrray is wrong"<<endl;
			}
		}
		else
		{
			cout<<"the line is no needed"<<endl;
		}
	}
	fclose(file_fd);
}

void StartAndStopFun(Parameter_series *param,VolumeTrendSeriesInfo *info,int param_index){
	int size = info->lastprice_vector_hour.size();
	info->pre_ema_val_5 = 0;
	info->pre_ema_val_60 = 0;
	info->open_status = 0;
	info->now_middle5_bar_tick = 0;
	
	info->tmp_sum_diff_volume = 0;
	info->lastprice_bar_tick = 0;
	info->current_hour_line = 9;
	info->current_hour_open = 0;
	info->has_open = 0;

	info->open_price = 0;
	info->max_profit = 0;

	if (info->lastprice_vector_hour.empty())
	{
		cout<<"the queue is empty and is init function series"<<endl;
		GetConfigInfoSeries(info->pre_ema_val_60,info->pre_ema_val_5,info->lastprice_vector_hour,param->m_Param[param_index].arbitrageTypeID);
		PrintInfo(info->pre_ema_val_60,info->pre_ema_val_5,
			info->lastprice_vector_hour,param->m_Param[param_index].arbitrageTypeID);
	}
	else{
		cout<<"the queue is not empty and is the stop function series"<<endl;
		info->lastprice_vector_hour.clear();
		info->pre_ema_val_5 = 0;
		info->pre_ema_val_60 = 0;
		info->open_status = 0;
		info->now_middle5_bar_tick = 0;
		
		info->tmp_sum_diff_volume = 0;
		info->lastprice_bar_tick = 0;
		info->current_hour_line = 9;
		info->current_hour_open = 0;
		info->has_open = 0;

		info->open_price = 0;
		info->max_profit = 0;
	}
}

BandAndSeriesRetStatus GetMdData(Parameter_series *param,VolumeTrendSeriesInfo *info,int param_index){
	BandAndSeriesRetStatus ret;
	ret.isTrendCloseTime = false;
	ret.isTrendOpenTime = false;
	double middle_val_60;
	double middle_val_5;
	double middle_val_1;
	double sd_val;
	double rsi_data;

	if (info->pre_ema_val_60 ==0)
	{
		info->pre_ema_val_60 = info->cur_price.LastPrice;
		info->pre_ema_val_5 = info->cur_price.LastPrice;
	}

	double lastprice = info->cur_price.LastPrice;
	int ema_period = param->m_Param[param_index].PositionAdj;
	int rsi_period = param->m_Param[param_index].AdjEmaFast;

	int limit_ema_tick_5 = param->m_Param[param_index+1].edgebWork;

	middle_val_60 = GetEMADataSeries(lastprice,info->pre_ema_val_60,ema_period);
	middle_val_5 = GetEMADataSeries(lastprice,info->pre_ema_val_5,ema_period);

	sd_val = GetSDDataSeries(lastprice,info->lastprice_vector_hour,ema_period);
	rsi_data = GetRSIDataSeries(lastprice,info->lastprice_vector_hour,rsi_period);

	try{
		string time = info->cur_price.updateTime;
	    string tmp = (time.substr(0,2));
		int now_hour = atoi(tmp.c_str());
		if (now_hour != info->current_hour_line && now_hour != 13 && now_hour != 21)
		{
			info->current_hour_line = now_hour;
			info->pre_ema_val_60 = middle_val_60;
			info->lastprice_vector_hour.push_back(lastprice);
		}
	}
	catch(exception e){
		cout<<"the time convert to int is wrong"<<endl;
		return ret;
	}

	if (info->now_middle5_bar_tick >= limit_ema_tick_5)
	{
		info->now_middle5_bar_tick = 0;
		info->pre_ema_val_5 = middle_val_5;
	}
	else
	{
		info->now_middle5_bar_tick +=1;
	}
	
	int limit_bar_tick = param->m_Param[param_index].spread;

	double diff_volume = info->cur_price.Volume - info->pre_price.Volume;
	info->tmp_sum_diff_volume += diff_volume;
	info->lastprice_bar_tick +=1;
	if (info->lastprice_bar_tick >= limit_bar_tick)
	{
		info->diff_volume_vector.push_back(info->tmp_sum_diff_volume);
		info->lastprice_vector_series.push_back(info->cur_price.LastPrice);
		info->tmp_sum_diff_volume = 0;
		info->lastprice_bar_tick = 0;
	}

	ret.isTrendOpenTime = IsOpenTime(middle_val_60,middle_val_5,param,info,param_index);
	ret.isTrendCloseTime = IsCloseTime(middle_val_60,sd_val,rsi_data,middle_val_5,param,info,param_index);
	return ret;
}

bool IsLimitTimeOpenTimeSeries(Parameter_series *param,VolumeTrendSeriesInfo *info,int param_index){
	int now_hour = 9;
	try{
		string time = info->cur_price.updateTime;
	    string tmp = (time.substr(0,2));
		now_hour = atoi(tmp.c_str());
	}
	catch(exception e){
		cout<<"the time convert to int is wrong"<<endl;
		return false;
	}
	int limit_open_time = param->m_Param[param_index+1].spread;
	if (info->current_hour_open == now_hour)
	{
		if (info->has_open >= limit_open_time)
		{
			return false;
		}
	}
	else{
		info->current_hour_open = now_hour;
		if (now_hour == 13)
		{
			if (info->has_open >= limit_open_time)
		    {
			     return false;
		    }
		}
		else{
			info->has_open = 0;
		}
	}
	return true;
};

bool IsDiffVolumeOpenTime(int tmp_sum_diff_volume,vector<int> &diff_volume_vector,int limit_multiple,int limit_large_period){
	int left_index = diff_volume_vector.size() - limit_large_period;
	if (diff_volume_vector.size() ==0 || left_index < 0 )
	{
		return false;
	}
	for (int i = left_index +1; i < diff_volume_vector.size(); i++)
	{
		int tmp = diff_volume_vector[left_index] * limit_multiple;
		if (tmp > diff_volume_vector[i])
		{
			return false;
		}
	}
	if ((diff_volume_vector[left_index] * limit_multiple) > tmp_sum_diff_volume)
	{
		return false;
	}
	return true;

}
bool IsLastpriceOpenTime(char direction,double lastprice,vector<double> &lastprice_vector,int limit_large_period){
	int left_index = lastprice_vector.size() - limit_large_period;
	if (lastprice_vector.size() ==0 || left_index < 0)
	{
		return false;
	}
	int series =0;
	if (direction =='l')
	{
		for (int i = left_index +1; i < lastprice_vector.size(); i++){
			if ( lastprice_vector[i] >= lastprice_vector[left_index])
			{
				series +=1;
			}
			else{
				return false;
			}
        }
		if (lastprice >= lastprice_vector[left_index])
		{
			return true;
		}

	}
	else if (direction == 's' )
	{
		for (int i = left_index +1; i < lastprice_vector.size(); i++){
			if ( lastprice_vector[i] <= lastprice_vector[left_index])
			{
				series +=1;
			}
			else{
				return false;
			}
        }
		if (lastprice <= lastprice_vector[left_index])
		{
			return true;
		}

	}
	else
	{
		return false;
	}
	return false;

}

bool IsOpenTime(double middle_val,double middle_val_5,Parameter_series *param,VolumeTrendSeriesInfo *info,int param_index){
	
	//band_open_noraml
	double band_open_edge1 =  ((double)param->m_Param[param_index+1].openEdge)/10;
	double band_open_edge2 =  ((double)param->m_Param[param_index+1].closeEdge)/10;

	double lastprice = info->cur_price.LastPrice;

	bool is_time_open = IsLimitTimeOpenTimeSeries(param,info,param_index);
	if (is_time_open == false)
	{
		return false;
	}
	bool is_band_open = IsBandOpenTimeSeries(info->direction,lastprice,middle_val,band_open_edge1,band_open_edge2);
	if (is_band_open ==false)
	{
		return false;
	}

	
	int limit_large_period = param->m_Param[param_index].openEdge;
	int limit_multiple = param->m_Param[param_index].closeEdge;

	bool is_diff_volume_open = IsDiffVolumeOpenTime(info->tmp_sum_diff_volume,info->diff_volume_vector,limit_multiple,limit_large_period);

	if (is_diff_volume_open ==false)
	{
		return false;
	}

	bool is_lastprice_open = IsLastpriceOpenTime(info->direction,lastprice,info->lastprice_vector_series,limit_large_period);
	if (is_lastprice_open == true)
	{
		if (info->direction =='l')
		{
			if (lastprice > middle_val && lastprice < middle_val_5 )
			{
				info->open_status = 1;
			}
		}
		else if (info->direction == 's')
		{
			if (lastprice < middle_val && lastprice > middle_val_5)
			{
				info->open_status =1;
			}
		}
		else{
		  return false;
		}
		return true;

	}
	return false;
}

bool IsCloseTime(double middle_val,double sd_val,double rsi_val,double middle_val_5,Parameter_series *param,VolumeTrendSeriesInfo *info,int param_index){
	//band_loss_close_edge
	double band_loss_edge = ((double)param->m_Param[param_index+1].EdgeAdj)/10;
	//band_profit_close_edge
	double band_profit_edge =((double) param->m_Param[param_index+1].cancelEdge)/10;

	//limit_rsi
	double limit_rsi = param->m_Param[param_index].AdjEmaSlow;


	double cross_middle_edge  =((double) param->m_Param[param_index+1].orderDelay)/10;

	double limit_max_profit  =(double) param->m_Param[param_index+1].edgePrice;


	double lastprice = info->cur_price.LastPrice;
	bool is_band_close = IsBandCloseTimeSeries(info->direction,lastprice,middle_val,sd_val,band_loss_edge,band_profit_edge,rsi_val,limit_rsi);
	if (is_band_close)
	{
		return true;
	}

	bool is_middle_cross_close = IsMiddleCrossCloseTime(info,lastprice,middle_val_5,cross_middle_edge);
	double tmp_profit;
	if (info->direction == 'l' && info->open_price != 0)
	{
		tmp_profit = lastprice - info->open_price;
	}
	else if (info->direction == 's' && info->open_price != 0)
	{
		tmp_profit = info->open_price - lastprice;
	}
	else
	{
		return false;
	}
	tmp_profit = tmp_profit*(info->multiple);
	if (info->max_profit < tmp_profit)
	{
		info->max_profit = tmp_profit;
	}
	if (is_middle_cross_close == true)
	{
		if (info->max_profit > limit_max_profit)
		{
			return true;
		}
	}
	return false;
}


void GetOpenSignal(VolumeTrendSeriesInfo *info){
	info->has_open +=1;
	info->open_price = info->cur_price.LastPrice;
}
void GetCloseSignal(VolumeTrendSeriesInfo *info){
	info->max_profit = 0;
	info->open_price = 0;
}