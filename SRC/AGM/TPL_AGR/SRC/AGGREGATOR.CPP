/******************************************************************
* 파 일 명     : Aggregator.cpp 			     				
* 기   능                                                       
* : Aggregation을 하기 위한 program				                
* 작 성 자     : Han Guen Hee( jameshans@hanmail.net )                    				
* first data  : 2000. 9. 29        	     						
* last updata : 2001. 05. 21        		 					
* 관 련 화 일  : Aggregator.hpp              					
* Version 	  : 1.1( 2001. 05. 21 )              
* program history                        						  
*   Aggregation Rule 수정(Interim) -> 2001. 01. 03		 		  
*   Aggregation된 UDR을 File에 써주는 부분을 주석처리.-> 2001. 1. 4
*   응답된 한도금액이 0이하일때 Service STOP요청 -> 2001. 1. 11    
*   Aggregation Rule 수정(Stop) -> 2001. 01. 12			 		  
*	비교구문수정(AccountSessionID+CorrelationID) -> 2001. 03. 12 (version 1.1)  
*   error 통계 추가 -> 2001. 03. 15								 
*	hand-off 무시(start) -> 2001. 03. 15
*	differ sassion ID 처리하는 부분을 주석처리 -> 2001. 05. 21
*   완료된 UDR은 남겨두지 않고, 삭제해준다.  -> 2001. 05. 21
*   완료된 UDR을 다시 삭제하지 않고 남겨둔다.  -> 2001. 05. 26   
*   로그정리  -> 2001. 06. 04   
*	Refactoring -> 2001. 06. 08     
*	한도관리부분 제거( 한도관리를 독립 프로세스로 분리) -> 2001. 06. 29   
*   로그정리( 에러나 특이사항만 로그를 남김 )  -> 2001. 07. 20          	        	 						 
******************************************************************/
#include <Aggregator.hpp>

void sigCapture(int sigNo );
void WritePMSMQ(int sigNo);

