#include "Cpms.hpp"
#include "fileSearchC.hpp"
#include <TempMsgQ_new.hpp>

#ifndef	MSGFORMAT_HPP
#include <MsgFormat.hpp>
#endif

MessageQueue<MsgPmsStatus>  *PMSMQ;
/*******************************************************************************
* Global variable & function
*******************************************************************************/
pthread_mutex_t	mutex;

pthread_t		idWaitThread;
void  CreateWaitChildStatusThread();
void* WaitChildStatusThread(void *a);

pthread_t 		idProcCheckThread;
void  CreateProcessCheckThread();
void* ProcessCheckThread(void *a);
int ProcCheck(int _procPid, char* _procName);

pthread_t		idMQThread;
void  CreateCheckMQThread();
void* CheckMQThread(void *a);

pthread_t		idOperationThread;
void  CreateOperationThread();
void* OperationThread(void *a);

pthread_t		idBackupOperationThread;
void  CreateBackupOperationThread();
void* BackupOperationThread(void *a);

void HandleSignal(int sig);
char** GetStringList(char*  src);
bool SendSignalProcess(char type,char* processId,int signo);
bool IsAllProcessStopped();

CPMS *ptrThis;
CPtrList procInfoList; /* process info */

void ShowList();

// uda1 or uda2에 따라 값이 바뀌며 값은 CPMS::ArgCheck 메소드에서 처리한다.
char PMSNAME    [10];		// pms alias
char LOGMGR		[10];		// Log Manager
char USER       [10];		// DB ID
char PASSWD     [10];		// DB PASSWORD
  // PMS alias 및 DB 접속 id, password

int g_sysId      ;		// system id
int g_svcId      ;		// service id
int g_groupType  ;		// group type
int g_proc_type  ;		// process type
int g_proc_sn    ;		// process sirial number
int g_proc_remark;		// process remark
  // process 형상구분

// 파일 로그 path
char LOGFILE[40];

// Log Manager
SendLogManage*	clsLogM;

/*******************************************************************************
* Desc : process 종료 signal을 받았을때 signal handler로 분기해서
*        바로 종료해버린다면 signal catch시에 interrupt되어진 system call
*        때문에 core dump가 발생하는 경우가 가끔 발생한다.
*        안전하게 종료하기 위해서 flag기능을 하는 변수를 생성.
*******************************************************************************/
int CPMS::nStop  = 1;

CPMS::CPMS()
{
	ptrThis = this;

	logSM = new LogManageSM(1, 1, 1, 2, 0, 0);	// 초기선언 값
	logSM->LogSMinit();
}

CPMS::~CPMS()
{
	DebugLog(LOGFILE, "quit pms");
	clsLogM->put_EventHistData(FLAG_END, 0);
	
	delete logSM;
	delete clsLogM;		
}

/*******************************************************************************
* Desc : PMS 초기화 작업을 한다.
*		 - RegisterSignalHandler() : signal 발생시 처리해야 할 부분들을 명시
*		 - GetProcessInfoFromDB()  : process 정보를 db로부터 가져옴
*	 	 - ForkProcess()		   : process 정보를 가지고 process 생성
*		 - CreateCheckMQThread()   : child process가 보내는 정보를 message
*				     				 queue를 통해서 전송받음.
*        시그널 핸들러 초기화
*        argument check
*        wait child thread 생성, process check thread 생성, MQ thread 생성 ...
* Argu : int    ac - main으로부터 받은 argc
*        char** av - main으로부터 받은 argv
* Retn : void
*******************************************************************************/
void CPMS::Initialize(int ac, char** av)
{
	clsLogM = new SendLogManage(atoi(av[1]), 1, 0, 1, 0, 0);
	*clsLogM << clsLogM->ErrorCode(0) << "PMS is ready !!" << endl;
	clsLogM->put_EventHistData(FLAG_START, FLAG_PROC_START);
	
	ArgCheck(ac, av);				// parameta check

	if (db.ConnectDBMultiThread(0, USER, PASSWD) == true)
		DebugLog(LOGFILE, "connect ok in main thread");
	else
	{
		DebugLog(LOGFILE, "fail to connect main thread");
		DebugLog(LOGFILE, "quit pms");
		clsLogM->put_EventHistData(FLAG_END, 1);
		exit(1);
	}

  	// thread lock initialize
  	if (pthread_mutex_init(&mutex, NULL))
  	{
     	DebugLog(LOGFILE,"Can't initialize mutex");
		//error log (Can't Initialize mutex)
		ErrorLog(7000, "Can't Initialize mutex");
   		DebugLog(LOGFILE, "quit pms");
		clsLogM->put_EventHistData(FLAG_END, 1);
   		exit(1);
  	}

	// process infomation load
	if (GetProcessInfoFromDB() == false)
	{
		DebugLog(LOGFILE, "no data in t_process_info table");
		//error log (no data in PROCESS INFO)
		ErrorLog(7001, "no data in process infomation table");
		DebugLog(LOGFILE, "quit pms");
		clsLogM->put_EventHistData(FLAG_END, 1);
		exit(1);
	}

//	StartResourceMGR();

	RegisterSignalHandler();		// Signal Handler

	CreateWaitChildStatusThread();	// child들의 종료 상태를 체크하는 thread
									// child가 종료 상태를 보낸다.

	CreateProcessCheckThread();		// child들의 종료 상태를 체크하는 thread
									// system명령 ps를 사용하여 종료 상태를 체크한다.

	CreateCheckMQThread();			// child들의 종료 상태를 체크하는 thread
									// Message Queue에 child들이 상태를 보낸다.

	CreateOperationThread();		// 업무 처리 thread
									// HA의 네트워크 상태 플래그 파일을 체크 및
									// 디스크 상태, cput상태를 DB insert한다.
									// PMS 및 child 종료 여부를 결정한다.

	CreateBackupOperationThread();	// 업무 처리 thread
								    // 파일 백업 처리를 한다.

	UpdatePmsProcStatus(RUNNING);	// update pms process info (2000.09.18)

	ExecuteCommand('S', 'O', LOGMGR);
	
	if (av[2][0] == '1')				// HA에 의해 절차되었을 경우
		ExecuteCommand('S', 'A', " ");	// 모든 프로세스 기동
}

