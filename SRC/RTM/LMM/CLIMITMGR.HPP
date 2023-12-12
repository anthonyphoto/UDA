/******************************************************************
* 파 일 명		:	CLimitMGR.hpp
* 기   능		:	Filter로 부터 들어온 한도 UDR을 관리하는 프로그램
* 작 성 자		:	정중기, 신미연
* first data	:	2001. 07.19
* last updata	:	2001. 07.19
* 관 련 화 일	:	CLimitMGR.cpp
* program history

* Version		:	1.1 ( 2001. 07.19 )
******************************************************************/


#ifndef _CLIMITMGR_HPP
#define _CLIMITMGR_HPP

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <SendLogManage.hpp>							// Log를 관리하는 클래스
#include <scpformat.hpp>								// SCP packet 
#include "RateRule.hpp"									// packet 요금 계산
#include <UDR.hpp>										// UDR format
#include <SqlCommon.hpp>
#include "CLimitTbl.hpp"								// 한도관리 테이블 Handling

#define		UDR_START			0						// START UDR
#define		UDR_INTRIM			1						// INTERIM UDR
#define		UDR_STOP			2						// STOP UDR

#define		LIMIT_RESULT		0x81					// 한도 정보 조회 결과 메세지 ID
#define		ACCESS_RESULT		0x82					// 중간 정산 결과 메세지 ID
#define		ACCESS_REQ_CNT		3						// SCP로 중간 정산 요청 간격 (Interim count)

MessageQueue<SCPFormatAgg>		*mqSCP;					// 한도요청 및 중간정산요청 송신
MessageQueue<UdrFormat>			*mqLMGR;				// 한도요청 결과 및 중간정산결과 수신, filter에서 한도 UDR수신
MessageQueue<MsgPmsStatus>		*mqPMS;					// PMS MQ 
SendLogManage					*LogM;					// Log Manager
RateRule						*stRate;				// 주어진 패킷량에 따라 금액을 계산하기 위한 인스턴스

static int	SystemId;									// System id
static int	ServiceId;									// Service id
static int	GroupId;									// Group id	
static int	ProcessType;								// Process type
static int 	PserialNo;									// Process serial number
static int	PreMark;									// Process remark

void sigCapture(int sigNo );							// Signal 처리
void WritePMSMQ(int sigNo);	



///////////////////////////////////
//			Class CLimitMGR		 //
///////////////////////////////////
// Filter로 부터 들어온 한도 UDR을 관리하는 클래스
class CLimitMGR
{
		private:
			UdrFormat		stUdr;						// Filter로부터 받는 UDR,SCP로부터 결과받은 UDR 
			SCPFormatAgg    stAccReqMsg;				// SCP I/F와 통신하기 위한 Data Format

			int				nTatalDataOctet;			// DOCT + DOCO
			int				nRatingPrice;				// 사용 요금
		

		public:
			CLimitMGR();								// 생성자
			~CLimitMGR();								// 소멸자

			DbConnect		IDbConnect;
			CLimitTbl		ILimitTbl;					// 한도관련 table Handling
			
			int		Start();							// 한도 관리의 전 과정을 관리

			int		ProcStart();						// START UDR이 들어왔을 때 처리
			int		ProcIntrim();						// INTERIM UDR이 들어왔을 때 처리
			int		ProcStop();							// STOP UDR이 들어왔을 때 처리

			// 한도관리 관련 데이블을 update한 후, SCP로 보낼 중간 정산 요청 메세지를 만들고, SCP MQ로 메세지를 보냄
			int		InterimAccountReq(int nPreviousRate, int nStopFlag = 0);
			// 총사용 금액이 한도 초과했는지 확인한 후,
			// 초과했다면 SCP로 보낼 Service stop request 메세지를 만들고, SCP MQ로 메세지를 보냄
			int		LimitMGR(int nPreLimitAmt, int nPreviousRate);
			// STOP UDR이 들어 왔을 때 SCP로 보낼 Service end request 메세지를 만들고, SCP MQ로 메세지를 보냄
			void	SndServiceEndReq();

			void	SndReqToSCP();						// SCP MQ로 메세지를 보냄
			int		DoResProcess();						// SCP로 부터 메세지 요청에 대한 응답을 받았을 때 처리
};

#endif
