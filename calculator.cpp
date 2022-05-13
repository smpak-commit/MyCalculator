#include "calculator.h"

calculator::calculator()
{
	bBadToken = false;
	define_name("pi", 3.1415926535, bConst);
	define_name("e",2.71828284, bConst);
	define_name("k", 1000, bConst);
}

void calculator::disphelp()
{
	cout << "Welcome to our simple calculator.\n"
		<< "Please enter expression using floating-point numbers.\n"
		<< "We can handle +,-,*,/,%, and sqrt(): e.g., 1+2*3+sqrt(9);\n"
		<< "handle variable: let x=9; sqrt(x);\n"
		<< "handle constant: pi = 3.1415926535, e = 2.71828284, k = 100. \n"
		<< "add new constant: const inch=2.54;\n"
		<< "Add an ; to end expression, enter quit/q to end calculate."
		<< "\n" << prompt;
}

void calculator::get()
{
	disphelp();
	bBadToken = false;
	vector<Token> temp;
	char ch;

	while (true)
	{
		cin >> ch;
		switch (ch)
		{
		case print:
			if (bBadToken||temp.size() == 0)
			{
				bBadToken = false;
			}
			else
			{
				in_expression.push_back(temp);
				cal(temp); 
			}
			if (cin.get() == '\n')
				cout << prompt;  //���һ�����������ʽʱ����ֹ��������ǰ��ʾ��ʾ����
			else
				cin.unget();
			temp.clear();
			break;

		case quit:
			if (temp.size() > 0 && !bBadToken)
			{
				in_expression.push_back(temp);
				cal(temp);
			}
			else
				bBadToken = false;
			return;

		case '(':case ')':case '+':case '-':case '*':case '/':case '%':
			temp.push_back(Token{ ch });
			break;

		case '.':
		case '0':case '1':case '2':case '3':case '4':
		case '5':case '6':case '7':case '8':case '9':
			cin.putback(ch);
			double val;
			cin >> val;
			if (cin.fail())  //��ȡ����.ʱ��ӦĬ��Ϊ0����������Ҫ�������״̬��
			{
				cin.clear();
				cout << "cin read number error, to clear: cin >> val = " << val << endl;
			}
			temp.push_back(Token{ number,val });
			break;

		case sqrts:
			temp.push_back(Token(sqrts));
			break;

		case let:
			temp.push_back(Token{ let });
			break;
		
		case constant:
			temp.push_back(Token{ constant });
			break;

		case '=':
			temp.push_back(Token{ ch });
			break;

		default:
			if (isalpha(ch))
			{
				string s;
				s += ch;
				while (cin.get(ch) && (isalpha(ch) || isdigit(ch))) s += ch; //���Ʊ�����������ֻ��ΪӢ����ĸ��ͷ����ĸ������
				cin.putback(ch);

				//������ַ��������жϣ���������Ϣת������ȻҲ����ֱ��������д����ˡ�
				if (s == "help" || s == "h" || s == "H") {
					disphelp();
					cin.get(ch);
					while (ch==print)
					{
						cin.get(ch);
					}
					cin.unget();
					break;
				}					
				if (s == "quit" || s == "q"||s=="Q") {
					cin.putback(quit); break;
				}
				if (s == declkey) {
					cin.putback(let);break;
				}
				if (s == sqrtkey) {
					cin.putback(sqrts);break;
				}
				if (s == constkey) {
					cin.putback(constant);break;
				}
				temp.push_back(Token{ name, s });
				break;
			}

			bBadToken = true;
			cerr << "Bad token.\n" ;
			temp.clear();
			break;
		}
	}
}

//���˳���������������б��ʽ�������չʾ�����㣨������ʽû�н����ų���
void calculator::cal()
{
	if (in_expression.size() == 0) return;
	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
	cout << "Your enter all expression:\n";
	for (auto& temp : in_expression)
	{
		for (auto& x : temp)
		{
			switch (x.kind)
			{
			case number:
				cout << x.value;
				break;
			case let:
				cout << declkey << ' ';  //�����͹ؼ���תΪ�����ַ���
				break;
			case constant:
				cout << constkey << ' ';
				break;
			case name:
				cout << x.name;
				break;
			case sqrts:
				cout << sqrtkey;
				break;
			default:
				cout << x.kind;
			}
		}
		switch (temp[0].kind) {
		case let:
			cout << " -> a Variable." << endl;
			break;
		case constant:
			cout << " -> a Constant." << endl;
			break;
		case name:
			if (temp.size() > 1 && temp[1].kind == '=')
				cout << " -> ";
			else
				cout << "= ";
			cout << cal_now(temp) << endl;
			break;
		default:
			cout << result << cal_now(temp) << endl;
		}
	}
}

