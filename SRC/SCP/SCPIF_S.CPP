/******************************************************************
* 파 일 명     
      : scpif_s.cpp                
* 기   능                                                          
     : SCP-IF : S-Channel 메인 프로그램
* 작 성 자     
     : 조상영                         
* first data  
     : 2000. 10. 23                      
* last updata 
     : 2000. 10. 23                  
* 관 련 화 일  
     : SRchannel_IF.hpp, scp_msg.hpp scpformat.hpp	
* program history                                
                           
* Version
  : 1.1 ( 2001. 05. 21 )                                           
******************************************************************/


#include "SRchannel_IF.hpp"
//#include <TempMsgQ_new.hpp>
#include <TempMsgQ_file.hpp>
#include <MsgFormat.hpp>
#include "TReqScp.hpp"

int	SystemId;
int	ServiceId;
static int	GroupId;
static int	ProcessType;
static int 	PserialNo;
static int	PreMark;

//--------------------------------------------------------------------
class scpIF_SMAIN {
	
	private:
		SRchannel_IF 				*schIF;
		TReqScp						tblSCP;		// AGG 요청을 DB에 저장한다.
		SCPFormatAgg				fmSCP;		// AGG -> scpif_s (MQ)
		SCPFormat					fmSCPIF;	// scpif_s -> SCP	
		ReCustFormat				fmReCust;	// scpif_s -> scpif_r (채널간의 연동을 위해)
		
		void	Mq_Put_Data();		/* TEST CODE..!! */
		
		int 	dbWriteAggReq();	// MQ에서 받은 Agg한도요청을 DB에 저장한다. : 0 (저장에러)
		int		dbGetAggReq();		// DB에 저장한 한도요청을 가져온다. : 0 (NO data)
		
		
	public:
		scpIF_SMAIN(char *addr, int nport);	//SCP S-Channel 주소및 포트
		~scpIF_SMAIN(void){ delete schIF; }
		
		void Run(void);						//통신시작
		void DisConnection(void)
		{
			schIF->DoDisConnection();
		}
};

//--------------------------------------------------------------------

//MessageQueue<SCPFormatAgg> mqSCP("scpformat.txt",    3000,  AGG_JOBID,  AGG_PROID, SCP_JOBID, SCP_PROID);
//MessageQueue<ReCustFormat> mqReCUST("custtoscp.txt",  3000,  GUE_JOBID, GUE_PROID, SCP_JOBID2, SCP_PROID);
MessageQueue<SCPFormatAgg>* mqSCP;		// AGG -> SCPIF_S
MessageQueue<ReCustFormat>* mqReCUST;	// SCPIF_S ->SCPIF_R
MessageQueue<MsgPmsStatus>* mqPMS;
scpIF_SMAIN*	sMain;
SendLogManage*	LogM;



//==============================================================================
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
			cout << "[ERROR] PMSMQ " << PMSMQ_NO << " IS FULL sleep(1)...!"	<< endl;
			sleep(1);
			if (i >= 10)
			{
				kill(0,SIGINT);
			}
		}else break;
	}
}



//==============================================================================
void sigCapture(int sigNo)
{
	cout << "SIGNAL :: Schannel-IF Process End " << endl;
	sMain->DisConnection();
	delete sMain;
	delete mqReCUST;
	delete mqSCP;
	
	switch(sigNo)
	{
		case SIGINT :
		case SIGQUIT :
		case SIGKILL :
		case SIGTERM :
				WritePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				LogM->put_EventHistData(FLAG_END, -1);
				delete LogM;
				delete mqPMS;
				exit(-1);
				break;
		case SIGUSR1:
				WritePMSMQ(FLAG_PROC_TERM);//0//프로세스정상stop (Client업무명령)
				LogM->put_EventHistData(FLAG_END, 0);
				delete LogM;
				delete mqPMS;
				exit(0);
				break;
		default:
				WritePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				LogM->put_EventHistData(FLAG_END, -1);
				delete LogM;
				delete mqPMS;
				exit(-1);
				break;
	}
}




