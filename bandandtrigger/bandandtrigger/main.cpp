#include <iostream>
#include "band\band.h"
using namespace std;

int main(){
  cout<<"hello world"<<endl;
  double prices[7] = {100,102,102.3,99.9,100,102.9,99};
  band bandtrigger;
  for (int i = 0; i < 7; i++)
  {
	  bandtrigger.getPrices(prices[i]);
  }
  int a =1;
  if (a ==1)
  {
	  cout<<"the a is -1"<<endl;
  }
  int i;
  cin>>i;
  return 0;
}