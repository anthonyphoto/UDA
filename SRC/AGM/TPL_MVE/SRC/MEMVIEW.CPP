/*********************************************
* 파 일 명  
	: MemView.cpp
* 기 능 
	: Memory DB의 내용을 보여주는 Program      
* writer 
	: Han Guen Hee( jameshans@hanmail.net )                   
* first data  
	: 2000. 9. 29        	     						  
* last updata 
	: 2001. 05. 21  
* 관 련 화 일  
	: MemView.hpp  
* program history                        
 
* Version
	: 1.1 ( 2001. 05. 21 )                    		 
***********************************************/
#include "MemView.hpp"

// Constructor
CMemView::CMemView( int nSystemId, int nServiceID, int nSerial )
{
	char   	cId='A';
    char   COMMAND_TOUCH[100];
	char 	strDataFile[100];
    char 	strBackUpFile[100];
    char 	strCfgFile[100];
    
    cout << "##### [" << nSerial << "]"
			<< " MEMVIEW START ######" << endl;
			
    memset( &strDataFile, NULL, sizeof(strDataFile) );
    memset( &strBackUpFile, NULL, sizeof(strBackUpFile) );
    memset( &strCfgFile, NULL, sizeof(strCfgFile) );
    memset( &COMMAND_TOUCH, NULL, sizeof(COMMAND_TOUCH) );
    
    // FLT -> AGR
    sprintf(strDataFile, "/SVC%d/DATA/AggShm%d.cfg", nServiceID,nSerial);
    sprintf(strBackUpFile, "/SVC%d/MB/Agg%d.mb", nServiceID,nSerial);
    sprintf(strCfgFile, "/SVC%d/DATA/CommCfgFile%d.cfg",nServiceID,nSerial);
  	
  	sprintf( COMMAND_TOUCH, "touch %s",strDataFile );
	system( COMMAND_TOUCH );
	memset( &COMMAND_TOUCH, NULL, sizeof(COMMAND_TOUCH) );
	sprintf( COMMAND_TOUCH, "touch %s",strBackUpFile );
	system( COMMAND_TOUCH );
	memset( &COMMAND_TOUCH, NULL, sizeof(COMMAND_TOUCH) );
	sprintf( COMMAND_TOUCH, "touch %s",strCfgFile );
	system( COMMAND_TOUCH );
	
	pMdbMan= new MdbMan<KeyDef,UdrFormat>( strDataFile,cId,strBackUpFile,strCfgFile,MEMBERPOINTER,MAXKEYMEMBER );
	// Btree shared memory의 pointer를 얻어온다.
	pBtree = pMdbMan->get_BtreePointer();
	
    nTotalCount=0;
    nProcessingCount=0;
	nNotProcessingCount=0;
	
	if( 0 )
	{
		cout	<< "Btree Pointer:[" << pBtree << "]" << endl;
		cout	<< "pMdbMan:[" << pMdbMan << "]" << endl;
	}
	
}
// destructor
CMemView::~CMemView()
{
	// 해당 Btree Info Flag를 0으로 바꿔준다.
	//pBtree->set_BtreeFlag( 1 );
}
// Observer Method.
// 외부에서 Observer를 호출하는 Method.
// 
void CMemView::MemView( void )
{
	pBtree->shm_lock(); // MEMORY LOCK
	// Btree의 Root Pointer를 얻는다.
	pBtreeNode=pBtree->getBtreeRoot();
	// Observer를 실행하는 재귀함수.
	if( _MemView( pBtreeNode ) )
	{
		cout << "MemView : _MemView Error!!! " << endl;
		exit( 1 );
	}
	pBtree->shm_release(); // MEMORY LOCK
	cout << "**********************************" << endl;
	cout << "* Total UDR Count = " << nTotalCount << endl;
	cout << "* Processing UDR Count = " << nProcessingCount << endl;
	cout << "* Not Processing UDR Count = " << nNotProcessingCount << endl;
	cout << "**********************************" << endl;
	
}
// 실제 MemView를 실행하는 재귀함수.
// return value : 0 -> 성공적인 종료
//                -1 -> 실패
int CMemView::_MemView( _BTREENODE<KeyDef,UdrFormat>* pBtreeNode )
{
	 UDRHEAD_TYPE*	pUdrHeadType;
	 
	// Btree Node가 Null이 아니면.
	if( pBtreeNode )
	{
		// Btree Node의 key를 차례로 검색한다.
		for( int i=0; i<pBtreeNode->iNumberKey; i++ )
		{
			
			pUdrFormat = pBtreeNode->kIndexKey[i].pdataType;
			pUdrHeadType = pBtreeNode->kIndexKey[i].pdataType->get_ItemHead();
			
			// 처리가 종료된 UDR들 중에 
			if( pUdrHeadType->get_nUdrStatus() == 1 )
			{
//				cout << " Not Processing UDR in MDB" << endl;
				NotProcessingPrint( pUdrFormat );
				nNotProcessingCount++;
			}
			// 현재 처리중인 UDR이라면 
			else if( pUdrHeadType->get_nUdrStatus() == 0 )
			{
//				cout << "Processing UDR in MDB" << endl;
				ProcessingPrint( pUdrFormat );
				nProcessingCount++;
			}
			// Error Data
			else
			{
				cout << "Error received Data"   << endl;
				return -1;
			}
			nTotalCount++;
		}
		// 다음 자식 Node를 호출하여
		for( i=0; i<=pBtreeNode->iNumberKey; i++ )
		{
			// 다시 Observer를 재귀호출한다.
			_MemView( pBtreeNode->pBTREENODE[i] );
		}
		//pBtree->mem_unlock(); // MEMORY LOCK
	}
	
	return 0;
}
void CMemView::NotProcessingPrint( UdrFormat* stDataNode )
{
	UDRHEAD_TYPE*       stHeadType;
	A_INT_TYPE*         stAtype;
	B_TYPE*             stBtype;
	C_TYPE*             stCtype;
	D_TYPE*             stDtype;
	E_TYPE*             stEtype;
	F_TYPE*             stFtype;
	G_TYPE*             stGtype;
	H_TYPE*             stHtype;
	I_TYPE*             stItype;

	stHeadType=stDataNode->get_ItemHead();
	stAtype=stDataNode->get_ItemA();
	stBtype=stDataNode->get_ItemB();
	stCtype=stDataNode->get_ItemC();
	stDtype=stDataNode->get_ItemD();
	stEtype=stDataNode->get_ItemE();
	stFtype=stDataNode->get_ItemF();
	stGtype=stDataNode->get_ItemG();
	stHtype=stDataNode->get_ItemH();
	stItype=stDataNode->get_ItemI();

	cout << "<GET UDR>------------------------------------------------------------------" << endl;
 	cout << "[UDR TYPE]:" << stHeadType->get_nUdrType() << " " << "[File Number]:" << stHeadType->get_nUdrFileSeq() << " " << "[UDR SEQ NUM]:" << stHeadType->get_nUdrSeqNo() << endl;
    cout << "[UDR STATUS]:" << stHeadType->get_nUdrStatus() << endl;
    cout << "[MIN]:"<<stAtype->get_unMsId()<<endl;
 	cout << "[ACCOUNT SESSION ID]:"<<stCtype->get_strAccountSessionID() <<endl;
 	cout << "[CORRELATION ID]:"<<stCtype->get_strCorrelationID()<<endl;
    cout << "[START EVENT TIME]:"<<stGtype->get_nStartEventTime()<<endl;
	cout << "[END EVENT TIME]:"<<stGtype->get_nEndEventTime()<<endl;
// 	cout << "[ACTIVE TIME]:"<<stGtype->get_nActiveTime()<<endl;
	cout << endl;
}

