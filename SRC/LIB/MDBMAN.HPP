/******************************************************************
* 파 일 명     :  MdbMan.hpp						  *
* 기   능                                                          *
* : This file is defines transaction of aggregator		   		  *
* 작 성 자     : Han Guen Hee                    				  *
* first data  : 2000. 09. 29       	     						  *
* last updata : 2000. 12. 21        		 					  *
* 관 련 화 일  : 					               					  *
* program history                        						  *
*                         	        	 				 		  *
* bug fix                        	     						  *
*                         	        	 						  *
******************************************************************/
#ifndef MDBMAN_HPP
#define MDBMAN_HPP

#include <fstream.h>
#include <Tpl_Btree.hpp>
#include <stdlib.h>
#include <CommCfgFile.hpp>
#include <SendLogManage.hpp>

extern SendLogManage*                   LogM;
//CommCfgFile*					pCommCfgFile;	// config file을 읽어오기 위한 포인터.
		
#define 	M_DEBUG	0

template <class keyType, class dataType>
class MdbMan
{
	public:
		MdbMan( char* DataFile, char DataId, char* BackupFile, char* CfgFile, int nMemberPoint, int nMaxKeyMember );
		~MdbMan();
		dataType* 					selectMdb( keyType cKeyType );
		int 						deleteMdb( keyType cKeyType );
		int 						updateMdb( keyType UpdateKey, dataType* pUpdateData );
		int 						insertMdb( keyType* pInputKey, dataType* pInputData );
		void						printBtree( void );
		CBtree<keyType, dataType>*	get_BtreePointer( void ) { return pBtree; }
		
		//dataType*					getUdrPointer( void ) { return stPtrDataIndexInfo; }

