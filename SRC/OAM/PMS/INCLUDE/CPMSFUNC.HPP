#include "fileSearchC.hpp"
#ifndef	MSGFORMAT_HPP
#include <MsgFormat.hpp>
#endif

/*******************************************************************************
* Global variable & function
*******************************************************************************/
MessageQueue<MsgPmsStatus>  *PMSMQ;

pthread_mutex_t	mutex;

pthread_t            idWaitThread;			// Wait child thread   val & func
void  CreateWaitChildStatusThread();
void*       WaitChildStatusThread(void *a);

pthread_t    idProcCheckThread;				// Process check thread val & func
void  CreateProcessCheckThread();
void*       ProcessCheckThread(void *a);
int ProcCheck(int _procPid, char* _procName);

pthread_t      idMQThread;					// Message Queue thread val & func
void  CreateCheckMQThread();
void*       CheckMQThread(void *a);

pthread_t idOperationThread;				// Operation thread     val & func
void  CreateOperationThread();
void*       OperationThread(void *a);

pthread_t idBackupOperationThread;			// File Backup thread   val & func
void  CreateBackupOperationThread();
void*       BackupOperationThread(void *a);

void HandleSignal(int sig);
char** GetStringList(char*  src);
bool SendSignalProcess(char type, char* processId, int signo);
bool IsAllProcessStopped();					// ��� ���μ��� ���� ���� ����
											// true = yes, false = no

void ShowList();							// ���μ��� ����Ʈ ���

// uda1 or uda2�� ���� ���� �ٲ�� ���� CPMS::ArgCheck �޼ҵ忡�� ó���Ѵ�.
// PMS, Log Manager alias �� DB ���� id, password
char PMSNAME    [10];		// pms alias
char LOGMGR		[10];		// Log Manager
char USER       [10];		// DB ID
char PASSWD     [10];		// DB PASSWORD

 // PMS ���󱸺�
int g_sysId      ;			// system id
int g_svcId      ;			// service id
int g_groupType  ;			// group type
int g_proc_type  ;			// process type
int g_proc_sn    ;			// process sirial number
int g_proc_remark;			// process remark

// ���� �α� path
char LOGFILE[40];

CPtrList procInfoList; /* process info */
CPMS *ptrThis;

// Log Manager
SendLogManage*	clsLogM;

/*******************************************************************************
* Desc : signal�� ó���ϴ� �Լ�
* ���ǻ���: thread�� wait�ϰ� �ִ� ������ pipethread ��� db connection��
* 	    �ϰ� �־� main thread�� ����Ǹ� disconnect �� ���� �ʰ� ����ȴ�.
*	    connection ���� �þ�� �Ǵ� ���� ���� ���ؼ�.
*******************************************************************************/
void HandleSignal(int sig)
{
	void *status;

	DebugLog(LOGFILE, "main() catch signal %d", sig);

	switch(sig)
	{
		case SIGINT:
			CPMS::SetStopFlag(0);					// PMS STOP

			if (pthread_mutex_unlock(&mutex))
				DebugLog(LOGFILE, "Can't unlock mutex");

			pthread_join(idWaitThread           , &status);
			pthread_join(idProcCheckThread      , &status);
			pthread_join(idMQThread             , &status);
			pthread_join(idOperationThread      , &status);
			pthread_join(idBackupOperationThread, &status);

			DebugLog(LOGFILE, "join all thread ");
			DebugLog(LOGFILE, "before sleep");

			// child���� �ñ׳��� ��������� �ణ�� �ð��� �ʿ���.
			sleep(1);
			DebugLog(LOGFILE, "wait for a minute..... " );
			break;

		case SIGUSR2:
		case SIGUSR1:
		case SIGTSTP:
		case SIGCONT:
			break;
		
		default:
			break;
	}
}

// Radius, Filter, Bds, Block, File, SCPS, SCPR, Aggregator Config File SELECT
void CallRadiusCfg(char* file_path_name, command_t &cfgFileInfo);
void CallFilterCfg(char* file_path_name, command_t &cfgFileInfo);
void CallBdsCfg   (char* file_path_name, command_t &cfgFileInfo);
void CallBlockCfg (char* file_path_name, command_t &cfgFileInfo);
void CallFileCfg  (char* file_path_name, command_t &cfgFileInfo);
void CallScpSCfg  (                      command_t &cfgFileInfo);
void CallScpRCfg  (                      command_t &cfgFileInfo);
void CallAggCfg   (char* file_path_name, command_t &cfgFileInfo);

void CallRadiusCfg(char* file_path_name, command_t &cfgFileInfo)
{
	RadiusFileBlock  aaa(file_path_name);

	// ���� �о����
	aaa.read_RadiusFileBlock();
	sprintf(cfgFileInfo.item1, "%u", aaa.getStartBlockNo());
	sprintf(cfgFileInfo.item2, "%u", aaa.getTimeInVal());
}

void CallFilterCfg(char* file_path_name, command_t &cfgFileInfo)
{
	FilterFileNo	 bbb(file_path_name);

	// ���� �о����
	bbb.read_FilterFileNo();
	sprintf(cfgFileInfo.item1, "%u", bbb.getStartBlockNo());
}

void CallBdsCfg   (char* file_path_name, command_t &cfgFileInfo)
{
	BdsFileBlock 	 ccc(file_path_name);

	// ���� �о����
	ccc.read_BdsFileBlock();
	sprintf(cfgFileInfo.item1, "%u", ccc.getFileNo());
	sprintf(cfgFileInfo.item2, "%u", ccc.getStartBlockNo());
	sprintf(cfgFileInfo.item3, "%u", ccc.getEndBlockNo());
	sprintf(cfgFileInfo.item4, "%u", ccc.getFlag());
	sprintf(cfgFileInfo.item5, "%u", ccc.getTimeInVal());
	sprintf(cfgFileInfo.item6, "%u", ccc.getMaxBillSize());
}

