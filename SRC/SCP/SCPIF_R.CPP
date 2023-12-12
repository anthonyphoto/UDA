/******************************************************************
* 파 일 명     
      : scpif_r.cpp                
* 기   능                                                          
     : SCP-IF : R-Channel 메인 프로그램
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


#include <iostream.h>
#include "SRchannel_IF.hpp"
//#include <TempMsgQ_new.hpp>
#include <TempMsgQ_file.hpp>
#include <SendLogManage.hpp>

int	SystemId;
int	ServiceId;
static int	GroupId;
static int	ProcessType;
static int 	PserialNo;
static int	PreMark;


//MessageQueue<UdrFormat> mqUDR("udr_r.txt",		  3000,  PARSER_JOBID, PARSER_PROID, FILTER_JOBID, FILTER_PROID);
//MessageQueue<CustFormat> mqCUST("scptocust.txt",  3000,  SCP_JOBID, SCP_PROID, GUE_JOBID, GUE_PROID);
//==============================================================================
class scpIF_RMAIN {
	
	private:
		SRchannel_IF 				*rchIF;
		ReCustFormat				fmReCust;	// scpif_s -> scpif_r (채널간의 연동을 위해)
		//CustFormat					fmCust;		// scpif_r -> 고객관리
		UdrFormat					UFormat;	// scpif_r -> filter
		
	public:
		scpIF_RMAIN(char *addr, int nport);	//SCP R-Channel 주소및 포트
		~scpIF_RMAIN(void){ delete rchIF; }
		
		void Run(void);						//통신시작
		void DisConnection(void)
		{
			rchIF->DoDisConnection();
		}
};
//==============================================================================

MessageQueue<UdrFormat>* mqUDR;
//MessageQueue<CustFormat>* mqCUST;
MessageQueue<MsgPmsStatus>* mqPMS;
MessageQueue<ReCustFormat>* mqReCUST;	// SCPIF_S ->SCPIF_R
scpIF_RMAIN* 	rMain;
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
	cout << "SIGNAL :: Rchannel-IF Process End " << endl;
	rMain->DisConnection();
	delete rMain;
	//delete mqCUST;
	delete mqUDR;
	delete mqReCUST;
	
	
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
scpIF_RMAIN::scpIF_RMAIN(char *addr, int nport)
{
	rchIF = new SRchannel_IF(addr, nport, RCHANNEL);
}



//--------------------------------------------------------------------
void scpIF_RMAIN::Run(void)
{
	int  i, msgid, re;
	int  first=1;
	int	 msgon=1;

	first=1;
	sleep(1);
	while(1)
	{
		if(first) {
			if(rchIF->DoConnection()==FA) 
			{	// 연결실패하면 30초후에 다시시도.. 
				cout << "[RCHANNEL] Connection ERROR!! Reconneting..!! " << endl;
				*LogM << "[RCHANNEL] Connection ERROR!! Reconneting..!! :" << LogM->ErrorCode(8201) << endl;
				WritePMSMQ(FLAG_NW_ERROR);
				sleep(RECONNECT_WAIT_TIME);
				continue;
			}
			i=first=0;
			WritePMSMQ(FLAG_NW_NORMAL);
			msgon=1;
			while(1) if(mqReCUST->recvMsg(&fmReCust) == FLAG_MQ_IS_EMPTY) break;	// ERROR MQ 비우기
		}
		
   		
		if((msgid=rchIF->RecvMsg2Rchannel())==FA) 
		{ 
			if(msgon){
				printf("[RCHANNEL] Read Time Out.. Retry..!! \n");
				*LogM << "[RCHANNEL] Read Time Out.. Retry..!! :" << LogM->ErrorCode(0) << endl;
			}
			if(mqReCUST->recvMsg(&fmReCust) != FLAG_MQ_IS_EMPTY)
			{
				if((!fmReCust.get_Result()) && (!fmReCust.get_UTransactionID()))
				{	// SCHANNEL 이 오래 끊겨있어서 RCHANNEL도 끊고 다시 연결한다.
					rchIF->DoDisConnection();
					first=1;
					sleep(RECONNECT_WAIT_TIME);
				}
			}
			msgon=0;
			continue; 
		}
		
		msgon=1;
		switch(msgid)
		{
			case DOWNLOAD_RESULT:		//Agg 요청에 대한 결과값 FILTER 앞큐에 보낸다.
				re=rchIF->Get_MQMsg(&UFormat);
				
				if(re)
				{	// 한도정보를 Q에 송신
					if(!(rchIF->ErrTID) && mqUDR->sendMsg(&UFormat) == FLAG_MQ_IS_FULL)
					{
						cout << "[RCHANNEL] mqUDR MessageQueue IS FULL" << endl;
						*LogM << "[RCHANNEL] mqUDR MessageQueue IS FULL :" << LogM->ErrorCode(0) << endl;
						rchIF->SendMsg2Server(DOWNLOAD_RESULT_ACK, FA);		
					}else rchIF->SendMsg2Server(DOWNLOAD_RESULT_ACK);		
				}else rchIF->SendMsg2Server(DOWNLOAD_RESULT_ACK);	// 처리에러를 SCP에서 수신함
				
				
				if(rchIF->ErrTID==2) first=1;
				break;
			/*
			case UPLOAD_REQ:	//SCP 고객정보변경 내용을 고객관리에 전송  
				rchIF->Get_MQMsg(&fmCust);	// SCP에서 받은 데이터를 큐형태로 얻는다.
				//printf("[RCHANNEL] ====SEND_CUST_Q=====  \n");
				//HexDump((char *)&fmCust,sizeof(CustFormat)); // @@@@ 
				if(mqCUST->sendMsg(&fmCust) == FLAG_MQ_IS_FULL)
				{
					cout << "[RCHANNEL] mqCUST - MessageQueue IS FULL" << endl;
					*LogM << "[RCHANNEL] mqCUST - MessageQueue IS FULL :" << LogM->ErrorCode(0) << endl;
					rchIF->SendMsg2Server(UPLOAD_REQ_ACK, FA);		
				}else rchIF->SendMsg2Server(UPLOAD_REQ_ACK);		
				break;	
			*/	
			case RCHANNEL_RELEASE_REQ:
				rchIF->SendMsg2Server(RCHANNEL_RELEASE_REQ_ACK);		
				
				rchIF->SockClose();
   				cout << "[RCHANNEL] SOCKET Close !! " << endl;
   				*LogM << "[RCHANNEL] SOCKET Close !! : " << LogM->ErrorCode(0) << endl;
   				first=1;
   				sleep(RECONNECT_WAIT_TIME);
				break;
		}
		
		//while(1) if(mqReCUST->recvMsg(&fmReCust) == FLAG_MQ_IS_EMPTY) break;	// MQ 비우기
		
	}// end of while(1)
	
	rchIF->DoDisConnection();
}

		


