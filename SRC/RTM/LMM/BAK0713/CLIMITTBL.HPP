#ifndef _CLIMITTBL_HPP
#define _CLIMITTBL_HPP

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <SendLogManage.hpp>

#define		DB_USERNAME		"uda"
#define		DB_PASSWORD		"uda"
#define		LEN_ID			8


class	CLimitTbl 
{
	private:
		int		nMin;
		char	strAccSId[LEN_ID+1];	// Account Session ID
		char	strCorrId[LEN_ID+1];	// Correlation ID
		int		nLimitAmt;			// 한도 금액
		int		nCurrentAmt;		// 현재 쓴 금액
		int		nRateAmt;			// Rating된 금액
		int		nLastDataOct;		// 예전 Data Size
		int		nOId;				// operation Id 세션이 끝날때 까지 유지한다
		int		nIntrimCnt;			// 5번에 한번 중간정산한다
		int		nTotalAmt;
		long	nEventTime;
		
	public:
		CLimitTbl();
		~CLimitTbl();
		int		ConnectToDB(void);
		int		DbClose(void);
		
		void	SetMin(int nVal)		{ nMin = nVal; }
		void	SetAccSId(char *str)	{ memset(strAccSId, 0x00,LEN_ID+1); strcpy(strAccSId, str); }
		void	SetCorrId(char *str)	{ memset(strCorrId, 0x00,LEN_ID+1); strcpy(strCorrId, str); }
		void	SetLimitAmt(int nVal)	{ nLimitAmt = nVal; }
		void	SetCurrentAmt(int nVal)	{ nCurrentAmt = nVal; }
		void	SetRateAmt(int nVal)	{ nRateAmt = nVal; }
		void	SetLastDataOct(int nVal){ nLastDataOct = nVal; }
		void	SetOId(int nVal)		{ nOId = nVal; }
		void	SetIntrimCnt(int nVal)	{ nIntrimCnt = nVal; }
		void	SetTotalAmt(int nVal)	{ nTotalAmt = nVal; }
		void	SetEventTime(long nVal)	{ nEventTime = nVal; }	
		
		int		GetMin(void)			{ return nMin; }
		char*	GetAccSId(void)	      	{ return strAccSId; }
		char*	GetCorrId(void)	      	{ return strCorrId; }
		int		GetLimitAmt(void)	    { return nLimitAmt; }
		int		GetCurrentAmt(void)	  	{ return nCurrentAmt; }
		int		GetRateAmt(void)	  	{ return nRateAmt; }
		int		GetLastDataOct(void)    { return nLastDataOct; }
		int		GetIntrimCnt(void)		{ return nIntrimCnt; }
		int		GetOId(void)			{ return  nOId; }
		int		GetTotalAmt(void)		{ return  nTotalAmt; }
		long	GetEventTime(void)		{ return  nEventTime; }
		int		UpdateLimitInfo1(void);	// Intrim UDR Recv T_THRESHOLD_AMT, T_THRESHOLD_MID_AMT update
		int		UpdateLimitInfo2(void);	// Stop UDR Recv T_THRESHOLD_AMT update
		int		UpdateLimitAmt(void);	// 중간정산 결과가 오면 update
		int		DeleteLimitInfo(void);	// stop이 오면 결과를 보내고...table에서 delete
		int 	DeleteAllLimitInfo(void);// 한도가 넘었을 때 전체 정보삭제
		// 두개의 table에서 정산에 필요한 정보를 읽어온다. nLimitAmt, nLastDataOct,nRemainAmt,nOId,nIntrimCnt
		int		GetLimitInfo(void);	
		int		SelectOId(void);
		int		UpdateOId(void);
		int		UpdateLimitNRateAmt(void);
		
};

#endif