void CallBlockCfg (char* file_path_name, command_t &cfgFileInfo)
{
	CBlockConfigData ddd(file_path_name);

	// ���� �о����
	sprintf(cfgFileInfo.item1, "%d", ddd.get_nBlockSize());
	sprintf(cfgFileInfo.item2, "%d", ddd.get_nBlockHeaderSize()	);
	sprintf(cfgFileInfo.item3, "%d", ddd.get_nRecSize());
	sprintf(cfgFileInfo.item4, "%d", ddd.get_nMaxRecCount());
	sprintf(cfgFileInfo.item5, "%u", ddd.get_unBlockSeq());
}

void CallFileCfg  (char* file_path_name, command_t &cfgFileInfo)
{
	CFileConfigData  eee(file_path_name);

	// ���� �о����
	sprintf(cfgFileInfo.item1, "%d", eee.get_nFileHeaderSize());
	sprintf(cfgFileInfo.item2, "%d", eee.get_nOneFileSize());
	sprintf(cfgFileInfo.item3, "%d", eee.get_nOneBlockSize());
	sprintf(cfgFileInfo.item4, "%d", eee.get_nMaxBlockCount());
	sprintf(cfgFileInfo.item5, "%u", eee.get_unFileSeq());
}

void CallScpSCfg  (command_t &cfgFileInfo)
{
	ScpIF_TID fff(g_svcId);

	// ���� �о����
	sprintf(cfgFileInfo.item1, "%d", fff.get_S_TID());
}

void CallScpRCfg  (command_t &cfgFileInfo)
{
	ScpIF_TID ggg(g_svcId);

	// ���� �о����
	sprintf(cfgFileInfo.item1, "%d", ggg.get_R_TID());
}

void CallAggCfg  (char* file_path_name, command_t &cfgFileInfo)
{
	AccountCnt hhh(file_path_name);

	// ���� �о����
	hhh.read_AccountCnt();
	sprintf(cfgFileInfo.item1, "%d", hhh.getAccountCnt());	
}

// Radius, Filter, Bds, Block, File, SCPS, SCPR, Aggregator Config File UPDATE
void CallRadiusCfgWrite(char* file_path_name, command_t cfgFileInfo);
void CallFilterCfgWrite(char* file_path_name, command_t cfgFileInfo);
void CallBdsCfgWrite   (char* file_path_name, command_t cfgFileInfo);
void CallBlockCfgWrite (char* file_path_name, command_t cfgFileInfo);
void CallFileCfgWrite  (char* file_path_name, command_t cfgFileInfo);
void CallScpSCfgWrite  (                      command_t cfgFileInfo);
void CallScpRCfgWrite  (                      command_t cfgFileInfo);
void CallAggCfgWrite   (char* file_path_name, command_t cfgFileInfo);

void CallRadiusCfgWrite(char* file_path_name, command_t cfgFileInfo)
{
	RadiusFileBlock  aaa(file_path_name);

	// ���� ����
	aaa.setStartBlockNo(atoi(cfgFileInfo.item1));
	aaa.setTimeInVal   (atoi(cfgFileInfo.item2));
	aaa.write_RadiusFileBlock();
}

void CallFilterCfgWrite(char* file_path_name, command_t cfgFileInfo)
{
	FilterFileNo	 bbb(file_path_name);

	// ���� �о����
	bbb.setStartBlockNo(atoi(cfgFileInfo.item1));
	bbb.write_FilterFileNo();
}

void CallBdsCfgWrite(char* file_path_name, command_t cfgFileInfo)
{
	BdsFileBlock 	 ccc(file_path_name);

	// ���� ����
	ccc.setFileNo        (atoi(cfgFileInfo.item1));
	ccc.setStartBlockNo  (atoi(cfgFileInfo.item2));
	ccc.setEndBlockNo    (atoi(cfgFileInfo.item3));
	ccc.setFlag          (atoi(cfgFileInfo.item4));
	ccc.setTimeInVal     (atoi(cfgFileInfo.item5));
	ccc.setMaxBillSize   (atoi(cfgFileInfo.item6));
	ccc.write_BdsFileBlock();
}

void CallBlockCfgWrite(char* file_path_name, command_t cfgFileInfo)
{
	CBlockConfigData ddd(file_path_name);

	// ���� ����
	ddd.put_nBlockSize      (atoi(cfgFileInfo.item1));
	ddd.put_nBlockHeaderSize(atoi(cfgFileInfo.item2));
	ddd.put_nMaxRecCount    (atoi(cfgFileInfo.item4));
	ddd.put_unBlockSeq      (atoi(cfgFileInfo.item5));
	ddd.write_BlockConfigFile();
}

void CallFileCfgWrite(char* file_path_name, command_t cfgFileInfo)
{
	CFileConfigData  eee(file_path_name);

	// ���� ����
	eee.put_nFileHeaderSize(atoi(cfgFileInfo.item1));
	eee.put_nOneBlockSize  (atoi(cfgFileInfo.item3));
	eee.put_nMaxBlockCount (atoi(cfgFileInfo.item4));
	eee.put_unFileSeq      (atoi(cfgFileInfo.item5));
	eee.write_FileConfigFile();
}

void CallScpSCfgWrite(command_t cfgFileInfo)
{
	ScpIF_TID fff(g_svcId);

	// ���� ����
	fff.set_S_TID(atoi(cfgFileInfo.item1));
}

void CallScpRCfgWrite(command_t cfgFileInfo)
{
	ScpIF_TID ggg(g_svcId);

	// ���� ����
	ggg.set_R_TID(atoi(cfgFileInfo.item1));
}

