#pragma once
#include <iostream>  
#include <fstream>  
#include <sstream>  
#include <string>  
#include <vector>  
   
using namespace std;

//删除字符串中空格，制表符tab等无效字符  
string Trim(string& str);

vector<vector<string>> GetCSVFileData(string path);
