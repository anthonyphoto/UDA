#include <iostream.h>
#include <fstream.h>
#include <unistd.h>
#include <cstdlib>

int main(int argc, char* argv[]) {

    fstream fout(argv[1], ios::app);
    cout << fout.tellp() << endl;
    if (fout.tellp() > 0) { cout << "app : " << fout.tellp() << endl; }
    int fd = fout.rdbuf()->fd();
    cout << "FD : " << fd << endl;
    lockf(fd, F_TLOCK, 0L);
    fout << "file lock ok " << getpid() << " FD : " << fd << endl;
    sleep(7);
    lockf(fd, F_ULOCK, 0L);
    sleep(atoi(argv[2]));
    fout.close();
    if (!fout) { cout << "fail close" << endl; }

    return 0;
}