/*******************************************************************************
* Desc : Resource Manager를 기동시킨다.
*        Resource Manager가 기동되지 않으면 시스템 전체 프로세스들을 기동해도
*        제대로 처리하지 못하게 되기 때문에 기동이 안될시 3번을 시도한다.
*        3번 시도후 기동이 되지 않으면 PMS가 관리하는 모든 프로세스들을
*        종료시키도록 한다.
* Argu : void
* Retn : void
*******************************************************************************/
void CPMS::StartResourceMGR()
{
	char RESOURCEMGR[30];
    char RESOURCEMGRNAME[20];

	sprintf(RESOURCEMGR    , "/home/uda%d/bin/ResourceMGR", g_sysId);
	sprintf(RESOURCEMGRNAME, "ResourceMGR %d", g_sysId);

	bool procFlag = false;

	if (IsAllProcessStopped() == false)	// child가 동작 중이다.
		procFlag = true;

	if(procFlag == true)	// 자식프로세스가 기동중일시 Resource Manager를
	{						// 기동하지 않음.
		DebugLog(LOGFILE, "*********************************");
		DebugLog(LOGFILE, "  Resource Manager running ....");
		DebugLog(LOGFILE, "*********************************");
		return ;
	}

	int cnt, reCnt;			// 재시도 카운트
	bool runFlag = false;	// 기동 상태 체크 false는 기동 실패, true는 기동 성공
	pid_t resourceMGRPid;	// Resource Manager의 pid
	int retStatus;			// 종료 상태

	bool resourceFlag = false;

	// Resource Manger가 제대로 실행되지 않을시 3번 재시도 한다.
	for(cnt = 1; cnt <= 3; cnt++)
	{
		resourceMGRPid = fork();

		switch(resourceMGRPid)
		{
			case -1:				// fork, exec 실패시
				runFlag = false;
				break;

			case 0:
				if (execvp(RESOURCEMGR, GetStringList(RESOURCEMGRNAME)) < 0)
				{
#ifdef _ERRLOG_
					//errcode (create process error)
					memset(g_errorMsg, 0x00, sizeof(g_errorMsg));
					sprintf(g_errorMsg,
					"process name = %s, process id = %s, process pid = %d",
					name, pNodeData->id, (*pNodeData).pid);
					ErrorLogInsertDB(0, g_processType, g_processId, "6508",
					g_errorMsg);
#endif
					//error log (fail to fork process)
					ErrorLog(7002, 
					"process path = %s, process name = %s, process pid = %d",
					RESOURCEMGR, RESOURCEMGRNAME, getpid());
					
					DebugLog(LOGFILE, "quit pms");
					clsLogM->put_EventHistData(FLAG_END, 5);
					exit(5);
				}//end if
				resourceFlag = true;
				break;

			default:
				reCnt = 0;

				// 처리 완료될때까지 blocking 하며 기다림
				while(waitpid(resourceMGRPid, &retStatus, WNOHANG) != resourceMGRPid)
				{
					if (reCnt == 10)
					{
						// 에러메세지 출력!!
						break;
					}
					sleep(1);
					reCnt++;
				}

				if(WEXITSTATUS(retStatus) == 0)
					runFlag = true;
				else
					runFlag = false;
//				cout << retStatus << endl;
//				cout << WEXITSTATUS(retStatus) << endl;
//				cout << "sizeof retStatus  = " << sizeof(retStatus) << endl;
		}//end switch

		if(runFlag == true && resourceFlag == true && WEXITSTATUS(retStatus) == 255)  // 255 = -1
			break;
	}

	if(runFlag == true)
		DebugLog(LOGFILE, "Resource Manager running........ pid = %d, status = %d",
		resourceMGRPid, WEXITSTATUS(retStatus));
	else										// 기동실패시 PMS 종료 모드 실행
	{
		DebugLog(LOGFILE, "Resource Manager no running........ PMS QUIT, pid = %d, status = %d",
		resourceMGRPid, WEXITSTATUS(retStatus));
		
		//error log (PMS quit)
		ErrorLog(7003, "Resource Manager no running........ PMS QUIT, pid = %d, status = %d",
		resourceMGRPid, WEXITSTATUS(retStatus));
		
		DebugLog(LOGFILE, "quit pms");
		clsLogM->put_EventHistData(FLAG_END, 1);
		exit(1);	// PMS 종료
 	}
}

/*******************************************************************************
*Desc: signal 발생시 처리해야 할 부분들을 명시
*******************************************************************************/
void CPMS::RegisterSignalHandler()
{
	static struct sigaction act;

	act.sa_handler = &HandleSignal;

	sigaction(SIGINT,  &act, (struct sigaction *) 0);
	sigaction(SIGTSTP, &act, (struct sigaction *) 0);
	sigaction(SIGCONT, &act, (struct sigaction *) 0);
	sigaction(SIGUSR1, &act, (struct sigaction *) 0);
	sigaction(SIGUSR2, &act, (struct sigaction *) 0);
}

/*******************************************************************************
* Desc : signal을 처리하는 함수
* 주의사항: thread는 wait하고 있는 이유는 pipethread 경우 db connection을
* 	    하고 있어 main thread가 종료되면 disconnect 를 하지 않고 종료된다.
*	    connection 수가 늘어나게 되는 것을 막기 위해서.
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

			//pms가 죽을때 child를 종료시킴.
			//SendSignalProcess(ALL, NULL, SIGINT);
			DebugLog(LOGFILE, "before sleep");

			// child에게 시그널을 보냈을경우 약간의 시간이 필요함.
			sleep(1);
			DebugLog(LOGFILE, "wait for a minute..... " );
			break;

		case SIGUSR2:
//			kill(0, SIGUSR1);
//			DebugLog(LOGFILE, "send signal : [%d]", SIGUSR2);
			break;

		case SIGUSR1:
			break;

		case SIGTSTP:
			break;

		case SIGCONT:

		default:
			break;
	}
}

/*******************************************************************************
* Desc : pms parameta check & common variable initialize
*		 ex) CPmsMain [1-2] [0-1]
*        첫번째 인자는 시스템 인자. 1=UDA1, 2=UDA2
*        두번째 인자는 시스템 절차 구분 인자. 0이면 PMS디폴트작업(PMS, LOGMGR run)
*        1이면 시스템 절차작업 PMS, LOGMGR 및 전체 프로세스 run. All Start
* Argu : char** av - main parameta
*******************************************************************************/
void CPMS::ArgCheck(int ac, char** av)
{
	if (ac != 3 || (av[1][0] != '1' && av[1][0] != '2')
	            || (av[2][0] != '0' && av[2][0] != '1'))
	{
		DebugLog(LOGFILE, "Usage: PMS [SYSTEM ID(1-2)] [HA FLAG(0-1)]");
		DebugLog(LOGFILE, "quit pms");
		clsLogM->put_EventHistData(FLAG_END, 1);
		exit(1);
	}

	sprintf(LOGFILE, "/SVC%c/LOG/pms", av[1][0]);

	switch(av[1][0])
	{
		case '1':
			strcpy(PMSNAME      , "11PMS00X");
			strcpy(LOGMGR       , "11LOG00X");
			strcpy(USER         , "uda"   );
       		strcpy(PASSWD       , "uda"   );
			g_sysId       = 1;
			g_svcId       = 1;
			g_groupType   = 0;
			g_proc_type   = 1;
			g_proc_sn     = 0;
			g_proc_remark = 0;
       		break;

       	case '2':
			strcpy(PMSNAME      , "22PMS00X");
			strcpy(LOGMGR       , "22LOG00X");
			strcpy(USER         , "uda"   );
       		strcpy(PASSWD       , "uda"   );
			g_sysId       = 2;
			g_svcId       = 2;
			g_groupType   = 0;
			g_proc_type   = 1;
			g_proc_sn     = 0;
			g_proc_remark = 0;
        	break;

       	default:
	        DebugLog(LOGFILE, "Usage: PMS [system id]");
			DebugLog(LOGFILE, "quit pms");
			clsLogM->put_EventHistData(FLAG_END, 1);
			exit(1);
	}//end switch
}