void CallAggCfgWrite (char* file_path_name, command_t cfgFileInfo)
{
	AccountCnt hhh(file_path_name);

	// ���� ����
	hhh.setAccountCnt(atoi(cfgFileInfo.item1));
	hhh.write_AccountCnt();
}

/*******************************************************************************
* Desc : fork-exec�� ���� ����� process�� �������� ����Ʈ�� �����ϴµ�
*		 kill signal�� �޾����� ��� process�� �����Ű�� �Լ�
*        Type : G(roup), O(ne), A(ll)
*******************************************************************************/
bool SendSignalProcess(char type, char* processId, int signo)
{
	CNode *node;
	PPROCINFO pNodeData;

	ShowList();				// ��� ���μ��� ���� ǥ��

	node = procInfoList.GetHeadPosition();

	do
	{
		pNodeData = (PPROCINFO)procInfoList.GetAt(node);

		if ( pNodeData->proc_status == STOPPED  ||
		     pNodeData->sys_pid     == getpid() ||
		    !strcmp(pNodeData->proc_name, LOGMGR))
			continue;

		switch(type)
		{
			case GROUP :
				if( pNodeData->group_type != atoi(processId) )
					continue;
				break;

			case ONE:
				if( strncmp(pNodeData->proc_name, processId, strlen(pNodeData->proc_name)) != 0)
					continue;
				break;

			case ALL:
				break;

			default:
				DebugLog(LOGFILE, "wrong command type : [%c]", type);
				//error log (wrong client_pms command)
				ErrorLog(7004, "wrong command type : [%c]", type);				
				return false;
		}

		if (kill(pNodeData->sys_pid, signo) == -1)	//false
		{
			DebugLog(LOGFILE, "==[Fail to SendSignal]==");
			DebugLog(LOGFILE, " TYPE[%c], PROCESSID[%s]", type, processId);
			DebugLog(LOGFILE, " id     : [%s]"          , pNodeData->proc_name);
			DebugLog(LOGFILE, " status : [%c]"          , pNodeData->proc_status);
			DebugLog(LOGFILE, " pid    : [%d]"          , pNodeData->sys_pid);
			DebugLog(LOGFILE, " runCnt : [%d]"          , pNodeData->runCnt);
			DebugLog(LOGFILE, "========================");
			continue;
		}
		else
		{
			DebugLog(LOGFILE, "======[Send Signal]=====");
			DebugLog(LOGFILE, " TYPE[%c], PROCESSID[%s]", type, processId);
			DebugLog(LOGFILE, " id     : [%s]"          , pNodeData->proc_name);
			DebugLog(LOGFILE, " status : [%c]"          , pNodeData->proc_status);
			DebugLog(LOGFILE, " pid    : [%d]"          , pNodeData->sys_pid);
			DebugLog(LOGFILE, " runCnt : [%d]"          , pNodeData->runCnt);
			DebugLog(LOGFILE, "========================");
		}

		pNodeData->exitFlag = 1;	// PMS�� ���μ��� ����� exitFlag 1�� ����

		DebugLog(LOGFILE, "SEND SIGNAL : [%d] TO [%s(%d)]",
				 signo, pNodeData->proc_name, pNodeData->sys_pid);
	} while(node = procInfoList.GetNextPosition(node));

	return true;
}

/*******************************************************************************
* Desc : ��Ʈ�����κ��� token���� ���еǾ��� ù �ܾ return�ϴ� �Լ�
* Argu : �ϳ��̻��� token���� �����Ǿ��� ���ڿ�
* Retn : src - token���� ���еǾ����� ù �ܾ�.
*******************************************************************************/
char** GetStringList(char* src)
{
	char*  str;
	char*  tmp = (char  *) 0;
	char** ar  = (char **) malloc(10*10);
	int i = 0;

	str = strtok(src, " ");
//	cout << str << endl;
	ar[i++] = str;

	while(1)
	{
		str = strtok(tmp, " ");
//		cout << str << endl;
		ar[i] = str;
		i++;
		if(!str) break;
	}

	ar[i] = (char *)0;

	return ar;
}

/*******************************************************************************
* Desc : stop��û�� ������ ��� child process�� ���Ḧ wait�ؾ��Ѵ�.
*		 ��� ���μ����� ���� ���� �ʾҴٸ� ����ɶ�����
*		 wait thread�� ������� �ʴ´�.
* Retn : true  - ��� child ���μ����� ���� ���� �ʴ´�.
*        false - child ���μ����� ���� ���̴�.
*******************************************************************************/
bool IsAllProcessStopped()
{
	CNode *pNode;
	PPROCINFO pNodeData;

	pNode = procInfoList.GetHeadPosition();

	do
	{
		pNodeData = (PPROCINFO)procInfoList.GetAt(pNode);

		//�ڱ��ڽ��� ���� ������ üũ���� �ʴ´�.
		if(pNodeData->sys_pid == getpid()||
		  !strcmp(pNodeData->proc_name, LOGMGR))	// 2000.11.21 changed by ky
			continue;

		if (pNodeData->proc_status == RUNNING)
			return false;
	}while(pNode = procInfoList.GetNextPosition(pNode));

	return true;
}

/*******************************************************************************
* Desc : child�� ���Ḧ wait�ϴ� thread�� ����.
*******************************************************************************/
void CreateWaitChildStatusThread()
{
	if (pthread_create(&idWaitThread, NULL, &WaitChildStatusThread, (void *)NULL) == 0)
		DebugLog(LOGFILE, "Create CheckWaitChildStatusThread");
	else
	{
		//error log (fail to create thread)
		ErrorLog(7006, "WaitThread id = %d", idWaitThread);
		DebugLog(LOGFILE,"Fail to Create Check Wait Child Status Thread");
	}
}

