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

void InitQQ(vector<string> &QQnum)//QQ�Ŷ����ڴ�
{
	ifstream rqfile;//��QQ����
	string tmp_qq;
	rqfile.open("C:\\Users\\doublezhuang\\Desktop\\MyQQ\\qqnum.txt", ios::in);
	while (!rqfile.eof())
	{
		rqfile >> tmp_qq;
		QQnum.push_back(tmp_qq);
	}
	rqfile.close();
}

void InitIDcard(vector<string>& IDname)//Ⱥ��Ƭ�����ڴ�
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

void Save_data(vector<string> &QQnum,vector<string> &IDname)//���뱾���ļ�
{
	ofstream wqfile;//д��QQ�ļ�
	ofstream wnfile;//д��ID�ļ�
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
	ofstream wnfile;//д��ID�ļ�
	ifstream rnfile;//��ID�ļ�
	vector<string> repeat_name;
	string tmp = "";
	string repeat_id;//����ظ�ID
	int id_flag = 0;
	if (e.msg=="#ROLL")
	{
		MQ::Api::GroupAPI::SetGroupCard(e.botQQ, e.sourceId, e.activeQQ, IDname[random(IDname.size())]);
		MQ::Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::Ⱥ, e.sourceId, e.activeQQ, "�޸����[@" + e.activeQQ + "]");
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
				MQ::Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::Ⱥ, e.sourceId, e.activeQQ, "ID" + tmp + "�ظ�" + to_string(count(repeat_name.begin(), repeat_name.end(), tmp)) + "��");
				repeat_id += tmp;
				repeat_id += ",";
				id_flag = 1;
			}
		}
		if (id_flag==1)
		{
			MQ::Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::Ⱥ, e.sourceId, e.activeQQ, "�����ظ�ID"+repeat_id);
		}
		else
		{
			MQ::Api::MessageAPI::SendMsg(e.botQQ, Enum::msgType::Ⱥ, e.sourceId, e.activeQQ, "�������ظ�ID");
		}
	}
	MQ::Api::FrameAPI::OutPut("finish");
	//vector<string>::iterator it;
	//it = find(name.begin(), name.end(), "����5");
	//if (it != name.end())
	//	cout << "success" << *it << endl;
	//else
	//	cout << "fail" << endl;
	//cout << name.size() << endl;

}