/*******************************************************************************
*Desc : oracle pipe로부터 msg를 수신하여 필요한 처리를 함
*
*argument : thread 생성시 사용되는 argument.
*		    이 함수에서는 쓰이지 않으니 컴파일시 warning 을 막기 위해서
*		    사용함.
*return : 이 함수에서는 쓰이지 않으니 컴파일시 warning 을 막기 위해서
*	  	  사용함.
*******************************************************************************/
void CPMS::ProcessCommand()
{
	command_t command;

	while(CPMS::GetStopFlag())
	{
		if ( !db.RecvMsgFromPipe(0, command) )
			continue;

		DebugLog(LOGFILE, "=====[oracle pipe received]==== ");
		DebugLog(LOGFILE, "  command    : [%c]", command.command);
		DebugLog(LOGFILE, "  type       : [%c]", command.type);
		DebugLog(LOGFILE, "  subContent : [%s]", command.subContent);
		DebugLog(LOGFILE, "=============================== ");

		//procInfoList.RemoveAll();
		GetProcessInfoFromDB();						// process list load

		if( IsRightCommand(&command) == false)
			continue;

		switch(command.command)
		{
			case 'S':	// Start
			case 'T':	// Terminate
				ExecuteCommand(command.command, 				// command
				               command.type,					// type
				               command.subContent);				// subContent
				break;

			case 'O':	// Operation
				switch(command.type)
				{
					// Select
					case 'S':
						SelectCfgFile(command);
						break;

					// Update
					case 'U':
						// 프로세스가 하나라도 실행중일 경우 config파일 수정막음.
//						if (IsAllProcessStopped() == true)
							UpdateCfgFile(command);
//						else
//							DebugLog(LOGFILE,
//							"fail to update config file. process running.");
						break;

					// Disk info
					case 'D':
						InsertSysInfo(command.date, 1);		// 1 : Filesystem
						break;

					// Cpu  info
					case 'C':
						InsertSysInfo(command.date, 2);		// 2 : CPU
						break;

					// Log Manager
					case 'L':
						ChangeStatusLogManager(command);
						break;

					default:
						DebugLog(LOGFILE, "wrong type [%c]", command.type);
				}//end switch
				break;

			default:
				DebugLog(LOGFILE, "wrong command [%c]", command.command);
				//error log (wrong client_pms command)
				ErrorLog(7004, "wrong command [%c]", command.command);				
		}//end switch
	}//end while

	UpdatePmsProcStatus(STOPPED);

	if (db.DisconnectDBMultiThread(0))
		DebugLog(LOGFILE, "success to disconnect DB in main");
	else
		DebugLog(LOGFILE, "ail to disconnect DB in main");
}

/*******************************************************************************
* Desc : client가 요청한 명령의 종류에 따라 처리하는 함수 호출.
* Argu : char  command    - 요청 명령
*        char  type       - 명령의 적용 범위
*		 char* subContent - 범위에 따른 적용범위 조건들.
*******************************************************************************/
void CPMS::ExecuteCommand(char command, char type, char* subContent)
{
	int signo;				// signal number

	if (command == START)
	{
		ForkProcess(type, subContent);
		DebugLog(LOGFILE, "exeucte comand %c, %c, %s", command, type, subContent);
	}
	else
	{
		switch(command)
		{
			case STOP:
				signo = SIGUSR1;
				break;
			case PAUSE:
				signo = SIGSTOP;
				break;
			case WAKE:
				signo = SIGCONT;
				break;
			case STATUS:
				signo = SIGUSR1;
		}//end switch

		SendSignalProcess(type, subContent, signo);
	}//end if
}

/*******************************************************************************
* Desc : Client로 부터의 업무처리 메소드.
*        system의 disk, cpu사용 정보를 DB에 Insert 한다.
*        sysinfo프로세스를 호출. (실제 작업처리는 sysinfo가 한다)
*		 sysinfo프로세스의 argument
*		 infotype : 1(filesystem), 2(cpu)
*        date     : date (YYYYMMDDhhmmss[14])
*        systemid : uda1, uda2 (1, 2)
* Argu : char cliDate - Client로 부터 받은 Date. Client에서 key로 사용한다.
*        int InfoType - infoType (Disk, Cpu)
*******************************************************************************/
void CPMS::InsertSysInfo(char* cliDate, int infoType)
{
	char sysInfoPath[50];
	char sysInfoName[50];

	sprintf(sysInfoPath, "/home/uda%d/bin/SysinfoMGR", g_sysId);
	sprintf(sysInfoName, "SysinfoMGR %d %s %d", infoType, cliDate, g_sysId);

	pid_t	sysInfo;

	sysInfo = fork();

	if (sysInfo == 0)
	{
		if (execvp(sysInfoPath, GetStringList(sysInfoName)) < 0)
		{
			DebugLog(LOGFILE, "start sysinfo fail");
			DebugLog(LOGFILE, "sysInfoPath = %s, sysInfoName = %s",
			sysInfoPath, sysInfoName);
			DebugLog(LOGFILE, "quit pms");
			clsLogM->put_EventHistData(FLAG_END, 5);
			exit(5);
		}
	}

	int reCnt = 0;

	if (sysInfo)
		while(waitpid(sysInfo, NULL, WNOHANG) != sysInfo)
		{
			if (reCnt == 10)
			{
				DebugLog(LOGFILE, "InsertSysInfo. %d wait fail", sysInfo);
				break;
			}
			sleep(1);
			reCnt++;
		}
}

void CallRadiusCfg(char* file_path_name, command_t &cfgFileInfo);
void CallFilterCfg(char* file_path_name, command_t &cfgFileInfo);
void CallBdsCfg   (char* file_path_name, command_t &cfgFileInfo);
void CallBlockCfg (char* file_path_name, command_t &cfgFileInfo);
void CallFileCfg  (char* file_path_name, command_t &cfgFileInfo);
void CallScpSCfg  (                      command_t &cfgFileInfo);
void CallScpRCfg  (                      command_t &cfgFileInfo);
void CallAggCfg   (char* file_path_name, command_t &cfgFileInfo);

/*******************************************************************************
* Desc : Client로 부터의 업무처리 메소드.
*        config file 조회를 한다. File정보를 읽어들여 DB로 Insert 한다.
* Argu : command_t cfgFileInfo - config file을 처리하기 위한 정보가 있다.
* Retn :
*******************************************************************************/
void CPMS::SelectCfgFile(command_t cfgFileInfo)
{
	char file_path		[50+1];
	char file_name		[30+1];
	int  item_count;
	char file_id  		[ 4+1];
	char file_path_name	[70+2];

	item_count = 0;
	memset(file_path     , 0x00, sizeof(file_path     ));
	memset(file_name     , 0x00, sizeof(file_name     ));
	memset(file_id       , 0x00, sizeof(file_id       ));
	memset(file_path_name, 0x00, sizeof(file_path_name));

	// config파일 정보를 DB로 부터 읽어 온다.
	db._SelectCfgFile(cfgFileInfo, file_path, file_name, item_count, file_id);

	sprintf(file_path_name, "%s%s", file_path, file_name);

	switch(file_id[3])
	{
		case '1':					// Radius Interface 1 file (IF)
			CallRadiusCfg(file_path_name, cfgFileInfo);
			break;
		case '2':					// Radius Interface 2 file (Filter)
			CallFilterCfg(file_path_name, cfgFileInfo);
			break;
		case '3':					// BDS    Interface 1 file (IF)
			CallBdsCfg   (file_path_name, cfgFileInfo);
			break;
		case '4':
			CallBlockCfg (file_path_name, cfgFileInfo);
			break;
		case '5':
			CallFileCfg  (file_path_name, cfgFileInfo);
			break;
		case '6':
			CallScpSCfg  (cfgFileInfo);
			break;
		case '7':
			CallScpRCfg  (cfgFileInfo);
			break;
		case '8':
			CallAggCfg   (file_path_name, cfgFileInfo);
			break;
		default:
			DebugLog(LOGFILE, "SelectCfgFile call fail");
			//error log (wrong file id)
			ErrorLog(7005, "wrong file id. File Id -> [%s]", file_id);
			return ;
			break;
	}

	db._InsertCfgFile(cfgFileInfo, item_count, 'S');
}

void CallRadiusCfg(char* file_path_name, command_t &cfgFileInfo)
{
	RadiusFileBlock  aaa(file_path_name);

	// 파일 읽어오기
	aaa.read_RadiusFileBlock();
	sprintf(cfgFileInfo.item1, "%u", aaa.getStartBlockNo());
	sprintf(cfgFileInfo.item2, "%u", aaa.getTimeInVal());
}

