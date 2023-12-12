/////////////////////////////////////////////////////////////
//
//   Class Name     : LogManagement
//   Usage          : 로그 관리
//   DATE           : 2000. 9 .1
//   LAST UPDATE    :
//   AUTHOR         : Kwon Chong Wook
//   History        : 
//
/////////////////////////////////////////////////////////////

#include <ConfigLog.hpp>
//#include "SharedConf.hpp"

const unsigned char			SHM_ID_LOG				=	'A';

#ifdef	_SVC1
	const char*				SHM_KEY_LOG_FILE	    =	"/SVC1/DATA/SharedMemory.cfg";
#elif	_SVC2
	const char*				SHM_KEY_LOG_FILE	   	=	"/SVC2/DATA/SharedMemory.cfg";
#endif

const char*				strJobId[]			=
						{	"OAM",							//	Operation & Administration Module
							"SSM",							//	Service Support Module
							"MHM",							//	Message Handling Module
							"STM",							//	Statistics Module
							"EIM",							//	External Interface Module
							"FTM",							//	Filtering Module
							"RTM",							//	RaTing	Module
							"AGM",							//	Aggregation Module
							"TST",							//	Test
							"UNKNOWN"						//	Default Value
						};						

LogManagement::LogManagement(const char* strPath, const char* strFile, int nJobId, int nProcId) : \
		    ShmMemory(SHM_KEY_LOG_FILE, SHM_ID_LOG, SZ_FLAG_INFO, 0660|IPC_CREAT)
{
	nLogInfo = (unsigned int*)ShmPtr();		//	get Address of Shared Memory 
	
	memset((char *)&stLogInfo, NULL, sizeof(ProcLogInfoType));
    setProcInfo(nJobId, nProcId);
	
	memcpy(stLogInfo.strPathName, strPath, strlen(strPath));
	memcpy(stLogInfo.strFileName, strFile, strlen(strFile));
	
	memset(strLogFileName, NULL, 128);
	strcat(stLogInfo.strPathName, *(strJobId+nJobId));
	strcat(strLogFileName, stLogInfo.strPathName);
	strcat(strLogFileName, "/");
	strcat(strLogFileName, strFile);
	
	strcat(strLogFileName, ".");
	strcat(strLogFileName, date_time.get_date());

    outfile = new ofstream(strLogFileName, ios::app);
    
    if (!(*outfile) )   //  File Open Fail
    {
        mkdir (stLogInfo.strPathName, 0777);    // Create Directory
        outfile = new ofstream(strLogFileName, ios::app);
    }
}

void LogManagement::setLogFlag(int nJobId, int nProcId, int nFlag)
{
	unsigned	int		unFilterMask = 0x00000001,
	                    unTempOffset = 0;
	                    
	unTempOffset = nJobId / CNT_FLAG_BOUND; //  get Offset of int Array
	
	unFilterMask <<= (CNT_INT_BIT - CNT_FLAG_BIT) - \
                     (nProcId % CNT_FLAG_BOUND);
	
	if ( nFlag == OFF )                 // Log Off
	    nLogInfo[unTempOffset] ^= unFilterMask;
	else if (nFlag == ON)               // Log On
	    nLogInfo[unTempOffset] |= unFilterMask;
}

void LogManagement::setProcInfo(int nJobId, int nProcId)
{
    stLogInfo.stProcInfo.nJobId = nJobId;
	stLogInfo.stProcInfo.nProcId = nProcId;
}

int LogManagement::getLogFlag()
{
	unsigned	int		unTempMask = 0,
	                    unTempProcID = 0,
	                    unTempOffset = 0;
	int                 nJobId = getJobId(),
	                    nProcId = getProcId();
	
    unTempMask = nLogInfo[nJobId / CNT_FLAG_BOUND];        //  get Offset Value of Flag
    
    unTempMask >>= 	(CNT_INT_BIT - CNT_FLAG_BIT) - \
					(nProcId % CNT_FLAG_BOUND);
					
    return unTempMask;
}

void LogManagement::writeLog(char* strMsg, int nCnt, int nFlag)
{
	if (getLogFlag() == ON)
    {
    	switch(nFlag)
    	{
    		case	LOG_CRITICAL:
    			*outfile << "[ERR]" << "[" << date_time.get_time(1) << "] " << strMsg << endl;
    			break;
    		case    LOG_SHORT:
    		    *outfile << "[MSG]" << "[" << date_time.get_time(1) << "] " << strMsg << endl;
    		    break;
    		case    LOG_DETAIL:
    		    writeLogHexaForm(strMsg, date_time.get_time(1), nCnt);
    		    break;
        }
    }
}

void LogManagement::writeLog(char* strMsg, int nFlag)
{
	if ((getLogFlag() == OFF) && (nFlag == LOG_CRITICAL))
	    *outfile << "[ERR]" << "[" << date_time.get_time(1) << "] " << strMsg << endl;
	else if (getLogFlag() == ON)
    {            
    	switch(nFlag)
    	{
    		case	LOG_CRITICAL:
    			*outfile << "[ERR]" << "[" << date_time.get_time(1) << "] " << strMsg << endl;
    			break;
    		case    LOG_SHORT:
    		    *outfile << "[MSG]" << "[" << date_time.get_time(1) << "] " << strMsg << endl;
    		    break;
    		case    LOG_DETAIL:
    		    writeLogHexaForm(strMsg, date_time.get_time(1), 0);
    		    break;
        }
    }
}

void LogManagement::writeLogHexaForm (  const void* strLogMsg, 
                                        const char* strTime, 
                                        int         nLoopCnt)
{	
	char        strDumpLog[32789],
	            strLogBuff[32789],
	            strLogTmp[20];
    int         nBufLen;

    memset(strDumpLog,0x00,sizeof(strDumpLog));
    memset(strLogBuff,0x00,sizeof(strLogBuff));
    
    memcpy(strDumpLog, strLogMsg, nLoopCnt);
    
    *outfile << "[MSG]" << "[" << strTime << "] ";

    for (int i = 0; i < nLoopCnt; i++)
	{
		if(isalpha(strDumpLog[i]) || isdigit(strDumpLog[i]))
		{
			sprintf(strLogTmp,"%c ", strDumpLog[i]);
			nBufLen += 1;
		}
		else
		{
			sprintf(strLogTmp,"0x%02x ",strDumpLog[i]);
			nBufLen += 4;
		}
		
		if ((i !=0) && ((i % 16) == 0))
		    strcat(strLogTmp,"\n");
		
		if(nBufLen > 32789)
		    break;
		strcat(strLogBuff,strLogTmp);
	}
	*outfile << strLogBuff << endl;
}


