#ifndef _COMMON_MSG_H
#define	_COMMON_MSG_H

#include <sys/stat.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <intndefs.hpp>
#include <socketC.hpp>
#include <SendLogManage.hpp>// log manager
#include <MsgFormat.hpp>
#include <TempMsgQ_file.hpp>
#include <time.hpp>
#include <CdsCustFormat.hpp>
#include <hexdump.h>
#include <fileSearchC.hpp>
#include "udpC.hpp"

#define  DB_USERNAME  "uda"
#define  DB_PASSWORD  "uda"

#define  SYSTEM2_ALIVE	  	1
#define  SYSTEM2_NOTALIVE 	0
#define  UDA1_IPADDR		"150.25.4.140"
#define  UDA2_IPADDR		"150.25.4.142"
#define	 CDSIF_UDP_PORT 	6100

// values of cLinkStatus  
#define LINK_STATUS_INIT                    0
#define LINK_STATUS_READY                   1
#define LINK_STATUS_BUSY                    2
#define LINK_STATUS_CLOSED_ABNORMAL         3
#define LINK_STATUS_CLOSED_BY_NE_REQUEST    4
#define LINK_STATUS_CLOSED_BY_MCS_REQUEST   5
#define LINK_STATUS_CLOSED_BY_IP_ERROR   	6

// values of fTidReset  
#define TID_RESET_READY                     0
#define TID_RESET_REQUEST                   1

// values of fLinkType  
#define LINK_TYPE_SCHANNEL		'S'
#define LINK_TYPE_RCHANNEL		'R'

// Mesage Identifier  
#define SCHANNEL_CONNECTION_REQUEST				1
#define SCHANNEL_CONNECTION_REQUEST_ACK			2
#define RCHANNEL_CONNECTION_REQUEST				3
#define RCHANNEL_CONNECTION_REQUEST_ACK			4
#define SCHANNEL_RELEASE_REQUEST				5
#define SCHANNEL_RELEASE_REQUEST_ACK			6
#define RCHANNEL_RELEASE_REQUEST				7
#define RCHANNEL_RELEASE_REQUEST_ACK			8
#define SYSTEM_STATE_REPORT						11
#define SYSTEM_STATE_REPORT_ACK					12
#define PROCESS_STATE_REQUEST					13
#define PROCESS_STATE_REQUEST_ACK				14
#define COMMAND_REQUEST							15
#define COMMAND_REQUEST_ACK						16
#define COMMAND_RESULT							17
#define COMMAND_RESULT_ACK						18
#define AUDIT_DATA_REQUEST						19
#define AUDIT_DATA_REQUEST_ACK					20
#define AUDIT_DATA_COMPLETE						21
#define AUDIT_DATA_COMPLETE_ACK					22
#define SUBS_DATA_REQUEST						29
#define SUBS_DATA_REQUEST_ACK					30
#define SUBS_DATA_RESULT						31
#define SUBS_DATA_RESULT_ACK					32
#define SCHANNEL_TID_RESET_REQUEST				37
#define SCHANNEL_TID_RESET_REQUEST_ACK			38
#define RCHANNEL_TID_RESET_REQUEST				39
#define RCHANNEL_TID_RESET_REQUEST_ACK			40

// Error Code : CDS COMMON

#define ERR_NOT_EXIST						1
#define ERR_NOT_REGISTERED					2
#define ERR_PREFIX_NOT_EXIST				3
#define ERR_ALREADY_EXIST					4
#define ERR_ALREADY_REGISTERED				5
#define ERR_MIN_STOLEN						9
#define ERR_CHANNEL_NOT_EXIST				10
#define ERR_WRONG_HEADER					11
#define ERR_WRONG_SIZE						12
#define ERR_WRONG_DATA						13
#define ERR_MISMATCH_TID					14
#define ERR_DB_FAULT						21
#define ERR_UNRECOGNIZED_MESSAGE			52
#define ERR_COMMAND_NOT_SUPPORT				54

// Length of each field of Standard Message  

#define LEN_MSG_ID				4
#define LEN_TRANS_ID			12
#define LEN_SRC_SYS_ID			6
#define LEN_DEST_SYS_ID			6
#define LEN_SRC_API_ID			6
#define LEN_DEST_API_ID			6
#define LEN_CONT_FLAG			2
#define LEN_SERIAL_NO			2
#define LEN_DATA_SIZE			4


// Location of each field of Standard Message  

#define LOC_MSG_ID					0
#define LOC_TRANS_ID				4
#define LOC_SRC_SYS_ID				16
#define LOC_DEST_SYS_ID				22
#define LOC_R_TRANS_ID				28
#define LOC_SRC_API_ID				28
#define LOC_DEST_API_ID				34
#define LOC_CONT_FLAG				40
#define LOC_SERIAL_NO				42
#define LOC_DATA_SIZE				44
#define LOC_DATA					48

// Header Length  
#define MAX_NE_PACKET				512
#define NE_PACKET_HEADER_LEN 		LOC_DATA
#define NE_PACKET_MAX_DATA_LEN		MAX_NE_PACKET - NE_PACKET_HEADER_LEN
#define CDS_MAX_TCP_BUF				2048

// Subs information
#define	LEN_JOB					2
#define LEN_MIN					12
#define LEN_RESERV				6
#define LOC_JOB					0
#define LOC_MIN					2
#define LOC_NEW_MIN				14
#define LOC_RESERV				26

// Standard Message Format between CDS and NE  
typedef struct {
	char	strDate[8];			// YYYYMMDD  
	int		nSeqNo;				// sequence no  
} TID, *TIDPTR;

