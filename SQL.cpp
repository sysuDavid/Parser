/*************************************************************************
> File Name: SQL.cpp
> Author: yuanwj
> Mail: 546377705@qq.com
> Created Time: Mon 22 Dec 2014 03:41:55 PM CST
************************************************************************/

#include<iostream>
#include<string>
#include<vector>
#include<map>
#include <sstream>
#include"SQL.h"
using namespace std;

SQL::SQL()
{
}

bool SQL::create(string table_name, vector<Column> table, vector<string> key_string) {
	map< string, map<string, Column> >::iterator iter;
	iter = database.find(table_name);
	if (iter != database.end()) {
		cout << "Table " << table_name << " has existed" << endl;
		return false;
	}
	if (key_string.size() > 100) {
		cout << "# of columns in primary key declaration > 100 in the table " << table_name << endl;
		return false;
	}
	map<string, Column> m;
	vector<Column>::iterator col;
	map<string, int> exist; // prevent duplicate column names
	bool contain = false; // detect primary key not in the table
	for (col = table.begin(); col != table.end(); col++) {
		if (exist[col->getName()] != 0) {
			cout << "Duplicate column names: " << col->getName() << ", in the table " << table_name << endl;
			return false;
		}
		m.insert(pair<string, Column>(col->getName(), *col));
		exist[col->getName()] = 1;
		if (m.size() > 100) {
			cout << "# of columns in the table " << table_name << " > 100." << endl;
			return false;
		}
	}
	for (int i = 0; i < key_string.size(); i++) {
		if (exist[key_string[i]] == 1){
			m[key_string[i]].setIsKey(true);
			contain = true;
		}
	}
	if (key_string.size() == 0) {
		contain = true;
	}
	if (contain == false) {
		cout << "Primary key not in the table " << table_name << endl;
		return false;
	}
	contain = false;
	database.insert(pair< string, map<string, Column> >(table_name, m));
	name_exist.insert(pair< string, map<string, int> >(table_name, exist));
	key_names.insert(pair< string, vector<string> >(table_name, key_string));
	return true;
}

string intToString(vector<int> vals) {
	string str;
	string tmp;
	if (vals.size() > 0) {
		stringstream ss;
		ss << vals[0];
		ss >> tmp;
		str += tmp;
	}
	if (vals.size() > 1) {
		for (int i = 1; i < vals.size(); i++) {
			stringstream ss;
			ss << vals[i];
			ss >> tmp;
			str += "-";
			str += tmp;
		}
	}
	return str;
}

bool SQL::insert(string table_name, vector<string> names, vector<int> rows) {
	map< string, map<string, Column> >::iterator iter;
	iter = database.find(table_name);
	if (iter == database.end()) {
		cout << "Table " << table_name << " not found" << endl;
		return false;
	}
	if (names.size() != rows.size()) {
		cout << "# of columns no equal to # of values" << endl;
		return false;
	}
	map<string, Column> old_table = iter->second;
	map<string, Column> new_table;
	map<string, Column>::iterator col_iter;
	int i = 0;
	vector<int> values;
	Column col;
	map<string, int> exist;
	vector<string> keyname = key_names[table_name];
	vector<int> keyvalue;
	for (vector<string>::iterator name = names.begin(); name != names.end(); name++) {
		col_iter = old_table.find(*name);
		if (col_iter == old_table.end()) {
			cout << "Column " << *name << " not found in the " << table_name << " table." << endl;
			return false;
		}
		if (exist[*name] != 0) {
			cout << "Duplicate column name: " << *name << endl;
			return false;
		}
		exist[*name] = 1;
		col = col_iter->second;
		values = col.getValues();
		for (int k = 0; k < keyname.size(); k++) {
			if (keyname[k] == *name) {
				keyvalue.push_back(rows[i]);
			}
		}
		values.push_back(rows[i]);
		i++;
		col.setValues(values);
		new_table.insert(pair<string, Column>(*name, col));

	}
	string str;
	int val;
	for (col_iter = old_table.begin(); col_iter != old_table.end(); col_iter++) {
		col = col_iter->second;
		str = col.getName();
		if (exist[str] == 0) {
			val = col.getDefaultValue();
			values = col.getValues();
			for (int k = 0; k < keyname.size(); k++) {
				if (keyname[k] == str) {
					keyvalue.push_back(val);
				}
			}
			values.push_back(val);
			col.setValues(values);
			new_table.insert(pair<string, Column>(str, col));
		}
	}
	string keyvalue_string = intToString(keyvalue);
	map<string, int> keyvalue_map = key_values[table_name];
	if (keyvalue_map[keyvalue_string] != 0) {
		cout << "Primary key value has exist" << endl;
		return false;
	}
	if (keyvalue_string != "") {
		keyvalue_map[keyvalue_string] = 1;
	}
	key_values[table_name] = keyvalue_map;
	iter->second = new_table;
	return true;
}

