#ifndef	UDR_HPP
#define	UDR_HPP
#include <fstream.h>
#include <iostream.h>
#include <stdlib.h>

const int	LEN_UDR_ITEM							=  3;
const int	LEN_UDR_PARAM							= 60;
const int	LEN_UDR_PATTERN							= 22;

const int	CNT_UDR_ITEM							= 57;

const char	TAG_TYPE_START[]						= "<";
const char	TAG_TYPE_END[]							= "</";

//	Associated with UDR Format
const int 	LEN_MSID								= 15;
const int 	LEN_NAI									= 64;
const int 	LEN_ACCOUNT_SESSION_ID					=  8;
const int	LEN_CORRELATION_ID						=  8;
const int 	LEN_CONTAINER							= 16;
const int 	LEN_BS_MSC_ID							= 12;

//	Associated with BDS UDR Format
const int	LEN_BDS_MS_ID                       	= 15;
const int	LEN_BDS_IP_ADDR                         = 12;
const int	LEN_BDS_NAI                             = 32;
const int	LEN_BDS_ACCOUNT_SESSION_ID              =  8;
const int	LEN_BDS_CORRELATION_ID                  =  8;
const int	LEN_BDS_MIP_HOME_AGENT                  = 12;
const int	LEN_BDS_PDSN_FA_ADDR                    = 12;
const int	LEN_BDS_SERVING_PCF                     = 12;
const int	LEN_BDS_BS_MSC_ID                       = 12;
const int	LEN_BDS_USER_ZONE                       =  5;
const int	LEN_BDS_FORWARD_MUX_OPT                 =  5;
const int	LEN_BDS_REVERSE_MUX_OPT                 =  5;
const int	LEN_BDS_FORWARD_FUND_RATE               =  3;
const int	LEN_BDS_REVERSE_FUND_RATE               =  3;
const int	LEN_BDS_SERVICE_OPT                     =  2;
const int	LEN_BDS_SERVICE_CORRELATION             =  2;	//	Add 9. 20
const int	LEN_BDS_FORWARD_TRAFFIC_TYPE            =  1;
const int	LEN_BDS_REVERSE_TRAFFIC_TYPE            =  1;
const int	LEN_BDS_FUND_FRAME_SIZE                 =  2;
const int	LEN_BDS_FORWARD_FUNDAMENTAL_RC          =  2;
const int	LEN_BDS_REVERSE_FUNDAMENTAL_RC          =  2;
const int	LEN_BDS_IP_TECHNOLOGY                   =  1;
const int	LEN_BDS_COMPULSORY_TUNNEL_IND           =  1;
const int	LEN_BDS_RELEASE_INDICATOR               =  1;
const int	LEN_BDS_DATA_OCTET_CNT_TERMINAT         = 10;
const int	LEN_BDS_DATA_OCTET_CNT_ORIGIN           = 10;
const int	LEN_BDS_BAD_PPP_FRAME_CNT               = 10;
const int	LEN_BDS_START_EVENT_TIME                = 15;
const int	LEN_BDS_END_EVENT_TIME                  = 15;
const int	LEN_BDS_ACTIVE_TIME                     = 10;	//	Modified 15 -> 7 (9. 20)
const int	LEN_BDS_NUMOFACTIVE_TRANSITIONS         = 10;
const int	LEN_BDS_SDB_OCTET_COUNT_TERMINAT        = 10;
const int	LEN_BDS_SDB_OCTET_COUNT_ORIGIN          = 10;
const int	LEN_BDS_NUMOFSDBS_TERMINAT              = 10;
const int	LEN_BDS_NUMOFSDBS_ORIGIN                = 10;
const int	LEN_BDS_NUMOFPPP_BYTES_RECEIVED         = 10;
const int	LEN_BDS_ALTERNATE_BILLING_ID            = 10;
const int	LEN_BDS_IP_QOS                          =  5;
const int	LEN_BDS_INTERCONNECTIP_NETWORKPROVIDERID= 12;
const int	LEN_BDS_INTERCONNECTIP_NETWORKSERVICEQOS= 10;
const int	LEN_BDS_AIRLINK_QOS                     = 10;
//const int	LEN_BDS_AIRLINK_REC_TYPE                =  0
//const int	LEN_BDS_RP_SESSION_ID                   =  0
//const int	LEN_BDS_AIRLINK_SEQ_NUM                 =  0
//const int	LEN_BDS_MOBILE_ORIGIN_TERMINAT_ID       =  0
//const int	LEN_BDS_Container                       =  0



enum UDR_ETYPE		{START, INTERIM, STOP};

//	UDR STATUS Type
enum STATUS_ETYPE	{PROCESSING, SENT};

//	Subs Type
enum SUBS_ETYPE		{NORMAL, PPS, TTL_S};

//	Service Type
enum SVC_ETYPE		{PACKET, NTOP};

enum TYPE			{TAG_TYPE, STR_TYPE, INT_TYPE};
enum ADJUST			{NONE, RIGHT, LEFT};

typedef struct	__UDR_ITEM
{
	char	strItem[LEN_UDR_ITEM+1];
	char	strParam[LEN_UDR_PARAM+1];
	char	strPattern[LEN_UDR_PATTERN+1];
	TYPE	nType;
	int		nLength;
	int 	nPosition;
	int     nEndPosition;
	int		nStringLen;
	int		nStringOffset;
	ADJUST	nAdjustment;
}	STUDRITEM, *STUDRITEMPTR;


