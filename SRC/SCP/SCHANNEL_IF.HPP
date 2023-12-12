#ifndef	_SCHANNEL_IF_HPP
#define	_SCHANNEL_IF_HPP

#include <intndefs.hpp>
#include <time.h>
#include <socketC.hpp>
#include "scpformat.hpp"
#include "scp_msg.hpp"


#define	READTIME			300
#define	SENDTIME			300

uint	TransacNum=0;		// Download client
uint	STransacNum=0;		// Upload server


class Schannel_IF {

	private:
			
		char			strBuf[2048];
		SockC			*c_Sock;
		ScpHeader		mHeaderRcv;	
		ScpMsg			msgC;
		
		
	public:

		Schannel_IF(char *address, int port);
		~Schannel_IF(void);
		
		
		int 	DoConnection(void);
		int 	DoDisConnection(void);
		int		SendMsg2Server(int msg, int st=SC);
		int 	RecvMsg2Server(void);
		
		int 	DownLoad(SCPFormat* OneRecord);
		//int		UpLoad(char *cmd);
		
		
		void	SockClose(void);
		
		//-------------------------------------------------
		int 	RecvMsg2Rchannel(void);
		//2000.10.06 위까지..
		
		/*
		int		MakeSend(int op=0);
		
		int		SendLen(void){ return mHeaderSend.length+MHEADSIZE;}
		int		RcvLen(void){ return mHeaderRcv.length;}
		char	*GetBuf(void){ return strBuf;}
		
		int		ReadUda(SockC *sockRadius);
		
		int		RecvCheck(int op=0);
		*/
};


//--------------------------------------------------------------------
Schannel_IF::Schannel_IF(char *address, int port)
{
	memset(strBuf, 0x00, sizeof(strBuf));
	TransacNum=0;
	c_Sock = new SockC(address, port);
}


//--------------------------------------------------------------------
Schannel_IF::~Schannel_IF(void)
{
	delete c_Sock;
}


//--------------------------------------------------------------------
void Schannel_IF::SockClose(void)
{
	c_Sock->CloseTCPSock();	
}


//--------------------------------------------------------------------
int Schannel_IF::DoConnection(void)
{
	int i=10;
	
	cout << "[SCHANNEL] Try connectting to server......" << endl;	
	if(c_Sock->CreateTCPClient()<0) return FA;
	cout << "[SCHANNEL] Connection success!" << endl;
	
	while(i--){
	//while(1){		
		if(SendMsg2Server(S_CONNECT_REQ)==SC){
			printf("[SCHANNEL] (S_CONNECT_REQ) Success! \n");
			break;
		}else {
			printf("[SCHANNEL] (S_CONNECT_REQ) Fail! \n");
			sleep(5);
		}			
	}
	if(i==-1) return FA;
	
	return(SC);
}



//--------------------------------------------------------------------
int Schannel_IF::DoDisConnection(void)
{

	if(SendMsg2Server(S_RELEASE_REQ)==SC)
	{
		printf("[SCHANNEL] (S_RELEASE_REQ) Success! \n");
		return SC;
	}else 
	{
		printf("[SCHANNEL] (S_RELEASE_REQ) Fail! \n");
		return FA;
	}
	
	/*
	i=3;
	while(i--){
		if(SendMsg2Server(R_RELEASE_REQ)==SC){
			printf("[NOTICE] (R_RELEASE_REQ) Success! \n");
			break;
		}else {
			printf("[NOTICE] (R_RELEASE_REQ) Fail! \n");
			sleep(5);
		}			
	}
	if(i==-1) return FA;
	*/
}



