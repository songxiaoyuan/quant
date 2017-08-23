#ifndef ARBITRAGEVOLUMETRENDOTHERFADE_H
#define ARBITRAGEVOLUMETRENDOTHERFADE_H
#include "hyArbitrage_Interface.h"
#include "func.h"
#include "band/BandAndTriggerSizeFadeFun.h"
using namespace std;

#define MaxPrice 100000

class CHyArbitrageVolumeTrendOtherFade:public CHyArbitrageBase
{
public:
	CHyArbitrageVolumeTrendOtherFade(Parameter* Param):CHyArbitrageBase(Param){}
	~CHyArbitrageVolumeTrendOtherFade(){};
public:
	virtual bool get_fv(SThreadChannel *threadChannel,double &fv);
	virtual void clearVector();
	virtual double calculateLessPrice(SThreadChannel *threadChannel,char OffsetFlag,char Direction,double fv,int perside);
	virtual void tradingOpenTraded(char *instrumentID);
	virtual void tradingCloseTraded(char *instrumentID);

private:
	void cancelNotMatchOrder(SThreadChannel *threadChannel,STraderChannel* pTraderInfo);

private:
	VolumeTrendOtherFadeInfo m_volumeTrendOtherFadeInfo[MAX_PARAM_LENGTH];
};


#endif