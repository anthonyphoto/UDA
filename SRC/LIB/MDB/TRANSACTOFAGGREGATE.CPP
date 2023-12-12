#include<MDB/TransactOfAggregate.hpp>

//	Log Manager
extern SendLogManage* AggregatorLogM;

// 생성자. 
// 1. file descriptor open.
// 2. Btree instance 생성. 
// 3. Data shared memory instance create.
// 4. Data index shared memory instance create.
// 
CTransactOfAggregate::CTransactOfAggregate(char* Data_Name_Shm, char Data_Id_Shm, int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark)
{
	char BtreeId;
	
	//*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			//		<< "CTransactOfAggregate Contructor!!" << endl;

	// old=============>shmData=new ShmMemory( Data_Name_Shm, Data_Id_Shm, SIZE_UDRFORMAT*MAXKEYMEMBER, 0660|IPC_CREAT );
	shmData=new SharedMemory( nSystemId,nServiceId,nGroupId,nProcessType,nPserialNo,nPreMark+3, SIZE_UDRFORMAT*MAXKEYMEMBER );
	
	Data_Id_Shm++;
	stPtrDataIndexInfo=( UdrFormat * )shmData->get_ShmPtr();
	// old========> pAllocDataIndexC=new HandleOfAllocIndex( Data_Id_Shm, MAXKEYMEMBER, ( unsigned int )stPtrDataIndexInfo );
	pAllocDataIndexC=new HandleOfAllocIndex_New( nSystemId,nServiceId,nGroupId,nProcessType,nPserialNo,nPreMark+2,MAXKEYMEMBER, ( unsigned int )stPtrDataIndexInfo );
	
	Data_Id_Shm++;
	pBtree=new CBtree( Data_Name_Shm, Data_Id_Shm,nSystemId,nServiceId,nGroupId,nProcessType,nPserialNo,nPreMark );
//	cout << " constructor pBtree: " << pBtree << endl;

	sprintf( PATH_NAME_AGGREGATOR_MDB, "/SVC%d/MB/Agg%d.mb",nServiceId,nPserialNo );	
	// initialize
	fInOut.open( PATH_NAME_AGGREGATOR_MDB, ios::in | ios::out | ios::binary );
	if( !fInOut )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(4207) 
					<< "File could not be opened!!" << endl;
		AggregatorLogM->put_EventHistData(FLAG_END, -1);
		delete AggregatorLogM;
		exit( 1 );
	}
	if( 0 )
	{
		cout << "shmData(DATA SHAREDMEM): " << shmData << endl;
		cout << "pAllocDataIndexC(DATA Alloc):" << pAllocDataIndexC << endl;
		cout << "Btree Pointer: " << pBtree << endl;
	}
}
CTransactOfAggregate::CTransactOfAggregate()
{
}
CTransactOfAggregate::~CTransactOfAggregate()
{ 
	delete shmData;
	delete pBtree;
	delete pAllocDataIndexC;
	fInOut.close(); 
}
// test data가 제대로 입력되고 변경되는지 확인하기 위한 메소드.
// 실제는 log file로 대체됨. 
void CTransactOfAggregate::readshm( )
{
}
// update 메소드.
// update하기위한 데이터를 입력받아 해당 메소드를 update.
// _TransactionUpdate메소드를 호출하여 처리. 
int CTransactOfAggregate::updateshm( UdrFormat *pUdrFormat )
{
	//*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
	//		<< "CTransactOfAggregate : updateshm!!" << endl;	
	if( _TransactionUpdate( pUdrFormat ) )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "updateshm:_TransactionUpdate Error!!" << endl;	
		return -1;
	}
	return 0;
}
// insert method.
// data를 입력받아, _TransactionInsert method를 호출해서 insert.
int CTransactOfAggregate::insertshm( UdrFormat*	pUdrFormat )
{
	//*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
		//	<< "CTransactOfAggregate : insertshm!!" << endl;	
	if( _TransactionInsert( pUdrFormat ) )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "insertshm:_TransactionInsert Error!!" << endl;	
		return -1;
	}
	return 0;
}
// delete method. 
// delete하고자 하는 키를 입력받아,
// _TransactionDelete method를 호출하여 delete.
int CTransactOfAggregate::deleteshm( UdrFormat*	pUdrFormat )
{
	A_INT_TYPE*	pAtype;

//	*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
	//		<< "CTransactOfAggregate : deleteshm!!" << endl;	
	pAtype=pUdrFormat->get_ItemA();
	if( _TransactionDelete( pAtype->get_unMsId() ) )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "deleteshm:_TransactionDelete Error!!" << endl;	
		return -1;
	}
	return 0;
}
// select method.
// 처리(update)하고자 하는 값을 입력받아,
// _TransactionSelect method를 호출하여,
// return vlaue( _DISKNODE *)를 가지고 일을 처리.
void CTransactOfAggregate::selectshm( )
{
	int iSelectKey;
	UdrFormat*	pSelectDataNode;
	UdrFormat pFileSelect;
	unsigned int unBaseAddress;
	unsigned int unCurrentAddress;
	A_INT_TYPE*		pAtype;

	//*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
		//	<< "CTransactOfAggregate : selectshm!!" << endl;	
	/*
	cout << "input select key: ";
	cin >> iSelectKey;
	*/
	for( int i=0; i<MAXKEYMEMBER; i++)
	{
		iSelectKey=rand() % MAXKEYMEMBER;
		pSelectDataNode=_TransactionSelect( iSelectKey );
		if(pSelectDataNode == NULL)
		{
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "selectshm: _TransactionSelect key not found !!" << endl;	
		}
	}
	// file에서 읽을 포인터를 setting하기위해서 
	// 해당 주소의 index를 계산하기위한 변수.
	unCurrentAddress=(unsigned int)pSelectDataNode;
	unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();

	// file read 
	// 해당 위치로 포인터를 옮겨준다.
	fInOut.seekg( ( ( unCurrentAddress-unBaseAddress )/sizeof( UdrFormat ) )*sizeof( UdrFormat ) );
	fInOut.read( ( char * )&pFileSelect, sizeof( UdrFormat ) );
	pAtype=pFileSelect.get_ItemA();

}
// _TransctionSelect method.
// return value: UdrFormat *(찾은 key의 실제 address.)
UdrFormat*	CTransactOfAggregate::_TransactionSelect( unsigned int iSearchKey )
{
    _DATATYPE stDataType;

    // Btree의 key(struct _DATATYPE)형으로 넣어준다.
    stDataType.iDataKey=iSearchKey;
    return pBtree->ShowSearch(&stDataType);
}

