#include <iostream.h>
#include <fstream.h>

#ifdef	_SVC1
	const char*		PMS_PROC_INFO_FILE	= "/SVC1/DATA/ProcInfo.cfg";
	const char*		PMS_NE_INFO_FILE	= "/SVC1/DATA/NeInfo.cfg";
#elif	_SVC2
	const char*		PMS_PROC_INFO_FILE	= "/SVC2/DATA/ProcInfo.cfg";
	const char*		PMS_NE_INFO_FILE	= "/SVC2/DATA/NeInfo.cfg";
#endif

enum					SW_FALG
						{	OFF             = 0x0000,
							ON
						};

enum					JOB_ID		
						{	OAM 			= 0x0000,
							RD1,
							RD2,
							PAS,
							FT,
							COL,
							BDS1,
							BDS2,
							TST,
							UNKNOWN
						};

class	StatusInfo
{
	private:
		int		nStatus;
		int		nFailCount;
	
	public:
		int		get_nStatus()				{	return nStatus;			}
		int		get_nFailCount()			{	return nFailCount;		}
		
		void	put_nStatus(int nValue)		{	nStatus		= nValue;	}
		void	put_nFailCount(int nValue)	{	nFailCount	= nValue;	}
};

class	Status
{
	private:
		StatusInfo*		ProcessInfo;
		char*			ProcessInfoFile;
		int				nProcessCount;
	
	public:
		Status(const char* strFileName, int nProcCount = 10)
		{
			ProcessInfoFile = new char[strlen(strFileName)+1];
			ProcessInfo		= new StatusInfo[nProcCount];
			
			strcpy(ProcessInfoFile, strFileName);
			nProcessCount = nProcCount;
		}
		~Status()
		{
			delete ProcessInfoFile;
			delete ProcessInfo;
		}
		
		int		get_ProcessFailCount(int nJobId)
		{
			if (read_ProcessStatus() < 0 )
				return -1;
			else
				return ProcessInfo[nJobId].get_nFailCount();
		}
		int		get_ProcessStatus(int nJobId)
		{
			if (read_ProcessStatus() < 0 )
				return -1;
			else
				return ProcessInfo[nJobId].get_nStatus();
		}
		
		int		put_ProcessFailCount(int nJobId)
		{
			StatusInfo		ProcessStatusInfo;
			
			if (read_ProcessStatus() < 0)
				return -1;
			
			ProcessStatusInfo.put_nFailCount(get_ProcessFailCount(nJobId)+1);
			if(write_ProcessStatus(ProcessStatusInfo, nJobId) < 0)
				return -1;
			
			return 0;
		}
		
		int		init_ProcessFailCount()
		{
			for (int i = 0; i < nProcessCount; i++)
			{
				ProcessInfo[i].put_nFailCount(0);
				write_ProcessStatus(ProcessInfo[i], i);
			}
			
			return 0;
		}
		
		int		get_StatusInfo (StatusInfo*	clStatusInfo)
		{
			if(read_ProcessStatus() < 0)
				return -1;
			else
				memcpy((char*)clStatusInfo, (char*)ProcessInfo, 
						sizeof(StatusInfo) * nProcessCount);

			return 0;
		}
		
		int		read_ProcessStatus()
		{
			int			nFileSize = sizeof(StatusInfo) * nProcessCount;
			ifstream	ProcStatusFile(ProcessInfoFile, ios::binary);
			if (!ProcStatusFile)
			{
				cout << "Process Status File could not be opened !!" << endl;
				return -1;
			}
			
			ProcStatusFile.read(reinterpret_cast<char*>(ProcessInfo), nFileSize);
			ProcStatusFile.close();
			
			return 0;
		}
		
		int		write_ProcessStatus(StatusInfo& ProcessStatusInfo, int nJobId)
		{
			ofstream	ProcStatusFile( ProcessInfoFile, ios::binary);
			if (!ProcStatusFile)
			{
				cout << "Process Status File could not be opened !!" << endl;
				return -1;
			}

			
			ProcStatusFile.seekp (nJobId * sizeof(StatusInfo));
			ProcStatusFile.write (reinterpret_cast<const char*>(&ProcessStatusInfo), \
								  sizeof(StatusInfo));
			ProcStatusFile.close();

			return 0;
		}
};