static STUDRITEM	stConfigUdr[CNT_UDR_ITEM]= \
{	{"A"  , "Mobile Identifiers",									"M_ID>", 		TAG_TYPE,  0, 0,   0,   0, 0  , NONE},
	{"A1" , "MS ID", 												"MSID>", 		STR_TYPE, 15, 40,  54,  15, 0  , LEFT},
	{"B"  , "User Identifiers",										"U_ID>", 		TAG_TYPE,  0, 0,   0,   0 , 0  , NONE},
	{"B1" , "IP Address", 											"IP_ADDR>",		INT_TYPE,  4, 84,  98,  12, 15 , LEFT},
	{"B2" , "Network Access Identifier ", 							"NAI>", 		STR_TYPE, 64, 114, 177, 32, 27 , LEFT},
	{"C"  , "Session Identifiers",									"S_ID>", 		TAG_TYPE,  0, 0,   0,   0 , 0  , NONE},
	{"C1" , "Account Session ID", 									"A_S_ID>",		STR_TYPE,  8, 205, 212, 8 , 59 , LEFT},
	{"C2" , "Correlation ID", 										"C_ID>",		STR_TYPE,  8, 228, 235, 8 , 67 , LEFT},
	{"D"  , "Infrastructure Identifiers",							"I_ID>", 		TAG_TYPE,  0, 0,   0,	0 , 0  , NONE},
	{"D1" , "MIP Home Agent", 										"HA>",			INT_TYPE,  4, 260, 274, 12, 75 , LEFT},
	{"D2" , "PDSN/FA Address", 										"PDSN_FA>",		INT_TYPE,  4, 289, 303, 12, 87 , LEFT},
	{"D3" , "Serving PCF", 											"PCF>",			INT_TYPE,  4, 319, 333, 12, 99 , LEFT},
	{"D4" , "BS / MSC ID", 											"BS_MSC_ID>",	STR_TYPE, 12, 351, 362, 12, 111, NONE},
	{"E"  , "Zone Identifiers",										"Z_ID>", 		TAG_TYPE,  0, 0,   0,   0 , 0  , NONE},
	{"E1" , "User Zone", 											"U_Z>",			INT_TYPE,  4, 393, 402, 5 , 123, RIGHT},
	{"F"  , "Session Status",										"S_S>", 		TAG_TYPE,  0, 0,   0,   0 , 0  , NONE},
	{"F1" , "Forward MUX Option", 									"F_M_O>",		INT_TYPE,  4, 428, 437, 5 , 128, RIGHT},
	{"F2" , "Reverse MUX Option", 									"R_M_O>",		INT_TYPE,  4, 453, 462, 5 , 133, RIGHT},
	{"F3" , "Forward Fundamental Rate", 							"F_F_R>",		INT_TYPE,  4, 478, 487, 3 , 138, NONE},
	{"F4" , "Reverse Fundamental Rate",		 						"R_F_R>",		INT_TYPE,  4, 503, 512, 3 , 141, NONE},
	{"F5" , "Service Option", 										"S_O>",			INT_TYPE,  4, 526, 535, 2 , 144, RIGHT},
	{"F51", "Service Correlation", 									"S_O>",			INT_TYPE,  4, 0,   0,   2 , 146, NONE},
	{"F6" , "Forward Traffic Type", 								"F_T_T>",		INT_TYPE,  4, 549, 558, 1 , 148, NONE},
	{"F7" , "Reverse Traffic Type (Primary, Secondary)",	 		"R_T_T>",		INT_TYPE,  4, 574, 583, 1 , 149, NONE},
	{"F8" , "Fundamental Frame Size", 								"F_F_S>",		INT_TYPE,  4, 599, 608, 2 , 150, RIGHT},
	{"F9" , "Forward Fundamental RC", 								"F_F_RC>",		INT_TYPE,  4, 625, 634, 2 , 152, NONE},
	{"F10", "Reverse Fundamental RC", 								"R_F_RC>",		INT_TYPE,  4, 652, 661, 2 , 154, NONE},
	{"F11", "IP Technology", 										"IP_T>",		INT_TYPE,  4, 677, 686, 1 , 156, NONE},
	{"F12", "Compulsory Tunnel Indicator", 							"C_T_I>",		INT_TYPE,  4, 701, 710, 1 , 157, NONE},
	{"F13", "Release Indicator", 									"R_I>",			INT_TYPE,  4, 724, 733, 1 , 158, NONE},
	{"G"  , "Session Activity",										"S_A>", 		TAG_TYPE,  0, 0,   0,   0 , 0  , NONE},
	{"G1" , "Data Octet Count (Terminating)", 						"D_O_C_T>",		INT_TYPE,  4, 760, 769, 10, 159, RIGHT},
	{"G2" , "Data Octet Count (Originating)", 						"D_O_C_O>",		INT_TYPE,  4, 789, 798, 10, 169, RIGHT},
	{"G3" , "Bad PPP frame Count", 									"PPP_F_C>",		INT_TYPE,  4, 818, 827, 10, 179, RIGHT},
	{"G4" , "Start Event Time", 									"E_T>",			INT_TYPE,  4, 843, 852, 15, 189, LEFT},
	{"G5" , "End Event Time", 										"E_T>",			INT_TYPE,  4, 0,   0,   15, 204, LEFT},
	{"G8" , "Active Time", 											"A_T>",			INT_TYPE,  4, 864, 873, 10, 219, LEFT},
	{"G9" , "Number of Active Transitions", 						"N_A_T>",		INT_TYPE,  4, 887, 896, 10, 229, RIGHT},
	{"G10", "SDB Octet Count (Terminating)", 						"SDB_I_C_T>",	INT_TYPE,  4, 916, 925, 10, 239, RIGHT},
	{"G11", "SDB Octet Count (Originating)", 						"SDB_I_C_O>",	INT_TYPE,  4, 949, 958, 10, 249, RIGHT},
	{"G12", "Number of SDBs (Terminating)", 						"N_SDB_T>",		INT_TYPE,  4, 980, 989, 10, 259, RIGHT},
	{"G13", "Number of SDBs (Originating)", 						"N_SDB_O>",		INT_TYPE,  4, 1009,1018,10, 269, RIGHT},
	{"G14", "Number of PPP bytes received", 						"N_PPP_R>",		INT_TYPE,  4, 1038,1047,10, 279, RIGHT},
	{"H"  , "Special Billing Instructions",							"S_B_I>", 		TAG_TYPE,  0, 0,   0,   0 , 0  , NONE},
	{"H1" , "Alternate Billing Identifier", 						"A_B_ID>",		INT_TYPE,  4, 1079,1088,10, 289, RIGHT},
	{"I"  , "Quality of Service",									"QOS>", 		TAG_TYPE,  0, 0,   0,   0 , 0  , NONE},
	{"I1" , "IP Quality of Service (QoS)", 							"IP_QOS>",		INT_TYPE,  4, 1119,1128,5 , 299, RIGHT},
	{"I2" , "Interconnection IP Network Provider ID", 				"IP_N_P_ID>",	INT_TYPE,  4, 1149,1163,12, 304, LEFT},
	{"I3" , "Interconnecting IP Network Service Quality of Service","IP_N_S_QOS>",	INT_TYPE,  4, 1188,1196,10, 316, RIGHT},
	{"I4" , "Airlink Quality of Service (QoS)", 					"A_QOS>",		INT_TYPE,  4, 1218,1226,10, 326, RIGHT},
	{"Y"  , "Airlink Record Specific Parameters",					"A_R_S_P>",		TAG_TYPE,  0,  0,  0,   0,   0, NONE},
	{"Y1" , "Airlink Record Type", 									"A_R_T>",		INT_TYPE,  4,  0,  0,   0,   0, NONE},
	{"Y2" , "R-P Session ID", 										"RP_S_ID>",		INT_TYPE,  4,  0,  0,   0,   0, NONE},
	{"Y3" , "Airlink Sequence Number", 								"A_S_N>",		INT_TYPE,  4,  0,  0,   0,   0, NONE},
	{"Y4" , "Mobile Originated / Mobile Terminated Indicator", 		"M_O_T_ID>",	INT_TYPE,  4,  0,  0,   0,   0, NONE},
	{"Z"  , "Container",											"C>",			TAG_TYPE,  0,  0,  0,   0,   0, NONE},
	{"Z1" , "Container", 											"CT>",			STR_TYPE, 16,  0,  0,   0, 336, NONE}
};