//--------------------------------------------------------------------
int Schannel_IF::DownLoad(SCPFormat* OneRecord)
{
	//명령어처리 요구(S-Channel)
	printf("[SCHANNEL] Send2Msg : DOWNLOAD_REQ \n");
	msgC.MK_Msg(DOWNLOAD_REQ, OneRecord);
	c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
	
	if(RecvMsg2Server()!=SC) return FA;
	if(msgC.CHK_Msg(DOWNLOAD_REQ_ACK)!=SC) return FA;
	
	return SC;
	/*
	//명령어처리결과수신(R-Channel)
	if(RecvMsg2Server(RCHANNEL)!=SC) return FA;
	if(msgC.CHK_Msg(DOWNLOAD_RESULT)!=SC) return FA;
	
	msgC.MK_Msg(DOWNLOAD_RESULT_ACK);
	r_Sock.SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
	*/
}



/*
//--------------------------------------------------------------------
int Schannel_IF::UpLoad(char *cmd)
{
	//명령어처리결과수신(R-Channel)
	if(RecvMsg2Server(RCHANNEL)!=SC) return FA;
	if(msgC.CHK_Msg(UPLOAD_REQ)!=SC) return FA;
	
	msgC.MK_Msg(UPLOAD_REQ_ACK);
	r_Sock.SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, 500);
	
	
	//명령어처리 요구(S-Channel)
	printf("[NOTICE] Send2Msg : UPLOAD_RESULT \n");
	msgC.MK_Msg(UPLOAD_RESULT, "test command..!!");
	c_Sock.SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, 500);
	
	if(RecvMsg2Server(SCHANNEL)!=SC) return FA;
	if(msgC.CHK_Msg(UPLOAD_RESULT_ACK)!=SC) return FA;
}
*/




//--------------------------------------------------------------------
int Schannel_IF::RecvMsg2Rchannel(void)
{
	if(c_Sock->ReadSocketTimeout((char *)&mHeaderRcv, MHEADSIZE, READTIME)!=SOCKET_SUCCESS) return FA;
	if(mHeaderRcv.DataSize>0) {
		if(c_Sock->ReadSocketTimeout(strBuf, mHeaderRcv.DataSize, READTIME)!=SOCKET_SUCCESS) return FA;
	}
	
	switch(mHeaderRcv.MessageId)
	{
		case DOWNLOAD_RESULT:
			printf("[RCHANNEL] Recv2Msg : DOWNLOAD_RESULT \n");
			break;
		case UPLOAD_RESULT:
			printf("[RCHANNEL] Recv2Msg : UPLOAD_RESULT \n");
			break;
		case RCHANNEL_RELEASE_REQ:
			printf("[RCHANNEL] Recv2Msg : RCHANNEL_RELEASE_REQ \n");
			break;						
		default:
			printf("[RCHANNEL] Recv2Msg : Error MessageId [%d] \n", mHeaderRcv.MessageId);
			break;			
	}
	
	msgC.SetMsgBuf(mHeaderRcv, strBuf, RCHANNEL);
	return SC;
}


