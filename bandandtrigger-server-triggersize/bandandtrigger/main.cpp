#include <iostream>
#include <Windows.h>
#include "band\BandAndTriggerSize.h"
#include "basicfun\BasicFun.h"
using namespace std;

int main(){
  long t1 =GetTickCount();
  int i;
  BandAndTriggerSize bt;
<<<<<<< HEAD:bandandtrigger-server/bandandtrigger/main.cpp
  string path = "rb1801_20170815.csv";
=======
  string path = "ru1801_20170911.csv";
>>>>>>> 6ba9f0859c94c8c4b9061e48232cf17c4c136680:bandandtrigger-server-triggersize/bandandtrigger/main.cpp
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