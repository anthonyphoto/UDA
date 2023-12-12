/******************************************************************
* 파 일 명     : Observer.cpp 			     					  
* 기   능                                                         
* : 일정시간마다 memory db를 검색해서 조건에 맞는 UDR를                
*    Message Queue로 보내거나 지우는 역할을 하는 Progream.	   		 
* 작 성 자     : Han Guen Hee                    				  
* first data  : 2000. 9. 20        	     						  
* last updata : 2001. 05. 21       		 					  
* 관 련 화 일  : observer.hpp, OBS_FlagDate.cfg	
* Version     : 1.1( 2001. 05. 21 )
* program history                        						  
* 	Observer 파일을 사용하지 않도록 함.	-> 2001. 01. 05		 		  
*   삭제하는 모듈을 list에서 꺼내서 삭제하도록 update -> 2001. 02. 16 
*   검색하는 모듈과 삭제하는 모듈을 새로 만듬. -> 2001. 02. 16      
*	Log 추가 -> 2001. 05. 26
	SNMP적용(file checking) -> 2001. 05. 28
******************************************************************/
#include <Observer.hpp>

static KeyDef						nFindList[100000];  // 2001. 02. 16
static int				 			nIndex;				// 2001. 02. 16
	   char							strFlagFile[100];	// SNMP check file 2001. 05. 28
	   
	 	
void sigCapture(int sigNo );
void WritePMSMQ(int sigNo);

// Constructor
CObserver::CObserver()
{
}
//===============================================================
// Constructor Method											=
//  : 시작시간과 config정보를 읽어온다.                        		=
// argument : 프로세스 형상구분 6가지								=
// return value : None											=
//																=
//===============================================================
CObserver::CObserver(int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark  )
{
	// SNMP File Path 2001. 05. 28
	memset( &strFlagFile, NULL, sizeof(strFlagFile) );
	sprintf( strFlagFile, "/SVC%d/DATA/Obs_Check.cfg", nServiceId );
	
	// Observer 시작시간을 구해준다.
	tStartTime = time( NULL );
	
	// 몇번만에 한도정보중간정산을 해야하는지를 나타내는 
	// Config data를 읽어오는 부분.
	sprintf( PATH_NAME_OBSERVER_FLAG, "/SVC%d/DATA/OBS_FlagDate.cfg",nServiceId );
	pObsFlagDate = new ObsFlagDate(PATH_NAME_OBSERVER_FLAG);
	pObsFlagDate->read_ObsFlagDate();
	unFlag = pObsFlagDate->getFlag();
	unInterval = pObsFlagDate->getInterval();
	unApplicationTime = pObsFlagDate->getBeforeTime();
	
	if( 0 )
	{
		*LogM << LogM->ErrorCode(0) 
		  	<< "[unFlag]=" << unFlag << endl;
		*LogM << LogM->ErrorCode(0) 
			<< "[unInterval]=" << unInterval << endl;
		*LogM << LogM->ErrorCode(0) 
			<< "[unApplicationTime]=" << unApplicationTime << endl;
		*LogM << LogM->ErrorCode(0) 
			<< "[tStartTime]=" << tStartTime << endl;
	}
	
	nIndex = 0;
	memset( &nFindList, NULL, sizeof(nFindList) );
}