/*
typedef struct	__MIN_INFO
{
	int		nServiceProvider;
	int		nMin1;
	int		nMin2;
}MININFO_TYPE, *MININFO_PTR_TYPE;

typedef struct	__SUBS_INFO
{
	short	nServiceType;	//	Packet or nTop
	short	nSubsType;		//	PPS or TTL_School
}SUBS_TYPE, *SUBS_PTR_TYPE;

typedef struct	__UDR_HEADER
{
	int				nUdrType;
	SUBS_TYPE		nSubsType;
	int				nUdrSeqNo;
	int				nUdrStatus;
	MININFO_TYPE	stMinInfo;
}UDRHEAD_TYPE, *UDRHEAD_PTR_TYPE;

typedef struct	__MobileIdenfier
{
	char	strMsId			[LEN_MSID+1];
}A_TYPE, *A_PTR_TYPE;

typedef struct	__STR_MobileIdenfier
{
	char	strMsId			[LEN_MSID+1];
}A_BDS_TYPE, *A_PTR_BDS_TYPE;


typedef struct	__INT_MobileIdenfier
{
	unsigned int	unMsId;
}A_INT_TYPE, *A_PTR_INT_TYPE;

typedef struct	__UserIdenfier
{
	unsigned int	nIpAddr;
	char			strNAI		[LEN_NAI+1];
}B_TYPE, *B_PTR_TYPE;

typedef struct	__SessionIdenfier
{
	char	strAccountSessionID	[LEN_ACCOUNT_SESSION_ID+1];
	char	strCorrelationID	[LEN_CORRELATION_ID+1];
}C_TYPE, *C_PTR_TYPE;

typedef struct	__InfraIdenfier
{
	unsigned int	nMIP_HomeAgent;
	unsigned int	nPDSN_FA_Addr;
	unsigned int	nServing_PCF;
	char			strBS_MSC_ID[LEN_BS_MSC_ID+1];
}D_TYPE, *D_PTR_TYPE;

typedef struct	__ZoneIdenfier
{
	int		nUserZone;
}E_TYPE, *E_PTR_TYPE;

typedef struct	__SessionStatus
{
	int		nForwardMUXOpt;
	int		nReverseMUXOpt;
	int		nForward_Fund_Rate;
	int		nReverse_Fund_Rate;
	int		nServiceOpt;
	int		nForwardTrafficType;
	int		nReverseTrafficType;
	int		nFundFrameSize;
	int		nForwardFundamentalRC;
	int		nReverseFundamentalRC;
	int		nIP_Technology;
	int		nCompulsoryTunnelInd;
	int		nReleaseIndicator;
}F_TYPE, *F_PTR_TYPE;

typedef struct	__SessionActivity
{
	int		nDataOctetCntTerminat;
	int		nDataOctetCntOrigin;
	int		nBadPPPframeCnt;
	int		nStartEventTime;
	int		nEndEventTime;
	int		nActiveTime;
	int		nNumOfActiveTransitions;
	int		nSDB_OctetCountTerminat;
	int		nSDB_OctetCountOrigin;
	int		nNumOfSDBsTerminat;
	int		nNumOfSDBsOrigin;
	int		nNumOfPPPbytesReceived;
}G_TYPE, *G_PTR_TYPE;

typedef struct	__SpecialBillingInstruction
{
	int		nAlternateBillingId;
}H_TYPE, *H_PTR_TYPE;

typedef struct	__QoS
{
	int				nIP_QoS;
	unsigned int	nInterconnectIP_NetworkProviderID;
	int				nInterconnectIP_NetworkServiceQoS;
	int				nAirlinkQoS;
}I_TYPE, *I_PTR_TYPE;

typedef struct	__AirlinkRecSpecParam
{
	int		nAirlinkRecType;
	int		nRP_SessionID;
	int		nAirlinkSeqNum;
	int		nMobileOrigin_TerminatID;
}Y_TYPE, *Y_PTR_TYPE;

typedef struct	__Container
{
	char	strContainer		[LEN_CONTAINER];
}Z_TYPE, *Z_PTR_TYPE;

*/

