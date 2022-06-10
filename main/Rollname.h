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
	rqfile.open("C:\\Users\\doublezhuang\\Desktop\\MyQQ\\qqnum.txt", ios::in);
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
	rnfile.open("C:\\Users\\doublezhuang\\Desktop\\MyQQ\\name.txt", ios::in);
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
	wqfile.open("C:\\Users\\doublezhuang\\Desktop\\MyQQ\\QQ.txt", ios::out);
	wnfile.open("C:\\Users\\doublezhuang\\Desktop\\MyQQ\\ID.txt", ios::out);
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

int random(int range)
{
	unsigned seed = time(0);
	srand(seed);
	return rand() % range;
}

void Roll_name(const MQ::Event::NormalEvent& e,vector<string> IDname,vector<string> QQnum)
{
	MQ::Api::FrameAPI::OutPut("start");
	ofstream wnfile;//写入ID文件
	ifstream rnfile;//读ID文件
	vector<string> repeat_name;
	string tmp = "";
	string repeat_id;//输出重复ID
	int id_flag = 0;
	if (e.msg=="#ROLL")
	{
		MQ::Api::GroupAPI::SetGroupCard(e.botQQ, e.sourceId, e.activeQQ, IDname[random(IDname.size())]);
		MQ::Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::群, e.sourceId, e.activeQQ, "修改完成[@" + e.activeQQ + "]");
	}
	if (e.msg=="#ROLLALL")
	{
		for (vector<string>::iterator it = QQnum.begin(); it != QQnum.end(); it++)
		{
			MQ::Api::GroupAPI::SetGroupCard(e.botQQ, e.sourceId, *it, IDname[random(IDname.size())]);
			tmp=MQ::Api::GroupAPI::GetGroupCard(e.botQQ, e.sourceId, *it);
			repeat_name.push_back(tmp);
			if (count(repeat_name.begin(),repeat_name.end(),tmp)!=1)
			{
				MQ::Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::群, e.sourceId, e.activeQQ, "ID" + tmp + "重复" + to_string(count(repeat_name.begin(), repeat_name.end(), tmp)) + "次");
				repeat_id += tmp;
				repeat_id += ",";
				id_flag = 1;
			}
		}
		if (id_flag==1)
		{
			MQ::Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::群, e.sourceId, e.activeQQ, "此轮重复ID"+repeat_id);
		}
		else
		{
			MQ::Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::群, e.sourceId, e.activeQQ, "此轮无重复ID");
		}
	}
	MQ::Api::FrameAPI::OutPut("finish");
	//vector<string>::iterator it;
	//it = find(name.begin(), name.end(), "测试5");
	//if (it != name.end())
	//	cout << "success" << *it << endl;
	//else
	//	cout << "fail" << endl;
	//cout << name.size() << endl;

}