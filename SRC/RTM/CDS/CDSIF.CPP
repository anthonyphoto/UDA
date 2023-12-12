
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned long magic;
  };
  typedef struct sql_cursor sql_cursor;
  typedef struct sql_cursor SQL_CURSOR;
#endif /* SQL_CRSR */

/* Thread Safety */
typedef void * sql_context;
typedef void * SQL_CONTEXT;

/* Object support */
struct sqltvn
{
  unsigned char *tvnvsn; 
  unsigned short tvnvsnl; 
  unsigned char *tvnnm;
  unsigned short tvnnml; 
  unsigned char *tvnsnm;
  unsigned short tvnsnml;
};
typedef struct sqltvn sqltvn;

struct sqladts
{
  unsigned int adtvsn; 
  unsigned short adtmode; 
  unsigned short adtnum;  
  sqltvn adttvn[1];       
};
typedef struct sqladts sqladts;

static struct sqladts sqladt = {
  1,1,0,
};

/* Binding to PL/SQL Records */
struct sqltdss
{
  unsigned int tdsvsn; 
  unsigned short tdsnum; 
  unsigned char *tdsval[1]; 
};
typedef struct sqltdss sqltdss;
static struct sqltdss sqltds =
{
  1,
  0,
};

/* File name & Package Name */
struct sqlcxp
{
  unsigned short fillen;
           char  filnam[9];
};
static const struct sqlcxp sqlfpn =
{
    8,
    "cdsif.pc"
};


static unsigned long sqlctx = 17723;


static struct sqlexd {
   unsigned int   sqlvsn;
   unsigned int   arrsiz;
   unsigned int   iters;
   unsigned int   offset;
   unsigned short selerr;
   unsigned short sqlety;
   unsigned int   unused;
      const short *cud;
   unsigned char  *sqlest;
      const char  *stmt;
   sqladts *sqladtp;
   sqltdss *sqltdsp;
            void  **sqphsv;
   unsigned int   *sqphsl;
            int   *sqphss;
            void  **sqpind;
            int   *sqpins;
   unsigned int   *sqparm;
   unsigned int   **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
            void  *sqhstv[4];
   unsigned int   sqhstl[4];
            int   sqhsts[4];
            void  *sqindv[4];
            int   sqinds[4];
   unsigned int   sqharm[4];
   unsigned int   *sqharc[4];
   unsigned short  sqadto[4];
   unsigned short  sqtdso[4];
} sqlstm = {10,4};

// Prototypes
extern "C" {
  void sqlcxt (void **, unsigned long *,
               struct sqlexd *, const struct sqlcxp *);
  void sqlcx2t(void **, unsigned long *,
               struct sqlexd *, const struct sqlcxp *);
  void sqlbuft(void **, char *);
  void sqlgs2t(void **, char *);
  void sqlorat(void **, unsigned long *, void *);
}

// Forms Interface
static const int IAPSUCC = 0;
static const int IAPFAIL = 1403;
static const int IAPFTL  = 535;
extern "C" { void sqliem(char *, int *); }

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{10,4130,0,0,0,
5,0,0,1,74,0,4,168,0,0,1,0,0,1,0,2,97,0,0,
24,0,0,2,0,0,27,1213,0,0,4,4,0,1,0,1,9,0,0,1,9,0,0,1,10,0,0,1,10,0,0,
55,0,0,3,0,0,30,1277,0,0,0,0,0,1,0,
};


/******************************************************************
* 파 일 명     
      : cdsif.pc                
* 기   능                                                          
     : CDS로부터 한도관리 가입자를 전송받아 CIM모듈로 전송하고 
       결과를 CDS로 전송한다.
       UDA1-CDSIF는 한도관리 가입자를 전송받으면 UDA2-CDSIF로 
       받은 DATA를 똑같이 전송한다.
* 작 성 자     
     : 정중기                         
* first data  
     :                     
* last updata 
     : 2000. 12. 1                  
* 관 련 화 일  
     : cdsif.hpp, cdsif.h	
* program history                                
                           
* Version
  : 1.1 ( 2001. 05. 21 )                                           
******************************************************************/

#include "cdsif.hpp"
#define SQLCA_STORAGE_CLASS extern
/* EXEC    SQL     INCLUDE SQLCA;
 */ 
/*
 * $Header: sqlca.h,v 1.3 1994/12/12 19:27:27 jbasu Exp $ sqlca.h 
 */

/* Copyright (c) 1985,1986, 1998 by Oracle Corporation. */
 
/*
NAME
  SQLCA : SQL Communications Area.
FUNCTION
  Contains no code. Oracle fills in the SQLCA with status info
  during the execution of a SQL stmt.
NOTES
  **************************************************************
  ***                                                        ***
  *** This file is SOSD.  Porters must change the data types ***
  *** appropriately on their platform.  See notes/pcport.doc ***
  *** for more information.                                  ***
  ***                                                        ***
  **************************************************************

  If the symbol SQLCA_STORAGE_CLASS is defined, then the SQLCA
  will be defined to have this storage class. For example:
 
    #define SQLCA_STORAGE_CLASS extern
 
  will define the SQLCA as an extern.
 
  If the symbol SQLCA_INIT is defined, then the SQLCA will be
  statically initialized. Although this is not necessary in order
  to use the SQLCA, it is a good pgming practice not to have
  unitialized variables. However, some C compilers/OS's don't
  allow automatic variables to be init'd in this manner. Therefore,
  if you are INCLUDE'ing the SQLCA in a place where it would be
  an automatic AND your C compiler/OS doesn't allow this style
  of initialization, then SQLCA_INIT should be left undefined --
  all others can define SQLCA_INIT if they wish.

  If the symbol SQLCA_NONE is defined, then the SQLCA variable will
  not be defined at all.  The symbol SQLCA_NONE should not be defined
  in source modules that have embedded SQL.  However, source modules
  that have no embedded SQL, but need to manipulate a sqlca struct
  passed in as a parameter, can set the SQLCA_NONE symbol to avoid
  creation of an extraneous sqlca variable.
 
MODIFIED
    lvbcheng   10/28/98 -  Undo long to int
    lvbcheng   08/03/98 -  Change sqlca long attrs to ints
    jbasu      12/12/94 -  Bug 217878: note this is an SOSD file
    losborne   08/11/92 -  No sqlca var if SQLCA_NONE macro set 
  Clare      12/06/84 - Ch SQLCA to not be an extern.
  Clare      10/21/85 - Add initialization.
  Bradbury   01/05/86 - Only initialize when SQLCA_INIT set
  Clare      06/12/86 - Add SQLCA_STORAGE_CLASS option.
*/
 
#ifndef SQLCA
#define SQLCA 1
 
struct   sqlca
         {
         /* ub1 */ char    sqlcaid[8];
         /* b4  */ long    sqlabc;
         /* b4  */ long    sqlcode;
         struct
           {
           /* ub2 */ unsigned short sqlerrml;
           /* ub1 */ char           sqlerrmc[70];
           } sqlerrm;
         /* ub1 */ char    sqlerrp[8];
         /* b4  */ long    sqlerrd[6];
         /* ub1 */ char    sqlwarn[8];
         /* ub1 */ char    sqlext[8];
         };

#ifndef SQLCA_NONE 
#ifdef   SQLCA_STORAGE_CLASS
SQLCA_STORAGE_CLASS struct sqlca sqlca
#else
         struct sqlca sqlca
#endif
 
#ifdef  SQLCA_INIT
         = {
         {'S', 'Q', 'L', 'C', 'A', ' ', ' ', ' '},
         sizeof(struct sqlca),
         0,
         { 0, {0}},
         {'N', 'O', 'T', ' ', 'S', 'E', 'T', ' '},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0}
         }
#endif
         ;
#endif
 
#endif
 
