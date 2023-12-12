#include <ctype.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
	char a[20]; //="12cbD34Ff";
	int	len;

	strcpy(a, (char *)"12cbD3Ff");
	len = strlen(a);

	while(len--) a[len]=toupper(a[len]);

	printf("\n%s\n", a);

	return 1; 

}

