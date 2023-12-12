/******************************************************************
* 파 일 명     
      : LogMGR.pc                
* 기   능                                                          
     : 에러 메세지, process event, udr 통계정보등을 관리한다.
* 작 성 자     
     : 조상영                         
* first data  
     :                    
* last updata 
     :                 
* 관 련 화 일  
     : 
* program history                                

* Version
  : 1.1 ( 2001. 05. 21 )                                           
******************************************************************/
 


#include <iostream.h>
#include <stdio.h>
#include <RecvLogManage.hpp>
//#include <SendLogManage.hpp>

MessageQueue<MsgPmsStatus>* mqPMS;	// PMS MQ define
//SendLogManage*				LogM;

static int  SystemId;
static int  ServiceId;
static int  GroupId;
static int	ProcessType;
static int	PserialNo;
static int	PreMark;


//==============================================================================
RecvLogManage::RecvLogManage(int nSystemId, int	nServiceId)
{
	perForm	= new ProcPerformData(nSystemId,nServiceId, 1, 1, 1, 1);
	performMQ = new MessageQueue<ProcPerformData>(nSystemId,nServiceId, 1, 2, 0, 11, 1000);	// 136
	
	eventHst = new ProcEventData(nSystemId,nServiceId, 1, 1, 1, 1);  
	eventHstMQ = new MessageQueue<ProcEventData>(nSystemId,nServiceId, 1, 2, 0, 12, 1000); // 92
    
	errorLog = new ProcErrInfo(nSystemId,nServiceId, 1, 1, 1, 1);
	errorLogMQ = new MessageQueue<ProcErrInfo>(nSystemId,nServiceId, 1, 2, 0, 13, 1000); // 676
    /*        
	custHst = new CustHist(..);
	custHstMQ = new MessageQueue<CustHist>(1, 1, 1, 2, 0, 10, 1000);
            
	observHst = new ObservHist(..);
	observHstMQ = new MessageQueue<ObservHist>(1, 1, 1, 2, 0, 10, 1000);
	*/
	EmptyN=0;
}



//--------------------------------------------------------------------
RecvLogManage::~RecvLogManage(void)
{
	delete errorLog;
	delete errorLogMQ;
	
	delete eventHst;
	delete eventHstMQ;
	
	delete perForm;
	delete performMQ;
	
	
    /*       
	delete custHst;
	delete custHstMQ;
           
	delete observHst;
	delete observHstMQ;
	*/
}

//--------------------------------------------------------------------
void RecvLogManage::WritePerForm(void)
{
	int 	i;
	
	//memset(perForm, 0x00, sizeof(ProcPerformData));
	if(performMQ->recvMsg(perForm)==FLAG_MQ_IS_EMPTY) 
	{
		//cout << "[RecvLog] <perforMQ> NO DATA IN MessageQueue" << endl;
		//*LogM << "[RecvLog] <perforMQ> NO DATA IN MessageQueue " << LogM->ErrorCode(0) << endl;
		EmptyN++;
		return;
	}
		
	dbPerForm = new ProcPerform(perForm->get_nSystemId(), perForm->get_nSvcId(), perForm->get_nGrpType(),
						perForm->get_nProcType(), perForm->get_nProcSerial(), perForm->get_nProcRemark());
	
	//int	billtype,int fileseq, int udrcount,int querycount, char	*sdate,char *edate);

	i=dbPerForm->Set_PerForm(perForm->get_unFileSeq(), perForm->get_nUdrCount(), 
						perForm->get_nFailUdrCount(), perForm->get_nQuery_Count(), perForm->get_strProcessTime());
	
	delete dbPerForm;
	EmptyN=0;
	
	// smy 2001. 06. 12 (changed for log arrangement)
	if(i != 0)
	{	
		sprintf(strLog, "[%1d_%1d_%02d_%02d] T_PROC_PERFORM Update Ret = %d", perForm->get_nGrpType(), perForm->get_nProcType(), 
								perForm->get_nProcSerial(), perForm->get_nProcRemark(), i); 
		WriteSelfLog();
	}	
	
}


