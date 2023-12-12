/******************************************************************
* �� �� ��     
      : SRchannel_IF.hpp                
* ��   ��                                                          
     : SCP�� S&R ä�ΰ� ��������� �ϴ� ��Ŭ����
* �� �� ��     
     : ����                         
* first data  
     : 2000. 10. 23                      
* last updata 
     : 2000. 10. 23                  
* �� �� ȭ ��  
     : scp_msg.hpp, scpformat.hpp	
* program history                                
                           
* Version
  : 1.1 ( 2001. 05. 21 )                                           
******************************************************************/


#ifndef	_SRCHANNEL_IF_HPP
#define	_SRCHANNEL_IF_HPP

#include <intndefs.hpp>
#include <time.h>
#include <socketC.hpp>
#include <scpformat.hpp>
#include "scp_msg.hpp"


#define	READTIME			500		// ����Read time-out
#define	SENDTIME			10		// ����Send time-out

#define	RECONNECT_WAIT_TIME	30		// ���������� �ٽ������ϱ����� ��ٸ��ð�
#define	RESEND_WAIT_TIME	10		// ACK ��������� �ٽ��ڷ������ϱ����� ��ٸ� �ð�

extern int	SystemId;
extern int	ServiceId;

uint	TransacNum=0;		// Download client
uint	STransacNum=0;		// Upload server
char	TDay[10];			// date
char	STDay[10];			// date
char 	PATH_NAME_SCPIFR_TID[80];	//	=   "/SVC#/DATA/SCPIF_TID.cfg";
char 	PATH_NAME_SCPIFS_TID[80];	//	=   "/SVC#/DATA/SCPIF_TID.cfg";

//--------------------------------------------------------------------
class SRchannel_IF {

	private:
						
		int				channel;		// S,R ä�� ����
		char			strBuf[2048];	// SCP�� ���͹��� �޽����� �����ͺκ�
		SockC			*c_Sock;
		ScpHeader		mHeaderRcv;		// SCP�� ���͹��� �޽����� �ش��κ�
		ScpMsg			msgC;			// �޽��������� ���� Ŭ����
		
		
		int 	RecvMsg2Server(void);	// SCP���� �޽����� �޴´�.
		void	SetTDay(int op);		// TDay, STDay �� �ý��� ��¥�� Setting
		
	public:
		int				ErrTID;			// 1. error(AGG�� �������� �ʴ´�)  0.����.
		
		SRchannel_IF(char *address, int port, int ch);	// SCP �ּ�, ��Ʈ��ȣ, ä�α���
		~SRchannel_IF(void);
		
		void	SockClose(void);		
		int 	DoConnection(void);		// ���ʿ���õ�
		int 	DoDisConnection(void);  // ���� ����
		int		ProcessCHK(void);		// S-Channel ������ ���μ�������Ȯ��
		
		
		//	SCP�� DownLoad������ ������ ACK�� �޴´�. 
		// 	SCPFormat : Agg���� MQ�� ���� ���� ����
		int 	DownLoad(SCPFormat* OneRecord);
		
		//	SCP�� DownLoadResult������ ������ ACK�� �޴´�. 
		// 	SCPFormat : ������������ MQ�� ���� ���� ���� (�����Ȯ���Ǹ� ������)
		// int 	UploadResult(ReCustFormat* OneRecord);	
		
		int		SendMsg2Server(int msg, int st=SC);	//SCP�� DownLoad������ ������ �޽����� ACK�� ������.
		int 	RecvMsg2Rchannel(void);				//SCP R-Channel�� ���� �޽�������
		
		// ������ �ڷḦ MQ ����ü �������� ��ȯ�Ѵ�.(Filter�� ����)
		int  	Get_MQMsg(UdrFormat* uformat) { return msgC.Get_MQMsg(channel, uformat); }	
		//void  	Get_MQMsg(CustFormat* fmSCP) { msgC.Get_MQMsg(channel, fmSCP); }	
		
};



