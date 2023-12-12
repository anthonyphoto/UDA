/*********************************************
* This file is defines btree
* based on shared memory
* create a btree node in shared memory
* file name : Btree.hpp                     
* writer : Han Guen Hee                     
* last update : 2000. 9. 29                 
*********************************************/
#ifndef BTREE_HPP
#define BTREE_HPP

#include<iomanip.h>
#include<HandleOfAllocIndex_new.hpp>
#include<UDR.hpp>
#include <SendLogManage.hpp>

enum status{ InsertNotComplete, Success, DuplicateKey, Underflow, NotFound };

// Btree Node의 Pointer갯수( child Node를 가리키는 포인터. )
const unsigned int		MEMBERPOINTER   = 128;
// UDR갯수.( shared memory size를 계산하기 위해 필요.)
const unsigned int      MAXKEYMEMBER	= 100000;
// Btree Node 갯수.
const unsigned int 		MAXBTREEMEMBER	= ( MAXKEYMEMBER/( ( MEMBERPOINTER-1 )/2 ) ) + 1;

// Btree에 들어갈 Key value.
struct _DATATYPE
{
	int			iDataKey;
	UdrFormat	*pUdrFormat;
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
	int 		nFlag;           // 처음인지 다시 시작했는지 구분하기 위한 flag
	_BTREENODE  *pBtreeRoot;     // Btree의 Root Address를 가지고 있는 변수.
}BTREEINFO;

// shared memory를 구성하기 위한 정보.
const int 				SIZE_BTREEINFO		= sizeof( _BTREEINFO );
const int				SIZE_BTREENODE		= sizeof( _BTREENODE );
//const char*	 			BTREE_NAME_SHM		= "../include/Btree.hpp";
//const unsigned char 	BTREE_ID_SHM		= 'B';

//	Log Manager
//SendLogManage*				AggregatorLogM;

class CBtree : virtual public SharedMemory
{
	public:
		CBtree( char* Btree_Name_Shm, char Btree_Id_Shm, int nSystemId,int nServiceId,int nGroupId,int nProcessType,int nPserialNo,int nPreMark );
		~CBtree();
		HandleOfAllocIndex_New*     pHandleBtreeIndex;  
		void 					InsertData( _DATATYPE stDataType );
		void 					Print()const{cout << "Contents:\n"; _print( pBtreeRoot, 0 );}
		void 					DelNode( _DATATYPE stDataType );
		UdrFormat*  			ShowSearch( _DATATYPE *pDataType )const;
		_BTREENODE* 			getBtreeRoot() { return pBtreeRoot; }
		void 					setBtreeRoot( _BTREENODE *pBtreeNode ) { pBtreeRoot=pBtreeNode; } 
	private:
		_BTREENODE*				pBtreeRoot;
		_BTREEINFO      	    stBtreeInfo;
	    struct _BTREENODE*  	 stPtrIndexInfo;
		status 					_Insert( _BTREENODE *pBtreeNode, _DATATYPE stDataType , _DATATYPE &pDataType, _BTREENODE* &apBtreeNode );
		void 					_print( const _BTREENODE *pBtreeNode, int iSpace )const;
		int 					_NodeSearch( int iInsertKey, const _DATATYPE *pDataType , int iCount )const;
		status 					_Delete( _BTREENODE *pBtreeNode, _DATATYPE stDataType );

};

#endif


