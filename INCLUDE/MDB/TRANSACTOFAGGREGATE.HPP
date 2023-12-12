/************************************************
* This file is defines transaction of aggregator 
* file name : TransactOfAggregate.hpp 
* writer : Han Guen Hee
* program history

* bug fix history

* last update : 2000. 9. 29
************************************************/
#ifndef TRANSACTOFAGGREGATE_HPP
#define TRANSACTOFAGGREGATE_HPP

#include <fstream.h>
#include <MDB/Btree.hpp>
#include <stdlib.h>
#include <SendLogManage.hpp>

#define 	DEBUG	0
const   int                 SIZE_UDRFORMAT      = sizeof( UdrFormat );
		char 	PATH_NAME_AGGREGATOR_MDB[80] 	= "/SVC#/MB/Agg#.mb";

class CTransactOfAggregate
{
	public:
		CTransactOfAggregate();
		CTransactOfAggregate(char* Data_Name_Shm, char Data_Id_Shm, int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark);
		~CTransactOfAggregate();
		// index shared memory 객체를 가리키기 위한 포인터 
		HandleOfAllocIndex_New*		pAllocDataIndexC;
		// btree shared memory 객체를 가리키기 위한 포인터 
		CBtree*					pBtree;
		// data shared memory 객체를 가리키기 위한 포인터  
		SharedMemory*		shmData;
		void	 			selectshm( );
		int 				deleteshm( UdrFormat*	pUdrFormat );
		void				readshm( );
		int 				updateshm( UdrFormat*	pUdrFormat );
		int 				insertshm( UdrFormat*	pUdrFormat );
		CBtree*				get_BtreePointer() { return pBtree; }
		UdrFormat*			getUdrPointer() { return stPtrDataIndexInfo; }

	private:
	    // file descriptor
		fstream 			fInOut;
		// data shared memory의 base address를 가리키기 위한 포인터. 
		UdrFormat*  		stPtrDataIndexInfo;
		int 		 		_TransactionInsert( UdrFormat*	stDataNode );
		int		 			_TransactionUpdate( UdrFormat*	stDataNode );
		int		 			_TransactionDelete( unsigned int 		iDeleteKey );
		UdrFormat*			_TransactionSelect( unsigned int 		iSelectKey );

};

#endif



