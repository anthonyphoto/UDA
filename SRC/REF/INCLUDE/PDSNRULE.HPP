#ifndef PDSN_RULE_HPP
#define PDSN_RULE_HPP

#include <iostream.h>
#include <stdlib.h>
#include <string.h>
#include <memory>

#include "../INCLUDE/convReFormat.hpp"
#include "../INCLUDE/FileLogWrite.hpp"

#define 	MAX_ONE_IN_DATA_LEN			128
#define 	ONE_TYPE_LEN				1
#define 	ONE_LENGTH_LEN				1

//	psdn code 정의 
#define 	USER_NAME					1
#define 	NAS_IP_ADDRESS				4
#define 	NAS_PORT					5
#define 	SERVICE_TYPE				6
#define		FRAMED_PROTOCOL				7
#define		FRAMED_IP_ADDRESS			8
#define		VENDOR_SPECIFIC				26
#define 	CALLING_STATION_ID			31
#define		ACCT_STATUS_TYPE			40
#define		ACCT_DELAY_TIME				41
#define 	ACCT_INPUT_OCTETS			42
#define		ACCT_OUTPUT_OCTETS			43
#define 	ACCT_SESSION_ID				44
#define 	ACCT_AUTHENTIC				45
#define 	ACCT_SESSION_TIME			46
#define 	ACCT_INPUT_PACKETS			47
#define 	ACCT_OUTPUT_PACKETS			48
#define 	ACCT_TERMINATE_CAUSE		49
#define 	ACCT_MULTI_SESSION_ID		50
#define 	ACCT_LINK_COUNT				51
#define 	ACCT_EVENT_TIME				55

#define 	NAS_PORT_TYPE				61

//////////////////////////////////////////////////////////////////////
//
//   Souce Name  	: pdsnRule.hpp
//   Usage          : UDR type을 pdsn(017) type으로 변형하는 프로그램 
//   DATE           : 2001. 3 .
//   LAST UPDATE    : 2001. 5. 21
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//					  2001. 5. 21 Version update
//   Version 		: 1.1 (2001. 5. 21)
//
//////////////////////////////////////////////////////////////////////

class CPdsnRule : public CConvReFormat
{
	private :
		int 			nOneFieldLen;
		int				nVendorLen;

		char*			strInOneData ;		// 	input data 중에서 변환될 data 저장 변수 

		//	Value 부분 헤더 정의 부분 (one data)
		unsigned char	ucType;				// 	code별 정의가 되어 있다.
		unsigned char 	ucLen;				// 	ucType size + ucLen size + data size
		char*			strOneData;			//	data 내용 부분 
	
	public :
		CPdsnRule();
		~CPdsnRule();

		// 	accessor
		int 	get_pdsnOneFieldLen()		{	return 	nOneFieldLen; 	}

		char* 	pdsnOneField	(char* indata, int nOffset, int nInOneLen, int nPdsnCode, int nVendorCode);
		char*	vendorSpecRule	(char* indata, int nOffset, int nInOneLen, int nVendorCode);
};
#endif