//--------------------------------------------------------------------
void SRchannel_IF::SetTDay(int op)
{
	char		imsi[10];
	time_t		Time;	
	struct  tm *Tm;
	
	time(&Time);	
	Tm = localtime(&Time);	
	sprintf(imsi, "%04d%02d%02d",Tm->tm_year+1900, Tm->tm_mon+1, Tm->tm_mday);
	
	if(op = SCHANNEL) {
		memset(TDay, 0x00, sizeof(TDay));
		memmove(TDay, imsi, 8);
	}else if(op = RCHANNEL) {
		memset(STDay, 0x00, sizeof(STDay));
		memmove(STDay, imsi, 8);
	}
}


//--------------------------------------------------------------------
SRchannel_IF::SRchannel_IF(char *address, int port, int ch)
{
	channel = ch;
	memset(strBuf, 0x00, sizeof(strBuf));
	TransacNum=0;
	c_Sock = new SockC(address, port);
	
	
	//-----------------------------
	sprintf(PATH_NAME_SCPIFS_TID, "/SVC%d/DATA/SCPIFS_TID.cfg", ServiceId);
	sprintf(PATH_NAME_SCPIFR_TID, "/SVC%d/DATA/SCPIFR_TID.cfg", ServiceId);
	
	FILE *fp;
	
	if(ch==SCHANNEL)
	{
		if(!(fp = fopen(PATH_NAME_SCPIFS_TID, "r+b"))) 
		{
			*LogM << "FILE READ FAIL : " << PATH_NAME_SCPIFS_TID << " : " << LogM->ErrorCode(0) << endl;	
			SetTDay(ch);
			TransacNum=0;
		}else
		{
			fread(TDay, 10, 1, fp);
			fread((void*)&TransacNum, 4, 1, fp);
			fclose(fp);
			*LogM << "[READ]: " <<  PATH_NAME_SCPIFS_TID << " : " << TransacNum << " : " <<LogM->ErrorCode(0) << endl;	
			cout << PATH_NAME_SCPIFS_TID << " : " << TransacNum << " : " << endl;	
		}
	}else
	{
		if(!(fp = fopen(PATH_NAME_SCPIFR_TID, "r+b"))) 
		{
			*LogM << "FILE READ FAIL : " << PATH_NAME_SCPIFR_TID << " : " << LogM->ErrorCode(0) << endl;	
			SetTDay(ch);
			STransacNum=0;
		}else
		{
			fread(STDay, 10, 1, fp);
			fread((void*)&STransacNum, 4, 1, fp);
			fclose(fp);
			*LogM <<"[READ]: "<< PATH_NAME_SCPIFR_TID << " : " << STransacNum << " : " <<LogM->ErrorCode(0) << endl;	
			cout << PATH_NAME_SCPIFR_TID << " : " << STransacNum << " : "  << endl;	
		}
	}
	//-----------------------------
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
	
	if(channel==SCHANNEL) 
	{
		cout << "[SCHANNEL] Try connectting to server......" << endl;	
		*LogM << "[SCHANNEL] Try connectting to server...... :" << LogM->ErrorCode(0) << endl;	
	}else 
	{
		cout << "[RCHANNEL] Try connectting to server......" << endl;	
		*LogM << "[RCHANNEL] Try connectting to server...... :" << LogM->ErrorCode(0) << endl;	
	}
	
	if(c_Sock->CreateTCPClient()<0) return FA;
	if(channel==SCHANNEL) 
	{
		cout << "[SCHANNEL] Connection success!" << endl;
		*LogM << "[SCHANNEL] Connection success!" << LogM->ErrorCode(0) << endl;	
	}else 
	{
		cout << "[RCHANNEL] Try connectting to success!" << endl;	
		*LogM << "[RCHANNEL] Try connectting to success! :" << LogM->ErrorCode(0) << endl;	
	}

	
	if(channel==SCHANNEL)
	{
		while(i--){
			if(SendMsg2Server(S_CONNECT_REQ)==SC){
				printf("[SCHANNEL] (S_CONNECT_REQ) Success! \n");
				*LogM << "[SCHANNEL] (S_CONNECT_REQ) Success! :" << LogM->ErrorCode(0) << endl;	
				break;
			}else {
				printf("[SCHANNEL] (S_CONNECT_REQ) Fail! : Waiting \n");
				*LogM << "[SCHANNEL] (S_CONNECT_REQ) Fail! : Waiting :" << LogM->ErrorCode(0) << endl;	
				sleep(RESEND_WAIT_TIME);
			}			
		}
	}else
	{
		while(i--){
			if(SendMsg2Server(R_CONNECT_REQ)==SC){
				printf("[RCHANNEL] (R_CONNECT_REQ) Success! \n");
				*LogM << "[RCHANNEL] (R_CONNECT_REQ) Success! :" << LogM->ErrorCode(0) << endl;	
				break;
			}else {
				printf("[RCHANNEL] (R_CONNECT_REQ) Fail! : Waiting \n");
				*LogM << "[RCHANNEL] (R_CONNECT_REQ) Fail! : Waiting :" << LogM->ErrorCode(0) << endl;	
				sleep(RESEND_WAIT_TIME);
			}			
		}				
		
	}
			
	if(i==-1) 
	{
		if(channel==SCHANNEL) *LogM << "[SCHANNEL] Connectting Fail SockClose : " << LogM->ErrorCode(8210) << endl;	
		else *LogM << "[RCHANNEL] Connectting Fail SockClose : " << LogM->ErrorCode(8211) << endl;	
		
		SockClose();
		return FA;
	}
	
	return(SC);
}



