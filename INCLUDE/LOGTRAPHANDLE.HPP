#ifndef _LOGTRAPHANDLE_
#define _LOGTRAPHANDLE_


#include <iostream.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include <time.hpp>
#include <SqlCommon.hpp>


class LogTrapHandle
{
	private:

	public :
	
		LogTrapHandle(char* strDBName, char* strDBPwd);
		~LogTrapHandle();
		void	insertTrap(int TrapNo, char* TrapName, int TrapValue, char* ErrorMsg);
};

#endif
