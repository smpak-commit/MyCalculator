/*
	�򵥼�������

	����ʵ��һ���򵥵ı��ʽ��������
	��cin���룬�����浽һ��vector<vector<Token>>�����У�֧��ͬʱ���������ʽ��
	ͬʱ֧�ֶ��������Ȼ��ʹ�ñ������Ʋ�����ʽ���㡣
	���ԶԶ�����ʽ���м��㣬�������cout��

	���뷽�����£�

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

//���嵥����
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

//����ʱ��vector�����е����ݵ��ɵ���Token������ȡ
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

//���������������
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
	void get();							//�Ӽ�����������ݲ�ת�浽����vector<vector<Token>>��
	void cal();							//��vector<vector<Token>>�����е����б��ʽ���м��㲢���
	void cal(vector<Token>&);			//���㴫���һ��vector<Token>���ʽ
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
	static const char number = '8';		//���ڱ�ʶ����
	static const char name = 'a';		//��ʶ�Ǳ�������
	static const char help = 'h';
	static const bool bConst = true;
	const string result = "= ";
	const string prompt = "> ";


	vector<vector<Token>> in_expression;  //��vector������������б��ʽ
	vector<Variable> var_table;			//�������������ڱ��涨��ı�����ֵ
	bool is_declared(string var);
	double define_name(string var, double val, bool bReadonly);
	double get_value(string s);
	void set_value(string s, double d);

	bool bBadToken;

	Token_stream ts;
	double cal_now(vector<Token>);			//�������ʵ�ֺ���
	double statement();
	double declaration(char key);
	double changevalue(const string& var_name);

	double primary();
	double term();
	double expression();
};

