/******************************************************************
* �� �� ��     :  TransactOfAggregate.hpp						  *
* ��   ��                                                          *
* : This file is defines transaction of aggregator		   		  *
* �� �� ��     : Han Guen Hee                    				  *
* first data  : 2000. 09. 29       	     						  *
* last updata : 2000. 12. 21        		 					  *
* �� �� ȭ ��  : 					               					  *
* program history                        						  *
*                         	        	 				 		  *
* bug fix                        	     						  *
*                         	        	 						  *
******************************************************************/
#ifndef TRANSACTOFAGGREGATE_HPP
#define TRANSACTOFAGGREGATE_HPP

#include <fstream.h>
#include <Btree.hpp>
#include <stdlib.h>
#include <sys/errno.h>

#define 	DEBUG	0
const   int                 SIZE_UDRFORMAT      = sizeof( UdrFormat );
		char 	PATH_NAME_AGGREGATOR_MDB[80] 	= "/SVC#/MB/Agg#.mb";

class CTransactOfAggregate
{
	public:
		CTransactOfAggregate();
		CTransactOfAggregate(char* Data_Name_Shm, char Data_Id_Shm, int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark);
		~CTransactOfAggregate();
		void	 					selectshm( void );
		int 						deleteshm( UdrFormat*	pUdrFormat );
		int 						updateshm( UdrFormat*	pUdrFormat );
		int 						insertshm( UdrFormat*	pUdrFormat );
		CBtree*						get_BtreePointer( void ) { return pBtree; }
		UdrFormat*					getUdrPointer( void ) { return stPtrDataIndexInfo; }