/*******************************************************************************
* Desc : fork-exec�� ���� ����� process list���� ���Ḧ wait�ϴ� �Լ�
*		 ������¸� check�Ͽ� ����� db�� �����Ѵ�.
*******************************************************************************/
void *WaitChildStatusThread(void *a)
{
	int status;				// child�κ��� ���� ������� ����
	int exit_status;		// ���� ����
	int sig_status;			// �ñ׳� ��ȣ
	int nStop  = 1; 		// ��� ���μ����� ����Ǳ⸦ ��ٸ�
	bool abnormalFlag;		// ������ ���� üũ �÷���

	CNode *pNode;
	PPROCINFO pNodeData;

	CPmsDbHandler db(1, "recvpipe", "sendpipe");

	if (db.ConnectDBMultiThread(1, USER, PASSWD))	//true
		DebugLog(LOGFILE, "connect ok in waitchildstatus thread ");
	else
		pthread_exit(NULL);

	while(nStop)
	{
		if(CPMS::GetStopFlag() == 0)
			nStop = 0;

		sleep(1);

		pNode = procInfoList.GetHeadPosition();

		if (pthread_mutex_lock(&mutex))
      		DebugLog(LOGFILE,"Can't lock mutex");

		do
		{
			pNodeData = (PPROCINFO) procInfoList.GetAt(pNode);

			if (pNodeData->proc_status == STOPPED ||
			    pNodeData->proc_status == PAUSED  ||
			    pNodeData->sys_pid     == 0       ||
			    pNodeData->sys_pid     == getpid())
				continue;

			abnormalFlag = false;

			if (waitpid(pNodeData->sys_pid, &status, WNOHANG) == pNodeData->sys_pid)
			{
				if(WIFEXITED(status)) // 0:abnormal exit, 1:normal exit
				{
					exit_status = WEXITSTATUS(status);	// ���� ���� ��ȣ
					if (exit_status == 0)
					{
						DebugLog(LOGFILE, "%s(%d) is terminated normally Exit Status = %d",
					             pNodeData->proc_name,
								 pNodeData->sys_pid,
								 exit_status);

						pNodeData->proc_status = STOPPED;
						pNodeData->sys_pid = 0;
					}
					else		// exit -1, -2 .. etc...
					{
						// abnormal exit
						DebugLog(LOGFILE, "%s(%d) is terminated abnormally Exit Status = %d",
					             pNodeData->proc_name,
								 pNodeData->sys_pid,
								 exit_status);

						pNodeData->proc_status = ABNORMAL;
						pNodeData->sys_pid = 0;

						abnormalFlag = true;
					}
					db.UpdateProcStatusTable(1, *pNodeData);
				}
				else		// SIGKILL, SIGSTOP�� �����ǵ� �ñ׳�
				{
					sig_status = WTERMSIG(status);	// �ñ׳� ��ȣ

					if (sig_status == 16)  // 16 = SIGUSR1 = normally exit
					{
						DebugLog(LOGFILE, "%s(%d) is terminated normally signal = %d(SIGUSR1)",
				       		     pNodeData->proc_name,
								 pNodeData->sys_pid,
								 sig_status);

						pNodeData->proc_status = STOPPED;
						pNodeData->sys_pid = 0;
					}
					else
					{
						DebugLog(LOGFILE, "%s(%d) is terminated abnormally signal = %d",
				       		     pNodeData->proc_name,
								 pNodeData->sys_pid,
								 sig_status);

						pNodeData->proc_status = KILLED;
						pNodeData->sys_pid = 0;

						abnormalFlag = true;
					}

					db.UpdateProcStatusTable(1, *pNodeData);
				}//end if

				// ��Ʈ��ũ ���� ������Ʈ. ��������̺��
				// ��Ʈ��ũ ���µ� �������·� ������Ʈ ��.
				if (pNodeData->net_status == REGNETWORK)
				{
					pNodeData->net_status = ERRNETWORK;
					db.UpdateProcStatusTable(1, *pNodeData, 'N');
				}

				// ������ ����� PMS���� ��⵿ �� PMS���� �Ǵ� ��ƾ.
				if (abnormalFlag == true)
				{
					pNodeData->runCnt = pNodeData->runCnt + 1;	// start�� runCnt 1�� ����

					// 0�̿ܿ� 5���̻� ������ ����� ��� child process ���� ��
					// PMS����.  HA�� ������� �ѱ�
					if (pNodeData->runCnt > 5 && pNodeData->exitFlag == 0)
						ptrThis->ExitPMS();
					else
					if (CPMS::GetStopFlag() != 0 && pNodeData->exitFlag == 0)
						ptrThis->ForkOneProcess(pNodeData, 1);
					else
					if (pNodeData->runCnt > 0) 	// start�� runCnt 1�� ����
						pNodeData->runCnt = pNodeData->runCnt - 1;
				}//end if
			}//end if
		}while(pNode = procInfoList.GetNextPosition(pNode));

  		if (pthread_mutex_unlock(&mutex))
	      	DebugLog(LOGFILE,"Can't unlock mutex");

		sleep(2);

	}//end while

	if (db.DisconnectDBMultiThread(1))
		DebugLog(LOGFILE, "success to disconnect DB in wait thread");
	else
		DebugLog(LOGFILE, "fail to disconnect DB in wait thread");

	pthread_exit(NULL);

	return NULL; // use just to prevent warning
}

