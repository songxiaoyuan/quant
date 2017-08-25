#include "BandAndTriggerSizeFun.h"

double GetMAData(vector<double> &vector_prices,int period){
	double sum =0;
	if (vector_prices.size()==0 || period ==0)
	{
		return 0;
	}
	for (int i = vector_prices.size()-1; i >=0 && i >= vector_prices.size() - period; i--)
	{
		sum +=vector_prices[i];
	}
	return sum/period;
}

int GetSumDataInt(vector<int> &diff_volume,int period){
	int sum=0;
    if (diff_volume.size()==0 || period ==0)
	{
		return 0;
	}
	for (int i = diff_volume.size()-1; i >=0 && i >= diff_volume.size() - period; i--)
	{
		sum +=diff_volume[i];
	}
	return sum;
}

double GetSumDataDouble(vector<double> &diff_openinterest,int period){
	double sum=0;
    if (diff_openinterest.size()==0 || period ==0)
	{
		return 0;
	}
	for (int i = diff_openinterest.size()-1; i >=0 && i >= diff_openinterest.size() - period; i--)
	{
		sum +=diff_openinterest[i];
	}
	return sum;
}

double GetWeightNum(vector<int> &diff_volume,vector<double> &spread,int period){
	double ret = 0;
	int sum = 0;
	if (diff_volume.size() != spread.size())
	{
		cout<<"the vector is not the same length"<<endl;
	}
	for (int i = diff_volume.size()-1; i >=0 && i >= diff_volume.size() - period; i--)
	{
		ret +=(diff_volume[i]*spread[i]);
		sum += diff_volume[i];
	}
	if (sum ==0)
	{
		return 0;
	}
	return ret/sum;
}

double GetSDData(vector<double> &vector_prices,int period){
	int size = vector_prices.size();
	if (size ==0 || period ==0)
	{
		return 0;
	}
	double sum = 0;
	for (int i = size-1; i >=0 && i >= size - period; i--)
	{
		sum +=vector_prices[i];
	}
	double avg = sum/period;
	sum = 0;
	for (int i = size-1; i >=0 && i >= size - period; i--)
	{
		sum += (vector_prices[i]-avg)*(vector_prices[i]-avg);
	}
	return sqrt(sum/period);
}

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
					,double open_edge1,double open_edge2 ,double limit_sd,double limit_sd_open_edge){
	if (sd < limit_sd)
	{
		open_edge2 = limit_sd_open_edge;
	}
	if (direction =='l')
	{
		double startval = middle + sd*open_edge1;
		double upval = middle + sd * open_edge2;
		if (lastprice > startval && lastprice < upval)
		{
			return true;
		}
	}
	else if (direction	==	's')
	{
		double startval = middle - sd*open_edge1;
		double downval = middle - sd * open_edge2;
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
	/*cout<<"the limit sd is"<<limit_sd<<endl;
	cout<<"the loss band is :"<<loss_band<<endl;
	cout<<"the profit_band band is :"<<profit_band<<endl;
	cout<<"the limit_sd_loss_band band is :"<<limit_sd_loss_band<<endl;*/
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
			if (limit_rsi == 0 || rsival >limit_rsi)
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
			if (limit_rsi == 0 || tmp_rsi > limit_rsi)
			{
				return true;
			}
		}
	}
	return false;
}

bool IsTriggerSizeOpenTime(char direction,vector<int> &diff_volume_vector,vector<double> &diff_openinterest_vector,vector<double> &spread_vector
				,double volume_edge,double openinterest_edge,double spread,int ticknum){

	int diff_volume_sum = GetSumDataInt(diff_volume_vector,ticknum);
	if (diff_volume_sum < volume_edge )
	 {
		 //cout<<now_price->Volume<<endl;
		return false;
	}
	double diff_openinterest_sum = GetSumDataDouble(diff_openinterest_vector,ticknum);
	if (openinterest_edge !=0 && diff_openinterest_sum <= openinterest_edge)
	{
		return false;
	}
	double spread_weight = GetWeightNum(diff_volume_vector,spread_vector,ticknum);
	if (direction =='l')
	{
		if (spread_weight >= spread)
		{
			return true;
		}
		
	}
	else if (direction =='s')
	{
		spread_weight = 100 - spread_weight;
		if (spread_weight >= spread)
		{
			return true;
		}
	}
	return false;
}

