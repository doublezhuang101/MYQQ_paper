#pragma once
#include<iostream>
#include <fstream>
#include <vector>
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

void test(vector<string> &QQnum)
{
	ofstream wqfile;
	wqfile.open("C:\\Users\\doublezhuang\\Desktop\\MyQQ\\QQ.txt", ios::out);
	for (vector<string>::iterator it = QQnum.begin(); it != QQnum.end(); it++)
	{
		wqfile << *it << endl;
	}
	wqfile.close();
}

void Roll_name()
{
	ofstream wnfile;//写入ID文件
	ifstream rnfile;//读ID文件
	string tmp_name="";
	vector<string> name ;
	rnfile.open("C:\\Users\\doublezhuang\\Desktop\\MyQQ\\name.txt", ios::out);
	while (!rnfile.eof())
	{
		rnfile >> tmp_name;
		name.push_back(tmp_name);
	}
	for (vector<string>::iterator it = name.begin(); it != name.end(); it++)
	{
		//cout << *it << endl;
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