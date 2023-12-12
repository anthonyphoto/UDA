#ifndef _LOGMANAGESM_HPP
#define _LOGMANAGESM_HPP

#ifndef	SHAREDMEMLEGACY_HPP
#include <SharedMemLegacy.hpp>
#endif

//#include <TempMsgQ_new.hpp>
#include <TempMsgQ_file.hpp>
#include <LogManage.hpp>

//--------------------------------------------------------------------
class LogManageSM  : public ShmMemory
{
	private:
		int				LogN;	// ���̺��� ����� ���ڵ尳��
		int				recSize;
		unsigned long*	unTempBaseAddr;

		LogManage*		LogDB;	// ���̺��� Access �Ͽ� UPdate
		LogManageRec	LogRecord;


	public:

		LogManageSM(  	int		nSystemId,		\
						int		nServiceId,		\
						int		nGroupId,		\
						int		nProcessType,	\
						int		nSerialNo,		\
						int		nRemark,		\
						char*	strPath,		\
						int		nCount=1000		\
					):ShmMemory	(	strPath,						\
									'A',							\
									sizeof(LogManageRec) * nCount	\
								)
					{
						LogDB = new LogManage(nSystemId,nServiceId,nGroupId,nProcessType,nSerialNo,nRemark);
					}


		~LogManageSM(){delete LogDB;}

		void LogSMinit(void);	// T_LOG_MANAGE ���̺����� ���� �о�� ShareMem �ʱ�ȭ

		int Set_SysSwitch(int sysid, int flag);
		int Set_ServSwitch(int sysid, int serviceid, int flag);
		int Set_GroupSwitch(int sysid, int serviceid, int grouptype, int flag);
		int Set_ProcesSwitch(int sysid, int serviceid, int grouptype, int processtype, int flag);
		int Set_Switch(int sysid, int serviceid, int grouptype, int processtype, int serialno,
						int remark, int logtype, int flag);

		int LogInsert(int sysid, int serviceid, int grouptype, int processtype, int serialno,
						int remark, int logtype, int logsw, int logmd, char *path);

		int LogDelete(int sysid, int serviceid, int grouptype, int processtype, int serialno,
						int remark, int logtype);

		int Get_SWGLAG(int sysid, int serviceid, int grouptype, int processtype, int serialno,
						int remark, int logtype);


		void Set_Init(void);	// PMS�� ���� ShareMem�� ���� �Ҵ���� �ʱ�ȭ�ϴ� �Լ�
};


/*
class LogManageSM  : public SharedMemory
{
	private:
		int				LogN;	// ���̺��� ����� ���ڵ尳��
		int				recSize;
		unsigned long*	unTempBaseAddr;

		LogManage*		LogDB;	// ���̺��� Access �Ͽ� UPdate
		LogManageRec	LogRecord;


	public:

		LogManageSM(  	int		nSystemId,		\
						int		nServiceId,		\
						int		nGroupId,		\
						int		nProcessType,	\
						int		nSerialNo,		\
						int		nRemark,		\
						int		nCount=1000		\
					):SharedMemory	(  		nSystemId,		\
											nServiceId,		\
											nGroupId,		\
											nProcessType,	\
											nSerialNo,		\
											nRemark,		\
											sizeof(LogManageRec) * nCount)
					{
						LogDB = new LogManage(nSystemId,nServiceId,nGroupId,nProcessType,nSerialNo,nRemark);
					}


		~LogManageSM(){delete LogDB;}

		void LogSMinit(void);	// T_LOG_MANAGE ���̺����� ���� �о�� ShareMem �ʱ�ȭ

		int Set_SysSwitch(int sysid, int flag);
		int Set_ServSwitch(int sysid, int serviceid, int flag);
		int Set_GroupSwitch(int sysid, int serviceid, int grouptype, int flag);
		int Set_ProcesSwitch(int sysid, int serviceid, int grouptype, int processtype, int flag);
		int Set_Switch(int sysid, int serviceid, int grouptype, int processtype, int serialno,
						int remark, int logtype, int flag);

		int LogInsert(int sysid, int serviceid, int grouptype, int processtype, int serialno,
						int remark, int logtype, int logsw, int logmd, char *path);

		int LogDelete(int sysid, int serviceid, int grouptype, int processtype, int serialno,
						int remark, int logtype);

		int Get_SWGLAG(int sysid, int serviceid, int grouptype, int processtype, int serialno,
						int remark, int logtype);


		void Set_Init(void);	// PMS�� ���� ShareMem�� ���� �Ҵ���� �ʱ�ȭ�ϴ� �Լ�
};
*/

#endif