#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <MQCore/MQHeader.h>
#include <GlobalVar.h>
#include <cstdlib> //random
#include <ctime>
#include "common.h"
using namespace std;
using namespace MQ;

string Data_History;//群历史记录


string Time2Str()
{
	time_t tm;
	time(&tm); //获取time_t类型的当前时间
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&tm));
	return tmp;
}

void TodayInHistory(const MQ::Event::NormalEvent& e)
{
	string Show_history = "";//临时输出变量
	if (e.msg.find("#历史上的今天") == 0)
	{
		string time = Time2Str();
		Data_History = time + e.msg.substr(13);
		MQ::Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::群, e.sourceId, e.activeQQ, time +" :"+ e.msg.substr(13) + "；历史上的今天添加成功.");
		Sava_Date_Add(Data_History, "history.txt");
	}
	if (e.msg=="编年史")
	{
		vector<string> History_Data;//输出群历史记录
		ifstream rqfile;
		string tmp;
		rqfile.open("C:\\Users\\Administrator\\Desktop\\MyQQ\\Save_data\\history.txt", ios::in);
		while (!rqfile.eof())
		{
			rqfile >> tmp;
			History_Data.push_back(tmp);
		}
		rqfile.close();
		vector<string>::iterator it = History_Data.begin();
		do
		{
			Show_history += *it;
			Show_history += "\n"; 
			it++;
		} while (it != History_Data.end()-1);
		Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::群, e.sourceId, e.activeQQ, Show_history);
	}
}