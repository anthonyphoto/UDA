/******************************************************************
* 파 일 명     : Observer.cpp 			     					  *
* 기   능                                                          *
* : 일정시간마다 memory db를 검색해서 조건에 맞는 UDR를                 *
*    Message Queue로 보내거나 지우는 역할을 하는 Progream.	   		  *
* 작 성 자     : Han Guen Hee                    				  *
* first data  : 2000. 9. 20        	     						  *
* last updata : 2001. 01. 05        		 					  *
* 관 련 화 일  : observer.hpp, OBS_FlagDate.cfg					  *
* program history                        						  *
* 	Observer 파일을 사용하지 않도록 함.	-> 2001. 01. 05		 		  *
* bug fix                        	     						  *
*                         	        	 						  *
******************************************************************/
#include "Observer.hpp"

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
		*ObserverLogM << ObserverLogM->ErrorCode(0) 
		  	<< "[unFlag]=" << unFlag << endl;
		*ObserverLogM << ObserverLogM->ErrorCode(0) 
			<< "[unInterval]=" << unInterval << endl;
		*ObserverLogM << ObserverLogM->ErrorCode(0) 
			<< "[unApplicationTime]=" << unApplicationTime << endl;
		*ObserverLogM << ObserverLogM->ErrorCode(0) 
			<< "[tStartTime]=" << tStartTime << endl;
	}
	/*
	sprintf( PATH_NAME_OBSERVER, "/SVC%d/LOG/Observer.txt",nServiceId );
	fObserver.open( PATH_NAME_OBSERVER, ios::out | ios::binary );
	if( !fObserver )
	{
		*ObserverLogM << ObserverLogM->ErrorCode(4500) 
			<< "File could not be opened!!" << endl;
		ObserverLogM->put_EventHistData(FLAG_END, -1);
		delete ObserverLogM;
		exit( 1 );
	}
	*/
}