/* end SQLCA */


// select Items from T_PROCESS_INFO
/* EXEC SQL BEGIN DECLARE SECTION; */ 

/* VARCHAR	user[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } user;

/* VARCHAR	password[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } password;

char    strProcName[13];
/* EXEC SQL END DECLARE SECTION; */ 


static int	  SystemId;
static int	  ServiceId;
static int	  GroupId;
static int	  ProcessType;
static int 	  PserialNo;
static int	  PreMark;
static int	  nSystem2Alive;

int	 gSendport;
int  gRecvport;
char strHostIp[16];
// Log Write
char strJob[LEN_JOB+1];
char strLogMin[11];
int  nResult;

NEPACKET gNeP;

Ccdsif 		  *cdsifC;
TID			  gstTid;
CdsResFormat  stResult;
SendLogManage				*mqLog;
MessageQueue<MsgPmsStatus>	*mqPMS;
MessageQueue<CdsCustFormat> *mqSubsS;
MessageQueue<CdsResFormat>  *mqSubsR;

int main(int argc, char	**argv)
{	
	int 	nComPort = -1;
	int 	nResPort = -1;
	int		svcid;
	
	signal(SIGUSR1,sigCapture); 	signal(SIGTERM,sigCapture);      
	signal(SIGTERM,sigCapture);                                      
	signal(SIGINT,sigCapture); 	signal(SIGSTOP,sigCapture);          
	signal(SIGQUIT,sigCapture);	signal(SIGTSTP,sigCapture);          
	signal(SIGKILL,sigCapture);	signal(SIGCONT,sigCapture);	//26     

	memset(strHostIp, NULL, 16);

	if(argc != 10 ){
    	cout <<"Usage: cdsif [sysId][svcId][gId][pId][pSn][pRemark][serverIp][comPort][resPort] "<<endl;
    	exit(-1);
    }
	// 프로세스 형상
	SystemId=atoi(argv[1]);
	ServiceId=atoi(argv[2]);
	GroupId=atoi(argv[3]);    
	ProcessType=atoi(argv[4]);
	PserialNo=atoi(argv[5]);  
	PreMark=atoi(argv[6]);    
	// 소켓 관련 
	nComPort = atoi(argv[8]); 
	nResPort = atoi(argv[9]);
	memcpy(strHostIp,argv[7],strlen(argv[7]));
	
	// 고객정보전송( to CIM ) 
	if(ServiceId == 1 || ServiceId == 2 ) svcid = 1;
	else if(ServiceId == 3 || ServiceId == 4 ) svcid = 3;
	mqSubsS = new MessageQueue<CdsCustFormat>(SystemId,svcid,3,2,0,10,1000);
//	mqSubsR = new MessageQueue<CdsResFormat>(SystemId,ServiceId,GroupId, \
//										     ProcessType,PserialNo,10,1000);
	// Log Write
	mqLog = new SendLogManage(SystemId,ServiceId,GroupId,ProcessType,PserialNo,PreMark);
	// 프로세스정보전송 ( to PMS ) 
	mqPMS    = new MessageQueue<MsgPmsStatus>(atoi(argv[1]),0,1,1,0,10,1000);
	
	mqLog->put_EventHistData(FLAG_START,FLAG_PROC_START);
	WritePMSMQ(FLAG_PROC_START);
	
	cdsifC   = new Ccdsif(strHostIp, nComPort, nResPort);
	cdsifC->run();
}	

Ccdsif::Ccdsif(char *strHostIp, int nComPort, int nResPort)
{
	comSockC = new SockC(strHostIp, nComPort);
	resSockC = new SockC(strHostIp, nResPort);
	
	WritePMSMQ(-4); // start report to PMS
	nAcceptComCh = -1;
	nAcceptResCh = -1;
}
	
Ccdsif::~Ccdsif()
{
	ConnectionRelease(1, gSendport, gNeP );
	Process4Cds(&gSendport);
	ConnectionRelease(2, gRecvport, gNeP );
	Process4Cds(&gRecvport);
	delete comSockC;
	delete resSockC;
	delete mqLog;
	delete mqPMS;
	delete mqSubsR;
	delete mqSubsS;
}

void Ccdsif::run()
{
	if((nListenComCh= comSockC->CreateTCPServer()) < 0 ){
		*mqLog<<mqLog->ErrorCode(0)<<"ServerCreateError(comSockC)..."<<endl;
		exit(1);
	}
	if((nListenResCh= resSockC->CreateTCPServer()) < 0 ){
		*mqLog<<mqLog->ErrorCode(0)<<"ServerCreateError(resSockC)..."<<endl;
		exit(1);
	}
	ReadLastTid(&gstTid);
	
	switch(SystemId) {
		
		case 1: ConnectToDB();
				// table에 system2의 cds정보가 존재하면 uda2에 서비스가 존재한다고 봄
				// udp socket을 만들고 uda2 cds I/F로 command request 전송
				nSystem2Alive = CheckSystem2();
				ReleaseDB();
				if(nSystem2Alive == SYSTEM2_ALIVE ) {
					udpSockC.CreateUDPClient(UDA2_IPADDR,UDA1_IPADDR,CDSIF_UDP_PORT);
				}
				cdsMainLoop1();
				break;
		
		case 2: udpSockC.CreateUDPServer(UDA2_IPADDR, CDSIF_UDP_PORT);
				cdsMainLoop2();
				break;
		
		default: break;
	}
}


