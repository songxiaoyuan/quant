#pragma once
#include <vector>
#include <math.h>
#include <string>
#include <string.h>
#include <iostream>
using namespace std;
class band
{
public:
	band(void);
	~band(void);
	void getPrices(double price);
private:
	double getMAData(vector<double> prices);
	double getSDData(vector<double> prices);
	double getEMAData(double price);
	bool isLongOpenTime(double price,double middleval,double upval);
	bool isLongCloseTime(double price,double profitval,double lossval);
	bool isShortOpenTime(double price,double middleval,double downval);
	bool isShortCloseTime(double price,double profitval,double lossval);
private:
	vector<double> prices;
	double lastPrice;
	int emaTickNum;
	int compXaveNum;
	double bandOpenEdge;
	double bandCloseEdge;
	string direction;
	string movingTheo;
};

