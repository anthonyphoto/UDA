#ifndef COLLECTOR_RULE_HPP
#define COLLECTOR_RULE_HPP

#include <iostream.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include "../INCLUDE/convReFormat.hpp"
#include "../INCLUDE/FileLogWrite.hpp"

/////////////////////////////////////////////////////////////
//
//   Source Name    : collectorRule.hpp
//   Usage          : data 변형의 기본이 되는 프로그램 
//   DATE           : 2001. 03. 
//   LAST UPDATE    : 2001. 5. 21
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//					  2001. 5. 21 Version update
//   Version 		: 1.1 (2001. 5. 21)
//
//////////////////////////////////////////////////////////////////////

class CCollectorRule : public CConvReFormat
{
	private :
		int 	nOneFieldLen;
	public :
		CCollectorRule();
		~CCollectorRule();

		// 	accessor
		int 	get_OneFieldLen()		{	return 	nOneFieldLen; 	}

		char* 	oneField(char* indata, int nOffset, int nInOneLen, int nOutLen, int nDataType, int nActionCode, char strSubAction);
};
#endif
