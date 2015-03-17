/*************************************************************************
	> File Name: Column.h
	> Author: yuanwj
	> Mail: 546377705@qq.com 
	> Created Time: Wed 24 Dec 2014 04:23:33 PM CST
 ************************************************************************/
#ifndef COLUMN_H
#define COLUMN_H

#include<iostream>
#include<vector>
#include<map>
using namespace std;

class Column {
	private:
		string name;
		vector<int> values;
		bool is_key;
		int default_value;
	public:
		Column(string, bool, int);
		Column(string, bool);
		Column(string, int);
		Column(string);
		Column(const Column&);
		Column();
		void setName(string);
		void setValues(vector<int>);
		void clearValues();
		void setIsKey(bool);
		void setDefaultValue(int);
		string getName();
		vector<int> getValues();
		bool getIsKey();
		int getDefaultValue();
};

#endif