typedef struct {
	// HEADER  
	unsigned int	unMsgId;					// MESSAGE ID  
	TID		stTid;								// TRANSACTION ID  	
	char	strSrcSysId[LEN_SRC_SYS_ID + 1];	// SOURCE SYSTEM ID  
	char	strDestSysId[LEN_DEST_SYS_ID + 1];	// DESTINATION SYSTEM ID  
	char	strSrcApiId[LEN_SRC_API_ID + 1];	// SOURCE API ID  
	char	strDestApiId[LEN_DEST_API_ID + 1];	// DESTINATION API ID  
	unsigned short	usContFlag;					// CONTINUE FLAG  
	unsigned short	usSerialNo;					// SERIAL NO  
	unsigned int	unDataSize;					// DATA SIZE  

	// DATA  
	char	strData[NE_PACKET_MAX_DATA_LEN + 1];// DATA  	
} NEPACKET, *NEPACKETPTR;

typedef struct {
	// HEADER  
	unsigned int	unMsgId;					// MESSAGE ID  
	TID		stTid;								// TRANSACTION ID  	
	char	strSrcSysId[LEN_SRC_SYS_ID + 1];	// SOURCE SYSTEM ID  
	char	strDestSysId[LEN_DEST_SYS_ID + 1];	// DESTINATION SYSTEM ID  
	TID		stRid;								// R Chl TRANSACTION ID  	
	unsigned short	usContFlag;					// CONTINUE FLAG  
	unsigned short	usSerialNo;					// SERIAL NO  
	unsigned int	unDataSize;					// DATA SIZE  

	// DATA  
	char	strData[NE_PACKET_MAX_DATA_LEN + 1];// DATA  	
} UPPACKET, *UPPACKETPTR;


// PMS Status Report

struct _DATANODE_PMS
{
	int		sys_id;			// MQHeader | system type   uda1 or uda2
	int		svc_id;			// MQHeader | service type  1 or 2 or ...
	int		group_type;		// MQHeader | group id
	int		proc_type;		// MQHeader | process type
	int		proc_sn;		// MQHeader | process serial
	int		proc_remark;	// MQHeader | process remark
	int		msg_type;		// MQHeader | msg type
	int		pid;			// MQData   | pid
	int		status_flag;	// MQData   | status flag
};

class	DataNodePMS	: public _DATANODE_PMS
{
	public:
		operator struct _DATANODE_PMS* () const { return (_DATANODE_PMS*)this; }
};
DataNodePMS		OneRec;

void WritePMSMQ(int sigNo);
void sigCapture(int sigNo );


class Ccdsif
{
	public:
		Ccdsif(char*, int, int);
		~Ccdsif();
		void run(void);
		void cdsMainLoop1(void);
		void cdsMainLoop2(void);
		void GetResHeader(NEPACKET);
		void GetComHeader(NEPACKET);
		void GetRespHeaderAndOption(int, NEPACKETPTR);
		void PrintNePacket(NEPACKET);
		void GetNePacketToSend(NEPACKETPTR, char *);
		void GetNeHeader(NEPACKETPTR ,char *);
		void PrintResult(CdsResFormat resultMsg);
		void PrintSubsInfo(CdsCustFormat msgToCIM);
		void WriteFileLog(int nflag, NEPACKET stNePacket);
		int ConnectToDB(void);
		int ReleaseDB(void);
		int CheckSystem2(void);
		int Process4Cds(int *);
		int SendCisUnrecognizedMsg(int, NEPACKET);
		int ProcessStateRequest(int, NEPACKET);
		int ProcessCommand(int, NEPACKET);
		int SendResToCds(int sd);
		int SendResultToCds(int, TID, char *, short);
		int CommandConnectionRequest(int, NEPACKET, TID);
		int CommandReleaseRequest(int, NEPACKET);
		int CommandRequest(int, TIDPTR, NEPACKET);
		int CheckAndUpdateTransId(TIDPTR, TIDPTR);
		int ResultConnectionRequest(int, NEPACKET);
		int ResultReleaseRequest(int, NEPACKET);
		int ReadLastTid(TIDPTR);
		int SaveTid(TID);
		int SendRequestAck(int, NEPACKET, int);
		int SendSubsDataResult(int, TID);
		int SendSubsInfoToCIM(NEPACKET stNePacket);
		int ConnectionRelease(int channel, int sdTcp, NEPACKET stNePacket );
		int AcceptSocket(int);

	private:
		
		SockC  *comSockC; // Command Channel Port
		SockC  *resSockC; // Result Channel Port
		UdpSockC udpSockC;
		DateTime dateTimeC;
							


		int 	nListenComCh, nAcceptComCh;
		int 	nListenResCh, nAcceptResCh;
		int		gnCommChPortNo, gnResChPortNo; 

	
		char	gstrComSrcSysId[8];			// COMMAND / SOURCE SYSTEM ID
		char    gstrComDestSysId[8];        // COMMAND / DESTINATION SYSTEM ID
		char    gstrComSrcApiId[8];         // COMMAND / SOURCE PROCESS ID
		char    gstrComDestApiId[8];        // COMMAND / DESTINATION PROCESS ID
		char	gstrResSrcSysId[8];			// RESULT / SOURCE SYSTEM ID
		char    gstrResDestSysId[8];        // RESULT / DESTINATION SYSTEM ID
		char    gstrResSrcApiId[8];         // RESULT / SOURCE PROCESS ID
		char    gstrResDestApiId[8];        // RESULT / DESTINATION PROCESS ID
};

#endif

