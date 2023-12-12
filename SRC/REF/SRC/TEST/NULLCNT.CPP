#include <iostream.h>

int main()
{
	char* a;
	int nNull;
	a = new char[5];
	char b = '0';
	memset (a , NULL , 5);
	memcpy (a , &b , 1);
	
	for (int i=0 ; i<5 ; i++)
	{
		if(a[i] == NULL)
			nNull++;
	}
	
	cout << "Null Cnt = " << nNull << endl;
	cout << "result = " << a << endl;
}
