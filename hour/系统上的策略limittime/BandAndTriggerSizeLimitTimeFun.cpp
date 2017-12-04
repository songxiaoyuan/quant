#include "BandAndTriggerSizeLimitTimeFun.h"

double GetSDData(double lastprice,vector<double> &vector_prices,int period){
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

double GetEMAData(double price,double pre_ema_val,int period){
	if (period <=1)
	{
		return price;
	}
	double ret = ((period - 1)*pre_ema_val + 2*price)/(period + 1);
	return ret;
}

double GetRSIData(double lastprice,vector<double> &lastprice_vector,int period){
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

bool IsBandOpenTime(char direction,double lastprice,double middle,double open_edge1,double open_edge2){
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

bool IsBandCloseTime(char direction,double lastprice,double middle,double sd
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

bool IsMiddleCrossCloseTime(VolumeTrendLimitTimeInfo *info,double lastprice,double middle_val_5,double cross_middle_edge){
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

bool IsTriggerSizeOpenTime(char direction,mdPrice_limittime *now_price,mdPrice_limittime *pre_price,int multiple
				,double volume_edge,double spread){

	if (now_price->Volume - pre_price->Volume < volume_edge )
	 {
		return false;
	}
	if (direction =='l')
	{
		return IsUpTime(now_price,pre_price,spread,multiple);
	}
	else if (direction =='s')
	{
		return IsDownTime(now_price,pre_price,spread,multiple);
	}
	return false;
}

bool IsTriggerSizeCloseTime(char direction,mdPrice_limittime *now_price,mdPrice_limittime *pre_price,int multiple
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

bool IsDownTime(mdPrice_limittime *now_price,mdPrice_limittime *pre_price,int spread,int multiple){
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

bool IsUpTime(mdPrice_limittime *now_price,mdPrice_limittime *pre_price,int spread,int multiple)
{
	int diffVolume	=	now_price->Volume	-	pre_price->Volume;  //·µ»Ø³Ö²ÖÁ¿µÄ±ä»¯
	double diffTurnover	=	now_price->Turnover	-	pre_price->Turnover;  //·µ»Ø³É½»½ð¶îµÄ±ä»¯

	if (diffVolume	==	0	||	diffTurnover	==	0	||	multiple	==	0)
	{
		return false;
	}
	double avePrice	=	diffTurnover/diffVolume/multiple;

	double temp	=	100*(avePrice	-	pre_price->BidPrice1)/(pre_price->AskPrice1 - pre_price->BidPrice1);

	/*
	string path = "test.txt";
	char tempchar[1024]={0};
	sprintf(tempchar,"%s,%d,%.2f,%.2f,%.2f",now_price->updateTime,diffVolume,diffTurnover,temp);
	WriteMesgToFile((string)path,(string)tempchar);
	*/

	if (temp >=	spread)
	{
		return true;

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

void GetConfigInfo(double &pre_ema_val_60,double &pre_ema_val_5,
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
			  cout<<"the  lastprice_array is wrong"<<endl;
			}
		}
		else
		{
			cout<<"the line is no needed"<<endl;
		}
	}
	fclose(file_fd);
}

void StartAndStopFun(Parameter_limittime *param,VolumeTrendLimitTimeInfo *info,int param_index){
	int size = info->lastprice_vector.size();

	info->pre_ema_val_60 = 0;
	info->pre_ema_val_5 = 0;
	info->now_middle5_bar_tick = 0;
	info->open_status = 0;
    info->current_hour_line = 9;
	info->current_hour_open = 0;
	info->has_open = 0;
	info->open_price = 0;
	info->open_status = 0;
	info->max_profit = 0;
	if (info->lastprice_vector.empty())
	{
		cout<<"the queue is empty and is init function limit time"<<endl;
		GetConfigInfo(info->pre_ema_val_60,info->pre_ema_val_5,info->lastprice_vector,param->m_Param[param_index].arbitrageTypeID);
		PrintInfo(info->pre_ema_val_60,info->pre_ema_val_5,
			info->lastprice_vector,param->m_Param[param_index].arbitrageTypeID);
	}
	else{
		cout<<"the queue is not empty and is the stop function limit time"<<endl;
		info->lastprice_vector.clear();
		info->lastprice_queue = queue<double>();
		info->pre_ema_val_60 = 0;
		info->pre_ema_val_5 = 0;
		info->now_middle5_bar_tick = 0;
		info->open_status = 0;
	    info->current_hour_line = 9;
		info->current_hour_open = 0;
		info->has_open = 0;
		info->open_price = 0;
		info->open_status = 0;
	}
}

BandAndTriggerSizeLimitTimeRetStatus GetMdData(Parameter_limittime *param,VolumeTrendLimitTimeInfo *info,int param_index){
	BandAndTriggerSizeLimitTimeRetStatus ret;
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

	middle_val_60 = GetEMAData(lastprice,info->pre_ema_val_60,ema_period);
	middle_val_5 = GetEMAData(lastprice,info->pre_ema_val_5,ema_period);

	sd_val = GetSDData(lastprice,info->lastprice_vector,ema_period);
	rsi_data = GetRSIData(lastprice,info->lastprice_vector,rsi_period);

	try{
		string time = info->cur_price.updateTime;
	    string tmp = (time.substr(0,2));
		int now_hour = atoi(tmp.c_str());
		if (now_hour != info->current_hour_line && now_hour != 13 && now_hour != 21)
		{
			info->current_hour_line = now_hour;
			info->pre_ema_val_60 = middle_val_60;
			info->lastprice_vector.push_back(lastprice);
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

	int queuetick_num = param->m_Param[param_index+1].compXave;
	info->lastprice_queue.push(lastprice);
	if (info->lastprice_queue.size() > queuetick_num)
	{
		info->lastprice_queue.pop();
	}
	ret.isTrendOpenTime = IsOpenTimeLimitTime(middle_val_60,middle_val_5,param,info,param_index);
	ret.isTrendCloseTime = IsCloseTimeLimitTime(middle_val_60,sd_val,rsi_data,middle_val_5,param,info,param_index);

	return ret;
}

bool IsLimitTimeOpenTime(Parameter_limittime *param,VolumeTrendLimitTimeInfo *info,int param_index){
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

bool IsOpenTimeLimitTime(double middle_val_60,double middle_val_5,Parameter_limittime *param,VolumeTrendLimitTimeInfo *info,int param_index){
	
	//band_open_noraml
	double band_open_edge1 =  ((double)param->m_Param[param_index+1].openEdge)/10;
	double band_open_edge2 =  ((double)param->m_Param[param_index+1].closeEdge)/10;

	//spread
	double spread = param->m_Param[param_index].spread;
	//diff_volume
	double diff_volume = param->m_Param[param_index].openEdge;

	double lastprice = info->cur_price.LastPrice;

	//ÅÐ¶ÏÊÇ²»ÊÇ¼±ÕÇ¼±µøµÄÇé¿ö£¬¼±ÕÇÊÇ²»³­µ×µÄ¡£
	if (info->direction =='l')
	{
		if(info->lastprice_queue.size() != 0){
			double pre_lastprice = info->lastprice_queue.front();
			if (lastprice < pre_lastprice)
			{
				return false;
			}
		}
	}
	else if (info->direction == 's')
	{
		if(info->lastprice_queue.size() != 0){
			double pre_lastprice = info->lastprice_queue.front();
			if (lastprice > pre_lastprice)
			{
				return false;
			}
		}
	}
	else{
		  return false;
	}
	bool is_time_open = IsLimitTimeOpenTime(param,info,param_index);
	if (is_time_open == false)
	{
		return false;
	}

	bool is_band_open = IsBandOpenTime(info->direction,lastprice,middle_val_60,band_open_edge1,band_open_edge2);
	if (is_band_open ==false)
	{
		return false;
	}

	bool is_trigger_open = IsTriggerSizeOpenTime(info->direction,&info->cur_price,&info->pre_price,info->multiple,
		diff_volume,spread);

	if (is_trigger_open == true)
	{
		if (info->direction =='l')
		{
			if (lastprice > middle_val_60 && lastprice < middle_val_5 )
			{
				info->open_status = 1;
			}
		}
		else if (info->direction == 's')
		{
			if (lastprice < middle_val_60 && lastprice > middle_val_5)
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

bool IsCloseTimeLimitTime(double middle_val,double sd_val,double rsi_val,double middle_val_5,Parameter_limittime *param,VolumeTrendLimitTimeInfo *info,int param_index){
	//band_loss_close_edge
	double band_loss_edge = ((double)param->m_Param[param_index+1].EdgeAdj)/10;
	//band_profit_close_edge
	double band_profit_edge =((double) param->m_Param[param_index+1].cancelEdge)/10;

	//limit_rsi
	double limit_rsi = param->m_Param[param_index].AdjEmaSlow;

	double cross_middle_edge  =((double) param->m_Param[param_index+1].orderDelay)/10;
	double limit_max_profit  =(double) param->m_Param[param_index+1].edgePrice;

	double lastprice = info->cur_price.LastPrice;
    
	bool is_band_close = IsBandCloseTime(info->direction,lastprice,middle_val,sd_val,band_loss_edge,band_profit_edge,rsi_val,limit_rsi);
	if (is_band_close)
	{
		return true;
	}

	bool is_middle_cross_close = IsMiddleCrossCloseTime(info,lastprice,middle_val_5,cross_middle_edge);
	double tmp_profit;
	if (info->direction == 'l'  && info->open_price != 0 )
	{
		tmp_profit = lastprice - info->open_price;
	}
	else if (info->direction == 's'  && info->open_price != 0)
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
			cout<<"the limit time max profit is "<<info->max_profit<<endl;
			return true;
		}
	}
	return false;
}


void GetOpenSignal(VolumeTrendLimitTimeInfo *info){
	info->has_open +=1;
	info->open_price = info->cur_price.LastPrice;
}
void GetCloseSignal(VolumeTrendLimitTimeInfo *info){
	info->max_profit = 0;
	info->open_price = 0;
}