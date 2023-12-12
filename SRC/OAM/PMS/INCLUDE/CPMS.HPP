/*******************************************************************************
* file : Cpms.cpp
*
* PMS Main Ŭ����.
* Thread���� �� ���μ��� start, stop, ���� ������Ʈ, ����ó�� ��
* PMS�� ��� ó���� �Ѵ�.
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
	static int nStop;		// process�� stop flag. 1=run, 0=stop
	LogManageSM *logSM;		// Log Manager
	char **m_av;			// main argument
	
public:
	CPmsDbHandler db;		// DB Handling
	static int GetStopFlag()			{ return nStop;  }	// return nStop
	static void SetStopFlag(int Flag)	{ nStop = Flag;  }	// Set	  nStop
	CPmsDbHandler GetDb()				{ return db;     }	// db��ü return

public :
	 CPMS();	// �ʱ�ȭ
	~CPMS();	// �޸� ����

	void Initialize(int ac, char** av);						// PMS �ʱ�ȭ.
	
	// Log Manager Start. 2000. 12. 08�� �߰�.
	void StartLogManager();	
	
	void StartResourceMGR();								// Resource Manager ȣ��
	void ArgCheck(int ac, char** av);						// argument check
	void ProcessCommand();									// PMS Main
	
	bool GetProcessInfoFromDB();							// DB�� ���� process ���� fetch �� list�ۼ�
	void ForkProcess(char type, char* subContent, int serv = -9);// start process
	int ForkOneProcess(PPROCINFO nodeData, int dFlag=0);	// start process
	
	void RegisterSignalHandler();							// signal handling

	bool IsRightCommand(pcommand_t const command);			// command�� �ùٸ��� üũ�Ѵ�.
	void UpdatePmsProcStatus(char status);					// PMS ���� ������Ʈ
	
	void ExitPMS();											// PMS ���� ó�� (���μ����� �������϶�, ��Ʈ���� �������϶�)
	
	//=[����ó�� �޼ҵ�]========================================================
	// Client�� ���� ���� ���� ó�� (process, start, stop, �������� ó��)
	void ExecuteCommand(char command, char type, char* subContent, char* service = "none");
	
	// disk, cpu ��뷮 ���� ó��
	void InsertSysInfo(char* cliDate, int infoType);
	
	// Config File ��ȸ �� ���� ó��
	void SelectCfgFile(command_t cfgFileInfo);
	void UpdateCfgFile(command_t cfgFileInfo);
	
	// Log Manager Switch ����
	void ChangeStatusLogManager(command_t logInfo);
	
	// ���� �ѵ� ���� ����
	void UserManagement(command_t loginfo);
	//==========================================================================
	
	void StartTimeSet();
};

#endif //_CPMS_H