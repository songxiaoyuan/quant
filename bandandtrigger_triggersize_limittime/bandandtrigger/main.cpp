#include <iostream>
#include <Windows.h>
#include "band\BandAndTriggerSize.h"
#include "basicfun\BasicFun.h"
using namespace std;

int main(){
  long t1 =GetTickCount();
  int i;
  BandAndTriggerSize bt;
  string path = "ru1801_20170911.csv";
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