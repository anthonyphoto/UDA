#ifndef CONVREFORMAT_HPP
#define CONVREFORMAT_HPP

#include <iostream.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <memory>
#include <string>
#include <time.h>

#define 	IPLEN	16
/////////////////////////////////////////////////////////////
//
//   Source Name    : convReFormat.hpp
//   Usage          : type별로 되어 있는 것을 string 형태로 변환
//					  하는 클래스의 prototype
//   DATE           : 2001. 03. 
//   LAST UPDATE    : 2001. 5. 21
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//					  2001. 5. 21 Version update
//   Version 		: 1.1 (2001. 5. 21)
//
//////////////////////////////////////////////////////////////////////

class CConvReFormat
{
	private :
	
	public : 
		//	형 변환에 관련된 함수
		char* 			convTimeToStr   (int nValue, int size);
		char*			convShortToStr 	(short sValue, int size);
		char*			convUnShortToStr(unsigned short usValue, int size);
		char*			convIntToStr 	(int nValue, int size);
		char*			convCharToStr 	(char cValue, int size);
		char*			convUnIntToStr 	(unsigned int unValue, int size);
		char*			convLongToStr 	(long lValue, int size);
		char*			convUnLongToStr (unsigned long ulValue, int size);
		int				convStrToInt	(char* strValue);
		unsigned int 	convStrToUnInt  (char* strValue);
		long 			convStrToLong	(char* strValue);
		unsigned long 	convStrToUnLong (char* strValue);
		short			convStrToShort	(char* strValue);
		unsigned short	convStrToUnShort(char* strValue);

		//	내부 구조의 변환에 관련된 함수
		char* 			spaceToNull 	(char* strValue, int size);
		char*			nullToSpace		(char* strValue, int size);
		char* 			ipToStr 		(unsigned int unIP);
		char*			convPrefixChar	(char* strValue, char cPrefix, int size);
		char*			convPostfixChar	(char* strValue, char cPostfix, int size);

};

#endif
