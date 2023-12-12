#include <stdio.h>
#include <time.h>
#include <strings.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class UdpSockC
{
	public:
		UdpSockC(){ nLen = sizeof(struct sockaddr_in); }
		~UdpSockC(){}
		int CreateUDPClient(char *desIp, char *myIp, int desPort);
		int CreateUDPServer(char *ip, int port);
		int Send2Msg(char *buf,int len,int nflag);
		int Reply2MsgSender(char *buf,int len,int nflag);
		int RecvFromMsg(char *buf, int len, int nflag);
		int RecvfromMsgEx(char *Buf, int nBytes, unsigned int nFlags);
		
	private:
		struct sockaddr_in srvAddr, cliAddr, recvAddr;
		int		nUdpSocket;
		int		maxSockNo;
		int		nLen;
		fd_set  readFDS;
};
		

int UdpSockC::CreateUDPClient(char *desIp, char *myIp, int desPort)
{
	if ((nUdpSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		return -1;

	// Destination 
    bzero((char*) &srvAddr, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr(desIp);
    srvAddr.sin_port = htons(desPort);

	// Source	
    bzero((char*) &cliAddr, sizeof(cliAddr));
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = inet_addr(myIp);
    cliAddr.sin_port = htons(desPort);
    if (bind(nUdpSocket, &cliAddr, sizeof(cliAddr)) < 0)
        return -1;

    return nUdpSocket;
}


int UdpSockC::CreateUDPServer(char *ip, int port)
{

    if ((nUdpSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -1;
    bzero((char*) &srvAddr, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr(ip);
    srvAddr.sin_port = htons(port);
    if (bind(nUdpSocket, &srvAddr, sizeof(srvAddr)) < 0)
        return -1;

    return nUdpSocket;
}


int UdpSockC::Send2Msg(char *buf,int len,int nflag)
{
	int nSendByte;

	nSendByte = sendto(nUdpSocket,buf, len, nflag, &srvAddr, nLen);

	return nSendByte;
}

int UdpSockC::Reply2MsgSender(char *buf,int len,int nflag)
{
	int nSendByte;

	nSendByte = sendto(nUdpSocket,buf, len, nflag, &recvAddr, nLen);

	return nSendByte;

}

int UdpSockC::RecvFromMsg(char *buf, int len, int nflag)
{
	int nRecvByte;

	nRecvByte = recvfrom(nUdpSocket, buf, len, nflag, &recvAddr, &nLen);

	return nRecvByte;
}


int UdpSockC::RecvfromMsgEx(char *Buf, int nBytes, unsigned int nFlags)
{
    int nRet;
    struct timeval timeout;
	int nRecvBytes;

	timeout.tv_sec = 3;//time setting
	timeout.tv_usec = 0;
	maxSockNo = nUdpSocket+1;
	
	FD_ZERO( &readFDS );
	FD_SET(nUdpSocket, &readFDS );
// < 0 : error
// = 0 : no data, timeout
// > 0 : exist data
	nRet = select( maxSockNo, (int*)&readFDS, (int*)0, (int*)0, &timeout);
	if( nRet < 0){
		if(errno == EINTR) return(0);
		else return(-1);
	}else if( nRet > 0) {
		if((nUdpSocket > 0)&&(FD_ISSET(nUdpSocket, &readFDS))){

			nRecvBytes = recvfrom(nUdpSocket,Buf,nBytes,nFlags,&recvAddr,&nLen);

	    	return nRecvBytes;
	    }
	}else return 0;

}