class UDRHEAD_TYPE
{
	private:
		int				nUdrType;
		unsigned int	nUdrFileSeq;
		int				nUdrSeqNo;
		int				nUdrStatus;
		//	__MIN_INFO
		int				nServiceProvider;
		int				nMin1;
		int				nMin2;
		//	__SUBS_INFO
		short			nServiceType;	//	Packet or nTop
		short			nSubsType;		//	PPS or TTL_School
		//	__Radius Info
		unsigned int processType;	
		unsigned int processSN;	
		unsigned int processRemark;

	public:
		//	Accessor
		int				get_nUdrType()						{	return nUdrType;		}
		unsigned int	get_nUdrFileSeq()					{	return nUdrFileSeq;		}
		int				get_nUdrSeqNo()						{	return nUdrSeqNo;		}
		int				get_nUdrStatus()					{	return nUdrStatus;		}
		int				get_nServiceProvider()				{	return nServiceProvider;}
		int				get_nMin1()							{	return nMin1;			}
		int				get_nMin2()							{	return nMin2;			}
		short			get_nServiceType()					{	return nServiceType;	}
		short			get_nSubsType()						{	return nSubsType;		}
		unsigned int	get_processType()					{	return processType;		}
		unsigned int	get_processSN()						{	return processSN;		}
		unsigned int	get_processRemark()					{	return processRemark;	}
	
		void			put_nUdrType(int nValue)			{	nUdrType = nValue;			}
		void			put_nUdrFileSeq(unsigned int nValue){	nUdrFileSeq = nValue;		}
		void			put_nUdrSeqNo(int nValue)			{	nUdrSeqNo = nValue;			}
		void			put_nUdrStatus(int nValue)			{	nUdrStatus = nValue;		}
		void			put_nServiceProvider(int nValue)	{	nServiceProvider = nValue;	}
		void			put_nMin1(int nValue)				{	nMin1 = nValue;				}
		void			put_nMin2(int nValue)				{	nMin2 = nValue;				}
		void			put_nServiceType(int nValue)		{	nServiceType=(short)nValue;	}
		void			put_nSubsType(int nValue)			{	nSubsType = (short)nValue;	}
		void			put_processType(unsigned int nValue){	processType = nValue;		}
		void			put_processSN(unsigned int nValue)	{	processSN = nValue;			}
		void			put_processRemark(unsigned int nValue){	processRemark = nValue;		}
		void 			put_MinInfo(char *strMin)
						{
							char strTmp[4];
							if(strMin[0]=='0')
							{
								// set service provider
								memset(strTmp,0x00,4);
								memcpy(strTmp,strMin,3);
								put_nServiceProvider(atoi(strTmp));
								// set min1
								memset(strTmp,0x00,4);
								memcpy(strTmp,&strMin[3],3);
								put_nMin1(atoi(strTmp));
								// set min2
								memset(strTmp,0x00,4);
								memcpy(strTmp,&strMin[6],4);
								put_nMin2(atoi(strTmp));
								
							}else
							{
								// set service provider
								memset(strTmp,0x00,4);
								memcpy(strTmp,strMin,2);
								put_nServiceProvider(atoi(strTmp));
								// set min1
								memset(strTmp,0x00,4);
								memcpy(strTmp,&strMin[2],4);
								put_nMin1(atoi(strTmp));
								// set min2
								memset(strTmp,0x00,4);
								memcpy(strTmp,&strMin[6],4);
								put_nMin2(atoi(strTmp));
							}
						}
};


class A_TYPE
{
	private:
		char	strMsId			[LEN_MSID+1];
	public:
		A_TYPE()									{	memset(strMsId, NULL, LEN_MSID+1);	}
		void	put_strMsId(const char* strValue)	{	memcpy(strMsId, strValue, LEN_MSID);}
		char*	get_strMsId()						{	return strMsId;	}
};

