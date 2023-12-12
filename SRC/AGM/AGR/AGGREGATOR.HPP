/******************************************************************
* 파 일 명     : Aggregator.hpp 			     					  *
* 기   능                                                          *
* : Aggregation을 하기 위한 program                                 *
* 작 성 자     : Han Guen Hee                    				  *
* first data  : 2000. 9. 29        	     						  *
* last updata : 2001. 01. 03        		 					  *
* 관 련 화 일  : Aggregator.cpp             						  *
* program history                        						  *
*                        	        	 				 		  *
* bug fix                        	     						  *
*                         	        	 						  *
******************************************************************/

#ifndef AGGREGATOR_HPP
#define AGGREGATOR_HPP

#include<iomanip.h>
#include<fstream.h>
#include<stdlib.h>
#include<stdio.h>
#include<TransactOfAggregate.hpp>
#include<PrintUdr_new.hpp>
#include <TempMsgQ_file.hpp>
#include <time.hpp>
#include <LimitMan.hpp>

#define AGGREGATOR_DEBUG 1
char 	PATH_NAME_AGGREGATOR[80] 	= "/SVC#/DATA/AggShm#.cfg";
//char 	PATH_NAME_AGGREGATOR_LOG[80] 	= "/SVC#/LOG/Aggregation#.txt";

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
// AGG -> Scp-if(S)
//MessageQueue<SCPFormatAgg> *mqSCP;


class CAggregator 
{
	public:
		CAggregator();
		CAggregator( int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark );
		~CAggregator();
		void					Aggregator( UdrFormat* pUdrFormat );
		
	private:
		CTransactOfAggregate* 	TransactOfAggregate;
		CBtree*				pBtree;
	//	fstream				fAggregator;    // Aggregation된 udr
		CPrintUdr			PrintData;
		CLimitMan*          TempLimitMan;
		_DATATYPE			stDataType;
		UdrFormat*			pSearchUdrFormat;
		int					tStartTime;
	//	int					tEndTime;
		unsigned int 		nUdrCount;
		unsigned int 		nSumAggregationUdr;
	//	DateTime			dateTime; // for time log add jkjung
		
		int 				InsertAggregator( UdrFormat*  pUdrFormat );
		int 				DeleteAggregator( UdrFormat*  pUdrFormat );
		int					UpdateAggregator( UdrFormat*  pUdrFormat );
		int					_StartProcess( UdrFormat* pUdrFormat );
		int					_InterimProcess( UdrFormat* pUdrFormat );
		int					_StopProcess( UdrFormat* pUdrFormat );
		void				_RemainsItem( UdrFormat* pSearchUdrFormat, UdrFormat* pUdrFormat );
		int					AccessRequestResult( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat );
		int					InterimAccountRequestResult( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat );
		
};

#endif




