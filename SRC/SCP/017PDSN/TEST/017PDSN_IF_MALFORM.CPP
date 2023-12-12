/******************************************************************
* 파 일 명     
      : 017pdsn_if.cpp                
* 기   능                                                          
     : Reformat으로 부터 받은 017을 STI UDA로 전송한다.
* 작 성 자     
     : 조상영                         
* first data  
     :                     
* last updata 
     : 2000. 05.25 	(snmp 추가 수정 : jung seung jin)
* 관 련 화 일  
     : 017pdsn_if.hpp	
* program history                                
                           
* Version
  : 1.2 ( 2001. 05. 25 )                                           
******************************************************************/


#include <intndefs.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <strings.h>

#include <socketudpC.hpp>
#include <TempMsgQ_file.hpp>
#include <MsgFormat.hpp>
#include <SendLogManage.hpp>


#include "017pdsn_if.hpp"
#include "hexdump.h"



//MessageQueue<UdrFormat>* mqUDR;
MessageQueue<MsgPmsStatus>* mqPMS;
SendLogManage*	LogM;
Pdsn_IF*		pdMain;

static int	SystemId;
static int	ServiceId;
static int	GroupId;
static int	ProcessType;
static int 	PserialNo;
static int	PreMark;
int			gnId; // 재전송 여부를 check하기위한 변수. snmp 관련 2001.05.28 jkjung

unsigned int JulianTime()
{
	time_t time1;
	time1 = time(NULL);
	
	return	(unsigned int)time1;
}

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
//			cout << "[ERROR] PMSMQ " << PMSMQ_NO << " IS FULL sleep(1)...!"	<< endl;
			sleep(1);
			if (i >= 10)
			{
				kill(0,SIGINT);
			}
		}else break;
	}
}