int Ccdsif::CheckSystem2()
{
	/* EXEC	SQL		select	prog_name 
					into 	:strProcName 
					from 	t_process_info
					where	proc_alias = '23CIF00X'; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 1;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select prog_name into :b0  from t_process_info where proc_al\
ias='23CIF00X'";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlstm.sqhstv[0] = (         void  *)strProcName;
 sqlstm.sqhstl[0] = (unsigned int  )13;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 // uda2 service 3
	
	if(sqlca.sqlcode = 1403) {
		 *mqLog<<mqLog->ErrorCode(0)<<"UDA2 CDS I/F IS NOT ALIVE!!"<<endl;
		 return SYSTEM2_NOTALIVE;
	}
	else if (sqlca.sqlcode = 0) {
		*mqLog<<mqLog->ErrorCode(0)<<"UDA2 CDS I/F IS  ALIVE!!"<<endl;
		return SYSTEM2_ALIVE;
	}
}

void Ccdsif::cdsMainLoop1()
{
	struct timeval	timeout;
	int		nFds;
	fd_set	readFds;

	timeout.tv_sec = 3;
	timeout.tv_usec = 0;
	
	*mqLog<<mqLog->ErrorCode(0)<<"CDS I/F Started!!"<<endl;
	while(1){
		
		nFds = (nListenComCh > nListenResCh ? nListenComCh : nListenResCh);
		nFds = (nFds > nAcceptComCh ? nFds : nAcceptComCh);
		nFds = (nFds > nAcceptResCh ? nFds : nAcceptResCh) + 1;

		FD_ZERO(&readFds);
		FD_SET(nListenComCh, &readFds);FD_SET(nListenResCh, &readFds);
		if (nAcceptComCh > 0) FD_SET(nAcceptComCh, &readFds);
		if (nAcceptResCh > 0) FD_SET(nAcceptResCh, &readFds);

		if (select(nFds, (int *) &readFds, 0, 0, &timeout) < 0) {
			if (errno == EINTR) {			// ignore  
				continue;
			}else *mqLog<<mqLog->ErrorCode(0)<<"select failed due to:"<<strerror(errno)<<endl;
		}
		else {

			if(FD_ISSET(nListenComCh, &readFds)) {
				nAcceptComCh = AcceptSocket(nListenComCh);
				gSendport = nAcceptComCh;
			}
			
			if(FD_ISSET(nListenResCh, &readFds)) {
				nAcceptResCh = AcceptSocket(nListenResCh);
				gRecvport = nAcceptResCh;
			}
			//// do action when received msg from other system
			if( nAcceptComCh > 0 && (FD_ISSET(nAcceptComCh, &readFds)))
				Process4Cds(&nAcceptComCh);

			if(nAcceptResCh > 0 && (FD_ISSET(nAcceptResCh, &readFds)))
				Process4Cds(&nAcceptResCh);

		}
	}// while
}

void Ccdsif::cdsMainLoop2()
{
	int nRet;
	CdsResFormat    subsHeader;
	CdsCustFormat	msgToCIM;// CIM으로 보낼 메시지

	while(1) {
			
		udpSockC.RecvFromMsg((char *)&msgToCIM, sizeof(CdsCustFormat), 0);
		memcpy((CdsResFormat *)&subsHeader,msgToCIM.get_CdsResFormat(),sizeof(CdsResFormat) );
			
		if((nRet = mqSubsS->sendMsg(&msgToCIM)) == FLAG_MQ_IS_FULL) {

			*mqLog<<mqLog->ErrorCode(0)<<"fail send subsinformation to CIM! error:"<<nRet<<endl;
			subsHeader.set_nRespond(0); // FA

		}else subsHeader.set_nRespond(1); // SC

		if(udpSockC.Reply2MsgSender((char *)&subsHeader,sizeof(CdsResFormat),0 )<0)
			*mqLog<<mqLog->ErrorCode(0)<<"send fail to uda1!"<<endl;	
	}
}


int Ccdsif::AcceptSocket(int fdListenSock)
{
	int fdSock;
    int nClientAddrLen;
    struct sockaddr_in *pClientAddr;

    nClientAddrLen = sizeof(struct sockaddr_in);

    fdSock = accept(fdListenSock,  pClientAddr, &nClientAddrLen);

	if (errno == EINTR)
	{
    	return fdSock;
	}
	
    if (fdSock == -1)
    {
        perror("AcceptSocket");
        return -1;
    }

    return fdSock;
}

int Ccdsif::Process4Cds(int *sd)
{
	int			ret = 0;
	int			nRecv, nDataLen;
	char		strData[CDS_MAX_TCP_BUF];
	NEPACKET	stNePacket;

//	*mqLog<<mqLog->ErrorCode(0)<<"\n";
	*mqLog<<mqLog->ErrorCode(0)<<"<<<<<<<<<<<< Recv from CDS >>>>>>>>>>>"<<endl;
	errno = 0;

	if ((nRecv = recv(*sd, (void *) strData, NE_PACKET_HEADER_LEN, 0)) < 0) {
		*mqLog<<mqLog->ErrorCode(0)<<"ReadNePacket : recv failed due to: "<< strerror(errno)<<endl;
		close(*sd);	*sd = -1;
		return(-1);
	}
	else if (nRecv == 0) {
		*mqLog<<mqLog->ErrorCode(0)<<"ProcTcp : recv failed after successful select ..."<<endl;
		*mqLog<<mqLog->ErrorCode(0)<<"				tcp connection closed"<<endl;
		close(*sd);	*sd = -1;
		return(0);
	}
	
	
	// get header data
	GetNeHeader(&stNePacket, strData);
	memcpy((NEPACKET *)&gNeP, (NEPACKET *)&stNePacket, sizeof(NEPACKET));
	
	// read real data
	nDataLen = (int) stNePacket.unDataSize;
	if (nDataLen != 0) {
		if ((nRecv = recv(*sd, (void *) strData, nDataLen, 0)) < 0) {
			*mqLog<<mqLog->ErrorCode(0)<<"ReadNePacket : recv failed due to "<< strerror(errno)<<endl;
			return(-1);
		}
	}

	memcpy(stNePacket.strData, strData, nRecv);
	stNePacket.strData[nRecv] = 0x00;
	
	// print recv message
	PrintNePacket(stNePacket);


	switch (stNePacket.unMsgId) {

	case SCHANNEL_CONNECTION_REQUEST :
		*mqLog<<mqLog->ErrorCode(0)<<"\tgot SCHANNEL_CONNECTION_REQUEST"<<endl;
		GetComHeader(stNePacket);
		ret = CommandConnectionRequest(*sd, stNePacket, gstTid);
		break;

	case RCHANNEL_CONNECTION_REQUEST :
		*mqLog<<mqLog->ErrorCode(0)<<"\tgot RCHANNEL_CONNECTION_REQUEST"<<endl;
		GetResHeader(stNePacket);
		ret = ResultConnectionRequest(*sd, stNePacket);
		break;

	case SCHANNEL_RELEASE_REQUEST :
		*mqLog<<mqLog->ErrorCode(0)<<"\tgot SCHANNEL_RELEASE_REQUEST"<<endl;
		ret = CommandReleaseRequest(*sd, stNePacket);
		break;

	case SCHANNEL_RELEASE_REQUEST_ACK :
		*mqLog<<mqLog->ErrorCode(0)<<"\tgot SCHANNEL_RELEASE_REQUEST_ACK"<<endl;
		ret = -1;
		break;

	case RCHANNEL_RELEASE_REQUEST :
		*mqLog<<mqLog->ErrorCode(0)<<"\tgot RCHANNEL_RELEASE_REQUEST"<<endl;
		ret = ResultReleaseRequest(*sd, stNePacket);
		break;

	case RCHANNEL_RELEASE_REQUEST_ACK :
		*mqLog<<mqLog->ErrorCode(0)<<"\tgot RCHANNEL_RELEASE_REQUEST_ACK"<<endl;
		ret = -1;
		break;

	case SYSTEM_STATE_REPORT_ACK :
		*mqLog<<mqLog->ErrorCode(0)<<"\tgot SYSTEM_STATE_REPORT_ACK"<<endl;
		break;

	case PROCESS_STATE_REQUEST :
		*mqLog<<mqLog->ErrorCode(0)<<"\tgot PROCESS_STATE_REQUEST"<<endl;
		ret = ProcessStateRequest(*sd, stNePacket);
		break;

	case COMMAND_REQUEST :
		*mqLog<<mqLog->ErrorCode(0)<<"\tgot COMMAND_REQUEST"<<endl;
		// packet check
		ret = CommandRequest(*sd, &gstTid, stNePacket);
		WriteFileLog(COMMAND_REQUEST_ACK, stNePacket);
		if (ret > 0) {			// accepted  
			ProcessCommand(nAcceptResCh, stNePacket);
		}
		break;

	case COMMAND_RESULT_ACK :
		*mqLog<<mqLog->ErrorCode(0)<<"\tgot COMMAND_RESULT_ACK"<<endl;
		if (strncmp(stNePacket.strData, "FA", 2) == 0) {
		}
		
		break;
	
	case SUBS_DATA_REQUEST :
		*mqLog<<mqLog->ErrorCode(0)<<"\tgot SUBS_DATA_REQUEST"<<endl;
		{
			TID		stTidSave;
			memcpy(&stTidSave, &(stNePacket.stTid), sizeof(TID));
			SendRequestAck(*sd, stNePacket, SUBS_DATA_REQUEST_ACK);
			SendSubsDataResult(nAcceptResCh, stTidSave);
		}
		break;
	default :
		*mqLog<<mqLog->ErrorCode(0)<<"undefined message !"<<endl;
		ret = SendCisUnrecognizedMsg(*sd, stNePacket);
	}

	if (ret < 0) {
		// tcp connection closed 
		close(*sd);	*sd = -1;
		return(-1);
	}
	return(0);
}


int Ccdsif::SendCisUnrecognizedMsg(int sdTcp, NEPACKET stNePacket)
{
	short	sTemp;
	int		nLen;
	char	strBuf[CDS_MAX_TCP_BUF];

	// send msgid what I got 
	GetRespHeaderAndOption(stNePacket.unMsgId, &stNePacket);
		
	// RESULT
	strncpy(stNePacket.strData, "FA", 2);
	sTemp = ERR_UNRECOGNIZED_MESSAGE;
	memcpy(&(stNePacket.strData)[2], &sTemp, 2);
	stNePacket.unDataSize = 4;

	GetNePacketToSend(&stNePacket, strBuf);
	nLen = NE_PACKET_HEADER_LEN + stNePacket.unDataSize;
	
	*mqLog<<mqLog->ErrorCode(0)<<"<<<<<<<<<<<< Send  to  CDS >>>>>>>>>>>>"<<endl;

	PrintNePacket(stNePacket);

	if (send(sdTcp, (void *) strBuf, nLen, 0) != nLen) {
		*mqLog<<mqLog->ErrorCode(0)<<"TCP send failed ("<<strerror(errno)<<")"<<endl;
		return(-1);
	}
	return(0);
}

int Ccdsif::ProcessStateRequest(int sdTcp, NEPACKET stNePacket)
{
	char	strBuf[1024];
	int		nLen;
	short	sTemp = 1;

	// send PROCESS_STATE_REQUEST_ACK  
	GetRespHeaderAndOption(PROCESS_STATE_REQUEST_ACK, &stNePacket);

	memcpy(stNePacket.strData, &sTemp, 2);
	stNePacket.unDataSize = 2;

	GetNePacketToSend(&stNePacket, strBuf);
	nLen = NE_PACKET_HEADER_LEN + stNePacket.unDataSize;

	*mqLog<<mqLog->ErrorCode(0)<<"<<<<<<<<<<<< Send  to  CDS >>>>>>>>>>>>"<<endl;
	
	if (send(sdTcp, (void *) strBuf, nLen, 0) != nLen) {
		*mqLog<<mqLog->ErrorCode(0)<<"send faield: "<<strerror(errno)<<endl;
		return(-1);
	}
	PrintNePacket(stNePacket);
	return(0);
}

int Ccdsif::ConnectionRelease(int channel, int sdTcp, NEPACKET stNePacket )
{
	char	strBuf[1024];
	int		nLen;
	short	sTemp = 1;

	// send RELEASE_REQUEST
	if (channel == 1)
		GetRespHeaderAndOption(SCHANNEL_RELEASE_REQUEST, &stNePacket);
	else GetRespHeaderAndOption(RCHANNEL_RELEASE_REQUEST, &stNePacket);

//	memcpy(stNePacket.strData, &sTemp, 2);
	stNePacket.unDataSize = 0;

	GetNePacketToSend(&stNePacket, strBuf);
	nLen = NE_PACKET_HEADER_LEN + stNePacket.unDataSize;

	*mqLog<<mqLog->ErrorCode(0)<<"<<<<<<<<<<<< Send  to  CDS >>>>>>>>>>>>"<<endl;
	PrintNePacket(stNePacket);
	
	if (send(sdTcp, (void *) strBuf, nLen, 0) != nLen) {
		*mqLog<<mqLog->ErrorCode(0)<<"send faield: "<<strerror(errno)<<endl;
		return(-1);
	}
	return(0);
}

int Ccdsif::SendSubsDataResult(int sdTcp, TID stTid)
{
	NEPACKET	stNePacket;
	int			nLen = 0;
	char		strBuf[CDS_MAX_TCP_BUF];
	short		sTemp;

	// MSG ID  
	stNePacket.unMsgId = SUBS_DATA_RESULT;

	// TRANSACTION ID  
	memcpy(&(stNePacket.stTid), &stTid, sizeof(TID));

	// SOURCE SYSTEM ID  
	sprintf(stNePacket.strSrcSysId, "%06s", gstrResSrcSysId);

	// DESTINATION SYSTEM ID  
	sprintf(stNePacket.strDestSysId, "%06s", gstrResDestSysId);

	// SOURCE APPLICATION ID  
	sprintf(stNePacket.strSrcApiId, "%06s", gstrResSrcApiId);

	// DESTINATION PROCESS ID  
	sprintf(stNePacket.strDestApiId, "%06s", gstrResDestApiId);

	// CONTINUE FLAG  
	stNePacket.usContFlag = 1;

	// SERIAL NO  
	stNePacket.usSerialNo = 0;

	// DATA SIZE  
	stNePacket.unDataSize = 100;

	// DATA  
	{
		int		i;

		for (i = 0; i < stNePacket.unDataSize; i++) {
			stNePacket.strData[i] = '1';
		}
	}

	GetNePacketToSend(&stNePacket, strBuf);
	nLen = NE_PACKET_HEADER_LEN + stNePacket.unDataSize;
	*mqLog<<mqLog->ErrorCode(0)<<"<<<<<<<<<<<< Send  to  CDS >>>>>>>>>>>>"<<endl;
	PrintNePacket(stNePacket);
	
	if (send(sdTcp, (void *) strBuf, nLen, 0) != nLen) {
		*mqLog<<mqLog->ErrorCode(0)<<"SendAuditComplete : send failed due to: "<<strerror(errno)<<endl;
		return(-1);
	}
	return(0);
}


int Ccdsif::ProcessCommand(int sd, NEPACKET stNePacket)
{
	char	strResult[4];
	short	sReason;

	sReason = SendSubsInfoToCIM(stNePacket);
	nResult = sReason;
	WriteFileLog(COMMAND_RESULT_ACK, stNePacket);
	
	if( sReason == ERR_DB_FAULT || sReason == ERR_COMMAND_NOT_SUPPORT ) {
		strcpy(strResult, "FA"); //FA
		if (SendResultToCds(sd, stNePacket.stTid, strResult, sReason) < 0)
			return(-1);
	}else {
		strcpy(strResult, "SC"); //FA
		if (SendResultToCds(sd, stNePacket.stTid, strResult, sReason) < 0)
			return(-1);
	}
		
	return(0);
}

int Ccdsif::SendSubsInfoToCIM(NEPACKET stNePacket)
{
	char strJobCode[LEN_JOB];
	char strMin[LEN_MIN];
	char strNewMin[LEN_MIN];
	char strTmp[8];
	short nRet;
	CdsResFormat    subsHeader;
	CustFormat		subsInfoC;
	CdsCustFormat	msgToCIM;// CIM으로 보낼 메시지
	
	memset(strJob,0x00,LEN_JOB+1);
	memset(strLogMin,0x00,11);
	memset(strTmp,0x00,8);
	memcpy(strJobCode, stNePacket.strData, LEN_JOB);
	memcpy(strMin, &(stNePacket.strData)[LOC_MIN], LEN_MIN);
	memcpy(strNewMin, &(stNePacket.strData)[LOC_NEW_MIN], LEN_MIN);

	memset((CdsResFormat *)&subsHeader,0x00,sizeof(CdsResFormat));
	memset((CustFormat *)&subsInfoC,0x00,sizeof(CustFormat));
	memset((CdsCustFormat *)&msgToCIM,0x00,sizeof(CdsCustFormat));
	
    if( !strncmp(strJobCode,"A1",LEN_JOB)||
    	!strncmp(strJobCode,"A2",LEN_JOB)||
    	!strncmp(strJobCode,"A3",LEN_JOB)||
    	!strncmp(strJobCode,"CX",LEN_JOB) )
    	memcpy(subsInfoC.JobCode, "A1", LEN_JOB);
    	
    else if( !strncmp(strJobCode,"Z1",LEN_JOB)||
    		 !strncmp(strJobCode,"Z2",LEN_JOB)||
    		 !strncmp(strJobCode,"CA",LEN_JOB) )
    	memcpy(subsInfoC.JobCode, "Z1", LEN_JOB);
    	
    else if( !strncmp(strJobCode,"D3",LEN_JOB) )
    	memcpy(subsInfoC.JobCode, "D1", LEN_JOB);
    else 
    	return ERR_COMMAND_NOT_SUPPORT;
    
	subsInfoC.Code = 0x11;
	if( !strncmp(strMin,"0",1) ) {
		memcpy(strTmp,&strMin[1],7);
		sprintf(subsInfoC.Min, "011%s",strTmp);
		memset(subsInfoC.Min2,0x00,10);
	} else {
		memcpy(strTmp,strMin,8);
		sprintf(subsInfoC.Min, "11%s",strTmp);
		memset(subsInfoC.Min2,0x00,10);
	}
		
	if( !strncmp(subsInfoC.JobCode,"D1",LEN_JOB) ) {
		if( !strncmp(strNewMin,"0",1) ) {
			memcpy(strTmp,&strNewMin[1],7);
			sprintf(subsInfoC.Min2, "011%s",strTmp);
		} else {
			memcpy(strTmp,strNewMin,8);
			sprintf(subsInfoC.Min2, "11%s",strTmp);
		}
	}	

	memcpy(strLogMin,subsInfoC.Min,10);	// Log 저장용
	memcpy(strJob,subsInfoC.JobCode, LEN_JOB);  // Log 저장용
	subsHeader.set_strDate(stNePacket.stTid.strDate);	
	subsHeader.set_nSeqNum(stNePacket.stTid.nSeqNo);				
	msgToCIM.setItem(&subsHeader);
	msgToCIM.setItem(&subsInfoC);
	
	if((nRet = mqSubsS->sendMsg(&msgToCIM)) == FLAG_MQ_IS_FULL) {
		
		*mqLog<<mqLog->ErrorCode(0)<<"fail send subsinformation to CIM! error:"<<nRet<<endl;
		return ERR_DB_FAULT;
		
	}else {
		
		if(nSystem2Alive == SYSTEM2_ALIVE ) {	
			if(udpSockC.Send2Msg((char *)&msgToCIM, sizeof(CdsCustFormat),0) <= 0) {
				*mqLog<<mqLog->ErrorCode(0)<<"fail send to uda2"<<endl;
				return ERR_DB_FAULT;
			}
		
			// 결과를 3초간 대기 결과 없으면 FA 처리 
			memset((CdsResFormat *)&subsHeader,0x00,sizeof(CdsResFormat));
			nRet = udpSockC.RecvfromMsgEx((char *)&subsHeader,sizeof(CdsResFormat),0);
			if( nRet <= 0 )return ERR_DB_FAULT;
			else {
				if(subsHeader.get_nRespond() == 1) return 0;
				else return ERR_DB_FAULT;
			}
		} else return 0; //sc
		
	} // else
}



int Ccdsif::SendResultToCds(int sdTcp, TID stTid, char *strResult, short sReason)
{
	NEPACKET	stNePacket;
	int			nLen = 0;
	char		strBuf[CDS_MAX_TCP_BUF];

	memset((NEPACKET *)&stNePacket,0x00,sizeof(NEPACKET));
	// MSG ID  
	stNePacket.unMsgId = COMMAND_RESULT;

	// TRANSACTION ID  
	memcpy(stNePacket.stTid.strDate, stTid.strDate, 8);
	stNePacket.stTid.nSeqNo = stTid.nSeqNo;

	// SOURCE SYSTEM ID  
	sprintf(stNePacket.strSrcSysId, "%06s", gstrResSrcSysId);

	// DESTINATION SYSTEM ID  
	sprintf(stNePacket.strDestSysId, "%06s", gstrResDestSysId);

	// SOURCE APPLICATION ID  
	sprintf(stNePacket.strSrcApiId, "%06s", gstrResSrcApiId);

	// DESTINATION PROCESS ID  
	sprintf(stNePacket.strDestApiId, "%06s", gstrResDestApiId);

	// CONTINUE FLAG  
	stNePacket.usContFlag = 1;

	// SERIAL NO  
	stNePacket.usSerialNo = 0;

	// DATA SIZE  
	stNePacket.unDataSize = 4;

	memcpy(stNePacket.strData,0x00,4);
	// RESULT  
	sprintf(stNePacket.strData, "%2s", strResult);

	// REASON  
	memcpy(&(stNePacket.strData)[2], &sReason, 2);

	GetNePacketToSend(&stNePacket, strBuf);
	nLen = NE_PACKET_HEADER_LEN + stNePacket.unDataSize;
	*mqLog<<mqLog->ErrorCode(0)<<"<<<<<<<<<<<< Send  to  CDS >>>>>>>>>>>>"<<endl;
	PrintNePacket(stNePacket);
	
	if (send(sdTcp, (void *) strBuf, nLen, 0) != nLen) {
		*mqLog<<mqLog->ErrorCode(0)<<"ProcessCommand : send failed due to: "<<strerror(errno)<<endl;
		return(-1);
	}
	return(0);
}

void Ccdsif::GetResHeader(NEPACKET stNePacket)
{
	// RESULT / SOURCE SYSTEM ID  
	memcpy(gstrResSrcSysId, stNePacket.strSrcSysId, LEN_SRC_SYS_ID);

	// RESULT / DESTINATION SYSTEM ID  
	memcpy(gstrResDestSysId, stNePacket.strDestSysId, LEN_DEST_SYS_ID);

	// RESULT / SOURCE PROCESS ID  
	memcpy(gstrResSrcApiId, stNePacket.strSrcApiId, LEN_SRC_SYS_ID);

	// RESULT / DESTINATION PROCESS ID 
	memcpy(gstrResDestApiId, stNePacket.strDestApiId, LEN_DEST_SYS_ID);
}


void Ccdsif::GetComHeader(NEPACKET stNePacket)
{
	// COMMAND / SOURCE SYSTEM ID  
	memcpy(gstrComSrcSysId, stNePacket.strSrcSysId, LEN_SRC_SYS_ID);

	// COMMAND / DESTINATION SYSTEM ID  
	memcpy(gstrComDestSysId, stNePacket.strDestSysId, LEN_DEST_SYS_ID);

	// COMMAND / SOURCE PROCESS ID  
	memcpy(gstrComSrcApiId, stNePacket.strSrcApiId, LEN_SRC_SYS_ID);

	// COMMAND / DESTINATION PROCESS ID 
	memcpy(gstrComDestApiId, stNePacket.strDestApiId, LEN_DEST_SYS_ID);
}


int Ccdsif::CommandConnectionRequest(int sdTcp,NEPACKET stNePacket,TID stTid)
{
	char	strBuf[CDS_MAX_TCP_BUF];
	int		nLen, nDataLen = 0;
	short	sTemp = 0;

	// send SCHANNEL_CONNECTION_REQUEST_ACK  
	GetRespHeaderAndOption(SCHANNEL_CONNECTION_REQUEST_ACK, &stNePacket);

	// DATA SIZE  
	stNePacket.unDataSize = 16;

	// DATA  
	// RESULT  
	memcpy(stNePacket.strData, "SC", 2);

	// REASON  
	memcpy(&(stNePacket.strData)[2], &sTemp, 2);

	// TRANSACTION ID  
	memcpy(&(stNePacket.strData)[4], stTid.strDate, 8);
	memcpy(&(stNePacket.strData)[12], &(stTid.nSeqNo), 4);

//	printf("TID DATE:%s, TIME:%d \n", stTid.strDate,stTid.nSeqNo); 
	GetNePacketToSend(&stNePacket, strBuf);
	nLen = NE_PACKET_HEADER_LEN + stNePacket.unDataSize;
	*mqLog<<mqLog->ErrorCode(0)<<"<<<<<<<<<<<< Send  to  CDS >>>>>>>>>>>>"<<endl;
	PrintNePacket(stNePacket);
	
	if (send(sdTcp, (void *) strBuf, nLen, 0) != nLen) {
		*mqLog<<mqLog->ErrorCode(0)<<"send faield : "<<strerror(errno)<<endl;
		return(-1);
	}
	return(0);
}

void Ccdsif::GetRespHeaderAndOption(int nMsgId, NEPACKETPTR ptrNePacket)
{	
	int	nLen = 0;
	char	strSrcSysIdSave[6], strSrcApiIdSave[6];

	ptrNePacket->unMsgId = nMsgId;

	// SOURCE SYSTEM ID  
	memcpy(strSrcSysIdSave, ptrNePacket->strSrcSysId, 6);
	memcpy(ptrNePacket->strSrcSysId, ptrNePacket->strDestSysId, 6);
	memcpy(ptrNePacket->strDestSysId, strSrcSysIdSave, 6);

	memcpy(strSrcApiIdSave, ptrNePacket->strSrcApiId, 6);
	memcpy(ptrNePacket->strSrcApiId, ptrNePacket->strDestApiId, 6);
	memcpy(ptrNePacket->strDestApiId, strSrcApiIdSave, 6);
}

int Ccdsif::CommandReleaseRequest(int sdTcp, NEPACKET stNePacket)
{
	int		nLen;
	short	sShort = 0;
	char	strBuf[CDS_MAX_TCP_BUF];

	// send SCHANNEL_RELEASE_REQUEUST_ACK  
	GetRespHeaderAndOption(SCHANNEL_RELEASE_REQUEST_ACK, &stNePacket);
		
	// DATA SIZE  
	stNePacket.unDataSize = 4;

	// RESULT  
	memcpy(stNePacket.strData, "SC", 2);

	// REASON  
	memcpy(&(stNePacket.strData)[2], &sShort, 2);

	GetNePacketToSend(&stNePacket, strBuf);
	nLen = NE_PACKET_HEADER_LEN + stNePacket.unDataSize;
	
	PrintNePacket(stNePacket);
	*mqLog<<mqLog->ErrorCode(0)<<"<<<<<<<<<<<< Send  to  CDS >>>>>>>>>>>>"<<endl;
	if (send(sdTcp, (void *) strBuf, nLen, 0) != nLen) {
		*mqLog<<mqLog->ErrorCode(0)<<"TCP write failed : "<<strerror(errno)<<endl;
		return(-1);
	}
	return(0);
}

int Ccdsif::CommandRequest(int sdTcp, TIDPTR ptrTid, NEPACKET stNePacket)
{
	int	nLen;
	int	accepted;
	short	sTemp;
	char	strBuf[CDS_MAX_TCP_BUF];

	// Send COMMAND_REQUEST_ACK  
	GetRespHeaderAndOption(COMMAND_REQUEST_ACK, &stNePacket);

	if (CheckAndUpdateTransId(ptrTid, &(stNePacket.stTid)) >= 0) {
		// RESULT  
		memcpy(stNePacket.strData, "SC", 2);

		// REASON  
		sTemp = 0;
		memcpy(&(stNePacket.strData)[2], &sTemp, 2);

		accepted = 1;

		// block SIGINT or SIGTERM before saving log 
		{
			long	oldmask;

			oldmask = sigsetmask(sigmask(SIGINT) | sigmask(SIGTERM));

			SaveTid(stNePacket.stTid);

			sigsetmask(oldmask);
		}
	}
	else {
		*mqLog<<mqLog->ErrorCode(0)<<"TRANSACTION ID ["<< stNePacket.stTid.strDate <<"]["<<stNePacket.stTid.nSeqNo<<"]"
			<<"is incorrect !!"<<endl; 
		*mqLog<<mqLog->ErrorCode(0)<<"It should be ["<<ptrTid->strDate<<"]["<< ptrTid->nSeqNo + 1<<"]" <<endl;

		// RESULT 
		memcpy(stNePacket.strData, "FA", 2);

		// REASON 
		sTemp = ERR_MISMATCH_TID;
		memcpy(&(stNePacket.strData)[2], &sTemp, 2);

		accepted = 0;
	}

	// DATA LEN 
	stNePacket.unDataSize = 4;

	GetNePacketToSend(&stNePacket, strBuf);
	nLen = NE_PACKET_HEADER_LEN + stNePacket.unDataSize;
	*mqLog<<mqLog->ErrorCode(0)<<"<<<<<<<<<<<< Send  to  CDS >>>>>>>>>>>>"<<endl;
	PrintNePacket(stNePacket);
	
	if (send(sdTcp, (void *) strBuf, nLen, 0) != nLen) {
		*mqLog<<mqLog->ErrorCode(0)<<"send failed failed :"<< strerror(errno)<<endl;
		return(-1);
	}
	return(accepted);
}


int Ccdsif::CheckAndUpdateTransId(TIDPTR ptrOldTid, TIDPTR ptrNewTid) 
{
	*mqLog<<mqLog->ErrorCode(0)<<"DATA FROM CDS : strDate = ("<<ptrNewTid->strDate
							   <<"), nSeqNo = ("<<ptrNewTid->nSeqNo<<")"<<endl;

	if (memcmp(ptrOldTid->strDate, ptrNewTid->strDate, 8) == 0) {
		if (ptrNewTid->nSeqNo == ptrOldTid->nSeqNo + 1) {
			memcpy(ptrOldTid->strDate, ptrNewTid->strDate, 8);
			ptrOldTid->nSeqNo = ptrNewTid->nSeqNo;
			return(0);
		}
		else {
			return(-1);
		}
	}
	else {
		if (ptrNewTid->nSeqNo == 1) {
			memcpy(ptrOldTid->strDate, ptrNewTid->strDate, 8);
			ptrOldTid->nSeqNo = ptrNewTid->nSeqNo;
			return(0);
		}
		else {
			return(-1);
		}
	}
}


int Ccdsif::ResultConnectionRequest(int sdTcp, NEPACKET stNePacket)
{
	int		nLen;
	time_t	tTime;
	struct tm	*tp;
	short	sTemp = 0;
	char	strBuf[CDS_MAX_TCP_BUF];
	int		nTemp = 0;

	// send RCHANNEL_CONNECTION_REQUEUST_ACK  
	GetRespHeaderAndOption(RCHANNEL_CONNECTION_REQUEST_ACK, &stNePacket);
		
	// RESULT  
	memcpy(stNePacket.strData, "SC", 2);

	// REASON  
	memcpy(&(stNePacket.strData)[2], &sTemp, 2);

	// TRANSACTION ID  
	tTime = time(NULL);
	tp = localtime(&tTime);
	sprintf(&(stNePacket.strData)[4], "%04d%02d%02d", 
			tp->tm_year+1900, tp->tm_mon+1, tp->tm_mday);
	memcpy(&(stNePacket.strData)[12], &nTemp, 4);

	// DATA LEN  
	stNePacket.unDataSize = 16;

	GetNePacketToSend(&stNePacket, strBuf);
	nLen = NE_PACKET_HEADER_LEN + stNePacket.unDataSize;
	*mqLog<<mqLog->ErrorCode(0)<<"<<<<<<<<<<<< Send  to  CDS >>>>>>>>>>>>"<<endl;
	PrintNePacket(stNePacket);
	
	if (send(sdTcp, (void *) strBuf, nLen, 0) != nLen) {
		*mqLog<<mqLog->ErrorCode(0)<<"TCP send failed ("<<strerror(errno)<<")"<<endl;
		return(-1);
	}
	return(0);
}

int Ccdsif::ResultReleaseRequest(int sdTcp, NEPACKET stNePacket)
{
	int		nLen;
	short	sTemp = 0;
	char	strBuf[CDS_MAX_TCP_BUF];

	// send RCHANNEL_RELEASE_REQUEUST_ACK  
	GetRespHeaderAndOption(RCHANNEL_RELEASE_REQUEST_ACK, &stNePacket);
		
	// RESULT  
	memcpy(stNePacket.strData, "SC", 2);

	// REASON  
	memcpy(&(stNePacket.strData)[2], &sTemp, 2);

	// DATA LEN  
	stNePacket.unDataSize = 4;

	GetNePacketToSend(&stNePacket, strBuf);
	nLen = NE_PACKET_HEADER_LEN + stNePacket.unDataSize;
	*mqLog<<mqLog->ErrorCode(0)<<"<<<<<<<<<<<< Send  to  CDS >>>>>>>>>>>>"<<endl;
	PrintNePacket(stNePacket);
	
	if (send(sdTcp, (void *) strBuf, nLen, 0) != nLen) {
		*mqLog<<mqLog->ErrorCode(0)<<"TCP send failed :"<< strerror(errno)<<endl;
		return(-1);
	}
	return(0);
}

int Ccdsif::SaveTid(TID stTid)
{
	FILE		 *fp;
	int			 i;
	unsigned int idate;
	char strFileName[100];
	CdsIF_TID  cdsTidC(SystemId);
	
	
	for (i = 0; i < 8; i++) {
		if (!isdigit(stTid.strDate[i])) {
			*mqLog<<mqLog->ErrorCode(0)<<"strDate ("<<stTid.strDate<<") incorrect !"<<endl;
			return(-1);
		}
	}

//	sprintf(strFileName, "/SVC%d/DATA/CDS_TID.cfg", ServiceId);
	memset (strFileName,0x00,100);
	sprintf(strFileName, "/SVC1/DATA/CDS_TID.cfg", ServiceId);
	if ((fp = fopen(strFileName, "w")) == NULL) {
		*mqLog<<mqLog->ErrorCode(0)<<"fopen failed ... "<<endl;
		return(-1);
	}

/*	fprintf(fp, "%8s %d\n", stTid.strDate, stTid.nSeqNo);
	fclose(fp);
	if (chmod(strFileName, 00666) < 0) {
//		*mqLog<<mqLog->ErrorCode(0)<<"chmod (%s) failed ... %s\n", strFileName, strerror(errno));
		return(-1);
	}
	return(1);
*/
	idate = (unsigned int)atoi(stTid.strDate);
	cdsTidC.set_S_TID(stTid.nSeqNo, idate);
	fclose(fp);
}


