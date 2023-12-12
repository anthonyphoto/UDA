#include "udpC.hpp"
#include "hexdump.h"
//#include "md5.h"
#include <md5.hpp>


typedef unsigned short 	sint;
typedef unsigned int 	uint;
typedef unsigned char	uchar;

//---------------------------------------------
//UDA-SCP 메시지 HEADER
struct NASMSGHEADER {
	uchar	Code;
	uchar	Identifier;
	sint	Length;
	uchar	Authenticator[16];
};	

struct ATTRIBUTES {
	uchar 	Type;
	uchar	Length;
	uint	Value;	
}; 

typedef NASMSGHEADER	NasHeader;
typedef ATTRIBUTES		Attributes;

#define USER_NAME					1
#define NAS_IP_ADDRESS				4
#define NAS_PORT					5
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




//----------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	
	int			i, pos;
	SockC 		*_socket;
	char		sockBuf[4096];
	char  		strBuf[4096];
	char		strMd[4096];
	char		imsi[1024];
	char		testkey[20]="testtesttesttest";
	char		smd5[16];
	uchar		ipadd[4];
	uchar		uimsi[10];
	uint		nport;
	//char		*testkey="TESTTESTTESTTEST";
	NasHeader	rHeader, sHeader;
	Attributes	attr;
	int 		SockStatus;
	MD5 		*conMd5;
	FILE		*fp;
	
	
	if(argc==2)
	{
		strcpy(testkey, argv[1]);
		printf("Key : %s\n", testkey);
		sleep(1);
	}
		

	_socket = new SockC((char*)SERV_UDP_ADDR, SERV_UDP_PORT);
	cout << "[NOTICE] CreateUDPServer => " << 
		_socket->CreateUDPServer((char*)SERV_UDP_ADDR,SERV_UDP_PORT) << endl;

	cout << "SERV_UDP_ADDR => " << SERV_UDP_ADDR << endl;
	cout << "SERV_UDP_PORT => " << SERV_UDP_PORT << endl;
	
	memset(sockBuf, 0x00, sizeof(sockBuf));
	memset(strBuf,0x00,sizeof(strBuf));
	memset((void *)&rHeader,0x00,sizeof(rHeader));
	memset((void *)&sHeader,0x00,sizeof(sHeader));
	memset(strMd,0x00,sizeof(strMd));
	
	//fp = fopen("radius.dat", "w+b");
	fp = fopen("radius.dat", "r+b");
	
	while (1)
	{
		memset((void *)&rHeader,0x00,sizeof(rHeader));
		// Receive Data...!!
		cout << "<<<<   RecvfromUDPSocket       >>>>" << endl;
		
		//SockStatus = _socket->RecvfromUDPSocket(sockBuf, 4096, 0);
		fread(sockBuf, sizeof(sockBuf), 1, fp);
		fclose(fp);
		
		memcpy((char *)&rHeader, sockBuf, sizeof(rHeader));
		SockStatus = rHeader.Length;	// fp
		if(rHeader.Length>20) memcpy(strBuf, (char *)&sockBuf[sizeof(rHeader)], rHeader.Length-20);
		if(rHeader.Length-20 > SockStatus)
		{
			cout << "SockStatus : " << SockStatus << endl;
			cout << "Size Small..Continue......!! " << endl;
			continue;	
		}
		
		/*
		cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
		cout << "SockStatus : " << SockStatus << endl;
		cout << "_socket->recv_addr.sin_addr.s_addr => " << inet_ntoa((in_addr &)_socket->recv_addr.sin_addr.s_addr) << endl;
		cout << "_socket->recv_addr.sin_port => " << _socket->recv_addr.sin_port << endl;
		cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
		*/
		
		//cout << "Code       : " << rHeader.Code << endl;
		//cout << "Identifier : " << rHeader.Identifier << endl;
		printf( "Code       : %d\n", rHeader.Code);
		printf( "Identifier : %d\n", rHeader.Identifier);
		cout << "Length     : " << rHeader.Length << endl;
		cout << "Authenticator : " << endl;
		//rHeader.Length = SockStatus + 20;	// @@@
		HexDump((char *)rHeader.Authenticator, 16); // @@@@ 
		
		cout << "-------HEX DUMP----------------------" << endl;
		HexDump(sockBuf, SockStatus); // @@@@ 
		cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
		if(rHeader.Length<=20) continue;
		
		//fwrite(sockBuf, 1, sizeof(sockBuf), fp);   
		//fclose(fp);

		
		//---------MD5 hash 검증---------------------------------------------
		conMd5 = new MD5;
		//끝에추가
		memcpy(strMd, sockBuf, SockStatus);
		memcpy((char *)&strMd[4], 0x00, 16);	//@@@
		memcpy((char *)&strMd[SockStatus], testkey, strlen(testkey));//@@ 비밀키 추가
		
		/*
		memcpy(strMd, testkey, strlen(testkey));//@@ 비밀키 앞에 추가
		memcpy((char *)&strMd[strlen(testkey)], sockBuf, SockStatus);
		memcpy((char *)&strMd[4+strlen(testkey)], 0x00, 16);	//@@@
		*/
		
		
		conMd5->update((unsigned char *)strMd, rHeader.Length+strlen(testkey));
		
		cout << "-------HEX DUMP [strMd] --------------" << endl;
		HexDump(strMd, rHeader.Length+strlen(testkey)); // @@@@ 
		conMd5->finalize();
		memcpy(smd5, conMd5->raw_digest(), 16);
		
		if(memcmp(conMd5->raw_digest(), rHeader.Authenticator, 16))
		{
			cout << " ### MD5 KEY ERROR ### " << endl;
			cout << "My Authenticator : " << endl;
			HexDump((char *)conMd5->raw_digest(), 16); // 
			
			cout << "Authenticator : " << endl;
			HexDump((char *)rHeader.Authenticator, 16); // 
		
			//cout << rHeader.Authenticator << ":" << conMd5->raw_digest() << endl;
			delete conMd5;
			//continue;
			break;
		}else 
		{ 
			cout << " ### MD5 KEY OK ### " << endl; 
			delete conMd5;
			//break;
		}
		//---------MD5 hash 검증----------------------------------------------
		
		
		
		cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
		pos=0;
		while(1){
		switch(strBuf[pos]) {
			case USER_NAME:
				memset(imsi, 0x00, sizeof(imsi));
				memcpy(imsi, (char *)&strBuf[pos+2], strBuf[pos+1]-2);
				printf("[USER_NAME] Length = %d\n",  strBuf[pos+1]);
				printf("[USER_NAME] Value = %s\n\n", imsi);
				pos+=strBuf[pos+1];
				break;
				
			case NAS_IP_ADDRESS:
				memset(ipadd, 0x00, 4);
				memcpy(ipadd, (char *)&strBuf[pos+2],  4);
				printf("[NAS_IP_Address] Length = %d \n", strBuf[pos+1]);
				printf("[NAS_IP_Address] Value = %d.%d.%d.%d \n\n", ipadd[0],ipadd[1],ipadd[2],ipadd[3]);
				pos+=strBuf[pos+1];
				break;	
				
			case NAS_PORT:	
				nport=0;
				memcpy((char *)&nport, (char *)&strBuf[pos+2], strBuf[pos+1]-2);
				printf("[NAS_PORT] Length = %d\n", strBuf[pos+1]);
				printf("[NAS_PORT] Value = %d\n\n", nport);
				pos+=strBuf[pos+1];	
				break;		
				
			case FRAMED_PROTOCOL:	//7
				nport=0;
				memcpy((char *)&nport, (char *)&strBuf[pos+2], strBuf[pos+1]-2);
				printf("[FRAMED_PROTOCOL] Length = %d\n", strBuf[pos+1]);
				printf("[FRAMED_PROTOCOL] Value = %d\n\n", nport);
				pos+=strBuf[pos+1];	
				break;				
				
			case FRAMED_IP_ADDRESS:
				memset(ipadd, 0x00, 4);
				memcpy(ipadd, (char *)&strBuf[pos+2], 4);
				printf("[FRAMED_IP_ADDRESS] Length = %d\n", strBuf[pos+1]);
				printf("[FRAMED_IP_ADDRESS] Value = %d.%d.%d.%d \n\n", ipadd[0],ipadd[1],ipadd[2],ipadd[3]);
				pos+=strBuf[pos+1];	
				break;
				
			case VENDOR_SPECIFIC: //26
				printf("[VENDOR_SPECIFIC] Length = %d\n", strBuf[pos+1]);
				
				memset(imsi, 0x00, sizeof(imsi));
				memcpy(imsi, (char *)&strBuf[pos+2], 4);
				printf("[VENDOR_SPECIFIC] Id     = %02X%02X%02X \n", (uchar)imsi[1], (uchar)imsi[2],(uchar)imsi[3]);
				
				printf("[VENDOR_SPECIFIC] Vendor Type = %d\n", strBuf[pos+6]);
				printf("[VENDOR_SPECIFIC] Vendor Len  = %d\n", strBuf[pos+7]);
				printf("[VENDOR_SPECIFIC] Vendor Attri= ");
				
				memcpy(imsi, (char *)&strBuf[pos+8], strBuf[pos+7]-2);
				for(i=0; i<strBuf[pos+7]-2; i++) printf("%02X", (uchar)imsi[i]);
				printf("\n\n");
				pos+=strBuf[pos+1];	
				break;
				
			case CALLING_STATION_ID:
				memset(imsi, 0x00, sizeof(imsi));
				memcpy(imsi, (char *)&strBuf[pos+2], strBuf[pos+1]-2);
				printf("[CALLING_STATION_ID] Length = %d\n", strBuf[pos+1]);
				printf("[CALLING_STATION_ID] Value = %s\n\n", imsi);
				pos+=strBuf[pos+1];	
				break;
				
			case FRAMED_APPLETALK_NETWORK:
				memcpy((char *)&attr, (char *)&strBuf[pos], sizeof(attr));
				cout << "[Framed_AppleTalk_Network] Value = " << attr.Value << endl;
				pos+=6;
				break;			
				
			case ACCT_STATUS_TYPE:	//40
				memset(uimsi, 0x00, sizeof(uimsi));
				memcpy(uimsi, (char *)&strBuf[pos+2], strBuf[pos+1]-2);
				printf("[ACCT_STATUS_TYPE] Length = %d\n", strBuf[pos+1]);
				printf("[ACCT_STATUS_TYPE] Value = %X [", uimsi[3]);
				switch(uimsi[3]){
					case 1: printf("START]\n\n"); break;
					case 2: printf("STOP]\n\n"); break;
					case 7: printf("Accounting-On]\n\n"); break;
					case 8: printf("Sccounting-Off]\n\n"); break;
					default : printf("Error]\n\n"); break;
				}
				pos+=strBuf[pos+1];	
				break;
				
			case ACCT_DELAY_TIME:
				memcpy((char *)&attr, (char *)&strBuf[pos], sizeof(attr));
				cout << "[ACCT_DELAY_TIME] Value = " << attr.Value << endl;
				pos+=6;
				break;			
									
			case ACCT_INPUT_OCTETS: //42
				nport=0;
				memcpy((char *)&nport, (char *)&strBuf[pos+2], strBuf[pos+1]-2);
				printf("[ACCT_INPUT_OCTETS] Length = %d\n", strBuf[pos+1]);
				printf("[ACCT_INPUT_OCTETS] Value = %d\n\n", nport);
				pos+=strBuf[pos+1];
				break;								
				
			case ACCT_OUTPUT_OCTETS: //43
				nport=0;
				memcpy((char *)&nport, (char *)&strBuf[pos+2], strBuf[pos+1]-2);
				printf("[ACCT_OUTPUT_OCTETS] Length = %d\n", strBuf[pos+1]);
				printf("[ACCT_OUTPUT_OCTETS] Value = %d\n\n", nport);
				pos+=strBuf[pos+1];
				break;	
				
			case ACCT_SESSION_ID: //44
				memset(imsi, 0x00, sizeof(imsi));
				memcpy(imsi, (char *)&strBuf[pos+2], strBuf[pos+1]-2);
				printf("[ACCT_SESSION_ID] Length = %d\n", strBuf[pos+1]);
				printf("[ACCT_SESSION_ID] Value = %s\n\n", imsi);
				pos+=strBuf[pos+1];
				break;						
				
			case ACCT_AUTHENTIC:
				memcpy((char *)&attr, (char *)&strBuf[pos], sizeof(attr));
				cout << "[ACCT_AUTHENTIC] Value = " << attr.Value << endl;
				pos+=6;
				break;
				
			case ACCT_SESSION_TIME: // 46
				nport=0;
				memcpy((char *)&nport, (char *)&strBuf[pos+2], strBuf[pos+1]-2);
				printf("[ACCT_SESSION_TIME] Length = %d\n", strBuf[pos+1]);
				printf("[ACCT_SESSION_TIME] Value = %d\n\n", nport);
				pos+=strBuf[pos+1];
				break;
				
			case ACCT_INPUT_PACKETS:
				nport=0;
				memcpy((char *)&nport, (char *)&strBuf[pos+2], strBuf[pos+1]-2);
				printf("[ACCT_INPUT_PACKETS] Length = %d\n", strBuf[pos+1]);
				printf("[ACCT_INPUT_PACKETS] Value = %d\n\n", nport);
				pos+=strBuf[pos+1];
				break;
				
			case ACCT_OUTPUT_PACKETS:
				nport=0;
				memcpy((char *)&nport, (char *)&strBuf[pos+2], strBuf[pos+1]-2);
				printf("[ACCT_OUTPUT_PACKETS] Length = %d\n", strBuf[pos+1]);
				printf("[ACCT_OUTPUT_PACKETS] Value = %d\n", nport);
				pos+=strBuf[pos+1];
				break;
				
			case ACCT_TERMINATE_CAUSE:
				memcpy((char *)&attr, (char *)&strBuf[pos], sizeof(attr));
				cout << "[ACCT_TERMINATE_CAUSE] Value = " << attr.Value << endl;
				pos+=6;
				break;
				
			case ACCT_MULTI_SESSION_ID:
				memset(imsi, 0x00, sizeof(imsi));
				memcpy(imsi, (char *)&strBuf[pos+2], strBuf[pos+1]-2);
				cout << "[ACCT_MULTI_SESSION_ID] Value = " << imsi << endl;
				pos+=(strBuf[pos+1]-2);
				break;
				
			case ACCT_LINK_COUNT:
				memcpy((char *)&attr, (char *)&strBuf[pos], sizeof(attr));
				cout << "[ACCT_LINK_COUNT] Value = " << attr.Value << endl;
				pos+=6;
				break;
				
			default:
				printf("[NOTICE] ERROR FOND NOT TYPE (sleep 5s..!!) : %d\n", strBuf[pos]);
				printf("[NOTICE] NOT TYPE Length = %d\n\n", strBuf[pos+1]);
				pos+=strBuf[pos+1];
				sleep(1);
				break;
			
		}
		
		if(pos >= (rHeader.Length-20)) break;
		} // end of while(1);
		cout << "<<<<<<<<<<<<<<<<<<<<<<<<<[ "<< pos <<"]>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl << endl;
		
		exit(1); //fp
		
		memcpy((char*)&sHeader, (char*)&rHeader, sizeof(sHeader));
		sHeader.Code=5; // 과금응답
		sHeader.Length=20;
		
		//---------MD5 hash 생성--------------
		conMd5 = new MD5;
		memset(strMd, 0x00, sizeof(strMd));
		memcpy(strMd, (char *)&sHeader, 4);
		memcpy((char *)&strMd[sHeader.Length], testkey, 16);	//@@ 비밀키 추가
		conMd5->update((unsigned char *)strMd, sHeader.Length+16);
		conMd5->finalize();
		memcpy(sHeader.Authenticator, conMd5->raw_digest(), 16);
		delete conMd5;
		//---------MD5 hash 검증--------------
		
		printf("<<<<   SendtoUDPSocket   : [%d:%d]   >>>>\n", rHeader.Identifier, sHeader.Identifier);
		SockStatus = _socket->SendtoUDPSocket((void *)&sHeader, sHeader.Length, (char *)_socket->recv_addr.sin_addr.s_addr, _socket->recv_addr.sin_port);
		if (SockStatus > 0)
		{
			cout << "[NOTICE] SendByte => " << SockStatus << endl;
			cout << "Send strBuf => " << strBuf << endl;
		}
		else//((SockStatus <0)
		{
			cout << "[ERROR] Send ERROR" << endl;
		}
		
		/*
		cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
		cout << "_socket->send_addr.sin_addr.s_addr => " << inet_ntoa((in_addr &)_socket->send_addr.sin_addr.s_addr) << endl;
		cout << "_socket->send_addr.sin_port => " << _socket->send_addr.sin_port << endl;
		cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
		*/
		cout << "Next...Call...(5s sleep!!)" << endl << endl;
		sleep(5);
		
	} // end of while(1);
	
	delete _socket;
	
}
	
