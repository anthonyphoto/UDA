/******************************************************************
* 파 일 명     
      : parsermainDB.cpp                
* 기   능                                                          
     : Radius로 부터 들어온 xml형태의 UDR을 parsing한 후, 
       UDR format에 맞게 변형한다.
       그런 다음, [t_trans_code] table에서 읽어 온 전송여부코드에 따라
       해당 MQ로 UDR을 전송한다. 
* 작 성 자     
     : 신미연                         
* first data  
     : 2000. 10.15                      
* last updata 
     : 2001. 04. 30                  
* 관 련 화 일  
     : 
* program history                                
                           
* Version
  : 1.1 ( 2001. 05. 21 )                                           
******************************************************************/


#include "parser.h"
#include "t_trans_code.hpp"
#include <unistd.h>
#include <TempMsgQ_file.hpp>
#include <StatusFile.hpp>
#include <MsgFormat.hpp>
#include <signal.h>
#include <SendLogManage.hpp>
#include <SqlCommon.hpp>

#define	SVC_PROVIDER_017	"017"

MessageQueue<ReadBuffer>	*readbufR;		// Radius에서 원시 UDR이 들어 오는 MQ 
MessageQueue<SendBuffer> 	*sendbufS;		// Parsing된 UDR을 011로 보내기 위한 MQ
MessageQueue<SendBuffer> 	*sendPdsn;		// Parsing된 UDR을 017로 보내기 위한 MQ
MessageQueue<MsgPmsStatus>  *PMSMQ;			// PMS MQ
SendLogManage				*clsLogM;		// Log를 남기기 위함	 
SendBuffer					sbuf;			// Parsing된 UDR을 011 or 017로 보내기 위한 데이타

int		systemId;							// System Id
int		serviceId;							// Service Id
int		groupId;							// Group Id
int		processType;						// Process Type
int		pserialNo;							// Process Serial Number
int		pRemark;							// Process Remark

void Send011();								// SKT로 Parsing된 Udr을 보내기 위함
void Send017();								// STi로 Parsing된 Udr을 보내기 위함
void writePMSMQ(int sigNo);
void sigCapture(int sigNo);					// Signal 처리


