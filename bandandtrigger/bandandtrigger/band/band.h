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
	// �ж��ǲ��Ǵﵽ�˲��ִ��Ŀ��ֺ�ƽ������
	bool IsBandCloseTime();
	bool IsBandOpenTime();
	//�������ڵ�price��һ���б������б������ma���ݡ�
	double GetMAData(vector<double> &prices);
	// �������ڵ�price��һ���б������б�����ı�׼��
	double GetSDData(vector<double> &prices);
	// ���ݴ�������lastprice�����㷵�ص�ema��ֵ��
	double GetEMAData(double price);
private:
	vector<double> vector_prices_;
	double cur_lastprice_;
	double last_mea_val_;
	int current_ema_tick_num_;
	double band_open_edge_;
	double band_close_edge_;
	double cur_middle_value_;
	double cur_sd_val_;
	int compXaveNum_;
	string direction_;
	string movingTheo_;
};

