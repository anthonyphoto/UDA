// DataBase Connect, Disconnect Class
#include <iostream.h>
#ifndef CDLIB_H
#define CDLIB_H

#define OK		1	// 성공
#define FAIL	0	// 실패

class CDblib
{
public:
	// user id, password infomatin. 
	char userid[15];
	char userpass[15];

	int connectDB(char *, char *);		// id, password argument.
	int disconnectDB();
};

#endif
