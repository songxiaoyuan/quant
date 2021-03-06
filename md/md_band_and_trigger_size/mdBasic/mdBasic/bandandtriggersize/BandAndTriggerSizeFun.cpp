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
	if (rsi_vector.size() ==0)
	{
		return 0;
	}
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

