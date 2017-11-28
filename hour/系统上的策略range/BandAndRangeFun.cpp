#include "BandAndRangeFun.h"



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

void StartAndStopFun(Parameter_range *param,VolumeTrendRangeInfo *info,int param_index){
	int size = info->lastprice_vector_hour.size();
	info->pre_ema_val_5 = 0;
	info->pre_ema_val_60 = 0;
	info->now_middle5_bar_tick = 0;
	
	info->open_price = 0;
	info->max_profit = 0;

	info->has_open = 0;
	info->open_status = 0;
	info->pre_open_status = 0;
	info->current_minute = -1;
	info->current_minute_num = 0;

	if (info->lastprice_vector_hour.empty())
	{
		cout<<"the queue is empty and is init function"<<endl;
		GetConfigInfoSeries(info->pre_ema_val_60,info->pre_ema_val_5,info->lastprice_vector_hour,param->m_Param[param_index].arbitrageTypeID);
		PrintInfo(info->pre_ema_val_60,info->pre_ema_val_5,
			info->lastprice_vector_hour,param->m_Param[param_index].arbitrageTypeID);
	}
	else{
		cout<<"the queue is not empty and is the stop function"<<endl;
		info->lastprice_vector_hour.clear();
		info->pre_ema_val_5 = 0;
		info->pre_ema_val_60 = 0;
		info->now_middle5_bar_tick = 0;
		
		info->open_price = 0;
		info->max_profit = 0;

		info->has_open = 0;
		info->open_status = 0;
		info->pre_open_status = 0;
		info->current_minute = -1;
		info->current_minute_num = 0;
	}
}