void CallFilterCfg(char* file_path_name, command_t &cfgFileInfo)
{
	FilterFileNo	 bbb(file_path_name);

	// 파일 읽어오기
	bbb.read_FilterFileNo();
	sprintf(cfgFileInfo.item1, "%u", bbb.getStartBlockNo());
}

void CallBdsCfg   (char* file_path_name, command_t &cfgFileInfo)
{
	BdsFileBlock 	 ccc(file_path_name);

	// 파일 읽어오기
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

	// 파일 읽어오기
	sprintf(cfgFileInfo.item1, "%d", ddd.get_nBlockSize());
	sprintf(cfgFileInfo.item2, "%d", ddd.get_nBlockHeaderSize()	);
	sprintf(cfgFileInfo.item3, "%d", ddd.get_nRecSize());
	sprintf(cfgFileInfo.item4, "%d", ddd.get_nMaxRecCount());
	sprintf(cfgFileInfo.item5, "%u", ddd.get_unBlockSeq());
}

void CallFileCfg  (char* file_path_name, command_t &cfgFileInfo)
{
	CFileConfigData  eee(file_path_name);

	// 파일 읽어오기
	sprintf(cfgFileInfo.item1, "%d", eee.get_nFileHeaderSize());
	sprintf(cfgFileInfo.item2, "%d", eee.get_nOneFileSize());
	sprintf(cfgFileInfo.item3, "%d", eee.get_nOneBlockSize());
	sprintf(cfgFileInfo.item4, "%d", eee.get_nMaxBlockCount());
	sprintf(cfgFileInfo.item5, "%u", eee.get_unFileSeq());
}

void CallScpSCfg  (command_t &cfgFileInfo)
{
	ScpIF_TID fff(g_svcId);

	// 파일 읽어오기
	sprintf(cfgFileInfo.item1, "%d", fff.get_S_TID());
}

void CallScpRCfg  (command_t &cfgFileInfo)
{
	ScpIF_TID ggg(g_svcId);

	// 파일 읽어오기
	sprintf(cfgFileInfo.item1, "%d", ggg.get_R_TID());
}

void CallAggCfg  (char* file_path_name, command_t &cfgFileInfo)
{
	AccountCnt hhh(file_path_name);

	// 파일 읽어오기
	hhh.read_AccountCnt();
	sprintf(cfgFileInfo.item1, "%d", hhh.getAccountCnt());	
}

void CallRadiusCfgWrite(char* file_path_name, command_t cfgFileInfo);
void CallFilterCfgWrite(char* file_path_name, command_t cfgFileInfo);
void CallBdsCfgWrite   (char* file_path_name, command_t cfgFileInfo);
void CallBlockCfgWrite (char* file_path_name, command_t cfgFileInfo);
void CallFileCfgWrite  (char* file_path_name, command_t cfgFileInfo);
void CallScpSCfgWrite  (                      command_t cfgFileInfo);
void CallScpRCfgWrite  (                      command_t cfgFileInfo);
void CallAggCfgWrite   (char* file_path_name, command_t cfgFileInfo);


/*******************************************************************************
* Desc : Client로 부터의 업무처리 메소드.
*        config file 수정을 한다. Client로 부터 수정 정보를 받아 File을 수정
*        한다.
* Argu : command_t cfgFileInfo - config file을 처리하기 위한 정보가 있다.
* Retn :
*******************************************************************************/
void CPMS::UpdateCfgFile(command_t cfgFileInfo)
{
	char file_path		[50+1];
	char file_name		[30+1];
	int  item_count;
	char file_id  		[ 4+1];
	char file_path_name	[70+1];

	item_count = 0;
	memset(file_path     , 0x00, sizeof(file_path     ));
	memset(file_name     , 0x00, sizeof(file_name     ));
	memset(file_id       , 0x00, sizeof(file_id       ));
	memset(file_path_name, 0x00, sizeof(file_path_name));

	// config파일 정보를 DB로 부터 읽어 온다.
	db._SelectCfgFile(cfgFileInfo, file_path, file_name, item_count, file_id);

	sprintf(file_path_name, "%s%s", file_path, file_name);

	// config 파일을 열어 파일의 내용을 수정한다.

	switch(file_id[3])
	{
		case '1':					// Radius Interface 1 file (IF)
			CallRadiusCfgWrite(file_path_name, cfgFileInfo);
			break;
		case '2':					// Radius Interface 2 file (Filter)
			CallFilterCfgWrite(file_path_name, cfgFileInfo);
			break;
		case '3':					// BDS    Interface 1 file (IF)
			CallBdsCfgWrite(file_path_name, cfgFileInfo);
			break;
		case '4':
			CallBlockCfgWrite(file_path_name, cfgFileInfo);
			break;
		case '5':
			CallFileCfgWrite(file_path_name, cfgFileInfo);
			break;
		case '6':
			CallScpSCfgWrite(cfgFileInfo);
			break;
		case '7':
			CallScpRCfgWrite(cfgFileInfo);
			break;
		case '8':
			CallAggCfgWrite(file_path_name, cfgFileInfo);		
			break;
		default:
			DebugLog(LOGFILE, "UpdateCfgFile call fail");
			//error log (wrong file id)
			ErrorLog(7005, "wrong file id. File Id -> [%s]", file_id);			
			return ;
			break;
	}

	// 수정한 내용을 DB로 Insert한다. SelectCfgFile메소드를 호출하여 DB Insert를 한다.
	SelectCfgFile(cfgFileInfo);
}

void CallRadiusCfgWrite(char* file_path_name, command_t cfgFileInfo)
{
	RadiusFileBlock  aaa(file_path_name);

	// 파일 수정
	aaa.setStartBlockNo(atoi(cfgFileInfo.item1));
	aaa.setTimeInVal   (atoi(cfgFileInfo.item2));
	aaa.write_RadiusFileBlock();
}

void CallFilterCfgWrite(char* file_path_name, command_t cfgFileInfo)
{
	FilterFileNo	 bbb(file_path_name);

	// 파일 읽어오기
	bbb.setStartBlockNo(atoi(cfgFileInfo.item1));
	bbb.write_FilterFileNo();
}

void CallBdsCfgWrite(char* file_path_name, command_t cfgFileInfo)
{
	BdsFileBlock 	 ccc(file_path_name);

	// 파일 수정
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

	// 파일 수정
	ddd.put_nBlockSize      (atoi(cfgFileInfo.item1));
	ddd.put_nBlockHeaderSize(atoi(cfgFileInfo.item2));
	ddd.put_nMaxRecCount    (atoi(cfgFileInfo.item4));
	ddd.put_unBlockSeq      (atoi(cfgFileInfo.item5));
	ddd.write_BlockConfigFile();
}

void CallFileCfgWrite(char* file_path_name, command_t cfgFileInfo)
{
	CFileConfigData  eee(file_path_name);

	// 파일 수정
	eee.put_nFileHeaderSize(atoi(cfgFileInfo.item1));
	eee.put_nOneBlockSize  (atoi(cfgFileInfo.item3));
	eee.put_nMaxBlockCount (atoi(cfgFileInfo.item4));
	eee.put_unFileSeq      (atoi(cfgFileInfo.item5));
	eee.write_FileConfigFile();
}

void CallScpSCfgWrite(command_t cfgFileInfo)
{
	ScpIF_TID fff(g_svcId);

	// 파일 수정
	fff.set_S_TID(atoi(cfgFileInfo.item1));
}

