#ifndef 	_REFMANAGER_HPP
#define 	_REFMANAGER_HPP

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#define SQLCA_STORAGE_CLASS extern
#include <sqlca.h>
#include "../INCLUDE/FileLogWrite.hpp"
#include "../INCLUDE/collectorRule.hpp"
#include "../INCLUDE/pdsnRule.hpp"

EXEC SQL INCLUDE "../INCLUDE/t_meta_action.hpp";
EXEC SQL INCLUDE "../INCLUDE/t_017pdsn_out.hpp";

#define 	OUT_MAX_BUFFER		480
#define		MAX_OUT_DATA_LEN 	128
#define		FAILREFMANAGER		-1
#define		OKREFMANAGER		1

//FileLogWrite*  	LogM;				// log 정의 
//////////////////////////////////////////////////////////////////////
//
//   Souce Name  	: refmanager.hpp
//   Usage          : data 형태의 변환을 하가 위한 class의 prototype
//   DATE           : 2001. 3 .
//   LAST UPDATE    : 2001. 5. 21
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//					  2001. 5. 21 Version update
//   Version 		: 1.1 (2001. 5. 21)
//
//////////////////////////////////////////////////////////////////////
class CRefMannager : public CLoadActTable
{
	private :
		int 	nActIndex;					// 	table에서 한 Record를 읽기 위한 Index (seq_no)
//		char*	strInputData;				//	Input을 저장한 data
		char*	strOutputData;				//	output을 저장할 data
		char*	strOutOneData;				// 	output one datas

		// 0번지에서 얻는 정보 저장변수 
		int 	nInTotLen;					//	input data 총 길이
		int 	nOutTotLen;					// 	output data 총 길이 
		int 	nZeroActionCode;			//	out mode를 결정하는 action code (0번지 값)

		int		nInOffset;					// 	해당 Input Field의 위치
		int 	nInOneLen;					//	input one data 길이
		int 	nOutOneLen;					//	변환될 한개의 field의 길이 
		int 	nOutOffset;					//	output data offset
		int 	nFieldCnt;					//	field의 갯수 
		
		C017Pdsn*	pdsn;					//	pdsn dbhandler 인스턴스 선언
				
	public :
        FileLogWrite*  	LogM;				// log 정의 
        
        //	accessor
        int 	get_InTotLen()		{	return 	nInTotLen;	}
		int 	get_OutTotLen()		{	return 	nOutTotLen;	}
		
		CRefMannager(char *strMappingName,char *strActVer);
		~CRefMannager();
		
		int 	collectorRule	(char* indata, char *outdata);
		int 	pdsnRule		(char* indata, char* outdata);

		void 	DataMove		(char *InBuf, char *OutBuf, int nTempOutOffset);
};

#endif 

