#include <iostream.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	sleep(5);

	if (argv[1][0] == 'T')
		exit(0);
	else
		exit(-1);
}