/*******************************************************************************
* Desc : ���μ��� ���� üũ ������ ����
*******************************************************************************/
void CreateProcessCheckThread()
{
	if (pthread_create(&idProcCheckThread, NULL, &ProcessCheckThread, (void *)NULL) == 0)
		DebugLog(LOGFILE, "Create Process Check Thread");
	else
	{
		//error log (fail to create thread)
		ErrorLog(7006, "Process Check Thread id = %d", idProcCheckThread);
		DebugLog(LOGFILE, "Fail to Create ProcessCheckThread");
	}
}

/*******************************************************************************
* Desc : ���μ��� ���� üũ ������
*        ���� status�� RUNNING���� ���μ����� ���� �ý��� ���� ps��
*        ���μ����� ���¸� üũ �������̸� �Ѿ�� �������� �ƴҽÿ���
*        status�� STOPPED�� ������Ʈ �Ѵ�.
*******************************************************************************/
void* ProcessCheckThread(void* a)
{
	CNode *pNode;
	PPROCINFO pNodeData;

	CPmsDbHandler db(2, "recvpipe", "sendpipe");

	if (db.ConnectDBMultiThread(2, USER, PASSWD) )	//true
		DebugLog(LOGFILE, "connect ok in Process Check Thread");
	else
	{
		DebugLog(LOGFILE, "fail to connect in Process Check Thread");
		pthread_exit(NULL);
	}

	while(CPMS::GetStopFlag())
	{
		pNode = procInfoList.GetHeadPosition();

		sleep(30);
//		DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@");
//		DebugLog(LOGFILE, " Proc Check Thread �Դϴ�.");
//		DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

		do
		{
			if (CPMS::GetStopFlag() == 0)
				break;

			pNodeData = (PPROCINFO)procInfoList.GetAt(pNode);

			// pid�� PMS�̰ų� ���°� STOP�̰ų� pid 0�̰ų� process name��
			// ���� ��� process check�� ���� ����.
			if ( pNodeData->sys_pid     == getpid() ||
			     pNodeData->proc_status == STOPPED  ||
			     pNodeData->sys_pid     == 0        ||
			     strcmp(pNodeData->proc_name, "") == 0 )
				continue;

			if( ProcCheck( pNodeData->sys_pid, pNodeData->proc_name ) == 0 )
			{
		        pthread_mutex_lock(&mutex);

				pNodeData->proc_status = STOPPED;
				db.UpdateProcStatusTable(2, *pNodeData);

				pthread_mutex_unlock(&mutex);
			}//end if
		} while( (pNode = procInfoList.GetNextPosition(pNode)) );
	}//end while

	if ( db.DisconnectDBMultiThread(2) )
		DebugLog(LOGFILE, "success to disconnect DB in Process Checkt Thread");
	else
		DebugLog(LOGFILE, "fail to disconnect DB in Process Check Thread");

	pthread_exit(NULL);

	return NULL;
}

/*******************************************************************************
* ���μ��� ���� ���� üũ
* return : -1 - pipe open error
*           1 - ���μ��� ���� �۵�
*           0 - ���μ��� ����.(����)
*******************************************************************************/
int ProcCheck(int _procPid, char* _procName)
{
	FILE* pp;		    // pipe open descriptor
	char osCmd[60];		// os command
	char strBuff[100];	// string temporary buffer

	int procPid;		// process pid buffer
	char procName[20];	// process name buffer

	sprintf(osCmd, "ps -ef | grep %s | grep -v grep", _procName);

	// pipe open error check
	if( (pp = popen(osCmd, "r")) == NULL )
	{
		DebugLog(LOGFILE, "pipe open error !!");
		//error log (pipe open error)
		ErrorLog(7007, "command = %s", osCmd);
		
		return -1;
	} // end if

	sleep(3);

	// process compare
	while( fgets(strBuff, 100, pp) )
	{
		memset(procName, 0x00, strlen(procName));
		memcpy(procName, &strBuff[9], 5);

		procPid = atoi(procName);

		memset(procName, 0x00, strlen(procName));
		memcpy(procName, strstr(strBuff, _procName), strlen(_procName));

//		DebugLog(LOGFILE, "%s", strBuff);
//		DebugLog(LOGFILE, "%d, %s", procPid, procName);

//		cout << "Check pid = " << _procPid << "  Check Pname = " << _procName << endl;
		if( procPid == _procPid && !strcmp(procName, _procName) )
		{
//			cout << "procPid = " << procPid << "  procName = " << procName << endl;
		        pclose(pp);
			return 1;
		}

		DebugLog(LOGFILE, "%s", strBuff);

	} // end while

	pclose(pp);

	DebugLog(LOGFILE, "=[process stopped check]=");
	DebugLog(LOGFILE, " Name1 = %s, Name2 = %s", procName, _procName);
	DebugLog(LOGFILE, " Pid1  = %d, Pid2  = %d", procPid,  _procPid);
	DebugLog(LOGFILE, "=========================");

	return 0;
} // end ProcCheck

/*******************************************************************************
* ���� �α� ���� (������ CPmsCommon.hpp��..)
*******************************************************************************/
bool ErrorLog(int errCode, char* format, ...)
{
	char strMessage[4000];
	char strFormat [2000];

	va_list var;
	va_start( var, format );

	 sprintf( strFormat, "%s", format );		// make output format
	vsprintf( strMessage, strFormat, var );		// SetFileName�޼ҵ�� ����.
	
	*clsLogM << clsLogM->ErrorCode(0)
	         << "[ERROR] ERROR CODE = " << errCode << ", "
	         << strMessage << endl;

	return true;
}