//--------------------------------------------------------------------
void RecvLogManage::WriteEventHst(void)
{
	int i;
	//memset(eventHst, 0x00, sizeof(ProcEventData));
	if(eventHstMQ->recvMsg(eventHst)==FLAG_MQ_IS_EMPTY) 
	{
		//cout << "[RecvLog] <eventHstMQ> NO DATA IN MessageQueue" << endl;
		//*LogM << "[RecvLog] <eventHstMQ> NO DATA IN MessageQueue " << LogM->ErrorCode(0) << endl;
		EmptyN++;
		return;
	}
	
	//cout << "eventHst->get_nSystemId() : " << eventHst->get_nSystemId() << endl;
	dbeventHst = new ProcEventHist(eventHst->get_nSystemId(), eventHst->get_nSvcId(), eventHst->get_nGrpType(),
						eventHst->get_nProcType(), eventHst->get_nProcSerial(), eventHst->get_nProcRemark());
	
	i=dbeventHst->Set_EventHist(eventHst->get_strEventDateTime(), eventHst->get_nExitCode());
	
	delete dbeventHst;
	EmptyN=0;
	
	// smy 2001. 06. 12 (changed for log arrangement)
	if(i != 0)
	{	
		sprintf(strLog, "[%1d_%1d_%02d_%02d] T_PROC_EVENT_HIST Update Ret = %d", eventHst->get_nGrpType(), eventHst->get_nProcType(), 
						eventHst->get_nProcSerial(), eventHst->get_nProcRemark(), i); 
		WriteSelfLog();
	}	
}




//--------------------------------------------------------------------
void RecvLogManage::WriteErrorLog(void)
{
	int i;
	
	//memset(errorLog, 0x00, sizeof(ProcErrInfo));
	//cout << "ERRORLOG MQ 1: " << errorLog->get_strErrDetail()<< endl;
	if(errorLogMQ->recvMsg(errorLog)==FLAG_MQ_IS_EMPTY) 
	{
		//cout << "[RecvLog] <errorLogMQ> NO DATA IN MessageQueue" << endl;
		//*LogM << "[RecvLog] <errorLogMQ> NO DATA IN MessageQueue " << LogM->ErrorCode(0) << endl;
		EmptyN++;
		return;
	}
	
	// FILE Write
	//i=errorLog->write_ErrorLog();	// sycho 2000.12.06

	//sprintf(strLog, "[%1d_%1d_%02d_%02d] T_ERROR_LOG File Write [%d]", 	errorLog->get_nGrpType(),errorLog->get_nProcType(), 
	//					errorLog->get_nProcSerial(), errorLog->get_nProcRemark(), i); 
	//WriteSelfLog();
	
	
	// DB Write
	if(atoi(errorLog->get_nErrCode())!=0)
	{
		dberrorLog = new ErrorLog(errorLog->get_nSystemId(), errorLog->get_nSvcId(), errorLog->get_nGrpType(),
						errorLog->get_nProcType(), errorLog->get_nProcSerial(), errorLog->get_nProcRemark());
	
		i=dberrorLog->Set_ErrorLog(errorLog->get_strEventDate(),errorLog->get_strEventTime(),
					errorLog->get_nErrCode() ,errorLog->get_strFileName(), errorLog->get_strErrDetail());
				
		delete dberrorLog;
		
		// smy 2001. 06. 12 (changed for log arrangement)
		if(i != 0)
		{			
			sprintf(strLog, "[%1d_%1d_%02d_%02d] T_ERROR_LOG Update Ret = %d", 	errorLog->get_nGrpType(),errorLog->get_nProcType(), 
							errorLog->get_nProcSerial(), errorLog->get_nProcRemark(), i); 
			WriteSelfLog();
		}
	}
	
	EmptyN=0;
}


/*
//--------------------------------------------------------------------
void RecvLogManage::WriteObservHst(void)
{
	if(observHstMQ->recvMsg(observHst)==FLAG_MQ_IS_EMPTY) 
	{
		cout << "[RecvLog] <observHstMQ> NO DATA IN MessageQueue" << endl;
		return;
	}
		
	dbobservHst = new ObservHist();
	
	//int SetObservHist(int trantype, int trancount, char *sdate, char *edate);
	dbobservHst->SetObservHist();
	
	delete dbobservHst;	
}


//--------------------------------------------------------------------
void RecvLogManage::WriteCustHst(void)
{

	if(custHstMQ->recvMsg(custHst)==FLAG_MQ_IS_EMPTY) 
	{
		cout << "[RecvLog] <custHstMQ> NO DATA IN MessageQueue" << endl;
		return;
	}
		
	dbcustHst = new CustHist();
	
	//int SetCustHist(int svctype, int custtype, char *strdate);
	dbcustHst->SetCustHist();
	
	delete dbcustHst;	
}
*/

//--------------------------------------------------------------------
int RecvLogManage::WriteLog(void)
{
	WritePerForm();
	WriteEventHst();
	WriteErrorLog();
	if(EmptyN>=3) 
	{
		sleep(1);
		//cout << "sleep(1)" << endl;
		EmptyN=0;
	}
	
	return 1;
	//WriteObservHst();
	//WriteCustHst();
}