void Ccdsif::PrintNePacket(NEPACKET stNePacket)
{
	int		i;
	char   strLog[64];
	char   strTmp[4];
	char   strData[512];

	memset(strLog,0x00,64);
	memset(strData,0x00,512);

	sprintf(strLog,"%04d/ %08s/ %06d/ %06s/ %06s/ %06s/ %06s/ %1d/ %02d/ %04d/",
		stNePacket.unMsgId, stNePacket.stTid.strDate, stNePacket.stTid.nSeqNo,
		stNePacket.strSrcSysId, stNePacket.strDestSysId, 
		stNePacket.strSrcApiId, stNePacket.strDestApiId,
		stNePacket.usContFlag, stNePacket.usSerialNo, 
		stNePacket.unDataSize);
	*mqLog<<mqLog->ErrorCode(0)<<strLog<<endl;
	
	for (i = 0; i < stNePacket.unDataSize; i++) {
		memset(strTmp,0x00,4);
		if (isalpha(stNePacket.strData[i]) || isdigit(stNePacket.strData[i])) {
			sprintf(strTmp,"%c ", stNePacket.strData[i]);
			strcat(strData,strTmp);
		}
		else {
			sprintf(strTmp,"0x%02x ", stNePacket.strData[i]);
			strcat(strData,strTmp);
		}
	}
	*mqLog<<mqLog->ErrorCode(0)<<strData<<endl;
	*mqLog<<mqLog->ErrorCode(0)<<" "<<endl;
/*	for (i = 0; i < stNePacket.unDataSize; i++) {
		if (isalpha(stNePacket.strData[i]) || isdigit(stNePacket.strData[i])) {
			printf("%c ", stNePacket.strData[i]);
		}
		else {
			printf("0x%02x ", stNePacket.strData[i]);
		}
	}
	printf("\n");
*/
}