void CallScpRCfgWrite(command_t cfgFileInfo)
{
	ScpIF_TID ggg(g_svcId);

	// 파일 수정
	ggg.set_R_TID(atoi(cfgFileInfo.item1));
}

void CallAggCfgWrite (char* file_path_name, command_t cfgFileInfo)
{
	AccountCnt hhh(file_path_name);

	// 파일 수정
	hhh.setAccountCnt(atoi(cfgFileInfo.item1));
	hhh.write_AccountCnt();
}

/*******************************************************************************
* Desc : PMS 업무.
*        LogManager의 테이블 상태값을 변경한다.
*        Client로 부터 명령을 받고 LogManager 클래스를 사용하여 값 변경.
* Argu : command_t logInfo - Client로 부터 받은 명령코드
*******************************************************************************/
void CPMS::ChangeStatusLogManager(command_t logInfo)
{
	char strBuff[5];

	int nSystemId;
	int nServiceId;
	int nGroupId;
	int nProcessType;
	int nSerialNo;
	int nRemark;
	int nLogType;
	int flag;

	memset(strBuff, 0x00, sizeof(strBuff)); sprintf(strBuff, "%c"  ,  logInfo.subContent[0]); nSystemId   = atoi(strBuff);
	memset(strBuff, 0x00, sizeof(strBuff)); sprintf(strBuff, "%c"  ,  logInfo.subContent[1]); nServiceId   = atoi(strBuff);
	memset(strBuff, 0x00, sizeof(strBuff)); sprintf(strBuff, "%c"  ,  logInfo.subContent[2]); nGroupId     = atoi(strBuff);
	memset(strBuff, 0x00, sizeof(strBuff)); sprintf(strBuff, "%c"  ,  logInfo.subContent[3]); nProcessType = atoi(strBuff);
	memset(strBuff, 0x00, sizeof(strBuff)); sprintf(strBuff, "%.2s", &logInfo.subContent[4]); nSerialNo    = atoi(strBuff);
	memset(strBuff, 0x00, sizeof(strBuff)); sprintf(strBuff, "%.2s", &logInfo.subContent[6]); nRemark      = atoi(strBuff);
	memset(strBuff, 0x00, sizeof(strBuff)); sprintf(strBuff, "%.2s", &logInfo.subContent[8]); nLogType     = atoi(strBuff);
	memset(strBuff, 0x00, sizeof(strBuff)); sprintf(strBuff, "%c"  ,  logInfo.filePart     ); flag         = atoi(strBuff);

	DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"  );
	DebugLog(LOGFILE, "   Change Status Log Manager   "  );
	DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"  );
	DebugLog(LOGFILE, "  nSystemId    = %d", nSystemId   );
	DebugLog(LOGFILE, "  nServiceId   = %d", nServiceId  );
	DebugLog(LOGFILE, "  nGroupId     = %d", nGroupId    );
	DebugLog(LOGFILE, "  nProcessType = %d", nProcessType);
	DebugLog(LOGFILE, "  nSerialNo    = %d", nSerialNo   );
	DebugLog(LOGFILE, "  nRemark      = %d", nRemark     );
	DebugLog(LOGFILE, "  nLogType     = %d", nLogType    );
	DebugLog(LOGFILE, "  flag         = %d", flag        );
	DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"  );

	logSM->Set_Switch(nSystemId, nServiceId, nGroupId, nProcessType,
	                  nSerialNo, nRemark   , nLogType, flag);
}

/*******************************************************************************
* desc : read process info from DB(or file), make list
*******************************************************************************/
bool CPMS::GetProcessInfoFromDB()
{
	return db.FetchProcessInfoFromDb(0, ALL, NULL);
}

/*******************************************************************************
* Desc : PMS의 상태를 업데이트 하여 프로세스의 동작여부를 알 수 있다.
*******************************************************************************/
void CPMS::UpdatePmsProcStatus(char status)
{
	CNode *pNode;
	PPROCINFO pNodeData;

	pNode = procInfoList.GetHeadPosition();

	do
	{
		pNodeData = (PPROCINFO)procInfoList.GetAt(pNode);

		if(strcmp((*pNodeData).proc_name, PMSNAME) == 0)
		{
  			if (pthread_mutex_lock(&mutex))
		    	DebugLog(LOGFILE,"Can't lock mutex");

			pNodeData->proc_status = status;			// PMS 상태 정보 변경

			// PMS 종료시 pid 0으로.. 그 외일경우 pid setting
			if (status == 'T')
				pNodeData->sys_pid     = 0;
			else
				pNodeData->sys_pid     = getpid();

			db.UpdateProcStatusTable(0, *pNodeData);	// Update DB

  			if (pthread_mutex_unlock(&mutex))
  				DebugLog(LOGFILE,"Can't unlock mutex");

			// PMS 상태 정보 출력
			DebugLog(LOGFILE, "======[Update PMS]======");
			DebugLog(LOGFILE, " id     : [%s]", pNodeData->proc_name);
			DebugLog(LOGFILE, " status : [%c]", pNodeData->proc_status);
			DebugLog(LOGFILE, " pid    : [%d]", pNodeData->sys_pid);
			DebugLog(LOGFILE, " runCnt : [%d]", pNodeData->runCnt);
			DebugLog(LOGFILE, "========================");

			break;
		}
	} while(pNode = procInfoList.GetNextPosition(pNode));
}

