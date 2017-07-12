#include "basicFun.h"

//此函数用来读取配置文件，然后根据文件信息，读出一个哈希表，用来进行保存
unordered_map<string,string> GetConfigInfo(string path){
	ifstream file(path);
	unordered_map<string,string> ret;
	if(file){
		string line;
		while(getline(file,line)){
			vector<string> config = split(line," ");
			if(config.size() ==(unsigned int)2){
			  ret[config[0]] = config[1];
			}
		}
		return ret;
	}
	else{
		cout << "cant find the config.txt, please checkup"<<endl;
		cout<<path<<endl;
	    exit(1);
	}
}

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