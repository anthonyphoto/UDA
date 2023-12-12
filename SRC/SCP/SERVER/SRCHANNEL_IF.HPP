/****************************************************************************/
/* 파 일 명 : SRchannel_IF.hpp												*/
/* 작 성 자 : KSI / 조상영								                    */
/* 기    능 : SCP의 S&R 채널과 소켓통신을 하는 주클래스 					*/
/* 작성일자 : 2000년 10월 23일                                              */
/* 변경일자 :     년   월   일                                              */
/* 관련화일 : scp_msg.hpp, scpformat.hpp									*/
/* 기    타 :                                                               */
/****************************************************************************/
#ifndef	_SRCHANNEL_IF_HPP
#define	_SRCHANNEL_IF_HPP

#include <intndefs.hpp>
#include <time.h>
#include <socketC.hpp>
#include <scpformat.hpp>
#include "scp_msg.hpp"


#define	READTIME			50		// 소켓Read time-out
#define	SENDTIME			10		// 소켓Send time-out

#define	RECONNECT_WAIT_TIME	30		// 소켓종료후 다시접속하기전에 기다릴시간
#define	RESEND_WAIT_TIME	10		// ACK 응답실패후 다시자료전송하기전에 기다릴 시간


uint	TransacNum=0;		// Download client
uint	STransacNum=0;		// Upload server

//--------------------------------------------------------------------
class SRchannel_IF {

	private:
						
		int				channel;		// S,R 채널 구분
		char			strBuf[2048];	// SCP로 부터받은 메시지의 데이터부분
		SockC			*c_Sock;
		ScpHeader		mHeaderRcv;		// SCP로 부터받은 메시지의 해더부분
		ScpMsg			msgC;			// 메시지생성및 관리 클레스
		
		
		int 	RecvMsg2Server(void);	// SCP에서 메시지를 받는다.
		
	public:

		SRchannel_IF(char *address, int port, int ch);	// SCP 주소, 포트번호, 채널구분
		~SRchannel_IF(void);
		
		void	SockClose(void);		
		int 	DoConnection(void);		// 최초연결시도
		int 	DoDisConnection(void);  // 접속 끊기
		int		ProcessCHK(void);		// S-Channel 을통한 프로세스상태확인
		
		
		//	SCP에 DownLoad정보를 보내고 ACK를 받는다. 
		// 	SCPFormat : Agg에서 MQ를 통해 받은 정보
		int 	DownLoad(SCPFormatAgg* OneRecord);
		
		//	SCP에 DownLoadResult정보를 보내고 ACK를 받는다. 
		// 	SCPFormat : 고객관리에서 MQ를 통해 받은 정보 (요건이확정되면 변경요망)
	//	int 	UploadResult(ReCustFormat* OneRecord);	
		
		int		SendMsg2Server(int msg, int st=SC);	//SCP에 DownLoad정보를 제외한 메시지및 ACK를 보낸다.
		int 	RecvMsg2Rchannel(void);				//SCP R-Channel를 통한 메시지수신
		
		// 수신한 자료를 MQ 구조체 형식으로 변환한다.(Filter에 전송)
		int  	Get_MQMsg(UdrFormat* uformat) { return msgC.Get_MQMsg(channel, uformat); }	
	//	void  	Get_MQMsg(CustFormat* fmSCP) { msgC.Get_MQMsg(channel, fmSCP); }	
		
};


//--------------------------------------------------------------------
SRchannel_IF::SRchannel_IF(char *address, int port, int ch)
{
	channel = ch;
	memset(strBuf, 0x00, sizeof(strBuf));
	TransacNum=0;
	c_Sock = new SockC(address, port);
}


//--------------------------------------------------------------------
SRchannel_IF::~SRchannel_IF(void)
{
	delete c_Sock;
}


//--------------------------------------------------------------------
void SRchannel_IF::SockClose(void)
{
	c_Sock->CloseTCPSock();	
}