	private:
		// index shared memory ��ü�� ����Ű�� ���� ������ 
		HandleOfAllocIndex_New*		pAllocDataIndexC;
		// btree shared memory ��ü�� ����Ű�� ���� ������ 
		CBtree*						pBtree;
		// data shared memory ��ü�� ����Ű�� ���� ������  
		SharedMemory*				shmData;
	    // file descriptor
		fstream 					fInOut;
		// data shared memory�� base address�� ����Ű�� ���� ������. 
		UdrFormat*  				stPtrDataIndexInfo;
		int 		 				_TransactionInsert( UdrFormat*	stDataNode );
		int		 					_TransactionUpdate( UdrFormat*	stDataNode );
		int		 					_TransactionDelete( unsigned int 		iDeleteKey );
		UdrFormat*					_TransactionSelect( unsigned int 		iSelectKey );

};
//===============================================================
// Constructor Method											=
//  : file descriptor open.			              				=
//	  Btree instance ����										=
//	  Data shared memory instance create						=
//	  Data index shared memory instance create.					=
// argument : ���μ��� ���󱸺� 6����, Key Path, Key Id				=
// return value : None											=
//																=
//===============================================================
CTransactOfAggregate::CTransactOfAggregate(char* Data_Name_Shm, char Data_Id_Shm, int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark)
{
	char 			BtreeId;
	unsigned int 	unAllocAddress;
	unsigned int 	unBtreeShmKey;
	unsigned int 	unBtreeAllocKey;
	
	// old=============>shmData=new ShmMemory( Data_Name_Shm, Data_Id_Shm, SIZE_UDRFORMAT*MAXKEYMEMBER, 0660|IPC_CREAT );
	shmData=new SharedMemory( nSystemId,nServiceId,nGroupId,nProcessType,nPserialNo,nPreMark+3, SIZE_UDRFORMAT*MAXKEYMEMBER );
	
	Data_Id_Shm++;
	
	stPtrDataIndexInfo=( UdrFormat * )shmData->get_ShmPtr();
	// old========> pAllocDataIndexC=new HandleOfAllocIndex( Data_Id_Shm, MAXKEYMEMBER, ( unsigned int )stPtrDataIndexInfo );
	pAllocDataIndexC=new HandleOfAllocIndex_New( nSystemId,nServiceId,nGroupId,nProcessType,nPserialNo,nPreMark+2,MAXKEYMEMBER, ( unsigned int )stPtrDataIndexInfo );
	unAllocAddress = pAllocDataIndexC->get_Shm_Key();
	Data_Id_Shm++;
	pBtree=new CBtree( Data_Name_Shm, Data_Id_Shm,nSystemId,nServiceId,nGroupId,nProcessType,nPserialNo,nPreMark );
	unBtreeShmKey = pBtree->getIndexKey();
	unBtreeAllocKey = pBtree->getAllocKey();

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
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "shmData(DATA SHAREDMEM): " << shmData << endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "stPtrDataIndexInfo: " << stPtrDataIndexInfo << endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "pAllocDataIndexC(DATA Alloc):" << pAllocDataIndexC << endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "Btree Pointer: " << pBtree << endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "unAllocAddress= " << unAllocAddress << endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "unBtreeShmKey= " << unBtreeShmKey << endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "unBtreeAllocKey= " << unBtreeAllocKey << endl;
	}
}
// Constructor
CTransactOfAggregate::CTransactOfAggregate()
{
}
// destructor
CTransactOfAggregate::~CTransactOfAggregate()
{ 
	delete shmData;
	delete pBtree;
	delete pAllocDataIndexC;
	fInOut.close(); 
}
//===============================================================
// updateshm Method					   							=
//  : update�ϱ����� �����͸� �Է¹޾� �ش� �޼ҵ带 update.			=
//	  _TransactionUpdate�޼ҵ带 ȣ���Ͽ� ó��.						=
// argument     :  pUdrFormat->Update�� UDR Format				=
// return value : 0-> success									=
//                -1-> fail										=
//																=
//===============================================================
int CTransactOfAggregate::updateshm( UdrFormat *pUdrFormat )
{
	if( _TransactionUpdate( pUdrFormat ) )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "updateshm:_TransactionUpdate Error!!" << endl;	
		return -1;
	}
	return 0;
}
//===============================================================
// insertshm Method					   							=
//  : data�� �Է¹޾�, _TransactionInsert method�� ȣ���ؼ� insert =
// argument     :  pUdrFormat->insert�� UDR Format				=
// return value : 0-> success									=
//                -1-> fail										=
//																=
//===============================================================
int CTransactOfAggregate::insertshm( UdrFormat*	pUdrFormat )
{
	if( _TransactionInsert( pUdrFormat ) )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "insertshm:_TransactionInsert Error!!" << endl;	
		return -1;
	}
	return 0;
}
//===============================================================
// deleteshm Method					   							=
//  : delete�ϰ��� �ϴ� Ű�� �Է¹޾�, 								=
//	  _TransactionDelete method�� ȣ���Ͽ� delete.				=
// argument     :  pUdrFormat->delete�� UDR Format				=
// return value : 0-> success									=
//                -1-> fail										=
//																=
//===============================================================
int CTransactOfAggregate::deleteshm( UdrFormat*	pUdrFormat )
{
	A_INT_TYPE*	pAtype;
	unsigned int	nTempKey;

	pAtype=pUdrFormat->get_ItemA();
	nTempKey=pAtype->get_unMsId();
	if( _TransactionDelete( nTempKey ) )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "deleteshm:_TransactionDelete Error!!" << endl;	
		return -1;
	}
	return 0;
}
//===============================================================
// selectshm Method					   							=
//  : ó��(update)�ϰ��� �ϴ� ���� �Է¹޾�,							=
//	 _TransactionSelect method�� ȣ���Ͽ�,						=
//	 return vlaue( _DISKNODE *)�� ������ ���� ó��.				=
// argument     :  None											=
// return value : None											=
//																=
//===============================================================
void CTransactOfAggregate::selectshm( )
{
	int iSelectKey;
	UdrFormat*	pSelectDataNode;
	UdrFormat pFileSelect;
	unsigned int unBaseAddress;
	unsigned int unCurrentAddress;
	A_INT_TYPE*		pAtype;

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
	// file���� ���� �����͸� setting�ϱ����ؼ� 
	// �ش� �ּ��� index�� ����ϱ����� ����.
	unCurrentAddress=(unsigned int)pSelectDataNode;
	unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();

	// file read 
	// �ش� ��ġ�� �����͸� �Ű��ش�.
	fInOut.seekg( ( ( unCurrentAddress-unBaseAddress )/sizeof( UdrFormat ) )*sizeof( UdrFormat ) );
	fInOut.read( ( char * )&pFileSelect, sizeof( UdrFormat ) );
	pAtype=pFileSelect.get_ItemA();
}
//===============================================================
// _TransactionSelect Method									=
//  : ������key�� �Է¹޾�, ã�� �ּҸ� ��ȯ�Ͽ� �ش�.					=
// argument     :  iSearchKey->ã������ Min��ȣ 					=
// return value :UdrFormat *(ã�� key�� ���� address.)			=
//                Null-> Not Found								=
//																=
//===============================================================
UdrFormat*	CTransactOfAggregate::_TransactionSelect( unsigned int iSearchKey )
{
    _DATATYPE stDataType;
    UdrFormat*	pTempUdr;

    // Btree�� key(struct _DATATYPE)������ �־��ش�.
    stDataType.iDataKey=iSearchKey;
    pTempUdr=pBtree->ShowSearch(&stDataType);
    return pTempUdr;
}
//===============================================================
// _TransactionDelete Method									=
//  : ������key�� �Է¹޾�,�ش� MDB�� backup file�� ����.			=
// argument     :  iSearchKey->������  Min��ȣ 					=
// return value : 0-> success									=
//                -1-> fail     								=
//																=
//===============================================================
int CTransactOfAggregate::_TransactionDelete( unsigned int iSearchKey )
{
      int iIndex;
     _DATATYPE stDataType;
    UdrFormat*	pDeleteDataNode;
    unsigned int unBaseAddress;
    unsigned int unCurrentAddress;
    
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
	
    //Btree index�� �ʱ�ȭ �����ش�.
    memset((char *)pDeleteDataNode,0x00,sizeof(UdrFormat));
	
	// file check 
	// 2001. 02. 16
	if( !fInOut )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "_TransactionDelete:File ReOpen!!!!!!!!" << endl;
		fInOut.open( PATH_NAME_AGGREGATOR_MDB, ios::in | ios::out | ios::binary );
	}
	// file update
