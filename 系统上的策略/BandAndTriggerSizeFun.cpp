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

bool IsBandOpenTime(char direction,double lastprice,double middle,double sd,double open_edge,double limit_sd,double limit_sd_open_edge){
	if (sd < limit_sd)
	{
		open_edge = limit_sd_open_edge;
	}
	if (direction =='l')
	{
		double upval = middle + sd * open_edge;
		if (lastprice > middle && lastprice < upval)
		{
			return true;
		}
	}
	else if (direction	==	's')
	{
		double downval = middle - sd * open_edge;
		if (lastprice > downval && lastprice < middle)
		{
			return true;
		}
	}
	return false;
}

bool IsBandCloseTime(char direction,double lastprice,double middle,double sd
					 ,double loss_band,double profit_band,double rsival,double limit_rsi
					 ,double limit_sd,double limit_sd_loss_band)
{
	 if (sd < limit_sd)
	{
		loss_band = limit_sd_loss_band;
	}
	if (direction	==	'l')
	{
		double profitval = middle + sd * profit_band;
		double lossval = middle - sd * loss_band;
		if ( loss_band !=0  && lastprice < lossval)
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
		if (loss_band !=0 && lastprice > lossval)
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

bool IsTriggerSizeOpenTime(char direction,BandAndTriggerSizePriceInfo *now_price,BandAndTriggerSizePriceInfo *pre_price
				,double volume_edge,double openinterest_edge,int spread ){
	if (now_price->Volume - pre_price->Volume < volume_edge )
	 {
		 //cout<<now_price->Volume<<endl;
		return false;
	}
	if (now_price->OpenInterest - pre_price->OpenInterest <= openinterest_edge)
	{
		return false;
	}
	if (direction =='l')
	{
		return IsUpTime(now_price,pre_price,spread);
		
	}
	else if (direction =='s')
	{
		return IsDownTime(now_price,pre_price,spread);
	}
	return false;
}

bool IsTriggerSizeCloseTime(char direction,BandAndTriggerSizePriceInfo *now_price,BandAndTriggerSizePriceInfo *pre_price
				,double volume_edge,double openinterest_edge,int spread){
	if (now_price->Volume - pre_price->Volume < volume_edge )
	 {
		return false;
	}
	if (now_price->OpenInterest - pre_price->OpenInterest >= - openinterest_edge)
	{
		return false;
	}
	bool ret = false;
	if (direction =='l')
	{
		return IsDownTime(now_price,pre_price,spread);
	}
	else if (direction =='s')
	{
		return IsUpTime(now_price,pre_price,spread);
	}
	return false;
}

bool IsDownTime(BandAndTriggerSizePriceInfo *now_price,BandAndTriggerSizePriceInfo *pre_price,int spread)
{
	int multiple=now_price->multiple;    
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

bool IsUpTime(BandAndTriggerSizePriceInfo *now_price,BandAndTriggerSizePriceInfo *pre_price,int spread)
{
	int multiple=now_price->multiple;   //返回合约乘数
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


void WriteMesgToFile(string path,string mesg){
  return;
  FILE *file_fd = fopen((char*)path.c_str(),"a");
  char tmp[1024] = {0};
  sprintf(tmp,"%s\n",mesg.c_str());
  int write_len = fwrite(tmp,1,strlen(tmp),file_fd);
  fclose(file_fd);
}

void WriteMesgToFileSO(string path,string mesg){
  FILE *file_fd = fopen((char*)path.c_str(),"a");
  char tmp[1024] = {0};
  sprintf(tmp,"%s\n",mesg.c_str());
  int write_len = fwrite(tmp,1,strlen(tmp),file_fd);
  fclose(file_fd);
}

void printInfo(double &pre_ema_val,queue<double> &lastprice_queue,map<double,int> &lastprice_map,
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
					 double rsi_period,double pre_rsi,int config_file_path){
	return;
	if (rsi_period >=100 || rsi_period <=0 || rsi_vector.size() < rsi_period) 
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
  while(!lastprice_queue.empty()){
	  queue_str = queue_str+"," + double2str(lastprice_queue.front());
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
  sprintf(rsi_tmp,"pre_rsi_val:,%.2f",pre_rsi);
  write_len = fwrite(rsi_tmp,1,strlen(rsi_tmp),file_fd);
  fclose(file_fd);
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
