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

double GetEMAData(double price,double pre_ema_val,int period){
	if (period <=1)
	{
		return price;
	}
	double ret = ((period - 1)*pre_ema_val + 2*price)/(period + 1);
	return ret;
}

double GetRSIData(double tmpdiff,vector<double> &rsi_vector,int period){
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

void WriteMesgToFile12(string path,string mesg){
  FILE *file_fd = fopen((char*)path.c_str(),"a");
  char tmp[1024] = {0};
  sprintf(tmp,"%s\n",mesg.c_str());
  int write_len = fwrite(tmp,1,strlen(tmp),file_fd);
  fclose(file_fd);
}

bool IsBandOpenTime(char direction,double lastprice,double middle,double sd,double openval){
	if (direction =='l')
	{
		double upval = middle + sd * openval;
		if (lastprice > middle && lastprice < upval)
		{
			return true;
		}
	}
	else if (direction	==	's')
	{
		double downval = middle - sd * openval;
		if (lastprice > downval && lastprice < middle)
		{
			return true;
		}
	}
	return false;
}

bool IsBandCloseTime(char direction,double lastprice,double middle,double sd,double loss_band,double profit_band,double rsival,double limit_rsi){

	if (direction	==	'l')
	{
		double profitval = middle + sd * profit_band;
		double lossval = middle - sd * loss_band;
		if ( loss_band !=0  && lastprice < lossval)
		{
			return true;
		}
		if (lastprice > profitval && rsival > limit_rsi )
		{
			return true;
		}
	}
	else if (direction	==	's')
	{
		double profitval = middle - sd * profitval;
		double lossval = middle + sd * loss_band;
		if (loss_band !=0 && lastprice > lossval)
		{
			return true;
		}
		double tmp_rsi = 100 - rsival;
		if (lastprice < profitval && tmp_rsi > limit_rsi)
		{
			return true;
		}
	}
	return false;
}

bool IsTriggerSizeOpenTime(char direction,BandAndTriggerSizePriceInfo *now_price,BandAndTriggerSizePriceInfo *pre_price
				,double volume_edge,double openinterest_edge,int spread ){
	if (now_price->Volume - pre_price->Volume < volume_edge )
	 {
		 cout<<now_price->Volume<<endl;
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

	double temp	=	100*(pre_price->AskPrice1	-	avePrice)/(pre_price->AskPrice1-pre_price->BidPrice1);
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

	double temp	=	100*(avePrice	-	pre_price->BidPrice1)/(pre_price->AskPrice1-pre_price->BidPrice1);

	if (temp >=	spread)
	{
		return true;

	}
	return false;

}

