#ifndef 	_HEADERMANAGER_HPP
#define 	_HEADERMANAGER_HPP

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#define SQLCA_STORAGE_CLASS extern
#include <sqlca.h>
#include "../INCLUDE/FileLogWrite.hpp"
#include "../INCLUDE/collectorRule.hpp"

EXEC SQL INCLUDE "../INCLUDE/t_meta_action.hpp";

#define		MAX_OUT_DATA_LEN 	128
#define		MAX_IN_LEN			64
#define		FAILHEADERMANAGER	-1
#define		OKHEADERMANAGER		1

//////////////////////////////////////////////////////////////////////
//
//   Souce Name  	: headermanager.hpp
//   Usage          : data 형태의 변환을 하고 header를 생성하는 
//					  class의 prototype
//   DATE           : 2001. 4 .
//   LAST UPDATE    : 2001. 5. 21
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//					  2001. 5. 21 Version update
//   Version 		: 1.1 (2001. 5. 21)
//
//////////////////////////////////////////////////////////////////////
class CHeaderMannager : public CLoadActTable
{
	private :
		int 	nActIndex;					// 	table에서 한 Record를 읽기 위한 Index (seq_no)
		char*	strOutputData;				//	output을 저장할 data
		char*	strOutOneData;				// 	output one datas
		char*	strHeaderData;				//	header data 저장 변수 

		// 0번지에서 얻는 정보 저장변수 
		int 	nOutTotLen;					// 	output data 총 길이 

		int 	nFieldCnt;					//	field의 갯수 
		
		int* 	nInOneLen;					//	one data length (내부 저장값)
		int* 	nInOffset;					//	one data offset (내부 저장값)
		int*	nDataType;					// 	DataType(input의 형을 알기 위해서 생성자에서 저장)
		int 	nInTotLen;					//	header data 생성시 총 길이 
		int 	nInputCnt;					//	input data의 count (내부 순차 증가값)
		int 	nOutOneLen;
		
	public :
        FileLogWrite*  	LogHeader;			// log 정의 
        
		CHeaderMannager(char *strMappingName,char *strActVer);
		~CHeaderMannager();
        //	accessor
		int 	get_OutTotLen()		{	return 	nOutTotLen;	}

		//	put 정의 
		void 	put_Data		(void* value);	//	data를 입력하는 부분 

		int 	headerRule		(char* indata, char *outdata);

		void 	DataMove		(char *InBuf, char *OutBuf, int nTempOutOffset);
};

#endif 
