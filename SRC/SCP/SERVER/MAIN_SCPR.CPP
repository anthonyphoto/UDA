#include <time.h>
#include <intndefs.hpp>
#include <unistd.h>
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
	USockC			svrSock("150.25.4.140", 6670);
	
	signal(SIGINT,sigCapture);
	signal(SIGTERM,sigCapture);
	signal(SIGKILL,sigCapture);
	signal(SIGQUIT,sigCapture);
	
	
	cout << "[SCP-R] Try Create to server......" << endl;	
	if((sockfd=svrSock.CreateTCPServer())<0)
	{
		cout << "[SCP-R] Cteate Error......" << endl;	
		return 0;
	}
	
	while(1){
		cout << "[SCP-R] Server Running.. Wating..." << endl;	
		if(svrSock.AcceptSocket(sockfd)==-1) {
			cout << "[SCP-R] Accept Error..sleep 10s" << endl;	
			sleep(10);
			continue;
		}
		
		
		
		pid=fork();
		if(pid<0) cout << "[SCP-R] Server fork Error.." << endl;	
		else if(pid==0)
		{
			SockRun(&svrSock);
			cout << "[SCP-R] SockRun Exit" << endl;	
			exit(0);
		}else if(pid>0)
		{
			wait(&pid);
			svrSock.CloseTCPSock();
			cout << "[SCP-R] Parent : Client Sock Close" << endl;	
		}
		
		
		/*
		if((pid=fork())<0) cout << "[SCP-R] Server fork Error.." << endl;	
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
	int             i=0, j=0;
	char			strBuf[2048];
	ScpHeader		mHeaderRcv;	
	ScpMsg			msgC;
	uchar			ukey;
	FILE			*fp;
	SCPResultFormat	reFormat; 	//A,B,D
	SCPFormat		scFormat;	//A,B,C,D
	
	// 최초 UDA에서 접속요구를 받는다. [0]
	if(c_Sock->RecvSocketTimeoutEx((char *)&mHeaderRcv, MHEADSIZE, 0, READTIME)<=0) 
	{
		printf("[SCP-R] Read Error [0]-1\n");
		return;
	}
		
	if(mHeaderRcv.DataSize>0) {
		if(c_Sock->RecvSocketTimeoutEx(strBuf, mHeaderRcv.DataSize, 0, READTIME)<=0) 
		{
			printf("[SCP-R] Read Error [0]-2\n");
			return;
		}
	}
	
	msgC.SetMsgBuf(mHeaderRcv, strBuf, RCHANNEL);
	if(msgC.CHK_Msg(R_CONNECT_REQ)==FA) return;
	printf("[SCP-R] Recv[%d] :: R_CONNECT_REQ \n", msgC.SendLen(RCHANNEL));
	
	
	// UDA에 접속ACK를 보낸다. [1]
	msgC.MK_Msg(R_CONNECT_REQ_ACK);
	c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
	printf("[SCP-R] Send :: R_CONNECT_REQ_ACK [1]-2 \n");
	
	
	while(1)
	{
		/*
		if(j) j=0; else j=1;
		
		i++;
		if(i>10) {sleep(5); i=0;}
		if(j) msgC.MK_Msg(UPLOAD_REQ);
		else msgC.MK_Msg(DOWNLOAD_RESULT);
		*/
		
		/*
		{
			SCPFormat		ssFormat;	//A,B,C,D	
			ssFormat.get_ItemA()->set_Key(0x01);
			ssFormat.get_ItemA()->set_MSID("1192025312     ");
			ssFormat.get_ItemB()->set_OperId(222);
			ssFormat.get_ItemB()->set_AccSessionId("chocho");
			ssFormat.get_ItemB()->set_CorreId("sang");
				
			FILE *fp;
				
			fp=fopen("aggsend.dat", "w+b");
			fwrite((char *)&ssFormat, 1, 36, fp);
			fclose(fp);
		}
		*/
			
		
		j=0;
		while(1)
		{
		
			if(!access("aggsend.dat", F_OK))
			{	// agg요청을 scpif_s로 부터 받았다.
				fp=fopen("aggsend.dat", "r+b");
				fread((void*)&ukey, 1, 1,  fp);
				fclose(fp);
				printf("FILEOK...%d\n", ukey);
			
				if(ukey==0x01 || ukey==0x02 ) 
				{
					msgC.MK_Msg(DOWNLOAD_RESULT);
					i=0;
					break;
				}else unlink("aggsend.dat");
				if( ukey==0x03){
					if(ukey == 0x03) limit = 0;
					break;
				}	

			}
			//sleep(1);
			i++;
			//---------------
			if(c_Sock->RecvSocketTimeoutSec((char *)&mHeaderRcv, MHEADSIZE, 0, READTIME, 1)>0)
			{
				if(mHeaderRcv.MessageId==R_RELEASE_REQ)
				{
					printf("[SCP-R] Recv :: R_RELEASE_REQ \n");
					msgC.MK_Msg(R_RELEASE_REQ_ACK);
					c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
					printf("[SCP-R] Send :: R_RELEASE_REQ_ACK => [Socket Close]\n");
					return;
				}
			}
			//---------------
			
			//--------------
			if(i>10) 
			//if(i>1) 
			{
				j=1;
				msgC.MK_Msg(UPLOAD_REQ);		
				i=0;
				break;
			}
		}
				
				
		c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
		if(j) printf("[SCP-R] Send[%d] :: UPLOAD_REQ \n", msgC.SendLen(RCHANNEL));
		else printf("[SCP-R] Send[%d] :: DOWNLOAD_RESULT \n", msgC.SendLen(RCHANNEL));
		
		
		// UDA에서 요청을 받는다. [2]
		if(c_Sock->RecvSocketTimeoutEx((char *)&mHeaderRcv, MHEADSIZE, 0, READTIME)<=0) 
		{
			printf("[SCP-R] Read Time Out.. Retry..!! [2]-1\n");
			continue;
		}
		
		if(mHeaderRcv.DataSize>0) {
			if(c_Sock->RecvSocketTimeoutEx(strBuf, mHeaderRcv.DataSize, 0, READTIME)<=0) 
			{
				printf("[SCP-R] Read Error [2]-2\n");
				return;
			}
		}	
		msgC.SetMsgBuf(mHeaderRcv, strBuf, RCHANNEL);
		
		
		
		switch(mHeaderRcv.MessageId)
		{
			case PROCESS_STATE_REQ :
				printf("[SCP-R] Recv[%d] :: PROCESS_STATE_REQ \n", msgC.SendLen(RCHANNEL));
				msgC.MK_Msg(PROCESS_STATE_REQ_ACK);
				c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
				printf("[SCP-R] Send :: PROCESS_STATE_REQ_ACK \n");
				break;	
			case R_RELEASE_REQ:
				printf("[SCP-R] Recv[%d] :: R_RELEASE_REQ \n", msgC.SendLen(RCHANNEL));
				msgC.MK_Msg(R_RELEASE_REQ_ACK);
				c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
				printf("[SCP-R] Send :: R_RELEASE_REQ_ACK => [Socket Close]\n");
				return;
			case DOWNLOAD_RESULT_ACK:
				printf("[SCP-R] Recv[%d] :: DOWNLOAD_RESULT_ACK \n", msgC.SendLen(RCHANNEL));
				break;	
			case UPLOAD_REQ_ACK:
				printf("[SCP-R] Recv[%d] :: UPLOAD_REQ_ACK \n", msgC.SendLen(RCHANNEL));
				break;								
			default:
				printf("[SCP-R] Error MessageID : %d \n", mHeaderRcv.MessageId);
				break;
		}
		
		sleep(1);
		
	} //endof while(1);
	
}

