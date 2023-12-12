#ifndef	MSGFORMAT_HPP
#define MSGFORMAT_HPP

#include <iostream.h>
#include <unistd.h>
#include <strstream.h>
#include <iomanip.h>

#ifndef	TIME_HPP
#include <time.hpp>
#endif



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
						);
						
		int 	get_nMsgType()		{	return   nMsgType	;	}
		int 	get_nSystemId()		{	return   nSystemId	;	}
		int 	get_nSvcId()		{	return   nSvcId		;	}
		int 	get_nGrpType()		{	return   nGrpType	;	}
		int 	get_nProcType()		{	return   nProcType	;	}
		int 	get_nProcSerial()	{	return   nProcSerial;	}
		int 	get_nProcRemark()	{	return   nProcRemark;	}
		char*	get_ProcessId();

		void	put_strDate();
		void	put_strTime();

		void displayHeader(void);
		
		// sycho 2000.11.24
		void	set_HeaderOfMessage(
							int		MsgType,		//  Message Type
							int		SystemId,		//	SYSTEM ID
							int		SvcId,			//	SERVICE ID
							int		GrpType,		//	GROUP ID
							int		ProcType,		//	PROCESS TYPE
							int		ProcSerial,		//	PROCESS SERIAL NUMBER
							int		ProcRemark		//	PROCESS REMARK
							)
		{
			nMsgType	= MsgType;
    		nSystemId	= SystemId;
    		nSvcId		= SvcId;
    		nGrpType	= GrpType;
    		nProcType	= ProcType;
    		nProcSerial	= ProcSerial;
    		nProcRemark	= ProcRemark;
		}
};


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
			nPID = (int)getpid();
		}

		void	set_nStatus	(int	nValue);
		int		get_nStatus	()				{	return nStatus;	}
		int		get_nPID	()				{	return nPID;	}
};

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

		int	 put_nErrorCode	(int nValue);		
		void put_strMsg		(const char* strValue, int nLen);
		void displayValue	(void);
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
		}

		void 			put_PerformData	(	int 		 nDataType,
											unsigned int unFileNo,
											unsigned int unUdrCnt,
											unsigned int unFailUdrCnt,		//smy 2000.11.25
											unsigned int nQueryCnt,
											int cnt
										);
		void 			put_strDateTime	(int nFlag, int cnt);

		//	Get Method
		int				get_nBillDataType	(void)		{	return nBillDataType;	}
		unsigned int	get_unFileSeq		(void)		{	return unFileSeq;		}
		char*			get_strProcId		(void)		{	return strProcId;		}
		unsigned int	get_nUdrCount		(void)		{	return nUdrCount;		}
		unsigned int	get_nFailUdrCount	(void)		{	return nFailUdrCount;	}	//smy 2000.11.25		
		unsigned int	get_nQuery_Count	(void)		{	return nQuery_Count;	}
		char*			get_strStartDateTime(void)		{	return strStartDateTime;}
		char*			get_strEndDateTime	(void)		{	return strEndDateTime;	}
		
		//smy 2000.11.25
		//Set Method
		void	set_ProcPerformData1()
		{
			nBillDataType	= 0;
			unFileSeq		= 0;
			nUdrCount		= 0;
			nFailUdrCount	= 0;			
			nQuery_Count	= 0;

		}
		
		void	set_ProcPerformData2(int 			nDataType,
									unsigned int	unFileNo,
									unsigned int	unUdrCnt,
									unsigned int	unFailUdrCnt,
									unsigned int	nQueryCnt
									)
		{
			nBillDataType	= nDataType;
			unFileSeq		= unFileNo;
			nUdrCount		= unUdrCnt;
			nFailUdrCount	= unFailUdrCnt;			
			nQuery_Count	= nQueryCnt;

		}
		
		
		
		// sycho 2000.11.24
		void	set_ProcPerformData(
							int		SystemId,				//	SYSTEM ID
							int		SvcId,					//	SERVICE ID
							int		GrpType,				//	GROUP ID
							int		ProcType,				//	PROCESS TYPE
							int		ProcSerial,				//	PROCESS SERIAL NUMBER
							int		ProcRemark				//	PROCESS REMARK
							)
		{
			set_HeaderOfMessage(MSG_STATS_PERFORM,SystemId,SvcId,GrpType,ProcType,ProcSerial,ProcRemark);
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

		void	put_ProcEventData	(int nExitValue);	//	Start Date & Time Setting
		void 	put_strEventDateTime(void);
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

		void	put_ProcErrInfo	(int nError, char* strErrMsg);
		void	put_strFileName	();
		int 	put_nErrorCode	(int nValue);
		int		write_ErrorLog	();
		
		//	Get Method
		char*	get_strEventDate(void)		{	return	strEventDate;	}
		char*	get_strEventTime(void)		{	return	strEventTime;	}
		char*	get_nErrCode	(void)		{	return	strErrCode	;	}
		char*	get_strFileName	(void)		{	return	strFileName	;	}
		char*	get_strErrDetail(void)		{	return	strErrDetail;	}
};


#endif

