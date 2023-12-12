/******************************************************************
* �� �� ��     : Observer.cpp 			     					  *
* ��   ��                                                          *
* : �����ð����� memory db�� �˻��ؼ� ���ǿ� �´� UDR��                 *
*    Message Queue�� �����ų� ����� ������ �ϴ� Progream.	   		  *
* �� �� ��     : Han Guen Hee                    				  *
* first data  : 2000. 9. 20        	     						  *
* last updata : 2001. 01. 05        		 					  *
* �� �� ȭ ��  : observer.hpp, OBS_FlagDate.cfg					  *
* program history                        						  *
* 	Observer ������ ������� �ʵ��� ��.	-> 2001. 01. 05		 		  *
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
//  : ���۽ð��� config������ �о�´�.                        		=
// argument : ���μ��� ���󱸺� 6����								=
// return value : None											=
//																=
//===============================================================
CObserver::CObserver(int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark  )
{
	// Observer ���۽ð��� �����ش�.
	tStartTime = time( NULL );
	
	// ������� �ѵ������߰������� �ؾ��ϴ����� ��Ÿ���� 
	// Config data�� �о���� �κ�.
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
	
	// ������� �ѵ������߰������� �ؾ��ϴ����� ��Ÿ���� 
	// Config data�� �о���� �κ�.
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
//  : �ܺο��� Observer�� ȣ���ϴ� Method                     		=
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
	
	// �д����� ó�������� �ʴ����� �ٲ��ش�.
	unSecInterval = unInterval * 60;
	while( 1 )
	{
		sleep(10);
		// ����ð��� �����ش�.
		tCurrentTime = time( NULL );
		// start�ð����� ����ð��� �� ���� ���Ѵ�.
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
	
		// ó�����ݸ�ŭ�� �ð��� �����ٸ�...
		if( unSubTime >= unSecInterval )
		{
			tProcessingTime = tCurrentTime - ( unApplicationTime * 60 );
			// 10���� MDB�� ���ʷ� ó���Ѵ�.
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
				// Memory Lock������ �־�� �Ѵ�.
				pBtree->mem_lock(); // MEMORY LOCK
				// Btree�� Root Pointer�� ��´�.
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
					// MDB�� �˻��ؼ� �ش� �ð��� ���� DATA�� �ϰ������Ѵ�.
					// MDB�� BackUp file���� �����Ѵ�.
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
					// MDB�� �˻��ؼ� �ش�ð��� ���� DATA�� STOP UDRó���Ѵ�.
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
					// MDB�� �˻��ؼ� �ش�ð��� ���� DATA�� STOP UDRó������
					// MDB�� BackUp file���� �����Ѵ�.
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
							    << "���� Flag�� �߸� �ԷµǾ���." << endl;
						break;
				} // switch
				pBtree->mem_unlock(); // MEMORY LOCK
				// attach�� �޸𸮸� detach�����ش�.
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
			// Start�ð��� �ٽ� setting���ش�.
			tStartTime = tCurrentTime;
		} // if
	} // while
	
}
//===============================================================
// _DeleteMdb Method						   					=
//  : MDB�� �˻��ؼ� �ش� �ð��� ���� DATA�� �ϰ������Ѵ�.       		=
// argument     :  pBtreeNode -> Btree Root Node				=
//				   tCompareTime -> Observer�� ������ �ð�			=
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
		// ó������ ���ƾ� �ϴ� UDR�� ������ �˾Ƴ��� ���� �κ�.
		// �����ǿ��� �ʿ���. 2000. 12. 14
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
			 // �̹� ó���� ����� ���¶��..
			 if( pBtreeNode->kIndexKey[ i].pUdrFormat->get_ItemHead()->get_nUdrStatus() == 1 )
			 {
			 	
			 	// �Էµ� �ð����� MDB�� �ð��� �۴ٸ�..(�ʹ� �������� UDR�� ������ �ʾҴٸ�.)
				if( tCompareTime >= pBtreeNode->kIndexKey[ i].pUdrFormat->get_ItemG()->get_nEndEventTime() )
				{
					nProcessingCount++;
					*ObserverLogM << ObserverLogM->ErrorCode(0) 
					 	<< "=====>>MsId = " << pBtreeNode->kIndexKey[ i].iDataKey << endl;
					
					stDataType.iDataKey = pBtreeNode->kIndexKey[ i].iDataKey;
				    stDataType.pUdrFormat = 0x00;
				    
					// Memory DB, Backup file, Btree���� �������ش�.
					unCurrentAddress=(unsigned int)pBtreeNode->kIndexKey[ i].pUdrFormat;
				    unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();
					 
				    //Btree index�� �ʱ�ȭ �����ش�.
				     memset((char *)pBtreeNode->kIndexKey[ i].pUdrFormat,0x00,sizeof(UdrFormat));
				
					fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
					fInOut.write( ( char * )pBtreeNode->kIndexKey[i].pUdrFormat, sizeof( UdrFormat ) );
					fInOut.flush();
					
				    // Btree data�� delete.
				    pBtree->DelNode(stDataType);
				    pAllocDataIndexC->deleteIndex( (( unCurrentAddress-unBaseAddress )/sizeof( UdrFormat )) );
				  	// Memory���� �����ϸ� �ϳ��� ������ ����ֱ� ������
				  	// index i�� ���ҽ����ش�.
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
		 // ���� Node�� �˻��Ѵ�.
		 for(  i=0; i<=pBtreeNode->iNumberKey; i++ )
		 {
			if( !_DeleteMdb( pBtreeNode->pBTREENODE[i], tCompareTime ) )
			{
				*ObserverLogM << ObserverLogM->ErrorCode(0) 
					<< "Observer : _DeleteMdb ����.!!! " << endl;
				return 0;
			}
		 } // for
	}// if
	
	return 1;
}
//===============================================================
// _StopProcessMdb Method					   					=
//  : MDB�� �˻��ؼ� �ش�ð��� ���� DATA�� STOP UDRó���Ѵ�.    		=
// argument     :  pBtreeNode -> Btree Root Node				=
//				   tCompareTime -> Observer�� ������ �ð�			=
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
			 // ���� ó���� �������� UDR�߿���..
			 if( pBtreeNode->kIndexKey[i].pUdrFormat->get_ItemHead()->get_nUdrStatus() == 0 )
			 {
			 	// �Էµ� �ð����� MDB�� �ð��� �۴ٸ�..(�ʹ� �������� UDR�� ������ �ʾҴٸ�.)
				if( tCompareTime >= pBtreeNode->kIndexKey[i].pUdrFormat->get_ItemG()->get_nEndEventTime() )
				{
					nProcessingCount++;
					*ObserverLogM << ObserverLogM->ErrorCode(0) 
					 	<< "=====>>MsId = " << pBtreeNode->kIndexKey[i].iDataKey << endl;
					
					// ó���� �Ϸ��ߴٰ� flag�� setting����...
					pBtreeNode->kIndexKey[i].pUdrFormat->get_ItemHead()->put_nUdrStatus(1);
					
					// �ش� backup file�� update���ش�.
					unCurrentAddress=(unsigned int)pBtreeNode->kIndexKey[i].pUdrFormat;
				    unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();
					
					fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
					fInOut.write( ( char * )pBtreeNode->kIndexKey[i].pUdrFormat, sizeof( UdrFormat ) );
					fInOut.flush();
					
					// Collector MQ�� �ش� UDR�� �����ش�.
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
		  // ���� Node�� �˻��Ѵ�.
		for(  i=0; i<=pBtreeNode->iNumberKey; i++ )
		{
			if( !_StopProcessMdb( pBtreeNode->pBTREENODE[i], tCompareTime ) )
			{
				*ObserverLogM << ObserverLogM->ErrorCode(0) 
					<< "Observer : _StopProcessMdb ����!!! " << endl;
				return 0;
			}
		} // for
	} // if
	return 1;
}
//===============================================================
// _AllProcessing Method					   					=
//  : MDB�� �˻��ؼ� �ش�ð��� ���� ó������ DATA�� STOP UDRó������,	=
//	 �����ϰ�, ó���� DATA�� MDB���� �����Ѵ�.						=
// argument     :  pBtreeNode -> Btree Root Node				=
//				   tCompareTime -> Observer�� ������ �ð�			=
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
		 	 // ó���ϰ� �ִ� ���¶��..
			 if( pBtreeNode->kIndexKey[i].pUdrFormat->get_ItemHead()->get_nUdrStatus() == 0 )
			 {
			 	// �Էµ� �ð����� MDB�� �ð��� �۴ٸ�..(�ʹ� �������� UDR�� ������ �ʾҴٸ�.)
			 	// STOP ó������, MDB���� �����Ѵ�.
				if( tCompareTime >= pBtreeNode->kIndexKey[i].pUdrFormat->get_ItemG()->get_nEndEventTime() )
				{
					nProcessingCount++;
					*ObserverLogM << ObserverLogM->ErrorCode(0) 
					 	 << "=====>>MsId = " << pBtreeNode->kIndexKey[i].iDataKey << endl;
					PrintUdr.Print( pBtreeNode->kIndexKey[i].pUdrFormat );
					
					// ó���� �Ϸ��ߴٰ� flag�� setting����...
					pBtreeNode->kIndexKey[i].pUdrFormat->get_ItemHead()->put_nUdrStatus(1);
					
					// Collector MQ�� �ش� UDR�� �����ش�.
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
				    
					// �ش� backup file�� update���ش�.
					// *********************>>>>>>>>>>>.memory lock ������ �־�� ��..
					unCurrentAddress=(unsigned int)pBtreeNode->kIndexKey[i].pUdrFormat;
				    unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();
					
					 //Btree index�� �ʱ�ȭ �����ش�.
				     memset((char *)pBtreeNode->kIndexKey[i].pUdrFormat,0x00,sizeof(UdrFormat));
					
					fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
					fInOut.write( ( char * )pBtreeNode->kIndexKey[i].pUdrFormat, sizeof( UdrFormat ) );
					fInOut.flush();
					
					 // Btree data�� delete.
				    pBtree->DelNode(stDataType);
				
				    pAllocDataIndexC->deleteIndex( (( unCurrentAddress-unBaseAddress )/sizeof( UdrFormat )) );
				    
				}// if
			 }// if
			 // �̹� ó���� ����� ���¶��..
			 else if( pBtreeNode->kIndexKey[i].pUdrFormat->get_ItemHead()->get_nUdrStatus() == 1 )
			 {
			 	// �Էµ� �ð����� MDB�� �ð��� �۴ٸ�..(�ʹ� �������� UDR�� ������ �ʾҴٸ�.)
			 	// MDB���� ����
				if( tCompareTime >= pBtreeNode->kIndexKey[i].pUdrFormat->get_ItemG()->get_nEndEventTime() )
				{
					nProcessingCount++;
					cout << "nProcessingCount=" << nProcessingCount << endl;
					*ObserverLogM << ObserverLogM->ErrorCode(0) 
					 	<< "=====>>MsId = " << pBtreeNode->kIndexKey[i].iDataKey << endl;
					PrintUdr.Print( pBtreeNode->kIndexKey[i].pUdrFormat );
					
					stDataType.iDataKey = pBtreeNode->kIndexKey[i].iDataKey;
				    stDataType.pUdrFormat = 0x00;
				    
					// Memory DB, Backup file, Btree���� �������ش�.
					unCurrentAddress=(unsigned int)pBtreeNode->kIndexKey[i].pUdrFormat;
				    unBaseAddress=(unsigned int)pAllocDataIndexC->get_unBaseAddress();
				
				    //Btree index�� �ʱ�ȭ �����ش�.
				    memset((char *)pBtreeNode->kIndexKey[i].pUdrFormat,0x00,sizeof(UdrFormat));
					
					fInOut.seekp( ( unCurrentAddress-unBaseAddress ) );
					fInOut.write( ( char * )pBtreeNode->kIndexKey[i-1].pUdrFormat, sizeof( UdrFormat ) );
					fInOut.flush();
					
				    // Btree data�� delete.
				    pBtree->DelNode(stDataType);
				
				    pAllocDataIndexC->deleteIndex( (( unCurrentAddress-unBaseAddress )/sizeof( UdrFormat )) );
				    
				}// if
			}// else if
			else
			{
				cout << "����ġ ���� ��Ȳ==> �޸��� ���� �߸��Ǿ� ����." << endl;
			}
		 } //while
		for( i=0; i<=pBtreeNode->iNumberKey; i++ )
		{
			if( !_AllProcessing( pBtreeNode->pBTREENODE[i], tCompareTime ) )
			{
				*ObserverLogM << ObserverLogM->ErrorCode(0) 
					<< "Observer : _AllProcessing ����.!!! " << endl;
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
	
	// Observer�� ���� Log Manager
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
	// Observer Method�� ȣ���Ѵ�.
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
//  : PMS���� Process�� ���¸� �����ϱ� ���� Method					=
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
//  : Process���� signal capture	�ϴ� Method      				=
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
				WritePMSMQ(FLAG_PROC_KILL);//-1//���μ��� ������ stop (killed)
				delete mqPMS;
				ObserverLogM->put_EventHistData(FLAG_END, -1);
				delete ObserverLogM;
				exit(-1);
				break;
		case SIGUSR1:
				*ObserverLogM << ObserverLogM->ErrorCode(0) 
					<< "Observer Process Normal Stop!! " <<endl;
				WritePMSMQ(FLAG_PROC_TERM);//0//���μ�������stop (Client��������)
				delete mqPMS;
				ObserverLogM->put_EventHistData(FLAG_END, 0);
				delete ObserverLogM;
				exit(0);
				break;
		default:
				*ObserverLogM << ObserverLogM->ErrorCode(0) 
					<< "Observer Process Killed2!! " <<endl;
				WritePMSMQ(FLAG_PROC_KILL);//-1//���μ��� ������ stop (killed)
				delete mqPMS;
				ObserverLogM->put_EventHistData(FLAG_END, -1);
				delete ObserverLogM;
				exit(-1);
				break;
	}
}



