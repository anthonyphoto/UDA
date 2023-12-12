#include "../INCLUDE/signal.hpp"
#include <iostream.h>
#include <unistd.h>

using namespace sig;


int main() {
	init_signal();

	while (g_stop) {
		sleep(1);
		cout << "go!" << endl;
    }

    cout << "work complete" << endl;

	return 0;
}