bool SQL::deletes(string table_name)
{
	map<string, map<string, Column> >::iterator iter;
	iter = database.find(table_name);
	if (iter == database.end())
	{
		cout << "Table " << table_name << " not found" << endl;
		return false;
	}
	map<string, Column> new_table;
	map<string, Column> old_table = iter->second;
	map<string, Column>::iterator col_iter = old_table.begin();
	int affect = col_iter->second.getValues().size();
	string col_name;
	Column col;
	for (; col_iter != old_table.end(); col_iter++)
	{
		col_name = col_iter->first;
		col = col_iter->second;
		col.clearValues();
		new_table.insert(pair<string, Column>(col_name, col));
	}
	iter->second = new_table;
	cout << affect << " rows affected." << endl;
	return true;
}

bool SQL::deletes(string table_name, vector<Token> t)
{
	int affect = 0;
	map<string, map<string, Column> >::iterator iter;
	iter = database.find(table_name);
	if (iter == database.end())
	{
		cout << "Table " << table_name << " not found" << endl;
		return false;
	}
	map<string, Column> new_table;
	map<string, Column> old_table = iter->second;
	map<string, Column>::iterator col_iter;
	vector<vector<int> > vec;
	map<string, int> m;
	vector<string> names;
	for (col_iter = old_table.begin(); col_iter != old_table.end(); col_iter++)
	{
		vec.push_back( (col_iter->second).getValues() );
		names.push_back(col_iter->first);
	}
	for (int i = 0; i < vec[0].size(); i++)
	{
		for (int j = 0; j < names.size(); j++)
		{
			m[names[j]] = vec[j][i];
		}
		try
		{
			where_dealer w = where_dealer(m, t);
			if (w.disjunct() == true)
			{
				for (int k = 0; k < names.size(); k++)
				{
					vec[k].erase(vec[k].begin() + i);
				}
				i--;
				affect++;	
			}			
		}
		catch (WrongTypeException)
		{
			return false;
		}
	}
	Column col;
	for (int i = 0; i < names.size(); i++)
	{
		col = old_table[names[i]];
		col.setValues(vec[i]);
		new_table.insert(pair<string, Column>(names[i], col) );
	}
	iter->second = new_table;
	cout << affect << " rows affected." << endl;
	return true;
}

void SQL::display(vector<vector<int> > vec, vector<string> str)
{
	int strlength = 10;
	for (int i = 0; i < str.size(); i++)
	{
		int l = str[i].length();
		if (l > strlength)
			strlength = l;
	}
	if (vec.size() == 0)
	{
		printEdge(strlength, str.size() );
		return;
	}
	int length = vec[0].size();
	printEdge(strlength, str.size());
	for (int k = 0; k < str.size(); k++)
	{
		cout << "|";
		for (int i = 0; i < (3 * strlength - str[k].length() ) / 2; i++)
			cout << " ";
		cout << str[k];
		for (int i = 0; i < (3 * strlength - str[k].length() + 1) / 2; i++)
			cout << " ";
	}
	cout << "|" << endl;
	for (int i = 0; i < length; i++)
	{
		printEdge(strlength, str.size());
		for (int j = 0; j < str.size(); j++)
		{
			int num = vec[j][i];
			cout << "|";
			for (int k = 0; k < (3 * strlength - digits(num)) / 2; k++)
				cout << " ";
			cout << num;
			for (int k = 0; k < (3 * strlength - digits(num) + 1) / 2; k++)
				cout << " ";
		}
		cout << "|" << endl;
	}
	printEdge(strlength, str.size());
}

