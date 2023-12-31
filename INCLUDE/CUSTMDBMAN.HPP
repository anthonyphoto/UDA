/******************************************************************
* 파 일 명     :  CustMdbMan.hpp			     					  
* 기   능                                                          
* : Customer Memory DB Operation 정의						      
* 작 성 자     : Han Guen Hee( jameshans@hanmail.net )                    				  
* first data  : 2000. 11. 07       	     						  
* last updata : 2000. 12. 21 
* Version	  : 1.1( 2001. 05. 21 )       		 					  
* program history                        						  
******************************************************************/
#ifndef CUSTMDBMAN_HPP
#define CUSTMDBMAN_HPP

#include <fstream.h>
#include <stdio.h>
#include <iostream.h>
#include <time.h>
#include <sys/unistd.h>

#include <CustomerBtree.hpp>
#include <CdsCustFormat.hpp>

const   int					MIN_LEN = 10;
const 	int					JOBCODE_LEN = 2;
const   int					ACCINFO_LEN = 2;
const   int					CUSTMDB_DEBUG=1;
const   int                 SIZE_CUSTFORMATMDB  = sizeof( CustFormatMdb );
//		char 				PATH_NAME_CUSTOMER_MDB[80] 	= "/SVC#/MB/SUBS.mb";

class CCustMdbMan
{
	public:	
		CCustMdbMan();
		CCustMdbMan( char* DataFile, char DataId, char* BackupFile, int nMemberPoint, int nMaxKeyMember );
		//CCustMdbMan(char *Data_Name_Shm, char Data_Id_Shm,int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark );
		~CCustMdbMan();
		int           				InsertCustMdb( CustFormat stCustFormat );
		int							UploadDbToMdb( CustFormatMdb stCustFormatMdb );
		int							SearchMdb( int nSearchMin );
		int          	 			DeleteCustMdb( CustFormat stCustFormat );
		int							UpdateCustMdb( CustFormat stCustFormat );
		CCustomerBtree*		 		get_CustBtreePointer() { return pCustomerBtree; }
	private:
		CustFormatMdb 				cCustFormatMdb;
		CCustomerBtree*         	pCustomerBtree;
		ShmMemory*         			shmData;
		CustFormatMdb* 				stPtrDataIndexInfo;
		HandleOfAllocIndex*   		pAllocDataIndexC;
		fstream         			fInOut;
		void 						PrintCustFormatMdb( ostream& output, CustFormatMdb* pCustFormatMdb );
		int 						ConversionData( CustFormatMdb* pCustFormatMdb, CustFormat stCustFormat );
};
// Constructor
CCustMdbMan::CCustMdbMan()
{
}
//===============================================================
// Constructor Method											=
//  : Customer MDB에 필요한 메모리와 객체를 생성.               		=
// argument : 프로세스 형상구분 6가지, Key Path, Key Id				=
// return value : None											=
//																=
//===============================================================
//CCustMdbMan::CCustMdbMan(char *Data_Name_Shm, char Data_Id_Shm, int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark )
CCustMdbMan::CCustMdbMan( char* DataFile, char DataId, char* BackupFile, int nMemberPoint, int nMaxKeyMember )
{
	/*
	shmData=new SharedMemory( nSystemId,nServiceId,nGroupId,nProcessType,nPserialNo,nPreMark+3, SIZE_CUSTFORMATMDB*MAXKEYMEMBER );
    Data_Id_Shm++;
    */
    shmData=new ShmMemory( DataFile, DataId, SIZE_CUSTFORMATMDB*nMaxKeyMember, 0660|IPC_CREAT );
	DataId++;
	
	stPtrDataIndexInfo=( CustFormatMdb * )shmData->ShmPtr();
	//stPtrDataIndexInfo=( dataType * )shmData->get_ShmPtr();
	pAllocDataIndexC=new HandleOfAllocIndex( (const char*)DataFile, ( const unsigned char )DataId, nMaxKeyMember, ( unsigned int )stPtrDataIndexInfo );
	//pAllocDataIndexC=new HandleOfAllocIndex( nSystemId,nServiceId,nGroupId,nProcessType,nPserialNo,nPreMark+2,MAXKEYMEMBER, ( unsigned int )stPtrDataIndexInfo );
	DataId++;
//	Data_Id_Shm++;
	pCustomerBtree=new CCustomerBtree( DataFile, DataId );
	
	/*
	sprintf( PATH_NAME_CUSTOMER_MDB, "/SVC%d/MB/SUBS.mb",nServiceId );
	// initialize
    fInOut.open( PATH_NAME_CUSTOMER_MDB, ios::in | ios::out | ios::binary );
    */
    fInOut.open( BackupFile, ios::in | ios::out | ios::binary );
    if( !fInOut )
    {
	    cout << "File could not be opened fMdbBackup.txt." << endl;
	    exit( 1 );
	}
}
// destructor
CCustMdbMan::~CCustMdbMan()
{
	delete shmData;
	delete pAllocDataIndexC;
	delete pCustomerBtree;
	fInOut.close();
}
//===============================================================
// ConversionData Method					   					=
//  : 입력된 CustFormat을 MDB Format으로 변환해주는 Method			=
// argument     :  pCustFormatMdb->MDB Format Pointer			=
//				   stCustFormat->입력받은 CustFormat				=
// return value : 1-> success									=
//                0-> fail										=
//																=
//===============================================================
int CCustMdbMan::ConversionData( CustFormatMdb* pCustFormatMdb, CustFormat stCustFormat )
{
	unsigned char 	cCode;		// 0x11
	char			cJobCode[JOBCODE_LEN+1];	// A1:신규가입 Z1:가입해지
	char			cMin[MIN_LEN+1];	
	char			cAccInfo[ACCINFO_LEN+1];	// 01:TTL	
	char 			cDate[20];  
    char 			cTime[20];
    unsigned int	nMin = 0;
	unsigned int    temp;
	unsigned long 	divtemp;
    time_t 			curtime;
    struct 			tm *loctime;
    int				nCount;

    curtime = time( NULL );
    loctime = localtime( &curtime);
 
	strftime( cDate,20, "%Y%m%d", loctime );              
    strftime( cTime,20, "%H%M%S", loctime);                 

	cCode = stCustFormat.get_Code();
	memcpy( cJobCode, stCustFormat.get_JobCode(), JOBCODE_LEN );
	memcpy( cMin, stCustFormat.get_Min(), MIN_LEN );
	memcpy( cAccInfo, stCustFormat.get_AccInfo(), ACCINFO_LEN );

	if( 0 )
	{
		cout << "cCode: " << cCode << endl;
		cout << "cJobCode: " << cJobCode << endl;
		cout << "cMin: " << cMin << endl;
		cout << "cAccInfo: " << cAccInfo << endl;
	}
	pCustFormatMdb->set_EventDate( cDate );	
	pCustFormatMdb->set_EventTime( cTime );
	// 아직 정의되지 않아서 NULL을 유지한다.
	//pCustFormatMdb->set_PricePlan(const char* strValue);
	
	if( !memcmp( cAccInfo, "01",2 ) )
		pCustFormatMdb->set_ServiceType(1);
	else
		pCustFormatMdb->set_ServiceType(0);
		
	if( !memcmp( cJobCode, "A1",2 ) )
		pCustFormatMdb->set_CustStatus(1);
	else if( !memcmp( cJobCode, "Z1",2 ) )
		pCustFormatMdb->set_CustStatus(2);
	else if( !memcmp( cJobCode, "D1",2 ) )
		pCustFormatMdb->set_CustStatus(1);
	else
	{
	//	cout << "different customer status" << endl;
		return 0;
	}
	nCount=9;	
	divtemp=1000000000;
	for( int i=0; i<10; i++ )
	{
		temp = (unsigned int)( cMin[i]-48 );
		nMin += (unsigned int)( temp * divtemp );
		divtemp /= 10;
		nCount--;
	}
	pCustFormatMdb->set_MsID( nMin );
	//PrintCustFormatMdb( cout, pCustFormatMdb );
	return 1;
}
//===============================================================
// InsertCustMdb Method						   					=
//  : 입력된 CustFormat을 MDB에 삽입하는 Method					=
// argument     :  stCustFormat->입력받은 CustFormat				=
// return value : 1-> success									=
//                0-> fail( Memory overflow )					=
//																=
//===============================================================
int CCustMdbMan::InsertCustMdb( CustFormat stCustFormat )
{
    _DATATYPE 		stDataType;
	CustFormatMdb*	stPtrReturnDataAddress;
	unsigned int 	unBaseAddress;
	unsigned int 	unCurrentAddress;

	if( ConversionData( &cCustFormatMdb, stCustFormat ) )
    {
		if( 0 )
		{
			cout << "Success Conversion CustFormat -> MDB Format" << endl;
		}
    }
    else
	{
		if( 0 )
		{
			cout << "Error Conversion CustFormat -> MDB Format" << endl;
		}
		return 0;
	}  

	if( 0 )
	{
		PrintCustFormatMdb( cout, &cCustFormatMdb );
	}

	// memory allocate.
	stPtrReturnDataAddress=( CustFormatMdb* )pAllocDataIndexC->newIndex( ( unsigned int )sizeof( CustFormatMdb ) );
    if(stPtrReturnDataAddress == 0 )
    {
		 cout << " Overflow***************" << endl;
		 fInOut.close();
		 return 0;
    }
    unCurrentAddress=( unsigned int )stPtrReturnDataAddress;
    unBaseAddress=( unsigned int )pAllocDataIndexC->get_unBaseAddress();

	//data insert.
	memcpy( stPtrReturnDataAddress, &cCustFormatMdb, sizeof( CustFormatMdb ) );
	if( 0 )
	{
		cout << "unMsId: " << stPtrReturnDataAddress->get_MsID() << endl;
	}

    // file wirte.
	fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
	fInOut.write( ( char * )stPtrReturnDataAddress, sizeof( CustFormatMdb ) );
	fInOut.flush();

	// Btree insert.
	stDataType.iDataKey=stPtrReturnDataAddress->get_MsID();
	stDataType.pCustFormatMdb=stPtrReturnDataAddress;
	pCustomerBtree->InsertData(stDataType);
	if( 0 )
	{
		cout << "stDataType iDataKey: " << stDataType.iDataKey << endl;
		cout << "stDataType pCustFormatMdb: " << stDataType.pCustFormatMdb << endl;
		cout << "unMsId: " << stPtrReturnDataAddress->get_MsID() << endl;
	}
	return 1;
}
//===============================================================
// UploadDbToMdb Method						   					=
//  : DB의 내용을 MDB에 넣어주는 Method							=
// argument     :  stCustFormatMdb->입력받은 CustFormatMdb		=
// return value : 1-> success									=
//                0-> fail( Memory overflow )					=
//																=
//===============================================================
int CCustMdbMan::UploadDbToMdb( CustFormatMdb stCustFormatMdb )
{
    _DATATYPE 		stDataType;
	CustFormatMdb*	stPtrReturnDataAddress;
	unsigned int 	unBaseAddress;
	unsigned int 	unCurrentAddress;

	// memory allocate.
	stPtrReturnDataAddress=( CustFormatMdb* )pAllocDataIndexC->newIndex( ( unsigned int )sizeof( CustFormatMdb ) );
    if(stPtrReturnDataAddress == 0 )
    {
		 cout << " Overflow***************" << endl;
		 fInOut.close();
		 return 0;
    }
    unCurrentAddress=( unsigned int )stPtrReturnDataAddress;
    unBaseAddress=( unsigned int )pAllocDataIndexC->get_unBaseAddress();

	//data insert.
	memcpy( stPtrReturnDataAddress, &stCustFormatMdb, sizeof( CustFormatMdb ) );
	if( 0 )
	{
		cout << "unMsId: " << stPtrReturnDataAddress->get_MsID() << endl;
	}

    // file wirte.
	fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
	fInOut.write( ( char * )stPtrReturnDataAddress, sizeof( CustFormatMdb ) );
	fInOut.flush();

	// Btree insert.
	stDataType.iDataKey=stPtrReturnDataAddress->get_MsID();
	stDataType.pCustFormatMdb=stPtrReturnDataAddress;
	pCustomerBtree->InsertData(stDataType);
	if( 0 )
	{
		cout << "stDataType iDataKey: " << stDataType.iDataKey << endl;
		cout << "stDataType pCustFormatMdb: " << stDataType.pCustFormatMdb << endl;
		cout << "unMsId: " << stPtrReturnDataAddress->get_MsID() << endl;
	}
	return 1;
}
//===============================================================
// DeleteCustMdb Method						   					=
//  : 입력된 CustFormat을 MDB에서 삭제하는 method					=
// argument     :  stCustFormat->입력받은 CustFormat				=
// return value : 1-> success									=
//                0-> fail										=
//																=
//===============================================================
int CCustMdbMan::DeleteCustMdb( CustFormat stCustFormat )
{
	int 			iIndex;
    _DATATYPE		stDataType;
    CustFormatMdb* 	pDeleteDataNode;
    unsigned int 	unBaseAddress;
    unsigned int	unCurrentAddress;

	if( ConversionData( &cCustFormatMdb, stCustFormat ) )
    {
		if( 0 )
		{
			cout << "Success Conversion CustFormat -> MDB Format" << endl;
		}
    }
    else
	{
		if( 0 )
		{
			cout << "Error Conversion CustFormat -> MDB Format" << endl;
		}
		return 0;
	}
	  
    stDataType.iDataKey=cCustFormatMdb.get_MsID();
    pDeleteDataNode=pCustomerBtree->ShowSearch(&stDataType);

    if(pDeleteDataNode == NULL)
    {
        cout << "_TransactionDelete:ShowSearch() Error!!\n";
        return 0;
    }
    unCurrentAddress=(unsigned int)pDeleteDataNode;
    unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();

    //Btree index를 초기화 시켜준다.
    memset((char *)pDeleteDataNode,0x00,sizeof(CustFormatMdb));

    // file update
    fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
    fInOut.write( ( char * )pDeleteDataNode, sizeof( CustFormatMdb ) );
    fInOut.flush();

    // Btree data를 delete.
    pCustomerBtree->DelNode(stDataType);
    pAllocDataIndexC->deleteIndex( (( unCurrentAddress-unBaseAddress )/sizeof( CustFormatMdb )));

	return 1;
}
/*
// return value : 1 -> Update success
//                0 -> Update fail
int CCustMdbMan::UpdateCustMdb( CustFormat stCustFormat )
{
    _DATATYPE 		stDataType;
	CustFormatMdb*	pUpdateDataNode;
	unsigned int 	unBaseAddress;
	unsigned int 	unCurrentAddress;

//	cout << "==========>UpdateCustMdb " << endl;
	if( ConversionData( &cCustFormatMdb, stCustFormat ) )
    {
		if( 0 )
		{
			cout << "Success Conversion CustFormat -> MDB Format" << endl;
		}
    }
    else
	{
		if( 0 )
		{
			cout << "Error Conversion CustFormat -> MDB Format" << endl;
		}
		return 0;
	}  

	if( 0 )
	{
		PrintCustFormatMdb( cout, &cCustFormatMdb );
	}

	stDataType.iDataKey=cCustFormatMdb.get_MsID();
    pUpdateDataNode=pCustomerBtree->ShowSearch(&stDataType);

    if(pUpdateDataNode == NULL)
    {
        cout << "_TransactionDelete:ShowSearch() Error!!\n";
        return 0;
    }
    unCurrentAddress=(unsigned int)pUpdateDataNode;
    unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();


	//data insert.
	memcpy( pUpdateDataNode, &cCustFormatMdb, sizeof( CustFormatMdb ) );
	if( 0 )
	{
		cout << "unMsId: " << pUpdateDataNode->get_MsID() << endl;
	}

    // file wirte.
	fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
	fInOut.write( ( char * )pUpdateDataNode, sizeof( CustFormatMdb ) );
	fInOut.flush();

	return 1;
}
*/
//===============================================================
// UpdateCustMdb Method						   					=
//  : 입력된 CustFormat을 MDB에서 갱신하는 method					=
// argument     :  stCustFormat->입력받은 CustFormat				=
// return value : 1-> success									=
//                0-> fail										=
//																=
//===============================================================
int CCustMdbMan::UpdateCustMdb( CustFormat stCustFormat )
{
	stCustFormat.set_JobCode("Z1");
	if( DeleteCustMdb(  stCustFormat ) )
	{
		// cout << " complete DeleteCustMdb in UpdateCustMdb" < endl;
	}
	else
		return 0;

	stCustFormat.set_JobCode("A1");
	stCustFormat.set_Min(stCustFormat.get_Min2());
	if( InsertCustMdb(  stCustFormat ) )
	{
		// cout << " complete InsertCustMdb in UpdateCustMdb" < endl;
	}
	else
		return 0;
	return 1;
}
//===============================================================
// SearchMdb Method						   						=
//  : 입력된 CustFormat을 MDB에서 갱신하는 method					=
// argument     :  nSearchMin->찾고자 하는 Min					=
// return value : 1-> Found										=
//                0-> Not Found									=
//																=
//===============================================================
int CCustMdbMan::SearchMdb( int nSearchMin )
{
	_DATATYPE		stTempDataType;
	CustFormatMdb*	pTempCustFormatMdb;
	if( 0 )
	{
		cout << "<<<<<<<<<<<<<<<<<<<" << endl;
		cout << "nSearchMin= " << nSearchMin << endl;
		cout << "<<<<<<<<<<<<<<<<<<<" << endl;
	}
	stTempDataType.iDataKey=nSearchMin;
	if( 0 )
	{
		cout << "pCustomerBtree: " << pCustomerBtree << endl;
		pCustomerBtree->Print();
	}
	pTempCustFormatMdb=pCustomerBtree->ShowSearch( &stTempDataType );

	if( 0 )
	{
		cout << "pTempCustFormatMdb: " << pTempCustFormatMdb << endl;
	}
	if( pTempCustFormatMdb == NULL )
	{
		if( 0 )
		{
			cout << "Not Exist MsID in DataBase!!" << endl;
		}
		return 0;
	}
	else
	{
		if( 0 )
		{
			cout << "Find MsID :" << pTempCustFormatMdb->get_MsID() << endl; 
		}
		return 1;
	}
}
//===============================================================
// PrintCustFormatMdb Method				   					=
//  : CustFormatMdb를 echo print해주기 위한 method.  		  		=
// argument     :  output -> 출력되는 대상.						=
//				   pCustFormatMdb -> 출력할 MDB data				=
// return value : None											=
//																=
//===============================================================
void CCustMdbMan::PrintCustFormatMdb( ostream& output, CustFormatMdb* pCustFormatMdb )
{
	output << setiosflags( ios::left) 
		   << setw( 11) << "MsID"  
		   << setw( 11) << "EventDate"
		   << setw( 11) << "EventTime"
		   << setw( 11) << "ServiceType"
		   << setw( 11) << "PricePlan"
		   << setw( 11) << "CustStatus" << '\n';
	output << setiosflags( ios::left) 
		   << setw( 11) << pCustFormatMdb->get_MsID()
		   << setw( 11) << pCustFormatMdb->get_EventDate()
		   << setw( 11) << pCustFormatMdb->get_EventTime()
		   << setw( 11) << pCustFormatMdb->get_ServiceType()
		   << setw( 11) << pCustFormatMdb->get_PricePlan()
		   << setw( 11) << pCustFormatMdb->get_CustStatus() << '\n';
}


#endif