//--------------------------------------------------------------------
int SRchannel_IF::DoDisConnection(void)
{
	char	fname[100];
	FILE	*fp;
	
	if(channel==SCHANNEL)
	{
		//----- STransacNum (Server���� ������ ������ ó����ȣ ID ����)
		if(!(fp = fopen(PATH_NAME_SCPIFS_TID, "w+b")))
		{
			*LogM << "[RCHANNEL] FILE WRITE FAIL : " << PATH_NAME_SCPIFS_TID << " : " << LogM->ErrorCode(0) << endl;	
		}else
		{
			fwrite(TDay, 1, 10, fp);
			fwrite((void*)&TransacNum, 1, 4, fp);
			fclose(fp);
			*LogM << "[WRITE]:" << PATH_NAME_SCPIFS_TID << " : " << TransacNum << " : " <<LogM->ErrorCode(0) << endl;	
			cout << PATH_NAME_SCPIFS_TID << " : " << TransacNum << " : " << endl;	
		}
		//----------------------------------------------------------
		
		if(SendMsg2Server(S_RELEASE_REQ)==SC)
		{
			printf("[SCHANNEL] (S_RELEASE_REQ) Success! \n");
			*LogM << "[SCHANNEL] (S_RELEASE_REQ) Success! : " << LogM->ErrorCode(0) << endl;	
			return SC;
		}else 
		{
			printf("[SCHANNEL] (S_RELEASE_REQ) Fail! \n");
			*LogM << "[SCHANNEL] (S_RELEASE_REQ) Fail!  : " << LogM->ErrorCode(0) << endl;	
			return FA;
		}
		
	}else
	{
		
		//----- STransacNum (Server���� ������ ������ ó����ȣ ID ����)
		if(!(fp = fopen(PATH_NAME_SCPIFR_TID, "w+b"))) 
		{
			*LogM << "[RCHANNEL] FILE WRITE FAIL : " << PATH_NAME_SCPIFR_TID << " : " << LogM->ErrorCode(0) << endl;	
		}else
		{
			fwrite(STDay, 1, 10, fp);
			fwrite((void*)&STransacNum, 1, 4, fp);
			fclose(fp);
			*LogM << "[WRITE]:" <<  PATH_NAME_SCPIFR_TID << " : " << STransacNum << " : " <<LogM->ErrorCode(0) << endl;	
			cout << PATH_NAME_SCPIFR_TID << " : " << STransacNum << " : " << endl;	
		}
		//----------------------------------------------------------
		
		if(SendMsg2Server(R_RELEASE_REQ)==SC)
		{
			printf("[RCHANNEL] (R_RELEASE_REQ) Success! \n");
			*LogM << "[RCHANNEL] (R_RELEASE_REQ) Success! : " << LogM->ErrorCode(0) << endl;	
			return SC;
		}else 
		{
			printf("[RCHANNEL] (R_RELEASE_REQ) Fail! \n");
			*LogM << "[RCHANNEL] (R_RELEASE_REQ) Fail! : " << LogM->ErrorCode(0) << endl;	
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
			*LogM << "[SCHANNEL] (PROCESS_STATE_REQ) Success! : " << LogM->ErrorCode(0) << endl;	
			
			break;
		}else {
			printf("[SCHANNEL] (PROCESS_STATE_REQ) Fail! : Waiting 10s\n");
			*LogM << "[SCHANNEL] (PROCESS_STATE_REQ) Fail! : Waiting 10s : " << LogM->ErrorCode(0) << endl;	
			sleep(RESEND_WAIT_TIME);
		}			
	}
	
	if(i==-1) 
	{
		*LogM << "[SCHANNEL] PROCESS_STATE_REQ Fail SockClose : " << LogM->ErrorCode(8212) << endl;	
		SockClose();
		return FA;
	}
	
	return(SC);
}


