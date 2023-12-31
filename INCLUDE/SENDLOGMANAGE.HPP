#ifndef	LOGMANAGEMENT_HPP
#define	LOGMANAGEMENT_HPP

#include <iostream.h>
#include <strstream.h>
#include <fstream.h>
#include <string.h>
#include <unistd.h>
#include <fstream.h>

#ifndef TEMPMSGQ_HPP
//	#include <TempMsgQ_new.hpp>
	#include <TempMsgQ_file.hpp>
#endif

#ifndef MSGFORMAT_HPP
	#include <MsgFormat.hpp>
#endif

#ifndef _LOGMANAGESM_HPP
	#include <LogManageSM.hpp>
#endif



//	Log Manager에서 Error Log Display를 위해 사용
typedef	struct
{
	int		nErrCode;
}err_t;
 
class SendLogManage	:	public HeaderOfMessage
{
	private:
		MsgLog*							clsMsgLog;
		ProcPerformData*				clsPerForm;
		ProcEventData*					clsEventHist;
		ProcErrInfo*					clsErrLog;

		MessageQueue<MsgLog>*			LogQ;
		MessageQueue<ProcPerformData>*	performMQ;
		MessageQueue<ProcEventData>*	eventHstMQ;
		MessageQueue<ProcErrInfo>*		errorLogMQ;

		LogManageSM*					clsLogMgrSM;

		ostrstream*    					m_ostring;
		ofstream*						fLogFile;


		void 							InitBuf(char *Buf,long Cnt);
		void 							GetHexDump(char *RBuf, const char *Buf,long BufLen);
	public:
		SendLogManage	(	int		nSystemId,
							int		nServiceId,
							int		nGroupId,
							int		nProcessType,
							int		nSerialNo,
							int		nRemark
						);
		~SendLogManage();

		SendLogManage& operator<<(const char* str);
		SendLogManage& operator<<(int n);
		SendLogManage& operator<<(err_t n);
		SendLogManage& operator<<(unsigned int n);
		SendLogManage& operator<<(char c);
		SendLogManage& operator<<(short s);
		SendLogManage& operator<<(long l);
		SendLogManage& operator<<(float f);
		SendLogManage& operator<<(double d);
		SendLogManage& operator<<(unsigned char c);
		SendLogManage& operator<<(unsigned short s);
		SendLogManage& operator<<(unsigned long l);
		SendLogManage& operator<<(void* v);
		SendLogManage& operator<<(ostream&(*)(ostream&));

		const char* get_string(void);
		int			get_StringLength(void);
		int			writeLog();

		//	Error Message의 Error Code Setting에 사용
		err_t		ErrorCode(int	nValue);


		//	Performance Data MQ 전송에 사용
		void		put_PerformanceData	(	int			 nFlag,
											int 		 nDataType,
											unsigned int unFileNo,
											unsigned int unUdrCnt,
											unsigned int unFailUdrCnt,		//smy 2000.11.25
											unsigned int nQueryCnt
										);
		void		put_EventHistData	(	int			 nFlag,
											int			 nExitCode
										);


		int			WriteHexLog(char *Buf,	long BufLen);
};

#endif 
