/******************************************************************
* 파 일 명     :  Btree.hpp		     							  *
* 기   능                                                          *
* : This file is defines btree based on shared memory   		  *
*   create a Customer btree node in shared memory            	  *
* 작 성 자     : Han Guen Hee                    				  *
* first data  : 2000. 09. 29       	     						  *
* last updata : 2000. 12. 21        		 					  *
* 관 련 화 일  : 					               					  *
* program history                        						  *
*                         	        	 				 		  *
* bug fix                        	     						  *
*                         	        	 						  *
******************************************************************/
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
//const char*	 		BTREE_NAME_SHM		= "../include/Btree.hpp";
//const unsigned char 	BTREE_ID_SHM		= 'B';

//	Log Manager
SendLogManage*				AggregatorLogM;

class CBtree : virtual public SharedMemory
{
	public:
		CBtree( char* Btree_Name_Shm, char Btree_Id_Shm, int nSystemId,int nServiceId,int nGroupId,int nProcessType,int nPserialNo,int nPreMark );
		~CBtree();
		void 					InsertData( _DATATYPE stDataType );
		void 					Print( void )const{cout << "Contents:\n"; _print( pBtreeRoot, 0 );}
		void 					DelNode( _DATATYPE stDataType );
		UdrFormat*  			ShowSearch( _DATATYPE *pDataType );
		_BTREENODE* 			getBtreeRoot( void ) { memcpy( &stBtreeInfo, stPtrIndexInfo, SIZE_BTREEINFO ); return stBtreeInfo.pBtreeRoot; }
		void 					setBtreeRoot( _BTREENODE *pBtreeNode ) { pBtreeRoot=pBtreeNode; }
		unsigned int			getIndexKey( void )  { return (unsigned int ) get_ShmPtr(); }
		unsigned int			getAllocKey( void )  { return (unsigned int ) pHandleBtreeIndex->get_Shm_Key(); }
		void					set_BtreeFlag( int nValue ) { stBtreeInfo.nFlag=nValue; memcpy( stPtrIndexInfo, &stBtreeInfo, SIZE_BTREEINFO );}
		int						get_BtreeFlag( void ) { memcpy( &stBtreeInfo, stPtrIndexInfo, SIZE_BTREEINFO ); return stBtreeInfo.nFlag; }
	private:
		_BTREENODE*				pBtreeRoot;
		_BTREEINFO      	    stBtreeInfo;
	    struct _BTREENODE*  	stPtrIndexInfo;
	    HandleOfAllocIndex_New* pHandleBtreeIndex;  
		status 					_Insert( _BTREENODE *pBtreeNode, _DATATYPE stDataType , _DATATYPE &pDataType, _BTREENODE* &apBtreeNode );
		void 					_print( const _BTREENODE *pBtreeNode, int iSpace )const;
		int 					_NodeSearch( int iInsertKey, const _DATATYPE *pDataType , int iCount )const;
		status 					_Delete( _BTREENODE *pBtreeNode, _DATATYPE stDataType );

};
//===============================================================
// Constructor Method											=
//  : Btree shared memory create.               				=
//	  Btree Index shared memory create.							=
// argument : 프로세스 형상구분 6가지, Key Path, Key Id				=
// return value : None											=
//																=
//===============================================================
CBtree::CBtree( char* Btree_Name_Shm, char Btree_Id_Shm,int nSystemId,int nServiceId,int nGroupId,int nProcessType,int nPserialNo,int nPreMark )
: SharedMemory( nSystemId,nServiceId,nGroupId,nProcessType,nPserialNo,nPreMark+1, ( SIZE_BTREENODE*MAXBTREEMEMBER )+SIZE_BTREEINFO )
{
	if( 0 )
	{
		cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
		cout << "Btree_Name_Shm=" << Btree_Name_Shm << endl;
		cout << "Btree_Id_Shm=" << Btree_Id_Shm << endl;
		cout << "Btree Data Size=" << ( SIZE_BTREENODE*MAXBTREEMEMBER )+SIZE_BTREEINFO << endl;
		cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
	}
	stPtrIndexInfo=( struct _BTREENODE* )get_ShmPtr();
	memcpy( &stBtreeInfo, stPtrIndexInfo, SIZE_BTREEINFO );
	// 처음으로 실행되는 경우.
	if( !stBtreeInfo.nFlag )
	{
		stBtreeInfo.nFlag=1;
		memcpy( stPtrIndexInfo, &stBtreeInfo, SIZE_BTREEINFO );
		pBtreeRoot=NULL;
	}
	// process가 재실행될때
	else
	{
		// Btree의 Root정보를 가져와서 Root에 넣어준다.
		pBtreeRoot=stBtreeInfo.pBtreeRoot;
	}
	++Btree_Id_Shm;
	// old==========>pHandleBtreeIndex = new HandleOfAllocIndex( ( const unsigned char )Btree_Id_Shm, MAXBTREEMEMBER, ( unsigned int )stPtrIndexInfo+SIZE_BTREEINFO );
	pHandleBtreeIndex = new HandleOfAllocIndex_New( nSystemId,nServiceId,nGroupId,nProcessType,nPserialNo,nPreMark, MAXBTREEMEMBER, ( unsigned int )stPtrIndexInfo+SIZE_BTREEINFO );
	if( 0 )
	{
		cout << "*******************************" << endl;
		cout << "stPtrIndexInfo(BTREE SHAREDMEM): " << stPtrIndexInfo << endl;
		cout << "pHandleBtreeIndex(BTREE Alloc): " << pHandleBtreeIndex << endl;
		cout << "*******************************" << endl;
	}
}
// destructor
CBtree::~CBtree()
{
	delete pHandleBtreeIndex;
}
//===============================================================
// InsertData Method					  	 					=
//  : 하나의 키값을 노드에 입력해주는 Method							=
// argument     :  stDataType->입력할 _DATATYPE					=
// return value : None											=
//																=
//===============================================================
void CBtree::InsertData( _DATATYPE stDataType )
{
	_BTREENODE *pNew,*temp;
	_DATATYPE xNew;
	unsigned int  		unBaseAddress;
	unsigned int 		unCurrentAddress;
	
	// Observer와 root정보를 맞추기 위해서 추가.. 2000. 12. 08
	// Btree의 Root정보를 가져와서 Root에 넣어준다.
	 memcpy( &stBtreeInfo, stPtrIndexInfo, SIZE_BTREEINFO );
	 pBtreeRoot=stBtreeInfo.pBtreeRoot;
	// 재귀함수 Insert를 호출하여 실제 입력이 일어나는 부분
	status code=_Insert( pBtreeRoot, stDataType, xNew, pNew );
	
	// 중복검사.
	if( code==DuplicateKey )
		cout << "key already exist.\n";
	// 최초 pBtreeRoot node와 node가 분할될때 부모노드를 만들어 준다.	
	if( code==InsertNotComplete )
	{
		// setting index shared memory flag=1
		// allocate shared memory
		
		//pHandleBtreeIndex->mem_lock(); // MEMORY LOCK
		temp =( _BTREENODE * )pHandleBtreeIndex->newIndex( SIZE_BTREENODE );
		unCurrentAddress=( unsigned int )temp;
		unBaseAddress=pHandleBtreeIndex->get_unBaseAddress();
		//pHandleBtreeIndex->mem_unlock(); // MEMORY UNLOCK
		
		_BTREENODE *root0=pBtreeRoot;
		pBtreeRoot=temp;

		// insert Btree Root info into Btree Shared Memory
		stBtreeInfo.pBtreeRoot=pBtreeRoot;
		memcpy( stPtrIndexInfo, &stBtreeInfo, SIZE_BTREEINFO );

		pBtreeRoot->iNumberKey=1;
		pBtreeRoot->iIndex=( unCurrentAddress-unBaseAddress )/sizeof( _BTREENODE );
		pBtreeRoot->kIndexKey[ 0 ]=xNew;
		pBtreeRoot->pBTREENODE[ 0 ]=root0; 
		pBtreeRoot->pBTREENODE[ 1 ]=pNew;
	}
}
//===============================================================
// _Insert Method						  	 					=
//  : 실제 입력이 일어나는 재귀함수									=
// argument     : r->Btree Root Node(삽입할 대상이 되는 Node)		=
//				  x->입력할 _DATATYPE							=
//				  y->처리를 위한 임시 _DATATYPE					=
//				  q->처리를 위한 임시 _BTREENODE					=
// return value : Success -> 입력이 성공적으로됐음.					=
//				  DuplicateKey -> 중복되는 값이 있음.				=
//				  InsertNotComplete -> 새로운 노드를 생성하거나,	=
//					다시 _Insert()함수를 호출하기 위한 상태변수		=
//===============================================================   
status CBtree::_Insert( _BTREENODE *r, _DATATYPE x, _DATATYPE &y, _BTREENODE* &q )
{
	_BTREENODE *pNew, *pFinal, *temp;
	int i,j,n;
	_DATATYPE xNew, kFinal;
	status code;
	unsigned int  		unBaseAddress;
	unsigned int 		unCurrentAddress;
	// 실제로 값이 대입되는 부분   
	if( r==NULL )
	{
		q=NULL; // 다음 노드를 가리키는 포인터에 NULL        
		y=x;    // xNew에 새로운 값을 대입       
		return InsertNotComplete;
	}
	n=r->iNumberKey;
	// 이전 노드들을 검색
	i=_NodeSearch( x.iDataKey,r->kIndexKey, n );
	// **********inset update code
	if( i<n && x.iDataKey==r->kIndexKey[ i ].iDataKey )
		return DuplicateKey;
	code=_Insert( r->pBTREENODE[ i ], x, xNew, pNew );
	if( code != InsertNotComplete )
		return code;
	// 현재 노드가 공간이 비어있고, 그 노드에 값을 insert   
	if( n<MEMBERPOINTER-1 )
	{
		// 중간부분에 값이 들어갈때 이전 값들을
		// 한칸씩 뒤로 밀어주는 부분.    
		i=_NodeSearch( xNew.iDataKey,r->kIndexKey, n );
		for( j=n; j>i; j-- )
		{
			r->kIndexKey[ j ]=r->kIndexKey[ j-1 ];
			r->pBTREENODE[ j+1 ]=r->pBTREENODE[ j ];
		}
		r->kIndexKey[ i ]=xNew;
		r->pBTREENODE[ i+1 ]=pNew;
		++r->iNumberKey;
		return Success;
	}
	// 현재 노드가 가득찼을때, 
	
	// 들어갈 값이 이노드의 값들보다 클때        
	if( i==MEMBERPOINTER-1 )
	{
		kFinal=xNew;
		pFinal=pNew;

	}
	// 현재 노드의 중간에 값이 들어가야 하므로          
	// 현재 노드를 둘로 분할해야 할때     
	else
	{
		kFinal=r->kIndexKey[ MEMBERPOINTER-2 ]; 
		pFinal=r->pBTREENODE[ MEMBERPOINTER-1 ];
		for( j=MEMBERPOINTER-2; j>i; j-- )
		{
			r->kIndexKey[ j ]=r->kIndexKey[ j-1 ];
			r->pBTREENODE[ j+1 ]=r->pBTREENODE[ j ];
		}
		r->kIndexKey[ i ]=xNew;
		r->pBTREENODE[ i+1 ]=pNew;
	}
	// setting index shared memory flag=1
	// allocate shared memory
	temp =( _BTREENODE * )pHandleBtreeIndex->newIndex( SIZE_BTREENODE );
	unCurrentAddress=( unsigned int )temp;
	unBaseAddress=pHandleBtreeIndex->get_unBaseAddress();
	//unBaseAddress=pHandleBtreeIndex->get_unBaseAddress()+SIZE_BTREEINFO;

	int h=( MEMBERPOINTER-1 )/2;   // 중간값을 구해준다. 
	y=r->kIndexKey[ h ];
	q=temp;
	r->iNumberKey=h;
	q->iNumberKey=MEMBERPOINTER-1-h;
	q->iIndex=( unCurrentAddress-unBaseAddress )/sizeof( _BTREENODE );
	// 끝의 두개의 값을 새로운 노드의 처음과 두번째로      
	// 넣어주는 부분   
	for( j=0; j<q->iNumberKey; j++ )
	{
		q->pBTREENODE[ j ]=r->pBTREENODE[ j+h+1 ];
		q->kIndexKey[ j ]=( j<q->iNumberKey-1 ? r->kIndexKey[ j+h+1 ] : kFinal );
	}
	q->pBTREENODE[ q->iNumberKey ]=pFinal;

	return InsertNotComplete;
}
//===============================================================
// _print Method						  	 					=
//  : root에서부터 node들의 키값들을 출력해주는 부분 					=
// argument     :  r->Btree Root Node Pointer					=
//				   nSpace->출력 간격								=
// return value : None											=
//																=
//===============================================================   
void CBtree::_print( const _BTREENODE *r, int nSpace )const
{
	if( r )
	{
		int i;
	//	cout << setw( nSpace ) << "";
		for( i=0; i<r->iNumberKey; i++ )
			cout << setw( 3 ) << r->kIndexKey[ i ].iDataKey << " " ;
		cout << endl;
		/*
		for( i=0; i<r->iNumberKey; i++ )
			cout << setw( 3 ) << r->kIndexKey[ i ].pUdrFormat << " " ;
		cout << endl;
		*/
		for( i=0; i<= r->iNumberKey; i++ ) _print( r->pBTREENODE[ i ], nSpace+8 );
	}
}
//===============================================================
// _NodeSearch Method						  	 				=
//  : 이전 노드들을 검색하여 새로들어온 값(x)이  한 node내에서			=
//		 몇번째에 들어가야 하는지 그 위치를 반환하여 준다.				=
// argument     :  x->찾고자 하는 Min								=
//				   n->현재노드에 들어있는 data갯수.					=
//				   a->Btree Node Pointer						=
// return value : Position										=
//																=
//===============================================================   
int CBtree::_NodeSearch( int x, const _DATATYPE *a, int n )const
{
	int i=0;
	while( i<n && x>a[ i ].iDataKey ) i++;
	return i;
}
//===============================================================
// ShowSearch Method					  	 					=
//  : root에서부터 node들의 search해서 찾은 memory주소를 반환		=
// argument     :  x->찾고자 하는  _DATATYPE						=
// return value : 찾은 key의 실제 shared memory address.			=
//				  NULL -> Not Exist								=
//===============================================================   
UdrFormat *CBtree::ShowSearch( _DATATYPE *x )
{
	int i,j,n;
	//_BTREENODE *r=pBtreeRoot;
	
	// root 정보를 가져온다.  2000. 12. 12
	// observer와 Aggregator가 동일한 곳(root)을 가리키기 위해서 ..
	memcpy( &stBtreeInfo, stPtrIndexInfo, SIZE_BTREEINFO );
	_BTREENODE *r=stBtreeInfo.pBtreeRoot;

	while( r )
	{
		n=r->iNumberKey;
		// 발견되기 전까지의 값들을 출력한후    
		/*
		for( j=0; j<r->iNumberKey; j++ )
			cout << " " << r->kIndexKey[ j ].iDataKey;
		cout << endl;
		*/
		
		i=_NodeSearch( x->iDataKey,r->kIndexKey,n );
		// search 
		if( i<n && x->iDataKey==r->kIndexKey[ i ].iDataKey )
		{
			return r->kIndexKey[ i ].pUdrFormat;
		}
		// next node search
		r=r->pBTREENODE[ i ];
	}
	return NULL;
}
//===============================================================
// DelNode Method					  	 						=
//  : 원하는 키값을 노드에서 delete 								=
// argument     :  x->삭제할 _DATATYPE							=
// return value : None											=
//																=
//===============================================================
void CBtree::DelNode( _DATATYPE x )
{
	_BTREENODE *root0;
	int 		nTempFlag;
	
	// Observer와 root정보를 맞추기 위해서 추가.. 2000. 12. 08
	// Btree의 Root정보를 가져와서 Root에 넣어준다.
	memcpy( &stBtreeInfo, stPtrIndexInfo, SIZE_BTREEINFO );
	pBtreeRoot=stBtreeInfo.pBtreeRoot;
	nTempFlag = _Delete( pBtreeRoot,x );
	switch ( nTempFlag )
	{
		case NotFound:
			cout << x.iDataKey << " not found.\n";
			break;
		case Underflow:
			root0 = pBtreeRoot;
			pBtreeRoot=pBtreeRoot->pBTREENODE[ 0 ];

			// insert Btree Root info into Btree Shared Memory
			stBtreeInfo.pBtreeRoot=pBtreeRoot;
			memcpy( stPtrIndexInfo, &stBtreeInfo, SIZE_BTREEINFO );
			// setting index shared memory flag=0
			// delete shared memory
			pHandleBtreeIndex->deleteIndex( root0->iIndex );
			break;
	}
}
//===============================================================
// _Delete Method						  	 					=
//  : 실제 삭제가 일어나는 부분 									=
// argument     :  r->Btree Root Node(삭제할 대상이 되는 Node)		=
//				   x->삭제할 _DATATYPE							=
// return value : NotFound : 원하는 값이 존재하지 않음				=
//				  Underflow : 자식노드의 키값의 수가 				=
//				 	 MEMBERPOINTER/2보다 작아져서 병합이 이루어질때,	=
//					부모노드를 삭제해야 할때 						=
//===============================================================  
status CBtree::_Delete( _BTREENODE *r, _DATATYPE x )
{
	if( r==NULL )
		return NotFound;
	int i,j,pivot,n=r->iNumberKey;
	// kIndexKey[ i ]를 r->kIndexKey[ i ]로 사용하기 위하여.  
	_DATATYPE *k=r->kIndexKey;    
	const int nMin=( MEMBERPOINTER-1 )/2;
	status code;
	// pBTREENODE[ i ]를 r->pBTREENODE[ i ]로 사용하기 위하여.  
	_BTREENODE **p=r->pBTREENODE, *pL, *pR;    
	i=_NodeSearch( x.iDataKey,k,n );
	// 현재 노드가 leaf node일때  
	if( p[ 0 ]==NULL )
	{
		if( i==n || x.iDataKey<k[ i ].iDataKey )
			return NotFound;
		for( j=i+1; j< n; j++ )
		{
			// 뒤의 값을 앞으로 땡겨준다.   
			// 5 6 7 8 에서 7을 삭제시   
			// 5 6 8 로 된다.  
			k[j-1]=k[j];
			p[j]=p[j+1];
		}
		// 키값의 갯수를 하나 감소시킨후,  
		// 루트일때는 1개, 그밖의 노드들은 최소한 mMin(2)     
		// 이상 가지고 있어야 하므로  비교해서  갯수가    
		// 많거나 같으면 Success 적으면 Underflow를 반환한다.   
		return --r->iNumberKey >= ( r==pBtreeRoot ? 1 : nMin ) ? Success : Underflow;
	}
	
	// 현재 노드가 root node도 아니고, leaf node도 아닐때   
	// 12 
	//     4   7
	//     16 22
	//            2 3 
	//            5 6
	//            8 9
	//            13 15
	//            17 18
	//            23 65
	// 여기서 16을 삭제하고자 할때    
	if( i<n && x.iDataKey==k[ i ].iDataKey )
	{
		_BTREENODE *q=p[ i ], *q1; 
		int nq;
		// leaf node까지 내려간다. 
		for( ;; )
		{
			nq=q->iNumberKey; 
			q1=q->pBTREENODE[ nq ];
			if( q1==NULL ) break;
			q=q1;
		}
		// 삭제하고자 하는 값을 leaf node의 가장 오른쪽 값과       
		// 바꾸어 준다. 
		k[ i ]=q->kIndexKey[ nq-1 ];
		q->kIndexKey[ nq-1 ]=x;
	}
	// 삭제하고자 하는 값이 현재노드의 자식노드에 있을때
	code = _Delete( p[ i ],x );
	if(code != Underflow) return code;
        
        // 5              -> root node
	//    2 3 4       -> left leaf node
 	//    7 9         -> right leaf node
	// 에서 9를 삭제하고자 할때       
	// 좌측형제노드에서 가장 우측값이 루트로 올라가고     
	// 루트값이 우측노드의 가장 왼쪽값으로 온다.    
	if( i>0 && p[ i-1 ]->iNumberKey > nMin )
	{
		pivot=i-1;
		pL=p[ pivot ];
		pR=p[ i ];
		// 우측노드의 값을 오른쪽으로 한칸씩 밀어준다.    
		pR->pBTREENODE[ pR->iNumberKey+1 ]=pR->pBTREENODE[ pR->iNumberKey ];
		for( j=pR->iNumberKey; j>0; j-- )
		{
			pR->kIndexKey[ j ]=pR->kIndexKey[ j-1 ];
			pR->pBTREENODE[ j ]=pR->pBTREENODE[ j-1 ];
		}
		pR->iNumberKey++;       // 우측노드의 키수를 증가시킨다.    
		// 루트값을 우측노드의 처음으로 넣어준다.    
		pR->kIndexKey[ 0 ]=k[ pivot ];    
		pR->pBTREENODE[ 0 ]=pL->pBTREENODE[ pL->iNumberKey ];
		// 루트값에 좌측노드의 가장 우측값을 넣어준다.     
		k[ pivot ]=pL->kIndexKey[ --pL->iNumberKey ]; 
		return Success;
	}
	 // 5               -> root node
	//    2 3           -> left leaf node
 	//    6 7 9         -> right leaf node
	// 에서 3를 삭제하고자 할때       
	// 우측형제노드에서 가장 좌측값이 루트로 올라가고     
	// 루트값이 좌측노드의 가장 우측값으로 온다.    
	if( i<n && p[ i+1 ]->iNumberKey > nMin )
	{
		pivot=i;
		pL=p[ pivot ];
		pR=p[ pivot+1 ];
		// 루트값이 좌측노드의 가장 오른쪽으로   
		pL->kIndexKey[ pL->iNumberKey ]=k[ pivot ];
		pL->pBTREENODE[ pL->iNumberKey+1 ]=pR->pBTREENODE[ 0 ];
		k[ pivot ]=pR->kIndexKey[ 0 ];
		pL->iNumberKey++;
		pR->iNumberKey--;
		for( j=0; j<pR->iNumberKey; j++ )
		{
			pR->kIndexKey[ j ]=pR->kIndexKey[ j+1 ];
			pR->pBTREENODE[ j ]=pR->pBTREENODE[ j+1 ];
		}
		pR->pBTREENODE[ pR->iNumberKey ]=pR->pBTREENODE[ pR->iNumberKey+1 ];
		return Success;
	}
	
	// 병합이 일어나는 경우   
	// 5              -> root node   
	//    2 3         -> left node
	//    6 7         -> right node   
	// 위에서 2가 삭제되거나 6일 삭제되어야 할때     
	pivot=( i==n ? i-1 : i );
	pL=p[ pivot ]; 
	pR=p[ pivot+1 ];
	// 좌측노드의 가장 우측에 root node의 키값을 넣어준다.     
	pL->kIndexKey[ pL->iNumberKey ]=k[ pivot ];
	// 우측 노드의 값들을 좌측노드의 뒷부분에 붙여준다.     
	pL->pBTREENODE[ pL->iNumberKey+1 ]=pR->pBTREENODE[ 0 ];
	for( j=0; j<pR->iNumberKey; j++ )
	{
		pL->kIndexKey[ pL->iNumberKey+1+j ]=pR->kIndexKey[ j ];
		pL->pBTREENODE[ pL->iNumberKey+2+j ]=pR->pBTREENODE[ j+1 ];
	}
	pL->iNumberKey += 1+pR->iNumberKey;
	// setting index shared memory flag=0
	// delete shared memory
	//pHandleBtreeIndex->mem_lock(); // MEMORY LOCK
	pHandleBtreeIndex->deleteIndex( pR->iIndex );
	//pHandleBtreeIndex->mem_unlock(); // MEMORY UNLOCK

	// 루트노드의 값들을 하나씩 좌측으로 당겨준다.    
	for( j=i+1; j<n; j++ )
	{
		k[ j-1 ]=k[ j ]; 
		p[ j ]=p[ j+1 ];
	}
	return --r->iNumberKey >= ( r==pBtreeRoot ? 1 : nMin ) ? Success : Underflow;
}

#endif


