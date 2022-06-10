#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <MQCore/MQHeader.h>
#include <GlobalVar.h>
#include <cstdlib> //random
#include <ctime>
using namespace std;

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
	ofstream wnfile;//写入ID文件
	ifstream rnfile;//读ID文件
	string tmp_name="";
	for (vector<string>::iterator it = QQnum.begin(); it != QQnum.end(); it++)
	{
		MQ::Api::GroupAPI::SetGroupCard(e.botQQ, e.sourceId, *it, IDname[random(IDname.size())]);
		//cout << *it << endl;
	}
	vector<string> name ;
	rnfile.open("C:\\Users\\doublezhuang\\Desktop\\MyQQ\\name.txt", ios::out);
	while (!rnfile.eof())
	{
		rnfile >> tmp_name;
		name.push_back(tmp_name);
	}

	vector<string>::iterator it;
	it = find(name.begin(), name.end(), "测试5");
	//if (it != name.end())
	//	cout << "success" << *it << endl;
	//else
	//	cout << "fail" << endl;
	//cout << name.size() << endl;
	string data;
	wnfile.open("C:\\Users\\doublezhuang\\Desktop\\MyQQ\\name.txt", ios::out);
	for (vector<string>::iterator it = name.begin(); it != name.end(); it++)
	{
		wnfile << *it << endl;
	}
	wnfile.close();

	ifstream ifile;
	ifile.open("name.txt");
	int i = 0;
	vector<string> name2;
	while (!ifile.eof())
	{
		ifile >> data;
		name2.push_back(data);
	}
	for (vector<string>::iterator it = name2.begin(); it != name2.end(); it++)
	{
		//cout << *it << endl;

	}
}