class A_INT_TYPE
{
	private:
		unsigned int	unMsId;
	public:
		void			put_unMsId(unsigned int nValue)	{	unMsId = nValue;}
		unsigned int	get_unMsId()					{	return unMsId;	}
};

class A_BDS_TYPE
{
	private:
		char	strMsId			[LEN_MSID+1];
	public:
		A_BDS_TYPE()								{	memset(strMsId, NULL, LEN_MSID+1);	}
		void	put_strMsId(const char* strValue)	{	memcpy(strMsId, strValue, LEN_MSID);}
		char*	get_strMsId()						{	return strMsId;	}
};

class B_TYPE
{
	private:
		unsigned int	nIpAddr;
		char			strNAI		[LEN_NAI+1];
	public:
		B_TYPE()										{	memset(strNAI, NULL, LEN_NAI+1);	}
		void			put_nIpAddr(unsigned int nValue){	nIpAddr = nValue;     }
		void			put_strNAI(const char* strValue){	memcpy(strNAI, strValue, LEN_NAI);}
		unsigned int	get_nIpAddr()					{	return nIpAddr;		}
		char*			get_strNAI()					{	return strNAI;		}
};

class C_TYPE
{		
	private:
		char	strAccountSessionID	[LEN_ACCOUNT_SESSION_ID+1];
		char	strCorrelationID	[LEN_CORRELATION_ID+1];
	public:
		C_TYPE()
		{
			memset(strAccountSessionID, NULL, LEN_ACCOUNT_SESSION_ID+1);
			memset(strCorrelationID,	NULL, LEN_CORRELATION_ID+1);
		}
			
		void	put_strAccountSessionID(const char* strValue)
		{	memcpy(strAccountSessionID,	strValue, LEN_ACCOUNT_SESSION_ID);	}
		void	put_strCorrelationID(const char* strValue)
		{	memcpy(strCorrelationID, 	strValue, LEN_CORRELATION_ID);}
		char*	get_strAccountSessionID()	{	return strAccountSessionID;	}
		char*	get_strCorrelationID()		{	return strCorrelationID;	}
};

class D_TYPE
{
	private:
		unsigned int	nMIP_HomeAgent;
		unsigned int	nPDSN_FA_Addr;
		unsigned int	nServing_PCF;
		char			strBS_MSC_ID[LEN_BS_MSC_ID+1];
	public:
		D_TYPE()								{	memset(strBS_MSC_ID, NULL, LEN_BS_MSC_ID+1);	}
		unsigned int	get_nMIP_HomeAgent()	{	return nMIP_HomeAgent;	}
		unsigned int	get_nPDSN_FA_Addr()		{	return nPDSN_FA_Addr;	}
		unsigned int	get_nServing_PCF()		{	return nServing_PCF;	}
		char*			get_strBS_MSC_ID()		{	return strBS_MSC_ID;	}
		
		void			put_nMIP_HomeAgent(unsigned int nValue)	{	nMIP_HomeAgent = nValue;}
		void			put_nPDSN_FA_Addr(unsigned int nValue)	{	nPDSN_FA_Addr = nValue;	}
		void			put_nServing_PCF(unsigned int nValue)	{	nServing_PCF = nValue;	}
		void			put_strBS_MSC_ID(const char* strValue)
		{	memcpy(strBS_MSC_ID, 	strValue, LEN_BS_MSC_ID);}	
};

class E_TYPE
{
	private:
		int		nUserZone;
	public:
		void	put_nUserZone(int nValue)	{	nUserZone = nValue;	}
		int		get_nUserZone()				{	return nUserZone;	}
};

class F_TYPE
{
	private:
		int		nForwardMUXOpt;
		int		nReverseMUXOpt;
		int		nForward_Fund_Rate;
		int		nReverse_Fund_Rate;
		int		nServiceOpt;
		int		nServiceCorrelation;
		int		nForwardTrafficType;
		int		nReverseTrafficType;
		int		nFundFrameSize;
		int		nForwardFundamentalRC;
		int		nReverseFundamentalRC;
		int		nIP_Technology;
		int		nCompulsoryTunnelInd;
		int		nReleaseIndicator;
	public:
		int		get_nForwardMUXOpt()		{	return nForwardMUXOpt;			}
		int		get_nReverseMUXOpt()		{	return nReverseMUXOpt;			}
		int		get_nForward_Fund_Rate()	{	return nForward_Fund_Rate;		}
		int		get_nReverse_Fund_Rate()	{	return nReverse_Fund_Rate;		}
		int		get_nServiceOpt()			{	return nServiceOpt;				}
		int		get_nServiceCorrelation()	{	return nServiceCorrelation;		}
		int		get_nForwardTrafficType()	{	return nForwardTrafficType;		}
		int		get_nReverseTrafficType()	{	return nReverseTrafficType;		}
		int		get_nFundFrameSize()		{	return nFundFrameSize;			}
		int		get_nForwardFundamentalRC()	{	return nForwardFundamentalRC;	}
		int		get_nReverseFundamentalRC()	{	return nReverseFundamentalRC;	}
		int		get_nIP_Technology()		{	return nIP_Technology;			}
		int		get_nCompulsoryTunnelInd()	{	return nCompulsoryTunnelInd;	}
		int		get_nReleaseIndicator()		{	return nReleaseIndicator;		}
		
