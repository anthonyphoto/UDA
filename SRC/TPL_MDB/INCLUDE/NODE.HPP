/******************************************************************
* �� �� ��     :  node.hpp			  			 				  *
* ��   ��                                                         *
* :  Btree�ȿ��� ���� �ϳ���  node�� �����ϴ� class.                *
*                                                 			   	  *
* �� �� ��     : Han Guen Hee                    				  *
* first data  : 2001. 02. 06      	     						  *
* last updata :                  		 		 				  *
* �� �� ȭ ��  : 					        					  *
* program history :                       						  *
* class argument :         	        	 						  *
* bug fix :                       	     						  *
*                         	        	 						  *
******************************************************************/
#ifndef NODE_HPP
#define NODE_HPP

#include <DataType.hpp>
#include <CommCfgFile.hpp>
#include <SendLogManage.hpp>                    // File Log

#define BN_DEBUG	0
// Btree Node�� Pointer����( child Node�� ����Ű�� ������. )
//const unsigned int		MEMBERPOINTER   = 	4;	// Order of B-tree: M link fields in each node
#define	MEMBERPOINTER	4
// UDR����.( shared memory size�� ����ϱ� ���� �ʿ�. )
//const unsigned int      MAXKEYMEMBER	= 100;
#define MAXKEYMEMBER	1000
// Btree Node ����.
//unsigned int 		MAXBTREEMEMBER	= ( MAXKEYMEMBER/( ( MEMBERPOINTER-1 )/2 ) ) + 1;
#define MAXBTREEMEMBER 	( MAXKEYMEMBER/( ( MEMBERPOINTER-1 )/2 ) ) + 1
extern SendLogManage*    LogM;   // �α׸Ŵ��� (Main Class���� ����)

template <class keyType, class dataType> 
class _BTREENODE : public _DATATYPE<keyType, dataType>
{
	public:
		_BTREENODE();
		~_BTREENODE(){;}
		
   		void								init( void );
   		void	 							print_node( void ) const;
		// Accessor									
		int									get_iIndex( void )   	 	const { return  iIndex;  }
		int									get_iNumberKey( void )    	const { return  iNumberKey;  }
	//	int									get_nMemberPointer( void ) 	const { return  nMemberPointer;  }
	//	int									get_nMaxKeyMember( void )   const { return  nMaxKeyMember;  }
	//	int									get_nMaxBtreeMember( void ) const { return  nMaxBtreeMember;  }
		
		void	   							set_iIndex( int nValue )			{ iIndex = nValue;	}
		void	   							set_iNumberKey( int nValue )		{ iNumberKey = nValue; }
	//	void	   							set_nMemberPointer( int nValue )	{ nMemberPointer = nValue;	}
	//	void	   							set_nMaxKeyMember( int nValue )		{ nMaxKeyMember = nValue; }
	//	void	   							set_nMaxBtreeMember( int nValue )	{ nMaxBtreeMember = nValue;	}
		
		int 								iIndex;        // find node_index with return address
  		int 								iNumberKey;        // Number of items stored in a node (n < M)
   		_DATATYPE<keyType, dataType> 		kIndexKey[MEMBERPOINTER-1];   // Data items (only the first n in use)
   		_BTREENODE<keyType, dataType> 		*pBTREENODE[MEMBERPOINTER];    // Pointers to other nodes (n+1 in use)
   	//	_DATATYPE<keyType, dataType>* 		kIndexKey;
   	//	_BTREENODE<keyType, dataType>**		pBTREENODE;
   		
   	//	CommCfgFile							cCommCfgFile;	// config file�� �о���� ���� ������.
   	//	int									nMemberPointer;
   	//	int									nMaxKeyMember;
   	//	int									nMaxBtreeMember;
};

template <class keyType, class dataType> 
_BTREENODE<keyType, dataType>::_BTREENODE()
{
//	cCommCfgFile.read_CommCfgFile();
//	nMemberPointer = MEMBERPOINTER;
//	nMaxKeyMember = MAXKEYMEMBER;
//	nMaxBtreeMember = ( nMaxKeyMember/( ( nMemberPointer-1 )/2 ) ) + 1;
	
//	kIndexKey = new _DATATYPE<keyType, dataType>[nMemberPointer-1];
//	pBTREENODE = new _BTREENODE<keyType, dataType>*[nMemberPointer];
	if( BN_DEBUG )
	{
//		*LogM << LogM->ErrorCode(0) << "=========config value========" << endl;
//		*LogM << LogM->ErrorCode(0) << "nMemberPointer : [" << nMemberPointer << "]" << endl;
//		*LogM << LogM->ErrorCode(0) << "nMaxKeyMember : [" << nMaxKeyMember << "]" << endl;
//		*LogM << LogM->ErrorCode(0) << "nMaxBtreeMember : [" << nMaxBtreeMember << "]" << endl;
//		*LogM << LogM->ErrorCode(0) << "=============================" << endl;
	}
	init();
}
/*
template <class keyType, class dataType> 
_BTREENODE<keyType, dataType>::~_BTREENODE()
{
	delete [] kIndexKey;
	kIndexKey = 0;
	delete [] pBTREENODE;
	pBTREENODE = 0;
}
*/
template <class keyType, class dataType> 
void _BTREENODE<keyType, dataType>::init( void )
{
	// �̰��� ���� ����� �ʱ�ȭ�� �ǳ�?
	iNumberKey = 0;
	iIndex = 0;

	memset( kIndexKey, NULL, sizeof( kIndexKey ) ) ;
	memset( pBTREENODE, NULL, sizeof( pBTREENODE ) ) ;
}
template <class keyType, class dataType> 
void _BTREENODE<keyType, dataType>::print_node( void ) const 
{
	*LogM << LogM->ErrorCode(0) << "=======_BTREENODE value======" << endl;
	*LogM << LogM->ErrorCode(0) << "iIndex : [" << iIndex << "]" << endl;
	*LogM << LogM->ErrorCode(0) << "iNumberKey : [" << iNumberKey << "]" << endl;
	*LogM << LogM->ErrorCode(0) << "kIndexKey : [" << (unsigned int)kIndexKey << "]" << endl;
	*LogM << LogM->ErrorCode(0) << "pBTREENODE : [" << (unsigned int)pBTREENODE << "]" << endl;
	*LogM << LogM->ErrorCode(0) << "=============================" << endl;
}

// shared memory�� Btree�� root�� ����� �������μ� 
// process�� �׾��ٰ� �ٽ� ����Ǿ�����, �����ϰ� �Ȱ��� �����ϵ��� �Ѵ�.
template <class keyType, class dataType> 
class _BTREEINFO
{
	public:
		_BTREEINFO() { init(); }
		~_BTREEINFO() { ; }
  		int 								nFlag;           // ó������ �ٽ� �����ߴ��� �����ϱ� ���� flag
		_BTREENODE<keyType, dataType>		*pBtreeRoot;     // Btree�� Root Address�� ������ �ִ� ����.
   		void								init( void )
   											{
   												nFlag = 0;
												pBtreeRoot = NULL;
											}
};

#endif