int SQL::digits(int num)
{
	if (num == 0)
		return 1;
	int digit;
	digit = num > 0 ? 0 : 1;
	while (num != 0)
	{
		num /= 10;
		digit++;
	}
	return digit;
}

void SQL::printEdge(int strlength, int n)
{
	for (int i = 0; i < n; i++)
	{
		cout << "|";
		for (int j = 0; j < strlength * 3; j++)
		{
			cout << "-";
		}
	}
	cout << "|" << endl;
}

bool SQL::select(string table_name, vector<string> names) {
	map< string, map<string, Column> >::iterator iter;
	iter = database.find(table_name);
	if (iter == database.end()) {
		cout << "Table " << table_name << " not found" << endl;
		return false;
	}
	map<string, Column> old_table = iter->second;
	vector< vector<int> > sel;
	map<string, Column>::iterator map_iter = old_table.begin();
	int affect = map_iter->second.getValues().size();
	vector<string> allnames;
	if (names[0] == "*") {
		for (; map_iter != old_table.end(); map_iter++) {
			sel.push_back((map_iter->second).getValues());
			allnames.push_back(map_iter->first);
		}
	}
	else {
		for (int i = 0; i < names.size(); i++) {
			map_iter = old_table.find(names[i]);
			if (map_iter == old_table.end()) {
				cout << "Column " << names[i] << " not found in the " << table_name << " table." << endl;
				return false;
			}
			sel.push_back((map_iter->second).getValues());
		}
	}
	if (names[0] == "*") {
		display(sel, allnames);
	}
	else {
		display(sel, names);
	}
	cout << affect << " rows affected." << endl;
	return true;
}

bool SQL::select(string table_name, vector<string> names, vector<Token> t) {
	map< string, map<string, Column> >::iterator iter;
	iter = database.find(table_name);
	if (iter == database.end()) {
		cout << "Table " << table_name << " not found" << endl;
		return false;
	}
	map<string, Column> old_table = iter->second;
	map<string, Column>::iterator col_iter;
	vector< vector<int> > allvec;
	map<string, int> m;
	vector<string> allnames;
	vector< vector<int> > vec(names.size());
	vector< vector<int> > vec_(old_table.size());
	map<string, int> exist;
	int affect = 0;
	for (col_iter = old_table.begin(); col_iter != old_table.end(); col_iter++) {
		allvec.push_back((col_iter->second).getValues());
		allnames.push_back(col_iter->first);
	}
	for (int i = 0; i < allvec[0].size(); i++) {
		for (int j = 0; j < allnames.size(); j++) {
			m[allnames[j]] = allvec[j][i];
			exist[allnames[j]] = 1;
		}
		where_dealer w = where_dealer(m, t);
		if (w.disjunct() == true) {
			if (names[0] != "*") {
				for (int k = 0; k < names.size(); k++) {
					if (exist[names[k]] == 0) {
						cout << "The select column " << names[k] << " not in the table " << endl;
						return false;
					}
					vec[k].push_back(m[names[k]]);
				}
			}
			else {
				for (int k = 0; k < allnames.size(); k++) {
					vec_[k].push_back(m[allnames[k]]);
				}
			}
			affect++;
		}
	}
	if (names[0] == "*") {
		display(vec_, allnames);
	}
	else {
		display(vec, names);
	}
	cout << affect << " rows affect." << endl;
	return true;
}

map<string, map<string, Column> > SQL::getDatabase()
{
	return database;
}
