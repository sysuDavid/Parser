#pragma once
#include "zzlexer.h"
#include "SQL.h"
#include "Column.h"

class Parser
{
public:
	Parser();
	void program();

	void creates();
	void inserts();
	void deletes();
	void querys();

	vector<Column> decl_list();
	Column decl();
	vector<Column> declA();
	int default_spec();
	vector<string> column_list();
	vector<string> column_llistB();

	vector<int> value_list();
	vector<int> value_listC();

	int simple_expr();
	int simple_term();
	vector<Token> simple_termF();
	int simple_unary();
	vector<Token> simple_unaryG();
	vector<Token> disjunct();
	vector<Token> conjunct();
	vector<Token> conjunctH();
	vector<Token> bools();
	vector<Token> boolE();
	vector<Token> comp();
	vector<Token> expr();
	vector<Token> term();
	vector<Token> termF();
	vector<Token> unary();
	vector<Token> unaryG();
	void move();
	void error(std::string s);
	void match(int);
	void setPath_Tag(int tag, const char* path);
private:
	ZZlexer* lexer;
	Token* token;
	SQL* sql;
	bool hasPrimaryKey;
	vector<string> primaryKeys;
	int tag;
};
