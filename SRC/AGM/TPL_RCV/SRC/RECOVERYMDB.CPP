/*****************************************************************
* 파 일 명  
	: RecoveryMdb.cpp
* 기 능 
	: System down시 backup file의 내용을 MDB로 복구하는 프로그램      
* writer 
	: Han Guen Hee( jameshans@hanmail.net )                   
* first data  
	: 2000. 9. 29        	     						  
* last updata 
	: 2001. 05. 21  
* 관 련 화 일  
	: RecoveryMdb.hpp  
* program history                        
 
* Version
	: 1.1 ( 2001. 05. 21 )                    		 
******************************************************************/
#include "RecoveryMdb.hpp"

// Constructor
CRecoveryMdb::CRecoveryMdb( int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark )
{
	char   cId='A';
	char   COMMAND_TOUCH[100];
	char 	strDataFile[100];
    char 	strBackUpFile[100];
    char 	strCfgFile[100];
    
    cout << "##### [" << nPserialNo << "]"
			<< " CRecoveryMdb START ######" << endl;
			
    memset( &strDataFile, NULL, sizeof(strDataFile) );
    memset( &strBackUpFile, NULL, sizeof(strBackUpFile) );
    memset( &strCfgFile, NULL, sizeof(strCfgFile) );
    memset( &COMMAND_TOUCH, NULL, sizeof(COMMAND_TOUCH) );
    
    // FLT -> AGR
    sprintf(strDataFile, "/SVC%d/DATA/AggShm%d.cfg", nServiceId,nPserialNo);
    sprintf(strBackUpFile, "/SVC%d/MB/Agg%d.mb", nServiceId,nPserialNo);
    sprintf(strCfgFile, "/SVC%d/DATA/CommCfgFile%d.cfg",nServiceId,nPserialNo);
  	
  	sprintf( COMMAND_TOUCH, "touch %s",strDataFile );
	system( COMMAND_TOUCH );
	memset( &COMMAND_TOUCH, NULL, sizeof(COMMAND_TOUCH) );
	sprintf( COMMAND_TOUCH, "touch %s",strBackUpFile );
	system( COMMAND_TOUCH );
	memset( &COMMAND_TOUCH, NULL, sizeof(COMMAND_TOUCH) );
	sprintf( COMMAND_TOUCH, "touch %s",strCfgFile );
	system( COMMAND_TOUCH );
	
	memset( &COMMAND_TOUCH, NULL, sizeof(COMMAND_TOUCH) );
	sprintf( PATH_NAME_DESTINATION, "/SVC%d/MB/LoadAggFile.mb",nServiceId );
	sprintf( COMMAND_TOUCH, "mv %s %s",strBackUpFile, PATH_NAME_DESTINATION );
	system( COMMAND_TOUCH );
	memset( &COMMAND_TOUCH, NULL, sizeof(COMMAND_TOUCH) );
	sprintf( COMMAND_TOUCH, "touch %s",strBackUpFile );
	system( COMMAND_TOUCH );
	
	pMdbMan= new MdbMan<KeyDef,UdrFormat>( strDataFile,cId,strBackUpFile,strCfgFile,MEMBERPOINTER,MAXKEYMEMBER );
	
	fInRecovery.open( PATH_NAME_DESTINATION, ios::in, ios::out );
	if( !fInRecovery )
	{
		cout << "fInRecovery File could not be opened" << endl;
		exit( 1 );
	}
}
// destructor
CRecoveryMdb::~CRecoveryMdb()
{
	fInRecovery.close();
	delete pMdbMan;
}
// ReadingFile method.
// 파일을 UDR size씩 읽어 들여서 RecoveryData method를 호출한다.
// return value : 0 -> success
//                -1 -> error
int CRecoveryMdb::ReadingFile( void )
{
	// end of file까지 UdrFormat크기씩 읽어들인다.
	fInRecovery.read( ( char *)&stRecoveryData, sizeof( UdrFormat ) );
	while( !fInRecovery.eof() )
	{
		// 읽어들인 UDR을 다시 Btree와 MDB에 넣어준다.
		if( RecoveryData( &stRecoveryData ) )
		{
			cout << "CRecoveryMdb ReadingFile : Error RecoveryData" << endl;
			return -1;
		}
		fInRecovery.read( ( char *)&stRecoveryData, sizeof( UdrFormat ) );
	}
	return 0;
}
// RecoveryData Method.
// UDR을 다시 MDB와 Btree에 Insert해서 복구시킨다.
// return value : 0 -> success
//                -1-> error
int CRecoveryMdb::RecoveryData( UdrFormat* pRecoveryData )
{
	KeyDef			cKeyDef;
	
	memset( &cKeyDef, NULL, sizeof( KeyDef ) );
	
	cKeyDef.set_unMsID( pRecoveryData->get_ItemA()->get_unMsId() );
	cKeyDef.set_strAccSessionID( pRecoveryData->get_ItemC()->get_strAccountSessionID() );
	cKeyDef.set_strCorrID( pRecoveryData->get_ItemC()->get_strCorrelationID() );

//	cout << "Recovery Msid=[" << pRecoveryData->get_ItemA()->get_unMsId() << "]" << endl;
	
	if( pRecoveryData->get_ItemA()->get_unMsId() != 0 )
	{
		// UDR을 다시 복구 시킨다.
		if( pMdbMan->insertMdb( &cKeyDef, pRecoveryData ) )
		{
			cout  << "[CRecoveryMdb[[RecoveryData]  : Error insertshm" << endl;
			return -1;
		}
		PrintData.Print( pRecoveryData );
	}
	else
	{
		cout << "Data Null or Format Error" << endl;
		PrintData.Print( pRecoveryData );
	}
	
	return 0;
}

int main( int argc, char* argv[] )
{
	CRecoveryMdb*	RecoveryMdb;
	
	if( argc != 7 )
	{
		cout << "Usage RecoveryMdb [SysId][SerId][GroupId][ProcessId][SerialNumber][Remark]" << endl;
		exit( 1 );
	}
	
	SystemId=atoi(argv[1]);
	ServiceId=atoi(argv[2]);
	GroupId=atoi(argv[3]);
	ProcessType=atoi(argv[4]);
	PserialNo=atoi(argv[5]);
	PreMark=atoi(argv[6]);
	
	LogM	= new SendLogManage(SystemId,ServiceId,GroupId ,ProcessType ,PserialNo, PreMark );
	
	RecoveryMdb = new CRecoveryMdb( SystemId,ServiceId,GroupId,ProcessType,PserialNo,PreMark );
	
	RecoveryMdb->ReadingFile();
	
	cout << "Complete Recovery DB" << endl;
	
//	system("rm -f /SVC1/MB/LoadAggFile.mb");
	delete LogM;
	delete RecoveryMdb;
	
	return 0;
}
