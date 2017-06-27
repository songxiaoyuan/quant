#include "basicFun.h"

vector<string> split(string target,string pattern){
	vector<string> ret;
	if(pattern.empty()) return ret;
	size_t start=0,index=target.find_first_of(pattern,0);
	while(index!=target.npos)
	{
		if(start!=index)
		ret.push_back(target.substr(start,index-start));
		start=index+1;
		index=target.find_first_of(pattern,start);
	}
	if(!target.substr(start).empty())
	{
		ret.push_back(target.substr(start));
	}
	return ret;
}

unordered_map<string,string> GetConfigInfoRetMap(string path){
	unordered_map<string,string> ret;
	FILE *file_fd = fopen((char*)path.c_str(),"r");
	if (NULL ==file_fd)
	{
		cout<<"cant find the config file"<<endl;
		return ret;
	}
	char tmp[1024] = {0};
	while(!feof(file_fd)){
		memset(tmp, 0, sizeof(tmp));
		fgets(tmp, sizeof(tmp) - 1, file_fd);
		vector<string> tmp_vector = split((string)tmp," ");
		ret[tmp_vector[0]] = tmp_vector[1].substr(0,tmp_vector[1].length()-1);
	}
	fclose(file_fd);
	/*
	 for(unordered_map<string,string>::iterator iter = ret.begin(); iter != ret.end(); iter++)    
    {    
		cout<<iter->first<<":"<<iter->second<<endl;    
    } */
	return ret;
}

//此函数是将sChangeStrToChartring的类型转化为char*
 char * ChangeStrToChar (string str)
 {
	char* c;
	int len = str.length();
	c =new char[len-1];
	if(str[len-1]=='\r')
	{
		strcpy(c,str.substr(0,len-1).c_str());
	}
	else
	{
		strcpy(c,str.c_str());
	}
	return c;

 }

 //把指定的字符串写入到txt中。
 void strToFile(string error) {
  char *filePath = "error.txt";
  FILE *file_fd = fopen(filePath, "a");
  char *huiche = "\n";
  const char *data = error.data();
  int writeLen = fwrite(data, 1, strlen(data), file_fd);
  int writeLen1 = fwrite(huiche, 1, 1, file_fd);
  fclose(file_fd);
}