		void	put_nForwardMUXOpt(int nValue)			{	nForwardMUXOpt 		= nValue;	}
		void	put_nReverseMUXOpt(int nValue)			{	nReverseMUXOpt 		= nValue;	}
		void	put_nForward_Fund_Rate(int nValue)		{	nForward_Fund_Rate 	= nValue;	}
		void	put_nReverse_Fund_Rate(int nValue)		{	nReverse_Fund_Rate 	= nValue;	}
		void	put_nServiceOpt(int nValue)				{	nServiceOpt 		= nValue;	}
		void	put_nServiceCorrelation(int nValue)		{	nServiceCorrelation	= nValue;	}
		void	put_nForwardTrafficType(int nValue)		{	nForwardTrafficType = nValue;	}
		void	put_nReverseTrafficType(int nValue)		{	nReverseTrafficType = nValue;	}
		void	put_nFundFrameSize(int nValue)			{	nFundFrameSize 		= nValue;	}
		void	put_nForwardFundamentalRC(int nValue)	{	nForwardFundamentalRC = nValue;	}
		void	put_nReverseFundamentalRC(int nValue)	{	nReverseFundamentalRC = nValue;	}
		void	put_nIP_Technology(int nValue)			{	nIP_Technology 		= nValue;	}
		void	put_nCompulsoryTunnelInd(int nValue)	{	nCompulsoryTunnelInd = nValue;	}
		void	put_nReleaseIndicator(int nValue)		{	nReleaseIndicator 	= nValue;	}
};

class G_TYPE
{
	private:
		int		nDataOctetCntTerminat;
		int		nDataOctetCntOrigin;
		int		nBadPPPframeCnt;
		int		nStartEventTime;
		int		nEndEventTime;
		int		nActiveTime;
		int		nNumOfActiveTransitions;
		int		nSDB_OctetCountTerminat;
		int		nSDB_OctetCountOrigin;
		int		nNumOfSDBsTerminat;
		int		nNumOfSDBsOrigin;
		int		nNumOfPPPbytesReceived;
	public:
		int		get_nDataOctetCntTerminat()		{	return nDataOctetCntTerminat;	}
		int		get_nDataOctetCntOrigin()		{	return nDataOctetCntOrigin;		}
		int		get_nBadPPPframeCnt()			{	return nBadPPPframeCnt;			}
		int		get_nStartEventTime()			{	return nStartEventTime;			}
		int		get_nEndEventTime()				{	return nEndEventTime;			}
		int		get_nActiveTime()				{	return nActiveTime;				}
		int		get_nNumOfActiveTransitions()	{	return nNumOfActiveTransitions;	}
		int		get_nSDB_OctetCountTerminat()	{	return nSDB_OctetCountTerminat;	}
		int		get_nSDB_OctetCountOrigin()		{	return nSDB_OctetCountOrigin;	}
		int		get_nNumOfSDBsTerminat()		{	return nNumOfSDBsTerminat;		}
		int		get_nNumOfSDBsOrigin()			{	return nNumOfSDBsOrigin;		}
		int		get_nNumOfPPPbytesReceived()	{	return nNumOfPPPbytesReceived;	}
		
		void	put_nDataOctetCntTerminat(int nValue)	{	nDataOctetCntTerminat	= nValue;	}	
		void	put_nDataOctetCntOrigin(int nValue)		{	nDataOctetCntOrigin		= nValue;	}
		void	put_nBadPPPframeCnt(int nValue)			{	nBadPPPframeCnt			= nValue;	}
		void	put_nStartEventTime(int nValue)			{	nStartEventTime			= nValue;	}
		void	put_nEndEventTime(int nValue)			{	nEndEventTime			= nValue;	}
		void	put_nActiveTime(int nValue)				{	nActiveTime				= nValue;	}
		void	put_nNumOfActiveTransitions(int nValue)	{	nNumOfActiveTransitions	= nValue;	}
		void	put_nSDB_OctetCountTerminat(int nValue)	{	nSDB_OctetCountTerminat	= nValue;	}
		void	put_nSDB_OctetCountOrigin(int nValue)	{	nSDB_OctetCountOrigin	= nValue;	}
		void	put_nNumOfSDBsTerminat(int nValue)		{	nNumOfSDBsTerminat		= nValue;	}
		void	put_nNumOfSDBsOrigin(int nValue)		{	nNumOfSDBsOrigin		= nValue;	}
		void	put_nNumOfPPPbytesReceived(int nValue)	{	nNumOfPPPbytesReceived	= nValue;	}
};

class H_TYPE
{
	private:
		int		nAlternateBillingId;
	public:
		int		get_nAlternateBillingId()			{	return nAlternateBillingId;		}
		void	put_nAlternateBillingId(int nValue)	{	nAlternateBillingId	= nValue;	}
};

class I_TYPE
{
	private:
		int				nIP_QoS;
		unsigned int	nInterconnectIP_NetworkProviderID;
		int				nInterconnectIP_NetworkServiceQoS;
		int				nAirlinkQoS;
	public:
		int			 	get_nIP_QoS()				{	return nIP_QoS;			}
		unsigned int 	get_nInterconnectIP_NetworkProviderID()	
		{			return nInterconnectIP_NetworkProviderID;					}
		int			 	get_nInterconnectIP_NetworkServiceQoS()	
		{			return nInterconnectIP_NetworkServiceQoS;					}
		int			 	get_nAirlinkQoS()			{	return nAirlinkQoS;		}
		
