#include "Cpms.hpp"
#include "CpmsFunc.hpp"
#include <TempMsgQ_new.hpp>

/*******************************************************************************
* Desc : process ���� signal�� �޾����� signal handler�� �б��ؼ�
*        �ٷ� �����ع����ٸ� signal catch�ÿ� interrupt�Ǿ��� system call
*        ������ core dump�� �߻��ϴ� ��찡 ���� �߻��Ѵ�.
*        �����ϰ� �����ϱ� ���ؼ� flag����� �ϴ� ������ ����.
*******************************************************************************/
int CPMS::nStop  = 1;

CPMS::CPMS()
{
	ptrThis = this;

	logSM = new LogManageSM(1, 1, 1, 2, 0, 0);	// �ʱ⼱�� ��
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
* Desc : PMS �ʱ�ȭ �۾��� �Ѵ�.
*		 - RegisterSignalHandler() : signal �߻��� ó���ؾ� �� �κе��� ����
*		 - GetProcessInfoFromDB()  : process ������ db�κ��� ������
*	 	 - ForkProcess()		   : process ������ ������ process ����
*		 - CreateCheckMQThread()   : child process�� ������ ������ message
*				     				 queue�� ���ؼ� ���۹���.
*        �ñ׳� �ڵ鷯 �ʱ�ȭ
*        argument check
*        wait child thread ����, process check thread ����, MQ thread ���� ...
* Argu : int    ac - main���κ��� ���� argc
*        char** av - main���κ��� ���� argv
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

	CreateWaitChildStatusThread();	// child���� ���� ���¸� üũ�ϴ� thread
									// child�� ���� ���¸� ������.

	CreateProcessCheckThread();		// child���� ���� ���¸� üũ�ϴ� thread
									// system���� ps�� ����Ͽ� ���� ���¸� üũ�Ѵ�.

	CreateCheckMQThread();			// child���� ���� ���¸� üũ�ϴ� thread
									// Message Queue�� child���� ���¸� ������.

	CreateOperationThread();		// ���� ó�� thread
									// HA�� ��Ʈ��ũ ���� �÷��� ������ üũ ��
									// ��ũ ����, cput���¸� DB insert�Ѵ�.
									// PMS �� child ���� ���θ� �����Ѵ�.

	CreateBackupOperationThread();	// ���� ó�� thread
								    // ���� ��� ó���� �Ѵ�.

	UpdatePmsProcStatus(RUNNING);	// update pms process info (2000.09.18)

	ExecuteCommand(START, ONE, LOGMGR);
	
	if (av[2][0] == '1')				// HA�� ���� �����Ǿ��� ���
		ExecuteCommand(START, ALL, " ");	// ��� ���μ��� �⵿
}

