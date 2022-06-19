#pragma once
#include <vector>
#include <string>
#include <fstream>
using namespace std;
string wait_ban = "";
int ban_flag = 0;

void Sava_Date(vector<string>& Wait_Save_Data,string filename) 
{
	ofstream file_name;
	string file_url = "C:\\Users\\Administrator\\Desktop\\MyQQ\\Save_data\\"+filename;
	file_name.open(file_url, ios::out);
	for (vector<string>::iterator it = Wait_Save_Data.begin(); it != Wait_Save_Data.end(); it++)
	{
		file_name << *it << endl;
	}
	file_name.close();
}

void Common_Init(vector<string>& initstring,string filename)
{
	ifstream init_file;
	string tmp;
	init_file.open("C:\\Users\\Administrator\\Desktop\\MyQQ\\init"+filename, ios::in);
	while (!init_file.eof())
	{
		init_file >> tmp;
		initstring.push_back(tmp);
	}
	init_file.close();
}

void Common_ban(const MQ::Event::NormalEvent& e)
{
	if (e.msg=="[@739287296] "|| e.msg == "[@739287296]")
	{
		ban_flag++;
		wait_ban = e.activeQQ;
	}
	if (ban_flag >=2 && wait_ban==e.activeQQ)
	{
		Api::GroupAPI::Shutup(e.botQQ, e.sourceId, wait_ban,"1");
		ban_flag = 0;
	}
}

void sava_ban()
{

}

void Common_at(const MQ::Event::NormalEvent& e)
{
	if (e.msg.find("#[@")==0 )
	{
		if ( e.activeQQ == "739287296")
		{
			if (atoi(e.msg.substr(e.msg.find("]") + 2).c_str())<=10)
			{
				for (int i = 0; i < atoi(e.msg.substr(e.msg.find("]") + 2).c_str()); i++)
				{
					Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::群, e.sourceId, e.activeQQ, "[@" + e.msg.substr(e.msg.find("[") + 2, e.msg.find("]") - e.msg.find("[") - 2) + "]");
				}
			}
			else
			{
				Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::群, e.sourceId, e.activeQQ, "禁止炸群");
			}
		}
		if(e.msg.substr(e.msg.find("[") + 2, e.msg.find("]") - e.msg.find("[") - 2)=="739287296")
		{
			Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::群, e.sourceId, e.activeQQ, "在路上了");
		}
	}
}