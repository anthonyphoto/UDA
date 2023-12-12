#include <iostream.h>

int sum (void* a, void* b);
int main ()
{
	int tot, a, b ;
	
	a = 10, b = 20 ;
	
	tot = sum(&a, &b);
	
	cout << "tot = " << tot << endl;
}

int sum (void* a, void* b)
{
	int aa, bb;
	
	memcpy (&aa, a, 4);
	memcpy (&bb, b, 4);
	
	return aa+bb;
}

