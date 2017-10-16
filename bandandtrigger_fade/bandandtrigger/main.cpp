#include <iostream>
#include <Windows.h>
#include "band\BandAndTriggerSizeFade.h"
#include "basicfun\BasicFun.h"
using namespace std;

int main(){
  long t1 =GetTickCount();
  int i;
  BandAndTriggerSizeFade bt;
  string path = "rb1801_20170912.csv";
  vector<vector<string>> data = GetCSVFileData(path);
  for (int i = 0; i < data.size(); i++)
  {
	  bt.getPrices(data[i]);
  }
  cout<<"end!!"<<endl;
  long t2 = GetTickCount();
  cout<<t2-t1<<endl;
  cin>>i;
  cout<<"the exe is stop"<<endl;
  return 0;
}