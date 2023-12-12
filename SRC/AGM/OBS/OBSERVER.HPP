/******************************************************************
* 파 일 명     : Observer.hpp 			     					  *
* 기   능                                                          *
* : CObserver class정의.                                           *
* 작 성 자     : Han Guen Hee                    				  *
* first data  : 2000. 9. 20        	     						  *
* last updata : 2000. 12. 21        		 					  *
* 관 련 화 일  : observer.cpp                						  *
* program history                        						  *
*                         	        	 				 		  *
* bug fix                        	     						  *
*                         	        	 						  *
******************************************************************/
#ifndef	OBSERVER_HPP
#define OBSERVER_HPP

#include <iostream.h>
#include <time.h>
#include <stdlib.h>
#include <Btree.hpp>
#include <TempMsgQ_file.hpp>
#include <SendLogManage.hpp>
#include <fileSearchC.hpp>
#include <PrintUdr_new.hpp>

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

// Aggregator To Collector
MessageQueue<UdrFormat>* MqUdr;
// PMS MQ
MessageQueue<MsgPmsStatus>* mqPMS;

//	Log Manager
SendLogManage*				ObserverLogM;
class CObserver
{
	public:
		CObserver();
		CObserver(int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark );
		~CObserver();
	 	void Observer( void );
	private:
        ObsFlagDate*			 pObsFlagDate;   
        SharedMemory*         	 shmData; 
        HandleOfAllocIndex_New*  pAllocDataIndexC;
        UdrFormat*  			 stPtrDataIndexInfo;
		CBtree*					 pBtree;
		//fstream					 fObserver;
		fstream 				 fInOut;
		_BTREENODE*				 pBtreeNode;
		unsigned int			 unFlag;			//in 0:MDB에서 삭제 1:END생성 2:all
		unsigned int 			 unInterval;		// 처리 간격
		unsigned int			 unApplicationTime; //적용시간. 현재시간에서 빼주는 시간.
	 	time_t					 tStartTime;
	 	time_t					 tProcessingTime;
	 //	time_t					 tMakeTime;
		int						 nProcessingCount;
		CPrintUdr				 PrintUdr;
		int 					 _DeleteMdb( _BTREENODE* pBtreeNode, time_t tCompareTime );
		int  					 _StopProcessMdb( _BTREENODE* pBtreeNode, time_t tCompareTime );
		int  					 _AllProcessing( _BTREENODE* pBtreeNode, time_t tCompareTime );
		
};


#endif



