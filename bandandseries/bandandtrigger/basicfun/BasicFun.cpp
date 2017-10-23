#include "BasicFun.h"


//删除字符串中空格，制表符tab等无效字符  
string Trim(string& str)  
{  
    //str.find_first_not_of(" \t\r\n"),在字符串str中从索引0开始，返回首次不匹配"\t\r\n"的位置  
    str.erase(0,str.find_first_not_of(" \t\r\n"));  
    str.erase(str.find_last_not_of(" \t\r\n") + 1);  
    return str;  
} 

vector<vector<string>> GetCSVFileData(string path)  
{  
	vector<vector<string>> ret;
	ifstream fin(path.c_str()); //打开文件流操作  
    string line;   
    while (getline(fin, line))   //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取  
    {  
        //cout <<"原始字符串："<< line << endl; //整行输出  
        istringstream sin(line); //将整行字符串line读入到字符串流istringstream中  
        vector<string> fields; //声明一个字符串向量  
        string field;  
        while (getline(sin, field, ',')) //将字符串流sin中的字符读入到field字符串中，以逗号为分隔符  
        {  
            fields.push_back(Trim(field)); //将刚刚读取的字符串添加到向量fields中  
        }  
        string id =fields[0];
        //cout <<"处理之后的字符串："<< id << endl; 
		ret.push_back(fields);
    }  
	return ret;  
}
