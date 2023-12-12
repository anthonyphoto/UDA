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

// Btree Node�� Pointer����( child Node�� ����Ű�� ������. )
const unsigned int		MEMBERPOINTER   = 128;
// UDR����.( shared memory size�� ����ϱ� ���� �ʿ�.)
const unsigned int      MAXKEYMEMBER	= 100000;
// Btree Node ����.
const unsigned int 		MAXBTREEMEMBER	= ( MAXKEYMEMBER/( ( MEMBERPOINTER-1 )/2 ) ) + 1;

// Btree�� �� Key value.
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
// shared memory�� Btree�� root�� ����� �������μ� 
// process�� �׾��ٰ� �ٽ� ����Ǿ�����, �����ϰ� �Ȱ��� �����ϵ��� �Ѵ�.
struct _BTREEINFO
{
	int 		nFlag;           // ó������ �ٽ� �����ߴ��� �����ϱ� ���� flag
	_BTREENODE  *pBtreeRoot;     // Btree�� Root Address�� ������ �ִ� ����.
}BTREEINFO;

// shared memory�� �����ϱ� ���� ����.
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

