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



void InitQQ(vector<string> &QQnum)//QQ号读入内存
{
	ifstream rqfile;//读QQ号码
	string tmp_qq;
	//rqfile.open("C:\\Users\\doublezhuang\\Desktop\\MyQQ\\qqnum.txt", ios::in);//本地
	rqfile.open("C:\\Users\\Administrator\\Desktop\\MyQQ\\qqnum.txt", ios::in);//服务端
	while (!rqfile.eof())
	{
		rqfile >> tmp_qq;
		QQnum.push_back(tmp_qq);
	}
	rqfile.close();
}

void InitIDcard(vector<string>& IDname)//群名片读入内存
{
	ifstream rnfile;
	string tmp_id;
	//rnfile.open("C:\\Users\\doublezhuang\\Desktop\\MyQQ\\name.txt", ios::in);
	rnfile.open("C:\\Users\\Administrator\\Desktop\\MyQQ\\name.txt", ios::in);
	while (!rnfile.eof())
	{
		rnfile >> tmp_id;
		IDname.push_back(tmp_id);
	}
	rnfile.close();
}

void Save_data(vector<string> &QQnum,vector<string> &IDname)//存入本地文件
{
	ofstream wqfile;//写入QQ文件
	ofstream wnfile;//写入ID文件
	//wqfile.open("C:\\Users\\doublezhuang\\Desktop\\MyQQ\\QQ.txt", ios::out);
	//wnfile.open("C:\\Users\\doublezhuang\\Desktop\\MyQQ\\ID.txt", ios::out);
	wqfile.open("C:\\Users\\Administrator\\Desktop\\MyQQ\\QQ.txt", ios::out);
	wnfile.open("C:\\Users\\Administrator\\Desktop\\MyQQ\\ID.txt", ios::out);
	for (vector<string>::iterator it = QQnum.begin(); it != QQnum.end(); it++)
	{
		wqfile << *it << endl;
	}
	for (vector<string>::iterator it = IDname.begin(); it != IDname.end(); it++)
	{
		wnfile << *it << endl;
	}
	wqfile.close();
	wnfile.close();
}

void Save_data(vector<string>& IDname)//存入本地文件
{
	ofstream wnfile;//写入ID文件
	//wnfile.open("C:\\Users\\doublezhuang\\Desktop\\MyQQ\\ID.txt", ios::out);
	wnfile.open("C:\\Users\\Administrator\\Desktop\\MyQQ\\name.txt", ios::out);
	for (vector<string>::iterator it = IDname.begin(); it != IDname.end(); it++)
	{
		wnfile << *it << endl;
	}
	MQ::Api::FrameAPI::OutPut("保存成功");
	wnfile.close();
}

int seed()
{
	return time(0);
}

int random(int range,int seed)
{
	srand(seed);
	Sleep(800);
	return rand() % range;
}

bool _find(vector<string> repeat_name,string tmp)
{
	vector<string>::iterator judge;
	judge = find(repeat_name.begin(), repeat_name.end(), tmp);
	if (judge != repeat_name.end())
		return true;
	else return false;
}

void Show_ID(vector<string> IDname,string &idstring)
{
	for (vector<string>::iterator it = IDname.begin(); it!=IDname.end(); it++)
	{
		idstring += *it;
		idstring += "、";
	}
}

bool Delete_ID(vector<string> &IDname,string del_string)
{
	vector<string>::iterator iElementFound;
	iElementFound = find(IDname.begin(), IDname.end(), del_string);
	if (iElementFound!=IDname.end())
	{
		IDname.erase(iElementFound);
		return true;
	}
	else
	{
		return false;
	}
}

void Roll_name(const MQ::Event::NormalEvent& e,vector<string> &IDname,vector<string> QQnum)
{
	int rand=0;
	string idstring = "";
	ofstream wnfile;//写入ID文件
	ifstream rnfile;//读ID文件
	vector<string> repeat_name;//判重用容器
	string tmp = "";//每次判断用
	string repeat_id;//输出重复ID
	int id_flag = 0;
	if (e.msg=="#ROLL")
	{
		MQ::Api::GroupAPI::SetGroupCard(e.botQQ, e.sourceId, e.activeQQ, IDname[random(IDname.size(),seed())]);
		MQ::Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::群, e.sourceId, e.activeQQ, "修改完成[@" + e.activeQQ + "]");
	}
	if (e.msg=="#ROLLALL")
	{
		for (vector<string>::iterator it = QQnum.begin(); it != QQnum.end(); it++)
		{
			Api::GroupAPI::SetGroupCard(e.botQQ, e.sourceId, *it, IDname[random(IDname.size(), seed())]);
			tmp=MQ::Api::GroupAPI::GetGroupCard(e.botQQ, e.sourceId, *it);//获取此轮修改QQ的ID
			if (_find(repeat_name,tmp))
			{
				//MQ::Api::FrameAPI::OutPut("ID" + tmp + "重复" + to_string(count(repeat_name.begin(), repeat_name.end(), tmp)) + "次");
				MQ::Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::群, e.sourceId, e.activeQQ, "ID" + tmp + "重复" + to_string(count(repeat_name.begin(), repeat_name.end(), tmp)) + "次");
				repeat_id += tmp;
				repeat_id += ",";
				id_flag = 1;
			}
			repeat_name.push_back(tmp);
		}
		if (id_flag==1)
		{
			MQ::Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::群, e.sourceId, e.activeQQ, "此轮重复ID"+repeat_id);
			//MQ::Api::FrameAPI::OutPut("此轮重复ID" + repeat_id);
		}
		else
		{
			MQ::Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::群, e.sourceId, e.activeQQ, "此轮无重复ID");
			//MQ::Api::FrameAPI::OutPut("此轮重复ID" + repeat_id);
		}
	}
	if (e.msg.find("#ROLL[@") == 0)
	{
		MQ::Api::GroupAPI::SetGroupCard(e.botQQ, e.sourceId, e.msg.substr(e.msg.find("[") + 2, e.msg.find("]") - e.msg.find("[") - 2), IDname[random(IDname.size(), seed())]);
		MQ::Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::群, e.sourceId, e.activeQQ, "修改完成[@" + e.activeQQ + "][@"+ e.msg.substr(e.msg.find("[") + 2, e.msg.find("]") - e.msg.find("[") - 2)+"]");
	}
	if (e.msg.find("#添加")==0)
	{
		string wait_join = e.msg.substr(5);
		if (wait_join.find(","))
		{

		}
		IDname.push_back(e.msg.substr(5));
		MQ::Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::群, e.sourceId, e.activeQQ, e.msg.substr(5)+"添加成功");
		Save_data(IDname);
	}
	if (e.msg.find("#ID")==0)
	{
		Show_ID(IDname,idstring);
		Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::群, e.sourceId, e.activeQQ, idstring);
	}
	if (e.msg.find("#删除")==0)
	{
		if (Delete_ID(IDname, e.msg.substr(5)))
		{
			Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::群, e.sourceId, e.activeQQ, e.msg.substr(5)+"完成");
			Save_data(IDname);
		}
		else
		{
			Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::群, e.sourceId, e.activeQQ, "未找到ID");
		}
	}
}