//--------------------------------------------------------------------
int Schannel_IF::RecvMsg2Server(void)
{
	//if(op==SCHANNEL){
	if(c_Sock->ReadSocketTimeout((char *)&mHeaderRcv, MHEADSIZE, READTIME)!=SOCKET_SUCCESS) return FA;
	if(mHeaderRcv.DataSize>0) {
		if(c_Sock->ReadSocketTimeout(strBuf, mHeaderRcv.DataSize, READTIME)!=SOCKET_SUCCESS) return FA;
	}
	
	//}
	/*
	else{
		if(r_Sock->ReadSocketTimeout((char *)&mHeaderRcv, MHEADSIZE, READTIME)!=SOCKET_SUCCESS) return FA;
		if(mHeaderRcv.DataSize>0) {
		 	if(r_Sock->ReadSocketTimeout(strBuf, mHeaderRcv.DataSize, READTIME)!=SOCKET_SUCCESS) return FA;
		}
	}
	*/
	
	
	switch(mHeaderRcv.MessageId)
	{
		case S_CONNECT_REQ_ACK:
			printf("[SCHANNEL] Recv2Msg : S_CONNECT_REQ_ACK \n");
			break;
		case R_CONNECT_REQ_ACK:
			printf("[SCHANNEL] Recv2Msg : R_CONNECT_REQ_ACK \n");
			break;
		case S_RELEASE_REQ:
			printf("[SCHANNEL] Recv2Msg : S_RELEASE_REQ \n");
			break;
		case S_RELEASE_REQ_ACK:
			printf("[SCHANNEL] Recv2Msg : S_RELEASE_REQ_ACK \n");
			break;			
		case R_RELEASE_REQ:
			printf("[SCHANNEL] Recv2Msg : R_RELEASE_REQ \n");
			break;
		case R_RELEASE_REQ_ACK:
			printf("[SCHANNEL] Recv2Msg : R_RELEASE_REQ_ACK \n");
			break;			
		case PROCESS_STATE_REQ_ACK:
			printf("[SCHANNEL] Recv2Msg : PROCESS_STATE_REQ_ACK \n");
			break;		
		case DOWNLOAD_REQ_ACK:
			printf("[SCHANNEL] Recv2Msg : DOWNLOAD_REQ_ACK \n");
			break;
		case DOWNLOAD_RESULT:
			printf("[SCHANNEL] Recv2Msg : DOWNLOAD_RESULT \n");
			break;
		default:
			printf("[SCHANNEL] Recv2Msg : Error MessageId [%d] \n", mHeaderRcv.MessageId);
			break;			
	}
	
	msgC.SetMsgBuf(mHeaderRcv, strBuf, SCHANNEL);
	return SC;
}



//--------------------------------------------------------------------
int Schannel_IF::SendMsg2Server(int msg, int st)
{
	
	switch(msg){
		case S_CONNECT_REQ:
			printf("[SCHANNEL] Send2Msg : S_CONNECT_REQ \n");
			msgC.MK_Msg(S_CONNECT_REQ);
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
			if(RecvMsg2Server()!=SC) return FA;
			return msgC.CHK_Msg(S_CONNECT_REQ_ACK);
		
		/*
		case R_CONNECT_REQ:
			printf("[NOTICE] Send2Msg : R_CONNECT_REQ \n");
			msgC.MK_Msg(R_CONNECT_REQ);
			r_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
			if(RecvMsg2Server(RCHANNEL)!=SC) return FA;
			return msgC.CHK_Msg(R_CONNECT_REQ_ACK);			
		*/			
			
		case S_RELEASE_REQ:
			printf("[SCHANNEL] Send2Msg : S_RELEASE_REQ \n");
			msgC.MK_Msg(S_RELEASE_REQ);
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
			if(RecvMsg2Server()!=SC) return FA;
			return msgC.CHK_Msg(S_RELEASE_REQ_ACK);			

		case R_RELEASE_REQ:
			printf("[SCHANNEL] Send2Msg : R_RELEASE_REQ \n");
			msgC.MK_Msg(R_RELEASE_REQ);
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
			if(RecvMsg2Server()!=SC) return FA;
			return msgC.CHK_Msg(R_RELEASE_REQ_ACK);						
			
		case PROCESS_STATE_REQ:
			printf("[SCHANNEL] Send2Msg : PROCESS_STATE_REQ \n");
			msgC.MK_Msg(PROCESS_STATE_REQ);
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
			if(RecvMsg2Server()!=SC) return FA;
			return msgC.CHK_Msg(PROCESS_STATE_REQ_ACK);		
		
		case DOWNLOAD_RESULT_ACK:
			printf("[RCHANNEL] Send2Msg : DOWNLOAD_RESULT_ACK \n");
			msgC.MK_Msg(DOWNLOAD_RESULT_ACK);
			if(st==FA) MK_MsgFA(2);	
			
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
			if(RecvMsg2Server()!=SC) return FA;
			return msgC.CHK_Msg(PROCESS_STATE_REQ_ACK);											
	}
}




#endif