		void		 	put_nIP_QoS(int nValue)		{	nIP_QoS	= nValue;		}
		void		 	put_nInterconnectIP_NetworkProviderID(unsigned int nValue)	
		{			nInterconnectIP_NetworkProviderID	= nValue;				}
		void		 	put_nInterconnectIP_NetworkServiceQoS(int nValue)	
		{			nInterconnectIP_NetworkServiceQoS	= nValue;				}
		void		 	put_nAirlinkQoS(int nValue)	{	nAirlinkQoS	= nValue;	}
};

class Y_TYPE	/////////////////////	__AirlinkRecSpecParam
{
	private:
		int		nAirlinkRecType;
		int		nRP_SessionID;
		int		nAirlinkSeqNum;
		int		nMobileOrigin_TerminatID;
	public:
		int		get_nAirlinkRecType()		{	return nAirlinkRecType;				}
		int 	get_nRP_SessionID()			{			return nRP_SessionID;		}
		int	 	get_nAirlinkSeqNum()		{			return nAirlinkSeqNum;		}
		int	 	get_nMobileOrigin_TerminatID()	{	return nMobileOrigin_TerminatID;	}
		
		void	put_nAirlinkRecType(int nValue)			{	nAirlinkRecType	= nValue;			}
		void	put_nRP_SessionID(int nValue)			{	nRP_SessionID	= nValue;			}
		void	put_nAirlinkSeqNum(int nValue)			{	nAirlinkSeqNum	= nValue;			}
		void	put_nMobileOrigin_TerminatID(int nValue){	nMobileOrigin_TerminatID = nValue;	}
};

class Z_TYPE
{
	private:
		char	strContainer		[LEN_CONTAINER+1];
	public:
		Z_TYPE()
		{	memset(strContainer, NULL, LEN_CONTAINER+1);		}
		void	put_strContainer(const char* strValue)	
		{	memcpy(strContainer, strValue, LEN_CONTAINER);		}
		char*	get_strContainer()	{	return strContainer;	}
};

// 한도관리를 위한 class
// Add 한근희
// 2000. 11. 6
class LIMIT_INFO
{
	private:
		unsigned int	nOperationID;        // 메시지를 구별하기 위한 unique key
		int				nSessionStartTime;   // 세션의 시작시간
		int				nSessionEndTime;     // 세션의 중간 정산 시간 
		int				nSessionCount;		 // 세션 Count. 10번마다 중간 정산한다.
		int				nLimitPrice;         // Access Request의 결과로 받은 금액(원)
		int				nNtopFreeTime;       // Access Request의 결과로 받은 금액(초)
		int				nRemain; // 짜투리 시간 or 패킷
		int             nQueryResult;   	// 한도정보요청에 대한 응답여부
		/*
		int				nRatedActiveTime;       // 과금된 사용 시간
		*/
		int				nUsePrice;           // 사용자가 사용한 금액
		int				nUseTime;            // nTop사용자 사용시간
	public:
		unsigned int	get_nOperationID( void ) 				{ return nOperationID; 		}
		int				get_nSessionStartTime( void )			{ return nSessionStartTime; }
		int				get_nSessionEndTime( void )				{ return nSessionEndTime; 	}
		int				get_nSessionCount( void )				{ return nSessionCount; 	}
		int				get_nLimitPrice( void )					{ return nLimitPrice; 		}
		int				get_nNtopFreeTime( void )				{ return nNtopFreeTime; 	}
		int				get_nUsePrice( void )					{ return nUsePrice; 		}
		int				get_nUseTime( void )					{ return nUseTime; 			}
		int				get_nRemain( void )						{ return nRemain;  }
		int				get_nQueryResult( void )				{ return nQueryResult; }
			/*
		int				get_nRatedOctetCntOrigin( void )		{ return nRatedOctetCntOrigin;  }
		*/
		
		void			put_nOperationID( unsigned int value )	{ nOperationID=value; 		}
		void			put_nSessionStartTime( int value )		{ nSessionStartTime=value;	}
		void			put_nSessionEndTime( int value )        { nSessionEndTime=value; 	}
		void			put_nSessionCount( int value )       	{ nSessionCount=value;	 	}
		void			put_nLimitPrice( int value )			{ nLimitPrice=value; 		}
		void			put_nNtopFreeTime( int value )			{ nNtopFreeTime=value; 		}
		void			put_nUsePrice( int value )				{ nUsePrice=value; 			}
		void			put_nUseTime( int value )				{ nUseTime=value; 			}
		void			put_nRemain( int value )				{ nRemain=value; }
		void			put_nQueryResult( int value )			{ nQueryResult=value; }
		/*
		void			put_nRatedOctetCntOrigin( int value )	{ nRatedOctetCntOrigin=value; 	}
		*/				
						LIMIT_INFO()
						{
							nOperationID=0;
							nSessionStartTime=0;
							nSessionEndTime=0;     
							nSessionCount=0;		 
							nLimitPrice=0;         
							nNtopFreeTime=0;      
							nUsePrice=0;           
							nUseTime=0;    
							nRemain=0;
							nQueryResult=0;
							/*
							nRatedOctetCntTerminat=0;
							nRatedOctetCntOrigin=0;
							*/        
						}
};

class UdrFormat
{
	private:
		