//--------------------------------------------------------------------
scpIF_SMAIN::scpIF_SMAIN(char *addr, int nport)
{
	schIF = new SRchannel_IF(addr, nport, SCHANNEL);
}



//--------------------------------------------------------------------
void scpIF_SMAIN::Mq_Put_Data()
{
	
	// 0x01 sendMsg
	memset((char*)&fmSCP, 0x00, sizeof(fmSCP));
	fmSCP.get_ItemA2()->set_Key(0x01);
	//fmSCP.get_ItemA()->set_MSID("0112823925     ");	
	fmSCP.get_ItemA2()->set_Min(114300240);	
	fmSCP.get_ItemB()->set_OperId(100);
	fmSCP.get_ItemB()->set_AccSessionId("01234567");
	fmSCP.get_ItemB()->set_CorreId("ABCDEFGH");

	mqSCP->sendMsg(&fmSCP);
	
	
	// 0x02 sendMsg
	memset((char*)&fmSCP, 0x00, sizeof(fmSCP));
	fmSCP.get_ItemA2()->set_Key(0x02);
	//fmSCP.get_ItemA()->set_MSID("0112825555     ");	
	fmSCP.get_ItemA2()->set_Min(114300240);	
	//fmSCP.get_ItemA2()->set_Min(172825555);	
	fmSCP.get_ItemB()->set_OperId(200);
	fmSCP.get_ItemB()->set_AccSessionId("77777777");
	fmSCP.get_ItemB()->set_CorreId("AAAAAAAA");
	fmSCP.get_ItemC()->set_StartT(600000);
	fmSCP.get_ItemC()->set_EndT(600100);
	fmSCP.get_ItemD()->set_LimitT(1000);
	fmSCP.get_ItemD()->set_NTuseT(500);
	mqSCP->sendMsg(&fmSCP);	
	
	
	/*
	// 0x02 sendMsg
	memset((char*)&fmSCP, 0x00, sizeof(fmSCP));
	fmSCP.get_ItemA2()->set_Key(0x02);
	//fmSCP.get_ItemA()->set_MSID("0112820000     ");	
	fmSCP.get_ItemA2()->set_Min(1132820000);	
	fmSCP.get_ItemB()->set_OperId(200);
	fmSCP.get_ItemB()->set_AccSessionId("@888888@");
	fmSCP.get_ItemB()->set_CorreId("#CCCCCC#");
	fmSCP.get_ItemC()->set_StartT(800);
	fmSCP.get_ItemC()->set_EndT(400);
	fmSCP.get_ItemD()->set_LimitT(800);
	fmSCP.get_ItemD()->set_NTuseT(400);
	mqSCP.sendMsg(&fmSCP);	
	
	*/
	
	// 0x03 sendMsg
	memset((char*)&fmSCP, 0x00, sizeof(fmSCP));
	fmSCP.get_ItemA2()->set_Key(0x03);
	fmSCP.get_ItemA2()->set_Min(114300240);	
	//fmSCP.get_ItemA2()->set_Min(1122826666);	
	//fmSCP.get_ItemA()->set_MSID("0112826666     ");	
	mqSCP->sendMsg(&fmSCP);
	
	
	
	
	// 0x04 sendMsg
	memset((char*)&fmSCP, 0x00, sizeof(fmSCP));
	fmSCP.get_ItemA2()->set_Key(0x04);
	fmSCP.get_ItemA2()->set_Min(114300240);	
	//fmSCP.get_ItemB()->set_OperId(300);
	//fmSCP.get_ItemB()->set_AccSessionId("88888888");
	//fmSCP.get_ItemB()->set_CorreId("BBBBBBBB");
	fmSCP.get_ItemC()->set_StartT(700000);
	fmSCP.get_ItemC()->set_EndT(700100);
	fmSCP.get_ItemD()->set_LimitT(1000);
	fmSCP.get_ItemD()->set_NTuseT(600);
	mqSCP->sendMsg(&fmSCP);	
		
}


	
//--------------------------------------------------------------------
// MQ에서 받은 Agg한도요청을 DB에 저장한다. : 0 (저장에러)	
int scpIF_SMAIN::dbWriteAggReq()
{
	memset((char*)&fmSCP, 0x00, sizeof(fmSCP));
	if(mqSCP->recvMsg(&fmSCP) == FLAG_MQ_IS_EMPTY)	// AGG로부터 메시지수신
	{
		// cout << "[SCHANNEL] mqSCP NO DATA IN MessageQueue" << endl;
		//*LogM << "[SCHANNEL] mqSCP NO DATA IN MessageQueue :" << LogM->ErrorCode(0) << endl;
		return 1;	
	}
	
	
	if(!tblSCP.Add_ScpRecord(fmSCP))
	{
		cout << "[SCHANNEL] T_REQ_SCP Table Insert ERROR " << endl;
		*LogM << "[SCHANNEL] T_REQ_SCP Table Insert ERROR " << LogM->ErrorCode(0) << endl;
		//schIF->DoDisConnection();
		//sleep(RECONNECT_WAIT_TIME);
		//first=1;
		return 0;
	}	
	
	return 1;
}