// destructor
CObserver::~CObserver()
{
	//fObserver.close();
	delete pObsFlagDate;
}
//===============================================================
// Observer Method						     					=
//  : 외부에서 Observer를 호출하는 Method                     		=
// argument     : None											=
// return value : None											=
//																=
//===============================================================
void CObserver::Observer( void )
{
	time_t 			tCurrentTime;
	unsigned int	unSecInterval;
	unsigned int	unSubTime;
	unsigned int 	unAllocAddress;
	unsigned int 	unBtreeShmKey;
	unsigned int 	unBtreeAllocKey;
	char  			cId='A';
	int				temp;
	int		 		nReTemp;
	
	// 분단위의 처리간격을 초단위로 바꿔준다.
	unSecInterval = unInterval * 60;
	while( 1 )
	{
		sleep(3600);
		*LogM << LogM->ErrorCode(0) 
				<< "======== OBSERVER PROCESSING START ========" << endl;
		// 현재시간을 구해준다.
		tCurrentTime = time( NULL );
		// start시간에서 현재시간을 뺀 차를 구한다.
		unSubTime =difftime( tCurrentTime, tStartTime );
		if( 0 )
		{
			*LogM << LogM->ErrorCode(0) 
				<< "[tStartTime]=" << tStartTime << endl;
			*LogM << LogM->ErrorCode(0) 
				<< "[tCurrentTime]=" << tCurrentTime << endl;
			*LogM << LogM->ErrorCode(0) 
				<< "[unSecInterval]=" << unSecInterval << endl;
			*LogM << LogM->ErrorCode(0) 
				<< "[unSubTime]=" << unSubTime << endl;
			*LogM << LogM->ErrorCode(0) 
				<< "[unApplicationTime * 60 ]=" << unApplicationTime * 60  << endl;
		}
	
		// 처리간격만큼의 시간이 지났다면...
		if( unSubTime >= unSecInterval )
		{
			// SNMP Flag File을 check하여, file이 존재하면 network 오류로그를 log manager에 보내고,
			// observer는 동작하지 않는다. (다음 동작시간을 기다린다.)   2001. 05. 28
			if( access(strFlagFile, 0) == 0 )
			{
				// Error Code 10001:udaTrapPdsnUdpFail
//				*LogM << LogM->ErrorCode(4504) 
//					<< "[Observer] PDSN I/F Error" << endl;
				continue;
			}

			tProcessingTime = tCurrentTime - ( unApplicationTime * 60 );
			// 10개의 MDB를 차례로 처리한다.
			for( int i=0; i<10; i++)
			{
				*LogM << LogM->ErrorCode(0) 
						<< "======== START OBSERVER " << i << "========"  << endl;
				sprintf( PATH_NAME_AGGREGATOR, "/SVC%d/DATA/AggShm%d.cfg",ServiceId,i );
				sprintf( PATH_NAME_AGGREGATOR_MDB, "/SVC%d/MB/Agg%d.mb",ServiceId,i );
				fInOut.open( PATH_NAME_AGGREGATOR_MDB, ios::in | ios::out | ios::binary );
				if( !fInOut )
				{
					*LogM << LogM->ErrorCode(4501) 
						<< "Memory Backup File could not be opened!!" << endl;
					LogM->put_EventHistData(FLAG_END, -1);        
					delete LogM;                                 
					exit( 1 );
				}
				shmData=new ShmMemory( PATH_NAME_AGGREGATOR, cId, SIZE_UDRFORMAT*MAXKEYMEMBER, 0660|IPC_CREAT );
				cId++;
				stPtrDataIndexInfo=( UdrFormat * )shmData->ShmPtr();
				pAllocDataIndexC=new HandleOfAllocIndex( (const char*)PATH_NAME_AGGREGATOR, ( const unsigned char )cId,MAXKEYMEMBER, ( unsigned int )stPtrDataIndexInfo );
				unAllocAddress = pAllocDataIndexC->get_Shm_Key();
				cId++;
				//pBtree=new CBtree( PATH_NAME_AGGREGATOR, 'A',SystemId,ServiceId,2,1,i,0,1 );
				pBtree=new CBtree<KeyDef,UdrFormat>( PATH_NAME_AGGREGATOR, cId );
				unBtreeShmKey = pBtree->getIndexKey();
				unBtreeAllocKey = pBtree->getAllocKey();
				// Memory Lock개념을 넣어야 한다.
				pBtree->shm_lock(); // MEMORY LOCK
				// Btree의 Root Pointer를 얻는다.
				/*
				pBtreeNode = pBtree->getBtreeRoot();
				*LogM << LogM->ErrorCode(0) 
						<< "[Observer] pBtreeNode[" << pBtreeNode <<"]" << endl;
				*/
				if( 0 )
				{
					*LogM << LogM->ErrorCode(0) 
						<< "shmData= " << shmData << endl;
					*LogM << LogM->ErrorCode(0) 
						<< "pAllocDataIndexC= " << pAllocDataIndexC << endl;
					*LogM << LogM->ErrorCode(0) 
						<< "pBtree= " << pBtree << endl;
					*LogM << LogM->ErrorCode(0) 
						<< "pBtreeNode= " << pBtreeNode << endl;
					*LogM << LogM->ErrorCode(0) 
						<< "stPtrDataIndexInfo= " << stPtrDataIndexInfo << endl;
					*LogM << LogM->ErrorCode(0) 
						<< "unAllocAddress= " << unAllocAddress << endl;
					*LogM << LogM->ErrorCode(0) 
						<< "unBtreeShmKey= " << unBtreeShmKey << endl;
					*LogM << LogM->ErrorCode(0) 
						<< "unBtreeAllocKey= " << unBtreeAllocKey << endl;
				}
				switch( unFlag )
				{
					// MDB를 검색해서 해당 시간이 지난 DATA는 일괄삭제한다.
					// MDB와 BackUp file에서 삭제한다.
					case 0 :
						nProcessingCount = 0;
						pBtreeNode = pBtree->getBtreeRoot();
			//			*LogM << LogM->ErrorCode(0) 
			//				<< "[Observer] pBtreeNode[" << pBtreeNode <<"]" << endl;
						if( _FindDeleteUdr( pBtreeNode, tProcessingTime ) )
						{
							*LogM << LogM->ErrorCode(0) 
							    << "success _FindDeleteUdr Mdb" << endl;
						}
						else
						{
							*LogM << LogM->ErrorCode(0) 
							    << "fail _FindDeleteUdr Mdb" << endl;
							break;
						}
				//		pBtreeNode = pBtree->getBtreeRoot();
				//		*LogM << LogM->ErrorCode(0) 
				//			<< "[Observer] pBtreeNode[" << pBtreeNode <<"]" << endl;
						if( _DeleteMdb( pBtreeNode, tProcessingTime ) )
						{
							*LogM << LogM->ErrorCode(0) 
							    << "success Delete Mdb" << endl;
						}
						else
						{
							*LogM << LogM->ErrorCode(0) 
							    << "fail Delete Mdb" << endl;
							 break;
						}
						break;
					// MDB를 검색해서 해당시간이 지난 DATA는 STOP UDR처리한다.
					case 1 : 
						nProcessingCount = 0;
						*LogM << LogM->ErrorCode(0) 
							<< "StopProcessMdb" << endl;
						pBtreeNode = pBtree->getBtreeRoot();
			//			*LogM << LogM->ErrorCode(0) 
			//				<< "[Observer] pBtreeNode[" << pBtreeNode <<"]" << endl;
						if( _StopProcessMdb( pBtreeNode, tProcessingTime ) )
						{
							*LogM << LogM->ErrorCode(0) 
							    << "success Stop Process" << endl;
						}
						else
						{
							*LogM << LogM->ErrorCode(0) 
							    << "fail Stop Process" << endl;
						}
						break;
					// MDB를 검색해서 해당시간이 지난 DATA는 STOP UDR처리한후
					// MDB와 BackUp file에서 삭제한다.
					case 2 :
						nProcessingCount = 0;
						pBtreeNode = pBtree->getBtreeRoot();
			//			*LogM << LogM->ErrorCode(0) 
			//				<< "[Observer] pBtreeNode[" << pBtreeNode <<"]" << endl;
						if( _FindDeleteUdr( pBtreeNode, tProcessingTime ) )
						{
							*LogM << LogM->ErrorCode(0) 
							    << "success _FindDeleteUdr Mdb" << endl;
						}
						else
						{
							*LogM << LogM->ErrorCode(0) 
							    << "fail _FindDeleteUdr Mdb" << endl;
							break;
						}
						// Btree의 Root Pointer를 얻는다.
						//pBtreeNode = pBtree->getBtreeRoot();
						if( _DeleteMdb( pBtreeNode, tProcessingTime ) )
						{
							*LogM << LogM->ErrorCode(0) 
							    << "success Delete Mdb" << endl;
						}
						else
						{
							*LogM << LogM->ErrorCode(0) 
							    << "fail Delete Mdb" << endl;
							break;
						}
						*LogM << LogM->ErrorCode(0) 
							<< "StopProcessMdb" << endl;
						// Btree의 Root Pointer를 얻는다.
						pBtreeNode = pBtree->getBtreeRoot();
						if( _StopProcessMdb( pBtreeNode, tProcessingTime ) )
						{
							*LogM << LogM->ErrorCode(0) 
							    << "success Stop Process" << endl;
						}
						else
						{
							*LogM << LogM->ErrorCode(0) 
							    << "fail Stop Process" << endl;
							break;
						}
						break;
					default :
						*LogM << LogM->ErrorCode(4503) 
							    << "상태 Flag가 잘못 입력되었음." << endl;
						break;
				} // switch
				nReTemp = pBtree->shm_release(); // MEMORY LOCK
				// attach된 메모리를 detach시켜준다.
				temp = shmData->shm_detach( stPtrDataIndexInfo );
				temp = pAllocDataIndexC->shm_detach( (char *)unAllocAddress );
				temp = pBtree->shm_detach( (char *)unBtreeShmKey );
				temp = pBtree->shm_detach( (char *)unBtreeAllocKey );
				nIndex = 0;
				cId='A';
				memset( &nFindList, NULL, sizeof(nFindList) );
				delete shmData;
				delete pAllocDataIndexC;
				delete pBtree;
			 	fInOut.close();
				*LogM << LogM->ErrorCode(0) 
					<< "======== END OBSERVER " << i << "========"  << endl;	
			} // for
			// Start시간을 다시 setting해준다.
			tStartTime = tCurrentTime;
		} // if
	} // while
	
}
//===============================================================
// _FindDeleteUdr Method						   				 
//  : MDB를 검색해서 삭제할 UDR을 Linked List에 넣어준다.    		 
// argument     :  pBtreeNode -> Btree Root Node				 
//				   tCompareTime -> Observer가 동작할 시간		 
// return value : 1-> success									 
//                0-> fail										 
// last update : 2001. 02. 16									 
//===============================================================
int CObserver::_FindDeleteUdr( _BTREENODE<KeyDef,UdrFormat>* pBtreeNode, time_t tCompareTime )
{
	int i;
	
//	*LogM << LogM->ErrorCode(0) 
//		<< "=====>>_FindDeleteUdr " <<  endl;
		 	
	if( pBtreeNode )
	{
		 for( i=0; i<pBtreeNode->iNumberKey; i++ )
		 {
			 // 현재 처리가 완료된 UDR중에서..
			 if( pBtreeNode->kIndexKey[i].pdataType->get_ItemHead()->get_nUdrStatus() == 1 )
			 {
			 	// 입력된 시간보다 MDB의 시간이 작다면..(너무 오랫동안 UDR이 들어오지 않았다면.)
				if( tCompareTime >= pBtreeNode->kIndexKey[i].pdataType->get_ItemG()->get_nEndEventTime() )
				{
					// 삭제될 Min번호를 저장.
					memcpy( &nFindList[nIndex], &pBtreeNode->kIndexKey[i].dtype, sizeof( KeyDef ) );
					nIndex++;
				}// if
			}// if
		 }// for
		  // 다음 Node를 검색한다.
		for(  i=0; i<=pBtreeNode->iNumberKey; i++ )
		{
			if( !_FindDeleteUdr( pBtreeNode->pBTREENODE[i], tCompareTime ) )
			{
				*LogM << LogM->ErrorCode(0) 
					<< "Observer : _FindDeleteUdr 실패!!! " << endl;
				return 0;
			}
		} // for
	} // if
	return 1;
}
//==================================================================
// _DeleteMdb Method						   					
//  : MDB를 검색해서 해당 시간이 지난 DATA는 일괄삭제한다.       	
// argument     :  pBtreeNode -> Btree Root Node				
//                0-> fail										
//				   tCompareTime -> Observer가 동작할 시간		
// return value : 1-> success									
//																
//==================================================================
int CObserver::_DeleteMdb( _BTREENODE<KeyDef,UdrFormat>* pBtreeNode, time_t tCompareTime )
{
	unsigned int 						unBaseAddress;       
	unsigned int 						unCurrentAddress;  
	_DATATYPE<KeyDef,UdrFormat> 		stDataType; 
	UdrFormat*							pDeleteDataNode;
	int									nTempCount=0; 
	int									k;
	
	*LogM << LogM->ErrorCode(0) 
		<< "DeleteMdb" << endl;
	// 저장해 놓은 번호들을 삭제한다.
	/// 2001. 02. 16
	for( k=0; k<nIndex; k++)
	{
//		*LogM << LogM->ErrorCode(0) 
//		   << "Delete Msid = " << nFindList[k].get_unMsID() << endl;
		memcpy( &stDataType.dtype, &nFindList[k], sizeof( KeyDef ) );
	    stDataType.pdataType = 0x00;
	    
	    pDeleteDataNode=pBtree->selectNode(nFindList[k]);
	    if(pDeleteDataNode == NULL)
	    {
	    	*LogM << LogM->ErrorCode(4211) 
				<< "_DeleteMdb:ShowSearch Error!!" << endl;
	        return 0;
	    }
	    unCurrentAddress=(unsigned int)pDeleteDataNode;
	    unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();
	    
	    //Btree index를 초기화 시켜준다.
   		memset((char *)pDeleteDataNode,NULL,sizeof(UdrFormat));
   		
   		fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
		fInOut.write( ( char * )pDeleteDataNode, sizeof( UdrFormat ) );
		fInOut.flush();
	
	    // Btree data를 delete.
	    pBtree->DelNode(stDataType);
		
	    pAllocDataIndexC->deleteIndex( (( unCurrentAddress-unBaseAddress )/sizeof( UdrFormat )) );
	}
	
	return 1;
}
//===============================================================
// _StopProcessMdb Method					   					=
//  : MDB를 검색해서 해당시간이 지난 DATA는 STOP UDR처리한다.    		=
// argument     :  pBtreeNode -> Btree Root Node				=
//				   tCompareTime -> Observer가 동작할 시간			=
// return value : 1-> success									=
//                0-> fail										=
//																=
//===============================================================
int CObserver::_StopProcessMdb( _BTREENODE<KeyDef,UdrFormat>* pBtreeNode, time_t tCompareTime )
{
	unsigned int 						unBaseAddress;       
	unsigned int 						unCurrentAddress;  
	_DATATYPE<KeyDef,UdrFormat> 		stDataType;  
	
	if( pBtreeNode )
	{
		// *LogM << LogM->ErrorCode(0) 
		// 	<< "=====>>pBtreeNode->iNumberKey = " << pBtreeNode->iNumberKey << endl;
		 for( int i=0; i<pBtreeNode->iNumberKey; i++ )
		 {
			 // 현재 처리가 진행중인 UDR중에서..
			 if( pBtreeNode->kIndexKey[i].pdataType->get_ItemHead()->get_nUdrStatus() == 0 )
			 {
			 	// 입력된 시간보다 MDB의 시간이 작다면..(너무 오랫동안 UDR이 들어오지 않았다면.)
				if( tCompareTime >= pBtreeNode->kIndexKey[i].pdataType->get_ItemG()->get_nEndEventTime() )
				{
					nProcessingCount++;
					
					*LogM << LogM->ErrorCode(0) 
					 	<< "=========== SEND MIN INFO ==========" << endl;
					*LogM << LogM->ErrorCode(0) 
					 	<< "MsId:[" << pBtreeNode->kIndexKey[i].dtype.get_unMsID() << "] ACC_SES_ID:[" 
					 	<< pBtreeNode->kIndexKey[i].pdataType->get_ItemC()->get_strAccountSessionID() << "] CORR_ID:["
					 	<< pBtreeNode->kIndexKey[i].pdataType->get_ItemC()->get_strCorrelationID() << "]" << endl;
					*LogM << LogM->ErrorCode(0) 
					 	<< "START E_T:[" << pBtreeNode->kIndexKey[i].pdataType->get_ItemG()->get_nStartEventTime() << "] END E_T:["
					 	<< pBtreeNode->kIndexKey[i].pdataType->get_ItemG()->get_nEndEventTime() << "]" << endl;
					
					// 처리를 완료했다고 flag를 setting한후...
					pBtreeNode->kIndexKey[i].pdataType->get_ItemHead()->put_nUdrStatus(1);
					
					// 해당 backup file도 update해준다.
					unCurrentAddress=(unsigned int)pBtreeNode->kIndexKey[i].pdataType;
				    unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();
					
					fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
					fInOut.write( ( char * )pBtreeNode->kIndexKey[i].pdataType, sizeof( UdrFormat ) );
					fInOut.flush();
					
					// Collector MQ에 해당 UDR을 보내준다.
					while(1)
					{
						if(MqUdr->sendMsg( pBtreeNode->kIndexKey[i].pdataType ) == FLAG_MQ_IS_FULL)
						{
							*LogM << LogM->ErrorCode(0) 
								<< "Aggregator->Collector MSG Queue FULL!!" << endl;
							usleep( 100 );
						//	 return 0;
						}
						else
						{
					//		*LogM << LogM->ErrorCode(0) 
					//			<< "send data"  << endl;
							break;
						}
					}// while 
				}// if
			}// if
		 }// for
		  // 다음 Node를 검색한다.
		for(  i=0; i<=pBtreeNode->iNumberKey; i++ )
		{
			if( !_StopProcessMdb( pBtreeNode->pBTREENODE[i], tCompareTime ) )
			{
				*LogM << LogM->ErrorCode(0) 
					<< "Observer : _StopProcessMdb 실패!!! " << endl;
				return 0;
			}
		} // for
	} // if
	return 1;
}
int main( int argc, char* argv[] )
{
	CObserver*	pObserver;
	
	if( argc != 7 )
	{
		cout << "Usage Observer [SysId][SerId][GroupId][ProcessId][SerialNumber][Remark]" << endl;
		exit( 1 );
	}
	
	SystemId=atoi(argv[1]);
	ServiceId=atoi(argv[2]);
	GroupId=atoi(argv[3]);
	ProcessType=atoi(argv[4]);
	PserialNo=atoi(argv[5]);
	PreMark=atoi(argv[6]);
	
	// Observer를 위한 Log Manager
	LogM	= new SendLogManage( SystemId,ServiceId,GroupId ,ProcessType ,PserialNo, PreMark );
	LogM->put_EventHistData( FLAG_START, FLAG_PROC_START );
	
	//signal(SIGNULL,sigCapture);	//0
	signal(SIGHUP,sigCapture);
	signal(SIGINT,sigCapture);
	signal(SIGQUIT,sigCapture);
	signal(SIGKILL,sigCapture);
	signal(SIGTERM,sigCapture);
	signal(SIGSTOP,sigCapture);
	signal(SIGTSTP,sigCapture);
	signal(SIGPIPE,SIG_IGN);   // 2001.05.10 sigpipe signal 무시.
//	signal(SIGCONT,sigCapture);	//26
	signal(SIGUSR1,sigCapture);
	
	// Collector MQ
	MqUdr = new MessageQueue<UdrFormat>(SystemId,ServiceId,5,2,0,10,1000 ); 
	// PMS MQ
	mqPMS = new MessageQueue<MsgPmsStatus>(SystemId,0,1,1,0,10, 1000);
	WritePMSMQ(FLAG_PROC_START);
	
	pObserver = new CObserver( SystemId,ServiceId,GroupId,ProcessType,PserialNo,PreMark );
	// Observer Method를 호출한다.
	pObserver->Observer();
	
	delete MqUdr;
	delete mqPMS;
	delete pObserver;
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
	cout << "SIGNAL :: Observer" << PserialNo << "Process End " << endl;
	delete MqUdr;

	switch(sigNo)
	{
		case SIGINT :
		case SIGQUIT :
		case SIGKILL :
		case SIGTERM :
				*LogM << LogM->ErrorCode(0) 
					<< "Observer Process Killed!! " <<endl;
				WritePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				delete mqPMS;
				LogM->put_EventHistData(FLAG_END, -1);
				delete LogM;
				exit(-1);
				break;
		case SIGUSR1:
				*LogM << LogM->ErrorCode(0) 
					<< "Observer Process Normal Stop!! " <<endl;
				WritePMSMQ(FLAG_PROC_TERM);//0//프로세스정상stop (Client업무명령)
				delete mqPMS;
				LogM->put_EventHistData(FLAG_END, 0);
				delete LogM;
				exit(0);
				break;
		default:
				*LogM << LogM->ErrorCode(0) 
					<< "Observer Process Killed2!! " <<endl;
				WritePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				delete mqPMS;
				LogM->put_EventHistData(FLAG_END, -1);
				delete LogM;
				exit(-1);
				break;
	}
}




