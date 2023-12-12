#ifndef _017PDSN_IF_HPP
#define _017PDSN_IF_HPP

//#include <UDR.hpp>
#include <ConfigCTRL.hpp>
#include "CSnmpCli.hpp"

#define SERV_UDP_ADDR 	"150.25.4.142"
#define SERV_UDP_PORT 	1813
#define	MD5_HASH_KEY	"testtesttesttest"
#define	HEAD_SIZE		20


typedef unsigned short 	sint;
typedef unsigned int 	uint;
typedef unsigned char	uchar;


//------------------------------------------------------------------
//UDA-SCP 메시지 HEADER
struct NASMSGHEADER {
	uchar	Code;
	uchar	Identifier;
	sint	Length;
	uchar	Authenticator[16];
};	


typedef NASMSGHEADER	NasHeader;		// size = 20;


//------------------------------------------------------------------------
#define USER_NAME					1
#define NAS_IP_ADDRESS				4
#define NAS_PORT					5
#define SERVICE_TYPE				6
#define	FRAMED_PROTOCOL				7
#define	FRAMED_IP_ADDRESS			8
#define	VENDOR_SPECIFIC				26
#define CALLING_STATION_ID			31
#define FRAMED_APPLETALK_NETWORK 	38
#define	ACCT_STATUS_TYPE			40
#define	ACCT_DELAY_TIME				41
#define ACCT_INPUT_OCTETS			42
#define	ACCT_OUTPUT_OCTETS			43
#define ACCT_SESSION_ID				44
#define ACCT_AUTHENTIC				45
#define ACCT_SESSION_TIME			46
#define ACCT_INPUT_PACKETS			47
#define ACCT_OUTPUT_PACKETS			48
#define ACCT_TERMINATE_CAUSE		49
#define ACCT_MULTI_SESSION_ID		50
#define ACCT_LINK_COUNT				51
#define ACCT_EVENT_TIME				55

#define NAS_PORT_TYPE				61

CSnmpCli*	snmp7IF;				//	snmp추가시 수정 (5. 25, jsj)
//------------------------------------------------------------------------
class Pdsn_IF
{
	
	private:
		
		SockC 		*_socket;
		int			NoAckN;	 // ACK를  연속적으로 못받은횟수
		
		ConfigCTRL  *configM;
		CPdsnMsg 	stPdsnMsg;
		
		NasHeader	rHeader;
		NasHeader	sHeader;
		
		char		sockBuf[4096];
		char 		Serv_Addr[20];
		char		Md5Key[20];
		int 		Serv_Port;
		
		int 		nRet;
		
	public:
		int		DATA_ERROR;		// 1.error (잘못된 data)
		int		SOCK_ERROR;
		
		Pdsn_IF(char *mdkey, char *serv_addr, int serv_port);
		~Pdsn_IF();
		
		int		Socket_Create();
		int		SendData();
		int 	RecvAck();
};

#endif 