//--------------------------------------------------------------------
int SRchannel_IF::DoConnection(void)
{
	int i=3;
	
	if(channel==SCHANNEL) cout << "[SCHANNEL] Try connectting to server......" << endl;	
	else cout << "[RCHANNEL] Try connectting to server......" << endl;	
	if(c_Sock->CreateTCPClient()<0) return FA;
	if(channel==SCHANNEL) cout << "[SCHANNEL] Connection success!" << endl;
	else cout << "[RCHANNEL] Try connectting to success!" << endl;	

	
	if(channel==SCHANNEL)
	{
		while(i--){
			if(SendMsg2Server(S_CONNECT_REQ)==SC){
				printf("[SCHANNEL] (S_CONNECT_REQ) Success! \n");
				break;
			}else {
				printf("[SCHANNEL] (S_CONNECT_REQ) Fail! : Waiting \n");
				sleep(RESEND_WAIT_TIME);
			}			
		}
	}else
	{
		while(i--){
			if(SendMsg2Server(R_CONNECT_REQ)==SC){
				printf("[RCHANNEL] (R_CONNECT_REQ) Success! \n");
				break;
			}else {
				printf("[RCHANNEL] (R_CONNECT_REQ) Fail! : Waiting \n");
				sleep(RESEND_WAIT_TIME);
			}			
		}				
		
	}
			
	if(i==-1) 
	{
		SockClose();
		return FA;
	}
	
	return(SC);
}



//--------------------------------------------------------------------
int SRchannel_IF::DoDisConnection(void)
{
	
	if(channel==SCHANNEL)
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
	}else
	{
		if(SendMsg2Server(R_RELEASE_REQ)==SC)
		{
			printf("[RCHANNEL] (R_RELEASE_REQ) Success! \n");
			return SC;
		}else 
		{
			printf("[RCHANNEL] (R_RELEASE_REQ) Fail! \n");
			return FA;
		}
	}		
	
}


//--------------------------------------------------------------------
int SRchannel_IF::ProcessCHK(void)
{
	int i=3;	
	
	if(channel==RCHANNEL) return FA;
	
	while(i--){
		if(SendMsg2Server(PROCESS_STATE_REQ)==SC){
			printf("[SCHANNEL] (PROCESS_STATE_REQ) Success! \n");
			break;
		}else {
			printf("[SCHANNEL] (PROCESS_STATE_REQ) Fail! : Waiting 10s\n");
			sleep(RESEND_WAIT_TIME);
		}			
	}
	
	if(i==-1) 
	{
		SockClose();
		return FA;
	}
	
	return(SC);
}


//--------------------------------------------------------------------
//Agg로 부터 받은 고객정보를 SCP에 전송해 한도정보를 요청한다. (SCHANNEL-Only)
int SRchannel_IF::DownLoad(SCPFormatAgg* OneRecord)
{
	int i=3;
	
	while(i--)
	{
		//명령어처리 요구(S-Channel)
		printf("[SCHANNEL] Send2Msg : DOWNLOAD_REQ \n");
		msgC.MK_Msg(DOWNLOAD_REQ, OneRecord);
		
		c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
		if(RecvMsg2Server()!=SC) 
		{
			sleep(RESEND_WAIT_TIME);
			continue;
		}
	
		if(mHeaderRcv.MessageId==S_RELEASE_REQ)
		{//ACK가 오지않고 종료메시지가 발생하면...
			printf("[SCHANNEL] Send2Msg : S_RELEASE_REQ_ACK \n");
			msgC.MK_Msg(S_RELEASE_REQ_ACK);			
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
			SockClose();
			return RELEASE_CLOSE; // 처리방법 필요
		}
		
		if(msgC.CHK_Msg(DOWNLOAD_REQ_ACK)!=SC) sleep(RESEND_WAIT_TIME);
		else break;
	}		
	
	if(i==-1) 
	{
		SockClose();
		return FA;
	}
	
	return(SC);
}


