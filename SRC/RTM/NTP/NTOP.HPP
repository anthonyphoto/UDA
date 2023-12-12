#include <time.h>
#include <MsgFormat.hpp>
#include <SendLogManage.hpp>
#include <TempMsgQ_file.hpp>
#include <scpformat.hpp>
#include <stdio.h>

// nTop과금 서버에서 FTP로 보내준 file에서 사용할 data의 offset   
#define POS_BILL_PHON_NUM		0
#define POS_ANS_DT				20
#define POS_ANS_TM				28
#define POS_RELEASE_DT			35
#define POS_RELEASE_TM			43
#define POS_CDR_TYPE			50  
#define POS_PACKET_NOMAL		52	   
#define POS_PACKET_DISCNT		59   
#define POS_PACKET_NIGHT		66   
#define POS_TOT_CHARGE_AMT		73    
       
   
// nTop과금 서버에서 FTP로 보내준 file에서 사용할 data의 길이         
#define	LEN_BILL_PHON_NUM		20   							// Calling or Billing Phone Number  
#define	LEN_ANS_DT				8                      			// 통화 시작 일자 (YYYMMDD)         
#define	LEN_ANS_TM				7                      			// 통화 시작 시간 (HHMMSSM)         
#define	LEN_RELEASE_DT			8                       		// 통화 종료 일자 (YYYMMDD)         
#define	LEN_RELEASE_TM			7                       		// 통화 종료 시간 (HHMMSSM)         
#define LEN_CDR_TYPE			2                               // WIG(IS-95C 저속) : "01"                            
																// WIG(IS-95C 고속) : "02"				
#define	LEN_PACKET_NOMAL		7                               // 비할인 Packet수 : "0000005"                               
#define	LEN_PACKET_DISCNT		7                               // 할인 Packet수 	: "0000005"                              
#define	LEN_PACKET_NIGHT		7                       		// 심야 Packet수	: "0000005"                        
#define	LEN_TOT_CHARGE_AMT		10								// 최종 통화료 과금액  


// nTop과금 서버에서 FTP로 보내준 file length       
#define LEN_INPUT_FILE			84
#define MAXLEN					81


// 비정상 종료됐을 경우를 대비해서 nTop과금 data를 SCP로 보낸후 저장             
class	nTopFileConfig
{
	private : 
		char	strNTopFileName1[MAXLEN];
		char	strNTopFileName2[MAXLEN];
		int		nUdrCount;
		
	public : 
		nTopFileConfig()	{ ; }
		~nTopFileConfig()	{ ; }
		
		char*	get_strNTopFileName1()				{	return	strNTopFileName1;	}	
		char*	get_strNTopFileName2()				{	return	strNTopFileName2;	}	
		int		get_nUdrCount()						{	return	nUdrCount;			}		

		void	put_strNTopFileName1(char* strValue){	memcpy(strNTopFileName1, strValue, sizeof(strNTopFileName1));}
		void	put_strNTopFileName2(char* strValue){	memcpy(strNTopFileName2, strValue, sizeof(strNTopFileName2));}
		void	put_nUdrCount(int nValue)			{	nUdrCount = nValue;			}
	
		void	read_nTopConfigFile();
		void	write_nTopConfigFile();


};		
		

// nTop과금 서버에서 FTP로 보내준 file format                     
class	nTop_FTP
{
	private : 
		nTopFileConfig	clsNTopConfig;
		char	strBill_Phone_Num[LEN_BILL_PHON_NUM + 1];		// Calling or Billing Phone Number
		char	strAns_Dt[LEN_ANS_DT + 1];						// 통화 시작 일자 (YYYYMMDD)   
		char	strAns_Tm[LEN_ANS_TM + 1];						// 통화 시작 시간 (HHMMSSM)   
		char	strRelease_Dt[LEN_RELEASE_DT + 1];				// 통화 종료 일자 (YYYYMMDD)       
		char	strRelease_tm[LEN_RELEASE_TM + 1];				// 통화 종료 시간 (HHMMSSM) 
		char	strCdr_Type[LEN_CDR_TYPE + 1]; 					// WIG(IS-95C 저속) : "01"                            
																// WIG(IS-95C 고속) : "02"			
		char	strPacket_Normal[LEN_PACKET_NOMAL + 1];			// 비할인 Packet수 : "0000005"
		char	strPacket_Discnt[LEN_PACKET_DISCNT + 1];		// 할인 Packet수 	: "0000005"
		char	strPacket_Night[LEN_PACKET_NIGHT + 1];			// 심야 Packet수	: "0000005"
		char	strTot_Charge_Amt[LEN_TOT_CHARGE_AMT + 1];		// 최종 통화료 과금액: "0000300"  
		
