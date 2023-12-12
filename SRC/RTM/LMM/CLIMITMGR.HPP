/******************************************************************
* �� �� ��		:	CLimitMGR.hpp
* ��   ��		:	Filter�� ���� ���� �ѵ� UDR�� �����ϴ� ���α׷�
* �� �� ��		:	���߱�, �Ź̿�
* first data	:	2001. 07.19
* last updata	:	2001. 07.19
* �� �� ȭ ��	:	CLimitMGR.cpp
* program history

* Version		:	1.1 ( 2001. 07.19 )
******************************************************************/


#ifndef _CLIMITMGR_HPP
#define _CLIMITMGR_HPP

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <SendLogManage.hpp>							// Log�� �����ϴ� Ŭ����
#include <scpformat.hpp>								// SCP packet 
#include "RateRule.hpp"									// packet ��� ���
#include <UDR.hpp>										// UDR format
#include <SqlCommon.hpp>
#include "CLimitTbl.hpp"								// �ѵ����� ���̺� Handling

#define		UDR_START			0						// START UDR
#define		UDR_INTRIM			1						// INTERIM UDR
#define		UDR_STOP			2						// STOP UDR

#define		LIMIT_RESULT		0x81					// �ѵ� ���� ��ȸ ��� �޼��� ID
#define		ACCESS_RESULT		0x82					// �߰� ���� ��� �޼��� ID
#define		ACCESS_REQ_CNT		3						// SCP�� �߰� ���� ��û ���� (Interim count)

MessageQueue<SCPFormatAgg>		*mqSCP;					// �ѵ���û �� �߰������û �۽�
MessageQueue<UdrFormat>			*mqLMGR;				// �ѵ���û ��� �� �߰������� ����, filter���� �ѵ� UDR����
MessageQueue<MsgPmsStatus>		*mqPMS;					// PMS MQ 
SendLogManage					*LogM;					// Log Manager
RateRule						*stRate;				// �־��� ��Ŷ���� ���� �ݾ��� ����ϱ� ���� �ν��Ͻ�

static int	SystemId;									// System id
static int	ServiceId;									// Service id
static int	GroupId;									// Group id	
static int	ProcessType;								// Process type
static int 	PserialNo;									// Process serial number
static int	PreMark;									// Process remark

void sigCapture(int sigNo );							// Signal ó��
void WritePMSMQ(int sigNo);	



///////////////////////////////////
//			Class CLimitMGR		 //
///////////////////////////////////
// Filter�� ���� ���� �ѵ� UDR�� �����ϴ� Ŭ����
class CLimitMGR
{
		private:
			UdrFormat		stUdr;						// Filter�κ��� �޴� UDR,SCP�κ��� ������� UDR 
			SCPFormatAgg    stAccReqMsg;				// SCP I/F�� ����ϱ� ���� Data Format

			int				nTatalDataOctet;			// DOCT + DOCO
			int				nRatingPrice;				// ��� ���
		

		public:
			CLimitMGR();								// ������
			~CLimitMGR();								// �Ҹ���

			DbConnect		IDbConnect;
			CLimitTbl		ILimitTbl;					// �ѵ����� table Handling
			
			int		Start();							// �ѵ� ������ �� ������ ����

			int		ProcStart();						// START UDR�� ������ �� ó��
			int		ProcIntrim();						// INTERIM UDR�� ������ �� ó��
			int		ProcStop();							// STOP UDR�� ������ �� ó��

			// �ѵ����� ���� ���̺��� update�� ��, SCP�� ���� �߰� ���� ��û �޼����� �����, SCP MQ�� �޼����� ����
			int		InterimAccountReq(int nPreviousRate, int nStopFlag = 0);
			// �ѻ�� �ݾ��� �ѵ� �ʰ��ߴ��� Ȯ���� ��,
			// �ʰ��ߴٸ� SCP�� ���� Service stop request �޼����� �����, SCP MQ�� �޼����� ����
			int		LimitMGR(int nPreLimitAmt, int nPreviousRate);
			// STOP UDR�� ��� ���� �� SCP�� ���� Service end request �޼����� �����, SCP MQ�� �޼����� ����
			void	SndServiceEndReq();

			void	SndReqToSCP();						// SCP MQ�� �޼����� ����
			int		DoResProcess();						// SCP�� ���� �޼��� ��û�� ���� ������ �޾��� �� ó��
};

#endif