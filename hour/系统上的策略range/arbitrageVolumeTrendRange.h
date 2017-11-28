#ifndef ARBITRAGEVOLUMETRENDSERIES_H
#define ARBITRAGEVOLUMETRENDSERIES_H
#include "hyArbitrage_Interface.h"
#include "func.h"
#include "band/BandAndRangeFun.h"
using namespace std;

#define MaxPrice 100000

class CHyArbitrageVolumeTrendRange:public CHyArbitrageBase
{
public:
	CHyArbitrageVolumeTrendRange(Parameter* Param):CHyArbitrageBase(Param){}
	~CHyArbitrageVolumeTrendRange(){};
public:
	virtual bool get_fv(SThreadChannel *threadChannel,double &fv);
	virtual void clearVector();
	virtual double calculateLessPrice(SThreadChannel *threadChannel,char OffsetFlag,char Direction,double fv,int perside);
	virtual void tradingOpenTraded(char *instrumentID);
	virtual void tradingCloseTraded(char *instrumentID);

private:
	void cancelNotMatchOrder(SThreadChannel *threadChannel,STraderChannel* pTraderInfo);

private:
	VolumeTrendRangeInfo m_VolumeTrendRangeInfo[MAX_PARAM_LENGTH];
};


#endif