//--------------------------------------------------------------------
// DB에 저장한 한도요청을 가져온다. : 0 (NO data)
int scpIF_SMAIN::dbGetAggReq()
{
	if(!tblSCP.Get_ScpRecord(&fmSCPIF, TDay, TransacNum)) return 0;
	strcpy(TDay, tblSCP.Get_Day());	
	TransacNum=tblSCP.Get_Tid();
	
	cout << "[SCHANNEL] Get T_Req_SCP  : " << TDay << " : " << TransacNum << endl;
	*LogM << "[SCHANNEL] Get T_Req_SCP : " << TDay << " : " << TransacNum << LogM->ErrorCode(0) << endl;
	return 1;
}


//--------------------------------------------------------------------
void scpIF_SMAIN::Run(void)
{
	int  i,re, idle;
	int  first=1;
	int  datamiss=0;	// MQ에서 받은 메시지 전송실패 (제전송필요)
	int	 conn=0;		// 연결시도횟수
	int	 msgon=1;
	
	/* TEST CODE..!! */
   	//Mq_Put_Data();	
	/* TEST CODE..!! */

	first=1;
	sleep(1);
	while(1)
	{
		if(first) {
			if(schIF->DoConnection()==FA) 
			{	// 연결실패하면 30초후에 다시시도.. 
				cout << "[SCHANNEL] Connection ERROR!! Reconneting..!! " << endl;
				*LogM << "[SCHANNEL] Connection ERROR!! Reconneting..!! :" << LogM->ErrorCode(8202) << endl;
				WritePMSMQ(FLAG_NW_ERROR);
				
				conn++;
				if(conn>3)
				{	// 연결이 연속적으로 3번이상 실패하면 R체널도 끊고 다시 연결하도록 한다.
					fmReCust.set_Result(0);
					fmReCust.set_UTransactionID(0);
					mqReCUST->sendMsg(&fmReCust);
				}
				
				sleep(RECONNECT_WAIT_TIME);
				msgon=1;
				//dbWriteAggReq();	// SCP와 연결이 끊어져도 요청은 계속받는다.
				continue;
			}
			conn=i=idle=first=0;
			WritePMSMQ(FLAG_NW_NORMAL);
		}
		
		// IMSI...R-CHANNEL 붙기까지 기달린다.		
		//sleep(5);
		
		if(idle>90) // 20->90
   		{	// 전송할 메시지가 한동안 없으면 서버상태를 확인한다.
			idle=0;
			if(schIF->ProcessCHK()!=SC) 
   			{
   				cout << "[SCHANNEL] Process-CHK ERROR!! Reconneting..!! " << endl;
   				*LogM << "[SCHANNEL] Process-CHK ERROR!! Reconneting..!! :" << LogM->ErrorCode(8203) << endl;
   				first=1;
   				
   				// R채낼 종료요청
   				fmReCust.set_Result(0);
				fmReCust.set_UTransactionID(0);
				mqReCUST->sendMsg(&fmReCust);
				
   				sleep(RECONNECT_WAIT_TIME);
   				continue;
   			}   				
  			msgon=1;
  			
  			/* TEST CODE..!! */
   			// Mq_Put_Data();	
			/* TEST CODE..!! */
   		} 
   		
   		
   		// Agg 로부터 MQ를 통해받은 한도요청을 테이블에 기록한다.
   		if(!dbWriteAggReq())
   		{
   			schIF->DoDisConnection();
			sleep(RECONNECT_WAIT_TIME);
			first=1;
		}
   		
   		
		if(!datamiss)
		{	// 한도요청 테이블에 보낼 데이터가 있나 검색한다.
			if(!dbGetAggReq())
			{
				if(msgon==1)
				{
					cout << "[SCHANNEL] NO DATA IN : Table_ReqSCP" << endl;
					*LogM << "[SCHANNEL] NO DATA IN : Table_ReqSCP " << LogM->ErrorCode(0) << endl;
				}
				
				msgon=0;
				idle++;
				sleep(1);
				continue;
			}
		} 
		
		
		msgon=1;
		idle=0;

		if((re=schIF->DownLoad(&fmSCPIF))!=SC) 
  		{
   			if(re!=RELEASE_CLOSE) 
   			{
   				cout << "[SCHANNEL] DownLoad ERROR!! Reconneting..!! " << endl;
   				*LogM << "[SCHANNEL] DownLoad ERROR!! Reconneting..!!" << LogM->ErrorCode(0) << endl;
   			}
 			first=1;
   			datamiss=1;
   				
   			// R채낼 종료요청
   			fmReCust.set_Result(0);
			fmReCust.set_UTransactionID(0);
			mqReCUST->sendMsg(&fmReCust);
				
   			sleep(RECONNECT_WAIT_TIME);
   		}else datamiss=0;
   
	} // end of while(1)
	
	
	schIF->DoDisConnection();
}




