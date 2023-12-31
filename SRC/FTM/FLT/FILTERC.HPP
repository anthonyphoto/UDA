#ifndef	udaFilterC_HEADER_
#define	udaFilterC_HEADER_

#include <pthread.h>
#include <iostream.h>
#include <sys/pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
//#include <TempMsgQ_new.hpp>
#include <TempMsgQ_file.hpp>
#include <UDR.hpp>		    // udr format
#include <time.hpp>
#include <SendLogManage.hpp>// log manager
#include <MsgFormat.hpp>
#include <CustMdbMan.hpp> // TTL가입자 MDB조회관련 
#include <fileSearchC.hpp>

#define  DB_USERNAME  "uda"
#define  DB_PASSWORD  "uda"
#define  SC	 			0
#define  FA 		   -1
#define  SVC_PACKET		0 // 일반
#define  SVC_NTOP		1 // nTop 서비스
#define  SUBS_NORMAL	0 // 일반 가입자
#define  SUBS_TTL		1 // TTL School가입자
#define  MAX_AGG_CNT	10

typedef struct _proInfo
{
	int nSystemId;
	int nServiceId;
	int nGroupType;
	int nProcType;
	int nProcSn;
	int nProcRmk;
}PROCINFO;

struct _DATANODE_PMS
{
	int		sys_id;			// MQHeader | system type   uda1 or uda2
	int		svc_id;			// MQHeader | service type  1 or 2 or ...
	int		group_type;		// MQHeader | group id
	int		proc_type;		// MQHeader | process type
	int		proc_sn;		// MQHeader | process serial
	int		proc_remark;		// MQHeader | process remark
	int		msg_type;		// MQHeader | msg type
	int		pid;			// MQData   | pid
	int		status_flag;		// MQData   | status flag
};

class	DataNodePMS	: public _DATANODE_PMS
{
	public:
		operator struct _DATANODE_PMS* () const { return (_DATANODE_PMS*)this; }
};

class filterC
{
	private:
		int          nUdrMin;
		unsigned int nLastSeqR;
		DateTime	 dateTime;
		CCustMdbMan  *subsInfoC;
		
	public:
		filterC();
		~filterC();
		void run(void);
		void display(void);
		int ConnectToDB(void);
		int ReleaseDB(void);
		int saveLastSeqNum();
		void setSubsInfo(void);
		
};

int 			nDataCnt;
int				nUdrType;
UdrFormat 		gStUdr;
UDRHEAD_TYPE	*gStUdrH;
PROCINFO		processInfo;
DataNodePMS		OneRec;

// recv from converter 
MessageQueue<UdrFormat> *filterMq;   // parser to filter
// define queue send udr to aggregatorNo.0 ~ 10 : filter to Aggregator
MessageQueue<UdrFormat> *AggMq[MAX_AGG_CNT]; 
// PMS MQ 
MessageQueue<MsgPmsStatus>* mqPMS;
// Log 
SendLogManage	*logWrite;
MessageQueue<UdrFormat>		*mqLMGR;	// 한도 UDR송신 2001.07.01

static int	SystemId;
static int	ServiceId;
static int	GroupId;
static int	ProcessType;
static int 	PserialNo;
static int	PreMark;

void sigCapture(int sigNo );
void WritePMSMQ(int sigNo);

#endif

