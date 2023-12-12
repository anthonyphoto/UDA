/******************************************************************
* 파 일 명     :  Btree.hpp		     							  
* 기   능                                                          
* : This file is defines btree based on shared memory   		  
*   create a Customer btree node in shared memory            	  
* 작 성 자     : Han Guen Hee                    				  
* first data  : 2000. 09. 29       	     						  
* last updata : 2001. 04. 02        		 					  
* program history                        						  
*                         	        	 				 		  
* bug fix                        	     						  
*   . btree full check -> 2001. 05. 01                      	        	 						 
******************************************************************/
#ifndef TPL_BTREE_HPP
#define TPL_BTREE_HPP

#include <iomanip.h>
#include <HandleOfAllocIndex_new.hpp>
#include <DataType.hpp>
#include <node.hpp>
#include <SendLogManage.hpp>
//#include <FileLogWrite.hpp>
#include <errno.h>

#define BTREE_DEBUG		0

extern SendLogManage*    LogM;   // 로그매니저 (Main Class에서 정의)

enum status{ InsertNotComplete, Success, DuplicateKey, Underflow, NotFound };

template <class keyType, class dataType>
class CBtree : virtual public ShmMemory
{
	public:
		CBtree( char* PathNameShm, char Btree_Id_Shm );
		~CBtree();
		void 								InsertData( _DATATYPE<keyType, dataType> cDataType );
		void 								Print( void );
		void 								DelNode( _DATATYPE<keyType, dataType> cDataType );
		dataType*  							selectNode( keyType cKeyType );
		_BTREENODE<keyType, dataType>* 		getBtreeRoot( void ) 
											{ 
												//memcpy( &stBtreeInfo, ShmPtr(), sizeof( _BTREEINFO<keyType, dataType> ) ); 
												memcpy( &stBtreeInfo, stPtrIndexInfo, sizeof( _BTREEINFO<keyType, dataType> ) ); 
												return stBtreeInfo.pBtreeRoot; 
											}
		void 								setBtreeRoot( _BTREENODE<keyType, dataType> *pBtreeNode ) 
											{ 
												pBtreeRoot=pBtreeNode; 
											}
		unsigned int						getIndexKey( void )  
											{ 
												return (unsigned int ) ShmPtr(); 
											}
		unsigned int						getAllocKey( void )  
											{ 
												return (unsigned int ) pHandleBtreeIndex->get_Shm_Key(); 
											}
		void								set_BtreeFlag( int nValue ) 
											{ 
												stBtreeInfo.nFlag=nValue; 
											//	memcpy( ShmPtr(), &stBtreeInfo, sizeof( _BTREEINFO<keyType, dataType> ) );
												memcpy( stPtrIndexInfo, &stBtreeInfo, sizeof( _BTREEINFO<keyType, dataType> ) );
											}
		int									get_BtreeFlag( void ) 
											{ 
											//	memcpy( &stBtreeInfo, ShmPtr(), sizeof( _BTREEINFO<keyType, dataType> ) ); 
												memcpy( &stBtreeInfo, stPtrIndexInfo, sizeof( _BTREEINFO<keyType, dataType> ) ); 
												return stBtreeInfo.nFlag; 
											}
	private:
		_BTREENODE<keyType, dataType>*		pBtreeRoot;
		_BTREEINFO<keyType, dataType>       stBtreeInfo;
	    _BTREENODE<keyType, dataType>*	  	stPtrIndexInfo;
	    HandleOfAllocIndex_New* 			pHandleBtreeIndex;  
//	    FileLogWrite*						pFileLogWrite;
		status 								_Insert( _BTREENODE<keyType, dataType> *pBtreeNode, 	\
													_DATATYPE<keyType, dataType> cInputDataType , 	\
													_DATATYPE<keyType, dataType> &cTempDataType, 	\
													_BTREENODE<keyType, dataType>* &cTempNode );
		void 								_print( const _BTREENODE<keyType, dataType> *pBtreeNode )const;
		int 								_NodeSearch( _DATATYPE<keyType, dataType> cDataType, 	\
   													const _DATATYPE<keyType, dataType> *pDataType, \
   													int n )const;
		status 								_Delete( _BTREENODE<keyType, dataType> *pBtreeNode, \
													_DATATYPE<keyType, dataType> cDataType );

};
// stPtrIndexInfo.get_nMaxBtreeMember()의 값을 가져오는걸 다시 생각해 볼 문제이다.************2001. 03. 23
template <class keyType, class dataType>
CBtree<keyType, dataType>::CBtree( char* PathNameShm, char Btree_Id_Shm )
: ShmMemory( PathNameShm, Btree_Id_Shm, ( sizeof( _BTREENODE<keyType, dataType> )*MAXBTREEMEMBER )+sizeof( _BTREEINFO<keyType, dataType> ), 0660|IPC_CREAT )
//: ShmMemory( PathNameShm, Btree_Id_Shm, ( sizeof( _BTREENODE<keyType, dataType> )*( stPtrIndexInfo.get_nMaxBtreeMember()) )+sizeof( _BTREEINFO<keyType, dataType> ), 0660|IPC_CREAT )
{
	if( BTREE_DEBUG )
	{
		*LogM << LogM->ErrorCode(0) << "[CBtree] Constructor" << endl;
		*LogM << LogM->ErrorCode(0) << "[CBtree] PathNameShm : [" << PathNameShm << "]" << endl;
		*LogM << LogM->ErrorCode(0) << "[CBtree] Btree_Id_Shm : [" << Btree_Id_Shm << "]" << endl;
	}
	stPtrIndexInfo=( _BTREENODE<keyType, dataType>* )ShmPtr();
	memcpy( &stBtreeInfo, stPtrIndexInfo, sizeof( _BTREEINFO<keyType, dataType> ) );
	// 처음으로 실행되는 경우.
	if( !stBtreeInfo.nFlag )
	{
		*LogM << LogM->ErrorCode(0) << "[CBtree] Flag = 0"  << endl;
		stBtreeInfo.nFlag=1;
		stBtreeInfo.pBtreeRoot = NULL;
	//	memcpy( ShmPtr(), &stBtreeInfo, sizeof( _BTREEINFO<keyType, dataType> ) );
		memcpy( stPtrIndexInfo, &stBtreeInfo, sizeof( _BTREEINFO<keyType, dataType> ) );
		pBtreeRoot=NULL;
	}
	// process가 재실행될때
	else
	{
		*LogM << LogM->ErrorCode(0) << "[CBtree] Flag = 1"  << endl;
		// Btree의 Root정보를 가져와서 Root에 넣어준다.
		pBtreeRoot=stBtreeInfo.pBtreeRoot;
	}
	++Btree_Id_Shm;
	pHandleBtreeIndex = new HandleOfAllocIndex_New( (const char*)PathNameShm, ( const unsigned char )Btree_Id_Shm,MAXBTREEMEMBER, ( unsigned int )stPtrIndexInfo+sizeof( _BTREEINFO<keyType, dataType> ) );
	if( BTREE_DEBUG )
	{
		*LogM << LogM->ErrorCode(0) << "[CBtree] ShmPtr : [" << (unsigned int)ShmPtr() << "]" << endl;
		*LogM << LogM->ErrorCode(0) << "[CBtree] MAXBTREEMEMBER : [" << MAXBTREEMEMBER << "]" << endl;
		*LogM << LogM->ErrorCode(0) << "[CBtree] pHandleBtreeIndex(BTREE Alloc) : [" << (unsigned int)pHandleBtreeIndex << "]" << endl;
		*LogM << LogM->ErrorCode(0) << "[CBtree] PathNameShm : [" << PathNameShm << "]" << endl;
		*LogM << LogM->ErrorCode(0) << "[CBtree] Btree_Id_Shm : [" << Btree_Id_Shm << "]" << endl;
	}
//	pFileLogWrite = new FileLogWrite("./", "testbtree");
}
// destructor
template <class keyType, class dataType>
CBtree<keyType, dataType>::~CBtree()
{
	delete pHandleBtreeIndex;
	pHandleBtreeIndex = 0;
//	delete pFileLogWrite;
}
//===============================================================
// InsertData Method					  	 					=
//  : 하나의 키값을 노드에 입력해주는 Method							=
// argument     :  cDataType->입력할 _DATATYPE					=
// return value : None											=
//																=
//===============================================================
template <class keyType, class dataType>
void CBtree<keyType, dataType>::InsertData( _DATATYPE<keyType, dataType> cDataType )
{
	_BTREENODE<keyType, dataType> 		*pNew,*temp;
	_DATATYPE<keyType, dataType> 		xNew;
	unsigned int  						unBaseAddress;
	unsigned int 						unCurrentAddress;
	
	if( BTREE_DEBUG )
	{
		*LogM << LogM->ErrorCode(0) << "[CBtree][InsertData]" << endl;
	}
	// Observer와 root정보를 맞추기 위해서 추가.. 2000. 12. 08
	// Btree의 Root정보를 가져와서 Root에 넣어준다.
	// memcpy( &stBtreeInfo, ShmPtr(), sizeof( _BTREEINFO<keyType, dataType> ) );
	  memcpy( &stBtreeInfo, stPtrIndexInfo, sizeof( _BTREEINFO<keyType, dataType> ) );
	 pBtreeRoot=stBtreeInfo.pBtreeRoot;
	// 재귀함수 Insert를 호출하여 실제 입력이 일어나는 부분
	status code=_Insert( pBtreeRoot, cDataType, xNew, pNew );
	
	// 중복검사.
	if( code==DuplicateKey )
		*LogM << LogM->ErrorCode(0) << "[CBtree][InsertData] key already exist." << endl;
	//	cout << "key already exist.\n";
	// 최초 pBtreeRoot node와 node가 분할될때 부모노드를 만들어 준다.	
	if( code==InsertNotComplete )
	{
		// setting index shared memory flag=1
		// allocate shared memory
		temp =( _BTREENODE<keyType, dataType> * )pHandleBtreeIndex->newIndex( sizeof( _BTREENODE<keyType, dataType> ) );
		// btree full check 2001. 05. 01
		if( temp == 0 )
		{
			*LogM << LogM->ErrorCode(0) << "[CBtree][InsertData] Btree Full!!!!" << endl;
			exit( 0 );
		}
		
		unCurrentAddress=( unsigned int )temp;
		unBaseAddress=pHandleBtreeIndex->get_unBaseAddress();
		_BTREENODE<keyType, dataType> *root0=pBtreeRoot;
		pBtreeRoot=temp;
		// insert Btree Root info into Btree Shared Memory
		stBtreeInfo.pBtreeRoot=pBtreeRoot;
	//	memcpy( ShmPtr(), &stBtreeInfo, sizeof( _BTREEINFO<keyType, dataType> ) );
		memcpy( stPtrIndexInfo, &stBtreeInfo, sizeof( _BTREEINFO<keyType, dataType> ) );
		
		//pBtreeRoot->iNumberKey=1;
		//pBtreeRoot->iIndex=( unCurrentAddress-unBaseAddress )/sizeof( _BTREENODE<keyType, dataType> );
		// 2001. 04. 05
		pBtreeRoot->set_iNumberKey(1);
		pBtreeRoot->set_iIndex( ( unCurrentAddress-unBaseAddress )/sizeof( _BTREENODE<keyType, dataType> ) );
	//	pBtreeRoot->kIndexKey[ 0 ]=xNew;
		memcpy( (char*)&pBtreeRoot->kIndexKey[0], (char*)&xNew, sizeof(  _DATATYPE<keyType, dataType> ));  // 2001. 03. 07
		
		// Null 포인터값에 NULL값을 assign시켜서 memory fault가 난다.
		pBtreeRoot->pBTREENODE[ 0 ]=root0;
	//	memcpy( pBtreeRoot->pBTREENODE[ 0 ], root0, sizeof(  pBtreeRoot->pBTREENODE[ 0 ] )); 
		pBtreeRoot->pBTREENODE[ 1 ]=pNew;
	//	memcpy( pBtreeRoot->pBTREENODE[ 1 ], pNew, sizeof(  pBtreeRoot->pBTREENODE[ 1 ] ));
		if( BTREE_DEBUG )
		{
			*LogM << LogM->ErrorCode(0) << "[CBtree][InsertData] pBtreeRoot : [" <<  (unsigned int)pBtreeRoot << "]" << endl;
			*LogM << LogM->ErrorCode(0) << "[CBtree][InsertData] root0 : [" << (unsigned int)root0 << "]" << endl;
			*LogM << LogM->ErrorCode(0) << "[CBtree][InsertData] pNew : [" << (unsigned int)pNew << "]" << endl;
		}
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
template <class keyType, class dataType>
status CBtree<keyType, dataType>::_Insert( _BTREENODE<keyType, dataType> *pRootNode, 	\
									_DATATYPE<keyType, dataType> cInputDataType, 		\
									_DATATYPE<keyType, dataType> &cTempDataType, 		\
									_BTREENODE<keyType, dataType>* &cTempNode )
{
	_BTREENODE<keyType, dataType> 	*pNew, *pFinal, *temp;
	_DATATYPE<keyType, dataType> 	xNew, kFinal;
	status 							code;
	unsigned int  					unBaseAddress;
	unsigned int 					unCurrentAddress;
	int 							i,j,n;  
	int 							compare;      // 비교 결과 flag   2001. 03. 07
	
	
	// 실제로 값이 대입되는 부분   
	if( pRootNode==NULL )
	{
		cTempNode=NULL; // 다음 노드를 가리키는 포인터에 NULL        
		cTempDataType=cInputDataType;    
		// xNew에 새로운 값을 대입  
		// 과연 이 &cTempDataType, &cInputDataType 중 어느것이 맞나?     
	//	memcpy( &cTempDataType, &cInputDataType, sizeof(  _DATATYPE<keyType, dataType> )); 
		return InsertNotComplete;
	}
	n=pRootNode->iNumberKey;
	// 이전 노드들을 검색
	i=_NodeSearch( cInputDataType,pRootNode->kIndexKey, n );
	/* 
	if( i<n && cInputDataType.iDataKey==pRootNode->kIndexKey[ i ].iDataKey )
		return DuplicateKey;
	*/
	// **********insert update code
	// 2001. 03. 07
    compare=memcmp( &cInputDataType.dtype, &pRootNode->kIndexKey[i].dtype, sizeof(keyType));
	if( BTREE_DEBUG )
	{
    	*LogM << LogM->ErrorCode(0) << "[CBtree][_Insert] compare : [" << (unsigned int)compare << "]" << endl;
    }
    if (i < n && compare == 0)
    {
    	*LogM << LogM->ErrorCode(0) << "[CBtree][_Insert] return DuplicateKey" << endl;
   		return DuplicateKey;
   	}
   	
	code=_Insert( pRootNode->pBTREENODE[ i ], cInputDataType, xNew, pNew );
	if( BTREE_DEBUG )
	{
		*LogM << LogM->ErrorCode(0) << "[CBtree][_Insert] code : [" << (unsigned int)code << "]" << endl;
	}
	if( code != InsertNotComplete )
		return code;
		
	// 현재 노드가 공간이 비어있고, 그 노드에 값을 insert 
	//if( n<stPtrIndexInfo.get_nMemberPointer()-1 )
	if( n<MEMBERPOINTER-1 )
	{
		// 중간부분에 값이 들어갈때 이전 값들을
		// 한칸씩 뒤로 밀어주는 부분.    
		i=_NodeSearch( xNew,pRootNode->kIndexKey, n );
		if( BTREE_DEBUG )
		{
			*LogM << LogM->ErrorCode(0) << "[CBtree][_Insert] i : [" << (unsigned int)i << "]" << endl;
		}
		for( j=n; j>i; j-- )
		{
			//pRootNode->kIndexKey[ j ]=pRootNode->kIndexKey[ j-1 ];
			// 2001. 03. 07
			memcpy( &pRootNode->kIndexKey[j], &pRootNode->kIndexKey[j-1], sizeof( _DATATYPE<keyType, dataType> ) );
			pRootNode->pBTREENODE[ j+1 ]=pRootNode->pBTREENODE[ j ];
		}
		//pRootNode->kIndexKey[ i ]=xNew;
		memcpy( &pRootNode->kIndexKey[i], &xNew, sizeof( _DATATYPE<keyType, dataType> ) );
		pRootNode->pBTREENODE[ i+1 ]=pNew;
		++pRootNode->iNumberKey;
		return Success;
	}

	// 현재 노드가 가득찼을때, 
	// 들어갈 값이 이노드의 값들보다 클때   
	//if( i==stPtrIndexInfo.get_nMemberPointer()-1 )
	if( i==MEMBERPOINTER-1 )
	{
		//kFinal=xNew;
		// 2001. 03. 07
		memcpy( &kFinal, &xNew, sizeof( _DATATYPE<keyType, dataType> ) );
		pFinal=pNew;
	}
	// 현재 노드의 중간에 값이 들어가야 하므로          
	// 현재 노드를 둘로 분할해야 할때     
	else
	{
		//kFinal=pRootNode->kIndexKey[ stPtrIndexInfo.get_nMemberPointer()-2 ]; 
		// 2001. 03. 07
		memcpy( &kFinal, &pRootNode->kIndexKey[MEMBERPOINTER-2], sizeof( _DATATYPE<keyType, dataType> ) );
		//memcpy( &kFinal, &pRootNode->kIndexKey[stPtrIndexInfo.get_nMemberPointer()-2], sizeof( _DATATYPE<keyType, dataType> ) );
		pFinal=pRootNode->pBTREENODE[ MEMBERPOINTER-1 ];
		//pFinal=pRootNode->pBTREENODE[ stPtrIndexInfo.get_nMemberPointer()-1 ];
	//	for( j=stPtrIndexInfo.get_nMemberPointer()-2; j>i; j-- )
		for( j=MEMBERPOINTER-2; j>i; j-- )
		{
		//	pRootNode->kIndexKey[ j ]=pRootNode->kIndexKey[ j-1 ];
		// 2001. 03. 07
			memcpy( &pRootNode->kIndexKey[j], &pRootNode->kIndexKey[j-1], sizeof( _DATATYPE<keyType, dataType> ) );
			pRootNode->pBTREENODE[ j+1 ]=pRootNode->pBTREENODE[ j ];
		}
	//	pRootNode->kIndexKey[ i ]=xNew;
	// 2001. 03. 07
		memcpy( &pRootNode->kIndexKey[i], &xNew, sizeof( _DATATYPE<keyType, dataType> ) );
		pRootNode->pBTREENODE[ i+1 ]=pNew;
	}
	// setting index shared memory flag=1
	// allocate shared memory
	temp =( _BTREENODE<keyType, dataType> * )pHandleBtreeIndex->newIndex( sizeof( _BTREENODE<keyType, dataType> ) );
	
	// btree full check 2001. 05. 01
	if( temp == 0 )
	{
		*LogM << LogM->ErrorCode(0) << "[CBtree][_Insert] Btree Full!!!!" << endl;
		exit( 0 );
	}
	unCurrentAddress=( unsigned int )temp;
	unBaseAddress=pHandleBtreeIndex->get_unBaseAddress();
	//unBaseAddress=pHandleBtreeIndex->get_unBaseAddress()+sizeof( _BTREEINFO<keyType, dataType> );

//	int h=( stPtrIndexInfo.get_nMemberPointer()-1 )/2;   // 중간값을 구해준다. 
	int h=( MEMBERPOINTER-1 )/2;   // 중간값을 구해준다.
	 //============이상한 부분임.. 다시 검토 요망. 왜 memcpy를 사용하지 않았나? 2001. 03. 07
	cTempDataType=pRootNode->kIndexKey[ h ];
	//memcpy( &cTempDataType, &pRootNode->kIndexKey[ h ], sizeof( _DATATYPE<keyType, dataType> ) );

	// 2001. 04. 05
	cTempNode=temp;
//	pRootNode->iNumberKey=h;
//	cTempNode->iNumberKey=stPtrIndexInfo.get_nMemberPointer()-1-h;
//  2001. 04. 05
	pRootNode->set_iNumberKey(h);
	//cTempNode->set_iNumberKey(stPtrIndexInfo.get_nMemberPointer()-1-h);
	cTempNode->set_iNumberKey(MEMBERPOINTER-1-h);
	cTempNode->set_iIndex(( unCurrentAddress-unBaseAddress )/sizeof( _BTREENODE<keyType, dataType> ));
	
	// 끝의 두개의 값을 새로운 노드의 처음과 두번째로      
	// 넣어주는 부분   
	for( j=0; j<cTempNode->iNumberKey; j++ )
	{
		cTempNode->pBTREENODE[ j ]=pRootNode->pBTREENODE[ j+h+1 ];
//		cTempNode->kIndexKey[ j ]=( j<cTempNode->iNumberKey-1 ? pRootNode->kIndexKey[ j+h+1 ] : kFinal );
// 		2001. 03. 07
      	if( j < cTempNode->iNumberKey-1 )
      	{
      		memcpy( &cTempNode->kIndexKey[j], &pRootNode->kIndexKey[j + h + 1], sizeof( _DATATYPE<keyType, dataType> ) );
      	}
      	else
      	{
      		memcpy( &cTempNode->kIndexKey[j], &kFinal, sizeof( _DATATYPE<keyType, dataType> ) );
      	}
	}
	cTempNode->pBTREENODE[ cTempNode->iNumberKey ]=pFinal;
	return InsertNotComplete;
}
template <class keyType, class dataType>
void CBtree<keyType, dataType>::Print( void )
{
	// memcpy( &stBtreeInfo, ShmPtr(), sizeof( _BTREEINFO<keyType, dataType> ) );
	 memcpy( &stBtreeInfo, stPtrIndexInfo, sizeof( _BTREEINFO<keyType, dataType> ) );
	 pBtreeRoot=stBtreeInfo.pBtreeRoot;
	 
	*LogM << LogM->ErrorCode(0) << "[CBtree]============== start print =============" << endl;
	_print( pBtreeRoot );
	*LogM << LogM->ErrorCode(0) << "[CBtree]============== end Print ===============" << endl;
}
//===============================================================
// _print Method						  	 					=
//  : root에서부터 node들의 키값들을 출력해주는 부분 					=
// argument     :  r->Btree Root Node Pointer					=
//				   nSpace->출력 간격								=
// return value : None											=
//																=
//===============================================================   
template <class keyType, class dataType>
void CBtree<keyType, dataType>::_print( const _BTREENODE<keyType, dataType> *pRootNode )const
{
	if( pRootNode )
	{
		*LogM << LogM->ErrorCode(0) << "[CBtree][_print] pRootNode : [" <<  (unsigned int)pRootNode << "]" << endl;
		int i;
		for( i=0; i<pRootNode->iNumberKey; i++ )
			pRootNode->kIndexKey[i].dtype.print_all();
		/*
		for( i=0; i<pRootNode->iNumberKey; i++ )
			cout << setw( 3 ) << pRootNode->kIndexKey[ i ].pdataType << " " ;
		cout << endl;
		*/
		for( i=0; i<= pRootNode->iNumberKey; i++ ) _print( pRootNode->pBTREENODE[ i ] );
	}
}
//===============================================================
// _NodeSearch Method						  	 				
//  : 이전 노드들을 검색하여 새로들어온 값(x)이  한 node내에서			
//		 몇번째에 들어가야 하는지 그 위치를 반환하여 준다.				
// argument     :  cDataType->찾고자 하는 Min								
//				   n->현재노드에 들어있는 data갯수.					
//				   pDataType->Btree Node Pointer						
// return value : Position										
//																
//===============================================================   
template <class keyType, class dataType>
int CBtree<keyType, dataType>::_NodeSearch( _DATATYPE<keyType, dataType> cDataType, \
						 const _DATATYPE<keyType, dataType> *pDataType, int n )const
{  
	int 			i=0;
	int 			compare;
	
	compare=memcmp( &cDataType.dtype, &pDataType[i].dtype, sizeof( keyType ) );
    while ( i < n && compare > 0 )
    {
    	i++;
    //	pDataType[i].dtype.print_all();
    	compare=memcmp( &cDataType.dtype, &pDataType[i].dtype, sizeof( keyType ) );
    }
    return i;
}
//===============================================================
// selectNode Method					  	 					=
//  : root에서부터 node들의 search해서 찾은 memory주소를 반환		=
// argument     :  x->찾고자 하는  _DATATYPE						=
// return value : 찾은 key의 실제 shared memory address.			=
//				  NULL -> Not Exist								=
//=============================================================== 
template <class keyType, class dataType>  
dataType *CBtree<keyType, dataType>::selectNode( keyType cKeyType )
{
	int 							i,j,n;
	int								compare;
	_DATATYPE<keyType, dataType> 	cDataType;
	//_BTREENODE<keyType, dataType> *r=pBtreeRoot;
	
	// root 정보를 가져온다.  2000. 12. 12
	// observer와 Aggregator가 동일한 곳(root)을 가리키기 위해
//	memcpy( &stBtreeInfo, ShmPtr(), sizeof( _BTREEINFO<keyType, dataType> ) );
	memcpy( &stBtreeInfo, stPtrIndexInfo, sizeof( _BTREEINFO<keyType, dataType> ) );
	_BTREENODE<keyType, dataType> *pRootNode=stBtreeInfo.pBtreeRoot;
	
	cDataType.set_keyType( &cKeyType );
	while( pRootNode )
	{
		n=pRootNode->iNumberKey;
		// 발견되기 전까지의 값들을 출력한후    
		/*
		for( j=0; j<pRootNode->iNumberKey; j++ )
			pRootNode->k[j].dtype.print_all();
		cout << endl;
		
		i=_NodeSearch( pDataType->iDataKey,pRootNode->kIndexKey,n );
		// search 
		if( i<n && pDataType->iDataKey==pRootNode->kIndexKey[ i ].iDataKey )
		{
			return pRootNode->kIndexKey[ i ].pdataType;
		}
		*/
		i = _NodeSearch(cDataType, pRootNode->kIndexKey, n);
		 // search 
        compare=memcmp( &cKeyType, &pRootNode->kIndexKey[i].dtype, sizeof(keyType));
        if (i < n && compare == 0)
        {  
        	*LogM << LogM->ErrorCode(0) << "[CBtree][selectNode] Key : " << endl;
        	cKeyType.print_all();
           	return pRootNode->kIndexKey[ i ].pdataType;
        }
		// next node search
		pRootNode=pRootNode->pBTREENODE[ i ];
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
template <class keyType, class dataType>
void CBtree<keyType, dataType>::DelNode( _DATATYPE<keyType, dataType> cDataType )
{
	_BTREENODE<keyType, dataType> *root0;
	int 		nTempFlag;
	
	if( BTREE_DEBUG )
	{
		*LogM << LogM->ErrorCode(0) << "[CBtree][DelNode]**************"  << endl;  
	}
	// Observer와 root정보를 맞추기 위해서 추가.. 2000. 12. 08
	// Btree의 Root정보를 가져와서 Root에 넣어준다.
	//memcpy( &stBtreeInfo, ShmPtr(), sizeof( _BTREEINFO<keyType, dataType> ) );
	memcpy( &stBtreeInfo,stPtrIndexInfo, sizeof( _BTREEINFO<keyType, dataType> ) );
	pBtreeRoot=stBtreeInfo.pBtreeRoot;
	
	nTempFlag = _Delete( pBtreeRoot,cDataType );
	switch ( nTempFlag )
	{
		case NotFound:
			cDataType.dtype.print_all();
			*LogM << LogM->ErrorCode(0) << "[CBtree][DelNode] not found. " << endl;
      		//cout  << "not found.\n";
			break;
		case Underflow:
			*LogM << LogM->ErrorCode(0) << "[CBtree][DelNode] Underflow****************** " << endl;
			root0 = pBtreeRoot;
			pBtreeRoot=pBtreeRoot->pBTREENODE[ 0 ];

			// insert Btree Root info into Btree Shared Memory
			stBtreeInfo.pBtreeRoot=pBtreeRoot;
		//	memcpy( ShmPtr(), &stBtreeInfo, sizeof( _BTREEINFO<keyType, dataType> ) );
			memcpy( stPtrIndexInfo, &stBtreeInfo, sizeof( _BTREEINFO<keyType, dataType> ) );
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
//				 	 nMemberPointer/2보다 작아져서 병합이 이루어질때,	=
//					부모노드를 삭제해야 할때 						=
//=============================================================== 
template <class keyType, class dataType>
status CBtree<keyType, dataType>::_Delete( _BTREENODE<keyType, dataType> *pRootNode, \
											_DATATYPE<keyType, dataType> cDataType )
{
	if( pRootNode==NULL )
		return NotFound;
		
	int 							i,j,pivot,n=pRootNode->iNumberKey;
	const int 						nMin=( MEMBERPOINTER-1 )/2;
	//const int 						nMin=( stPtrIndexInfo.get_nMemberPointer()-1 )/2;
	int								compare;
	_DATATYPE<keyType, dataType> 	*k=pRootNode->kIndexKey;    // kIndexKey[ i ]를 r->kIndexKey[ i ]로 사용하기 위하여.  
	status 							code;
	_BTREENODE<keyType, dataType>	*pL, *pR;
	_BTREENODE<keyType, dataType> 	**p=pRootNode->pBTREENODE;    // pBTREENODE[ i ]를 r->pBTREENODE[ i ]로 사용하기 위하여.  
	
	i=_NodeSearch( cDataType,k,n );
	
	// 현재 노드가 leaf node일때  
	if( p[ 0 ]==NULL )
	{
		if( BTREE_DEBUG )
		{
			*LogM << LogM->ErrorCode(0) << "[CBtree][_Delete] p[ 0 ]==NULL " << endl;
		}
		/*
		if( i==n || cDataType.iDataKey<k[ i ].iDataKey )
			return NotFound;
		*/
		//2001. 03. 08
		compare=memcmp( &cDataType.dtype, &k[i].dtype, sizeof(keyType));
   		if (i == n || compare < 0)
   		{
   			if( BTREE_DEBUG )
			{
	   			*LogM << LogM->ErrorCode(0) << "[CBtree][_Delete] NotFound" << endl;
	   		}
   			return NotFound;
   		}
		for( j=i+1; j< n; j++ )
		{
			// 뒤의 값을 앞으로 땡겨준다.   
			// 5 6 7 8 에서 7을 삭제시   
			// 5 6 8 로 된다.  
		//	k[j-1]=k[j];
		// 2001. 03. 08
			memcpy( &k[j-1], &k[j], sizeof( _DATATYPE<keyType, dataType> ) ); 
			p[j]=p[j+1];
		}
		// 키값의 갯수를 하나 감소시킨후,  
		// 루트일때는 1개, 그밖의 노드들은 최소한 mMin(2)     
		// 이상 가지고 있어야 하므로  비교해서  갯수가    
		// 많거나 같으면 Success 적으면 Underflow를 반환한다.
		return --pRootNode->iNumberKey >= ( pRootNode==pBtreeRoot ? 1 : nMin ) ? Success : Underflow;
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
	     
	//if( i<n && cDataType.iDataKey==k[ i ].iDataKey )
	// 2001. 03. 08
	compare=memcmp( &cDataType.dtype, &k[i].dtype, sizeof(keyType));
    if (i < n && compare == 0)
	{
		_BTREENODE<keyType, dataType> *q=p[ i ], *q1; 
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
		//k[ i ]=q->kIndexKey[ nq-1 ];
		// 2001. 03. 08
		memcpy( &k[i], &q->kIndexKey[nq-1], sizeof( _DATATYPE<keyType, dataType> ) );
		q->kIndexKey[ nq-1 ]=cDataType;
	}
	// 삭제하고자 하는 값이 현재노드의 자식노드에 있을때
	code = _Delete( p[ i ],cDataType );
	if(code != Underflow)
	{
		*LogM << LogM->ErrorCode(0) << "[CBtree][_Delete]code != Underflow" << endl;
		return code;
	}
        
    // 5              -> root node
	//    2 3 4       -> left leaf node
 	//    7 9         -> right leaf node
	// 에서 9를 삭제하고자 할때       
	// 좌측형제노드에서 가장 우측값이 루트로 올라가고     
	// 루트값이 우측노드의 가장 왼쪽값으로 온다.  
	// int값의 비교는 memcmp를 사용하지 않아도 된다. 2001. 03. 08  
	
	if( i>0 && p[ i-1 ]->iNumberKey > nMin )
	{
		pivot=i-1;
		pL=p[ pivot ];
		pR=p[ i ];
		// 우측노드의 값을 오른쪽으로 한칸씩 밀어준다.    
		pR->pBTREENODE[ pR->iNumberKey+1 ]=pR->pBTREENODE[ pR->iNumberKey ];
		for( j=pR->iNumberKey; j>0; j-- )
		{
			//pR->kIndexKey[ j ]=pR->kIndexKey[ j-1 ];
			// 2001. 03. 08
			memcpy( &pR->kIndexKey[j], &pR->kIndexKey[j-1], sizeof( _DATATYPE<keyType, dataType> ) );
			pR->pBTREENODE[ j ]=pR->pBTREENODE[ j-1 ];
		}
		pR->iNumberKey++;       // 우측노드의 키수를 증가시킨다.    
		// 루트값을 우측노드의 처음으로 넣어준다.    
		//pR->kIndexKey[ 0 ]=k[ pivot ];    
		// 2001. 03. 08
		memcpy( &pR->kIndexKey[0], &k[pivot], sizeof( _DATATYPE<keyType, dataType> ) );
		pR->pBTREENODE[ 0 ]=pL->pBTREENODE[ pL->iNumberKey ];
		// 루트값에 좌측노드의 가장 우측값을 넣어준다.     
		//k[ pivot ]=pL->kIndexKey[ --pL->iNumberKey ]; 
		// 2001. 03. 08
		memcpy( &k[pivot], &pL->kIndexKey[--pL->iNumberKey], sizeof( _DATATYPE<keyType, dataType> ) );
		return Success;
	}
	 // 5               -> root node
	//    2 3           -> left leaf node
 	//    6 7 9         -> right leaf node
	// 에서 3를 삭제하고자 할때       
	// 우측형제노드에서 가장 좌측값이 루트로 올라가고     
	// 루트값이 좌측노드의 가장 우측값으로 온다.
	// int값의 비교는 memcmp를 사용하지 않아도 된다. 2001. 03. 08     
	if( i<n && p[ i+1 ]->iNumberKey > nMin )
	{
		pivot=i;
		pL=p[ pivot ];
		pR=p[ pivot+1 ];
		// 루트값이 좌측노드의 가장 오른쪽으로   
		//pL->kIndexKey[ pL->iNumberKey ]=k[ pivot ];
		// 2001. 03. 08
		memcpy( &pL->kIndexKey[pL->iNumberKey], &k[pivot], sizeof( _DATATYPE<keyType, dataType> ) );
		pL->pBTREENODE[ pL->iNumberKey+1 ]=pR->pBTREENODE[ 0 ];
		//k[ pivot ]=pR->kIndexKey[ 0 ];
		// 2001. 03. 08
		memcpy( &k[pivot], &pR->kIndexKey[0], sizeof( _DATATYPE<keyType,dataType> ) );
		pL->iNumberKey++;
		pR->iNumberKey--;
		for( j=0; j<pR->iNumberKey; j++ )
		{
			//pR->kIndexKey[ j ]=pR->kIndexKey[ j+1 ];
			// 2001. 03. 08
			memcpy( &pR->kIndexKey[j], &pR->kIndexKey[j+1], sizeof( _DATATYPE<keyType,dataType> ) );
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
	//pL->kIndexKey[ pL->iNumberKey ]=k[ pivot ];
	// 2001. 03. 08
	 memcpy( &pL->kIndexKey[pL->iNumberKey], &k[pivot], sizeof( _DATATYPE<keyType,dataType> ) );
	// 우측 노드의 값들을 좌측노드의 뒷부분에 붙여준다.     
	pL->pBTREENODE[ pL->iNumberKey+1 ]=pR->pBTREENODE[ 0 ];
	for( j=0; j<pR->iNumberKey; j++ )
	{
		//pL->kIndexKey[ pL->iNumberKey+1+j ]=pR->kIndexKey[ j ];
		// 2001. 03. 08
		memcpy( &pL->kIndexKey[pL->iNumberKey + 1 + j], &pR->kIndexKey[j], sizeof( _DATATYPE<keyType,dataType> ) );
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
		//k[ j-1 ]=k[ j ]; 
		// 2001. 03. 08
		memcpy( &k[j-1], &k[j], sizeof( _DATATYPE<keyType,dataType> ) );
		p[ j ]=p[ j+1 ];
	}
	return --pRootNode->iNumberKey >= ( pRootNode==pBtreeRoot ? 1 : nMin ) ? Success : Underflow;
}

#endif