void CMemView::ProcessingPrint( UdrFormat* stDataNode )
{
	UDRHEAD_TYPE*       stHeadType;
	A_INT_TYPE*         stAtype;
	B_TYPE*             stBtype;
	C_TYPE*             stCtype;
	D_TYPE*             stDtype;
	E_TYPE*             stEtype;
	F_TYPE*             stFtype;
	G_TYPE*             stGtype;
	H_TYPE*             stHtype;
	I_TYPE*             stItype;

	stHeadType=stDataNode->get_ItemHead();
	stAtype=stDataNode->get_ItemA();
	stBtype=stDataNode->get_ItemB();
	stCtype=stDataNode->get_ItemC();
	stDtype=stDataNode->get_ItemD();
	stEtype=stDataNode->get_ItemE();
	stFtype=stDataNode->get_ItemF();
	stGtype=stDataNode->get_ItemG();
	stHtype=stDataNode->get_ItemH();
	stItype=stDataNode->get_ItemI();

	cout << "<GET UDR>------------------------------------------------------------------" << endl;
 	cout << "[UDR TYPE]:" << stHeadType->get_nUdrType() << " " << "[File Number]:" << stHeadType->get_nUdrFileSeq() << " " << "[UDR SEQ NUM]:" << stHeadType->get_nUdrSeqNo() << endl;
    cout << "[UDR STATUS]:" << stHeadType->get_nUdrStatus() << endl;
    cout << "[MIN]:"<<stAtype->get_unMsId()<<endl;
 	cout << "[ACCOUNT SESSION ID]:"<<stCtype->get_strAccountSessionID() <<endl;
 	cout << "[CORRELATION ID]:"<<stCtype->get_strCorrelationID()<<endl;
    cout << "[START EVENT TIME]:"<<stGtype->get_nStartEventTime()<<endl;
	cout << "[END EVENT TIME]:"<<stGtype->get_nEndEventTime()<<endl;
// 	cout << "[ACTIVE TIME]:"<<stGtype->get_nActiveTime()<<endl;
	cout << endl;
}

int main( int argc, char* argv[] )
{
	CMemView*		cMemView;
	int				nSystemId=0;
	int				nSvcId=0;
	int				PserialNo=0;

	if( argc != 4 )
	{
		cout << "Usage CMveMain nSystemId nSvcId PserialNo" << endl;
		exit( 1 );
	}
	
	nSystemId=atoi(argv[1]);
	nSvcId=atoi(argv[2]);
	PserialNo=atoi(argv[3]);
	
    LogM	= new SendLogManage( nSystemId, nSvcId,2,1,PserialNo,0  );
    
	cMemView = new CMemView( nSystemId, nSvcId, PserialNo );
	cMemView->MemView();
	
	delete cMemView;
	delete LogM;
	return 0;
}


