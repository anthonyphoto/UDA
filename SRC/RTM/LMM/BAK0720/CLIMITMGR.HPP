#ifndef _CLIMITMGR_HPP
#define _CLIMITMGR_HPP

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <SendLogManage.hpp>							// Log�� �����ϴ� Ŭ����
#include <scpformat.hpp>								// SCP packet 
#include <RateRule.hpp>									// packet ��� ���
#include "CLimitTbl.hpp"								// �ѵ����� ���̺� Handling
#include <UDR.hpp>										// UDR format

#define		UDR_START			0						// START UDR
#define		UDR_INTRIM			1						// INTERIM UDR
#define		UDR_STOP			2						// STOP UDR
#define		LIMIT_RESULT		0x81					// �ѵ� ���� ��ȸ ���
#define		ACCESS_RESULT		0x82					// �߰� ���� ���

#define		ACCESS_REQ		0							// �ѵ�������û
#define		INTRIM_ACCESS	1							// �߰������û
#define		SVC_STOP		2							// �ѵ��ʰ�, ���� �ߴܿ�û
#define		SVC_END			3							// STOP UDR�� ���ŵǾ��� �� SCP�� ������ ��� ���� ��û 

// CLimitMGR -> Scp-if(S)
MessageQueue<SCPFormatAgg>	*mqSCP;		// �ѵ���û �� �߰������û �۽�
MessageQueue<UdrFormat>		*mqLMGR;	// �ѵ���û ��� �� �߰������� ����, filter���� �ѵ� UDR����
SendLogManage				*LogM;		//	Log Manager
MessageQueue<MsgPmsStatus>* mqPMS;		// PMS MQ 

////////////////// PMS���� //////////////////////
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

CLimitTbl		stTblMgr;	// �ѵ����� table Handling



//------------------------------------------------------------------------------
class	CLimitMGR
{
		private:
			UdrFormat		stUdr	;	// Filter�κ��� �޴� UDR,SCP�κ��� ������� UDR 
			SCPFormatAgg    stAccReqMsg;// SCP I/F�� ����ϱ� ���� Data Format
			RateRule		*stRate;	// �־��� ��Ŷ���� ���� �ݾ��� ����ϱ� ���� �ν��Ͻ�
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