/*******************************************************************************
* Desc : Resource Manager�� �⵿��Ų��.
*        Resource Manager�� �⵿���� ������ �ý��� ��ü ���μ������� �⵿�ص�
*        ����� ó������ ���ϰ� �Ǳ� ������ �⵿�� �ȵɽ� 3���� �õ��Ѵ�.
*        3�� �õ��� �⵿�� ���� ������ PMS�� �����ϴ� ��� ���μ�������
*        �����Ű���� �Ѵ�.
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

	if (IsAllProcessStopped() == false)	// child�� ���� ���̴�.
		procFlag = true;

	if(procFlag == true)	// �ڽ����μ����� �⵿���Ͻ� Resource Manager��
	{						// �⵿���� ����.
		DebugLog(LOGFILE, "*********************************");
		DebugLog(LOGFILE, "  Resource Manager running ....");
		DebugLog(LOGFILE, "*********************************");
		return ;
	}

	int cnt, reCnt;			// ��õ� ī��Ʈ
	bool runFlag = false;	// �⵿ ���� üũ false�� �⵿ ����, true�� �⵿ ����
	pid_t resourceMGRPid;	// Resource Manager�� pid
	int retStatus;			// ���� ����

	bool resourceFlag = false;

	// Resource Manger�� ����� ������� ������ 3�� ��õ� �Ѵ�.
	for(cnt = 1; cnt <= 3; cnt++)
	{
		resourceMGRPid = fork();

		switch(resourceMGRPid)
		{
			case -1:				// fork, exec ���н�
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

				// ó�� �Ϸ�ɶ����� blocking �ϸ� ��ٸ�
				while(waitpid(resourceMGRPid, &retStatus, WNOHANG) != resourceMGRPid)
				{
					if (reCnt == 10)
					{
						// �����޼��� ���!!
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
	else										// �⵿���н� PMS ���� ��� ����
	{
		DebugLog(LOGFILE, "Resource Manager no running........ PMS QUIT, pid = %d, status = %d",
		resourceMGRPid, WEXITSTATUS(retStatus));
		
		//error log (PMS quit)
		ErrorLog(7003, "Resource Manager no running........ PMS QUIT, pid = %d, status = %d",
		resourceMGRPid, WEXITSTATUS(retStatus));
		
		DebugLog(LOGFILE, "quit pms");
		clsLogM->put_EventHistData(FLAG_END, 1);
		exit(1);	// PMS ����
 	}
}

/*******************************************************************************
*Desc: signal �߻��� ó���ؾ� �� �κе��� ����
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
*        ù��° ���ڴ� �ý��� ����. 1=UDA1, 2=UDA2
*        �ι�° ���ڴ� �ý��� ���� ���� ����. 0�̸� PMS����Ʈ�۾�(PMS, LOGMGR run)
*        1�̸� �ý��� �����۾� PMS, LOGMGR �� ��ü ���μ��� run. All Start
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
*Desc : oracle pipe�κ��� msg�� �����Ͽ� �ʿ��� ó���� ��
*
*argument : thread ������ ���Ǵ� argument.
*		    �� �Լ������� ������ ������ �����Ͻ� warning �� ���� ���ؼ�
*		    �����.
*return : �� �Լ������� ������ ������ �����Ͻ� warning �� ���� ���ؼ�
*	  	  �����.
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
						// ���μ����� �ϳ��� �������� ��� config���� ��������.
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
* Desc : client�� ��û�� ������ ������ ���� ó���ϴ� �Լ� ȣ��.
* Argu : char  command    - ��û ����
*        char  type       - ������ ���� ����
*		 char* subContent - ������ ���� ������� ���ǵ�.
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
* Desc : Client�� ������ ����ó�� �޼ҵ�.
*        system�� disk, cpu��� ������ DB�� Insert �Ѵ�.
*        sysinfo���μ����� ȣ��. (���� �۾�ó���� sysinfo�� �Ѵ�)
*		 sysinfo���μ����� argument
*		 infotype : 1(filesystem), 2(cpu)
*        date     : date (YYYYMMDDhhmmss[14])
*        systemid : uda1, uda2 (1, 2)
* Argu : char cliDate - Client�� ���� ���� Date. Client���� key�� ����Ѵ�.
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
* Desc : Client�� ������ ����ó�� �޼ҵ�.
*        config file ��ȸ�� �Ѵ�. File������ �о�鿩 DB�� Insert �Ѵ�.
* Argu : command_t cfgFileInfo - config file�� ó���ϱ� ���� ������ �ִ�.
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

	// config���� ������ DB�� ���� �о� �´�.
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
* Desc : Client�� ������ ����ó�� �޼ҵ�.
*        config file ������ �Ѵ�. Client�� ���� ���� ������ �޾� File�� ����
*        �Ѵ�.
* Argu : command_t cfgFileInfo - config file�� ó���ϱ� ���� ������ �ִ�.
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

	// config���� ������ DB�� ���� �о� �´�.
	db._SelectCfgFile(cfgFileInfo, file_path, file_name, item_count, file_id);

	sprintf(file_path_name, "%s%s", file_path, file_name);

	// config ������ ���� ������ ������ �����Ѵ�.

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

	// ������ ������ DB�� Insert�Ѵ�. SelectCfgFile�޼ҵ带 ȣ���Ͽ� DB Insert�� �Ѵ�.
	SelectCfgFile(cfgFileInfo);
}

/*******************************************************************************
* Desc : PMS ����.
*        LogManager�� ���̺� ���°��� �����Ѵ�.
*        Client�� ���� ������ �ް� LogManager Ŭ������ ����Ͽ� �� ����.
* Argu : command_t logInfo - Client�� ���� ���� �����ڵ�
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
* Desc : PMS�� ���¸� ������Ʈ �Ͽ� ���μ����� ���ۿ��θ� �� �� �ִ�.
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

			pNodeData->proc_status = status;			// PMS ���� ���� ����

			// PMS ����� pid 0����.. �� ���ϰ�� pid setting
			if (status == 'T')
				pNodeData->sys_pid     = 0;
			else
				pNodeData->sys_pid     = getpid();

			db.UpdateProcStatusTable(0, *pNodeData);	// Update DB

  			if (pthread_mutex_unlock(&mutex))
  				DebugLog(LOGFILE,"Can't unlock mutex");

			// PMS ���� ���� ���
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
* Desc : pms ���۽ÿ� db�κ��� ���� process������ ����Ʈ�� �����ϴµ�
*        ����Ʈ�κ��� ������ �о
*	     ��� ���μ����� fork-exec�� ���� �����ϴ� �Լ�.
*	     ������ type�� ���� �����ϴ� ���μ����� ������ �޶�����
*	     subContent�� type�� ���� group code/process id�� �ȴ�.
*
* Aagu : char type       -  A(ll), G(roup), O(ne)
*        char subContent - ���� �׸��.
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

		if( pNodeData->sys_pid == getpid())		// pms �ڽ��� ���..
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
* Desc: �ϳ��� ���μ����� fork-exec���� �����Ű�� �Լ�.
*
* Argu: process list�� �ϳ��� node
*
* ���ǻ��� : 08.25
*	     fork�Ŀ� exec�� ����������� process lis���� process statu��
*	     RUNNING�� �Ǿ� ������ ���� child�� ������� ���� �����̴�.
*	     ������ Ȯ���ϴ� ����� �ʿ�.
*******************************************************************************/
int CPMS::ForkOneProcess(PPROCINFO pNodeData, int dFlag)
{
	int pid;
	int i;
	char name[100];
	char strArgs[100];
	char strSharp[20];

	// ���� ����ܿ� ������ ���ᵵ �߰� �� �� ����...
	// �ؿ� ���ǹ� ���� ������ ���� �� ����...
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
* Desc : ��û�� ������ ��ȿ ���� �˻�. ��û�� ������ ��������� ��ȿ ���� �˻�.
* Argu : pcommand_t command : Client�κ��� ���� ���� ����ü
* Retn : true  - Right command, Right Type
*        false - Wrong command, Wrong Type
*******************************************************************************/
bool CPMS::IsRightCommand(pcommand_t const command)
{
	bool retFlag;

	// ��û�� ������ ��ȿ ���� �˻�.
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

	// ��û�� ������ ��������� ��ȿ ���� �˻�.
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
* Desc : PMS ����.
*        1. ��� child ���μ����� ���� ��Ų��.
*        2. /bin/uda1stop.HaFlag������ �����Ѵ�.
*        3. PMS�� ���� �Ѵ�.
*******************************************************************************/
// �ý��ۿ� ���� ����
void CPMS::ExitPMS()
{
	// PMS ���� �÷���
	char	EXITFLAGFILE[50];
	sprintf(EXITFLAGFILE, "touch /home/uda%d/bin/uda%dstop.HaFlag", g_sysId, g_sysId);
	
	DebugLog(LOGFILE, "");
	DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	DebugLog(LOGFILE, "            PMS EXIT !!         ");
	DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	DebugLog(LOGFILE, "");
	
	void *status;

	//pms�� ������ child�� �����Ŵ.
	SendSignalProcess(ALL, NULL, SIGUSR1);	// SIGUSR1�� ������ child���� ����.

	// ���� ī��Ʈ �߰� - �����α� �߰�
	int reCnt = 0;
	while(IsAllProcessStopped() == false) // child���� ��� �����Ҷ����� ��ٸ�.
	{
		if (reCnt == 10)
		{
			// ���� �޼��� ���!!
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

	system(EXITFLAGFILE);	// ���� �÷��� ���� ����
							// ���� �÷��״� ���� �����ؾ� �ϳ�???

	DebugLog(LOGFILE, "before sleep");
	DebugLog(LOGFILE, "wait for a minute....." );
}