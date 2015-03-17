/*************************************************************************
> File Name: Column.cpp
> Author: yuanwj
> Mail: yuanwj1993@gmail.com
> Created Time: Wed 24 Dec 2014 04:32:47 PM CST
************************************************************************/

#include<iostream>
#include<vector>
#include<map>
#include"Column.h"
using namespace std;

Column::Column() {
}

Column::Column(string col_name, bool flag, int value) {
	setName(col_name);
	setIsKey(flag);
	setDefaultValue(value);
}

Column::Column(string col_name, bool flag) {
	setName(col_name);
	setIsKey(flag);
	setDefaultValue(0);

}

Column::Column(string col_name, int value) {
	setName(col_name);
	setIsKey(false);
	setDefaultValue(value);
}

Column::Column(string col_name) {
	setName(col_name);
	setIsKey(false);
	setDefaultValue(0);
}

Column::Column(const Column& other) {
	name = other.name;
	values = other.values;
	is_key = other.is_key;
	default_value = other.default_value;
}

void Column::setName(string name_) {
	name = name_;

}

void Column::setValues(vector<int> values_) {
	values = values_;
}

void Column::clearValues() {
	values.clear();
}

void Column::setIsKey(bool flag) {
	is_key = flag;
}

void Column::setDefaultValue(int value) {
	default_value = value;
}

string Column::getName() {
	return name;
}

vector<int> Column::getValues() {
	return values;
}

bool Column::getIsKey() {
	return is_key;
}

int Column::getDefaultValue() {
	return default_value;
}