//--------------------------------------------------------------------
void RecvLogManage::WriteSelfLog(void)
{
	char		strFileName[100];
	char		strDayTime[30];
	//char		strEventDate[10];
	//char		strEventTime[10];
	DateTime	clsDateTime;

	//memset(strEventDate, 0x00, sizeof(strEventDate));
	//memset(strEventTime, 0x00, sizeof(strEventTime));
	
	//memcpy(strEventDate, clsDateTime.get_date(), 8);
	//memcpy(strEventTime, clsDateTime.get_time(), 6);
	strcpy(strDayTime, clsDateTime.get_DateTime(1)); //"%s %s", clsDateTime.get_date(), clsDateTime.get_time(1));
	
	sprintf(strFileName, 	"/SVC%1d/LOG/%1d_%1d_%02d_%02d.%8s", 
							ServiceId,	
							GroupId, 
							ProcessType,
							PserialNo,	
							PreMark,	
							clsDateTime .get_date());
							
	
	ofstream	ErrLogFile(strFileName, ios::out |  ios::app);

	
	if (!ErrLogFile) return;
	
	//ErrLogFile 	<<	"<"	<< strEventDate << "-" << strEventTime << "> "
				//<<	strLog << endl;

	ErrLogFile 	<<	"["	<< strDayTime << "] " <<	strLog << endl;				
				
	ErrLogFile.close();
	
}


//--------------------------------------------------------------------
void RecvLogManage::SelfEvent(int first)	// LogMGR 이 시작하고 종료할때 남긴다.
{

	if(first) strcpy(strLog, "[LogMGR] START EVENT ============================== "); 
	else strcpy(strLog, "[LogMGR] END  EVENT  ================================ "); 

	WriteSelfLog();
}


//==============================================================================
RecvLogManage	*clsLogM;


void WritePMSMQ(int sigNo)
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
			//cout << "[ERROR] PMSMQ " << PMSMQ_NO << " IS FULL sleep(1)...!"	<< endl;
			sleep(1);
			if (i >= 10)
			{
				kill(0,SIGINT);
			}
		}else break;
	}
}


//--------------------------------------------------------------------
void sigCapture(int sigNo)
{
	//cout << "SIGNAL :: RecvLogManage Process End " << endl;
	clsLogM->SelfEvent(0);
	delete clsLogM;
	
	switch(sigNo)
	{
		case SIGINT :
		case SIGQUIT :
		case SIGKILL :
		case SIGTERM :
				WritePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				//LogM->put_EventHistData(FLAG_END, -1);
				//delete LogM;
				delete mqPMS;
				exit(-1);
				break;
		case SIGUSR1:
				WritePMSMQ(FLAG_PROC_TERM);//0//프로세스정상stop (Client업무명령)
				//LogM->put_EventHistData(FLAG_END, 0);
				//delete LogM;
				delete mqPMS;
				exit(0);
				break;
		default:
				WritePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				//LogM->put_EventHistData(FLAG_END, -1);
				//delete LogM;
				delete mqPMS;
				exit(-1);
				break;
	}
}


//--------------------------------------------------------------------
int main(int argc, char *argv[])
{
	if(argc <7)
	{
		cout <<"Usage:" << argv[0] << "[systemid] [serviceid] [groupid] [processtype] [serialno] [remarks]" << endl;
		exit(-2);
	}
	
	SystemId=atoi(argv[1]);
	ServiceId=atoi(argv[2]);
	GroupId=atoi(argv[3]);
	ProcessType=atoi(argv[4]);
	PserialNo=atoi(argv[5]);
	PreMark=atoi(argv[6]);
	
	
	//signal(SIGNULL,sigCapture);	//0
	signal(SIGHUP,sigCapture);
	signal(SIGINT,sigCapture);
	signal(SIGQUIT,sigCapture);
	signal(SIGKILL,sigCapture);
	signal(SIGTERM,sigCapture);
	signal(SIGSTOP,sigCapture);
	signal(SIGTSTP,sigCapture);
//	signal(SIGCONT,sigCapture);
	signal(SIGUSR1,sigCapture);
	
	int				nRet;
	DbConnect		clsDbConnect;
	//RecvLogManage	clsLogM;
	
	clsLogM = new RecvLogManage(SystemId, ServiceId);
	mqPMS = new MessageQueue<MsgPmsStatus>(atoi(argv[1]), 0,1,1,0,10, 1000);
	//LogM = new SendLogManage(SystemId, ServiceId, GroupId, ProcessType, PserialNo, PreMark);
	//LogM->put_EventHistData(FLAG_START, 0);
	
	clsLogM->SelfEvent(1);
	WritePMSMQ(FLAG_PROC_START);

	nRet = clsDbConnect.connect();
	if (nRet < 0)
	{
		//cout << "DB Connect Failure [" << nRet << "]" << endl;
		return nRet;
	}
	//else cout << "DB Connect Success " << endl;
	
	while(1) clsLogM->WriteLog();
	
	clsDbConnect.disconnect();
	
	//LogM->put_EventHistData(FLAG_END, 1);
	//delete LogM;
	clsLogM->SelfEvent(0);
	
	delete clsLogM;
	delete mqPMS;
	exit(1);
}

