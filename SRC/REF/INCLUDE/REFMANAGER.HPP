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

//FileLogWrite*  	LogM;				// log ���� 
//////////////////////////////////////////////////////////////////////
//
//   Souce Name  	: refmanager.hpp
//   Usage          : data ������ ��ȯ�� �ϰ� ���� class�� prototype
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
		int 	nActIndex;					// 	table���� �� Record�� �б� ���� Index (seq_no)
//		char*	strInputData;				//	Input�� ������ data
		char*	strOutputData;				//	output�� ������ data
		char*	strOutOneData;				// 	output one datas

		// 0�������� ��� ���� ���庯�� 
		int 	nInTotLen;					//	input data �� ����
		int 	nOutTotLen;					// 	output data �� ���� 
		int 	nZeroActionCode;			//	out mode�� �����ϴ� action code (0���� ��)

		int		nInOffset;					// 	�ش� Input Field�� ��ġ
		int 	nInOneLen;					//	input one data ����
		int 	nOutOneLen;					//	��ȯ�� �Ѱ��� field�� ���� 
		int 	nOutOffset;					//	output data offset
		int 	nFieldCnt;					//	field�� ���� 
		
		C017Pdsn*	pdsn;					//	pdsn dbhandler �ν��Ͻ� ����
				
	public :
        FileLogWrite*  	LogM;				// log ���� 
        
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
