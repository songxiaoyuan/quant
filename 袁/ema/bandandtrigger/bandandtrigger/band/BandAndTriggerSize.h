#pragma once
#include "BandAndTriggerSizeFun.h"
#include <vector>
#include <math.h>
#include <string>
#include <string.h>
#include <iostream>
#include <unordered_map>
#include <map>
#include <queue>
using namespace std;

class BandAndTriggerSize
{
public:
	BandAndTriggerSize(void);
	~BandAndTriggerSize(void);
	void getPrices(vector<string> &line_data);
	void clearVector();

private:
	VolumeTrendOther3Info TriggerSize3_info_;

	Param interface_param_;

	//this is the myself test the server is not needed
	int now_interest_;
	int limit_interest_;
};