BandAndRangeRetStatus GetMdData(Parameter_range *param,VolumeTrendRangeInfo *info,int param_index){
	BandAndRangeRetStatus ret;
	ret.isTrendCloseTime = false;
	ret.isTrendOpenTime = false;
	double middle_val_60;
	double middle_val_5;

	if (info->pre_ema_val_60 ==0)
	{
		info->pre_ema_val_60 = info->cur_price.LastPrice;
		info->pre_ema_val_5 = info->cur_price.LastPrice;
	}

	double lastprice = info->cur_price.LastPrice;
	int ema_period = param->m_Param[param_index].PositionAdj;
	int rsi_period = param->m_Param[param_index].AdjEmaFast;

	int limit_ema_tick_5 = param->m_Param[param_index+1].edgebWork;
	int limit_minute_num = param->m_Param[param_index+1].PositionAdj;

	middle_val_60 = GetEMADataSeries(lastprice,info->pre_ema_val_60,ema_period);
	middle_val_5 = GetEMADataSeries(lastprice,info->pre_ema_val_5,ema_period);

	double rsi_data = GetRSIDataSeries(lastprice,info->lastprice_vector_hour,rsi_period);

	try{
		string time = info->cur_price.updateTime;
	    string tmp = (time.substr(3,2));
		//cout<<tmp<<endl;
		int minute = atoi(tmp.c_str());
		if (minute != info->current_minute)
		{
			info->current_minute = minute;
			info->current_minute_num +=1;
			if (info->current_minute_num > limit_minute_num)
			{
				info->current_minute_num = 0;
				info->pre_ema_val_60 = middle_val_60;
			    info->lastprice_vector_hour.push_back(lastprice);
				//limit the time num
				info->has_open = 0;
			}
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
	
	ret.isTrendOpenTime = IsOpenTime(middle_val_60,middle_val_5,param,info,param_index);
	ret.isTrendCloseTime = IsCloseTime(middle_val_60,middle_val_5,param,info,param_index);
	return ret;
}



bool IsBandOpenTimeSeries(VolumeTrendRangeInfo* info,double lastprice,double middle,double open_edge1,double open_edge_increase){
	if (info->direction =='l')
	{
		double startval = middle + open_edge1*(info->price_tick);
		double upval = middle + (open_edge1+open_edge_increase)*(info->price_tick);
		if (lastprice < startval)
		{
			info->pre_open_status = 1;
			return false;
		}
		else if (lastprice > upval)
		{
			info->pre_open_status = -1;
			return false;
		}
		else
		{
			if (info->pre_open_status ==1)
			{
				return true;
			}
			else{
				return false;
			}
		}
	}
	else if (info->direction	==	's')
	{
		double startval = middle - open_edge1*(info->price_tick);
		double upval = middle - (open_edge1+open_edge_increase)*(info->price_tick);
		if (lastprice > startval)
		{
			info->pre_open_status = 1;
			return false;
		}
		else if (lastprice < upval)
		{
			info->pre_open_status = -1;
			return false;
		}
		else
		{
			if (info->pre_open_status ==1)
			{
				if (info->open_price ==0)
				{
					cout<<"the middle is: "<<middle<<" the lastprice is :"<<lastprice<<endl;
				}
				return true;
			}
			else{
				return false;
			}
		}
	}
	return false;
}

bool IsBandCloseTimeSeries(VolumeTrendRangeInfo* info,double lastprice,double middle
					 ,double loss_band,double profit_band){
	/*cout<<"the limit sd is"<<limit_sd<<endl;
	cout<<"the loss band is :"<<loss_band<<endl;
	cout<<"the profit_band band is :"<<profit_band<<endl;
	cout<<"the limit_sd_loss_band band is :"<<limit_sd_loss_band<<endl;*/
	if (info->direction	==	'l')
	{
		double profitval = middle + profit_band*(info->price_tick);
		double lossval = middle - loss_band*(info->price_tick);
		if ( lastprice < lossval)
		{
			return true;
		}
		if (lastprice > profitval)
		{
			//this is used to jude the range is completed
			info->open_status = 1;
		}
	}
	else if (info->direction	==	's')
	{
		double profitval = middle - profit_band*(info->price_tick);
		double lossval = middle + loss_band*(info->price_tick);
		if (lastprice > lossval)
		{
			cout<<"the middle is: "<<info->pre_ema_val_60<<" the lastprice is :"<<lastprice<<endl;
			return true;
		}
		if (lastprice < profitval)
		{
			info->open_status = 1;
		}
	}
	return false;
}

bool IsMiddleCrossCloseTime(VolumeTrendRangeInfo *info,double lastprice,double middle_val_5){
	if (info->direction	==	'l')
	{
		if ( lastprice <  middle_val_5 && info->open_status ==1)
		{
			
			return true;
		}
	}
	else if (info->direction ==	's')
	{
		if ( lastprice >  middle_val_5  && info->open_status ==1)
		{
			cout<<"the middle is: "<<info->pre_ema_val_60<<" the lastprice is :"<<lastprice<<endl;
			return true;
		}
	}
	return false;
}


bool IsLimitTimeOpenTimeSeries(Parameter_range *param,VolumeTrendRangeInfo *info,int param_index){
	int limit_open_time = param->m_Param[param_index+1].spread;
	if (info->has_open >= limit_open_time)
	{
			return false;
		}
	else{
		return true;
	}
};


bool IsOpenTime(double middle_val,double middle_val_5,Parameter_range *param,VolumeTrendRangeInfo *info,int param_index){
	
	//band_open_noraml
	int band_open_edge =  param->m_Param[param_index+1].openEdge;
	int band_open_increase =  param->m_Param[param_index+1].closeEdge;

	double lastprice = info->cur_price.LastPrice;

	bool is_time_open = IsLimitTimeOpenTimeSeries(param,info,param_index);
	if (is_time_open == false)
	{
		return false;
	}
	bool is_band_open = IsBandOpenTimeSeries(info,lastprice,middle_val,band_open_edge,band_open_increase);
	return is_band_open;
}

bool IsCloseTime(double middle_val,double middle_val_5,Parameter_range *param,VolumeTrendRangeInfo *info,int param_index){
	if (info->open_price ==0)
	{
		return false;
	}
	//band_loss_close_edge
	int band_loss_edge = param->m_Param[param_index+1].EdgeAdj;
	//band_profit_close_edge
	int band_profit_edge =param->m_Param[param_index+1].cancelEdge;

	double lastprice = info->cur_price.LastPrice;

	bool is_band_close = IsBandCloseTimeSeries(info,lastprice,middle_val,band_loss_edge,band_profit_edge);
	if (is_band_close)
	{
		return true;
	}

	bool is_middle_cross_close = IsMiddleCrossCloseTime(info,lastprice,middle_val_5);
	return is_middle_cross_close;
}


void GetOpenSignal(VolumeTrendRangeInfo *info){
	info->has_open +=1;
	info->open_price = info->cur_price.LastPrice;
}
void GetCloseSignal(VolumeTrendRangeInfo *info){
	info->max_profit = 0;
	info->open_status = 0;
	info->open_price = 0;
}