/*****************************************************************
* �� �� ��  
	: RecoveryMdb.hpp
* �� �� 
	: System down�� backup file�� ������ MDB�� �����ϴ� ���α׷�      
* writer 
	: Han Guen Hee ( jameshans@hanmail.net )                  
* first data  
	: 2000. 9. 29        	     						  
* last updata 
	: 2001. 05. 21  
* �� �� ȭ ��  
	: RecoveryMdb.cpp  
* Version
	: 1.1 ( 2001. 05. 21 )                    		 
******************************************************************/
#ifndef	RECOVERYMDB_HPP
#define RECOVERYMDB_HPP

#include<iostream.h>
#include<fstream.h>
#include<MdbMan.hpp>
#include<UDR.hpp>
#include<PrintUdr_new.hpp>

char 	PATH_NAME_DESTINATION[80] 	= "/SVC#/MB/LoadAggFile.mb";

static int	SystemId;
static int	ServiceId;
static int	GroupId;
static int	ProcessType;
static int 	PserialNo;
static int	PreMark;

//	Log Manager
SendLogManage*				LogM;

class CRecoveryMdb
{
	public:
		CRecoveryMdb( int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark );
		~CRecoveryMdb();
		int								ReadingFile( void );
	private:
		MdbMan<KeyDef,UdrFormat>*		pMdbMan;
		fstream			 				fInRecovery;
		int								RecoveryData( UdrFormat* pRecoveryData );
		UdrFormat						stRecoveryData;
		CPrintUdr						PrintData;
};


#endif

