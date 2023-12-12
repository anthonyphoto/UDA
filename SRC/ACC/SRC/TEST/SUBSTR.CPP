#include <iostream.h>
#include <string>
#include <cstdlib>

int main() {
	string s("11ACC01");

    cout << atoi(s.substr(s.length() - 2, 2).c_str()) << endl;

	string x("ACC0102");
	cout << x.substr(3,2) << endl;
	cout << x.substr(5,2) << endl;
	return 0;
}

