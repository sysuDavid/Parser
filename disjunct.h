#pragma once
#include "token.h"
#include <queue>
#include <map>
#include <string>
#include <vector>
using namespace std;
struct G_temp {             //处理右递归文法处理整数的乘法和除法问题
    bool is_mul;            //比如 4 * 3 / 4 如果从左到右(左递归)结果是3,右递归结果是4
    int value;
};
class where_dealer{         
public:
    queue<Token> ql;
    map<string, int> mp;
	map<string, int> check;
    where_dealer(map<string,int> m,vector<Token> q){    
        mp = m;            
        for (int i = 0; i < q.size(); i++) {
            ql.push(q[i]);
        }
		map<string, int>::iterator itr = mp.begin();
		while (itr != mp.end())
		{
			check[itr->first] = 1;
			++itr;
		}
    }                      
    bool disjunct();
private:                   
    bool H();
    bool conjunct();
    bool E();
    bool bool_func();
    bool comp();
    int expr();
    vector<Token> F();
    vector<G_temp> G();
    int term();
    int unary();
    bool judge(int l, int op , int r);
};