	private:
		// index shared memory 객체를 가리키기 위한 포인터 
		HandleOfAllocIndex_New*		pAllocDataIndexC;
		// btree shared memory 객체를 가리키기 위한 포인터 
		CBtree<keyType, dataType>*	pBtree;
		// data shared memory 객체를 가리키기 위한 포인터  
		ShmMemory*					shmData;
	    // file descriptor
		fstream 					fInOut;
		// data shared memory의 base address를 가리키기 위한 포인터. 
		dataType*  					stPtrDataIndexInfo;
		

};
//===============================================================
// Constructor Method											=
//  : file descriptor open.			              				=
//	  Btree instance 생성										=
//	  Data shared memory instance create						=
//	  Data index shared memory instance create.					=
// argument : 프로세스 형상구분 6가지, Key Path, Key Id			=
// return value : None											=
//																=
//===============================================================
template <class keyType, class dataType>
MdbMan<keyType, dataType>::MdbMan( char* DataFile, char DataId, char* BackupFile, char* CfgFile, int nMemberPoint, int nMaxKeyMember )
{
	char 			BtreeId;
	unsigned int 	unAllocAddress;
	unsigned int 	unBtreeShmKey;
	unsigned int 	unBtreeAllocKey;
	
//	pCommCfgFile = new CommCfgFile( CfgFile );
	// Config file로부터 정보를 읽어들인다.
//	pCommCfgFile->set_MemberPoint( nMemberPoint );
//	pCommCfgFile->set_MaxKeyMember( nMaxKeyMember );
//	pCommCfgFile->write_CommCfgFile();

	if( M_DEBUG )
	{
//		*LogM << LogM->ErrorCode(0) << "[MdbMan] cfgPath = [" << pCommCfgFile->get_cfgPath() << "]" << endl;
//		*LogM << LogM->ErrorCode(0) << "[MdbMan] cfgFileName = [" << pCommCfgFile->get_cfgFileName() << "]" << endl;
//		*LogM << LogM->ErrorCode(0) << "[MdbMan] cfgID = [" << pCommCfgFile->get_cfgId() << "]" << endl;
	}
	// 공통config file을 읽어와야 한다.-> 예정.
	shmData=new ShmMemory( DataFile, DataId, sizeof(dataType)*nMaxKeyMember, 0660|IPC_CREAT );
	DataId++;
	
	stPtrDataIndexInfo=( dataType * )shmData->ShmPtr();
	pAllocDataIndexC=new HandleOfAllocIndex_New( (const char*)DataFile, ( const unsigned char )DataId, nMaxKeyMember, ( unsigned int )stPtrDataIndexInfo );
	unAllocAddress = pAllocDataIndexC->get_Shm_Key();
	DataId++;
	pBtree=new CBtree<keyType, dataType>( DataFile, DataId );
	unBtreeShmKey = pBtree->getIndexKey();
	unBtreeAllocKey = pBtree->getAllocKey();

	if( M_DEBUG )
	{
//		*LogM << LogM->ErrorCode(0) << "[MdbMan] backPath = [" << pCommCfgFile->get_backPath() << "]" << endl;
//		*LogM << LogM->ErrorCode(0) << "[MdbMan] backupFileName = [" << pCommCfgFile->get_backupFileName() << "]" << endl;
	}
	// initialize
	fInOut.open( BackupFile, ios::in | ios::out | ios::binary );
	if( !fInOut )
	{
		// 에러처리후 다시 오픈하는 로직을 추가해야 함.@@@@@@@@@@@@@@@@@@@@@@@@@@
	//	*LogM << LogM->ErrorCode(0) << "[MdbMan] [" << BackupFile << "]" << endl;
		delete shmData;
		shmData = 0;
		delete pAllocDataIndexC;
		pAllocDataIndexC = 0;
		delete pBtree;
		pBtree = 0;
	//	delete pCommCfgFile;
	//	pCommCfgFile = 0;
		exit( 1 );
	}
	if( M_DEBUG )
	{
		*LogM << LogM->ErrorCode(0) << "[MdbMan] shmData(DATA SHAREDMEM) = [" << (unsigned int)shmData << "]" << endl;
		*LogM << LogM->ErrorCode(0) << "[MdbMan] stPtrDataIndexInfo = [" << (unsigned int)stPtrDataIndexInfo << "]" << endl;
		*LogM << LogM->ErrorCode(0) << "[MdbMan] pAllocDataIndexC(DATA Alloc) = [" << (unsigned int)pAllocDataIndexC << "]" << endl;
		*LogM << LogM->ErrorCode(0) << "[MdbMan] Btree Pointer = [" << (unsigned int)pBtree << "]" << endl;
		*LogM << LogM->ErrorCode(0) << "[MdbMan] unAllocAddress = [" << (unsigned int)unAllocAddress << "]" << endl;
		*LogM << LogM->ErrorCode(0) << "[MdbMan] unBtreeShmKey = [" << (unsigned int)unBtreeShmKey << "]" << endl;
		*LogM << LogM->ErrorCode(0) << "[MdbMan] unBtreeAllocKey = [" << (unsigned int)unBtreeAllocKey << "]" << endl;
	}
}
// destructor
template <class keyType, class dataType>
MdbMan<keyType, dataType>::~MdbMan()
{ 
	delete shmData;
	shmData = 0;
	delete pBtree;
	pBtree = 0;
	delete pAllocDataIndexC;
	pAllocDataIndexC = 0;
//	delete pCommCfgFile;
//	pCommCfgFile = 0;
	fInOut.close(); 
}
//===============================================================
// insertMdb Method					   							
//  : data를 입력받아,  insert 
// argument     :  pInputKey->Btree에 insert할 keyType	
//				   pInputData->insert DataType			
// return value : 0-> success									
//                -1-> fail										
//																
//===============================================================
template <class keyType, class dataType>
int MdbMan<keyType, dataType>::insertMdb( keyType* pInputKey, dataType* pInputData )
{
	 _DATATYPE<keyType, dataType> 	stDataType;
    dataType*						pInsertDataAddress;
    unsigned int 					unBaseAddress;
    unsigned int 					unCurrentAddress;
    int								uRetryCount=0;
	
	if( M_DEBUG )
	{
		*LogM << LogM->ErrorCode(0) << "[MdbMan] insertMdb " << endl;
	}
    // memory allocate.
    // 메모리가 가득 찼을때, 재시도를 한다. 2001. 05. 16
    while( 1 )
    {
    	pInsertDataAddress=( dataType * )pAllocDataIndexC->newIndex( (unsigned int)sizeof(dataType) );
    	
    	if( pInsertDataAddress != 0 )
    		break;
    	
	    *LogM << LogM->ErrorCode(0) <<  "[MdbMan][insertMdb] Memory Full, So wait some seconds and Retry " << endl;
	    sleep(2);
	    if (++uRetryCount > 150)
	    {
	    	*LogM << LogM->ErrorCode(0) <<  "[MdbMan][insertMdb] Warning Memory Full is over 5 Min. So exit!!!!! " << endl;
	    	exit( 1 );
		}
    }
    memset( pInsertDataAddress, NULL, sizeof(dataType) );
    
    unCurrentAddress=(unsigned int)pInsertDataAddress;
    unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();

    //Memory data insert.
	memcpy( pInsertDataAddress, pInputData, sizeof( dataType ) );

    // file wirte.
    //fInOut.seekp( ( ( unCurrentAddress-unBaseAddress )/sizeof( dataType ) )*sizeof( dataType) );
	fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
    fInOut.write( ( char * )pInsertDataAddress, sizeof( dataType ) );
	fInOut.flush();

    // Btree insert.
    stDataType.set_keyType( pInputKey );
    stDataType.set_dataType( ( dataType * )pInsertDataAddress );
    pBtree->InsertData(stDataType);

	return 0;
}
//===============================================================
// deleteMdb Method					   							
//  : delete하고자 하는 키를 입력받아 delete							
// argument     :  pdataType->delete할 Data Format				
// return value : 0-> success									
//                -1-> fail										
//																
//===============================================================
template <class keyType, class dataType>
int MdbMan<keyType, dataType>::deleteMdb( keyType cKeyType )
{
	int 							iIndex;
     _DATATYPE<keyType, dataType>	stDataType;
    dataType*						pDeleteDataAddress;
    unsigned int 					unBaseAddress;
    unsigned int 					unCurrentAddress;
    
    if( M_DEBUG )
	{
		*LogM << LogM->ErrorCode(0) << "[MdbMan] deleteMdb " << endl;
	}
    pDeleteDataAddress=pBtree->selectNode( cKeyType );
    if(pDeleteDataAddress == NULL)
    {
    	*LogM << LogM->ErrorCode(0) <<  "[MdbMan][deleteMdb] selectNode Error!!" << endl;
        return -1;
    }
	stDataType.set_keyType( &cKeyType );
	stDataType.set_dataType( ( dataType * )pDeleteDataAddress );
	
    unCurrentAddress=(unsigned int)pDeleteDataAddress;
    unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();
	
    //data를 초기화 시켜준다.
    memset((char *)pDeleteDataAddress,0x00,sizeof(dataType));
	
	// file update
//	fInOut.seekp( ( ( unCurrentAddress-unBaseAddress )/sizeof( UdrFormat ) )*sizeof( UdrFormat ) );
	fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
	fInOut.write( ( char * )pDeleteDataAddress, sizeof( dataType ) );
	fInOut.flush();

    // Btree data를 delete.
    pBtree->DelNode( stDataType );
	
    pAllocDataIndexC->deleteIndex( (( unCurrentAddress-unBaseAddress )/sizeof( dataType )) );
    return 0;
}