//---------------------------------------------------------------------------------
void sigCapture(int sigNo)
{
//	cout << "017PDSN-IF Process End " << endl;
	*LogM<< "017PDSN-IF Process End " << LogM->ErrorCode(0) << endl;

	//delete mqUDR;
	delete pdMain;
	
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


//==============================================================================
Pdsn_IF::Pdsn_IF(char *mdkey, char *serv_addr, int serv_port)
{
	strcpy(Serv_Addr, serv_addr); 
	Serv_Port = serv_port;
	strcpy(Md5Key, mdkey);
	
	Socket_Create();
	
	memset((char*)&sHeader, 0x00, sizeof(sHeader));
	memset((void *)&rHeader,0x00, sizeof(rHeader));
	
	configM = new ConfigCTRL(ServiceId, PserialNo, 0, Md5Key);

	NoAckN=0;
	//configM->set_iNum();
}


//-------------------------------------------------------------------------------------
Pdsn_IF::~Pdsn_IF()
{
	if(!SOCK_ERROR) delete _socket;	
	
	delete configM;
}


//-------------------------------------------------------------------------------------
int Pdsn_IF::Socket_Create()
{
	int ret;
	
	_socket = new SockC(Serv_Addr, Serv_Port);
	ret = _socket->CreateUDPClient(Serv_Addr, Serv_Port, (char *)"150.25.4.140", 1914); //Local_Addr, Local_Port);	
//	ret = _socket->CreateUDPClient(Serv_Addr, Serv_Port, (char *)"150.2.26.200", 1914); //Local_Addr, Local_Port);	
	
//	cout << "[017PDSNIF] Create PDSN Server : " << ret;
	*LogM << "[017PDSNIF] Create PDSN Server [" << Serv_Addr <<":" << Serv_Port << "]" <<  LogM->ErrorCode(0) <<endl;
	
	if(ret < 0) 
	{
		delete _socket;
		SOCK_ERROR = 1;
	}else SOCK_ERROR = 0;
	
	return ret;	
}



//-------------------------------------------------------------------------------------
int Pdsn_IF::RecvAck()
{
	int recvLen;
	
		
	*LogM << "<<<<    RecvData 017PDSN         >>>>" << LogM->ErrorCode(0) << endl;
	
	//recvLen = _socket->RecvfromUDPSocket(sockBuf, 1024, 0);
	recvLen = _socket->RecvfromUDPSocketEx(sockBuf, HEAD_SIZE, 0);
	if(recvLen < HEAD_SIZE) 
	{

		*LogM << "Recv Size Error : "<< recvLen <<  LogM->ErrorCode(0) << endl << endl;
		NoAckN++;
		if(NoAckN==6) WritePMSMQ(FLAG_NW_ERROR);
		
		if(recvLen==0) 
		{
			//	snmp 사용시 추가 된 내용 (5. 25, jsj) -- time out unTimeOutCnt, unReTransCnt 감소 
			/* snmp (5. 25)****************************************/
			nRet = snmp7IF->FindMyNas();
			snmp7IF->Set_TimeOutCnt(snmp7IF->Get_TimeOutCnt()+1);
			snmp7IF->Set_PendingCnt(snmp7IF->Get_PendingCnt()-1);
			snmp7IF->WritePerform();
			/* snmp (5. 25)****************************************/
			return 0;	// Time Out
		}
		else if (recvLen > 0)
		{
			//	snmp 사용시 추가 된 내용 (5. 25, jsj) -- header 오류 unMalformCnt 증가 
			/* snmp (5. 25)****************************************/
			nRet = snmp7IF->FindMyNas();
			snmp7IF->Set_MalformCnt(snmp7IF->Get_MalformCnt()+1);
			snmp7IF->WritePerform();
			/* snmp (5. 25)****************************************/
			return 0;
		}
		else
		{
			return -1;	// Soeck Error
		}
	}
	memcpy((char *)&rHeader, sockBuf, HEAD_SIZE);
	*LogM<< "Code       : " << (int)rHeader.Code << LogM->ErrorCode(0) << endl;
	*LogM<< "Identifier : " << (int)rHeader.Identifier << LogM->ErrorCode(0) << endl;
	*LogM<< "Length     : " << rHeader.Length << LogM->ErrorCode(0) << endl;
	
	if((rHeader.Code == 5) && (rHeader.Identifier == sHeader.Identifier))
	{
		*LogM << "RecvData : Ack OK " <<  LogM->ErrorCode(0) << endl << endl;
		configM->read_Udr_COMMIT();
		if(NoAckN>5) WritePMSMQ(FLAG_NW_NORMAL);
		NoAckN=0;

		//	snmp 사용시 추가 된 내용 (5. 25, jsj) -- ack unPendingCnt 감소, unResCnt 증가
		/* snmp (5. 25)****************************************/
		nRet = snmp7IF->FindMyNas();
		snmp7IF->Set_PendingCnt(snmp7IF->Get_PendingCnt()-1);
		snmp7IF->Set_ResCnt(snmp7IF->Get_ResCnt()+1);
		snmp7IF->WritePerform();
		/* snmp (5. 25)****************************************/

		return 1;
	}
	
	NoAckN++;
	if(NoAckN==6) WritePMSMQ(FLAG_NW_ERROR);

	//	snmp 사용시 추가 된 내용 (5. 25, jsj) -- 잘못된 data 처리 unUnKnownMsgCnt 증가 
	/* snmp (5. 25)****************************************/
	nRet = snmp7IF->FindMyNas();
	snmp7IF->Set_UnknownMsgCnt(snmp7IF->Get_UnknownMsgCnt()+1);
	snmp7IF->WritePerform();
	/* snmp (5. 25)****************************************/

	return -1;	// 잘못된 Data
}



//-------------------------------------------------------------------------------------
int Pdsn_IF::SendData()
{
	int		sendLen;
	
	if(configM->read_Udr(&stPdsnMsg)!=1) return 0;
	memcpy((char *)&sHeader, (char *)&stPdsnMsg, HEAD_SIZE);
	
	DATA_ERROR = 0;	
	*LogM<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << LogM->ErrorCode(0) << endl;
	*LogM<< "<<<<   SendData 017PDSN  : " <<  inet_ntoa((in_addr &)_socket->recv_addr.sin_addr.s_addr) << " >>>>" << LogM->ErrorCode(0) << endl;
	
	//recvLen = _socket->RecvfromUDPSocket(sockBuf, 4096, 0);
//	sendLen=_socket->SendtoUDPSocket((void *)&stPdsnMsg, sHeader.Length);
	sendLen=_socket->SendtoUDPSocket((void *)&stPdsnMsg, 100);
		
	*LogM<< "Code       : " << (int)sHeader.Code << LogM->ErrorCode(0) << endl;
	*LogM<< "Identifier : " << (int)sHeader.Identifier << LogM->ErrorCode(0) << endl;
	*LogM<< "Length     : " << sHeader.Length << LogM->ErrorCode(0) << endl;
	*LogM<< "sendLen    : " << sendLen << LogM->ErrorCode(0) << endl;
	
	//*LogM<< "Authenticator : " << LogM->ErrorCode(0) << endl;
	
	//cout << "-------HEX DUMP----------------------" << endl;
	//HexDump(sockBuf, recvLen); // @@@@ 
//	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;

	//	snmp 사용시 추가 된 내용 (5. 25, jsj) -- send unPendingCnt 증가 
	/* snmp (5. 25)****************************************/
	if(gnId == (int)sHeader.Identifier ) { //재전송시
		nRet = snmp7IF->FindMyNas();
		snmp7IF->Set_RetransCnt(snmp7IF->Get_RetransCnt()+1);
		snmp7IF->WritePerform();
	} else { // Identifier가 같지 않으면 전송된 것임
		nRet = snmp7IF->FindMyNas();
		snmp7IF->Set_PendingCnt(snmp7IF->Get_PendingCnt()+1);
		snmp7IF->Set_SndUdrCnt(snmp7IF->Get_SndUdrCnt()+1);
		snmp7IF->WritePerform();
		/* snmp (5. 25)****************************************/
	}
	
	gnId = (int)sHeader.Identifier;
	
	return 1;
}


	
//====================================================================================
int main(int argc, char *argv[])
{
	//	snmp 사용시 추가 된 내용 (5. 25, jsj) -- time keeping, alias 정의 
	/* snmp (5. 25)****************************************/
	unsigned int nSendTime=0;
	unsigned int nAckTime=0;
	int			 nRetmain;

	char 	strAlias[64];
	memset (strAlias, 0x00, 64);
	sprintf(strAlias, "%s%s%s%s%s", argv[1], argv[2], "7IF", argv[5], "X");

	snmp7IF = new CSnmpCli(argv[7], (unsigned int)atoi(argv[8]), strAlias);
	/* snmp (5. 25)****************************************/

	if(argc < 10)
	{
		cout <<"Usage:" << argv[0] << "[systemid] [serviceid] [groupid] [processtype] [serialno] [remarks] [ipaddr] [port] [key]" << endl;
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
	
	
	//mqUDR = new MessageQueue<UdrFormat>(1, 1, 3,1,0,10,1000);
	mqPMS = new MessageQueue<MsgPmsStatus>(SystemId, 0,1,1,0,10, 1000);
	LogM = new SendLogManage(SystemId, ServiceId, GroupId, ProcessType, PserialNo, PreMark);
	pdMain = new Pdsn_IF(argv[9], argv[7], atoi(argv[8]));	
	//pdMain = new Pdsn_IF((char *)MD5_HASH_KEY, (char *)SERV_UDP_ADDR, SERV_UDP_PORT);
	
	LogM->put_EventHistData(FLAG_START, FLAG_PROC_START);
	WritePMSMQ(FLAG_PROC_START);
	
//	cout << "Argv : " << SystemId << "/" << ServiceId << "/" << GroupId <<"/" << ProcessType 
//		<< "/" << PserialNo <<"/" << PreMark << "/" << argv[7] <<"/" << atoi(argv[8]) 
//		<< "/" << argv[9] <<  endl;
		
	*LogM << "Argv : " << SystemId << "/" << ServiceId << "/" << GroupId <<"/" << ProcessType 
		<< "/" << PserialNo <<"/" << PreMark << "/" << argv[7] <<"/" << atoi(argv[8]) 
		<< "/" << argv[9] << LogM->ErrorCode(0) << endl;
	
	snmp7IF->FileChkNMake();
	
	while(pdMain->SOCK_ERROR)
	{
		WritePMSMQ(FLAG_NW_ERROR);
		sleep(30);
		pdMain->Socket_Create();
	}
	
	
	WritePMSMQ(FLAG_NW_NORMAL);
	while(1)
	{
				//	test용 
		sleep(2);
		if(!pdMain->SendData()) 
		{
			//break;	// fp 
//			cout << "[017PDSNIF] sleep 2..." << endl;		// 5.25 (cout 삭제 jsj)
			sleep(1);
			continue;
		}
		else
		{
			//	snmp 사용시 추가 된 내용 (5. 25, jsj) -- time interval unTimeInterval 계산 
			/* snmp (5. 25)****************************************/
			if ((nAckTime != 0)&&(nSendTime!=0))		// 	둘다 0이 아닐경우 계산한다. 
			{
				nRetmain = snmp7IF->FindMyNas();
				snmp7IF->Set_TimeInterval(nAckTime - nSendTime);
				snmp7IF->WritePerform();
			}
			/* snmp (5. 25)****************************************/
			nSendTime = JulianTime();
			
		}
		


		if(pdMain->RecvAck()<0) 
		{
			if(pdMain->RecvAck()>=0)	// Socket Error 나 잘못된 Data면 한번더 recv
				nAckTime = JulianTime();
				
		}
		else
		{
//			sleep(2);	//test용
			nAckTime = JulianTime();
		}

			
	}
	
	LogM->put_EventHistData(FLAG_END, 1);
	
	delete pdMain;
	delete LogM;
	delete mqPMS;
	delete snmp7IF;
	//delete mqUDR;
	
	
	exit(1);
}