void Ccdsif::GetNePacketToSend(NEPACKETPTR ptrNePacket, char *strBuf)
{
	// MESSAGE ID  
	memcpy(&strBuf[LOC_MSG_ID], &(ptrNePacket->unMsgId), LEN_MSG_ID);

	// TRANSACTION ID  
	memcpy(&strBuf[LOC_TRANS_ID], ptrNePacket->stTid.strDate, 8);
	memcpy(&strBuf[LOC_TRANS_ID + 8], &(ptrNePacket->stTid.nSeqNo), 4);

	// SOURCE SYSTEM ID  
	memcpy(&strBuf[LOC_SRC_SYS_ID], ptrNePacket->strSrcSysId, LEN_SRC_SYS_ID);

	// DESTINATION SYSTEM ID  
	memcpy(&strBuf[LOC_DEST_SYS_ID], ptrNePacket->strDestSysId,LEN_DEST_SYS_ID);

	// SOURCE APPLICATION ID  
	memcpy(&strBuf[LOC_SRC_API_ID], ptrNePacket->strSrcApiId, LEN_SRC_API_ID);

	// DESTINATION APPLICATION ID  
	memcpy(&strBuf[LOC_DEST_API_ID], ptrNePacket->strDestApiId,LEN_DEST_API_ID);

	// CONTINUE FLAG  
	memcpy(&strBuf[LOC_CONT_FLAG], &(ptrNePacket->usContFlag), LEN_CONT_FLAG);

	// SERIAL_NO  
	memcpy(&strBuf[LOC_SERIAL_NO], &(ptrNePacket->usSerialNo), LEN_SERIAL_NO);

	// DATA SIZE  
	memcpy(&strBuf[LOC_DATA_SIZE], &(ptrNePacket->unDataSize), LEN_DATA_SIZE);

	// DATA  
	memcpy(&strBuf[LOC_DATA], ptrNePacket->strData, ptrNePacket->unDataSize);
}


