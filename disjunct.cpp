#include "disjunct.h"
#include <queue>
#include <map>
#include <string>
#include <vector>
#include <iostream>
using namespace std;


const int
UNKNOWN    = 256,
CREATE     = 257,  TABLE      = 258,  INT    = 259,
DEFAULT    = 260,  PRIMARY    = 261,  KEY    = 262,
INSERT     = 263,  INTO       = 264,  VALUES = 265,
DELETE     = 266,  FROM       = 267,  WHERE  = 268,
SELECT     = 269,  NEQ        = 270,  EQ     = 271,
LT         = 272,  GT         = 273,  LEQ    = 274,
GEQ        = 275,  PLUS       = 276,  MINUS  = 277,
STAR       = 278,  DIV        = 279,  AND    = 280,
OR         = 281,  NOT        = 282,  ASN    = 283,
ID         = 284,  NUM        = 285,  EEOS   = 286,
COMMA      = 287,  LP         = 288,  RP     = 289,
EEOF       = 290;

bool where_dealer::disjunct()       //文法(下同,略去)disjunct → conjunct H
{
    bool result = conjunct();
    result = result || H();
    return result;
}

bool where_dealer::H()              //H → || conjunct H | ε
{
    bool result;
    switch (ql.front().tag) {
        case OR:
            ql.pop();               //当在本文法产生式中,遇到终结符(就是对应非终结符的那个终结符)
            result = conjunct();    //队列才会pop
            break;
        default:
            ql.pop();
            return false;
    }
    result = result || H();
    return result;
}

bool where_dealer::conjunct()       //conjunct → bool E
{
    bool result = bool_func();
    result = result && E();
    return result;
}

bool where_dealer::E()              //E → && bool E | ε
{
    bool result;
    switch(ql.front().tag) {
        case AND:
            ql.pop();
            result = bool_func();
            break;
        default:
            return true;
    }
    result = result && E();
    return result;
}

bool where_dealer::bool_func()      //bool → (disjunct) | !bool | comp
{
    bool result;
    switch(ql.front().tag)
    {
        case LP:
            ql.pop();
            result = disjunct();
            ql.pop();
            break;
        case NOT:
            ql.pop();
            result = bool_func();
            if (result) result = false;
            else result = true;
            break;
        case MINUS:
        case PLUS:
        case ID:
        case NUM:
            result = comp();
            break;
    }
    return result;
}

bool where_dealer::comp()           //comp → expr rop expr
{
    /*
     For temp_op:
     1:<> , 2:== , 3:> , 4:< , 5:>= , 6:<=
     */
    int temp1,temp2,temp_op;
    bool result;
    switch(ql.front().tag)
    {
        case MINUS:
        case PLUS:
        case ID:
        case NUM:
            temp1 = expr();
            break;
    }
    switch(ql.front().tag)
    {
        case NEQ:
            temp_op = 1;
            break;
        case EQ:
            temp_op = 2;
            break;
        case GT:
            temp_op = 3;
            break;
        case LT:
            temp_op = 4;
            break;
        case GEQ:
            temp_op = 5;
            break;
        case LEQ:
            temp_op = 6;
            break;
    }
    ql.pop();
    switch(ql.front().tag)
    {
        case MINUS:
        case PLUS:
        case ID:
        case NUM:
            temp2 = expr();
            break;
    }
    return judge(temp1, temp_op, temp2);
}

bool where_dealer::judge(int l, int op , int r)
{
    switch(op) {
        case 1:
            return l != r;
        case 2:
            return l == r;
        case 3:
            return l > r;
        case 4:
            return l < r;
        case 5:
            return l >= r;
        default:
            return l <= r;
    }
}

int where_dealer::expr()            //expr → term F
{
    int temp = term();
    if (ql.front().tag == MINUS || ql.front().tag == PLUS)
    {
        vector<Token> tokens = F();
        for (int i = 0; i < tokens.size(); i++)
        {
            temp = tokens[i].tag == MINUS ? temp - tokens[++i].value : temp + tokens[++i].value;
        }
    }
    return temp;
}

vector<Token> where_dealer::F()               //F → + term F | - term F | ε
{
    vector<Token> temp;
    switch(ql.front().tag)
    {
        case PLUS:
        case MINUS:
            temp.push_back(ql.front());
            ql.pop();
            temp.push_back(term());
            break;
        default:
            return temp;
    }
    std::vector<Token> v = F();
    temp.insert(temp.end(),v.begin(),v.end());
    return temp;
}

int where_dealer::term()            //term → unary G
{
    int temp = unary();
    vector<G_temp> t = G();
    /*
    结果右递归整数除法的问题:用一个vector来存储所有的要进行操作的对象(G_temp类型)
    每个G_temp的变量是一个带有bool is_mul和int value的结构体
    is_mul为true表明做乘法，反之做除法
    */
    for (vector<G_temp>::iterator it = t.begin(); it != t.end(); it++) {
        if (it->is_mul) temp *= it->value;
        else {
            //当除以0的时候，抛出异常，本cpp文件唯一会抛出异常的地方
            if (it->value == 0) { 
                cout << "divide by zreo" << endl;
                throw WrongTypeException();
            }
            temp /= it->value;
        }
    }
    return temp;
}

vector<G_temp> where_dealer::G()    //G → * unary G | / unary G | ε
{
    vector<G_temp> temp_v,temp_v2;
    G_temp temp;
    switch(ql.front().tag)
    {
        case STAR:
            ql.pop();
            temp.value = unary();
            temp.is_mul = true;
            break;
        case DIV:
            ql.pop();
            temp.value = unary();
            temp.is_mul = false;
            break;
        default:
            return temp_v;
    }
    temp_v.push_back(temp);
    temp_v2 = G();
    for (vector<G_temp>::iterator it = temp_v2.begin(); it != temp_v2.end(); it++) {
        temp_v.push_back(*it);
    }
    return temp_v;
}

int where_dealer::unary()           //unary → -unary | +unary | id | num
{
    int temp;
    switch(ql.front().tag)
    {
        case MINUS:
            ql.pop();
            temp = -1 * unary();
            break;
        case PLUS:
            ql.pop();
            temp = unary();
            break;
        case ID:
			if (check[ql.front().lexeme]){
				temp = mp[ql.front().lexeme];
			}
			else 
			{
				cout << "column not found in table" << endl;
				throw WrongTypeException();
			}
			ql.pop();
			break;
        case NUM:
            temp = ql.front().value;
            ql.pop();
            break;
    }
    return temp;
}
