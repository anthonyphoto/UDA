#include <iostream.h>
#include <fcntl.h>
#include <fstream.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

    struct flock fvar;
    int fd;

    fvar.l_type   = F_WRLCK;
    fvar.l_whence = SEEK_SET;
    fvar.l_start   = 0;
    fvar.l_len     = 0;

    fstream fout(argv[1], ios::app);
    fd = fout.rdbuf()->fd();

    while (fcntl(fd, F_SETLKW, &fvar) == -1) {
        while (fcntl(fd, F_GETLK, &fvar) != -1 && fvar.l_type != F_UNLCK) {
            cout << "this is " << getpid() 
                 << " locked by " << fvar.l_pid << " from " << fvar.l_start << " for "
                 << fvar.l_len << " byte for " << (fvar.l_type == F_WRLCK ? 'w' : 'r') << endl;
        }    
    }
    
    fout << "file lock ok" << fvar.l_pid << endl;
    sleep(5);
    fvar.l_type   = F_UNLCK;
    fvar.l_whence = SEEK_SET;
    fvar.l_start   = 0;
    fvar.l_len     = 0;
    if (fcntl(fd, F_SETLKW, &fvar) == -1) { cout << "fail to fcntl" << endl; return 0; }
    fout << "file unlock ok" << fvar.l_pid << endl;

    return 0;
}
