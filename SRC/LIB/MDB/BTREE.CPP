#include <MDB/Btree.hpp>

//	Log Manager
extern SendLogManage* AggregatorLogM;

// Constructor
// Btree shared memory create.
// Btree Index shared memory create. 
//CBtree::CBtree()
//{
//}
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
		pBtreeRoot=NULL;
	}
	// process가 재실행될때
	else
	{
		// Btree의 Root정보를 가져와서 Root에 넣어준다.
		pBtreeRoot=stBtreeInfo.pBtreeRoot;
	}
	++Btree_Id_Shm;
	//cout << "Btree_Id_Shm=" << Btree_Id_Shm << endl;
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
CBtree::~CBtree()
{
	//delete pHandleBtreeIndex;
}
// InsertData Method.
// 하나의 키값을 노드에 입력.   
void CBtree::InsertData( _DATATYPE stDataType )
{
	_BTREENODE *pNew,*temp;
	_DATATYPE xNew;
	unsigned int  		unBaseAddress;
	unsigned int 		unCurrentAddress;
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
		temp =( _BTREENODE * )pHandleBtreeIndex->newIndex( SIZE_BTREENODE );
		unCurrentAddress=( unsigned int )temp;
		unBaseAddress=pHandleBtreeIndex->get_unBaseAddress();
		
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
// _Insert method.
//실제 입력이 일어나는 재귀함수
// 반환되는 상태변수 
// Success : 입력이 성공적으로됐음.
// DuplicateKey : 중복되는 값이 있음.
// InsertNotComplete : 새로운 노드를 생성하거나,
//     다시 _Insert()함수를 호출하기 위한 상태변수      
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
	//shm_lock();
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
	//shm_release();
	return InsertNotComplete;
}
// _print method. 
// root에서부터 node들의 키값들을 출력해주는 부분    
void CBtree::_print( const _BTREENODE *r, int nSpace )const
{
	if( r )
	{
		int i;
		cout << setw( nSpace ) << "";
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
// _NodeSearch method. 
// return value : position.
// 이전 노드들을 검색하여 새로들어온 값(x)이  한 node내에서    
// 몇번째에 들어가야 하는지 그 위치를 반환하여 준다.     
int CBtree::_NodeSearch( int x, const _DATATYPE *a, int n )const
{
	int i=0;
	while( i<n && x>a[ i ].iDataKey ) i++;
	return i;
}
// ShowSearch method.
// return value : 찾은 key의 실제 shared memory address.
UdrFormat *CBtree::ShowSearch( _DATATYPE *x )const
{
	int i,j,n;
	_BTREENODE *r=pBtreeRoot;

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
// DelNode method.
// 원하는 키값을 노드에서 delete   
void CBtree::DelNode( _DATATYPE x )
{
	_BTREENODE *root0;
	
	//shm_lock();
	switch ( _Delete( pBtreeRoot,x ) )
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
//	shm_release();
}
// _Delete method.
// 실제 삭제가 일어나는 부분    
// 반환되는 상태변수    
//  NotFound : 원하는 값이 존재하지 않음.    
//  Underflow : 자식노드의 키값의 수가 MEMBERPOINTER/2보다  
//             작아져서 병합이 이루어질때, 부모노드를     
//             삭제해야 할때    
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
	pHandleBtreeIndex->deleteIndex( pR->iIndex );

	// 루트노드의 값들을 하나씩 좌측으로 당겨준다.    
	for( j=i+1; j<n; j++ )
	{
		k[ j-1 ]=k[ j ]; 
		p[ j ]=p[ j+1 ];
	}
	return --r->iNumberKey >= ( r==pBtreeRoot ? 1 : nMin ) ? Success : Underflow;
}




