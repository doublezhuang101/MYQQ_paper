#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <MQCore/MQHeader.h>
#include <GlobalVar.h>
#include <cstdlib> //random
#include <ctime>
using namespace std;
using namespace MQ;

void Sava_History(vector<string>& History_Data)
{
	ifstream rnfile;
	string tmp_id;
	rnfile.open("C:\\Users\\Administrator\\Desktop\\MyQQ\\history.txt", ios::in);
	while (!rnfile.eof())
	{
		rnfile >> tmp_id;
		History_Data.push_back(tmp_id);
	}
	rnfile.close();
}

string Time2Str()
{
	time_t tm;
	time(&tm); //获取time_t类型的当前时间
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&tm));
	return tmp;
}

vector<string> History_Data;

void TodayInHistory(const MQ::Event::NormalEvent& e)
{

	if (e.msg.find("#历史上的今天") == 0)
	{
		string time = Time2Str();
		History_Data.push_back(time + e.msg.substr(13));
		MQ::Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::群, e.sourceId, e.activeQQ, time + e.msg.substr(13) + "；历史上的今天添加成功，望警钟长鸣。");
		Sava_History(History_Data);
	}
}