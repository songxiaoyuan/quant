#pragma once
#include <iostream>  
#include <fstream>  
#include <sstream>  
#include <string>  
#include <vector>  
   
using namespace std;

//ɾ���ַ����пո��Ʊ��tab����Ч�ַ�  
string Trim(string& str);

vector<vector<string>> GetCSVFileData(string path);