//==============================================================================
int main(int argc, char **argv)
{
	
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
	
	
	mqUDR = new MessageQueue<UdrFormat>(SystemId, ServiceId, 4,4,0,10,1000);//한도관리수정 2001.07.04 jkjung
	//mqCUST = new MessageQueue<CustFormat>(SystemId, ServiceId, 3,2,0,10,1000);
	mqPMS = new MessageQueue<MsgPmsStatus>(SystemId, 0,1,1,0,10, 1000);
	LogM = new SendLogManage(SystemId, ServiceId, GroupId, ProcessType, PserialNo, PreMark);
	mqReCUST = new MessageQueue<ReCustFormat>(SystemId, ServiceId, GroupId, ProcessType, PserialNo, 11, 1000);
	
	LogM->put_EventHistData(FLAG_START, FLAG_PROC_START);
	WritePMSMQ(FLAG_PROC_START);
	
	*LogM << "Argv : " << SystemId << "/" << ServiceId << "/" << GroupId <<"/" << ProcessType 
		<< "/" << PserialNo <<"/" << PreMark << "/" << argv[7] <<"/" << atoi(argv[8]) << endl;
	
	
	
	//scpIF_RMAIN *rMain;
	rMain = new scpIF_RMAIN(argv[7], atoi(argv[8]));
	
	rMain->Run();
	
	LogM->put_EventHistData(FLAG_END, 1);
	
	delete mqReCUST;
	
	delete LogM;
	delete rMain;
	//delete mqCUST;
	delete mqUDR;
	delete mqPMS;
	
	exit(1);
}
	
