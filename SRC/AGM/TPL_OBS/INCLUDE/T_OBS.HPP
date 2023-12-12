/******************************************************************
* 파 일 명     : Observer.hpp 			     					  
* 기   능                                                          
* : CObserver class정의.                                           
* 작 성 자     : Han Guen Hee( jameshans@hanmail.net )                    				  
* first data  : 2000. 9. 20        	     						  
* last updata : 2001. 05. 21  
* 관 련 화 일  : observer.cpp                						 
* Version     : 1.1( 2001. 05. 21 )
* program history                        						  
******************************************************************/
#ifndef	OBSERVER_HPP
#define OBSERVER_HPP

#include <iostream.h>
#include <time.h>
#include <stdlib.h>
#include <Tpl_Btree.hpp>
#include <TempMsgQ_file.hpp>
#include <SendLogManage.hpp>
#include <fileSearchC.hpp>
#include <PrintUdr_new.hpp>

#define DAY				1440
#define OBSERVER_DEBUG	1

const   int SIZE_UDRFORMAT     			= sizeof( UdrFormat );
char 	PATH_NAME_AGGREGATOR[80] 		= "/SVC#/DATA/AggShm#.cfg";
char 	PATH_NAME_OBSERVER[80] 			= "/SVC#/LOG/Observer.txt";
char 	PATH_NAME_OBSERVER_FLAG[80] 	= "/SVC#/DATA/OBS_FlagDate.cfg";
char 	PATH_NAME_AGGREGATOR_MDB[80] 	= "/SVC#/MB/Agg#.mb";

static int	SystemId;
static int	ServiceId;
static int	GroupId;
static int	ProcessType;
static int 	PserialNo;
static int	PreMark;
static int	Flag;

// Aggregator To Collector
MessageQueue<UdrFormat>* MqUdr;
// PMS MQ
MessageQueue<MsgPmsStatus>* mqPMS;

//	Log Manager
SendLogManage*				LogM;
class CObserver
{
	public:
		CObserver();
		CObserver(int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark, int nFlag );
		~CObserver();
	 	void Observer( void );
	private:
        ObsFlagDate*						pObsFlagDate;   
        ShmMemory*         					shmData; 
        HandleOfAllocIndex*  				pAllocDataIndexC;
        UdrFormat*  						stPtrDataIndexInfo;
		CBtree<KeyDef,UdrFormat>*			pBtree;
		fstream 				 			fInOut;
		_BTREENODE<KeyDef,UdrFormat>*		pBtreeNode;
		unsigned int			 			unFlag;			//in 0:MDB에서 삭제 1:END생성 2:all
		unsigned int 			 			unInterval;		// 처리 간격
		unsigned int						unApplicationTime; //적용시간. 현재시간에서 빼주는 시간.
	 	time_t								tStartTime;
	 	time_t								tProcessingTime;
		int						 			nProcessingCount;
		CPrintUdr				 			PrintUdr;
		
		int 					 			_DeleteMdb( _BTREENODE<KeyDef,UdrFormat>* pBtreeNode, time_t tCompareTime );
		int  					 			_StopProcessMdb( _BTREENODE<KeyDef,UdrFormat>* pBtreeNode, time_t tCompareTime );
		int  					 			_StopProcessMdb2( _BTREENODE<KeyDef,UdrFormat>* pBtreeNode, time_t tCompareTime );
		int  					 			_StopProcessMdb3( _BTREENODE<KeyDef,UdrFormat>* pBtreeNode, time_t tCompareTime );
		int 					 			_FindDeleteUdr( _BTREENODE<KeyDef,UdrFormat>* pBtreeNode, time_t tCompareTime ); // 2001. 02. 16
		
};


#endif



