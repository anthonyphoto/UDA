/******************************************************************
* 파 일 명     : Aggregator.hpp 			     					
* 기   능                                                        
* : Aggregation을 하기 위한 program                              
* 작 성 자     : Han Guen Hee( jameshans@hanmail.net )              			
* first data  : 2000. 9. 29        	     						 
* last updata : 2001. 05. 21        		 					 
* 관 련 화 일  : Aggregator.cpp             						
* program history                        						 
* Version 	  : 1.1( 2001. 05. 21 )                       	        	 				 		 
******************************************************************/

#ifndef AGGREGATOR_HPP
#define AGGREGATOR_HPP

#include <iomanip.h>
#include <fstream.h>
#include <stdlib.h>
#include <stdio.h>
#include <MdbMan.hpp>
#include <PrintUdr_new.hpp>
#include <TempMsgQ_file.hpp>
#include <time.hpp>

#define AGGREGATOR_DEBUG 1
#define LOG_ON			 0

static int	SystemId;
static int	ServiceId;
static int	GroupId;
static int	ProcessType;
static int 	PserialNo;
static int	PreMark;

// filter To Aggragator
MessageQueue<UdrFormat> *ReadAggregator;
// Aggregator To Collector
MessageQueue<UdrFormat> *MqUdr;
// PMS MQ
MessageQueue<MsgPmsStatus> *mqPMS;

//	Log Manager
SendLogManage*				LogM;

class CAggregator 
{
	public:
		CAggregator( int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark );
		~CAggregator();
		void					Aggregator( UdrFormat* pUdrFormat );
		
	private:
		MdbMan<KeyDef,UdrFormat>*		pMdbMan;
		CBtree<KeyDef,UdrFormat>*		pBtree;
		CPrintUdr						PrintData;
		_DATATYPE<KeyDef,UdrFormat>		stDataType;
		UdrFormat*						pSearchUdrFormat;
		int								tStartTime;
		unsigned int 					nUdrCount;
		unsigned int 					nSumAggregationUdr;
		
		int 							InsertAggregator( UdrFormat*  pUdrFormat );
		int 							DeleteAggregator( UdrFormat*  pUdrFormat );
		int								UpdateAggregator( UdrFormat*  pUdrFormat );
		int								_StartProcess( UdrFormat* pUdrFormat );
		int								_InterimProcess( UdrFormat* pUdrFormat );
		int								_StopProcess( UdrFormat* pUdrFormat );
		void							_RemainsItem( UdrFormat* pSearchUdrFormat, UdrFormat* pUdrFormat );
		
};

#endif




