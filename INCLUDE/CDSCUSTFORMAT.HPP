/******************************************************************
* 파 일 명     : CdsCustFormat.hpp		     					  *
* 기   능                                                          *
* : CDS I/F와 통신하는 프로세스나 MQ들이 주고받는 class 정의			  *
* 작 성 자     : 한 근 희               		      				  *
* first data  :  				  	     						  *
* last updata : 2000. 12. 26  by 한 근 희   	 					  *
* 관 련 화 일  : 							     					  *
* program history                        						  *
*  2000.12.26 -> 프로그램 주석추가        	 				 		  *
* bug fix                        	     						  *
*                         	        	 						  *
******************************************************************/
#ifndef CDSCUSTFORMAT_HPP
#define	CDSCUSTFORMAT_HPP

#include<stdio.h>
#include<string.h>

typedef unsigned short 	sint;
typedef unsigned int 	uint;
typedef unsigned char	uchar;

//===============================================================
// CdsResFormat class											=
//  : CDS I/F에게 ACK응답을 할때 사용하는 format			  		=
// argument : None												=
// return value : None											=
//																=
//===============================================================
class CdsResFormat // size 16byte
{
	private:
		char 	strDate[8];
		uint	nSeqNum;
		uint	nRespond; // SC: 1, FA: 0
		
	public:
		void	set_strDate(const char *strValue)	{	memcpy(strDate, strValue, sizeof(strDate)); }
		void	set_nSeqNum(uint key)				{	nSeqNum=key; }
		void	set_nRespond(uint key)				{	nRespond=key; }
		
		char*	get_strDate(void)					{	return strDate; }
		uint	get_nSeqNum(void)					{	return nSeqNum; }
		uint	get_nRespond(void)					{	return nRespond; }
		
		CdsResFormat()
		{
			memset(strDate, 0x00, sizeof(strDate));
			nSeqNum=0;	
			nRespond=0;				
		}
};
//===============================================================
// CustFormat class												=
//  : CDS I/F와 고객관리간의 MQ정의 format					  		=
// argument : None												=
// return value : None											=
// 2000.11.15 : 변경될 가입자번호 추가								=
//===============================================================
class CustFormat // size 25byte
{
	private:
		
	public:
		uchar	Code;		// 0x11
		char	JobCode[2];	// A1:신규가입 Z1:가입해지 D1:번호변경
		char	Min[10];	
		char	Min2[10];	
		char	AccInfo[2];	// 01:TTL
		
		//uint	UTransactionID;	// SCP고객정보 수정에 대한요청 처리결과를 돌려줄때 필요
		
		CustFormat()
		{
			Code=0;
			memset(JobCode, 0x00, sizeof(JobCode));
			memset(Min, 	0x00, sizeof(Min));
			memset(Min2, 	0x00, sizeof(Min2));
			memset(AccInfo, 0x00, sizeof(AccInfo));
			//UTransactionID=0;				
		}
		
		void	set_Code(uint key)					{	Code=key; }
		void	set_JobCode(const char *strValue)	{	memcpy(JobCode, strValue, sizeof(JobCode)); }
		void	set_Min(const char *strValue)		{	memcpy(Min, 	strValue, sizeof(Min)); 	}
		void	set_Min2(const char *strValue)		{	memcpy(Min2, 	strValue, sizeof(Min2)); 	}
		void	set_AccInfo(const char *strValue)	{	memcpy(AccInfo,	strValue, sizeof(AccInfo));	}
		//void	set_UTransactionID(uint key)		{	UTransactionID=key; }
		
		uchar	get_Code(void)			{	return Code; }
		char*	get_JobCode(void)		{	return JobCode; }
		char*	get_Min(void)			{	return Min; }
		char*	get_Min2(void)			{	return Min2; }
		char*	get_AccInfo(void)		{	return AccInfo; }
		//uint	get_UTransactionID(void){	return UTransactionID; }
		
		void	prn_Code(void)			{ printf("[Code      ] %d\n", Code); }
		void	prn_JobCode(void)		
				{ 
					char imsi[4];
					memset(imsi, 0x00, sizeof(imsi));
					memcpy(imsi, JobCode, sizeof(JobCode));
					printf("[JobCode   ] %s\n", imsi); 
				}
		void	prn_Min(void)		
				{ 
					char imsi[12];
					memset(imsi, 0x00, sizeof(imsi));
					memcpy(imsi, Min, sizeof(Min));
					printf("[Min       ] %s\n", imsi); 
				}
		void	prn_Min2(void)		
				{ 
					char imsi[12];
					memset(imsi, 0x00, sizeof(imsi));
					memcpy(imsi, Min2, sizeof(Min2));
					printf("[Min       ] %s\n", imsi); 
				}				
		void	prn_AccInfo(void)		
				{ 
					char imsi[4];
					memset(imsi, 0x00, sizeof(imsi));
					memcpy(imsi, AccInfo, sizeof(AccInfo));
					printf("[AccInfo   ] %s\n", imsi); 
				}				
		//void	prn_UTransactionID(void){ printf("[UTransacID] %d\n",UTransactionID); }				
								
		void	prn_All(void)
		{
			prn_Code();
			prn_JobCode();
			prn_Min();
			prn_Min2();
			prn_AccInfo();
			//prn_UTransactionID();
		}
		
};
//===============================================================
// CdsCustFormat class											=
//  : CDS I/F와 고객관리간 format							  		=
// argument : None												=
// return value : None											=
// 2000.11.15 : 변경될 가입자번호 추가								=
//===============================================================
class CdsCustFormat // size 44byte
{
	private:
		CdsResFormat		cCdsResFormat;
		CustFormat			cCustFormat;
	
	public:
		void	setItem(const CdsResFormat* stItem)
		{	memcpy((char*)&cCdsResFormat, (char*)stItem, sizeof(CdsResFormat));	}
		void	setItem(const CustFormat* stItem)
		{	memcpy((char*)&cCustFormat, (char*)stItem, sizeof(CustFormat));	}
		CdsResFormat*		get_CdsResFormat(void) 	{	return (&cCdsResFormat);	}
		CustFormat*			get_CustFormat(void) 	{	return (&cCustFormat);		}
		
		CdsCustFormat()
		{
			memset((char*)&cCdsResFormat, 0x00, sizeof(CdsResFormat));
			memset((char*)&cCustFormat, 0x00, sizeof(CustFormat));
		}
};

#endif


