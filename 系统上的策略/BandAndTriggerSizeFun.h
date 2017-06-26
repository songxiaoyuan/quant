#pragma once
#include <vector>
#include <unordered_map>
using namespace std;
//�������ڵ�price��һ���б������б������ma���ݡ�
double GetMAData(vector<double> &vector_prices,int period);
// �������ڵ�price��һ���б������б�����ı�׼��
double GetSDData(vector<double> &vector_prices,int period);
// ���ݴ�������lastprice�����㷵�ص�ema��ֵ��
double GetEMAData(double price,double pre_ema_val,int period);
//���ݴ���ĵ�ǰ�Ĳ�ֵ�ͱ����ֵ�����ݣ��Լ�rsi�����ڣ����󷵻ص�rsi��ֵ��
double GetRSIData(double tmpdiff,vector<double> &rsi_vector,int period);
void WriteMesgToFile(string path,string mesg);
unordered_map<string,string> GetConfigInfo(string path);;