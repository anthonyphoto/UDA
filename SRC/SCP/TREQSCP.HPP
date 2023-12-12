#ifndef _TREQSCP_HPP
#define _TREQSCP_HPP

#include <iostream.h>
#include <iomanip.h>
#include <string.h>
#include <stdlib.h>
#include <strstream.h>
#include <SqlCommon.hpp>
#include <scpformat.hpp>

/*
//--------------------------------------------------------------------
class TReqScpRec
{
	private:
		char			SysDate[8+1];		
		unsigned long 	nTid;				
		int				nCode;				
		char			Msid[15+1];			
		unsigned long	nOper_Id;			
		char			Account_Id[8+1];
		char			Correl_Id[8+1];
		unsigned long	nStart_Time;
		unsigned long	nEnd_Time;
		unsigned long	nLimit_Money;
		unsigned long	nTop_Use;
		
	public:	
		TReqScpRec(void)
		{
			memset(SysDate, 0x00, sizeof(SysDate);
			nTid=0;			
			nCode=0;			
			memset(Msid, 0x00, sizeof(Msid));
			nOper_Id=0;		
			memset(Account_Id, 0x00, sizeof(Account_Id));    
			memset(Correl_Id, 0x00, sizeof(Correl_Id));    
			nStart_Time=0;    
			nEnd_Time=0;      
			nLimit_Money=0;   
			nTop_Use=0;       
		}   
		
		~TReqScpRec(void){}
		
		
		char*			get_SysDate() 	{ return SysDate; }
		unsigned long 	get_nTid() 		{ return nTid; }
		int				get_nCode() 	{ return nCode; }
		char*			get_Msid()		{ return Msid; }
		unsigned long 	get_nOper_Id()	{ return nOper_Id;}
		char*			get_Account_Id()  { return nAccount_Id; }
		char*			get_Correl_Id()   { return Correl_Id; }
		unsigned long	get_nStart_Time() { return nStart_Time; }
		unsigned long	get_nEnd_Time()	  { return nEnd_Time; }
		unsigned long  	get_nLimit_Money(){ return nLimit_Money; }
		unsigned long 	get_nTop_Use()	  { return nTop_Use; }
		
		void 	put_SysDate(char *str) 			 { memcpy(SysDate, str, 8); }
		void 	put_nTid(unsigned long n) 		 { nTid=n;}
		void 	put_nCode(int n) 				 { nCode=n;}
		void 	put_Msid(char *str)				 { memcpy(Msid, str, 15);} 
		void	put_nOper_Id(unsigned long n)	 { nOper_Id=n; }
		void 	put_Account_Id(char *str)    	 { memcpy(Account_Id, str, 8); }
		void	put_Correl_Id(char *str)    	 { memcpy(Correl_Id, str, 8); }
		void	put_nStart_Time(unsigned long n) { nStart_Time=n;} 
		void	put_nEnd_Time(unsigned long n)	 { nEnd_Time=n; }
		void	put_nLimit_Money(unsigned long n){nLimit_Money=n;}
		void	put_nTop_Use(unsigned long n)	 {nTop_Use=n;}
};
*/


//--------------------------------------------------------------------
/*(������)
  1. ���ó�¥�� �����Ͱ� ������ ���ó�¥�� TID=1�� �����Ѵ�.
  2. ���ó�¥�� �����Ͱ� ������ ���ó�¥�� TID++�� �����Ѵ�.
  
  (�б���)
  1. SCP�� Sä�ηκ��� ���� �ʱ� DAY, TID�� ���� �ش緹�ڵ带 �˻��Ѵ�.
  2. ���ڵ带 �˻��� �����ϸ� Day, Tid+1 �ڷḦ �����Ѵ�.
     (������ �������� Tid=1�� �ִ��� ���� ������ �����Ѵ�.)
  3. ���ڵ� �˻��� �����ϸ� ���ó�¥ Tid=1�� �߻��Ҷ����� ����Ѵ�.
------------------------------------------------------------------------*/
  
class TReqScp
{
	private:
		int			DBerror;
		DbConnect 	clsDbConnect;
		//TReqScpRec	LogRecord;			// T_Req_Scp ���̺� ���ڵ�
		
		
		int			Get_MaxTid(void);	// ���ó�¥�� ����ū TID�� ��´�.
		
	public:
        
        TReqScp();
		~TReqScp();
										
					// AGG���� ��û���� ����(MQ)�� DB���߰��Ѵ�. 0: �߰�����
		int			Add_ScpRecord(SCPFormatAgg Aggrec);	
					// SCP-ACK���� ���� TID�� �޾� DB���� �о�´�. 0: �б����
		int			Get_ScpRecord(SCPFormat *Scprec, char *day, unsigned long n);	
		
		char*			Get_Day(void);	
		unsigned long	Get_Tid(void);
};



#endif
