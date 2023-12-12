/****************************************************************************/
/* 파 일 명 : FileLogWrite.hpp												*/
/* 작 성 자 : KSI / 조상영								                    */
/* 기    능 : 프로세스 로그내용을 화일에 출력한다. 							*/
/* 작성일자 : 2000년 12월   일                                              */
/* 변경일자 :     년   월   일                                              */
/* 관련화일 : FileLogWrite.cpp												*/
/* 기    타 :                                                               */
/****************************************************************************/
#ifndef _FILELOGWRITE_HPP
#define _FILELOGWRITE_HPP

#include <string.h>
//#include <varargs.h>
#include <stdarg.h>
#include <time.h>
#include <stdio.h>

class FileLogWrite
{
	private:
		FILE 		*fp;
		time_t  	Time;
		struct tm 	Tm;
		char		Hname[20];
		char		Path[100];
		char 		strLog[2048];		// ## 로그size (< 2000) ##
		
		void 		InitBuf(char *Buf,long Cnt);
		void 		GetHexDump(char *RBuf, const char *Buf,long BufLen);
	public:
		// path  : 로그화일이 출력될 경로
		// hname : 프로세스명 (로그화일명 = hname.YYMMDD)
		FileLogWrite(const char *path, const char *hname); 	// changed by ky. 2001.02. 07
		~FileLogWrite(){;}
		
					// 로그내용을 가변인자를 사용하여 출력한다. (ex. printf, sprintf)
		void 		Write(const char *format, ...);
					// 16진수 Hexdump를 화일에 출력한다.
		int			WriteHexLog(char *Buf,	long BufLen);
};

#endif
