#include <iostream.h>

void display(int nA)
{
	cout << "nA = " << nA << endl;
}

int main()
{
	unsigned char ucA;
	char*	ucPtrA;
	ucA = 'A';

	ucPtrA = (char*)&ucA;

	display((int)*ucPtrA);
}

