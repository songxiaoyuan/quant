#include "BasicFun.h"


//ɾ���ַ����пո��Ʊ��tab����Ч�ַ�  
string Trim(string& str)  
{  
    //str.find_first_not_of(" \t\r\n"),���ַ���str�д�����0��ʼ�������״β�ƥ��"\t\r\n"��λ��  
    str.erase(0,str.find_first_not_of(" \t\r\n"));  
    str.erase(str.find_last_not_of(" \t\r\n") + 1);  
    return str;  
} 

vector<vector<string>> GetCSVFileData(string path)  
{  
	vector<vector<string>> ret;
	ifstream fin(path.c_str()); //���ļ�������  
    string line;   
    while (getline(fin, line))   //���ж�ȡ�����з���\n�����֣������ļ�β��־eof��ֹ��ȡ  
    {  
        //cout <<"ԭʼ�ַ�����"<< line << endl; //�������  
        istringstream sin(line); //�������ַ���line���뵽�ַ�����istringstream��  
        vector<string> fields; //����һ���ַ�������  
        string field;  
        while (getline(sin, field, ',')) //���ַ�����sin�е��ַ����뵽field�ַ����У��Զ���Ϊ�ָ���  
        {  
            fields.push_back(Trim(field)); //���ոն�ȡ���ַ�����ӵ�����fields��  
        }  
        string id =fields[0];
        //cout <<"����֮����ַ�����"<< id << endl; 
		ret.push_back(fields);
    }  
	return ret;  
}
