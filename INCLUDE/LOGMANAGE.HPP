#ifndef _LOGMANAGE_HPP
#define _LOGMANAGE_HPP

#include <iostream.h>
#include <iomanip.h>
#include <string.h>
#include <stdlib.h>
#include <strstream.h>
#include <SqlCommon.hpp>

#define	LOG_FILEPATH_LEN	32	

enum	LOG_TYPE	{	ERR_LOG_TYPE = 1,
						MSG_LOG_TYPE,
						SLA_LOG_TYPE,
						STATS_LOG_TYPE,
						PERFORM_LOG_TYPE,
						EVENT_LOG_TYPE
					};

//--------------------------------------------------------------------
class LogManageRec
{
	private:
		int		nSystemId;		//	SYSTEM ID
		int		nSvcId;			//	SERVICE ID
		int		nGrpType;		//	GROUP ID
		int		nProcType;		//	PROCESS TYPE
		int		nProcSerial;	//	PROCESS SERIAL NUMBER
		int		nProcRemark;	//	PROCESS REMARK
		
		int		nLogType;
		int		nLogSwGlag;
		int		nLogMdType;
		char	LogFilePath[LOG_FILEPATH_LEN+1];
		
	public:	
		LogManageRec(void)
		{
			nSystemId=0;	
			nSvcId=0;		
			nGrpType=0;	
			nProcType=0;	
			nProcSerial=0;
			nProcRemark=0;
			nLogType=0;   
			nLogSwGlag=0; 
			nLogMdType=0; 
			
			memset(LogFilePath, 0x00, sizeof(LogFilePath));	
		}   
		
		~LogManageRec(void){}
		
		    
		int 	get_nSystemId()		{	return   nSystemId	;	}
		int 	get_nSvcId()		{	return   nSvcId		;	}
		int 	get_nGrpType()		{	return   nGrpType	;	}
		int 	get_nProcType()		{	return   nProcType	;	}
		int 	get_nProcSerial()	{	return   nProcSerial;	}
		int 	get_nProcRemark()	{	return   nProcRemark;	}
		
		int		get_nLogType()		{	return   nLogType	;	}
		int		get_nLogSwGlag()	{	return   nLogSwGlag	;	}
		int		get_nLogMdType()	{	return   nLogMdType	;	}
		char*	get_LogFilePath()	{	return   LogFilePath;	}
		
		
		void 	put_nSystemId(int n)		{	nSystemId=n	;	}
		void 	put_nSvcId(int n)			{	nSvcId=n	;	}
		void 	put_nGrpType(int n)			{	nGrpType=n	;	}
		void 	put_nProcType(int n)		{	nProcType=n	;	}
		void	put_nProcSerial(int n)		{	nProcSerial=n;	}
		void 	put_nProcRemark(int n)		{	nProcRemark=n;	}
		
		void	put_nLogType(int n)			{	nLogType=n	;	}
		void	put_nLogSwGlag(int n)		{	nLogSwGlag=n;	}
		void	put_nLogMdType(int n)		{	nLogMdType=n;	}
		void	put_LogFilePath(char *str)	
				{	
					memset(LogFilePath, 0x00, sizeof(LogFilePath));	
					memcpy(LogFilePath, str, strlen(str));
				}
		
};


//--------------------------------------------------------------------
class LogManage	: public ProcessID
{
	private:
	/*
		int			nBillType;
		int			nFileSeq;
        char		ProcId[PROC_ID_LEN+1];
        int			nUdrCount;
        int			nQueryCount;
        char		SDate[DATE_LEN+1];
        char		EDate[DATE_LEN+1];
    */
		LogManageRec	LogRecord;		// T_LOG_MANAGE 테이블 레코드
		
		
		int			FetchLogDB(void);
	public:
        
        LogManage(	int		nLSystemId,					\
					int		nLServiceId,				\
					int		nLGroupId,					\
					int		nLProcessType,				\
					int		nLSerialNo,					\
					int		nLRemark					\
					)	: 	ProcessID 	(	nLSystemId,		\
											nLServiceId,	\
											nLGroupId,		\
											nLProcessType,	\
											nLSerialNo,		\
											nLRemark		\
										){}
				
		~LogManage(){}
										
		int 			Set_LogManage(int logtype, int logsw, int logmd, char *filepath);
		int				Run_FetchDB(int first=0);
		
		LogManageRec*	Get_LogRecord(void) 	{	return (&LogRecord);	}
		int				Set_LogRecord(LogManageRec *LogRecord);	 // 레코드 Update
		int				Put_LogRecord(LogManageRec *LogRecord);	 // 레코드 추가
		int				Del_LogRecord(LogManageRec *LogRecord);	 // 레코드 삭제
		
		int				Get_RecCount(void);
};



#endif