	public : 
		nTop_FTP()		{ ; }
		~nTop_FTP()		{ ; }
		
		char*	get_strBill_Phone_Num()					{	return	strBill_Phone_Num;			}
		char*	get_strAns_Dt()							{	return	strAns_Dt;					}
		char*	get_strAns_Tm()							{	return	strAns_Tm;					}
		char*	get_strRelease_Dt()						{	return	strRelease_Dt;				}
		char*	get_strRelease_tm()						{	return	strRelease_tm;				}
		char*	get_strCdr_Type()						{	return	strCdr_Type;				}
		char*	get_strPacket_Normal()					{	return	strPacket_Normal;			}
		char*	get_strPacket_Discnt()					{	return	strPacket_Discnt;			}
		char*	get_strPacket_Night()					{	return	strPacket_Night;			}
		char*	get_strTot_Charge_Amt()					{	return	strTot_Charge_Amt;			}
		
		void	put_strBill_Phone_Num(char* strValue)	{	memcpy(strBill_Phone_Num, strValue, LEN_BILL_PHON_NUM);	}
		void	put_strAns_Dt(char* strValue)			{	memcpy(strAns_Dt, strValue, LEN_ANS_DT);				}
		void	put_strAns_Tm(char* strValue)			{	memcpy(strAns_Tm, strValue, LEN_ANS_TM);				}
		void	put_strRelease_Dt(char* strValue)		{	memcpy(strRelease_Dt, strValue, LEN_RELEASE_DT);		}
		void	put_strRelease_tm(char* strValue)		{	memcpy(strRelease_tm, strValue, LEN_RELEASE_TM);		}
		void	put_strCdr_Type(char* strValue)			{	memcpy(strCdr_Type, strValue, LEN_CDR_TYPE);			}
		void	put_strPacket_Normal(char* strValue)	{	memcpy(strPacket_Normal, strValue, LEN_PACKET_NOMAL);	}
		void	put_strPacket_Discnt(char* strValue)	{	memcpy(strPacket_Discnt, strValue, LEN_PACKET_DISCNT);	}
		void	put_strPacket_Night(char* strValue)		{	memcpy(strPacket_Night, strValue, LEN_PACKET_NIGHT);	}
		void	put_strTot_Charge_Amt(char* strValue)	{	memcpy(strTot_Charge_Amt, strValue, LEN_TOT_CHARGE_AMT);}
		
		void	SendNTopFile(int nRecCount);
		void	UseTime();
		int		MoveFile(char* strMovePath);

		char*	get_strNTopFileName1()					{	return	clsNTopConfig.get_strNTopFileName1();	}	
		char*	get_strNTopFileName2()					{	return	clsNTopConfig.get_strNTopFileName2();	}	
		int		get_nUdrCount()							{	return	clsNTopConfig.get_nUdrCount();			}		
		void	put_strNTopFileName1(char* strValue)	{	clsNTopConfig.put_strNTopFileName1(strValue);	}
		void	put_strNTopFileName2(char* strValue)	{	clsNTopConfig.put_strNTopFileName2(strValue);	}
		void	put_nUdrCount(int nValue)				{	clsNTopConfig.put_nUdrCount(nValue);			}

		void	read_nTopConfigFile()					{	clsNTopConfig.read_nTopConfigFile();			}
		void	write_nTopConfigFile()					{	clsNTopConfig.write_nTopConfigFile();			}

};	



// nTop과금 서버에서 FTP로 보내준 file의 data를 저장    
typedef struct XString
{
	char		strXString[LEN_INPUT_FILE];
}	STRINGBUF;	




