#ifndef ARBITRAGEVOLUMETRENDOTHER3_H
#define ARBITRAGEVOLUMETRENDOTHER3_H
#include "hyArbitrage_Interface.h"
#include "func.h"
#include "band/BandAndTriggerSizeFun.h"
using namespace std;

#define MaxPrice 100000

class CHyArbitrageVolumeTrendOther3:public CHyArbitrageBase
{
public:
	CHyArbitrageVolumeTrendOther3(Parameter* Param):CHyArbitrageBase(Param){}
	~CHyArbitrageVolumeTrendOther3(){};
public:
	virtual bool get_fv(SThreadChannel *threadChannel,double &fv);
	virtual void clearVector();
	virtual double calculateLessPrice(SThreadChannel *threadChannel,char OffsetFlag,char Direction,double fv,int perside);
	virtual void tradingOpenTraded(char *instrumentID);
	virtual void tradingCloseTraded(char *instrumentID);

private:
	void cancelNotMatchOrder(SThreadChannel *threadChannel,STraderChannel* pTraderInfo);

private:
	VolumeTrendOther3Info m_volumeTrendOther3Info[MAX_PARAM_LENGTH];
};


#endif