void Ccdsif::GetNeHeader(NEPACKETPTR ptrNePacket,char* strData)
{
	// MESSAGE ID  
	memcpy(&(ptrNePacket->unMsgId), &strData[LOC_MSG_ID], LEN_MSG_ID);

	// TRANSACTION ID  
	memcpy(ptrNePacket->stTid.strDate, &strData[LOC_TRANS_ID], 8);
	ptrNePacket->stTid.strDate[8] = 0x00;
	memcpy(&(ptrNePacket->stTid.nSeqNo), &strData[LOC_TRANS_ID + 8], 4);

	// SOURCE SYSTEM ID  
	memcpy(ptrNePacket->strSrcSysId, &strData[LOC_SRC_SYS_ID], LEN_SRC_SYS_ID);
	ptrNePacket->strSrcSysId[LEN_SRC_SYS_ID] = 0x00;

	// DESTINATION SYSTEM ID  
	memcpy(ptrNePacket->strDestSysId, &strData[LOC_DEST_SYS_ID], 
															LEN_DEST_SYS_ID);
	ptrNePacket->strDestSysId[LEN_DEST_SYS_ID] = 0x00;
	
	// SOURCE APPLICATION ID  
	memcpy(ptrNePacket->strSrcApiId, &strData[LOC_SRC_API_ID], LEN_SRC_API_ID);
	ptrNePacket->strSrcApiId[LEN_SRC_API_ID] = 0x00;

	// DESTINATION APPLICATION ID  
	memcpy(ptrNePacket->strDestApiId, &strData[LOC_DEST_API_ID], 
															LEN_DEST_API_ID);
	ptrNePacket->strDestApiId[LEN_DEST_API_ID] = 0x00;

	// CONTINUE FLAG  
	memcpy(&(ptrNePacket->usContFlag), &strData[LOC_CONT_FLAG], LEN_CONT_FLAG);

	// SERIAL_NO  
	memcpy(&(ptrNePacket->usSerialNo), &strData[LOC_SERIAL_NO], LEN_SERIAL_NO);

	// DATA SIZE  
	memcpy(&(ptrNePacket->unDataSize), &strData[LOC_DATA_SIZE], LEN_DATA_SIZE);
}