//===============================================================
// Constructor Method											=
//  : Aggregation에 필요한 CTransactOfAggregate class 선언   		=
// argument : 프로세스 형상구분 6가지								=
// return value : None											=
//																=
//===============================================================
CAggregator::CAggregator( int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark )
{
	char   cId='A';
	char   COMMAND_TOUCH[100];
	char 	strDataFile[100];
    char 	strBackUpFile[100];
    char 	strCfgFile[100];
    
    *LogM << LogM->ErrorCode(0) << "##### [" << nPserialNo << "]"
			<< " AGGREGATOR START ######" << endl;
			
    memset( &strDataFile, NULL, sizeof(strDataFile) );
    memset( &strBackUpFile, NULL, sizeof(strBackUpFile) );
    memset( &strCfgFile, NULL, sizeof(strCfgFile) );
    memset( &COMMAND_TOUCH, NULL, sizeof(COMMAND_TOUCH) );
    
    // FLT -> AGR
    sprintf(strDataFile, "/SVC%d/DATA/AggShm%d.cfg", nServiceId,nPserialNo);
    sprintf(strBackUpFile, "/SVC%d/MB/Agg%d.mb", nServiceId,nPserialNo);
    sprintf(strCfgFile, "/SVC%d/DATA/CommCfgFile%d.cfg",nServiceId,nPserialNo);
  	
  	sprintf( COMMAND_TOUCH, "touch %s",strDataFile );
	system( COMMAND_TOUCH );
	memset( &COMMAND_TOUCH, NULL, sizeof(COMMAND_TOUCH) );
	sprintf( COMMAND_TOUCH, "touch %s",strBackUpFile );
	system( COMMAND_TOUCH );
	memset( &COMMAND_TOUCH, NULL, sizeof(COMMAND_TOUCH) );
	sprintf( COMMAND_TOUCH, "touch %s",strCfgFile );
	system( COMMAND_TOUCH );
	
	pMdbMan= new MdbMan<KeyDef,UdrFormat>( strDataFile,cId,strBackUpFile,strCfgFile,MEMBERPOINTER,MAXKEYMEMBER );
	// Btree shared memory의 pointer를 얻어온다.
	pBtree = pMdbMan->get_BtreePointer();
	nUdrCount=0;
	nSumAggregationUdr=0;
}
// destructor
CAggregator::~CAggregator()
{
	delete pMdbMan;
	delete mqPMS;
	delete LogM;
}
//===============================================================
// Aggregator Method						   					=
//  : 외부에서 Aggregator를 실행시키는 Method			       		=
//		실제 들어온 data를 비교해서 호출할 메소드를 결정.				=
// argument     :  pUdrFormat -> 처리하기 위해 들어온 UdrFormat	=
// return value : 0-> success									=
//                -1-> fail										=
//																=
//===============================================================
void CAggregator::Aggregator( UdrFormat* pUdrFormat )
{
	KeyDef			cKeyDef;
	int				nReTemp;
	
	memset( &cKeyDef, NULL, sizeof( KeyDef ) );

	cKeyDef.set_unMsID( pUdrFormat->get_ItemA()->get_unMsId() );
	cKeyDef.set_strAccSessionID( pUdrFormat->get_ItemC()->get_strAccountSessionID() );
	cKeyDef.set_strCorrID( pUdrFormat->get_ItemC()->get_strCorrelationID() );
	
	// 들어온 data가 현재 존재하는지 검색한다.
	stDataType.set_keyType( &cKeyDef );
	// Memory Lock개념을 넣어야 한다.
	nReTemp = pBtree->shm_lock(); // MEMORY LOCK
	pSearchUdrFormat = pBtree->selectNode( cKeyDef );

	if( pSearchUdrFormat == NULL )
	{
//		*LogM << LogM->ErrorCode(0) 
//			<< "Aggregator : Data Not Exist started aggregator!!" << endl;	
	}
	
	// 로그 제거 ( 특이사항이나 에러만 로그로 남김 ) ----> 2001. 07. 20
	if( 0 )
	{
		*LogM << LogM->ErrorCode(0) 
			<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
			<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
			<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
			<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
	}
	
	// SLA 통계-일반UDR
	LogM->put_PerformanceData(FLAG_ACC,0,pUdrFormat->get_ItemHead()->get_nUdrFileSeq(),1,0,0);
	if( pUdrFormat->get_ItemHead()->get_nSubsType() == 1 )
		// SLA 통계-한도관리UDR
		LogM->put_PerformanceData(FLAG_ACC,0,pUdrFormat->get_ItemHead()->get_nUdrFileSeq(),0,0,1);
		

	switch( pUdrFormat->get_ItemHead()->get_nUdrType() )
	{
		// INTERIM이 START라면
		case START:
			if( _StartProcess( pUdrFormat ) )
			{
				// SLA 통계-error UDR    error 통계 추가 -> 2001. 03. 15
				LogM->put_PerformanceData(FLAG_ACC,0,pUdrFormat->get_ItemHead()->get_nUdrFileSeq(),0,1,0);
				*LogM << LogM->ErrorCode(4216) 
					<< "Aggregator : _StartProcess Error!!" << endl;	
			}
	 		nUdrCount++;
//	 		*LogM << LogM->ErrorCode(0) 
//					<< "[Total Udr Count]:" << " " << nUdrCount << endl;	
			break;
		case INTERIM:
			if( _InterimProcess( pUdrFormat ) )
			{
				// SLA 통계-error UDR	error 통계 추가 -> 2001. 03. 15
				LogM->put_PerformanceData(FLAG_ACC,0,pUdrFormat->get_ItemHead()->get_nUdrFileSeq(),0,1,0);
				*LogM << LogM->ErrorCode(4217) 
					<< "Aggregator : _InterimProcess Error!!" << endl;	
			}
			 nUdrCount++;
//			 *LogM << LogM->ErrorCode(0) 
//					<< "[Total Udr Count]:" << " " << nUdrCount << endl;	
			break;
		case STOP:
			if( _StopProcess( pUdrFormat ) )
			{
				// SLA 통계-error UDR	error 통계 추가 -> 2001. 03. 15
				LogM->put_PerformanceData(FLAG_ACC,0,pUdrFormat->get_ItemHead()->get_nUdrFileSeq(),0,1,0);
				*LogM << LogM->ErrorCode(4218) 
					<< "Aggregator : _StopProcess Error!!" << endl;	
			}
	 		nUdrCount++;
//	 		*LogM << LogM->ErrorCode(0) 
//					<< "[Total Udr Count]:" << " " << nUdrCount << endl;	
			break;
		default:
			// SLA 통계-Reject UDR
			LogM->put_PerformanceData(FLAG_ACC,0,pUdrFormat->get_ItemHead()->get_nUdrFileSeq(),0,1,0);
			
			// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
			*LogM << LogM->ErrorCode(0) 
				<< "=====================================================" << endl;
			*LogM << LogM->ErrorCode(0) 
				<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
				<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
				<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
				<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
			*LogM << LogM->ErrorCode(0) 
				<< "different Udr Type Error !!" << pUdrFormat->get_ItemHead()->get_nUdrType() <<endl;	
			break;
	}	
	nReTemp = pBtree->shm_release(); // MEMORY LOCK

	// 정수값의 범위를 check.
	if( nUdrCount ==  4294967294 )
		nUdrCount = 0;
	if( nSumAggregationUdr == 4294967294 )
		nSumAggregationUdr=0;
}
//===============================================================
// _StartProcess Method						   					=
//  : START UDR이 들어왔을때 동작하는 Method			       		=
// argument     :  pUdrFormat -> 처리하기 위해 들어온 UdrFormat	=
// return value : 0-> success									=
//                -1-> fail										=
//																=
//===============================================================
int CAggregator::_StartProcess( UdrFormat* pUdrFormat )
{
	// 같은 Data가 존재한다면
	if( pSearchUdrFormat != NULL )
	{
		// 이미 처리가 종료된 상태라면 
		if( pSearchUdrFormat->get_ItemHead()->get_nUdrStatus() == 1)
		{
			if(AGGREGATOR_DEBUG)
			{
				// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
				*LogM << LogM->ErrorCode(0) 
					<< "=====================================================" << endl;
				*LogM << LogM->ErrorCode(0) 
					<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
					<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
					<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
					<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
				*LogM << LogM->ErrorCode(0) 
					<< "START->EXIST->STATUS=1(end)" << endl;	
			}
			return -1;
		} // if
		// 현재 처리하고 있는 상태라면
		else if( pSearchUdrFormat->get_ItemHead()->get_nUdrStatus() == 0 )
		{
			// 만약 새로들어온 시간이 기존에 있던 시간보다 작다면 
			if( ( pUdrFormat->get_ItemG()->get_nEndEventTime() - pSearchUdrFormat->get_ItemG()->get_nEndEventTime() ) <= 0 )
			{
				if(AGGREGATOR_DEBUG)
				{
					// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
					*LogM << LogM->ErrorCode(0) 
						<< "=====================================================" << endl;
					*LogM << LogM->ErrorCode(0) 
						<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
						<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
						<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
						<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
					*LogM << LogM->ErrorCode(0) 
						<< "START->EXIST->STATUS=0->Small E_T" << endl;	
				}
				return -1;
			}
			else
			{
				// 만약 Active_Time이 작다면 
				// 비정상 DATA로 처리한다.
				if( ( pUdrFormat->get_ItemG()->get_nActiveTime() - pSearchUdrFormat->get_ItemG()->get_nActiveTime() ) < 0 )
				{
					if(AGGREGATOR_DEBUG)
					{
						// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
						*LogM << LogM->ErrorCode(0) 
							<< "=====================================================" << endl;
						*LogM << LogM->ErrorCode(0) 
							<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
							<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
							<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
							<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
						*LogM << LogM->ErrorCode(0) 
							<< "START->EXIST->STATUS=0->Big E_T->Small A_T" << endl;	
					}
					return -1;
				}
				// hand-off 무시(start) -> 2001. 03. 15
				else
				{
					// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
					*LogM << LogM->ErrorCode(0) 
						<< "=====================================================" << endl;
					*LogM << LogM->ErrorCode(0) 
						<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
						<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
						<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
						<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
					*LogM << LogM->ErrorCode(0) 
						<< "START->EXIST->STATUS=0->Big E_T->Big A_T:Hand-off 무시" << endl;	
					return -1;
				}	
			} // else							
		}// else if
		// 잘못된 data가 들어온 경우
		else
		{
			// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
			*LogM << LogM->ErrorCode(0) 
				<< "=====================================================" << endl;
			*LogM << LogM->ErrorCode(0) 
				<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
				<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
				<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
				<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
			*LogM << LogM->ErrorCode(0) 
				<< "nUdrStatus data Error!!" << endl;
			return -1;
		}
	} // if
	// 같은 Data가 존재하지 않는다면
	else
	{
		tStartTime = pUdrFormat->get_ItemG()->get_nEndEventTime();
		pUdrFormat->get_ItemG()->put_nStartEventTime( tStartTime );
	
		// 최초의 Data. 정상적으로 insert.
		if( InsertAggregator( pUdrFormat ) )
		{
			// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
			*LogM << LogM->ErrorCode(0) 
				<< "=====================================================" << endl;
			*LogM << LogM->ErrorCode(0) 
				<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
				<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
				<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
				<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
			*LogM << LogM->ErrorCode(4227) 
				<< "START->Not EXIST : InsertAggregator Error" << endl;
			return -1;
		}
		if(LOG_ON)
		{
			*LogM << LogM->ErrorCode(0) 
					<< "START->Not EXIST->Run: InsertAggregator" << endl;
		}
		return 0;
	} // else
}
//===============================================================
// _InterimProcess Method					   					=
//  : INTERIM UDR이 들어왔을때 동작하는 Method			       	=
// argument     :  pUdrFormat -> 처리하기 위해 들어온 UdrFormat	=
// return value : 0-> success									=
//                -1-> fail										=
//																=
//===============================================================
int CAggregator::_InterimProcess( UdrFormat* pUdrFormat )
{
	// update하고자 하는 data가 존재한다면
	if( pSearchUdrFormat != NULL )
	{
		// 이미 처리가 종료된 상태라면 
		if( pSearchUdrFormat->get_ItemHead()->get_nUdrStatus() == 1)
		{
			// 만약 새로들어온 시간이 기존에 있던 시간보다 작다면 
			if( ( pUdrFormat->get_ItemG()->get_nEndEventTime() - pSearchUdrFormat->get_ItemG()->get_nEndEventTime() ) <= 0 )
			{
				if(AGGREGATOR_DEBUG)
				{
					// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
					*LogM << LogM->ErrorCode(0) 
						<< "=====================================================" << endl;
					*LogM << LogM->ErrorCode(0) 
						<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
						<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
						<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
						<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
					*LogM << LogM->ErrorCode(0) 
						<< "INTERIM->EXIST->STATUS=1->Small E_T" << endl;
				}
				return -1;
			}
			else
			{
				// Active time 변화시, Dormant에서 Active로
				// 신규 Aggregation을 시작
				// 만약 Active_Time이 작다면 
				// 비정상 DATA로 처리한다.
				if( ( pUdrFormat->get_ItemG()->get_nActiveTime() - pSearchUdrFormat->get_ItemG()->get_nActiveTime() ) < 0 )
				{
					if(AGGREGATOR_DEBUG)
					{
						// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
						*LogM << LogM->ErrorCode(0) 
							<< "=====================================================" << endl;
						*LogM << LogM->ErrorCode(0) 
							<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
							<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
							<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
							<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
						*LogM << LogM->ErrorCode(0) 
							<< "INTERIM->EXIST->STATUS=1->Big E_T->Small A_T" << endl;
					}
					return -1;
				}
				else
				{
					// 예전 start time을 가져온다.
					// update해준다.
					tStartTime = pSearchUdrFormat->get_ItemG()->get_nStartEventTime();
					pUdrFormat->get_ItemG()->put_nStartEventTime( tStartTime );
					
					// 과거의 정보를 유지해주는 모듈
					_RemainsItem( pSearchUdrFormat, pUdrFormat );
					
					
					if( UpdateAggregator( pUdrFormat ) )
			 		{
			 			// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
						*LogM << LogM->ErrorCode(0) 
							<< "=====================================================" << endl;
						*LogM << LogM->ErrorCode(0) 
							<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
							<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
							<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
							<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
						*LogM << LogM->ErrorCode(4224) 
							<< "INTERIM->EXIST->STATUS=1->Big E_T->Big A_T : UpdateAggregator Error!!!" << endl;
						 return -1;
			 		}

					if(LOG_ON)
					{
						*LogM << LogM->ErrorCode(0) 
							<< "INTERIM->EXIST->STATUS=1->Big E_T->Big A_T->Run: UpdateAggregator" << endl;
					}
					return 0;
				} // else
			} // else						
		} // if
		// 현재 처리하고 있는 상태라면
		else if( pSearchUdrFormat->get_ItemHead()->get_nUdrStatus() == 0 )
		{
			// 만약 새로들어온 시간이 기존에 있던 시간보다 작다면 
			if( ( pUdrFormat->get_ItemG()->get_nEndEventTime() - pSearchUdrFormat->get_ItemG()->get_nEndEventTime() ) < 0 )
			{
				if(AGGREGATOR_DEBUG)
				{
					// 이전 UDR로 간주.	
					// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
					*LogM << LogM->ErrorCode(0)                                            
						<< "=====================================================" << endl;
					*LogM << LogM->ErrorCode(0) 
						<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
						<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
						<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
						<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
					*LogM << LogM->ErrorCode(0) 
						<< "INTERIM->EXIST->STATUS=0->Small E_T" << endl;	
				}
				return -1;
			}
			else
			{
				// 정상처리 기존것과 합산한다.
				// 만약 Active_Time이 작다면 
				// 비정상 DATA로 처리한다.
			 	if( ( pUdrFormat->get_ItemG()->get_nActiveTime() - pSearchUdrFormat->get_ItemG()->get_nActiveTime() ) < 0 )
				{
					if(AGGREGATOR_DEBUG)
					{
						// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
						*LogM << LogM->ErrorCode(0)                                            
							<< "=====================================================" << endl;
						*LogM << LogM->ErrorCode(0) 
							<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
							<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
							<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
							<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
						*LogM << LogM->ErrorCode(0) 
							<< "INTERIM->EXIST->STATUS=0->Big E_T->Small A_T" << endl;	
					}
					return -1;
				}
				else
				{
					// 예전 start time을 새로들어온 UDR의 start time에 넣어주고, 
					// update해준다.
					tStartTime = pSearchUdrFormat->get_ItemG()->get_nStartEventTime();
					pUdrFormat->get_ItemG()->put_nStartEventTime( tStartTime );
					
					// 과거의 정보를 유지해주는 모듈
					_RemainsItem( pSearchUdrFormat, pUdrFormat );
					
					
					if( UpdateAggregator( pUdrFormat ) )
			 		{
			 			// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
						*LogM << LogM->ErrorCode(0)                                            
							<< "=====================================================" << endl;
						*LogM << LogM->ErrorCode(0) 
							<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
							<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
							<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
							<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
						*LogM << LogM->ErrorCode(4224) 
							<< "INTERIM->EXIST->STATUS=0->Big E_T->Big A_T : UpdateAggregator Error!!!" << endl;
						return -1;
			 		}

					if(LOG_ON)
					{
						*LogM << LogM->ErrorCode(0) 
							<< "INTERIM->EXIST->STATUS=0->Big E_T->Big A_T->Run: UpdateAggregator" << endl;
					}
					return 0;
				} // else
			} // else
		} // else if
		// 잘못된 data가 들어온 경우
		else
		{
			// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
			*LogM << LogM->ErrorCode(0)                                            
				<< "=====================================================" << endl;
			*LogM << LogM->ErrorCode(0) 
				<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
				<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
				<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
				<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
			*LogM << LogM->ErrorCode(0) 
				<< "nUdrStatus data Error!!" << endl;
			return -1;
		}
	} // if
	// non-exist data
	else
	{
		// start를 잃어버린 data로 간주한다.
		// 새로 들어온 end time을 start time으로 전환후,
		// insert해준다.
		tStartTime = pUdrFormat->get_ItemG()->get_nEndEventTime();
		tStartTime -= pUdrFormat->get_ItemG()->get_nActiveTime();
		pUdrFormat->get_ItemG()->put_nStartEventTime( tStartTime );
		
		
		if( InsertAggregator( pUdrFormat ) )
		{
			// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
			*LogM << LogM->ErrorCode(0)                                            
				<< "=====================================================" << endl;
			*LogM << LogM->ErrorCode(0) 
				<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
				<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
				<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
				<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
			 *LogM << LogM->ErrorCode(4227) 
					<< "INTERIM->Not EXIST : InsertAggregator Error" << endl;
			return -1;
		}
		if(LOG_ON)
		{
			*LogM << LogM->ErrorCode(0) 
				<< "INTERIM->Not EXIST->Run: InsertAggregator" << endl;
		}
		return 0;
	} // else
}
//===============================================================
// _StopProcess Method						   					=
//  : STOP UDR이 들어왔을때 동작하는 Method				       		=
// argument     :  pUdrFormat -> 처리하기 위해 들어온 UdrFormat	=
// return value : 0-> success									=
//                -1-> fail										=
//																=
//===============================================================
int CAggregator::_StopProcess( UdrFormat* pUdrFormat )
{
	// delete하고자 하는 data가 존재한다면
	if( pSearchUdrFormat != NULL )
	{
		// 이미 처리가 종료된 상태라면 
		if( pSearchUdrFormat->get_ItemHead()->get_nUdrStatus() == 1)
		{
			if(AGGREGATOR_DEBUG)
			{
				// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
				*LogM << LogM->ErrorCode(0)                                            
					<< "=====================================================" << endl;
				*LogM << LogM->ErrorCode(0) 
					<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
					<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
					<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
					<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
				*LogM << LogM->ErrorCode(0) 
					<< "STOP->EXIST->STATUS=1" << endl;	
			}
			return -1;	
		} // if
		// 현재 처리하고 있는 상태라면
		else if( pSearchUdrFormat->get_ItemHead()->get_nUdrStatus() == 0 )
		{
			// 만약 새로들어온 시간이 기존에 있던 시간보다 작다면 
			if( ( pUdrFormat->get_ItemG()->get_nEndEventTime() - pSearchUdrFormat->get_ItemG()->get_nEndEventTime() ) < 0 )
			{
				if(AGGREGATOR_DEBUG)
				{
					// 이전 UDR로 간주.
					// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
					*LogM << LogM->ErrorCode(0)                                            
						<< "=====================================================" << endl;
					*LogM << LogM->ErrorCode(0) 
						<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
						<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
						<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
						<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
					*LogM << LogM->ErrorCode(0) 
						<< "STOP->EXIST->STATUS=0->Small E_T" << endl;			
				}
				return -1;
			}
			// 정상처리 기존 session 종료.
			else
			{
				// 만약 Active_Time이 작다면 
				// 비정상 DATA로 처리한다.
			 	if( ( pUdrFormat->get_ItemG()->get_nActiveTime() - pSearchUdrFormat->get_ItemG()->get_nActiveTime() ) < 0 )
				{
					if(AGGREGATOR_DEBUG)
					{
						// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
						*LogM << LogM->ErrorCode(0)                                            
							<< "=====================================================" << endl;
						*LogM << LogM->ErrorCode(0) 
							<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
							<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
							<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
							<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
						*LogM << LogM->ErrorCode(0) 
							<< "STOP->EXIST->STATUS(0)->Big E_T->Small A_T" << endl;
					}
					return -1;
				}
				else
				{
					// 끝난 data를 표시해주는 flag.
					pUdrFormat->get_ItemHead()->put_nUdrStatus(1);
					tStartTime = pSearchUdrFormat->get_ItemG()->get_nStartEventTime();
					pUdrFormat->get_ItemG()->put_nStartEventTime( tStartTime );
					
					// 과거의 정보를 유지해주는 모듈
					_RemainsItem( pSearchUdrFormat, pUdrFormat );
					
					// 완료된 UDR은 남겨둔다.
					// 2001. 05. 26
				    if( UpdateAggregator( pUdrFormat ) )
				    {
				    	// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
						*LogM << LogM->ErrorCode(0)                                            
							<< "=====================================================" << endl;
						*LogM << LogM->ErrorCode(0) 
							<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
							<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
							<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
							<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
						*LogM << LogM->ErrorCode(4224) 
							<< "STOP->EXIST->STATUS=0->Big E_T->Big A_T : UpdateAggregator Error!!!" << endl;
						return -1;
			  	 	}
			   		while(1)
			   		{
				    	if(MqUdr->sendMsg(pUdrFormat) == FLAG_MQ_IS_FULL)
						{
						//    *LogM << LogM->ErrorCode(4225) 
						//		<< "Aggregator->Collector MSG Queue FULL!!" << endl;
							usleep(100);
							//return -1;
						}
				    	else break;
				   	}
				   	
					nSumAggregationUdr++;
		//			*LogM << LogM->ErrorCode(0) 
			//			<< "Sum Aggregation Udr Count => " << nSumAggregationUdr << endl;
					
					// 완료된 UDR은 남겨두지 않고, 삭제해준다.
					// 2001. 05. 21
					/*
					if( DeleteAggregator( pUdrFormat ) )
					{
						*LogM << LogM->ErrorCode(4226) 
								<< "STOP->EXIST->STATUS=0->Big E_T->Big A_T : DeleteAggregator Error!!" << endl;
						 return -1;
					}
					*/
					if(LOG_ON)
					{
						*LogM << LogM->ErrorCode(0) 
							<< "STOP->EXIST->STATUS=0->Big E_T->Big A_T->Run: UpdateAggregator->Send Data" << endl;
					}
					return 0;
				} // else
			} // else	
		} // else if
	} // if
	// non-exist data
	else
	{
		// 신규 session을 만들어 insert한후, 
		// flag를 setting하고 종료시킨다.
		// 추후에 이미 처리된 후에 들어오는 STOP는 
		// active time과 interval을 비교하여 처리한다. 
		pUdrFormat->get_ItemHead()->put_nUdrStatus(1);
		tStartTime = pUdrFormat->get_ItemG()->get_nEndEventTime();
		tStartTime -= pUdrFormat->get_ItemG()->get_nActiveTime();
		pUdrFormat->get_ItemG()->put_nStartEventTime( tStartTime );
		
		// 완료된 UDR은 남겨둔다.
		// 2001. 05. 26
		if( InsertAggregator( pUdrFormat ) )
		{
			// 특이사항이나 에러만 로그로 남김 ----> 2001. 07. 20
			*LogM << LogM->ErrorCode(0)                                            
				<< "=====================================================" << endl;
			*LogM << LogM->ErrorCode(0) 
				<< "== [Type:" << pUdrFormat->get_ItemHead()->get_nUdrType() << "] [MsId:"
				<< pUdrFormat->get_ItemA()->get_unMsId() << "] [AccSesID:" 	
				<< pUdrFormat->get_ItemC()->get_strAccountSessionID() << "] [CorrID:"
				<< pUdrFormat->get_ItemC()->get_strCorrelationID() << "] ==" << endl;
		 	*LogM << LogM->ErrorCode(4227) 
				<< "STOP->Not EXIST : InsertAggregator Error" << endl;
			return -1;
		}
	   	while(1)
	   	{
		    if(MqUdr->sendMsg(pUdrFormat) == FLAG_MQ_IS_FULL)
			{
		//	     *LogM << LogM->ErrorCode(4225) 
		//			<< "Aggregator->Collector MSG Queue FULL!!" << endl;
				usleep(100);
				//return -1;
			}
		   	else break;
	   	}
		nSumAggregationUdr++;
//		*LogM << LogM->ErrorCode(0) 
//			<< "Sum Aggregation Udr Count => " << nSumAggregationUdr << endl;

		if(LOG_ON)
		{
			*LogM << LogM->ErrorCode(0) 
				<< "STOP->Not EXIST->Run: InsertAggregator->Send New Data" << endl;
		}
		return 0;
	} // else
}
//===============================================================
// _RemainsItem Method		   									=
//  : 과거의 정보를 유지해야 하는 부분을 유지해 주는 Method	 		=
// argument     :  pUdrFormat -> 처리하기 위해 들어온 UdrFormat	=
//				   pSearchUdrFormat -> 이전에 MDB에 있던 DATA		=
// return value : None											=
//																=
//===============================================================
void CAggregator::_RemainsItem( UdrFormat* pSearchUdrFormat, UdrFormat* pUdrFormat )
{
	F_TYPE*          pItemF;
	int				 nReleaseIndicator;
	A_INT_TYPE*      pSearchItemA;
	B_TYPE*          pSearchItemB;
	C_TYPE*          pSearchItemC;
	D_TYPE*          pSearchItemD;
	E_TYPE*          pSearchItemE;
	F_TYPE*          pSearchItemF;

	pSearchItemA=pSearchUdrFormat->get_ItemA();
	pSearchItemB=pSearchUdrFormat->get_ItemB();
	pSearchItemC=pSearchUdrFormat->get_ItemC();
	pSearchItemD=pSearchUdrFormat->get_ItemD();
	pSearchItemE=pSearchUdrFormat->get_ItemE();
	pSearchItemF=pSearchUdrFormat->get_ItemF();


	pItemF=pUdrFormat->get_ItemF();
    nReleaseIndicator=pItemF->get_nReleaseIndicator();
	pSearchItemF->put_nReleaseIndicator( nReleaseIndicator );

	pUdrFormat->setItem( pSearchItemA );
	pUdrFormat->setItem( pSearchItemB );
	pUdrFormat->setItem( pSearchItemC );
	pUdrFormat->setItem( pSearchItemD );
	pUdrFormat->setItem( pSearchItemE );
	pUdrFormat->setItem( pSearchItemF );
}
//===============================================================
// InsertAggregator Method					   					=
//  : 실제 Insert Transaction을 호출하는 Method			  		=
// argument     :  pUdrFormat -> 처리하기 위해 들어온 UdrFormat	=
// return value : 0-> success									=
//                -1-> fail										=
//																=
//===============================================================
int CAggregator::InsertAggregator( UdrFormat* pUdrFormat )
{
	KeyDef			cKeyDef;
	
	memset( &cKeyDef, NULL, sizeof( KeyDef ) );
	
	cKeyDef.set_unMsID( pUdrFormat->get_ItemA()->get_unMsId() );
	cKeyDef.set_strAccSessionID( pUdrFormat->get_ItemC()->get_strAccountSessionID() );
	cKeyDef.set_strCorrID( pUdrFormat->get_ItemC()->get_strCorrelationID() );
	
	if( pMdbMan->insertMdb( &cKeyDef, pUdrFormat ) )
	{
		*LogM << LogM->ErrorCode(4209) 
			<< "InsertAggregator:insertMdb Error!!" << endl;	
		return -1;
	}
	return 0;
}
//===============================================================
// DeleteAggregator Method					   					=
//  :  실제 Delete Transaction을 호출하는 Method			  		=
// argument     :  pUdrFormat -> 처리하기 위해 들어온 UdrFormat	=
// return value : 0-> success									=
//                -1-> fail										=
//																=
//===============================================================
int CAggregator::DeleteAggregator( UdrFormat* pUdrFormat )
{
	KeyDef			cKeyDef;
	
	memset( &cKeyDef, NULL, sizeof( KeyDef ) );
	
	cKeyDef.set_unMsID( pUdrFormat->get_ItemA()->get_unMsId() );
	cKeyDef.set_strAccSessionID( pUdrFormat->get_ItemC()->get_strAccountSessionID() );
	cKeyDef.set_strCorrID( pUdrFormat->get_ItemC()->get_strCorrelationID() );
	
	if( pMdbMan->deleteMdb( cKeyDef ) == -1 )
	{
		*LogM << LogM->ErrorCode(4210) 
			<< "DeleteAggregator:deleteMdb Error!!" << endl;	
		return -1;
	}
	return 0;
}
//===============================================================
// UpdateAggregator Method					   					=
//  : 실제 Update Transaction을 호출하는 Method			  		=
// argument     :  pUdrFormat -> 처리하기 위해 들어온 UdrFormat	=
// return value : 0-> success									=
//                -1-> fail										=
//																=
//===============================================================
int CAggregator::UpdateAggregator( UdrFormat* pUdrFormat )
{
	KeyDef			cKeyDef;
	
	memset( &cKeyDef, NULL, sizeof( KeyDef ) );
	
	cKeyDef.set_unMsID( pUdrFormat->get_ItemA()->get_unMsId() );
	cKeyDef.set_strAccSessionID( pUdrFormat->get_ItemC()->get_strAccountSessionID() );
	cKeyDef.set_strCorrID( pUdrFormat->get_ItemC()->get_strCorrelationID() );
	
	if( pMdbMan->updateMdb( cKeyDef, pUdrFormat ) )
	{
		*LogM << LogM->ErrorCode(4208) 
			<< "UpdateAggregator:updateMdb Error!!" << endl;	
		return -1;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	UdrFormat   	stUdr;
	CAggregator* 	pAggregator;
	

	if( argc != 7 )
	{
		cout << "Usage Aggregator [SysId][SerId][GroupId][ProcessId][SerialNumber][Remark]" << endl;
		exit( 1 );
	}
	
	SystemId=atoi(argv[1]);
	ServiceId=atoi(argv[2]);
	GroupId=atoi(argv[3]);
	ProcessType=atoi(argv[4]);
	PserialNo=atoi(argv[5]);
	PreMark=atoi(argv[6]);
	
	
	LogM	= new SendLogManage(SystemId,ServiceId,GroupId ,ProcessType ,PserialNo, PreMark );
	LogM->put_EventHistData(FLAG_START, FLAG_PROC_START);
	
	//signal(SIGNULL,sigCapture);	//0
	signal(SIGHUP,sigCapture);
	signal(SIGINT,sigCapture);
	signal(SIGQUIT,sigCapture);
	signal(SIGKILL,sigCapture);
	signal(SIGTERM,sigCapture);
	signal(SIGSTOP,sigCapture);
	signal(SIGTSTP,sigCapture);
//	signal(SIGCONT,sigCapture);	//26
	signal(SIGUSR1,sigCapture);
	
	ReadAggregator = new MessageQueue<UdrFormat>(SystemId,ServiceId,GroupId ,ProcessType ,PserialNo ,10 ,1000);
	MqUdr = new MessageQueue<UdrFormat>(SystemId,ServiceId,5,2,0,10,1000 );
	mqPMS = new MessageQueue<MsgPmsStatus>(SystemId, 0,1,1,0,10, 1000);
	WritePMSMQ(FLAG_PROC_START);
	// 앞으로 number error check를 해야 하고
	// 중복check도 해야 합니다.
	pAggregator = new CAggregator( SystemId,ServiceId,GroupId,ProcessType,PserialNo,PreMark );
   
    while(1)
	{
		
		memset((char*)&stUdr, NULL, sizeof(UdrFormat));
		
	    if (ReadAggregator->recvMsg(&stUdr) == FLAG_MQ_IS_EMPTY)
	    {
	 //   	*LogM << LogM->ErrorCode(0) 
	//			<< "==Recv=NO DATA IN MessageQueue=====" << endl;
		    usleep(100);
		}
		else 
		{
//			*LogM << LogM->ErrorCode(0) 
//				<< "===============START   Processing==============" << endl;
			pAggregator->Aggregator( &stUdr );
//	    	*LogM << LogM->ErrorCode(0) 
//				<< "===============END     Processing==============" << endl;
		}
	}
	
	*LogM << LogM->ErrorCode(0) 
			<< "End Aggregator Process!!" << endl;
	delete ReadAggregator;
	delete MqUdr;
	delete pAggregator;
	delete mqPMS;
	LogM->put_EventHistData(FLAG_END, 0);
	delete LogM;
	return 0;
}
//===============================================================
// WritePMSMQ Method		     			   					=
//  : PMS에게 Process의 상태를 보고하기 위한 Method					=
//	                                    						=
// argument     :  sigNo -> Signal Number						=
// return value : None											=
//																=
//===============================================================
void WritePMSMQ(int sigNo )
{
	int 			i=0;
	char 			PMSMQ_NO[2];
	MsgPmsStatus	OneRec(MSG_STATUS, SystemId, ServiceId, GroupId, ProcessType, PserialNo, PreMark);

	
	sprintf(PMSMQ_NO, "%d", SystemId);
	OneRec.set_nStatus(sigNo);
	
	while(1)
	{
		i++;
		if (mqPMS->sendMsg(&OneRec) == FLAG_MQ_IS_FULL)
		{
			cout << "[ERROR] PMSMQ " << PMSMQ_NO << " IS FULL sleep(1)...!"	<< endl;
			sleep(1);
			if (i >= 10)
			{
				kill(0,SIGINT);
			}
		}else break;
	}
}
//===============================================================
// sigCapture Method		     			   					=
//  : Process에서 signal capture	하는 Method      				=
//	                                    						=
// argument     :  sigNo -> Signal Number						=
// return value : None											=
//																=
//===============================================================
void sigCapture(int sigNo )
{
	cout << "SIGNAL :: Aggregator" << PserialNo << "Process End " << endl;
	delete ReadAggregator;
	delete MqUdr;

	switch(sigNo)
	{
		case SIGINT :
		case SIGQUIT :
		case SIGKILL :
		case SIGTERM :
				*LogM << LogM->ErrorCode(4213) 
					<< "Aggregator Process Killed!! " <<endl;
				WritePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				LogM->put_EventHistData(FLAG_END, -1);
				delete LogM;
				delete mqPMS;
				exit(-1);
				break;
		case SIGUSR1:
				*LogM << LogM->ErrorCode(0) 
					<< "Aggregator Process Normal Stop!! " <<endl;
				WritePMSMQ(FLAG_PROC_TERM);//0//프로세스정상stop (Client업무명령)
				LogM->put_EventHistData(FLAG_END, 0);
				delete LogM;
				delete mqPMS;
				exit(0);
				break;
		default:
				*LogM << LogM->ErrorCode(4213) 
					<< "Aggregator Process Killed2!! " <<endl;
				WritePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				LogM->put_EventHistData(FLAG_END, -1);
				delete LogM;
				delete mqPMS;
				exit(-1);
				break;
	}
}



