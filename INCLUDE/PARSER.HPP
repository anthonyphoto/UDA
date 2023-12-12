#ifndef	PARSER_HPP
#define PARSER_HPP

#include <iostream.h>

const	int		LEN_UDR_ITEM	= 3;
const	int		LEN_UDR_PARAM	= 60;
const	int		LEN_UDR_PATTERN	= 22;

const	int		CNT_UDR_ITEM	= 55;

const	char	TAG_TYPE_START[]		= "<";
const	char	TAG_TYPE_END[]		= "</";

//	Associated with UDR Format
const int LEN_MSCID					= 15;
const int LEN_NAI					=  8;
const int LEN_ACCOUNT_SESSION_ID	=  8;
const int LEN_CORRELATION_ID		=  8;
const int LEN_CONTAINER				= 16;
const int LEN_BS_MSC_ID				= 12;

enum TYPE	{TAG_TYPE, STR_TYPE, INT_TYPE};
enum ADJUST	{NONE, RIGHT, LEFT};

typedef struct	__UDR_ITEM
{
	char	strItem[LEN_UDR_ITEM+1];
	char	strParam[LEN_UDR_PARAM+1];
	char	strPattern[LEN_UDR_PATTERN+1];
	TYPE	nType;
	int		nLength;
	int		nStringLen;
	ADJUST		nAdjustment;
}	STUDRITEM, *STUDRITEMPTR;


