/*
	简单计算器类

	本类实现一个简单的表达式计算器。
	从cin读入，并保存到一个vector<vector<Token>>容器中，支持同时输入多个表达式。
	同时支持定义变量，然后使用变量名称参与表达式计算。
	可以对多个表达式进行计算，并输出到cout。

	输入方法如下：

	Statement:
		Declaration
		Expression

	Print:
		;

	Quit:
		quit or q

	
	Declaration:
		"let" name "=" expression

	Expression:
		Term
		Expression + Term
		Expression - Term

	Term:
		Primary
		Term * Primary
		Term / Primary
		Term % Primary

	Primary:
		Number
		(Expression)
		-Primary
		+Primary
		name

	Number:
		floating-point-literal
*/

#pragma once
#include "z:\std_lib_facilities.h"

//定义单词类
class Token {
public:
	char kind;
	double value;
	string name;
	Token() :kind(0), value(0){}
	Token(char ch) :kind(ch),value(0){}
	Token(char ch, double val) :kind(ch), value(val){}
	Token(char ch,string n):kind(ch),value(0), name(n){}
};

//计算时把vector容器中的内容当成单词Token流来读取
class Token_stream
{
public:
	Token_stream():token_at(0),token_in(0) {}
	Token_stream(size_t n) :token_at(0){}
	Token_stream(const vector<Token>& token);
	void put(const vector<Token>& token);
	Token get();
	void putback();
private:
	vector<Token>::size_type token_at;
	vector<Token> token_in;
};

//定义变量数据类型
class Variable {
public:
	string name;
	double value;
	bool bConst;
};
class calculator
{
public:
	calculator();
	void get();							//从键盘流获得数据并转存到容器vector<vector<Token>>中
	void cal();							//对vector<vector<Token>>容器中的所有表达式进行计算并输出
	void cal(vector<Token>&);			//计算传入的一个vector<Token>表达式
	void disphelp();

private:
	static const char sqrts = 'S';		//sqrt()
	const string sqrtkey = "sqrt";
	static const char let = 'L';		//let
	const string declkey = "let";
	static const char constant = 'C';
	const string constkey = "const";
	static const char quit = 'q';		//quit the program.
	static const char print = ';';
	static const char number = '8';		//用于标识数字
	static const char name = 'a';		//标识是变量名称
	static const char help = 'h';
	static const bool bConst = true;
	const string result = "= ";
	const string prompt = "> ";


	vector<vector<Token>> in_expression;  //用vector保存输入的所有表达式
	vector<Variable> var_table;			//变量容器，用于保存定义的变量和值
	bool is_declared(string var);
	double define_name(string var, double val, bool bReadonly);
	double get_value(string s);
	void set_value(string s, double d);

	bool bBadToken;

	Token_stream ts;
	double cal_now(vector<Token>);			//具体计算实现函数
	double statement();
	double declaration(char key);
	double changevalue(const string& var_name);

	double primary();
	double term();
	double expression();
};

