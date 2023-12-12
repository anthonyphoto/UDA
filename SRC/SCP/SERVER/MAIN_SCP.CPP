#include <time.h>
#include <intndefs.hpp>
#include "UsocketC.hpp"
#include "scp_msg.hpp"
#include "SRchannel_IF.hpp"

uint	MsgNumber;

void sigCapture(int sigNo)
{
	exit(-1);
}
void SockRun(USockC *c_Sock);



int main(void)
{
	int				pid, sockfd;
	USockC			svrSock("150.25.4.140", 6667);
	
	signal(SIGINT,sigCapture);
	signal(SIGTERM,sigCapture);
	signal(SIGKILL,sigCapture);
	signal(SIGQUIT,sigCapture);
	
	
	cout << "[SCP-S] Try Create to server......" << endl;	
	if((sockfd=svrSock.CreateTCPServer())<0)
	{
		cout << "[SCP-S] Cteate Error......" << endl;	
		return 0;
	}
	
	while(1){
		cout << "[SCP-S] Server Running.. Wating..." << endl;	
		if(svrSock.AcceptSocket(sockfd)==-1) {
			cout << "[SCP-S] Accept Error..sleep 10s" << endl;	
			sleep(10);
			continue;
		}
		
		
		
		pid=fork();
		if(pid<0) cout << "[SCP-S] Server fork Error.." << endl;	
		else if(pid==0)
		{
			SockRun(&svrSock);
			cout << "[SCP-S] SockRun Exit" << endl;	
			exit(0);
		}else if(pid>0)
		{
			wait(&pid);
			svrSock.CloseTCPSock();
			cout << "[SCP-S] Parent : Client Sock Close" << endl;	
		}
		
		
		/*
		if((pid=fork())<0) cout << "[SCP-S] Server fork Error.." << endl;	
		else if(pid==0){
			SockRun(&svrSock);
			exit(0);
		}else if(pid>0) {
			wait(&pid);
			svrSock.CloseTCPSock(sockfd);
		}
		*/
		
		
	}
}


	

void SockRun(USockC *c_Sock)	
{
	char			strBuf[2048];
	ScpHeader		mHeaderRcv;	
	ScpMsg			msgC;
	
	// 최초 UDA에서 접속요구를 받는다. [0]
	if(c_Sock->RecvSocketTimeoutEx((char *)&mHeaderRcv, MHEADSIZE, 0, READTIME)<=0) 
	{
		printf("[SCP-S] Read Error [0]-1\n");
		return;
	}
		
	if(mHeaderRcv.DataSize>0) {
		if(c_Sock->RecvSocketTimeoutEx(strBuf, mHeaderRcv.DataSize, 0, READTIME)<=0) 
		{
			printf("[SCP-S] Read Error [0]-2\n");
			return;
		}
	}
	
	msgC.SetMsgBuf(mHeaderRcv, strBuf, SCHANNEL);
	if(msgC.CHK_Msg(S_CONNECT_REQ)==FA) return;
	printf("[SCP-S] Recv[%d] :: S_CONNECT_REQ \n", msgC.SendLen(SCHANNEL));
	
	
	// UDA에 접속ACK를 보낸다. [1]
	msgC.MK_Msg(S_CONNECT_REQ_ACK);
	c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
	printf("[SCP-S] Send :: S_CONNECT_REQ_ACK [1]-2 \n");
	
	
	
	while(1)
	{
		// UDA에서 요청을 받는다. [2]
		if(c_Sock->RecvSocketTimeoutEx((char *)&mHeaderRcv, MHEADSIZE, 0, READTIME)<=0) 
		{
			printf("[SCP-S] Read Time Out.. Retry..!! [2]-1\n");
			continue;
		}
		
		if(mHeaderRcv.DataSize>0) {
			if(c_Sock->RecvSocketTimeoutEx(strBuf, mHeaderRcv.DataSize, 0, READTIME)<=0) 
			{
				printf("[SCP-S] Read Error [2]-2\n");
				return;
			}
		}	
		msgC.SetMsgBuf(mHeaderRcv, strBuf, SCHANNEL);
		
		
		switch(mHeaderRcv.MessageId)
		{
			case PROCESS_STATE_REQ :
				printf("[SCP-S] Recv[%d] :: PROCESS_STATE_REQ \n", msgC.SendLen(SCHANNEL));
				msgC.MK_Msg(PROCESS_STATE_REQ_ACK);
				c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
				printf("[SCP-S] Send :: PROCESS_STATE_REQ_ACK \n");
				break;	
			case S_RELEASE_REQ:
				printf("[SCP-S] Recv[%d] :: S_RELEASE_REQ \n", msgC.SendLen(SCHANNEL));
				msgC.MK_Msg(S_RELEASE_REQ_ACK);
				c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
				printf("[SCP-S] Send :: S_RELEASE_REQ_ACK => [Socket Close]\n");
				return;
			case DOWNLOAD_REQ:
				printf("[SCP-S] Recv[%d] :: DOWNLOAD_REQ \n", msgC.SendLen(SCHANNEL));
				msgC.MK_Msg(DOWNLOAD_REQ_ACK);
				c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
				printf("[SCP-S] Send :: DOWNLOAD_REQ_ACK \n");
				{
					FILE	*fp;	
					fp=fopen("aggsend.dat", "w+b");
					fwrite(strBuf, 1, mHeaderRcv.DataSize, fp);
					fclose(fp);
					sleep(1);
				}
				break;									
			case UPLOAD_RESULT:
				printf("[SCP-S] Recv[%d] :: UPLOAD_RESULT \n", msgC.SendLen(SCHANNEL));
				msgC.MK_Msg(UPLOAD_RESULT_ACK);
				c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
				printf("[SCP-S] Send :: UPLOAD_RESULT_ACK \n");
				break;
			default:
				printf("[SCP-S] Error MessageID : %d \n", mHeaderRcv.MessageId);
				break;
		}
		
		sleep(1);
	}
	
}