//==============================================================================
int main(int argc, char **argv)
{
	if(argc != 7 )
    {
		cout << "Usage: parser [sysId][svcId][gId][pId][pSn][pRemark]" <<endl;
    	exit(-1);
    }

//	signal(SIGNULL, sigCapture);
	signal(SIGHUP,  sigCapture);	
	signal(SIGINT,  sigCapture);
	signal(SIGQUIT, sigCapture);
	signal(SIGKILL, sigCapture);
	signal(SIGSTOP, sigCapture);
	signal(SIGTSTP, sigCapture);
	signal(SIGCONT, sigCapture);
	signal(SIGUSR1, sigCapture);
	
	systemId    = atoi(argv[1]);
	serviceId   = atoi(argv[2]);
	groupId     = atoi(argv[3]);
	processType = atoi(argv[4]);
	pserialNo   = atoi(argv[5]);
	pRemark     = atoi(argv[6]); 

	// radius to parser
    readbufR = new MessageQueue<ReadBuffer>(systemId, serviceId, groupId, processType, pserialNo, 10, 1000);
    // parser to filter
    sendbufS = new MessageQueue<SendBuffer>(systemId, serviceId, 3, 1, 0, 10, 1000);
	sendPdsn = new MessageQueue<SendBuffer>(systemId, serviceId, 6, 5, 0, 10, 1000);
	PMSMQ 	 = new MessageQueue<MsgPmsStatus>(systemId, 0, 1, 1, 0, 10, 1000);
    clsLogM	 = new SendLogManage(systemId, serviceId, groupId, processType, pserialNo, pRemark);
    
	*clsLogM << clsLogM->ErrorCode(0) << "=====START Parser=====" << endl;

    ConvUdr				cudr;
    ReadBuffer			rbuf;    
	Rec_Trans_Code		clsRecTrans;
	T_Trans_Code		clsTTrans;

	int		i				= 0;	
	int		cnt				= 0;	// Radius로 부터 들어온 한 파일에서 Parsing된 Udr 갯수
    int		inputudrcount	= 0;	// Radius로 부터 들어온 한 파일에서 총 Udr 갯수
    int		nTrans017		= 0;	// 017일경우 Udr 전송 Flag를 저장	
    char	logbuf[79];
    char	strProcID[50];			// DB로 부터 값을 읽어들이기 위한 Process Id를 저장
    char	strTransCode[4];
    
    memset(strProcID   , 0x00, sizeof(strProcID));
    memset(strTransCode, 0x00, sizeof(strTransCode));
	sprintf(strProcID, "%d%d%d%d%02d%02d", systemId, serviceId, groupId, processType, pserialNo, pRemark);    

	DbConnect	clsDbConnect;
	if(clsDbConnect.connect() != 0)
	{
		*clsLogM << "Error No = [" << clsLogM->ErrorCode(1047) << "] Fail DB Connect" << endl;
		clsLogM->put_EventHistData(FLAG_END, -1);
		delete readbufR;
		delete sendbufS;
		delete sendPdsn;
		delete PMSMQ;
		delete clsLogM;
		exit(-1);
	}	
		
	// [t_trans_code] table에서 Parsing된 Udr 전송 여부를 읽어온다.
	//--------------------------------------------------------
    clsRecTrans.set_svc_provider(SVC_PROVIDER_017); 
    if(clsTTrans.Get_Record(&clsRecTrans) < 0)
    {
 		*clsLogM << "Error No = [" << clsLogM->ErrorCode(1048) << "] [t_trans_code] Fail Get Record" << endl;
		clsLogM->put_EventHistData(FLAG_END, -1);
		delete readbufR;
		delete sendbufS;
		delete sendPdsn;
		delete PMSMQ;
		delete clsLogM;
		clsDbConnect.disconnect();
		exit(-1);
	}	  
  
    memcpy(strTransCode, clsRecTrans.get_trans_flag(), 3);
    nTrans017 = atoi(strTransCode);  
	//--------------------------------------------------------//
	clsDbConnect.disconnect();

	*clsLogM << clsLogM->ErrorCode(0) << "-017 Trans Code : " << nTrans017 << endl;

	clsLogM->put_EventHistData(FLAG_START, FLAG_PROC_START);
	writePMSMQ(FLAG_PROC_START);

    while(1)
    {
	    memset ((char*)&rbuf, 0x00, sizeof(rbuf));

        if (readbufR->recvMsg(&rbuf) == FLAG_MQ_IS_EMPTY)
        {
            usleep(1000);
            continue;
        }
        
		clsLogM->put_PerformanceData(FLAG_START, 0, rbuf.fileseqno, 0, 0, 0);

        cudr.readQueue(rbuf);
		*clsLogM << clsLogM->ErrorCode(0) << "Receive from RadiusNo[" << cudr.getProcessSN() <<  "]" << " FileSequenceNo[" << rbuf.fileseqno << "]" << endl;

        if ( cudr.convert() == FALSE )
            continue;

		clsLogM->put_PerformanceData(FLAG_END, 0, rbuf.fileseqno, cudr.getCount("P"), cudr.getCount("D"), 0);

        memset(logbuf,0x00,sizeof(logbuf));
        sprintf(logbuf, "FSEQ [ %10u ] TOTAL [ %02d ] PROCESS [ %02d ] DROP [ %02d ]\n",
                cudr.getFileSeq(), cudr.getCount("T"), cudr.getCount("P"), cudr.getCount("D"));

        i = 0;
        int nSvcProvider = 0;
        cnt = cudr.getCount("P");

        while (i < cnt)
        {
            cudr.getByIndex(sbuf, i);
			nSvcProvider = cudr.getServiceProvider(i);
			*clsLogM << clsLogM->ErrorCode(0) << "-service Provider : " << nSvcProvider << endl;
			if(nSvcProvider == 11)
			{
				*clsLogM << clsLogM->ErrorCode(0) << "-Send to SKT..." << endl;
				Send011();
			}	

			else if(nSvcProvider == 17)
			{
				switch(nTrans017)
				{
					case 11 :
							*clsLogM << clsLogM->ErrorCode(0) << "-Send to SKT..." << endl;
							Send011();
							*clsLogM << clsLogM->ErrorCode(0) << "-Send to STi..." << endl;
							Send017();
							break;
					case 10 :
							*clsLogM << clsLogM->ErrorCode(0) << "-Send to SKT..." << endl;
							Send011();
							break;
					case  1 :
							*clsLogM << clsLogM->ErrorCode(0) << "-Send to STi..." << endl;
							Send017();
							break;
					case  0 :
							*clsLogM << clsLogM->ErrorCode(0) << "-Send to SKT..." << endl;
							Send011();
							*clsLogM << clsLogM->ErrorCode(0) << "-Send to STi..." << endl;
							Send017();
							break;
				}
			}	

            i++;
            memset((char*)&sbuf, 0x00, sizeof(sbuf));

            inputudrcount++;
            nSvcProvider = 0;
        }

        i = 0;
        cudr.clear();

    } // end of while
	
	clsLogM->put_EventHistData(FLAG_END, EXIT_SUCCESS);
	delete readbufR;
	delete sendbufS;
	delete sendPdsn;
	delete PMSMQ;
	delete clsLogM;
    return EXIT_SUCCESS;
}




