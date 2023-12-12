#include "Cpms.hpp"
#include "CpmsFunc.hpp"
#include <TempMsgQ_new.hpp>

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

	ExecuteCommand(START, ONE, LOGMGR);
	
	if (av[2][0] == '1')				// HA에 의해 절차되었을 경우
		ExecuteCommand(START, ALL, " ");	// 모든 프로세스 기동
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