//==============================================================================
int main(int argc, char **argv)
{
	int  i, idle;
	int  first=1;
	
	if(argc !=9)
	{
		cout <<"Usage:" << argv[0] << "[systemid] [serviceid] [groupid] [processtype] [serialno] [remarks] [ipaddr] [port]" << endl;
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
	signal(SIGCONT,sigCapture);	//26
	signal(SIGUSR1,sigCapture);
	

	mqSCP = new MessageQueue<SCPFormatAgg>(SystemId, ServiceId, GroupId, ProcessType, PserialNo, 10, 1000);//AGG
	mqReCUST = new MessageQueue<ReCustFormat>(SystemId, ServiceId, GroupId, ProcessType, PserialNo, 11, 1000);	//SCP_IF
	mqPMS = new MessageQueue<MsgPmsStatus>(SystemId, 0,1,1,0,10, 1000);
	LogM = new SendLogManage(SystemId, ServiceId, GroupId, ProcessType, PserialNo, PreMark);
	
	LogM->put_EventHistData(FLAG_START, FLAG_PROC_START);
	WritePMSMQ(FLAG_PROC_START);
	
	*LogM << "Argv : " << SystemId << "/" << ServiceId << "/" << GroupId <<"/" << ProcessType 
		<< "/" << PserialNo <<"/" << PreMark << "/" << argv[7] <<"/" << atoi(argv[8]) << endl;
	
	
	sMain = new scpIF_SMAIN(argv[7], atoi(argv[8]));
	sMain->Run();

	LogM->put_EventHistData(FLAG_END, 1);
	delete sMain;
	
	delete LogM;
	delete mqReCUST;
	delete mqSCP;
	delete mqPMS;
	
	exit(1);
}
	




