#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

unordered_map<string,string> GetConfigInfo(string path);
vector<string> split(string target,string pattern);
char * ChangeStrToChar(string str);
void strToFile(string error);