//	fInOut.seekp( ( ( unCurrentAddress-unBaseAddress )/sizeof( UdrFormat ) )*sizeof( UdrFormat ) );
	fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
	fInOut.write( ( char * )pDeleteDataNode, sizeof( UdrFormat ) );
	fInOut.flush();

    // Btree data�� delete.
    pBtree->DelNode(stDataType);
	
    pAllocDataIndexC->deleteIndex( (( unCurrentAddress-unBaseAddress )/sizeof( UdrFormat )) );
    return 0;
}
//===============================================================
// _TransactionUpdate Method									=
//  : �Է¹��� UDR�� MDB�� backup file�� update        			=
// argument     :  stDataNode->update�� UDR Format				=
// return value : 0-> success									=
//                -1-> fail     								=
//																=
//===============================================================
int CTransactOfAggregate::_TransactionUpdate( UdrFormat*	stDataNode )
{
	 _DATATYPE stDataType;
	UdrFormat*   pUpdateDataNode;
	unsigned int unBaseAddress;
	unsigned int unCurrentAddress;
	A_INT_TYPE*	pAtype;
	H_TYPE*		pHtype;
	UDRHEAD_TYPE*   pItemHead;
	
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
	
	// file check 
	// 2001. 02. 16
	if( !fInOut )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "_TransactionUpdate:File ReOpen!!!!!!!!" << endl;
		fInOut.open( PATH_NAME_AGGREGATOR_MDB, ios::in | ios::out | ios::binary );
	}
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
//===============================================================
// _TransactionInsert Method									=
//  : �Է¹��� UDR�� MDB�� backup file�� insert        			=
// argument     :  stDataNode->insert�� UDR Format				=
// return value : 0-> success									=
//                -1-> fail     								=
//																=
//===============================================================
int CTransactOfAggregate::_TransactionInsert( UdrFormat*	stDataNode )
{
     _DATATYPE stDataType;
    UdrFormat*	stPtrReturnDataAddress;
    UdrFormat   pFileSelect;
    unsigned int unBaseAddress;
    unsigned int unCurrentAddress;
	A_INT_TYPE*	pAtype;
	unsigned int test;
	
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

	// file check 
	// 2001. 02. 16
	if( !fInOut )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "_TransactionInsert:File ReOpen!!!!!!!!" << endl;
		fInOut.open( PATH_NAME_AGGREGATOR_MDB, ios::in | ios::out | ios::binary );
	}
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

#endif


