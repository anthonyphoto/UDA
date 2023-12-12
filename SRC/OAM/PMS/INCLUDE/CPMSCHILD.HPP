
#ifndef _PMS_CLIENT_HANDLER
#define _PMS_CLIENT_HANDLER
#include <iostream.h>
#include <signal.h>
#include <unistd.h>
#include "ipcMQ.hpp"
#include "errno.h"

extern bool RegisterSignalHandler();
extern void sig_handler(int i);
extern char id[3];
extern int nStop;
#endif