int Ccdsif::ReadLastTid(TIDPTR ptrTid)
{
	FILE	*fp;
	char	strFileName[100];
	time_t	tTime;
	struct tm	*tp;

	memset((TID *)&gstTid,0x00,sizeof(gstTid));
	sprintf(strFileName, "/SVC%d/DATA/CDS_TID.cfg", ServiceId);

	if ((fp = fopen(strFileName, "r")) == NULL) {
		*mqLog<<mqLog->ErrorCode(0)<<"file open failed ... "<<endl;

		// initialize  
		tTime = time(NULL);
		tp = localtime(&tTime);
		sprintf(ptrTid->strDate, "%04d%02d%02d",
					tp->tm_year+1900, tp->tm_mon+1, tp->tm_mday);
		ptrTid->nSeqNo = 0;
		return(0);
	}
	else {
//		fscanf(fp, "%s %d", ptrTid->strDate, &(ptrTid->nSeqNo));
		fread(ptrTid->strDate, 10, 1, fp);
		fread((void*)&(ptrTid->nSeqNo), 4, 1, fp);
	}
	fclose(fp);
	*mqLog<<mqLog->ErrorCode(0)<<"LastTid Date:"<<ptrTid->strDate<<" Seq:"<<ptrTid->nSeqNo<<endl;
	return(1);
}