bool DebugLog(char* file, char* format, ...)
{
	char strMessage[4000];
	char strFormat [2000];

	va_list var2;
	va_start( var2, format );

	 sprintf( strFormat, "%s", format );		// make output format
	vsprintf( strMessage, strFormat, var2 );	// SetFileName�޼ҵ�� ����.
	
	*clsLogM << clsLogM->ErrorCode(0) << strMessage << endl;

#ifdef _OUTPUT_
	CTime t;

	// make debuglog file name
	char time[30];
	char fname[50];
	t.Time_Display(time, 4);						// System Date
	sprintf(fname, "%s.%s",file,time);				// make file name

	char* out = new char[strlen(format) + 400];

	t.Time_Display(time, 2);						// System Time

	sprintf(out, "[%s] -> %s\n", time,  format);	// make format

	va_list var;
	FILE *fp;

	va_start( var, format );
	if ( fp = fopen(fname, "a+") )	// m_fname�� �������.
	{								// m_fname�� �����ڿ�����
		vfprintf( fp, out, var );	// SetFileName�޼ҵ�� ����.
		fclose(fp);
		delete out;
		return true;
	}

//	cout << "can't make log file " << strerror(errno) << endl;
	fclose(fp);
	delete out;
	return false;
#else
	return true;
#endif
}

/*******************************************************************************
* Desc : Process infomation print
*******************************************************************************/
void ShowList()
{
#if 0
	CNode *pNode;
	PPROCINFO pNodeData;

	pNode = procInfoList.GetHeadPosition();

	DebugLog(LOGFILE, "=======[Proc List]======");

	do
	{
		pNodeData = (PPROCINFO)procInfoList.GetAt(pNode);

		DebugLog(LOGFILE, " id     : [%s]"  , pNodeData->proc_name);
		DebugLog(LOGFILE, " status : [%c]"  , pNodeData->proc_status);
		DebugLog(LOGFILE, " pid    : [%d]"  , pNodeData->sys_pid);
		DebugLog(LOGFILE, " runCnt : [%d]\n", pNodeData->runCnt);
	} while(pNode = procInfoList.GetNextPosition(pNode));

	DebugLog(LOGFILE, "========================");
#endif
}

/*******************************************************************************
* Desc : fork-exec�� ���� ����� process�κ��� ���������� ���۹ޱ� ����
*        message queue�κ��� message�� wait�ϴ� �Լ��� thread�� ������Ű�� �Լ�.
* argument :
* return :
*******************************************************************************/
void CreateCheckMQThread()
{
	if (pthread_create(&idMQThread, NULL, &CheckMQThread, (void *) NULL) == 0)
		DebugLog(LOGFILE, "Create CheckMQThread..");
	else
	{
		DebugLog(LOGFILE, "Fail to Create Check MQ Thread..");

		//error log (fail to create thread)
		ErrorLog(7006, "Process Create Check MQ Thread id = %d", idMQThread);
	}
}

/*******************************************************************************
* Desc : fork-exec�� ���� ����� process�κ��� ����������
*	     ���۹ޱ� ���� message queue�κ��� message�� wait�ϴ� �Լ�,
* argument :
* return :
*******************************************************************************/
void *CheckMQThread(void *a)
{
	char PMSMQ_NO[2];
	PMSMQ = new MessageQueue<MsgPmsStatus>(g_sysId, 0, 1, 1, 0, 10, 1000);
	MsgPmsStatus	OneRec(MSG_STATUS, g_sysId, 0, 1, 1, 0, 0);

	sprintf(PMSMQ_NO, "%d", g_sysId);

	char status;
	CNode *pNode;
	PPROCINFO pNodeData;
	CPmsDbHandler db(3, "recvpipe", "sendpipe");

	if (db.ConnectDBMultiThread(3, USER, PASSWD) )		//true
		DebugLog(LOGFILE, "connect ok in check MQ thread ");
	else
	{
		DebugLog(LOGFILE, "fail to connect in checkmq thread");
		pthread_exit(NULL);
	}

	while(CPMS::GetStopFlag())
	{
		memset ((char*)&OneRec, NULL, sizeof(MsgPmsStatus));

		if (PMSMQ->recvMsg(&OneRec) == FLAG_MQ_IS_EMPTY)
		{
//			cout << "[ERROR] PMSMQ " << PMSMQ_NO << " IS EMPTY sleep(1)...!"	<< endl;
			sleep(2);
			continue;
		}

		DebugLog(LOGFILE, "");
		DebugLog(LOGFILE, "@@@@@@@@@@[receive message]@@@@@@@@@@");
		DebugLog(LOGFILE, "  OneRec.get_nPID = %d"   , OneRec.get_nPID());
		DebugLog(LOGFILE, "  OneRec.get_nStatus = %d", OneRec.get_nStatus());
		DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
		DebugLog(LOGFILE, "");

		switch(OneRec.get_nStatus())
		{
			case  0:
				status = STOPPED;
				break;
			case  1:
				status = RUNNING;
				break;
			case -1:
				status = KILLED;
				break;
			case -2:
				status = ABNORMAL;
				break;
			case -3:
				status = ERRNETWORK;
				break;
			case -4:
				status = REGNETWORK;
				break;
			default:
				continue;
		}

		pNode = procInfoList.GetHeadPosition();

		do
		{
			if (CPMS::GetStopFlag() == 0)	break;

			pNodeData = (PPROCINFO)procInfoList.GetAt(pNode);

			if ( pNodeData->sys_pid == getpid() )	continue;

			if( pNodeData->sys_pid == OneRec.get_nPID() )
			{
//				if (pthread_mutex_lock(&mutex))
//   					DebugLog(LOGFILE,"Can't unlock mutex");

			    switch (status)
			    {
			    	case ERRNETWORK:
			    	case REGNETWORK:
			    		pNodeData->net_status = status;
			    		db.UpdateProcStatusTable(3, *pNodeData, 'N');
						break;

					case STOPPED:
					case KILLED:
					case ABNORMAL:
   						pNodeData->proc_status = status;
				    	db.UpdateProcStatusTable(3, *pNodeData, 'P');

   						// ���μ����� ������ ��Ʈ��ũ ���µ� ���� ������Ʈ ��Ŵ.
   						if (pNodeData->net_status == REGNETWORK)
   						{
   							pNodeData->net_status = ERRNETWORK;
			    			db.UpdateProcStatusTable(3, *pNodeData, 'N');
			    		}
			    		break;

					case RUNNING:
   						pNodeData->proc_status = status;
				    	db.UpdateProcStatusTable(3, *pNodeData, 'P');
				}//end switch

//				if (pthread_mutex_unlock(&mutex))
//   					DebugLog(LOGFILE,"Can't unlock mutex");
			}//end if
		} while( (pNode = procInfoList.GetNextPosition(pNode)) );
	}//end while

	if (db.DisconnectDBMultiThread(3))
		DebugLog(LOGFILE," success to disconnect DB in MQ thread");
	else
		DebugLog(LOGFILE," fail to disconnect DB in MQ thread");

	delete PMSMQ;

	pthread_exit(NULL);
	return NULL;

#if 0
	while(CPMS::GetStopFlag())
	{
		pNode = procInfoList.GetHeadPosition();

		sleep(1);
//		DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@");
//		DebugLog(LOGFILE, "     MQ Thread �Դϴ�."    );
//		DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

		do
		{
			if (CPMS::GetStopFlag() == 0)	break;

			pNodeData = (PPROCINFO)procInfoList.GetAt(pNode);

			if ( pNodeData->sys_pid == getpid() )	continue;

			if( pNodeData->proc_status == RUNNING )
			{
//				if (pthread_mutex_lock(&mutex))
//   					DebugLog(LOGFILE,"Can't unlock mutex");

//				DebugLog(LOGFILE, "%s(%d) is not send ",
//				pNodeData->proc_name, pNodeData->sys_pid);
//   		        pNodeData->proc_status = ABNORMAL;
//			    db.UpdateProcStatusTable(3, *pNodeData);

//				if (pthread_mutex_unlock(&mutex))
//   					DebugLog(LOGFILE,"Can't unlock mutex");
			}
		} while( (pNode = procInfoList.GetNextPosition(pNode)) );
	}//end while

#endif
}

