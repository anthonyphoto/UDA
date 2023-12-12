/*********************************************
* 파 일 명  
	: MemView.hpp
* 기 능 
	: Memory DB의 내용을 보여주는 Program      
* writer 
	: Han Guen Hee ( jameshans@hanmail.net )                  
* first data  
	: 2000. 9. 29        	     						  
* last updata 
	: 2001. 05. 21  
* 관 련 화 일  
	: MemView.cpp  
* Version
	: 1.1 ( 2001. 05. 21 )                    		 
***********************************************/
#ifndef MEMVIEW_HPP
#define MeMVIEW_HPP

#include<iostream.h>
#include <MdbMan.hpp>
#include<iomanip.h>
#include<UDR.hpp>
#include <SendLogManage.hpp>

const   int SIZE_UDRFORMAT      = sizeof( UdrFormat );

//	Log Manager
SendLogManage*				LogM;

class CMemView
{
	public:
		CMemView( int nSystemId, int nServiceID, int nSerial );
		~CMemView();
		void							MemView( void );
		void 							ProcessingPrint( UdrFormat* stDataNode );
		void 							NotProcessingPrint( UdrFormat* stDataNode );
	private:
		 UdrFormat*             		pUdrFormat;
		  CBtree<KeyDef,UdrFormat>*		pBtree;
		 _BTREENODE<KeyDef,UdrFormat>*	pBtreeNode;
		 MdbMan<KeyDef,UdrFormat>*		pMdbMan;
		 int 					 		_MemView( _BTREENODE<KeyDef,UdrFormat>* pBtreeNode );
		 int					 		nTotalCount;
		 int					 		nProcessingCount;
		 int					 		nNotProcessingCount;
		
};


#endif



