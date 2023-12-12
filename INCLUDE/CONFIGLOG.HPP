#ifndef	CONFIGLOG_HPP
#define CONFIGLOG_HPP

#include <iostream.h>
#include <fstream.h>
#include <ctype.h>
#include <sys/stat.h>
//
#include "SharedMemLegacy.hpp"
#include "time.hpp"

const int				LEN_PATH_NAME		= 128;
const int				LEN_FILE_NAME		= 64;

const unsigned int  	CNT_INT_BIT			= sizeof(int)*8;	//	32
const unsigned int		CNT_JOB_ID			= 64;
const unsigned int		CNT_PROC_ID			= 256;
const unsigned int		CNT_FLAG_BIT		= 1;
const unsigned int		CNT_FLAG_BOUND		= CNT_PROC_ID/CNT_INT_BIT;   // 8

const int				SZ_FLAG_INFO		= sizeof(int) * CNT_JOB_ID * CNT_FLAG_BOUND;    // 2048

enum					LOG_FALG
						{	LOG_OFF,
							LOG_CRITICAL,
							LOG_SHORT,
							LOG_DETAIL
						};

enum					SW_FALG
						{	OFF             = 0x0000,
							ON
						};

enum					JOB_ID
						{	OAM 			= 0x0000,		//	Operation & Administration Module
							SSM,							//	Service Support Module
							MHM,							//	Message Handling Module
							STM,							//	Statistics Module
							EIM,							//	External Interface Module
							FTM,							//	Filtering Module
							RTM,							//	RaTing	Module
							AGM,							//	Aggregation Module
							DSM,							//	Distributor Module
							TST,							//	Test
							UNKNOWN							//	Default Value
						};


typedef struct	__PROC_INFO
{
	unsigned int    nJobId;						//	Available Range : 0 ~  63
	unsigned int	nProcId;						//	Available Range	: 0 ~ 255
	unsigned int	nFlag;		    				//	0 	    		: None,
													//	1		    	: Critical Err Log,
													//	2			    : Shortly Msg Log,
													//	3			    : Detail Msg Log
}ProcInfoType, *ProcInfoPtrType;



typedef struct	__PROC_LOG_INFO
{
	ProcInfoType	stProcInfo;
	char			strPathName[LEN_PATH_NAME];
	char			strFileName[LEN_FILE_NAME];
}ProcLogInfoType, *ProcLogInfoPtrType;


class	LogManagement   : public ShmMemory
{
	private:
		ProcLogInfoType		stLogInfo;
		ofstream*           outfile;
		DateTime            date_time;
		unsigned int*       nLogInfo;
		char                strLogFileName[128];

	public:
		LogManagement(const char* strPath, const char* strFile, int nJobId, int nProcId);
		~LogManagement()    {   outfile->close();    delete outfile; }

		void setLogFlag(int nJobId, int nProcId, int nFlag);
		void setProcInfo(int nJobId, int nProcId);
		int getLogFlag();
		int getJobId()  { return stLogInfo.stProcInfo.nJobId;   }
		int getProcId() { return stLogInfo.stProcInfo.nProcId;  }

		void writeLog(char* strMsg, int nCnt, int nFlag);
		void writeLog(char* strMsg, int nFlag = 0x01);
        void writeLogHexaForm (const void* strLogMsg, const char* strTime, int nLoopCnt);

};









#endif