/*******************************************************************************
* Desc : PMS����ó�� thread
*        20�ʸ��� ��Ʈ��ũ ���� �÷��� ������ üũ�Ѵ�.
*        ��Ʈ��ũ �̻�� child���μ����� ��� �������� PMS�� �����Ѵ�.
*        pms1stop.HaFlag������ ����� HA���� PMS�� �������� �˸���.
*
*        1�ð����� Disk�� CPU�� ���� ������ DB Insert�Ѵ�.
*******************************************************************************/
void  CreateOperationThread()
{
	if (pthread_create(&idOperationThread, NULL, &OperationThread, (void *) NULL) == 0)
		DebugLog(LOGFILE, "Create Operation Thread..");
	else
	{
		DebugLog(LOGFILE, "Fail to Create Operation Thread..");
		//error log (fail to create thread)
		ErrorLog(7006, "Process Create Operation Thread id = %d", idOperationThread);
	}
}

/*******************************************************************************
* Desc : PMS�� ����ó���� ���� Thread
*******************************************************************************/
#define NETWAITTIME			    20		//   20 second
#define SYSINFOWAITTIME		  3600		// 3600 second

void* OperationThread(void *a)
{
	time_t netErrChkST, netErrChkET;// Network ���� �÷��� ���� üũ time
	time_t sysInfoST  , sysInfoET;	// system disk, cpu ���� ���� ������Ʈ üũ time

	char	stopFileNAME[30];
	sprintf(stopFileNAME, "/home/uda%d/bin/pms%dstop.HaFlag", g_sysId, g_sysId);

	bool netErrChkFlag = true;
	bool sysInfoFlag   = true;

	CTime t;
	char sTime[10];			// system time
	char sDate[10];			// system date
	char systemDate[20];	// system time & date ex)20001115114454

	while(CPMS::GetStopFlag())
	{
		sleep(2);

		// Network ���� üũ ***************************************************
		if (netErrChkFlag)
		{
			time(&netErrChkST);
			netErrChkFlag = false;
		}

		time(&netErrChkET);

		if ( (netErrChkET - netErrChkST) >= NETWAITTIME )	// 20�ʸ��� üũ
		{
			// HA�� ��Ʈ��ũ ���� �÷��� ���� üũ.
			if (access(stopFileNAME, F_OK) == 0)
			{
				DebugLog(LOGFILE, "");
				DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
				DebugLog(LOGFILE, "    pms%dstop.HaFlag Check...   ", g_sysId);
				DebugLog(LOGFILE, "    PMS EXIT !!                 ");
				DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
				DebugLog(LOGFILE, "");
				
				ptrThis->ExitPMS();		// ������ ������ PMS ����. ExitPMS ȣ��
				sprintf(stopFileNAME, "rm /home/uda%d/bin/pms%dstop.HaFlag",
				        g_sysId, g_sysId);
				system(stopFileNAME);	// PMS STOP Flag File ����
			}
			netErrChkFlag = true;
		}
		/**********************************************************************/

		// Disk, CPU ���� ���� ������Ʈ ****************************************
		if (sysInfoFlag)
		{
			time(&sysInfoST);
			sysInfoFlag = false;
		}

		time(&sysInfoET);

		if( (sysInfoET - sysInfoST) >= SYSINFOWAITTIME )
		{
			memset(sTime     , 0x00, sizeof(sTime     ));
			memset(sDate     , 0x00, sizeof(sDate     ));
			memset(systemDate, 0x00, sizeof(systemDate));
			t.Time_Display(sDate, 4);
			t.Time_Display(sTime, 5);
			sprintf(systemDate, "%s%s", sDate, sTime);

			ptrThis->InsertSysInfo(systemDate, 1);			// Disk ���� ���� ������Ʈ
			ptrThis->InsertSysInfo(systemDate, 2);			// CPU  ���� ���� ������Ʈ
			sysInfoFlag = true;
		}
		/**********************************************************************/
	}//end while

	pthread_exit(NULL);
	return NULL;
}