//------------------------------------------------------------------------------
// SKT로 Parsing된 Udr을 보내기 위함
void Send011()
{
	while(1)
	{
       	if (sendbufS->sendMsg(&sbuf) == FLAG_MQ_IS_FULL)
       	{
			*clsLogM << "Error No = [" << clsLogM->ErrorCode(1049) << "] =====Send to filter=====Waiting=====" << endl;
//			*clsLogM 	<< "LOG : =====Send to filter=====Waiting===== " 	<< endl;
            usleep(1000);                
            continue;
        } 
        break;
     }  
}




//------------------------------------------------------------------------------
// STi로 Parsing된 Udr을 보내기 위함
void Send017()
{
	while(1)
	{
       	if (sendPdsn->sendMsg(&sbuf) == FLAG_MQ_IS_FULL)
       	{
			*clsLogM << "Error No = [" << clsLogM->ErrorCode(1050) << "] =====Send to Reforamt=====Waiting=====" << endl;
//			*clsLogM 	<< "LOG : =====Send to Reforamt=====Waiting===== " 	<< endl;
            usleep(1000);                 
            continue;
         }
        break;
     }  
}




//------------------------------------------------------------------------------
void writePMSMQ(int sigNo)
{
	int i = 0;
	char PMSMQ_NO[2];
	MsgPmsStatus	OneRec(MSG_STATUS, systemId, serviceId, groupId, processType, pserialNo, 10);

	OneRec.set_nStatus(sigNo);
	
	sprintf(PMSMQ_NO, "%d", systemId);
	
	while(1)
	{
		i++;
		if (PMSMQ->sendMsg(&OneRec) == FLAG_MQ_IS_FULL)
		{
			*clsLogM << clsLogM->ErrorCode(0) << "PMSMQIS FULL sleep(1)...!" << endl;
			sleep(1);
			if (i >= 10)
			{
				kill(0,SIGINT);
			}
		}
		else
		{
			break;
		}
	}
}




//------------------------------------------------------------------------------
void sigCapture(int sigNo)
{
	*clsLogM << clsLogM->ErrorCode(0) << "Got Signal...Process End!!!" << endl;
	switch(sigNo)
	{
		case SIGINT :
		case SIGQUIT :
		case SIGKILL :
		case SIGTERM :
				writePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				clsLogM->put_EventHistData(FLAG_END, -1);
				delete readbufR;
				delete sendbufS;
				delete sendPdsn;
				delete PMSMQ;				
				delete clsLogM;
				exit(-1);
				break;
		case SIGUSR1:
				writePMSMQ(FLAG_PROC_TERM);//0//프로세스정상stop (Client업무명령)
				clsLogM->put_EventHistData(FLAG_END, 0);
				delete readbufR;
				delete sendbufS;
				delete sendPdsn;
				delete PMSMQ;				
				delete clsLogM;
				exit(0);
				break;
		default:
				writePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				clsLogM->put_EventHistData(FLAG_END, -1);
				delete readbufR;
				delete sendbufS;
				delete sendPdsn;
				delete PMSMQ;				
				delete clsLogM;
				exit(-1);
				break;
	}
}
