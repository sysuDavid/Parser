#pragma once
#include <string>
class WrongTypeException
{
};
class FileNotOpenException
{
};
class Token
{
public:
	int tag;

	int ln;
	int col;

	char character;
	int value;
	std::string lexeme;

	// default constructor
	Token() {
	}

	// Token UNKNOWN
	Token(char c) {
		this->tag = 256;
		this->character = c;
	}

	// Token is a Number
	Token(int v) {
		this->tag = 285;
		this->value = v;
	}

	// Token is a Word
	Token(int wt, std::string l) {
		this->tag = wt;
		this->lexeme = l;
	}

	Token(int wt, const char* cstr) {
		this->tag = wt;
		std::string l(cstr);
		this->lexeme = l;
	}

	void setLocation(int line, int column) {
		this->ln = line;
		this->col = column;
	}
	void setToken(Token token)
	{
		this->lexeme = token.lexeme;
		this->ln = token.ln;
		this->col = token.col;
		this->character = token.character;
		this->value = token.value;
		this->tag = token.tag;
	}
};