// _TransctionSelect method.
// return value: UdrFormat *(찾은 key의 실제 address.)
int CTransactOfAggregate::_TransactionDelete( unsigned int iSearchKey )
{
      int iIndex;
     _DATATYPE stDataType;
    UdrFormat*	pDeleteDataNode;
    unsigned int unBaseAddress;
    unsigned int unCurrentAddress;
    
   //*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
	//		<< "CTransactOfAggregate : _TransactionDelete!!" << endl;	

    stDataType.iDataKey=iSearchKey;
    pDeleteDataNode=pBtree->ShowSearch(&stDataType);

    if(pDeleteDataNode == NULL)
    {
    	*AggregatorLogM << AggregatorLogM->ErrorCode(4211) 
			<< "_TransactionDelete:ShowSearch Error!!" << endl;
        return -1;
    }
    unCurrentAddress=(unsigned int)pDeleteDataNode;
    unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();

    //Btree index를 초기화 시켜준다.
    memset((char *)pDeleteDataNode,0x00,sizeof(UdrFormat));

	// file update
//	fInOut.seekp( ( ( unCurrentAddress-unBaseAddress )/sizeof( UdrFormat ) )*sizeof( UdrFormat ) );
	fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
	fInOut.write( ( char * )pDeleteDataNode, sizeof( UdrFormat ) );
	fInOut.flush();

    // Btree data를 delete.
    pBtree->DelNode(stDataType);

    pAllocDataIndexC->deleteIndex( (( unCurrentAddress-unBaseAddress )/sizeof( UdrFormat )) );

    return 0;
}
//_TransactionUpdate method. 
// return value : -1 update error
//                 0 update complete
int CTransactOfAggregate::_TransactionUpdate( UdrFormat*	stDataNode )
{
	 _DATATYPE stDataType;
	UdrFormat*   pUpdateDataNode;
	unsigned int unBaseAddress;
	unsigned int unCurrentAddress;
	A_INT_TYPE*	pAtype;
	H_TYPE*		pHtype;
	UDRHEAD_TYPE*   pItemHead;
	
//	*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
	//		<< "CTransactOfAggregate : _TransactionUpdate!!" << endl;	

	pAtype=stDataNode->get_ItemA();
	stDataType.iDataKey=pAtype->get_unMsId();
	pItemHead=stDataNode->get_ItemHead();

	pUpdateDataNode=pBtree->ShowSearch(&stDataType);
	if(pUpdateDataNode == NULL)
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(4211) 
			<< "_TransactionUpdate:ShowSearch Error!!" << endl;
		return -1;
	}
	unCurrentAddress=(unsigned int)pUpdateDataNode;
	unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();


	//data node update
	memcpy( pUpdateDataNode, stDataNode, sizeof( UdrFormat ) );

	// file update
	//fInOut.seekp( ( ( unCurrentAddress-unBaseAddress )/sizeof( UdrFormat ) )*sizeof( UdrFormat ) );
	fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
	fInOut.write( ( char * )pUpdateDataNode, sizeof( UdrFormat ) );
	fInOut.flush();

	// btree update
	/*
	pBtree->DelNode(stDataType);
	stDataType.iDataKey=pAtype->get_unMsId();
	stDataType.pUdrFormat=pUpdateDataNode;
	pBtree->InsertData(stDataType);
	*/
	return 0;
}
// _TransactionInsert method.
int CTransactOfAggregate::_TransactionInsert( UdrFormat*	stDataNode )
{
     _DATATYPE stDataType;
    UdrFormat*	stPtrReturnDataAddress;
    UdrFormat   pFileSelect;
    unsigned int unBaseAddress;
    unsigned int unCurrentAddress;
	A_INT_TYPE*	pAtype;
	unsigned int test;
	
//	*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
	//		<< "CTransactOfAggregate : _TransactionInsert!!" << endl;	
	
    // memory allocate.
    stPtrReturnDataAddress=( UdrFormat * )pAllocDataIndexC->newIndex( (unsigned int)sizeof(UdrFormat) );
    if(stPtrReturnDataAddress == 0 )
    {
    	*AggregatorLogM << AggregatorLogM->ErrorCode(4212) 
			<< "_TransactionInsert:Overflow Error!!" << endl;
        fInOut.close();
		return -1;
    }
    unCurrentAddress=(unsigned int)stPtrReturnDataAddress;
    unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();

    //data insert.
	memcpy( stPtrReturnDataAddress, stDataNode, sizeof( UdrFormat ) );
	pAtype=stDataNode->get_ItemA();
	stPtrReturnDataAddress->setItem(pAtype);


    // file wirte.
    //fInOut.seekp( ( ( unCurrentAddress-unBaseAddress )/sizeof( UdrFormat ) )*sizeof( UdrFormat) );
	fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
    fInOut.write( ( char * )stPtrReturnDataAddress, sizeof( UdrFormat ) );
	fInOut.flush();

    // Btree insert.
    stDataType.iDataKey=pAtype->get_unMsId();
    stDataType.pUdrFormat=stPtrReturnDataAddress;
    pBtree->InsertData(stDataType);

	return 0;
}




