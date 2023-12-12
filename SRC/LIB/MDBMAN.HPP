/******************************************************************
* �� �� ��     :  MdbMan.hpp						  *
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
#ifndef MDBMAN_HPP
#define MDBMAN_HPP

#include <fstream.h>
#include <Tpl_Btree.hpp>
#include <stdlib.h>
#include <CommCfgFile.hpp>
#include <SendLogManage.hpp>

extern SendLogManage*                   LogM;
//CommCfgFile*					pCommCfgFile;	// config file�� �о���� ���� ������.
		
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
		// index shared memory ��ü�� ����Ű�� ���� ������ 
		HandleOfAllocIndex_New*		pAllocDataIndexC;
		// btree shared memory ��ü�� ����Ű�� ���� ������ 
		CBtree<keyType, dataType>*	pBtree;
		// data shared memory ��ü�� ����Ű�� ���� ������  
		ShmMemory*					shmData;
	    // file descriptor
		fstream 					fInOut;
		// data shared memory�� base address�� ����Ű�� ���� ������. 
		dataType*  					stPtrDataIndexInfo;
		

};
//===============================================================
// Constructor Method											=
//  : file descriptor open.			              				=
//	  Btree instance ����										=
//	  Data shared memory instance create						=
//	  Data index shared memory instance create.					=
// argument : ���μ��� ���󱸺� 6����, Key Path, Key Id			=
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
	// Config file�κ��� ������ �о���δ�.
//	pCommCfgFile->set_MemberPoint( nMemberPoint );
//	pCommCfgFile->set_MaxKeyMember( nMaxKeyMember );
//	pCommCfgFile->write_CommCfgFile();

	if( M_DEBUG )
	{
//		*LogM << LogM->ErrorCode(0) << "[MdbMan] cfgPath = [" << pCommCfgFile->get_cfgPath() << "]" << endl;
//		*LogM << LogM->ErrorCode(0) << "[MdbMan] cfgFileName = [" << pCommCfgFile->get_cfgFileName() << "]" << endl;
//		*LogM << LogM->ErrorCode(0) << "[MdbMan] cfgID = [" << pCommCfgFile->get_cfgId() << "]" << endl;
	}
	// ����config file�� �о�;� �Ѵ�.-> ����.
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
		// ����ó���� �ٽ� �����ϴ� ������ �߰��ؾ� ��.@@@@@@@@@@@@@@@@@@@@@@@@@@
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
//  : data�� �Է¹޾�,  insert 
// argument     :  pInputKey->Btree�� insert�� keyType	
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
    // �޸𸮰� ���� á����, ��õ��� �Ѵ�. 2001. 05. 16
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
//  : delete�ϰ��� �ϴ� Ű�� �Է¹޾� delete							
// argument     :  pdataType->delete�� Data Format				
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
	
    //data�� �ʱ�ȭ �����ش�.
    memset((char *)pDeleteDataAddress,0x00,sizeof(dataType));
	
	// file update
//	fInOut.seekp( ( ( unCurrentAddress-unBaseAddress )/sizeof( UdrFormat ) )*sizeof( UdrFormat ) );
	fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
	fInOut.write( ( char * )pDeleteDataAddress, sizeof( dataType ) );
	fInOut.flush();

    // Btree data�� delete.
    pBtree->DelNode( stDataType );
	
    pAllocDataIndexC->deleteIndex( (( unCurrentAddress-unBaseAddress )/sizeof( dataType )) );
    return 0;
}

//===============================================================
// selectshm Method					   							
//  : search�� key�� �Է¹޾�, �ش� data address�� ��ȯ			
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
//  : update�ϱ����� �����͸� �Է¹޾� �ش� �޼ҵ带 update.			
// argument     :  pUdrFormat->Update�� UDR Format				
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

	// btree update�� �ʿ����(?)
	return 0;
}
template <class keyType, class dataType>
void MdbMan<keyType, dataType>::printBtree( void )
{
	pBtree->Print();
}
#endif