STUDRITEM	stConfigUdr[CNT_UDR_ITEM]= \
{	{"A"  , "Mobile Identifiers",									"M_Id>", 		TAG_TYPE, 0	, 0 , NONE},
	{"A1" , "MSC ID", 												"MSID>", 		STR_TYPE, 15, 15, LEFT},
	{"B"  , "User Identifiers",										"U_Id>", 		TAG_TYPE, 0	, 0 , NONE},
	{"B1" , "IP Address", 											"IP_Add>",		INT_TYPE, 4	, 12, LEFT},
	{"B2" , "Network Access Identifier ", 							"NAI>", 		STR_TYPE, 64, 32, LEFT},
	{"C"  , "Session Identifiers",									"S_Id>", 		TAG_TYPE, 0	, 0 , NONE},
	{"C1" , "Account Session ID", 									"A_S_ID>",		STR_TYPE, 8	, 8 , LEFT},
	{"C2" , "Correlation ID", 										"C_ID>",		STR_TYPE, 8	, 8 , LEFT},
	{"D"  , "Infrastructure Identifiers",							"I_Id>", 		TAG_TYPE, 0	, 0 , NONE},
	{"D1" , "MIP Home Agent", 										"HA>",			INT_TYPE, 4	, 12, LEFT},
	{"D2" , "PDSN/FA Address", 										"PDSN_FA>",		INT_TYPE, 4	, 12, LEFT},
	{"D3" , "Serving PCF", 											"PCF>",			INT_TYPE, 4	, 12, LEFT},
	{"D4" , "BS / MSC ID", 											"BS_MSC_ID>",	STR_TYPE, 12, 12, NONE},
	{"E"  , "Zone Identifiers",										"Z_Id>", 		TAG_TYPE, 0	, 0 , NONE},
	{"E1" , "User Zone", 											"U_Z>",			INT_TYPE, 4	, 2 , RIGHT},
	{"F"  , "Session Status",										"S_S>", 		TAG_TYPE, 0	, 0 , NONE},
	{"F1" , "Forward MUX Option", 									"F_M_O>",		INT_TYPE, 4	, 2 , RIGHT},
	{"F2" , "Reverse MUX Option", 									"R_M_O>",		INT_TYPE, 4	, 2 , RIGHT},
	{"F3" , "Forward Fundamental Rate", 							"F_F_R>",		INT_TYPE, 4	, 3 , NONE},
	{"F4" , "Reverse Fundamental Rate",		 						"R_F_R>",		INT_TYPE, 4	, 3 , NONE},
	{"F5" , "Service Option", 										"S_O>",			INT_TYPE, 4	, 2 , RIGHT},
	{"F6" , "Forward Traffic Type", 								"F_T_T>",		INT_TYPE, 4	, 1 , NONE},
	{"F7" , "Reverse Traffic Type (Primary, Secondary)",	 		"R_T_T>",		INT_TYPE, 4	, 1 , NONE},
	{"F8" , "Fundamental Frame Size", 								"F_F_S>",		INT_TYPE, 4	, 2 , RIGHT},
	{"F9" , "Forward Fundamental RC", 								"F_F_RC>",		INT_TYPE, 4	, 1 , NONE},
	{"F10", "Reverse Fundamental RC", 								"R_F_RC>",		INT_TYPE, 4	, 1 , NONE},
	{"F11", "IP Technology", 										"IP_T>",		INT_TYPE, 4	, 1 , NONE},
	{"F12", "Compulsory Tunnel Indicator", 							"C_T_I>",		INT_TYPE, 4	, 1 , NONE},
	{"F13", "Release Indicator", 									"R_I>",			INT_TYPE, 4	, 1 , NONE},
	{"G"  , "Session Activity",										"S_A>", 		TAG_TYPE, 0	, 0 , NONE},
	{"G1" , "Data Octet Count (Terminating)", 						"D_O_C_T>",		INT_TYPE, 4	, 10, RIGHT},
	{"G2" , "Data Octet Count (Originating)", 						"D_O_C_O>",		INT_TYPE, 4	, 10, RIGHT},
	{"G3" , "Bad PPP frame Count", 									"PPP_F_C>",		INT_TYPE, 4	, 10, RIGHT},
	{"G4" , "Event Time", 											"E_T>",			INT_TYPE, 4	, 15, LEFT},
	{"G8" , "Active Time", 											"A_T>",			INT_TYPE, 4	, 15, LEFT},
	{"G9" , "Number of Active Transitions", 						"N_A_T>",		INT_TYPE, 4	, 15, LEFT},
	{"G10", "SDB Octet Count (Terminating)", 						"SDB_I_C_T>",	INT_TYPE, 4	, 10, RIGHT},
	{"G11", "SDB Octet Count (Originating)", 						"SDB_I_C_O>",	INT_TYPE, 4	, 10, RIGHT},
	{"G12", "Number of SDBs (Terminating)", 						"N_SDB_T>",		INT_TYPE, 4	, 10, RIGHT},
	{"G13", "Number of SDBs (Originating)", 						"N_SDB_O>",		INT_TYPE, 4	, 10, RIGHT},
	{"G14", "Number of PPP bytes received", 						"N_PPP_R>",		INT_TYPE, 4	, 10, RIGHT},
	{"H"  , "Special Billing Instructions",							"S_B_I>", 		TAG_TYPE, 0	, 0	, NONE},
	{"H1" , "Alternate Billing Identifier", 						"A_B_Id>",		INT_TYPE, 4	, 10, RIGHT},
	{"I"  , "Quality of Service",									"QOS>", 		TAG_TYPE, 0	, 0 , NONE},
	{"I1" , "IP Quality of Service (QoS)", 							"IP_QOS>",		INT_TYPE, 4	, 2 , RIGHT},
	{"I2" , "Interconnection IP Network Provider ID", 				"IP_N_P_ID>",	INT_TYPE, 4	, 12, LEFT},
	{"I3" , "Interconnecting IP Network Service Quality of Service","IP_N_S_QOS>",	INT_TYPE, 4	, 2 , RIGHT},
	{"I4" , "Airlink Quality of Service (QoS)", 					"A_QOS>",		INT_TYPE, 4	, 2 , RIGHT},
	{"Y"  , "Airlink Record Specific Parameters",					"A_R_S_P>",		TAG_TYPE, 0	, 0 , NONE},
	{"Y1" , "Airlink Record Type", 									"A_R_T>",		INT_TYPE, 4	, 0 , NONE},
	{"Y2" , "R-P Session ID", 										"RP_S_ID>",		INT_TYPE, 4	, 0 , NONE},
	{"Y3" , "Airlink Sequence Number", 								"A_S_N>",		INT_TYPE, 4	, 0 , NONE},
	{"Y4" , "Mobile Originated / Mobile Terminated Indicator", 		"M_O_T_Id>",	INT_TYPE, 4	, 0 , NONE},
	{"Z"  , "Container",											"C>",			TAG_TYPE, 0	, 0 , NONE},
	{"Z1" , "Container", 											"Ct>",			STR_TYPE, 16, 0	, NONE}	
};