bool IsTriggerSizeCloseTime(char direction,mdPrice_triggersize3 *now_price,mdPrice_triggersize3 *pre_price,int multiple
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

bool IsDownTime(mdPrice_triggersize3 *now_price,mdPrice_triggersize3 *pre_price,int spread,int multiple){
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

bool IsUpTime(mdPrice_triggersize3 *now_price,mdPrice_triggersize3 *pre_price,int spread,int multiple)
{
	int diffVolume	=	now_price->Volume	-	pre_price->Volume;  //返回持仓量的变化
	double diffTurnover	=	now_price->Turnover	-	pre_price->Turnover;  //返回成交金额的变化

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

void PrintInfo(double &pre_ema_val,queue<double> &lastprice_queue,map<double,int> &lastprice_map,
			   vector<double> &rsi_vector,double &pre_rsi,int config_file_path){
    cout<<"the pre ema val is: "<<pre_ema_val<<endl;
	cout<<"the size of queue prices is : " <<lastprice_queue.size()<<endl;
	cout<<"the size of rsi_vector_ is : " <<rsi_vector.size()<<endl;
	cout<<"the size of map_prices_ is : " <<lastprice_map.size()<<endl;
	cout<<"the pre rsi last price is : "<<pre_rsi<<endl;
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

void GetConfigInfo(double &pre_ema_val,queue<double> &lastprice_queue,map<double,int> &lastprice_map,
				   vector<double> &rsi_vector,double &pre_rsi,int config_file_path){
	char path[256]={0};
	sprintf(path,"band_and_triggersize_config/%d",config_file_path);
	FILE *file_fd = fopen(path,"r");
	if (NULL ==file_fd)
	{
		cout<<"cant find the config file"<<endl;
		pre_ema_val = 0;
		//queue<double>().swap(lastprice_queue);
		lastprice_map.clear();
		rsi_vector.clear();
		pre_rsi = 0;
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
		if (front.compare("pre_ema_val:")==0)
		{
			cout<<"in the pre_ema_val"<<endl;
			//cout<<front<<endl;
			if (tmp_vector.size() >0)
			{
				pre_ema_val = tmp_vector[0];
			}
			else{
			  cout<<"the pre_ema_val is wrong"<<endl;
			}
		}
		else if (front.compare("lastpricearray:")==0)
		{
			cout<<"in the lastpricearrray"<<endl;
			//cout<<front<<endl;
			cout<<"the size of lastprice arrray is: "<<tmp_vector.size()<<endl;
			if (tmp_vector.size() >0)
			{
				for (int i = 0; i < tmp_vector.size(); i++)
				{
					lastprice_queue.push(tmp_vector[i]);
					map<double,int>::iterator iter = lastprice_map.find(tmp_vector[i]);
					if (iter == lastprice_map.end())
					{
						lastprice_map[tmp_vector[i]] = 1;
					}
					else{
						iter->second +=1;
					}
				}
			}
			else{
			  cout<<"the rsiarrray is wrong"<<endl;
			}
		}
		else if (front.compare("rsiarray:")==0)
		{
			cout<<"in the rsiarrray"<<endl;
			//cout<<front<<endl;
			cout<<"the size of rsi array is: "<<tmp_vector.size()<<endl;
			if (tmp_vector.size() >0)
			{
				for (int i = 0; i < tmp_vector.size(); i++)
				{
					rsi_vector.push_back(tmp_vector[i]);
				}
			}
			else{
			  cout<<"the rsiarrray is wrong"<<endl;
			}
		}
		else if (front.compare("pre_rsi_val:")==0)
		{
			cout<<"in the pre_rsi_val"<<endl;
			//cout<<front<<endl;
			if (tmp_vector.size() >0)
			{
				pre_rsi = tmp_vector[0];
			}
			else{
			  cout<<"the pre_rsi_val is wrong"<<endl;
			}
		}
		else
		{
			cout<<"the line is no needed"<<endl;
		}
	}
	fclose(file_fd);
}


void WriteConfigInfo(double &pre_ema_val,queue<double> &lastprice_queue,vector<double> &rsi_vector,
					 double rsi_period,int config_file_path){
	return;
	cout<<"this is end start to write the log"<<endl;
	cout<<"the pre ema val is: "<<pre_ema_val<<endl;
	cout<<"the size of queue prices is : " <<lastprice_queue.size()<<endl;
	cout<<"the size of rsi_vector_ is : " <<rsi_vector.size()<<endl;
	cout<<"the path is  : " <<config_file_path<<endl;
	cout<<"the rsi_period is  : " <<rsi_period<<endl;
	if (rsi_period >100 || rsi_period <0 || rsi_vector.size() < rsi_period) 
	{
		return;
	}
	char path[256]={0};
	sprintf(path,"band_and_triggersize_config/%d",config_file_path);
	FILE *file_fd = fopen(path,"w");
  char tmp[1024] = {0};
  sprintf(tmp,"pre_ema_val:,%.2f\n",pre_ema_val);
  int write_len = fwrite(tmp,1,strlen(tmp),file_fd);
  string queue_str = "lastpricearray:";
  double queue_last=0;
  while(!lastprice_queue.empty()){
	  queue_last = lastprice_queue.front();
	  queue_str = queue_str+"," + double2str(queue_last);
	  lastprice_queue.pop();
  }
  queue_str = queue_str+"\n";
  write_len = fwrite(queue_str.c_str(),1,strlen(queue_str.c_str()),file_fd);
  string rsi_str = "rsiarray:";
  for (int i = rsi_vector.size() - rsi_period; i < rsi_vector.size(); i++)
  {
	  rsi_str = rsi_str + "," + double2str(rsi_vector[i]);
  }
  rsi_str = rsi_str+"\n";
  write_len = fwrite(rsi_str.c_str(),1,strlen(rsi_str.c_str()),file_fd);
  char rsi_tmp[1024] = {0};
  sprintf(rsi_tmp,"pre_rsi_val:,%.2f",queue_last);
  write_len = fwrite(rsi_tmp,1,strlen(rsi_tmp),file_fd);
  fclose(file_fd);
  cout<<"we have write the status in :"<<config_file_path<<endl;
}

bool IsMaxDrawDown(char direction,double cur_lastprice,double open_price,int multiple,double &max_profit,double limit_max_drawdown){
	if (limit_max_drawdown ==0 || open_price ==0 )
	{
		return false;
	}
	double tmp_profit;
	if (direction =='l')
	{
		tmp_profit = cur_lastprice - open_price;
	}
	else if (direction =='s')
	{
		tmp_profit = open_price - cur_lastprice;
	}
	else{
		return false;
	}
	tmp_profit = tmp_profit * multiple;
	if (tmp_profit > max_profit){
		max_profit = tmp_profit;
	}
	if ((max_profit - tmp_profit) >= limit_max_drawdown)
	{
		return true;
	}
	else{
	  return false;
	}
}

void StartAndStopFun(Parameter_triggersize3 *param,VolumeTrendOther3Info *info,int param_index){
	int size = info->prices_queue.size();
	if (info->prices_queue.empty())
	{
		cout<<"the queue is empty and is init function"<<endl;
		GetConfigInfo(info->pre_ema_val,info->prices_queue,info->prices_map,
			info->rsi_vector,info->pre_rsi_lastprice,param->m_Param[param_index].arbitrageTypeID);
		PrintInfo(info->pre_ema_val,info->prices_queue,info->prices_map,
			info->rsi_vector,info->pre_rsi_lastprice,param->m_Param[param_index].arbitrageTypeID);
	}
	else{
		cout<<"the queue is not empty and is the stop function"<<endl;
		WriteConfigInfo(info->pre_ema_val,info->prices_queue,info->rsi_vector,
			param->m_Param[param_index+1].AdjEmaFast,param->m_Param[param_index].arbitrageTypeID);
	}
}

BandAndTriggerSizeRetStatus GetMdData(Parameter_triggersize3 *param,VolumeTrendOther3Info *info,int param_index){
	BandAndTriggerSizeRetStatus ret;
	ret.isTrendCloseTime = false;
	ret.isTrendOpenTime = false;
	double rsi_data;
	double middle_val;
	double sd_val;

	double lastprice = info->cur_price.LastPrice;
	int compXave = param->m_Param[param_index].compXave;
	int rsi_period = param->m_Param[param_index].AdjEmaFast;
	int rsi_bar_limit_tick = param->m_Param[param_index].PositionAdj;

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

	int diffVolume	=	info->cur_price.Volume - info->pre_price.Volume;  //返回持仓量的变化
	double diffTurnover	=	info->cur_price.Turnover - info->pre_price.Turnover;  //返回成交金额的变化
	double diffopeninterest	=	info->cur_price.OpenInterest - info->pre_price.OpenInterest;

	info->diff_volume_vector.push_back(info->cur_price.Volume - info->pre_price.Volume);
	info->diff_openinterest_vector.push_back(info->cur_price.OpenInterest - info->pre_price.OpenInterest);
		
	if (diffVolume	==	0	||	diffTurnover	==	0	||	info->multiple	==	0)
	{
		info->spread_vector.push_back(0);
	}
	else
	{
		double avePrice	=	diffTurnover/diffVolume/info->multiple;
		double temp	=	100*(avePrice	-	info->pre_price.BidPrice1)/(info->pre_price.AskPrice1 - info->pre_price.BidPrice1);
		info->spread_vector.push_back(temp);
	}

	if (info->prices_queue.size() < compXave )
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

	middle_val =  GetEMAData(lastprice,info->pre_ema_val,compXave);
	info->pre_ema_val = middle_val;
	sd_val = GetSDDataByMap(info->prices_map,compXave);

	ret.isTrendOpenTime = IsOpenTime(middle_val,sd_val,param,info,param_index);
	ret.isTrendCloseTime = IsCloseTime(middle_val,sd_val,rsi_data,param,info,param_index);

	return ret;
}

bool IsOpenTime(double middle_val,double sd_val,Parameter_triggersize3 *param,VolumeTrendOther3Info *info,int param_index){
	//the trigger size tick num
	int ticknum =param->m_Param[param_index+1].spread;
	//band_open_noraml
	double band_open_edge1 =  ((double)param->m_Param[param_index+1].openEdge)/10;
	double band_open_edge2 =  ((double)param->m_Param[param_index+1].closeEdge)/10;

	//limit_sd
	double limit_sd = ((double)param->m_Param[param_index+1].edgebWork)/10;
	//limit_sd_open_edge
	double limit_sd_open_edge = ((double)param->m_Param[param_index+1].orderDelay)/10;


	double spread = param->m_Param[param_index].spread;
	//diff_volume
	double diff_volume = param->m_Param[param_index].openEdge;
	//open interest
	double diff_openinterest = param->m_Param[param_index].index;
	//tick num
	

	double lastprice = info->cur_price.LastPrice;

	bool is_band_open = IsBandOpenTime(info->direction,lastprice,middle_val,sd_val,
		band_open_edge1,band_open_edge2,limit_sd,limit_sd_open_edge);
	if (is_band_open ==false)
	{
		return false;
	}

	bool is_trigger_open = IsTriggerSizeOpenTime(info->direction,info->diff_volume_vector,info->diff_openinterest_vector,info->spread_vector,
		diff_volume,diff_openinterest,spread,ticknum);

	return is_trigger_open;
}

bool IsCloseTime(double middle_val,double sd_val,double rsi_val,Parameter_triggersize3 *param,VolumeTrendOther3Info *info,int param_index){
	//band_loss_close_edge
	double band_loss_close_edge = ((double)param->m_Param[param_index+1].EdgeAdj)/10;
	//band_profit_close_edge
	double band_profit_close_edge =((double) param->m_Param[param_index+1].cancelEdge)/10;

	//limit_rsi
	double limit_rsi = param->m_Param[param_index].AdjEmaSlow;

	//limit_sd
	double limit_sd = ((double)param->m_Param[param_index+1].edgebWork)/10;
	//limit_sd_loss_close_edge
	double limit_sd_close_edge =  ((double)param->m_Param[param_index+1].edgePrice)/10;

	//cout<<"the limit_sd_close_edge is "<<limit_sd_close_edge<<endl;

	//limit_max_draw_down
	double limit_max_drawn = param->m_Param[param_index+1].maxDrawDown;

	double lastprice = info->cur_price.LastPrice;

	bool max_draw_down = IsMaxDrawDown(info->direction,lastprice,info->open_price,info->multiple,
		info->max_profit,limit_max_drawn);
	if (max_draw_down)
	{
		return true;
	}
	return IsBandCloseTime(info->direction,lastprice,middle_val,sd_val,
		band_loss_close_edge,band_profit_close_edge,rsi_val,limit_rsi,limit_sd,limit_sd_close_edge);
}


void GetOpenSignal(VolumeTrendOther3Info *info){
	info->open_price = info->cur_price.LastPrice;
	info->max_profit = 0;
}
void GetCloseSignal(VolumeTrendOther3Info *info){
	info->open_price = 0;
	info->max_profit = 0;
}