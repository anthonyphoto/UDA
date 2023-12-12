#include "hexdump.h"

int main(void)
{
	char rbuf[1024];
	char buf[50]="1234567890";
	
	GetHexDump(rbuf, buf, 50);
	printf("%s", rbuf);
	
	return 0;
}