//ÿ��һ�����ʽ����������͵��ý���������ʽ�����㡣
void calculator::cal(vector<Token>& temp)
{
	if (temp.size() == 0) return;
	for (auto& x : temp)
	{
		switch (x.kind)
		{
		case number:
			cout << x.value;
			break;
		case sqrts:
			cout << sqrtkey;
			break;
		case let:
			cout << declkey << ' ';
			break;
		case constant:
			cout << constkey << ' ';
			break;
		case name:
			cout << x.name;
			break;
		default:
			cout << x.kind;
		}
	}
	switch (temp[0].kind)
	{
	case let:case constant:
		cout << " -> " << cal_now(temp) << endl;
		break;
	case name:
		if (temp.size()>1 && temp[1].kind == '=')
			cout << " -> ";
		else
			cout << "= ";
		cout << cal_now(temp) << endl;
		break;
	
	default:
		cout << result << cal_now(temp) << endl;
	}
}

//ͨ��cal����������ʽ�󣬿�ʼ����
double calculator::cal_now(vector<Token> in)
{
	try {
		ts.put(in);
		return statement();
	}
	catch (exception& e) {
		cerr << e.what() << endl;
	}
}

bool calculator::is_declared(string var)
{
	for (const Variable& v : var_table)
		if (v.name == var)return true;
	return false;
}

double calculator::define_name(string var, double val, bool bReadonly)
{
	if (is_declared(var)) error(var, ", declared twice.");
	var_table.push_back(Variable{ var, val, bReadonly });
	return val;
}

double calculator::get_value(string s)
{
	for (const Variable& v : var_table)
		if (v.name == s)return v.value;
	error(", get: underfined variable ", s);
}

void calculator::set_value(string s, double d)
{
	for(Variable& v:var_table)
		if (v.name == s) {
			if (v.bConst)
				error(", set: readonly constant ", s);
			else
				v.value = d;
			return;
		}
	error(", set: undefined variable ", s);
}

//���ݱ��ʽ���ͣ��ֱ���ò�ͬ�ĺ������д���
double calculator::statement()
{
	Token t = ts.get();
	switch (t.kind)
	{
	case let:
		return declaration(let);
	case constant:
		return declaration(constant);
	case name:
		return changevalue(t.name);
	default:
		ts.putback();
		return expression();
	}	
}

double calculator::declaration(char key)
{
	Token t = ts.get();
	if (t.kind != name) error(", name expected in declaration.");
	string var_name = t.name;

	Token t2 = ts.get();
	if (t2.kind != '=')error(", = missing in declaration of ", var_name);

	double d = expression();
	switch (key) {
	case let:
		define_name(var_name, d, !bConst);
		break;
	case constant:
		define_name(var_name, d, bConst);
		break;
	default:
		error("nothing...");
	}	
	return d;	
}

double calculator::changevalue(const string& var_name)
{
	Token t = ts.get();

	if (t.kind != '=')   //��ֹ���Ǹ�ֵ����������ñ������߳������м���
	{
		ts.putback();
		ts.putback();
		double d = expression();
		return d;
	}

	double d = expression();
	set_value(var_name, d);
	return d;
}

//���������������Ǻ��ı��ʽ���㺯��
double calculator::primary()
{
	Token t = ts.get();
	switch (t.kind)
	{
	case '(':
		{
			double d = expression();
			t = ts.get();
			if (t.kind != ')') error(", ')' expected.");
			return d;
		}

	case number:
		return t.value;

	case '-':
		return -primary();

	case '+':
		return primary();

	case name:
		return get_value(t.name);

	case sqrts:
		return sqrt(primary());

	default:
		error(", primary expected.");
	}
}

double calculator::term()
{
	double left = primary();
	
	while (true)
	{
		Token t = ts.get();
		switch (t.kind)
		{
		case '*':
			left *= primary();
			break;

		case '/':
		{
			double d = primary();
			if (d == 0) error(", divide by zero.");
			left /= d;
			break;
		}
		case '%':
		{
			double d = primary();
			if (d == 0) error(", divide by zero.");
			left = fmod(left, d);
			break;
		}

		case ';':
			return left;

		default:
			ts.putback();
			return left;
		}
	}
}

double calculator::expression()
{
	double left = term();
	while (true)
	{
		Token t = ts.get();
		switch (t.kind)
		{
		case '+':
			left += term();
			break;

		case '-':
			left -= term();
			break;

		case ';':
			return left;

		default:
			ts.putback();
			return left;
		}
	}
}

//������������������
Token_stream::Token_stream(const vector<Token>& token)
{
	token_at = 0;
	token_in = token;
}

void Token_stream::put(const vector<Token>& token)
{
	token_at = 0;
	token_in = token;
}


Token Token_stream::get()
{
	if (token_at < token_in.size())
	{
		Token t = token_in[token_at];
		++token_at;
		return t;
	}
	else
		return Token{ ';', 0 };
}

void Token_stream::putback()
{
	if (token_at > 0)
		--token_at;
}
