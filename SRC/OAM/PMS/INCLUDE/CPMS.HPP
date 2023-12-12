/*******************************************************************************
* file : Cpms.cpp
*
* PMS Main 클래스.
* Thread생성 및 프로세스 start, stop, 상태 업데이트, 업무처리 등
* PMS의 모든 처리를 한다.
* Version 1.1 ( 2001. 05. 21 )
*******************************************************************************/
#ifndef _CPMS_H
#define _CPMS_H

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fstream.h>
#include <BlockFile.hpp>
#include <LogManageSM.hpp>
#include <CdsCustFormat.hpp>
#include "ptrList.hpp"
#include "CPmsDbHandler.hpp"

// PMS main class
class CPMS
{
private:
	static int nStop;		// process의 stop flag. 1=run, 0=stop
	LogManageSM *logSM;		// Log Manager
	char **m_av;			// main argument
	
public:
	CPmsDbHandler db;		// DB Handling
	static int GetStopFlag()			{ return nStop;  }	// return nStop
	static void SetStopFlag(int Flag)	{ nStop = Flag;  }	// Set	  nStop
	CPmsDbHandler GetDb()				{ return db;     }	// db객체 return

public :
	 CPMS();	// 초기화
	~CPMS();	// 메모리 정리

	void Initialize(int ac, char** av);						// PMS 초기화.
	
	// Log Manager Start. 2000. 12. 08일 추가.
	void StartLogManager();	
	
	void StartResourceMGR();								// Resource Manager 호출
	void ArgCheck(int ac, char** av);						// argument check
	void ProcessCommand();									// PMS Main
	
	bool GetProcessInfoFromDB();							// DB로 부터 process 정보 fetch 및 list작성
	void ForkProcess(char type, char* subContent, int serv = -9);// start process
	int ForkOneProcess(PPROCINFO nodeData, int dFlag=0);	// start process
	
	void RegisterSignalHandler();							// signal handling

	bool IsRightCommand(pcommand_t const command);			// command가 올바른지 체크한다.
	void UpdatePmsProcStatus(char status);					// PMS 상태 업데이트
	
	void ExitPMS();											// PMS 종료 처리 (프로세스가 비정상일때, 네트웍이 비정상일때)
	
	//=[업무처리 메소드]========================================================
	// Client로 부터 받은 명령 처리 (process, start, stop, 업무명령 처리)
	void ExecuteCommand(char command, char type, char* subContent, char* service = "none");
	
	// disk, cpu 사용량 정보 처리
	void InsertSysInfo(char* cliDate, int infoType);
	
	// Config File 조회 및 수정 처리
	void SelectCfgFile(command_t cfgFileInfo);
	void UpdateCfgFile(command_t cfgFileInfo);
	
	// Log Manager Switch 변경
	void ChangeStatusLogManager(command_t logInfo);
	
	// 고객 한도 정보 관리
	void UserManagement(command_t loginfo);
	//==========================================================================
	
	void StartTimeSet();
};

#endif //_CPMS_H
