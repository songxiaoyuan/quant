#include <iostream>
#include <Windows.h>
#include "band\BandAndTriggerSize.h"
#include "basicfun\BasicFun.h"
using namespace std;

int main(){
	long t1 =GetTickCount();
  int i;
  BandAndTriggerSize bt;
  string path = "rb1710_20170727.csv";
  vector<vector<string>> data = GetCSVFileData(path);
  for (int i = 0; i < data.size(); i++)
  {
	  bt.getPrices(data[i]);
  }
  cout<<"end!!"<<endl;
  long t2 = GetTickCount();
  cout<<t2-t1<<endl;
  cin>>i;
  return 0;
}