//--------------------------------------------------------------------
//고객관리로 부터 받은 처리결과를 전송한다. 
/*
int SRchannel_IF::UploadResult(ReCustFormat* OneRecord)
{
	int i=3;
	
	while(i--)
	{
		//명령어처리 요구(S-Channel)
		printf("[SCHANNEL] Send2Msg : UPLOAD_RESULT \n");
		msgC.MK_Msg(UPLOAD_RESULT, OneRecord);
		
		c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
		if(RecvMsg2Server()!=SC) 
		{
			sleep(RESEND_WAIT_TIME);
			continue;
		}
	
		if(mHeaderRcv.MessageId==S_RELEASE_REQ)
		{//ACK가 오지않고 종료메시지가 발생하면...
			printf("[SCHANNEL] Send2Msg : S_RELEASE_REQ_ACK \n");
			msgC.MK_Msg(S_RELEASE_REQ_ACK);			
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
			SockClose();
			return RELEASE_CLOSE; // 처리방법 필요
		}
		
		if(msgC.CHK_Msg(UPLOAD_RESULT_ACK)!=SC) sleep(RESEND_WAIT_TIME);
		else break;
	}		
	
	if(i==-1) 
	{
		SockClose();
		return FA;
	}
	
	return(SC);
}




*/

//--------------------------------------------------------------------
int SRchannel_IF::RecvMsg2Rchannel(void)
{
	if(c_Sock->RecvSocketTimeoutEx((char *)&mHeaderRcv, MHEADSIZE, 0, READTIME)<=0) return FA;
	if(mHeaderRcv.DataSize>0) {
		if(c_Sock->RecvSocketTimeoutEx(strBuf, mHeaderRcv.DataSize, 0, READTIME)<=0) return FA;
	}
	
	switch(mHeaderRcv.MessageId)
	{
		case DOWNLOAD_RESULT:
			printf("[RCHANNEL] Recv2Msg : DOWNLOAD_RESULT \n");
			break;
		case UPLOAD_REQ:
			printf("[RCHANNEL] Recv2Msg : UPLOAD_REQ \n");
			break;
		case UPLOAD_RESULT:
			printf("[RCHANNEL] Recv2Msg : UPLOAD_RESULT \n");
			break;
		case RCHANNEL_RELEASE_REQ:
			printf("[RCHANNEL] Recv2Msg : RCHANNEL_RELEASE_REQ \n");
			break;
		case RCHANNEL_RELEASE_REQ_ACK:
			printf("[RCHANNEL] Recv2Msg : RCHANNEL_RELEASE_REQ_ACK \n");
			break;									
		default:
			printf("[RCHANNEL] Recv2Msg : Error MessageId [%d] \n", mHeaderRcv.MessageId);
			c_Sock->ClearSocketTimeout(10);
			return FA;
			break;			
	}
	
	msgC.SetMsgBuf(mHeaderRcv, strBuf, RCHANNEL);
	return (mHeaderRcv.MessageId);
}


//--------------------------------------------------------------------
int SRchannel_IF::RecvMsg2Server(void)
{
	//if(op==SCHANNEL){
	if(c_Sock->RecvSocketTimeoutEx((char *)&mHeaderRcv, MHEADSIZE, 0, READTIME)<=0) return SOCK_FA;
	if(mHeaderRcv.DataSize>0) {
		if(c_Sock->RecvSocketTimeoutEx(strBuf, mHeaderRcv.DataSize, 0, READTIME)<=0) return SOCK_FA;
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
			TransacNum = mHeaderRcv.TransacId.Num;
			break;
		case R_CONNECT_REQ_ACK:
			printf("[RCHANNEL] Recv2Msg : R_CONNECT_REQ_ACK \n");
			break;
		case S_RELEASE_REQ:
			printf("[SCHANNEL] Recv2Msg : S_RELEASE_REQ \n");
			break;
		case S_RELEASE_REQ_ACK:
			printf("[SCHANNEL] Recv2Msg : S_RELEASE_REQ_ACK \n");
			break;			
		case R_RELEASE_REQ:
			printf("[RCHANNEL] Recv2Msg : R_RELEASE_REQ \n");
			break;
		case R_RELEASE_REQ_ACK:
			printf("[RCHANNEL] Recv2Msg : R_RELEASE_REQ_ACK \n");
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
		case UPLOAD_RESULT_ACK:
			printf("[SCHANNEL] Recv2Msg : UPLOAD_RESULT_ACK \n");		
			break;	
		default:
			printf("[#ERROR##] SRchannel_IF::RecvMsg2Server(void) [%d] \n", mHeaderRcv.MessageId);
			c_Sock->ClearSocketTimeout(10);
			break;			
	}
	
	msgC.SetMsgBuf(mHeaderRcv, strBuf, channel);
	return SC;
}



