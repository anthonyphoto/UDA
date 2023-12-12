#include <stdio.h>
#include <string.h>


const int       LEN_DATE    = 20;
const int       LEN_TIME    = 20;


enum MSG_TYPE 		{	MSG_STATUS, 		//	Msg Type : To PMS
						MSG_LOG,			//	Msg Log
						MSG_ERR,			//	Msg Error
						MSG_STATS_PERFORM,	//	Msg Perform
						MSG_STATS_EVENT,		//	Msg Event
						MSG_STATS_ERROR		//	Msg Stats of Error
					};

enum FLAG_STATUS 	{	FLAG_NW_NORMAL = -4,
						FLAG_NW_ERROR,
						FLAG_PROC_ABNORMAL,
						FLAG_PROC_KILL,
						FLAG_PROC_TERM,
						FLAG_PROC_START
					};
					
enum FLAG_PERFORM	{	FLAG_START,
						FLAG_END,
						FLAG_ACC
					};
					
					
//-------------------------------------------------------------------------------------
class	HeaderOfMessage
{
	private:
		int		nMsgType;		//  Message Type
		int		nSystemId;		//	SYSTEM ID
		int		nSvcId;			//	SERVICE ID
		int		nGrpType;		//	GROUP ID
		int		nProcType;		//	PROCESS TYPE
		int		nProcSerial;	//	PROCESS SERIAL NUMBER
		int		nProcRemark;	//	PROCESS REMARK
		char	strDate[LEN_DATE+1];
		char	strTime[LEN_TIME+1];

	public:
		HeaderOfMessage()
		{
			nMsgType	= 0;
    		nSystemId	= 0;
    		nSvcId		= 0;
    		nGrpType	= 0;
    		nProcType	= 0;
    		nProcSerial	= 0;
    		nProcRemark	= 0;
		}
		
		HeaderOfMessage	(	int		MsgType,		//  Message Type
							int		SystemId,		//	SYSTEM ID
							int		SvcId,			//	SERVICE ID
							int		GrpType,		//	GROUP ID
							int		ProcType,		//	PROCESS TYPE
							int		ProcSerial,		//	PROCESS SERIAL NUMBER
							int		ProcRemark		//	PROCESS REMARK
						)
			{
			;	
			}
						
};

/*
//-------------------------------------------------------------------------------------
class	MsgPmsStatus	: public HeaderOfMessage
{
	private:
		int		nPID;
		int		nStatus;

	public:
		MsgPmsStatus	(	int		MsgType,		//  Message Type
							int		SystemId,		//	SYSTEM ID
							int		SvcId,			//	SERVICE ID
							int		GrpType,		//	GROUP ID
							int		ProcType,		//	PROCESS TYPE
							int		ProcSerial,		//	PROCESS SERIAL NUMBER
							int		ProcRemark		//	PROCESS REMARK
						)
		:	HeaderOfMessage	(		MsgType,
									SystemId,
									SvcId,
									GrpType,
									ProcType,
									ProcSerial,
									ProcRemark
							)
		{
			;//nPID = (int)getpid();
		}

		int		get_nStatus	()				{	return nStatus;	}
		int		get_nPID	()				{	return nPID;	}
};
*/
class	MsgLog	: public HeaderOfMessage
{
	private:
		int		nErrorCode;
		char	strMsg[256];

	public:
		MsgLog			(	int		MsgType,				//  Message Type
							int		SystemId,				//	SYSTEM ID
							int		SvcId,					//	SERVICE ID
							int		GrpType,				//	GROUP ID
							int		ProcType,				//	PROCESS TYPE
							int		ProcSerial,				//	PROCESS SERIAL NUMBER
							int		ProcRemark,				//	PROCESS REMARK
							int		nStrLength = 256		//	Length of Log Message
						)
		:	HeaderOfMessage	(		MsgType,
									SystemId,
									SvcId,
									GrpType,
									ProcType,
									ProcSerial,
									ProcRemark
							)
		{
			nErrorCode = 0;
			memset(strMsg, NULL, nStrLength+1);
		}

};


//-------------------------------------------------------------------------------------
class	ProcPerformData	: public HeaderOfMessage
{
	private:
		int				nBillDataType;
		unsigned int	unFileSeq;
		char			strProcId[9];
		unsigned int	nUdrCount;
		unsigned int	nFailUdrCount;						//smy 2000.11.25
		unsigned int	nQuery_Count;
		char			strStartDateTime[15];
		char			strEndDateTime[15];

	public:
		ProcPerformData(){;}
		ProcPerformData(	int		SystemId,				//	SYSTEM ID
							int		SvcId,					//	SERVICE ID
							int		GrpType,				//	GROUP ID
							int		ProcType,				//	PROCESS TYPE
							int		ProcSerial,				//	PROCESS SERIAL NUMBER
							int		ProcRemark				//	PROCESS REMARK
						)
			:	HeaderOfMessage	(	MSG_STATS_PERFORM,
									SystemId,
									SvcId,
									GrpType,
									ProcType,
									ProcSerial,
									ProcRemark
							)
		{
			;
		}

};

 
//-------------------------------------------------------------------------------------
class	ProcEventData	: public HeaderOfMessage
{
	private:
		char	strEventDateTime[15];
		int		nExitCode;

	public:
		ProcEventData	(	int		SystemId,		//	SYSTEM ID
							int		SvcId,			//	SERVICE ID
							int		GrpType,		//	GROUP ID
							int		ProcType,		//	PROCESS TYPE
							int		ProcSerial,		//	PROCESS SERIAL NUMBER
							int		ProcRemark		//	PROCESS REMARK
						)
		:	HeaderOfMessage	(		MSG_STATS_EVENT,
									SystemId,
									SvcId,
									GrpType,
									ProcType,
									ProcSerial,
									ProcRemark
							)
		{
		}

		//	Get Method
		char*	get_strEventDateTime(void)		{	return	strEventDateTime;	}
		int		get_nExitCode		(void)		{	return	nExitCode		;	}

};


//-------------------------------------------------------------------------------------
class	ProcErrInfo	: public HeaderOfMessage
{
	private:
		char	strDayTime[30];
		char	strEventDate[9];
		char	strEventTime[7];
		char	strErrCode[5];
		char	strFileName[40];
		char	strErrDetail[512];

	public:
		ProcErrInfo	(	int		SystemId,		//	SYSTEM ID
						int		SvcId,			//	SERVICE ID
						int		GrpType,		//	GROUP ID
						int		ProcType,		//	PROCESS TYPE
						int		ProcSerial,		//	PROCESS SERIAL NUMBER
						int		ProcRemark		//	PROCESS REMARK
					)
		:	HeaderOfMessage	(		MSG_STATS_ERROR,
									SystemId,
									SvcId,
									GrpType,
									ProcType,
									ProcSerial,
									ProcRemark
							)
		{
			memset(strEventDate, NULL,	9);
			memset(strEventTime, NULL,	7);
			memset(strFileName , NULL,	40);
			memset(strErrDetail, NULL,	512);
		}

};


int main(void)
{
	
	ProcPerformData	 	Pdata;
	ProcEventData		PEvent(1,1,1,1,1,1);
	ProcErrInfo			PErr(1,1,1,1,1,1);
	MsgLog				Msg(1,1,1,1,1,1,1);
	
	
	printf("ProcPerformData = %d\n", sizeof(ProcPerformData));
	printf("ProcEventData = %d\n", sizeof(ProcEventData));
	printf("ProcErrInfo = %d\n", sizeof(ProcErrInfo));
	printf("MsgLog = %d\n", sizeof(MsgLog));
	
}