typedef struct	__MobileIdenfier
{
	char	strMscId			[LEN_MSCID+1];
}A_TYPE, *A_PTR_TYPE;

typedef struct	__UserIdenfier
{
	int		nIpAddr;
	char	strNAI				[LEN_NAI+1];
}B_TYPE, *B_PTR_TYPE;

typedef struct	__SessionIdenfier
{
	char	strAccountSessionID	[LEN_ACCOUNT_SESSION_ID+1];
	char	strCorrelationID	[LEN_CORRELATION_ID+1];
}C_TYPE, *C_PTR_TYPE;

typedef struct	__InfraIdenfier
{
	int		nMIP_HomeAgent;
	int		nPDSN_FA_Addr;
	int		nServing_PCF;
	char	strBS_MSC_ID		[LEN_BS_MSC_ID+1];
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
	int		nEventTime;
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
	int		nIP_QoS;
	int		nInterconnectIP_NetworkProviderID;
	int		nInterconnectIP_NetworkServiceQoS;
	int		nAirlinkQoS;
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


class UdrFormat
{
	private:
		A_TYPE	stItemA;
		B_TYPE	stItemB;
		C_TYPE	stItemC;
		D_TYPE	stItemD;
		E_TYPE	stItemE;
		F_TYPE	stItemF;
		G_TYPE	stItemG;
		H_TYPE	stItemH;
		I_TYPE	stItemI;
		Y_TYPE	stItemY;
		Z_TYPE	stItemZ;
		
	public:
		operator const struct __MobileIdenfier* () { return (__MobileIdenfier*)this; }
		
		void	getItem(A_TYPE* stItem)
		{	memcpy((char*)stItem, (char*)&stItemA, sizeof(A_TYPE));	}
		void	getItem(B_TYPE* stItem)
		{	memcpy((char*)stItem, (char*)&stItemB, sizeof(B_TYPE));	}
		void	getItem(C_TYPE* stItem)
		{	memcpy((char*)stItem, (char*)&stItemC, sizeof(C_TYPE));	}
		void	getItem(D_TYPE* stItem)
		{	memcpy((char*)stItem, (char*)&stItemD, sizeof(D_TYPE));	}
		void	getItem(E_TYPE* stItem)
		{	memcpy((char*)stItem, (char*)&stItemE, sizeof(E_TYPE));	}
		void	getItem(F_TYPE* stItem)
		{	memcpy((char*)stItem, (char*)&stItemF, sizeof(F_TYPE));	}
		void	getItem(G_TYPE* stItem)
		{	memcpy((char*)stItem, (char*)&stItemG, sizeof(G_TYPE));	}
		void	getItem(H_TYPE* stItem)
		{	memcpy((char*)stItem, (char*)&stItemH, sizeof(H_TYPE));	}
		void	getItem(I_TYPE* stItem)
		{	memcpy((char*)stItem, (char*)&stItemI, sizeof(I_TYPE));	}
		void	getItem(Y_TYPE* stItem)
		{	memcpy((char*)stItem, (char*)&stItemY, sizeof(Y_TYPE));	}
		void	getItem(Z_TYPE* stItem)
		{	memcpy((char*)stItem, (char*)&stItemZ, sizeof(Z_TYPE));	}
		
		void	setItem(const A_TYPE* stItem)
		{	memcpy((char*)&stItemA, (char*)stItem, sizeof(A_TYPE));	}
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
};


/*
		char	strMscId			[LEN_MSCID+1];
		
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

#endif