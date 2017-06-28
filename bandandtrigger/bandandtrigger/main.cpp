#include <iostream>
#include "band\BandAndTriggerSize.h"
#include "basicfun\BasicFun.h"
using namespace std;

int main(){
  int i;
  BandAndTriggerSize bt;
  string path = "rb1710_20170627.csv";
  vector<vector<string>> data = GetCSVFileData(path);
  for (int i = 0; i < data.size(); i++)
  {
	  bt.getPrices(data[i]);
  }
  cout<<"end!!"<<endl;
  cin>>i;
  return 0;
}