/*******************************************************************************
* Desc : fork-exec에 의해 실행된 process의 정보들을 리스트로 관리하는데
*		 kill signal을 받았을때 모든 process를 종료시키는 함수
*        Type : G(roup), O(ne), A(ll)
*******************************************************************************/
bool SendSignalProcess(char type, char* processId, int signo)
{
	CNode *node;
	PPROCINFO pNodeData;

	node = procInfoList.GetHeadPosition();

	ShowList();				// 모든 프로세스 상태 표시

	do
	{
		pNodeData = (PPROCINFO)procInfoList.GetAt(node);

		if ( pNodeData->proc_status == STOPPED ||
		     pNodeData->sys_pid == getpid()	   ||
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

		pNodeData->exitFlag = 1;	// PMS가 프로세스 종료시 exitFlag 1로 셋팅

		if(signo == SIGSTOP || signo == SIGCONT )
		{
 			if (pthread_mutex_lock(&mutex))
				DebugLog(LOGFILE, "Can't lock mutex");

			switch(signo)
			{
				case SIGSTOP:
					pNodeData->proc_status = PAUSED;
					break;
				case SIGCONT:
					pNodeData->proc_status = RUNNING;
					break;
			}

			ptrThis->GetDb().UpdateProcStatusTable(0, *pNodeData);

  			if (pthread_mutex_unlock(&mutex))
    				DebugLog(LOGFILE,"Can't unlock mutex");
		}

		DebugLog(LOGFILE, "SEND SIGNAL : [%d] TO [%s(%d)]",
				 signo, pNodeData->proc_name, pNodeData->sys_pid);
	} while(node = procInfoList.GetNextPosition(node));

	return true;
}

/*******************************************************************************
* Desc : pms 시작시에 db로부터 읽은 process정보를 리스트로 관리하는데
*        리스트로부터 정보를 읽어서
*	     모든 프로세스를 fork-exec을 통해 실행하는 함수.
*	     선택한 type에 따라 실행하는 프로세스의 범위가 달라지고
*	     subContent는 type에 따라 group code/process id가 된다.
*
* Aagu : char type       -  A(ll), G(roup), O(ne)
*        char subContent - 세부 항목들.
*******************************************************************************/
void CPMS::ForkProcess(char type, char* subContent)
{
	CNode *pNode;
	PPROCINFO pNodeData;

	if (pthread_mutex_lock(&mutex))
		DebugLog(LOGFILE,"Can't lock mutex");

	pNode = procInfoList.GetHeadPosition();

	do
	{
		pNodeData = (PPROCINFO)procInfoList.GetAt(pNode);

		if( pNodeData->sys_pid == getpid())		// pms 자신일 경우..
			continue;

		if (type == GROUP && pNodeData->group_type != atoi(subContent))
			continue;

		if (type == ONE	&& strncmp(pNodeData->proc_name, subContent, sizeof(pNodeData->proc_name)) != 0)
		{
//			DebugLog(LOGFILE, "proc_name  : %s", pNodeData->proc_name);
//			DebugLog(LOGFILE, "subContent : %s", subContent);
			continue;
		}

		DebugLog(LOGFILE, "=====[Fork Process]=====");
		DebugLog(LOGFILE, " id     : [%s]", pNodeData->proc_name);
		DebugLog(LOGFILE, " status : [%c]", pNodeData->proc_status);
		DebugLog(LOGFILE, " pid    : [%d]", pNodeData->sys_pid);
		DebugLog(LOGFILE, " runCnt : [%d]", pNodeData->runCnt);
		DebugLog(LOGFILE, "========================");

		if ( !ForkOneProcess(pNodeData) )
		{
#ifdef _ERRLOG_
			//errcode (fail to fork process)
			memset(g_errorMsg, 0x00, sizeof(g_errorMsg));
			sprintf(g_errorMsg,
			"process name = %s, process status = %c, process pid = %d",
			(*pNodeData).id, (*pNodeData).status, (*pNodeData).pid);
			ErrorLogInsertDB(0, g_processType, g_processId, "6507",
			g_errorMsg);
#endif
			DebugLog(LOGFILE, "fail to Fork process (%s), (%s)",
			         pNodeData->prog_name, pNodeData->proc_name);
			         
			//error log (fail to fork process)
			ErrorLog(7002, "process name = %s, process status = %c, process pid = %d",
			pNodeData->proc_name, pNodeData->proc_status, pNodeData->sys_pid);
			
		} //end if
	} while(pNode = procInfoList.GetNextPosition(pNode));

	if (pthread_mutex_unlock(&mutex))
		DebugLog(LOGFILE,"Can't unlock mutex");
}

/*******************************************************************************
* Desc: 하나의 프로세스를 fork-exec으로 실행시키는 함수.
*
* Argu: process list의 하나의 node
*
* 주의사항 : 08.25
*	     fork후에 exec를 실패했을경우 process lis에는 process statu가
*	     RUNNING로 되어 있지만 실제 child는 실행되지 않은 상태이다.
*	     이점을 확인하는 모듈이 필요.
*******************************************************************************/
int CPMS::ForkOneProcess(PPROCINFO pNodeData, int dFlag)
{
	int pid;
	int i;
	char name[100];
	char strArgs[100];
	char strSharp[20];

	// 정상 종료외에 비정상 종료도 추가 될 수 있음...
	// 밑에 조건문 수정 사항이 생길 수 있음...
	if (pNodeData->proc_status != STOPPED &&
	    pNodeData->proc_status != KILLED  &&
	    pNodeData->proc_status != ABNORMAL)
	{
		DebugLog(LOGFILE, "(%s) is running ", pNodeData->proc_name);
		return TRUE;
	}

	pid = fork();

	if ( pid < 0 ) /* error */
	{
#ifdef _ERRLOG_
		//errcode (fail to fork process)
		memset(g_errorMsg, 0x00, sizeof(g_errorMsg));
		sprintf(g_errorMsg,
		"process name = %s, process status = %c, process pid = %d",
		(*pNodeData).id, (*pNodeData).status, (*pNodeData).pid);
		ErrorLogInsertDB(0, g_processType, g_processId, "6507",
		g_errorMsg);
#endif
		//error log (fail to fork process)
		ErrorLog(7002, "process name = %s, process status = %c, process pid = %d",
		pNodeData->proc_name, pNodeData->proc_status, pNodeData->sys_pid);
		
		DebugLog(LOGFILE, "fail to fork process ");
		return FALSE;
	}
	else
	if ( pid == 0) /* child */
	{
		sprintf(name, "%s%s", pNodeData->prog_path, pNodeData->prog_name);

		sprintf(strSharp, "%1d %1d %1d %1d %02d %02d", pNodeData->sys_id,
		pNodeData->svc_id, pNodeData->group_type, pNodeData->proc_type,
		pNodeData->proc_sn, pNodeData->proc_remark);

		switch (pNodeData->ip_cnt)
		{
			case 1:
				switch (pNodeData->channel_cnt)
				{
					case 1:
						sprintf(strArgs, "%s %s %s %d",
						pNodeData->proc_name,
						strSharp,
						pNodeData->p_ipaddr1,
						pNodeData->p_send1);
						break;
					case 2:
						sprintf(strArgs, "%s %s %s %d %d",
						pNodeData->proc_name,
						strSharp,
						pNodeData->p_ipaddr1,
						pNodeData->p_send1,
						pNodeData->p_recv1);
						break;
					default:
						sprintf(strArgs, "%s %s %s",
						pNodeData->proc_name,
						strSharp,
						pNodeData->p_ipaddr1);
						break;
				}//end switch
				break;
			case 2:
				switch (pNodeData->channel_cnt)
				{
					case 1:
						sprintf(strArgs, "%s %s %s %d %s %d",
						pNodeData->proc_name,
						strSharp,
						pNodeData->p_ipaddr1,
						pNodeData->p_send1,
						pNodeData->p_ipaddr2,
						pNodeData->p_send1);
						break;
					case 2:
						sprintf(strArgs, "%s %s %s %d %d %s %d %d",
						pNodeData->proc_name,
						strSharp,
						pNodeData->p_ipaddr1,
						pNodeData->p_send1,
						pNodeData->p_recv1,
						pNodeData->p_ipaddr2,
						pNodeData->p_send2,
						pNodeData->p_recv2);
						break;
					default:
						sprintf(strArgs, "%s %s %s %s",
						pNodeData->proc_name,
						strSharp,
						pNodeData->p_ipaddr1,
						pNodeData->p_ipaddr2);
				}//end switch
				break;
			default:
				sprintf(strArgs, "%s %s",
				pNodeData->proc_name, strSharp);
		}//end switch

//		if (pthread_mutex_unlock(&mutex))
//			DebugLog(LOGFILE, "Can't unlock mutex");

		if (execvp(name, GetStringList(strArgs)) < 0)
		{
#ifdef _ERRLOG_
			//errcode (create process error)
			memset(g_errorMsg, 0x00, sizeof(g_errorMsg));
			sprintf(g_errorMsg,
			"process name = %s, process id = %s, process pid = %d",
			name, pNodeData->id, (*pNodeData).pid);
			ErrorLogInsertDB(0, g_processType, g_processId, "6508",
			g_errorMsg);
#endif
			//error log (fail to fork process)
			ErrorLog(7002, "process name = %s, process id = %s, process pid = %d",
			name, pNodeData->proc_name, pNodeData->sys_pid);
			
//			cout << "create [" << name, << "][" << pNodeData->id
//               << " Error:[" strerror(errno) << "]" << endl;
			DebugLog(LOGFILE, "quit pms");
			clsLogM->put_EventHistData(FLAG_END, 1);
			exit(1);
		}//end if
	}//end if

	if (pid > 0) /* parent */
	{
		pNodeData->sys_pid = pid;
		pNodeData->proc_status = RUNNING;
		pNodeData->exitFlag = 0;	// default 0
		db.UpdateProcStatusTable(dFlag, *pNodeData);

		ShowList();

		DebugLog(LOGFILE, "success to run (%s) ", pNodeData->proc_name);
	}//end if

	return TRUE;
}

/*******************************************************************************
* Desc : 요청한 명령의 유효 여부 검사. 요청한 명령의 적용범위의 유효 여부 검사.
* Argu : pcommand_t command : Client로부터 받은 명령 구조체
* Retn : true  - Right command, Right Type
*        false - Wrong command, Wrong Type
*******************************************************************************/
bool CPMS::IsRightCommand(pcommand_t const command)
{
	bool retFlag;

	// 요청한 명령의 유효 여부 검사.
	switch(command->command)
	{
		case START:
		case STOP:
		case OPERATION:
			retFlag = true;
			break;
		default:
			DebugLog(LOGFILE, "wrong command [%c]", command->command);
			//error log (wrong client_pms command)
			ErrorLog(7004, "wrong command [%c]", command->command);
			retFlag = false;
	}

	if (retFlag == false)
		return retFlag;

	// 요청한 명령의 적용범위의 유효 여부 검사.
	switch(command->type)
	{
		case ALL:
		case GROUP:
		case ONE:
		case SELECT:
		case UPDATE:
		case DISK:
		case CPU:
		case LOGMANAGER:
			retFlag = true;
			break;
		default:
			DebugLog(LOGFILE, "wrong type [%c]", command->type);
			//error log (wrong client_pms command)
			ErrorLog(7004, "wrong type [%c]", command->type);
			retFlag = false;
	}
	return retFlag;
}

/*******************************************************************************
* Desc : PMS 종료.
*        1. 모든 child 프로세스를 종료 시킨다.
*        2. /bin/uda1stop.HaFlag파일을 생성한다.
*        3. PMS를 종료 한다.
*******************************************************************************/
// 시스템에 따라서 변경
void CPMS::ExitPMS()
{
	// PMS 종료 플래그
	char	EXITFLAGFILE[50];
	sprintf(EXITFLAGFILE, "touch /home/uda%d/bin/uda%dstop.HaFlag", g_sysId, g_sysId);
	
	DebugLog(LOGFILE, "");
	DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	DebugLog(LOGFILE, "            PMS EXIT !!         ");
	DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	DebugLog(LOGFILE, "");
	
	void *status;

	//pms가 죽을때 child를 종료시킴.
	SendSignalProcess(ALL, NULL, SIGUSR1);	// SIGUSR1을 받으면 child들은 종료.

	// 루프 카운트 추가 - 에러로그 추가
	int reCnt = 0;
	while(IsAllProcessStopped() == false) // child들이 모두 종료할때까지 기다림.
	{
		if (reCnt == 10)
		{
			// 에러 메세지 출력!!
			break;
		}
		sleep(1);
		reCnt++;
	}

	CPMS::SetStopFlag(0);	// PMS STOP

	sleep(5);

	if (pthread_mutex_unlock(&mutex))
		DebugLog(LOGFILE, "Can't unlock mutex");

	pthread_join(idWaitThread           , &status);
	pthread_join(idProcCheckThread      , &status);
	pthread_join(idMQThread             , &status);
	pthread_join(idOperationThread      , &status);
	pthread_join(idBackupOperationThread, &status);

	DebugLog(LOGFILE, "join all thread ");

	system(EXITFLAGFILE);	// 종료 플래그 파일 생성
							// 종료 플래그는 언제 생성해야 하나???

	DebugLog(LOGFILE, "before sleep");
	DebugLog(LOGFILE, "wait for a minute....." );
}

/*******************************************************************************
* Desc : 스트링으로부터 token으로 구분되어진 첫 단어를 return하는 함수
* Argu : 하나이상의 token으로 구성되어진 문자열
* Retn : src - token으로 구분되어지는 첫 단어.
*******************************************************************************/
char** GetStringList(char*  src)
{
	char* str;
	char* tmp = (char *) 0;
	char** ar = (char **) malloc(10*10);

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
* Desc : stop요청이 있을때 모든 child process의 종료를 wait해야한다.
*		 모든 프로세스가 종료 되지 않았다면 종료될때까지
*		 wait thread는 종료되지 않는다.
* Retn : true  - 모든 child 프로세스가 동작 하지 않는다.
*        false - child 프로세스가 동작 중이다.
*******************************************************************************/
bool IsAllProcessStopped()
{
	CNode *pNode;
	PPROCINFO pNodeData;

	pNode = procInfoList.GetHeadPosition();

	do
	{
		pNodeData = (PPROCINFO)procInfoList.GetAt(pNode);

		//자기자신의 종료 정보는 체크하지 않는다.
		if(pNodeData->sys_pid == getpid() )
			continue;

		if (pNodeData->proc_status == RUNNING)
			return false;
	}while(pNode = procInfoList.GetNextPosition(pNode));

	return true;
}

/*******************************************************************************
* Desc : child가 종료를 wait하는 thread를 생성.
*******************************************************************************/
void CreateWaitChildStatusThread()
{
	if (pthread_create(&idWaitThread, NULL, &WaitChildStatusThread, (void *)NULL) == 0)
		DebugLog(LOGFILE, "Create CheckWaitChildStatusThread");
	else
	{
#ifdef _ERRLOG_
		//errcode (fail to create Check Wait Thread)
		memset(g_errorMsg, 0x00, sizeof(g_errorMsg));
		sprintf(g_errorMsg, "WaitThread id = %d", idWaitThread);
		ErrorLogInsertDB(0, g_processType, g_processId, "6512", g_errorMsg);
#endif
		//error log (fail to create thread)
		ErrorLog(7006, "WaitThread id = %d", idWaitThread);
		
		DebugLog(LOGFILE,"Fail to Create Check Wait Child Status Thread");
	}
}

/*******************************************************************************
* Desc : fork-exec을 통해 실행된 process list들의 종료를 wait하는 함수
*		 종료상태를 check하여 결과를 db에 저장한다.
*******************************************************************************/
void *WaitChildStatusThread(void *a)
{
	int status;				// child로부터 받은 종료상태 정보
	int exit_status;		// 종료 상태
	int sig_status;			// 시그널 번호
	int nStop  = 1; 		// 모든 프로세스가 종료되기를 기다림
	bool abnormalFlag;		// 비정상 종료 체크 플래그

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
					exit_status = WEXITSTATUS(status);	// 종료 상태 번호
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
				else		// SIGKILL, SIGSTOP과 미정의된 시그널
				{
					sig_status = WTERMSIG(status);	// 시그널 번호

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

				// 네트워크 상태 업데이트. 종료상태이브로
				// 네트워크 상태도 에러상태로 업데이트 함.
				if (pNodeData->net_status == REGNETWORK)
				{
					pNodeData->net_status = ERRNETWORK;
					db.UpdateProcStatusTable(1, *pNodeData, 'N');
				}

				// 비정상 종료시 PMS에서 재기동 및 PMS종료 판단 루틴.
				if (abnormalFlag == true)
				{
					pNodeData->runCnt = pNodeData->runCnt + 1;	// start시 runCnt 1씩 증가

					// 0이외에 5번이상 비정상 종료시 모든 child process 종료 후
					// PMS종료.  HA로 제어권을 넘김
					if (pNodeData->runCnt > 5 && pNodeData->exitFlag == 0)
						ptrThis->ExitPMS();
					else
					if (CPMS::GetStopFlag() != 0 && pNodeData->exitFlag == 0)
						ptrThis->ForkOneProcess(pNodeData, 1);
					else
					if (pNodeData->runCnt > 0) 	// start시 runCnt 1씩 증가
						pNodeData->runCnt = pNodeData->runCnt - 1;
				}//end if
			}//end if
		}while(pNode = procInfoList.GetNextPosition(pNode));

  		if (pthread_mutex_unlock(&mutex))
	      	DebugLog(LOGFILE,"Can't unlock mutex");

		sleep(2);

	}//end while

	if (db.DisconnectDBMultiThread(1))
		DebugLog(LOGFILE,"success to disconnect DB in wait thread");
	else
	{
#ifdef _ERRLOG_
		//errcode (disconnect db error)
		memset(g_errorMsg, 0x00, sizeof(g_errorMsg));
		sprintf(g_errorMsg, "Wait Child Status Thread. disconnect db error");
		ErrorLogInsertDB(0, g_processType, g_processId, "6511", g_errorMsg);
#endif
		DebugLog(LOGFILE,"fail to disconnect DB in wait thread");
	}

	pthread_exit(NULL);

	return NULL; // use just to prevent warning
}

