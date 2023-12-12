/******************************************************************
* 파 일 명     :  DataType.hpp		  			 				  
* 기   능                                                         
* :  Btree안에서 실제 key class와 해당 메모리의 pointer를 저장하는
*    Class. 이 class가 Btree node안의 Key 값이 된다.				
*                                                 			   	  
* 작 성 자     : Han Guen Hee( jameshans@hanmail.net )                     				  
* first data  : 2001. 02. 06      	     						  
* last updata : 2001. 05. 21
* Version	  : 1.1( 2001. 05. 21 )                 		 		 				  
* program history :                       						  
******************************************************************/
#ifndef DATATYPE_HPP
#define DATATYPE_HPP
#include <SendLogManage.hpp>                    // File Log
#include <KeyDef.hpp>

extern SendLogManage*    LogM;   // 로그매니저 (Main Class에서 정의)

template <class keyType, class dataType> 
class _DATATYPE
{
	public:
		_DATATYPE() { init(); }
		~_DATATYPE(){ ; }
		
		void					init( void );
		// Accessor
		keyType*				get_keyType( void )    		 { return  &dtype;  }
		dataType*				get_dataType( void )    	 { return  pdataType;  }
		_DATATYPE*				get_DATATYPE( void ) 	 	 { return  this; }
		void	   				set_keyType( keyType *pValue );
		void	   				set_dataType( dataType *pValue );
		void	   				set_DATATYPE( _DATATYPE* pValue );
		void	 				print_all( void );
								
		keyType 				dtype;	
		dataType*				pdataType;
	protected:
		//friend class			CBtree(keyType, dataType);
};

template <class keyType, class dataType> 
void _DATATYPE<keyType, dataType>::init( void )
{
	memset( &dtype, NULL, sizeof( keyType ) );
	pdataType = NULL;
}
template <class keyType, class dataType> 
void _DATATYPE<keyType, dataType>::set_keyType( keyType *pValue )
{	
	memset( &dtype, NULL, sizeof( keyType ) );
	memcpy( &dtype, pValue, sizeof( keyType ) ); 
}
template <class keyType, class dataType> 
void _DATATYPE<keyType, dataType>::set_dataType( dataType *pValue )
{
	pdataType = pValue; 
}
template <class keyType, class dataType> 
void _DATATYPE<keyType, dataType>::set_DATATYPE( _DATATYPE* pValue )
{	
	init();
	memcpy( this, pValue, sizeof( *this ) ); 
}
template <class keyType, class dataType> 
void _DATATYPE<keyType, dataType>::print_all( void ) 
{
	*LogM << LogM->ErrorCode(0) << "=======_DATATYPE  value======" << endl;
	this->dtype.print_all();
//	*LogM << LogM->ErrorCode(0) << "unMsID : [" << this->dtype.get_unMsID() << "]" << endl;
//	*LogM << LogM->ErrorCode(0) << "strAccSessionID : [" << this->dtype.get_strAccSessionID() << "]" << endl;
//	*LogM << LogM->ErrorCode(0) << "strCorrID : [" << this->dtype.get_strCorrID() << "]" << endl;
	*LogM << LogM->ErrorCode(0) << "pdataType :[" << (unsigned int)pdataType << "]" << endl;
	*LogM << LogM->ErrorCode(0) << "=============================" << endl;
}
#endif