/*
CObserver::CObserver(int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark  )
{
	unsigned int	unYYMMDD;		
	unsigned int	unHHMMSS;	
	struct tm       stTime;	
	int     		nYear;
	int 			nMonth;
	int				nDay;
	int				nTime;
	int				nMinute;
	int				nSecond;
	int				nTemp;
	
	// 몇번만에 한도정보중간정산을 해야하는지를 나타내는 
	// Config data를 읽어오는 부분.
	sprintf( PATH_NAME_OBSERVER_FLAG, "/SVC%d/DATA/OBS_FlagDate.cfg",nServiceId );
	pObsFlagDate = new ObsFlagDate(PATH_NAME_OBSERVER_FLAG);
	pObsFlagDate->read_ObsFlagDate();
	unFlag = pObsFlagDate->getFlag();
	unYYMMDD = pObsFlagDate->getYYMMDD();
	unHHMMSS = pObsFlagDate->getHHMMSS();
	if( 1 )
	{
		cout << "[unFlag]=" << unFlag << endl;
		cout << "[unYYMMDD]=" << unYYMMDD << endl;
		cout << "[unHHMMSS]=" << unHHMMSS << endl;
	}
	
	nYear = unYYMMDD / 10000;
	nTemp = unYYMMDD % 10000;
	nMonth = nTemp / 100;
	nDay = nTemp % 100;
	nTime = unHHMMSS / 10000;
	nTemp = unHHMMSS % 10000;
	nMinute = nTemp / 100;
	nSecond = nTemp % 100;
	
	if( 1 )
	{
		cout << "nYear= " << nYear<< endl;
		cout << "nMonth= " << nMonth<< endl;
		cout << "nDay= " << nDay<< endl;
		cout << "nTime= " << nTime<< endl;
		cout << "nMinute= " << nMinute<< endl;
		cout << "nSecond= " << nSecond<< endl;
	}
	
	stTime.tm_year=nYear - 1900;
	stTime.tm_mon=nMonth - 1;
	stTime.tm_mday=nDay;
	stTime.tm_hour=nTime;
	stTime.tm_min=nMinute;
	stTime.tm_sec=nSecond;
	
	tMakeTime = mktime( &stTime );
	if( 1 )
	{
		cout << "tMakeTime = " << tMakeTime << endl;
	}
	
	fObserver.open( "/SVC1/LOG/Observer.txt", ios::out | ios::binary );
	if( !fObserver )
	{
		*ObserverLogM << ObserverLogM->ErrorCode(0) 
			<< "File could not be opened!!" << endl;
		ObserverLogM->put_EventHistData(FLAG_END, -1);
		delete ObserverLogM;
		exit( 1 );
	}
}
*/
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
	
	// 분단위의 처리간격을 초단위로 바꿔준다.
	unSecInterval = unInterval * 60;
	while( 1 )
	{
		sleep(10);
		// 현재시간을 구해준다.
		tCurrentTime = time( NULL );
		// start시간에서 현재시간을 뺀 차를 구한다.
		unSubTime =difftime( tCurrentTime, tStartTime );
		if( 1 )
		{
			*ObserverLogM << ObserverLogM->ErrorCode(0) 
				<< "[tStartTime]=" << tStartTime << endl;
			*ObserverLogM << ObserverLogM->ErrorCode(0) 
				<< "[tCurrentTime]=" << tCurrentTime << endl;
			*ObserverLogM << ObserverLogM->ErrorCode(0) 
				<< "[unSecInterval]=" << unSecInterval << endl;
			*ObserverLogM << ObserverLogM->ErrorCode(0) 
				<< "[unSubTime]=" << unSubTime << endl;
			*ObserverLogM << ObserverLogM->ErrorCode(0) 
				<< "[unApplicationTime * 60 ]=" << unApplicationTime * 60  << endl;
		}
	
		// 처리간격만큼의 시간이 지났다면...
		if( unSubTime >= unSecInterval )
		{
			tProcessingTime = tCurrentTime - ( unApplicationTime * 60 );
			// 10개의 MDB를 차례로 처리한다.
			for( int i=0; i<10; i++)
			{
				*ObserverLogM << ObserverLogM->ErrorCode(0) 
						<< "======== START OBSERVER " << i << "========"  << endl;
				sprintf( PATH_NAME_AGGREGATOR, "/SVC%d/DATA/AggShm%d.cfg",1,i );
				sprintf( PATH_NAME_AGGREGATOR_MDB, "/SVC%d/MB/Agg%d.mb",1,i );
				fInOut.open( PATH_NAME_AGGREGATOR_MDB, ios::in | ios::out | ios::binary );
				if( !fInOut )
				{
					*ObserverLogM << ObserverLogM->ErrorCode(4501) 
						<< "Memory Backup File could not be opened!!" << endl;
					ObserverLogM->put_EventHistData(FLAG_END, -1);        
					delete ObserverLogM;                                 
					exit( 1 );
				}
				shmData=new SharedMemory( SystemId,ServiceId,2,1,i,3, SIZE_UDRFORMAT*MAXKEYMEMBER );
				stPtrDataIndexInfo=( UdrFormat * )shmData->get_ShmPtr();
				pAllocDataIndexC=new HandleOfAllocIndex_New( SystemId,ServiceId,2,1,i,2,MAXKEYMEMBER, ( unsigned int )stPtrDataIndexInfo );
				unAllocAddress = pAllocDataIndexC->get_Shm_Key();
				//pBtree=new CBtree( PATH_NAME_AGGREGATOR, 'A',SystemId,ServiceId,2,1,i,0,1 );
				pBtree=new CBtree( PATH_NAME_AGGREGATOR, 'A',SystemId,ServiceId,2,1,i,0 );
				unBtreeShmKey = pBtree->getIndexKey();
				unBtreeAllocKey = pBtree->getAllocKey();
				// Memory Lock개념을 넣어야 한다.
				pBtree->mem_lock(); // MEMORY LOCK
				// Btree의 Root Pointer를 얻는다.
				pBtreeNode = pBtree->getBtreeRoot();
				if( 0 )
				{
					*ObserverLogM << ObserverLogM->ErrorCode(0) 
						<< "shmData= " << shmData << endl;
					*ObserverLogM << ObserverLogM->ErrorCode(0) 
						<< "pAllocDataIndexC= " << pAllocDataIndexC << endl;
					*ObserverLogM << ObserverLogM->ErrorCode(0) 
						<< "pBtree= " << pBtree << endl;
					*ObserverLogM << ObserverLogM->ErrorCode(0) 
						<< "pBtreeNode= " << pBtreeNode << endl;
					*ObserverLogM << ObserverLogM->ErrorCode(0) 
						<< "stPtrDataIndexInfo= " << stPtrDataIndexInfo << endl;
					*ObserverLogM << ObserverLogM->ErrorCode(0) 
						<< "unAllocAddress= " << unAllocAddress << endl;
					*ObserverLogM << ObserverLogM->ErrorCode(0) 
						<< "unBtreeShmKey= " << unBtreeShmKey << endl;
					*ObserverLogM << ObserverLogM->ErrorCode(0) 
						<< "unBtreeAllocKey= " << unBtreeAllocKey << endl;
				}
				switch( unFlag )
				{
					// MDB를 검색해서 해당 시간이 지난 DATA는 일괄삭제한다.
					// MDB와 BackUp file에서 삭제한다.
					case 0 :
						nProcessingCount = 0;
						if( _DeleteMdb( pBtreeNode, tProcessingTime ) )
						{
							*ObserverLogM << ObserverLogM->ErrorCode(0) 
							    << "Complete Delete Mdb" << endl;
						}
						else
						{
							*ObserverLogM << ObserverLogM->ErrorCode(0) 
							    << "fail Delete Mdb" << endl;
						}
						break;
					// MDB를 검색해서 해당시간이 지난 DATA는 STOP UDR처리한다.
					case 1 : 
						nProcessingCount = 0;
						if( _StopProcessMdb( pBtreeNode, tProcessingTime ) )
						{
							*ObserverLogM << ObserverLogM->ErrorCode(0) 
							    << "Complete Stop Process" << endl;
						}
						else
						{
							*ObserverLogM << ObserverLogM->ErrorCode(0) 
							    << "fail Stop Process" << endl;
						}
						break;
					// MDB를 검색해서 해당시간이 지난 DATA는 STOP UDR처리한후
					// MDB와 BackUp file에서 삭제한다.
					case 2 :
						nProcessingCount = 0;
						if( _AllProcessing( pBtreeNode, tProcessingTime ) )
						{
							*ObserverLogM << ObserverLogM->ErrorCode(0) 
							    << "Complete _AllProcessing Process" << endl;
						}
						else
						{
							*ObserverLogM << ObserverLogM->ErrorCode(0) 
							    << "fail _AllProcessing Process" << endl;
						}
					
						break;
					default :
						*ObserverLogM << ObserverLogM->ErrorCode(4503) 
							    << "상태 Flag가 잘못 입력되었음." << endl;
						break;
				} // switch
				pBtree->mem_unlock(); // MEMORY LOCK
				// attach된 메모리를 detach시켜준다.
				int temp = shmData->detach_ShmPtr( stPtrDataIndexInfo );
				temp = pAllocDataIndexC->detach_ShmPtr( (char *)unAllocAddress );
				temp = pBtree->detach_ShmPtr( (char *)unBtreeShmKey );
				temp = pBtree->detach_ShmPtr( (char *)unBtreeAllocKey );
				
				delete shmData;
				delete pAllocDataIndexC;
				delete pBtree;
				fInOut.close();
				*ObserverLogM << ObserverLogM->ErrorCode(0) 
					<< "======== END OBSERVER " << i << "========"  << endl;	
			} // for
			// Start시간을 다시 setting해준다.
			tStartTime = tCurrentTime;
		} // if
	} // while
	
}
//===============================================================
// _DeleteMdb Method						   					=
//  : MDB를 검색해서 해당 시간이 지난 DATA는 일괄삭제한다.       		=
// argument     :  pBtreeNode -> Btree Root Node				=
//				   tCompareTime -> Observer가 동작할 시간			=
// return value : 1-> success									=
//                0-> fail										=
//																=
//===============================================================
int CObserver::_DeleteMdb( _BTREENODE* pBtreeNode, time_t tCompareTime )
{
	unsigned int 	unBaseAddress;       
	unsigned int 	unCurrentAddress;  
	_DATATYPE 		stDataType; 
	int				nTempCount=0; 
	
	*ObserverLogM << ObserverLogM->ErrorCode(0) 
		<< "DeleteMdb" << endl;
	if( pBtreeNode )
	{
	//	*ObserverLogM << ObserverLogM->ErrorCode(0) 
	//		<< "=====>>iNumberKey = " << pBtreeNode->iNumberKey << endl;
		// 처리하지 말아야 하는 UDR의 갯수를 알아내기 위한 부분.
		// 비교조건에서 필요함. 2000. 12. 14
		for( int i=0; i<pBtreeNode->iNumberKey; i++ )
			if( pBtreeNode->kIndexKey[ i].pUdrFormat->get_ItemHead()->get_nUdrStatus() == 0 )
				nTempCount++;
	//	*ObserverLogM << ObserverLogM->ErrorCode(0) 
	//		<< "=====>>nTempCount = " << nTempCount << endl;
		 i=0;
		 while ( 1 )
		 {
		 	if ( pBtreeNode->iNumberKey ==  nTempCount )
		 		break;
			 // 이미 처리가 종료된 상태라면..
			 if( pBtreeNode->kIndexKey[ i].pUdrFormat->get_ItemHead()->get_nUdrStatus() == 1 )
			 {
			 	
			 	// 입력된 시간보다 MDB의 시간이 작다면..(너무 오랫동안 UDR이 들어오지 않았다면.)
				if( tCompareTime >= pBtreeNode->kIndexKey[ i].pUdrFormat->get_ItemG()->get_nEndEventTime() )
				{
					nProcessingCount++;
					*ObserverLogM << ObserverLogM->ErrorCode(0) 
					 	<< "=====>>MsId = " << pBtreeNode->kIndexKey[ i].iDataKey << endl;
					
					stDataType.iDataKey = pBtreeNode->kIndexKey[ i].iDataKey;
				    stDataType.pUdrFormat = 0x00;
				    
					// Memory DB, Backup file, Btree에서 삭제해준다.
					unCurrentAddress=(unsigned int)pBtreeNode->kIndexKey[ i].pUdrFormat;
				    unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();
					 
				    //Btree index를 초기화 시켜준다.
				     memset((char *)pBtreeNode->kIndexKey[ i].pUdrFormat,0x00,sizeof(UdrFormat));
				
					fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
					fInOut.write( ( char * )pBtreeNode->kIndexKey[i].pUdrFormat, sizeof( UdrFormat ) );
					fInOut.flush();
					
				    // Btree data를 delete.
				    pBtree->DelNode(stDataType);
				    pAllocDataIndexC->deleteIndex( (( unCurrentAddress-unBaseAddress )/sizeof( UdrFormat )) );
				  	// Memory에서 삭제하면 하나씩 앞으로 당겨주기 때문에
				  	// index i를 감소시켜준다.
				    if( i != 0 )
					{
						i--;
					}
				} // if
			} // if
			else
			{
				i++;
			}
		 } // while
		 // 다음 Node를 검색한다.
		 for(  i=0; i<=pBtreeNode->iNumberKey; i++ )
		 {
			if( !_DeleteMdb( pBtreeNode->pBTREENODE[i], tCompareTime ) )
			{
				*ObserverLogM << ObserverLogM->ErrorCode(0) 
					<< "Observer : _DeleteMdb 실패.!!! " << endl;
				return 0;
			}
		 } // for
	}// if
	
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
int CObserver::_StopProcessMdb( _BTREENODE* pBtreeNode, time_t tCompareTime )
{
	unsigned int 	unBaseAddress;       
	unsigned int 	unCurrentAddress;  
	_DATATYPE 		stDataType;  
	
	*ObserverLogM << ObserverLogM->ErrorCode(0) 
		<< "StopProcessMdb" << endl;
	
	if( pBtreeNode )
	{
		// *ObserverLogM << ObserverLogM->ErrorCode(0) 
		//	<< "=====>>iNumberKey = " << pBtreeNode->iNumberKey << endl;
		 for( int i=0; i<pBtreeNode->iNumberKey; i++ )
		 {
			 // 현재 처리가 진행중인 UDR중에서..
			 if( pBtreeNode->kIndexKey[i].pUdrFormat->get_ItemHead()->get_nUdrStatus() == 0 )
			 {
			 	// 입력된 시간보다 MDB의 시간이 작다면..(너무 오랫동안 UDR이 들어오지 않았다면.)
				if( tCompareTime >= pBtreeNode->kIndexKey[i].pUdrFormat->get_ItemG()->get_nEndEventTime() )
				{
					nProcessingCount++;
					*ObserverLogM << ObserverLogM->ErrorCode(0) 
					 	<< "=====>>MsId = " << pBtreeNode->kIndexKey[i].iDataKey << endl;
					
					// 처리를 완료했다고 flag를 setting한후...
					pBtreeNode->kIndexKey[i].pUdrFormat->get_ItemHead()->put_nUdrStatus(1);
					
					// 해당 backup file도 update해준다.
					unCurrentAddress=(unsigned int)pBtreeNode->kIndexKey[i].pUdrFormat;
				    unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();
					
					fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
					fInOut.write( ( char * )pBtreeNode->kIndexKey[i].pUdrFormat, sizeof( UdrFormat ) );
					fInOut.flush();
					
					// Collector MQ에 해당 UDR을 보내준다.
					while(1)
					{
						if(MqUdr->sendMsg( pBtreeNode->kIndexKey[i].pUdrFormat ) == FLAG_MQ_IS_FULL)
						{
							*ObserverLogM << ObserverLogM->ErrorCode(0) 
								<< "Aggregator->Collector MSG Queue FULL!!" << endl;
							usleep( 100 );
						//	 return 0;
						}
						else
						{
							*ObserverLogM << ObserverLogM->ErrorCode(0) 
								<< "send data"  << endl;
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
				*ObserverLogM << ObserverLogM->ErrorCode(0) 
					<< "Observer : _StopProcessMdb 실패!!! " << endl;
				return 0;
			}
		} // for
	} // if
	return 1;
}
//===============================================================
// _AllProcessing Method					   					=
//  : MDB를 검색해서 해당시간이 지난 처리중인 DATA는 STOP UDR처리한후,	=
//	 삭제하고, 처리한 DATA는 MDB에서 삭제한다.						=
// argument     :  pBtreeNode -> Btree Root Node				=
//				   tCompareTime -> Observer가 동작할 시간			=
// return value : 1-> success									=
//                0-> fail										=
//																=
//===============================================================
int CObserver::_AllProcessing( _BTREENODE* pBtreeNode, time_t tCompareTime )
{
	unsigned int 	unBaseAddress;       
	unsigned int 	unCurrentAddress;  
	_DATATYPE 		stDataType; 
	
	*ObserverLogM << ObserverLogM->ErrorCode(0) 
		<< "_AllProcessing" << endl;
	if( pBtreeNode )
	{
	//	*ObserverLogM << ObserverLogM->ErrorCode(0) 
	//		<< "=====>>iNumberKey = " << pBtreeNode->iNumberKey << endl;
		 int i=0;
		 while( pBtreeNode->iNumberKey != 0 )
		 {
		 	 // 처리하고 있는 상태라면..
			 if( pBtreeNode->kIndexKey[i].pUdrFormat->get_ItemHead()->get_nUdrStatus() == 0 )
			 {
			 	// 입력된 시간보다 MDB의 시간이 작다면..(너무 오랫동안 UDR이 들어오지 않았다면.)
			 	// STOP 처리한후, MDB에서 삭제한다.
				if( tCompareTime >= pBtreeNode->kIndexKey[i].pUdrFormat->get_ItemG()->get_nEndEventTime() )
				{
					nProcessingCount++;
					*ObserverLogM << ObserverLogM->ErrorCode(0) 
					 	 << "=====>>MsId = " << pBtreeNode->kIndexKey[i].iDataKey << endl;
					PrintUdr.Print( pBtreeNode->kIndexKey[i].pUdrFormat );
					
					// 처리를 완료했다고 flag를 setting한후...
					pBtreeNode->kIndexKey[i].pUdrFormat->get_ItemHead()->put_nUdrStatus(1);
					
					// Collector MQ에 해당 UDR을 보내준다.
					while(1)
					{
						if(MqUdr->sendMsg( pBtreeNode->kIndexKey[i].pUdrFormat ) == FLAG_MQ_IS_FULL)
						{
							*ObserverLogM << ObserverLogM->ErrorCode(0) 
								<< "Aggregator->Collector MSG Queue FULL!!" << endl;
							usleep( 100 );
						//	 return 0;
						}
						else
						{
							*ObserverLogM << ObserverLogM->ErrorCode(0) 
								<< "send data"  << endl;
							break;
						}
					}
					
					stDataType.iDataKey = pBtreeNode->kIndexKey[i].iDataKey;
				    stDataType.pUdrFormat = 0x00;
				    
					// 해당 backup file도 update해준다.
					// *********************>>>>>>>>>>>.memory lock 개념이 있어야 함..
					unCurrentAddress=(unsigned int)pBtreeNode->kIndexKey[i].pUdrFormat;
				    unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();
					
					 //Btree index를 초기화 시켜준다.
				     memset((char *)pBtreeNode->kIndexKey[i].pUdrFormat,0x00,sizeof(UdrFormat));
					
					fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
					fInOut.write( ( char * )pBtreeNode->kIndexKey[i].pUdrFormat, sizeof( UdrFormat ) );
					fInOut.flush();
					
					 // Btree data를 delete.
				    pBtree->DelNode(stDataType);
				
				    pAllocDataIndexC->deleteIndex( (( unCurrentAddress-unBaseAddress )/sizeof( UdrFormat )) );
				    
				}// if
			 }// if
			 // 이미 처리가 종료된 상태라면..
			 else if( pBtreeNode->kIndexKey[i].pUdrFormat->get_ItemHead()->get_nUdrStatus() == 1 )
			 {
			 	// 입력된 시간보다 MDB의 시간이 작다면..(너무 오랫동안 UDR이 들어오지 않았다면.)
			 	// MDB에서 삭제
				if( tCompareTime >= pBtreeNode->kIndexKey[i].pUdrFormat->get_ItemG()->get_nEndEventTime() )
				{
					nProcessingCount++;
					cout << "nProcessingCount=" << nProcessingCount << endl;
					*ObserverLogM << ObserverLogM->ErrorCode(0) 
					 	<< "=====>>MsId = " << pBtreeNode->kIndexKey[i].iDataKey << endl;
					PrintUdr.Print( pBtreeNode->kIndexKey[i].pUdrFormat );
					
					stDataType.iDataKey = pBtreeNode->kIndexKey[i].iDataKey;
				    stDataType.pUdrFormat = 0x00;
				    
					// Memory DB, Backup file, Btree에서 삭제해준다.
					unCurrentAddress=(unsigned int)pBtreeNode->kIndexKey[i].pUdrFormat;
				    unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();
				
				    //Btree index를 초기화 시켜준다.
				    memset((char *)pBtreeNode->kIndexKey[i].pUdrFormat,0x00,sizeof(UdrFormat));
					
					fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
					fInOut.write( ( char * )pBtreeNode->kIndexKey[i-1].pUdrFormat, sizeof( UdrFormat ) );
					fInOut.flush();
					
				    // Btree data를 delete.
				    pBtree->DelNode(stDataType);
				
				    pAllocDataIndexC->deleteIndex( (( unCurrentAddress-unBaseAddress )/sizeof( UdrFormat )) );
				    
				}// if
			}// else if
			else
			{
				cout << "예기치 않은 상황==> 메모리의 값이 잘못되어 있음." << endl;
			}
		 } //while
		for( i=0; i<=pBtreeNode->iNumberKey; i++ )
		{
			if( !_AllProcessing( pBtreeNode->pBTREENODE[i], tCompareTime ) )
			{
				*ObserverLogM << ObserverLogM->ErrorCode(0) 
					<< "Observer : _AllProcessing 실패.!!! " << endl;
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
	ObserverLogM	= new SendLogManage( SystemId,ServiceId,GroupId ,ProcessType ,PserialNo, PreMark );
	ObserverLogM->put_EventHistData( FLAG_START, FLAG_PROC_START );
	
	//signal(SIGNULL,sigCapture);	//0
	signal(SIGHUP,sigCapture);
	signal(SIGINT,sigCapture);
	signal(SIGQUIT,sigCapture);
	signal(SIGKILL,sigCapture);
	signal(SIGTERM,sigCapture);
	signal(SIGSTOP,sigCapture);
	signal(SIGTSTP,sigCapture);
	signal(SIGCONT,sigCapture);	//26
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
	ObserverLogM->put_EventHistData(FLAG_END, 0);
	delete ObserverLogM;
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
				*ObserverLogM << ObserverLogM->ErrorCode(0) 
					<< "Observer Process Killed!! " <<endl;
				WritePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				delete mqPMS;
				ObserverLogM->put_EventHistData(FLAG_END, -1);
				delete ObserverLogM;
				exit(-1);
				break;
		case SIGUSR1:
				*ObserverLogM << ObserverLogM->ErrorCode(0) 
					<< "Observer Process Normal Stop!! " <<endl;
				WritePMSMQ(FLAG_PROC_TERM);//0//프로세스정상stop (Client업무명령)
				delete mqPMS;
				ObserverLogM->put_EventHistData(FLAG_END, 0);
				delete ObserverLogM;
				exit(0);
				break;
		default:
				*ObserverLogM << ObserverLogM->ErrorCode(0) 
					<< "Observer Process Killed2!! " <<endl;
				WritePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				delete mqPMS;
				ObserverLogM->put_EventHistData(FLAG_END, -1);
				delete ObserverLogM;
				exit(-1);
				break;
	}
}




