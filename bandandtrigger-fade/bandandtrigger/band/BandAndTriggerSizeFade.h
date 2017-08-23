#pragma once
#include "BandAndTriggerSizeFadeFun.h"
#include <vector>
#include <math.h>
#include <string>
#include <string.h>
#include <iostream>
#include <unordered_map>
#include <map>
#include <queue>
using namespace std;

class BandAndTriggerSizeFade
{
public:
	BandAndTriggerSizeFade(void);
	~BandAndTriggerSizeFade(void);
	void getPrices(vector<string> &line_data);
	void clearVector();

private:
	VolumeTrendOtherFadeInfo TriggerSizeFade_info_;

	Parameter_Fade interface_param_;

	//this is the myself test the server is not needed
	int now_interest_;
	int limit_interest_;
};

