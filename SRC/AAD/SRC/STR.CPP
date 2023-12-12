#include <iostream.h>
#include <string>

int main() {
	string s("abde    ");
	string x("abde");


	if (s.find(x) == string::npos) {
		cout << "failure" << endl;
    }

	return 0;
}
