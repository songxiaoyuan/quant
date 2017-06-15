#include <iostream>
#include "application\application.h"
using namespace std;

int main(){
  application* app = new application();
  app->ConfigFun();
  app->Init();
  app->Run();

  return 0;
}