//--------------------------------------------------------------------
//Agg�� ���� ���� ���������� SCP�� ������ �ѵ������� ��û�Ѵ�. (SCHANNEL-Only)
int SRchannel_IF::DownLoad(SCPFormat* OneRecord)
{
	int i=3;
	
	while(i--)
	{
		//���ɾ�ó�� �䱸(S-Channel)
		printf("[SCHANNEL] Send2Msg : DOWNLOAD_REQ \n");
		*LogM << "[SCHANNEL] Send2Msg : DOWNLOAD_REQ : " << LogM->ErrorCode(0) << endl;	
		
		msgC.MK_Msg(DOWNLOAD_REQ, OneRecord);
		
		c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
		if(RecvMsg2Server()!=SC) 
		{
			sleep(RESEND_WAIT_TIME);
			continue;
		}
	
		if(mHeaderRcv.MessageId==S_RELEASE_REQ)
		{//ACK�� �����ʰ� ����޽����� �߻��ϸ�...
			printf("[SCHANNEL] Send2Msg : S_RELEASE_REQ_ACK \n");
			*LogM << "[SCHANNEL] Send2Msg : S_RELEASE_REQ_ACK : " << LogM->ErrorCode(0) << endl;	
			msgC.MK_Msg(S_RELEASE_REQ_ACK);			
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
			SockClose();
			return RELEASE_CLOSE; // ó����� �ʿ�
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


/*
//--------------------------------------------------------------------
//���������� ���� ���� ó������� �����Ѵ�. 
int SRchannel_IF::UploadResult(ReCustFormat* OneRecord)
{
	int i=3;
	
	while(i--)
	{
		//���ɾ�ó�� �䱸(S-Channel)
		printf("[SCHANNEL] Send2Msg : UPLOAD_RESULT \n");
		*LogM << "[SCHANNEL] Send2Msg : UPLOAD_RESULT : " << LogM->ErrorCode(0) << endl;	
		msgC.MK_Msg(UPLOAD_RESULT, OneRecord);
		
		c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
		if(RecvMsg2Server()!=SC) 
		{
			sleep(RESEND_WAIT_TIME);
			continue;
		}
	
		if(mHeaderRcv.MessageId==S_RELEASE_REQ)
		{//ACK�� �����ʰ� ����޽����� �߻��ϸ�...
			printf("[SCHANNEL] Send2Msg : S_RELEASE_REQ_ACK \n");
			*LogM << "[SCHANNEL] Send2Msg : S_RELEASE_REQ_ACK : " << LogM->ErrorCode(0) << endl;	
			msgC.MK_Msg(S_RELEASE_REQ_ACK);			
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
			SockClose();
			return RELEASE_CLOSE; // ó����� �ʿ�
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
			*LogM << "[RCHANNEL] Recv2Msg : DOWNLOAD_RESULT : " << LogM->ErrorCode(0) << endl;	
			if(strncmp(STDay, mHeaderRcv.TransacId.Day, 8)>=0 && (STransacNum == mHeaderRcv.TransacId.Num))
			{
				ErrTID = 1;	// AGG�� �Ⱥ�����
				printf("[RCHANNEL] Recv2Msg : DOWNLOAD_RESULT ERROR TID [%d] \n", mHeaderRcv.TransacId.Num);
				*LogM << "[RCHANNEL] Recv2Msg : DOWNLOAD_RESULT ERROR TID : " << mHeaderRcv.TransacId.Num << LogM->ErrorCode(0) << endl;	
			}else if(strncmp(STDay, mHeaderRcv.TransacId.Day, 8)>=0 && (STransacNum > mHeaderRcv.TransacId.Num))
			{
				ErrTID = 2; // AGG�� �Ⱥ����� ���� ����
				
			}else
			{
				ErrTID = 0;
				memset(STDay, 0x00, sizeof(STDay));
				memcpy(STDay, mHeaderRcv.TransacId.Day, 8);
				STransacNum = mHeaderRcv.TransacId.Num;		
			}
			break;
		case UPLOAD_REQ:
			printf("[RCHANNEL] Recv2Msg : UPLOAD_REQ \n");
			*LogM << "[RCHANNEL] Recv2Msg : UPLOAD_REQ : " << LogM->ErrorCode(0) << endl;
			memset(STDay, 0x00, sizeof(STDay));
			memcpy(STDay, mHeaderRcv.TransacId.Day, 8);
			STransacNum = mHeaderRcv.TransacId.Num;	
			break;
		case UPLOAD_RESULT:
			printf("[RCHANNEL] Recv2Msg : UPLOAD_RESULT \n");
			*LogM << "[RCHANNEL] Recv2Msg : UPLOAD_RESULT " << LogM->ErrorCode(0) << endl;
			break;
		case RCHANNEL_RELEASE_REQ:
			printf("[RCHANNEL] Recv2Msg : RCHANNEL_RELEASE_REQ \n");
			*LogM << "[RCHANNEL] Recv2Msg : RCHANNEL_RELEASE_REQ " << LogM->ErrorCode(0) << endl;
			break;
		case RCHANNEL_RELEASE_REQ_ACK:
			printf("[RCHANNEL] Recv2Msg : RCHANNEL_RELEASE_REQ_ACK \n");
			*LogM << "[RCHANNEL] Recv2Msg : RCHANNEL_RELEASE_REQ_ACK " << LogM->ErrorCode(0) << endl;
			break;									
		default:
			printf("[RCHANNEL] Recv2Msg : Error MessageId [%d] \n", mHeaderRcv.MessageId);
			*LogM << "[RCHANNEL] Recv2Msg : Error MessageId : " << mHeaderRcv.MessageId 
				<< " : " << LogM->ErrorCode(8250) << endl;
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
			*LogM << "[SCHANNEL] Recv2Msg : S_CONNECT_REQ_ACK :" << LogM->ErrorCode(0) << endl;
			
			memset(TDay, 0x00, sizeof(TDay));
			memcpy(TDay, mHeaderRcv.TransacId.Day, 8);
			TransacNum = mHeaderRcv.TransacId.Num;
			printf("[SCHANNEL] SCONNECT_REQ_ACK : [%s|%d] \n", TDay, TransacNum);
			*LogM << "[SCHANNEL] TDay = " << TDay << " | TransacNum = " << TransacNum << LogM->ErrorCode(0) << endl;
			break;
		case R_CONNECT_REQ_ACK:
			printf("[RCHANNEL] Recv2Msg : R_CONNECT_REQ_ACK \n");
			*LogM << "[RCHANNEL] Recv2Msg : R_CONNECT_REQ_ACK : " << LogM->ErrorCode(0) << endl;
			break;
		case S_RELEASE_REQ:
			printf("[SCHANNEL] Recv2Msg : S_RELEASE_REQ \n");
			*LogM << "[SCHANNEL] Recv2Msg : S_RELEASE_REQ  : " << LogM->ErrorCode(0) << endl;
			break;
		case S_RELEASE_REQ_ACK:
			printf("[SCHANNEL] Recv2Msg : S_RELEASE_REQ_ACK \n");
			*LogM << "[SCHANNEL] Recv2Msg : S_RELEASE_REQ_ACK  : " << LogM->ErrorCode(0) << endl;
			break;			
		case R_RELEASE_REQ:
			printf("[RCHANNEL] Recv2Msg : R_RELEASE_REQ \n");
			*LogM << "[RCHANNEL] Recv2Msg : R_RELEASE_REQ  : " << LogM->ErrorCode(0) << endl;
			break;
		case R_RELEASE_REQ_ACK:
			printf("[RCHANNEL] Recv2Msg : R_RELEASE_REQ_ACK \n");
			*LogM << "[RCHANNEL] Recv2Msg : R_RELEASE_REQ_ACK  : " << LogM->ErrorCode(0) << endl;
			break;			
		case PROCESS_STATE_REQ_ACK:
			printf("[SCHANNEL] Recv2Msg : PROCESS_STATE_REQ_ACK \n");
			*LogM << "[SCHANNEL] Recv2Msg : PROCESS_STATE_REQ_ACK  : " << LogM->ErrorCode(0) << endl;
			break;		
		case DOWNLOAD_REQ_ACK:
			printf("[SCHANNEL] Recv2Msg : DOWNLOAD_REQ_ACK \n");
			*LogM << "[SCHANNEL] Recv2Msg : DOWNLOAD_REQ_ACK  : " << LogM->ErrorCode(0) << endl;
			break;
		/*	
		case DOWNLOAD_RESULT:
			printf("[SCHANNEL] Recv2Msg : DOWNLOAD_RESULT \n");
			*LogM << "[SCHANNEL] Recv2Msg : DOWNLOAD_RESULT  : " << LogM->ErrorCode(0) << endl;
			break;
		*/
		case UPLOAD_RESULT_ACK:
			printf("[SCHANNEL] Recv2Msg : UPLOAD_RESULT_ACK \n");		
			*LogM << "[SCHANNEL] Recv2Msg : UPLOAD_RESULT_ACK  : " << LogM->ErrorCode(0) << endl;
			break;	
		default:
			printf("[#ERROR##] SRchannel_IF::RecvMsg2Server(void) [%d] \n", mHeaderRcv.MessageId);
			*LogM << "[SCHANNEL] SRchannel_IF::RecvMsg2Server(void)  : ERRMSGID : " 
				  << mHeaderRcv.MessageId << " : " << LogM->ErrorCode(0) << endl;
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
			*LogM << "[SCHANNEL] Send2Msg : S_CONNECT_REQ : " << LogM->ErrorCode(0) << endl;
			msgC.MK_Msg(S_CONNECT_REQ);
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
			if((re=RecvMsg2Server())!=SC) return re;
			return msgC.CHK_Msg(S_CONNECT_REQ_ACK);
		
		
		case R_CONNECT_REQ:
			printf("[RCHANNEL] Send2Msg : R_CONNECT_REQ \n");
			*LogM << "[RCHANNEL] Send2Msg : R_CONNECT_REQ : " << LogM->ErrorCode(0) << endl;
			msgC.MK_Msg(R_CONNECT_REQ);
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
			if((re=RecvMsg2Server())!=SC) return re;
			return msgC.CHK_Msg(R_CONNECT_REQ_ACK);			
					
			
		case S_RELEASE_REQ:
			printf("[SCHANNEL] Send2Msg : S_RELEASE_REQ \n");
			*LogM << "[SCHANNEL] Send2Msg : S_RELEASE_REQ : " << LogM->ErrorCode(0) << endl;
			msgC.MK_Msg(S_RELEASE_REQ);
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
			if((re=RecvMsg2Server())!=SC) return re;
			return msgC.CHK_Msg(S_RELEASE_REQ_ACK);			

		case R_RELEASE_REQ:
			printf("[RCHANNEL] Send2Msg : R_RELEASE_REQ \n");
			*LogM << "[RCHANNEL] Send2Msg : R_RELEASE_REQ : " << LogM->ErrorCode(0) << endl;
			msgC.MK_Msg(R_RELEASE_REQ);
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
			if((re=RecvMsg2Server())!=SC) return re;
			return msgC.CHK_Msg(R_RELEASE_REQ_ACK);						
			
		case PROCESS_STATE_REQ:
			printf("[SCHANNEL] Send2Msg : PROCESS_STATE_REQ \n");
			*LogM << "[SCHANNEL] Send2Msg : PROCESS_STATE_REQ : " << LogM->ErrorCode(0) << endl;
			msgC.MK_Msg(PROCESS_STATE_REQ);
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(SCHANNEL), msgC.SendLen(SCHANNEL), 0, SENDTIME);
			if((re=RecvMsg2Server())!=SC) return re;
			return msgC.CHK_Msg(PROCESS_STATE_REQ_ACK);		
		
		case DOWNLOAD_RESULT_ACK:
			printf("[RCHANNEL] Send2Msg : DOWNLOAD_RESULT_ACK \n");
			*LogM << "[RCHANNEL] Send2Msg : DOWNLOAD_RESULT_ACK : " << LogM->ErrorCode(0) << endl;
			msgC.MK_Msg(DOWNLOAD_RESULT_ACK);
			if(st==FA) msgC.MK_MsgFA(RCHANNEL, 2);	
			
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
			return SC;
					
		case UPLOAD_RESULT_ACK:
			printf("[RCHANNEL] Send2Msg : UPLOAD_RESULT_ACK \n");
			*LogM << "[RCHANNEL] Send2Msg : UPLOAD_RESULT_ACK : " << LogM->ErrorCode(0) << endl;
			msgC.MK_Msg(UPLOAD_RESULT_ACK);
			if(st==FA) msgC.MK_MsgFA(RCHANNEL, 2);	
			
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
			return SC;
			
		case UPLOAD_REQ_ACK:
			printf("[RCHANNEL] Send2Msg : UPLOAD_REQ_ACK \n");
			*LogM << "[RCHANNEL] Send2Msg : UPLOAD_REQ_ACK : " << LogM->ErrorCode(0) << endl;
			msgC.MK_Msg(UPLOAD_REQ_ACK);
			if(st==FA) msgC.MK_MsgFA(RCHANNEL, 2);	
			
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
			return SC;
			
		case RCHANNEL_RELEASE_REQ_ACK:			
			printf("[RCHANNEL] Send2Msg : RCHANNEL_RELEASE_REQ_ACK \n");
			*LogM << "[RCHANNEL] Send2Msg : RCHANNEL_RELEASE_REQ_ACK : " << LogM->ErrorCode(0) << endl;
			msgC.MK_Msg(RCHANNEL_RELEASE_REQ_ACK);
			c_Sock->SendSocketTimeoutEx(msgC.GetMsgBuf(RCHANNEL), msgC.SendLen(RCHANNEL), 0, SENDTIME);
			return SC;
		
		default:
			printf("[##ERROR##] SRchannel_IF::SendMsg2Server(int msg, int st) \n");
			*LogM << "SRchannel_IF::SendMsg2Server(int msg, int st) : ERRORMSG : " 
				  << msg  << " : " << LogM->ErrorCode(8251) << endl;
			sleep(5);
			return SC;
	}
}




#endif

