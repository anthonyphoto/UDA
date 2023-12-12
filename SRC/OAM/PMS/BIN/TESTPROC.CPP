#include <iostream.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	for(;;)
	{
		for(int x = 0; x < argc; x++)
			cout << argv[x] << " ";
		cout << endl;
		sleep(3);
	}

	return 0;
}