//===============================================================
// selectshm Method					   							
//  : search할 key를 입력받아, 해당 data address를 반환			
// argument     :  cKeyType -> search key						
// return value : dataType*											
//																
//===============================================================
template <class keyType, class dataType>
dataType* MdbMan<keyType, dataType>::selectMdb( keyType cKeyType )
{
	return ( dataType* )pBtree->selectNode( cKeyType );
}
//===============================================================
// updateMdb Method					   							
//  : update하기위한 데이터를 입력받아 해당 메소드를 update.			
// argument     :  pUdrFormat->Update할 UDR Format				
// return value : 0-> success									
//                -1-> fail										
//																
//===============================================================
template <class keyType, class dataType>
int MdbMan<keyType, dataType>::updateMdb( keyType UpdateKey, dataType* pUpdateData )
{
	_DATATYPE<keyType, dataType> 	stDataType;
    dataType*						pUpdateDataAddress;
    unsigned int 					unBaseAddress;
    unsigned int 					unCurrentAddress;
	
	 if( M_DEBUG )
	{
		*LogM << LogM->ErrorCode(0) << "[MdbMan] updateMdb "<< endl;
	}
	pUpdateDataAddress=pBtree->selectNode( UpdateKey );
    if(pUpdateDataAddress == NULL)
    {
    	*LogM << LogM->ErrorCode(0) <<  "[MdbMan][updateMdb] selectNode Error!!" << endl;
        return -1;
    }
	stDataType.set_keyType( &UpdateKey );
	stDataType.set_dataType( ( dataType * )pUpdateDataAddress );
 
	unCurrentAddress=(unsigned int)pUpdateDataAddress;
	unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();

	//Memory data update
	memcpy( pUpdateDataAddress, pUpdateData, sizeof( dataType ) );
	
	// file update
	//fInOut.seekp( ( ( unCurrentAddress-unBaseAddress )/sizeof( UdrFormat ) )*sizeof( UdrFormat ) );
	fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
	fInOut.write( ( char * )pUpdateDataAddress, sizeof( dataType ) );
	fInOut.flush();

	// btree update는 필요없다(?)
	return 0;
}
template <class keyType, class dataType>
void MdbMan<keyType, dataType>::printBtree( void )
{
	pBtree->Print();
}
#endif