int Ccdsif::SendRequestAck(int sdTcp, NEPACKET stNePacket, int nMsgType)
{
	int		nLen;
	short	sTemp;
	char	strBuf[CDS_MAX_TCP_BUF];

	// send AUDIT_DATA_REQUEST_ACK  
	GetRespHeaderAndOption(nMsgType, &stNePacket);

	// RESULT  
	memcpy(stNePacket.strData, "SC", 2);

	// REASON  
	sTemp = 0;
	memcpy(&(stNePacket.strData)[2], &sTemp, 2);

	// DATA LEN  
	stNePacket.unDataSize = 4;

	GetNePacketToSend(&stNePacket, strBuf);
	nLen = NE_PACKET_HEADER_LEN + stNePacket.unDataSize;
	*mqLog<<mqLog->ErrorCode(0)<<"<<<<<<<<<<<< Send  to  CDS >>>>>>>>>>>>"<<endl;
	PrintNePacket(stNePacket);
	
	if (send(sdTcp, (void *) strBuf, nLen, 0) != nLen) {
		*mqLog<<mqLog->ErrorCode(0)<<"send failed ("<<strerror(errno)<<")"<<endl;
		return(-1);
	}
	return(0);
}

int Ccdsif::ConnectToDB( void )
{
	strcpy((char *) user.arr, DB_USERNAME);
	user.len = (unsigned short) strlen(DB_USERNAME);

	strcpy((char *) password.arr, DB_PASSWORD);
	password.len = (unsigned short) strlen(DB_PASSWORD);
	
	/* EXEC SQL connect :user identified by :password; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )10;
 sqlstm.offset = (unsigned int  )24;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlstm.sqhstv[0] = (         void  *)&user;
 sqlstm.sqhstl[0] = (unsigned int  )34;
 sqlstm.sqhsts[0] = (         int  )34;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&password;
 sqlstm.sqhstl[1] = (unsigned int  )34;
 sqlstm.sqhsts[1] = (         int  )34;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	return(sqlca.sqlcode);
}

// command result ack , command requset ack 보낸후 write
void Ccdsif::WriteFileLog(int nflag, NEPACKET stNePacket)
{
	FILE *fp;
	char strFileName[100];
	char strLog[128];
	char strTmp[7];
	char strReult[5];
	char *strTime;
	char *strDate;
	
	memset (strFileName,0x00,100);
	memset (strLog,0x00,128);
	memset (strTmp,0x00,7);
	memset (strReult,0x00,5);
	
	strTime = dateTimeC.get_time(1);
	strDate = dateTimeC.get_date(0);
	sprintf(strFileName, "/SVC%d/LOG/CDS_LOG.%s", ServiceId, strDate);
	
	if ((fp = fopen(strFileName, "r")) == NULL) {
		 fp = fopen(strFileName, "w");
		 sprintf(strLog,"Time,TransactionID,Data/Result,Operation,MSID\n");
		 fprintf(fp,strLog);
		 sprintf(strLog,"%s,%08s-%06d,%s\n",strTime,stNePacket.stTid.strDate,
							stNePacket.stTid.nSeqNo,stNePacket.strData);
		 fprintf(fp,strLog);
		 fclose(fp);
	}else {
		fp = fopen(strFileName, "a");
		memset (strLog,0x00,128);
		switch(nflag) {
			
			case COMMAND_REQUEST_ACK:
				sprintf(strLog,"%s,%08s-%06d,%s\n",strTime,stNePacket.stTid.strDate,
							stNePacket.stTid.nSeqNo,stNePacket.strData);
				break;

			case COMMAND_RESULT_ACK:
				if(!strncmp(strJob,"A1",2)) memcpy(strTmp,"INSERT",6);
				else if(!strncmp(strJob,"Z1",2)) memcpy(strTmp,"DELETE",6);
				else if(!strncmp(strJob,"D1",2)) memcpy(strTmp,"UPDATE",6);
				
				if(nResult == 0) memcpy(strReult,"SUCC",4);
				else memcpy(strReult,"FAIL",4);
				
				sprintf(strLog,"%s,%08s-%06d,%s,%06s,%s\n",strTime,stNePacket.stTid.strDate,
							stNePacket.stTid.nSeqNo,strReult,strTmp,strLogMin);
		
				break;
		}
//		*mqLog<<mqLog->ErrorCode(0)<<"LOG: "<<strLog<<endl;
		fprintf(fp,strLog);
		fclose(fp);
	}
}	

	
int Ccdsif::ReleaseDB()
{
	/* EXEC	SQL	COMMIT	WORK	RELEASE; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )55;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



	return(sqlca.sqlcode); 
}


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
			*mqLog<<"PmpMQ IS FULL.. sleep(1)!"<<mqLog->ErrorCode(2005)<< endl;
			sleep(1);
			if (i >= 10)
			{
				kill(0,SIGINT);
			}
		}else break;
	}
}

void sigCapture(int sigNo)
{
	mqLog->put_EventHistData(FLAG_END,0);
	cdsifC->SaveTid(gstTid);
	cdsifC->ConnectionRelease(1, gSendport, gNeP );
	cdsifC->Process4Cds(&gSendport);
	cdsifC->ConnectionRelease(2, gRecvport, gNeP );
	cdsifC->Process4Cds(&gRecvport);
//	delete cdsifC;

	switch(sigNo)
	{
		case SIGINT :
		case SIGQUIT :
		case SIGKILL :
		case SIGTERM :
				WritePMSMQ(-3);//-1//프로세스 비정상 stop (killed)
				delete mqPMS;
				exit(-1);
				break;
		case SIGUSR1:
				WritePMSMQ(-3);//0//프로세스정상stop (Client업무명령)
				delete mqPMS;
				exit(0);
				break;
		default:
				WritePMSMQ(-3);//-1//프로세스 비정상 stop (killed)
				delete mqPMS;
				exit(-1);
				break;
	}
}

