#include "basicFun.h"

//�˺���������ȡ�����ļ���Ȼ������ļ���Ϣ������һ����ϣ���������б���
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

//�˺����ǽ�sChangeStrToChartring������ת��Ϊchar*
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

 //��ָ�����ַ���д�뵽txt�С�
 void strToFile(string error) {
  char *filePath = "error.txt";
  FILE *file_fd = fopen(filePath, "a");
  char *huiche = "\n";
  const char *data = error.data();
  int writeLen = fwrite(data, 1, strlen(data), file_fd);
  int writeLen1 = fwrite(huiche, 1, 1, file_fd);
  fclose(file_fd);
}