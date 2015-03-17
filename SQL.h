/*************************************************************************
> File Name: sql.h
> Author: yuanwj
> Mail: 546377705@qq.com
> Created Time: Mon 22 Dec 2014 02:25:30 PM CST
************************************************************************/
#ifndef SQL_H
#define SQL_H
#include<iostream>
#include<vector>
#include<map>
#include<string>
#include"Column.h"
#include "disjunct.h"
using namespace std;

class SQL {
private:
	map< string, map<string, Column> > database;
	map< string, map<string, int> > name_exist;
	map< string, map<string, int> > key_values;
	map< string, vector<string> > key_names;
public:
	SQL();
	bool create(string table_name, vector<Column> table, vector<string> key_string);
	bool insert(string table_name, vector<string> names, vector<int> row);
	bool deletes(string table_name);
	bool deletes(string table_name, vector<Token> t);
	bool select(string table_name, vector<string> names);
	bool select(string table_name, vector<string> names, vector<Token> t);
	map< string, map<string, Column> > getDatabase();
	void display(vector< vector<int> > vec, vector<string> str);
	void printEdge(int, int);
	int digits(int num);
};
#endif