/*******************************************************************************
* Desc : 프로세스 상태 체크 스레드 생성
*******************************************************************************/
void CreateProcessCheckThread()
{
	if (pthread_create(&idProcCheckThread, NULL, &ProcessCheckThread, (void *)NULL) == 0)
		DebugLog(LOGFILE, "Create Process Check Thread");
	else
	{
#ifdef _ERRLOG_
		//errcode (fail to create CheckMQThread)
		memset(g_errorMsg, 0x00, sizeof(g_errorMsg));
		sprintf(g_errorMsg, "Process Check Thread id = %d", idProcCheckThread);
		ErrorLogInsertDB(0, g_processType, g_processId, "6512", g_errorMsg);
#endif
		//error log (fail to create thread)
		ErrorLog(7006, "Process Check Thread id = %d", idProcCheckThread);
		DebugLog(LOGFILE, "Fail to Create ProcessCheckThread");
	}
}

/*******************************************************************************
* Desc : 프로세스 상태 체크 스레드
*        현재 status가 RUNNING중인 프로세스에 한해 시스템 명령 ps로
*        프로세스의 상태를 체크 동작중이면 넘어가고 동작중이 아닐시에는
*        status를 STOPPED로 업데이트 한다.
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
//		DebugLog(LOGFILE, " Proc Check Thread 입니다.");
//		DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

		do
		{
			if (CPMS::GetStopFlag() == 0)
				break;

			pNodeData = (PPROCINFO)procInfoList.GetAt(pNode);

			// pid가 PMS이거나 상태가 STOP이거나 pid 0이거나 process name이
			// 없을 경우 process check를 하지 않음.
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
	{
#ifdef _ERRLOG_
		//errcode (disconnect db error)
		memset(g_errorMsg, 0x00, sizeof(g_errorMsg));
		sprintf(g_errorMsg, "Process Check Thread. disconnect db error");
		ErrorLogInsertDB(0, g_processType, g_processId, "6511", g_errorMsg);
#endif
		DebugLog(LOGFILE, "fail to disconnect DB in Process Check Thread");
	}

	pthread_exit(NULL);

	return NULL;
}

/*******************************************************************************
* 프로세스 동작 상태 체크
* return : -1 - pipe open error
*           1 - 프로세스 정상 작동
*           0 - 프로세스 정지.(없음)
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
* 파일 로그 남김 (선언은 CPmsCommon.hpp에..)
*******************************************************************************/
bool ErrorLog(int errCode, char* format, ...)
{
	char strMessage[4000];
	char strFormat [2000];

	va_list var;
	va_start( var, format );

	 sprintf( strFormat, "%s", format );		// make output format
	vsprintf( strMessage, strFormat, var );		// SetFileName메소드로 지정.
	
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
	vsprintf( strMessage, strFormat, var2 );	// SetFileName메소드로 지정.
	
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
	if ( fp = fopen(fname, "a+") )	// m_fname은 멤버변수.
	{								// m_fname은 생성자에서나
		vfprintf( fp, out, var );	// SetFileName메소드로 지정.
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
* Desc : fork-exec을 통해 실행된 process로부터 상태정보를 전송받기 위해
*        message queue로부터 message를 wait하는 함수를 thread로 생성시키는 함수.
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
* Desc : fork-exec을 통해 실행된 process로부터 상태정보를
*	     전송받기 위해 message queue로부터 message를 wait하는 함수,
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

   						// 프로세스가 죽을때 네트워크 상태도 같이 업데이트 시킴.
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
//		DebugLog(LOGFILE, "     MQ Thread 입니다."    );
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
* Desc : PMS업무처리 thread
*        20초마다 네트워크 에러 플래그 파일을 체크한다.
*        네트워크 이상시 child프로세스를 모두 종료한후 PMS도 종료한다.
*        pms1stop.HaFlag파일을 만들어 HA에게 PMS가 종료함을 알린다.
*
*        1시간마다 Disk와 CPU의 상태 정보를 DB Insert한다.
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
* Desc : PMS의 업무처리를 위한 Thread
*******************************************************************************/
#define NETWAITTIME			    20		//   20 second
#define SYSINFOWAITTIME		  3600		// 3600 second

void* OperationThread(void *a)
{
	time_t netErrChkST, netErrChkET;// Network 오류 플래그 파일 체크 time
	time_t sysInfoST  , sysInfoET;	// system disk, cpu 상태 정보 업데이트 체크 time

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

		// Network 오류 체크 ***************************************************
		if (netErrChkFlag)
		{
			time(&netErrChkST);
			netErrChkFlag = false;
		}

		time(&netErrChkET);

		if ( (netErrChkET - netErrChkST) >= NETWAITTIME )	// 20초마다 체크
		{
			// HA의 네트워크 오류 플래그 파일 체크.
			if (access(stopFileNAME, F_OK) == 0)
			{
				DebugLog(LOGFILE, "");
				DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
				DebugLog(LOGFILE, "    pms%dstop.HaFlag Check...   ", g_sysId);
				DebugLog(LOGFILE, "    PMS EXIT !!                 ");
				DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
				DebugLog(LOGFILE, "");
				
				ptrThis->ExitPMS();		// 파일이 있으면 PMS 종료. ExitPMS 호출
				sprintf(stopFileNAME, "rm /home/uda%d/bin/pms%dstop.HaFlag",
				        g_sysId, g_sysId);
				system(stopFileNAME);	// PMS STOP Flag File 삭제
			}
			netErrChkFlag = true;
		}
		/**********************************************************************/

		// Disk, CPU 상태 정보 업데이트 ****************************************
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

			ptrThis->InsertSysInfo(systemDate, 1);			// Disk 상태 정보 업데이트
			ptrThis->InsertSysInfo(systemDate, 2);			// CPU  상태 정보 업데이트
			sysInfoFlag = true;
		}
		/**********************************************************************/
	}//end while

	pthread_exit(NULL);
	return NULL;
}

/*******************************************************************************
* Desc : PMS 업무 처리 thread를 생성한다.
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
* Desc : 파일 백업 배치 작업을 처리 한다.
*        DB table T_BACKUP_INFO를 참조하여 backup관련 정보를 읽어온다.
*        백업 시간을 체크하여 하루 한번 백업시간대에 백업 프로세스를 호출한다.
*        배치 작업이며 DB의 정보가 변경될 소지가 있기때문에 sleep 1시간씩 주어
*        테이블의 정보를 refresh한다.
*******************************************************************************/
void* BackupOperationThread(void *a)
{
	time_t backupST, backupET;	// Backup Info Table Load 시간 설정
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

	// 변수 초기화
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
		
		if ( (backupET - backupST) >= 1 )		// 1시간 한번 실행 3600으로 수정!!
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
							// 에러메세지 출력!!
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
