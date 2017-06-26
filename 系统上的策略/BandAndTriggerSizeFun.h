#pragma once
#include <vector>
#include <unordered_map>
using namespace std;
//根据现在的price的一个列表，计算列表里面的ma数据。
double GetMAData(vector<double> &vector_prices,int period);
// 根据现在的price的一个列表，计算列表里面的标准差
double GetSDData(vector<double> &vector_prices,int period);
// 根据传入的这个lastprice，计算返回的ema的值。
double GetEMAData(double price,double pre_ema_val,int period);
//根据传入的当前的差值和保存差值的数据，以及rsi的周期，来求返回的rsi的值。
double GetRSIData(double tmpdiff,vector<double> &rsi_vector,int period);
void WriteMesgToFile(string path,string mesg);
unordered_map<string,string> GetConfigInfo(string path);;