/*******************************************************************************
* Desc : PMS ���� ó�� thread�� �����Ѵ�.
*******************************************************************************/
void  CreateBackupOperationThread()
{
	if (pthread_create(&idBackupOperationThread, NULL, &BackupOperationThread, (void *) NULL) == 0)
		DebugLog(LOGFILE, "Create Backup Operation Thread..");
	else
	{
		DebugLog(LOGFILE, "Fail to Create Backup Operation Thread..");
		//error log (fail to create thread)
		ErrorLog(7006, "Process Create Backup Operation Thread id = %d", idBackupOperationThread);
	}
}

/*******************************************************************************
* Desc : ���� ��� ��ġ �۾��� ó�� �Ѵ�.
*        DB table T_BACKUP_INFO�� �����Ͽ� backup���� ������ �о�´�.
*        ��� �ð��� üũ�Ͽ� �Ϸ� �ѹ� ����ð��뿡 ��� ���μ����� ȣ���Ѵ�.
*        ��ġ �۾��̸� DB�� ������ ����� ������ �ֱ⶧���� sleep 1�ð��� �־�
*        ���̺��� ������ refresh�Ѵ�.
*******************************************************************************/
void* BackupOperationThread(void *a)
{
	time_t backupST, backupET;	// Backup Info Table Load �ð� ����
	bool backupChkFlag = true;
	
	BackInfo backInfo[100];
	int readCnt;

	int x;						// for count
	int reCnt = 0;				// fork retry count
	pid_t	backupInfo;			// child pid

	char backupInfoPath[ 30];	// backup info process path
	char backupInfoName[300];	// backup info process name

	memset(backupInfoPath, 0x00, sizeof(backupInfoPath));
	sprintf(backupInfoPath, "/home/uda%d/bin/BackupMGR", g_sysId);

	CTime t;
	char curTime[30];

	CPmsDbHandler db(4, "recvpipe", "sendpipe");

	if (db.ConnectDBMultiThread(4, USER, PASSWD) )		//true
		DebugLog(LOGFILE, "connect ok in check Backup operation thread ");
	else
	{
		DebugLog(LOGFILE, "fail to connect in Backup operation thread");
		pthread_exit(NULL);
	}

	// ���� �ʱ�ȭ
//	for(x = 0; x < 100; x++)
//		backInfo.procFlag = false;

	db._SelectBackupInfo(backInfo, readCnt);	// T_BACKUP_INFO data select

	while(CPMS::GetStopFlag())
	{
		if (backupChkFlag)
		{
			time(&backupST);
			backupChkFlag = false;
		}
		
		time(&backupET);
		
		if ( (backupET - backupST) >= 1 )		// 1�ð� �ѹ� ���� 3600���� ����!!
		{
			db._SelectBackupInfo(backInfo, readCnt);// T_BACKUP_INFO data select
			backupChkFlag = true;
		}

		t.Time_Display(curTime, 5);			// System Time  ex)154125

		for(x = 0 ; x < readCnt ; x++)
		{
//			if ( atoi(backInfo[x].bak_time) b<= (atoi(curTime)) &&
//			    (atoi(curTime)-atoi(backInfo[x].bak_time)) < 10000 )
//			if (backInfo[x].procFlag == true)
	
			if ( atoi(backInfo[x].bak_time) == atoi(curTime) )
			{
				memset(backupInfoName, 0x00, sizeof(backupInfoName));
				sprintf(backupInfoName, "BackupMGR %s %s %s %s %d %d",
				backInfo[x].srcDir , backInfo[x].srcFile, 
				backInfo[x].dstDir , backInfo[x].dstFile, 
				backInfo[x].bak_day, backInfo[x].bak_delday);

				DebugLog(LOGFILE, "=========Backup File=========");
				DebugLog(LOGFILE, "  %s"                         , backupInfoName);
				DebugLog(LOGFILE, "=============================");

				backupInfo = fork();

				if (backupInfo == 0)
				{
					if (execvp(backupInfoPath, GetStringList(backupInfoName)) < 0)
					{
						DebugLog(LOGFILE, "start backupInfo fail");
						DebugLog(LOGFILE, "backupInfoPath = %s, backupInfoName = %s",
						backupInfoPath, backupInfoName);
						
						//error log (fail to fork process)
						ErrorLog(7002, "backupInfoPath = %s, backupInfoName = %s",
						backupInfoPath, backupInfoName);
						
						DebugLog(LOGFILE, "quit pms");
						clsLogM->put_EventHistData(FLAG_END, 5);
						exit(5);
					}
				}

				reCnt = 0;

				if (backupInfo)
					while(waitpid(backupInfo, NULL, WNOHANG) != backupInfo)
					{
						if (reCnt == 10)
						{
							// �����޼��� ���!!
							break;
						}
						sleep(1);
						reCnt++;
					}//end while
//				backInfo[x].procFlag = false;
			}//end if
		}//end for
		sleep(1);
	}//end while

	if (db.DisconnectDBMultiThread(4))
		DebugLog(LOGFILE, "success to disconnect DB in Backup Operation Thread");
	else
		DebugLog(LOGFILE, "fail to disconnect DB in Backup Operation Thread");

	pthread_exit(NULL);
	return NULL;
}