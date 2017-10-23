#ifndef ARBITRAGEVOLUMETRENDOTHER3_H
#define ARBITRAGEVOLUMETRENDOTHER3_H
#include "hyArbitrage_Interface.h"
#include "func.h"
#include "band/BandAndSeriesFun.h"
using namespace std;

#define MaxPrice 100000

class CHyArbitrageVolumeTrendLimitTime:public CHyArbitrageBase
{
public:
	CHyArbitrageVolumeTrendLimitTime(Parameter* Param):CHyArbitrageBase(Param){}
	~CHyArbitrageVolumeTrendLimitTime(){};
public:
	virtual bool get_fv(SThreadChannel *threadChannel,double &fv);
	virtual void clearVector();
	virtual double calculateLessPrice(SThreadChannel *threadChannel,char OffsetFlag,char Direction,double fv,int perside);
	virtual void tradingOpenTraded(char *instrumentID);
	virtual void tradingCloseTraded(char *instrumentID);

private:
	void cancelNotMatchOrder(SThreadChannel *threadChannel,STraderChannel* pTraderInfo);

private:
	VolumeTrendSeriesInfo m_VolumeTrendSeriesInfo[MAX_PARAM_LENGTH];
};


#endif