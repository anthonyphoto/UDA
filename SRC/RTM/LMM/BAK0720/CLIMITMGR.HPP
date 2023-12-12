#ifndef _CLIMITMGR_HPP
#define _CLIMITMGR_HPP

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <SendLogManage.hpp>							// Log를 관리하는 클래스
#include <scpformat.hpp>								// SCP packet 
#include <RateRule.hpp>									// packet 요금 계산
#include "CLimitTbl.hpp"								// 한도관리 테이블 Handling
#include <UDR.hpp>										// UDR format

#define		UDR_START			0						// START UDR
#define		UDR_INTRIM			1						// INTERIM UDR
#define		UDR_STOP			2						// STOP UDR
#define		LIMIT_RESULT		0x81					// 한도 정보 조회 결과
#define		ACCESS_RESULT		0x82					// 중간 정산 결과

#define		ACCESS_REQ		0							// 한도정보요청
#define		INTRIM_ACCESS	1							// 중간정산요청
#define		SVC_STOP		2							// 한도초과, 서비스 중단요청
#define		SVC_END			3							// STOP UDR이 수신되었을 때 SCP로 가입자 요금 정산 요청 

// CLimitMGR -> Scp-if(S)
MessageQueue<SCPFormatAgg>	*mqSCP;		// 한도요청 및 중간정산요청 송신
MessageQueue<UdrFormat>		*mqLMGR;	// 한도요청 결과 및 중간정산결과 수신, filter에서 한도 UDR수신
SendLogManage				*LogM;		//	Log Manager
MessageQueue<MsgPmsStatus>* mqPMS;		// PMS MQ 

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



//------------------------------------------------------------------------------
class	CLimitMGR
{
		private:
			UdrFormat		stUdr	;	// Filter로부터 받는 UDR,SCP로부터 결과받은 UDR 
			SCPFormatAgg    stAccReqMsg;// SCP I/F와 통신하기 위한 Data Format
			RateRule		*stRate;	// 주어진 패킷량에 따라 금액을 계산하기 위한 인스턴스
			int				gnAccReqCnt;
			int				nTatalDataOctet;
			int				nRatingPrice;
		
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

			void SndServiceEndReq();
			void InterimAccountReq(int nPreviousRate, int nPreRemainAmt,int nStopFlag = 0);
			void LimitMGR(int nPreLimitAmt, int nPreviousRate);
};

#endif




