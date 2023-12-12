/****************************************************************************/
/* �� �� �� : FileLogWrite.hpp												*/
/* �� �� �� : KSI / ����								                    */
/* ��    �� : ���μ��� �α׳����� ȭ�Ͽ� ����Ѵ�. 							*/
/* �ۼ����� : 2000�� 12��   ��                                              */
/* �������� :     ��   ��   ��                                              */
/* ����ȭ�� : FileLogWrite.cpp												*/
/* ��    Ÿ :                                                               */
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
		char 		strLog[2048];		// ## �α�size (< 2000) ##
		
		void 		InitBuf(char *Buf,long Cnt);
		void 		GetHexDump(char *RBuf, const char *Buf,long BufLen);
	public:
		// path  : �α�ȭ���� ��µ� ���
		// hname : ���μ����� (�α�ȭ�ϸ� = hname.YYMMDD)
		FileLogWrite(const char *path, const char *hname); 	// changed by ky. 2001.02. 07
		~FileLogWrite(){;}
		
					// �α׳����� �������ڸ� ����Ͽ� ����Ѵ�. (ex. printf, sprintf)
		void 		Write(const char *format, ...);
					// 16���� Hexdump�� ȭ�Ͽ� ����Ѵ�.
		int			WriteHexLog(char *Buf,	long BufLen);
};

#endif