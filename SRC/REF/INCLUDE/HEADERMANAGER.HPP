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
//   Usage          : data ������ ��ȯ�� �ϰ� header�� �����ϴ� 
//					  class�� prototype
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
		int 	nActIndex;					// 	table���� �� Record�� �б� ���� Index (seq_no)
		char*	strOutputData;				//	output�� ������ data
		char*	strOutOneData;				// 	output one datas
		char*	strHeaderData;				//	header data ���� ���� 

		// 0�������� ��� ���� ���庯�� 
		int 	nOutTotLen;					// 	output data �� ���� 

		int 	nFieldCnt;					//	field�� ���� 
		
		int* 	nInOneLen;					//	one data length (���� ���尪)
		int* 	nInOffset;					//	one data offset (���� ���尪)
		int*	nDataType;					// 	DataType(input�� ���� �˱� ���ؼ� �����ڿ��� ����)
		int 	nInTotLen;					//	header data ������ �� ���� 
		int 	nInputCnt;					//	input data�� count (���� ���� ������)
		int 	nOutOneLen;
		
	public :
        FileLogWrite*  	LogHeader;			// log ���� 
        
		CHeaderMannager(char *strMappingName,char *strActVer);
		~CHeaderMannager();
        //	accessor
		int 	get_OutTotLen()		{	return 	nOutTotLen;	}

		//	put ���� 
		void 	put_Data		(void* value);	//	data�� �Է��ϴ� �κ� 

		int 	headerRule		(char* indata, char *outdata);

		void 	DataMove		(char *InBuf, char *OutBuf, int nTempOutOffset);
};

#endif 