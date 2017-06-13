#include "band.h"


band::band(void)
{
	compXaveNum =2;
	movingTheo = "EMA";
	bandOpenEdge = 0.5;
	bandCloseEdge = 3;
	direction = "SHORT";
}


band::~band(void)
{
}

double band::getMAData(vector<double> prices){
	double sum =0;
	if (prices.size()==0)
	{
		return 0;
	}
	for (int i = 0; i < prices.size(); i++)
	{
		sum +=prices[i];
	}
	return sum/prices.size();
}

double band::getSDData(vector<double> prices){
	int size = prices.size();
	if (size ==0)
	{
		return 0;
	}
	double sum = 0;
	for (int i = 0; i < prices.size(); i++)
	{
		sum +=prices[i];
	}
	double avg = sum/size;
	sum = 0;
	for (int i = 0; i < size; i++)
	{
		sum += (prices[i]-avg)*(prices[i]-avg);
	}
	return sqrt(sum);
}

double band::getEMAData(double price){
	if (prices.size()==1)
	{
		emaTickNum =1;
		lastPrice = price;
		return lastPrice;
	}
	if (emaTickNum < compXaveNum)
	{
		emaTickNum +=1;
	}
	double ret = ((emaTickNum-1)*lastPrice + 2*price)/(emaTickNum+1);
	lastPrice = ret;
	return ret;
}


bool band::isLongOpenTime(double price,double middleval,double upval){
	if (price < upval && price > middleval)
	{
		return true;
	}
	return false;
}

bool band::isLongCloseTime(double price,double profitval,double lossval){
	if (price < lossval || price > profitval)
	{
		return true;
	}
	return false;
}

bool band::isShortOpenTime(double price,double middleval,double downval){
	if (price>downval && price < middleval)
	{
		return true;
	}
	return false;
}

bool band::isShortCloseTime(double price,double profitval,double lossval){
	if (price > lossval || price < profitval )
	{
		return true;
	}
	return false;
}

void band::getPrices(double price){
	if (prices.size() < compXaveNum)
	{
		prices.push_back(price);
		double tmp = getEMAData(price);
	}
	else{
		prices.push_back(price);
		vector<double>::iterator it = prices.begin();
		it = prices.erase(it);
	}
	double tmpma;
	if (movingTheo == "MA"){
		tmpma = getMAData(prices);
	}
	else
	{
		tmpma = getEMAData(price);
	}
	double tmpsd = getSDData(prices);
	if (direction =="LONG")
	{
		bool isLoneOpen = isLongOpenTime(price,tmpma,tmpma + tmpsd*bandOpenEdge);
		bool isLongClose = isLongCloseTime(price,tmpma+tmpsd*bandCloseEdge,tmpma - tmpsd*bandOpenEdge);
		if (isLoneOpen)
		{
			cout<<"this is long open "<<price<<endl;
			cout<<"the tmpsd is  "<<tmpma<<endl;
			cout<<"the tmpsd up is  "<<tmpma+tmpsd*bandOpenEdge<<endl;
		}
		else if(isLongClose){
		  cout<<"this is long close " <<price<<endl;
		  	cout<<"the tmpma is "<<tmpma<<endl;
			cout<<"the tmpsd  profit is  "<<tmpma+tmpsd*bandCloseEdge<<endl;
			cout<<"the tmpsd  loss is  "<<tmpma - tmpsd*bandOpenEdge<<endl;

		}
	}
	else if (direction =="SHORT")
	{
		bool isShortOpen =isShortOpenTime(price,tmpma,tmpma-tmpsd*bandOpenEdge);
		bool isShortClose = isShortCloseTime(price,tmpma-tmpsd*bandCloseEdge,tmpma + tmpsd*bandOpenEdge);
		if (isShortOpen)
		{
			cout<<"this is short open "<<price<<endl;
			cout<<"the tmpsd is  "<<tmpma<<endl;
			cout<<"the tmpsd  down is  "<<tmpma - tmpsd*bandOpenEdge<<endl;
		}
		else if (isShortClose){
			cout<<"this is short close "<<price<<endl;
			cout<<"the tmpsd  profit is  "<<tmpma-tmpsd*bandCloseEdge<<endl;
			cout<<"the tmpsd  loss is  "<<tmpma + tmpsd*bandOpenEdge<<endl;
		}
	}
}
