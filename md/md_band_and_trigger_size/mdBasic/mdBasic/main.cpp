#include <iostream>
#include "application\application.h"
using namespace std;

int main(){
	cout<<"hello world"<<endl;
  application* app = new application();
  app->ConfigFun();
  app->Init();
  app->Run();

  return 0;
}