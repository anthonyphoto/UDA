#include <signal.h>
#include <iostream.h>

#ifndef _SIGNAL_HPP_
#define _SIGNAL_HPP_

namespace sig {

int g_stop = 1;

void stop(int sig_no) {
    cout << "catched signal : " << sig_no << endl;
    g_stop = 0;
}

void init_signal() {
    static struct sigaction act;
    act.sa_handler = stop;
    sigaction(SIGUSR1, &act, NULL);
}

}; // end of namespace sig

#endif // _SIGNAL_HPP_

