#include "calculator.h"

int main(int argc, char* argv[])
{
	vector<string> argument;
	for (int i = 0; i < argc;++i)
		argument.push_back(argv[i]);
	cout << "Argument: \n";
	for (auto x : argument)
		cout << x << endl;
	
	try 
	{
		calculator mycal;
		mycal.get();
		mycal.cal();
		keep_window_open("~~");
		return 0;
	}
	catch (exception& e) {
		cerr << "\nexception: " << e.what() << endl;
		char c;
		while (cin >> c && c != ';');
		keep_window_open("~~");
		return 1;
	}
	catch (...) {
		cerr << "exception\n";
		char c;
		while (cin >> c && c != ';');
		keep_window_open("~~");
		return 2;
	}
 }
