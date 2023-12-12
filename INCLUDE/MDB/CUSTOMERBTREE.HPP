/*********************************************
* This file is defines Customer btree
* based on shared memory
* create a Customer btree node in shared memory
* file name : CustomerBtree.hpp                     
* writer : Han Guen Hee                     
* last update : 2000. 11. 02                 
*********************************************/
#ifndef CUSTOMERBTREE_HPP
#define CUSTOMERBTREE_HPP

#include<iomanip.h>
#include<HandleOfAllocIndex_new.hpp>
#include<scpformat.hpp>
#include <SendLogManage.hpp>

enum status{ InsertNotComplete, Success, DuplicateKey, Underflow, NotFound };

// Btree Node의 Pointer갯수( child Node를 가리키는 포인터. )
const unsigned int		MEMBERPOINTER   = 128;
// UDR갯수.( shared memory size를 계산하기 위해 필요.)
const unsigned int      MAXKEYMEMBER	= 1000000;
// Btree Node 갯수.
const unsigned int 		MAXBTREEMEMBER	= ( MAXKEYMEMBER/( ( MEMBERPOINTER-1 )/2 ) ) + 1;

// Btree에 들어갈 Key value.
struct _DATATYPE
{
	int					iDataKey;
	CustFormatMdb		*pCustFormatMdb;
};
// Btree Node
struct _BTREENODE
{
	int 		iIndex;        // find node_index with return address
	int 		iNumberKey;
	_DATATYPE	kIndexKey[ MEMBERPOINTER-1 ];
	_BTREENODE	*pBTREENODE[ MEMBERPOINTER ];
}BTREENODE;
// shared memory에 Btree의 root를 기억해 놓음으로서 
// process가 죽었다가 다시 실행되었을때, 이전하고 똑같이 동작하도록 한다.
struct _BTREEINFO
{
	int 		nFlag;
	_BTREENODE  *pBtreeRoot;
}BTREEINFO;

// shared memory를 구성하기 위한 정보.
const int 				SIZE_BTREEINFO		= sizeof( _BTREEINFO );
const int				SIZE_BTREENODE		= sizeof( _BTREENODE );
//const char*	 			BTREE_NAME_SHM		= "../include/CustomerBtree.hpp";
//const unsigned char 	BTREE_ID_SHM		= 'B';

//	Log Manager
//SendLogManage*				CustomerManLogM;

class CCustomerBtree : virtual public SharedMemory
{
	public:
		CCustomerBtree( char* Btree_Name_Shm, char Btree_Id_Shm, int nSystemId,int nServiceId,int nGroupId,int nProcessType,int nPserialNo,int nPreMark );
		~CCustomerBtree();
		HandleOfAllocIndex_New*     pHandleBtreeIndex;  
		void 						InsertData( _DATATYPE stDataType );
		void 						Print()const{cout << "Contents:\n"; _print( pBtreeRoot, 0 );}
		void 						DelNode( _DATATYPE stDataType );
		CustFormatMdb*  			ShowSearch( _DATATYPE *pDataType )const;
		_BTREENODE* 				getBtreeRoot() { return pBtreeRoot; }
		void 						setBtreeRoot( _BTREENODE *pBtreeNode ) { pBtreeRoot=pBtreeNode; } 
	private:
		_BTREENODE*					pBtreeRoot;
		_BTREEINFO      		    stBtreeInfo;
	    struct _BTREENODE*  		 stPtrIndexInfo;
	    //	Log Manager
		//SendLogManage*				clsLogM;
		
		status 						_Insert( _BTREENODE *pBtreeNode, _DATATYPE stDataType , _DATATYPE &pDataType, _BTREENODE* &apBtreeNode );
		void 						_print( const _BTREENODE *pBtreeNode, int iSpace )const;
		int 						_NodeSearch( int iInsertKey, const _DATATYPE *pDataType , int iCount )const;
		status 						_Delete( _BTREENODE *pBtreeNode, _DATATYPE stDataType );

};

#endif