	public:
		UDRHEAD_TYPE	stItemHead;
		A_INT_TYPE		stItemA;
		B_TYPE			stItemB;
		C_TYPE			stItemC;
		D_TYPE			stItemD;
		E_TYPE			stItemE;
		F_TYPE			stItemF;
		G_TYPE			stItemG;
		H_TYPE			stItemH;
		I_TYPE			stItemI;
		Y_TYPE			stItemY;
		Z_TYPE			stItemZ;
		LIMIT_INFO		stLimitInfo;

		UDRHEAD_TYPE*	get_ItemHead(void)	{	return (&stItemHead);}
		A_INT_TYPE*	get_ItemA(void) 	{	return (&stItemA);	}
		B_TYPE*		get_ItemB(void) 	{	return (&stItemB);	}
		C_TYPE*		get_ItemC(void) 	{	return (&stItemC);	}
		D_TYPE*		get_ItemD(void) 	{	return (&stItemD);	}
		E_TYPE*		get_ItemE(void) 	{	return (&stItemE);	}
		F_TYPE*		get_ItemF(void) 	{	return (&stItemF);	}
		G_TYPE*		get_ItemG(void) 	{	return (&stItemG);	}
		H_TYPE*		get_ItemH(void) 	{	return (&stItemH);	}
		I_TYPE*		get_ItemI(void) 	{	return (&stItemI);	}
		Y_TYPE*		get_ItemY(void) 	{	return (&stItemY);	}
		Z_TYPE*		get_ItemZ(void) 	{	return (&stItemZ);	}
		LIMIT_INFO*	get_LimitInfo(void)	{	return (&stLimitInfo);	}
		
		void	setItem(const UDRHEAD_TYPE* stItem)
		{	memcpy((char*)&stItemHead, (char*)stItem, sizeof(UDRHEAD_TYPE));	}
		void	setItem(const A_INT_TYPE* stItem)
		{	memcpy((char*)&stItemA, (char*)stItem, sizeof(A_INT_TYPE));	}
		void	setItem(const B_TYPE* stItem)
		{	memcpy((char*)&stItemB, (char*)stItem, sizeof(B_TYPE));	}
		void	setItem(const C_TYPE* stItem)
		{	memcpy((char*)&stItemC, (char*)stItem, sizeof(C_TYPE));	}
		void	setItem(const D_TYPE* stItem)
		{	memcpy((char*)&stItemD, (char*)stItem, sizeof(D_TYPE));	}
		void	setItem(const E_TYPE* stItem)
		{	memcpy((char*)&stItemE, (char*)stItem, sizeof(E_TYPE));	}
		void	setItem(const F_TYPE* stItem)
		{	memcpy((char*)&stItemF, (char*)stItem, sizeof(F_TYPE));	}
		void	setItem(const G_TYPE* stItem)
		{	memcpy((char*)&stItemG, (char*)stItem, sizeof(G_TYPE));	}
		void	setItem(const H_TYPE* stItem)
		{	memcpy((char*)&stItemH, (char*)stItem, sizeof(H_TYPE));	}
		void	setItem(const I_TYPE* stItem)
		{	memcpy((char*)&stItemI, (char*)stItem, sizeof(I_TYPE));	}
		void	setItem(const Y_TYPE* stItem)
		{	memcpy((char*)&stItemY, (char*)stItem, sizeof(Y_TYPE));	}
		void	setItem(const Z_TYPE* stItem)
		{	memcpy((char*)&stItemZ, (char*)stItem, sizeof(Z_TYPE));	}
		void	setLimitInfo(const LIMIT_INFO* stItem)
		{	memcpy((char*)&stLimitInfo, (char*)stItem, sizeof(LIMIT_INFO));	}
};


#endif




/*
		char	strMsId			[LEN_MSID+1];

		int		nIpAddr;
		char	strNAI				[LEN_NAI+1];

		char	strAccountSessionID	[LEN_ACCOUNT_SESSION_ID+1];
		char	strCorrelationID	[LEN_CORRELATION_ID+1];

		int		nMIP_HomeAgent;
		int		nPDSN_FA_Addr;
		int		nServing_PCF;
		char	strBS_MSC_ID		[LEN_BS_MSC_ID+1];

		int		nUserZone;

		int		nForwardMUXOpt;
		int		nReverseMUXOpt;
		int		nForward_Fund_Rate;
		int		nReverse_Fund_Rate;
		int		nServiceOpt;
		int		nForwardTrafficType;
		int		nReverseTrafficType;
		int		nFundFrameSize;
		int		nForwardFundamentalRC;
		int		nReverseFundamentalRC;
		int		nIP_Technology;
		int		nCompulsoryTunnelInd;
		int		nReleaseIndicator;

		int		nDataOctetCntTerminat;
		int		nDataOctetCntOrigin;
		int		nBadPPPframeCnt;
		int		nStartEventTime;
		int		nEndEventTime;		// For BDS
		int		nActiveTime;
		int		nNumOfActiveTransitions;
		int		nSDB_OctetCountTerminat;
		int		nSDB_OctetCountOrigin;
		int		nNumOfSDBsTerminat;
		int		nNumOfSDBsOrigin;
		int		nNumOfPPPbytesReceived;

		int		nAlternateBillingId;

		int		nIP_QoS;
		int		nInterconnectIP_NetworkProviderID;
		int		nInterconnectIP_NetworkServiceQoS;
		int		nAirlinkQoS;

		int		nAirlinkRecType;
		int		nRP_SessionID;
		int		nAirlinkSeqNum;
		int		nMobileOrigin_TerminatID;

		char	strContainer		[LEN_CONTAINER];
*/