//--------------------------------------------------------------------
int SRchannel_IF::SendMsg2Server(int msg, int st)
{
	int re;
	
	switch(msg){
		case S_CONNECT_REQ:
			printf("[SCHANNEL] Send2Msg : S_CONNECT_REQ \n");
			msgC.MK_Msg(S_CONNECT_REQ);
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
			if((re=RecvMsg2Server())!=SC) return re;
			return msgC.CHK_Msg(S_CONNECT_REQ_ACK);
		
		
		case R_CONNECT_REQ:
			printf("[RCHANNEL] Send2Msg : R_CONNECT_REQ \n");
			msgC.MK_Msg(R_CONNECT_REQ);
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
			if((re=RecvMsg2Server())!=SC) return re;
			return msgC.CHK_Msg(R_CONNECT_REQ_ACK);			
					
			
		case S_RELEASE_REQ:
			printf("[SCHANNEL] Send2Msg : S_RELEASE_REQ \n");
			msgC.MK_Msg(S_RELEASE_REQ);
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
			if((re=RecvMsg2Server())!=SC) return re;
			return msgC.CHK_Msg(S_RELEASE_REQ_ACK);			

		case R_RELEASE_REQ:
			printf("[RCHANNEL] Send2Msg : R_RELEASE_REQ \n");
			msgC.MK_Msg(R_RELEASE_REQ);
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
			if((re=RecvMsg2Server())!=SC) return re;
			return msgC.CHK_Msg(R_RELEASE_REQ_ACK);						
			
		case PROCESS_STATE_REQ:
			printf("[SCHANNEL] Send2Msg : PROCESS_STATE_REQ \n");
			msgC.MK_Msg(PROCESS_STATE_REQ);
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
			if((re=RecvMsg2Server())!=SC) return re;
			return msgC.CHK_Msg(PROCESS_STATE_REQ_ACK);		
		
		case DOWNLOAD_RESULT_ACK:
			printf("[RCHANNEL] Send2Msg : DOWNLOAD_RESULT_ACK \n");
			msgC.MK_Msg(DOWNLOAD_RESULT_ACK);
			if(st==FA) msgC.MK_MsgFA(RCHANNEL, 2);	
			
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
			return SC;
					
		case UPLOAD_RESULT_ACK:
			printf("[RCHANNEL] Send2Msg : UPLOAD_RESULT_ACK \n");
			msgC.MK_Msg(UPLOAD_RESULT_ACK);
			if(st==FA) msgC.MK_MsgFA(RCHANNEL, 2);	
			
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
			return SC;
			
		case UPLOAD_REQ_ACK:
			printf("[RCHANNEL] Send2Msg : UPLOAD_REQ_ACK \n");
			msgC.MK_Msg(UPLOAD_REQ_ACK);
			if(st==FA) msgC.MK_MsgFA(RCHANNEL, 2);	
			
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
			return SC;
			
		case RCHANNEL_RELEASE_REQ_ACK:			
			printf("[RCHANNEL] Send2Msg : RCHANNEL_RELEASE_REQ_ACK \n");
			msgC.MK_Msg(RCHANNEL_RELEASE_REQ_ACK);
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
			return SC;
		
		default:
			printf("[##ERROR##] SRchannel_IF::SendMsg2Server(int msg, int st) \n");
			sleep(5);
			return SC;
	}
}




#endif


