#ifndef _CLIMITMGR_HPP
#define _CLIMITMGR_HPP

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <SendLogManage.hpp>// MQ 
#include <scpformat.hpp>	// SCP packet 
#include <RateRule.hpp>		// packet 요금 계산
#include "CLimitTbl.hpp"	// 한도관리 테이블 Handling
#include <UDR.hpp>			// UDR format

#define		UDR_START			0	// start udr
#define		UDR_INTRIM			1	// intrim udr
#define		UDR_STOP			2	// stop udr
#define		LIMIT_RESULT		0x81// stop udr
#define		ACCESS_RESULT		0x82// stop udr

#define		ACCESS_REQ		0	// 한도정보요청
#define		INTRIM_ACCESS	1	// 중간정산요청
#define		SVC_STOP		2	// 한도초과, 서비스 중단요청
#define		SVC_END			3	// stop udr이 들어왔을 때 보낸다

// CLimitMGR -> Scp-if(S)
MessageQueue<SCPFormatAgg>	*mqSCP;		// 한도요청 및 중간정산요청 송신
MessageQueue<UdrFormat>		*mqLMGR;	// 한도요청 결과 및 중간정산결과 수신, filter에서 한도 UDR수신
//	Log Manager
SendLogManage				*LogM;
// PMS MQ 
MessageQueue<MsgPmsStatus>* mqPMS;

////////////////// PMS관련 //////////////////////
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

//////////////////// END /////////////////////

static int	SystemId;
static int	ServiceId;
static int	GroupId;
static int	ProcessType;
static int 	PserialNo;
static int	PreMark;

void sigCapture(int sigNo );
void WritePMSMQ(int sigNo);

CLimitTbl		stTblMgr;	// 한도관련 table Handling

class	CLimitMGR
{
		private:
			UdrFormat		stUdr	;	// Filter로부터 받는 UDR,SCP로부터 결과받은 UDR 
//			AccountCnt*		pAccReqCnt; // 한도정보요청 count를 가져오기 위한 인스턴스
			SCPFormatAgg    stAccReqMsg;// SCP I/F와 통신하기 위한 Data Format
			RateRule		*stRate;	// 주어진 패킷량에 따라 금액을 계산하기 위한 인스턴스
			int				gnAccReqCnt;
			int				nTatalDataOctet;
		
		public:
			CLimitMGR(int);
			~CLimitMGR();
			void Start();
			void ProcStart();
			void ProcIntrim();
			void ProcStop();
			void SndReqToScp();
			void SndReqToSCP();
			void DoResProcess();
};

#endif




