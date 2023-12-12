/*******************************************************************************
*���α׷� ����:
*   multi threaded application�� �����Ҷ� ��������.
*   oracle developing multi-threaded application ����.
*
*   1. �����Ͻÿ� thread=yes��� �ɼ��� �ݵ�� ����Ѵ�.
*      �̰��� sql ������ ������ runtime context�� ����������
*          ����ǵ��� �ϱ� ���� ���̴�.
*      ex)proc iname=*.pc thread=yes
*
*   2.EXEC SQL ENABLE THREADS
*     multiple thread �� �����ϴ� ���μ����� �ʱ�ȭ�۾�.
*
*   3.EXEC SQL CONTEXT ALLOCATE :context_var
*     context variable�� �ʱ�ȭ.
*         context variable�� �ʱ�ȭ�ϱ⶧���� runtime context����
*         �߻��� ��� �۾����� �ʱ�ȭ �Ǿ�����.
*     connect�� ������ context�� �������Ƿ� connect���Ŀ�
*         allocate �Ѵٸ� not logged on ���°� �ǹǷ� �������Ѵ�.
*
*   4.EXEC SQL CONTEXT USE :context_var
*     sql ������ ����� ������ ��Ÿ���°����� sql������ ���� ÷��
*     ���;� �Ѵ�.
* Version 1.1 ( 2001. 05. 21 )
*******************************************************************************/
#ifndef _CPMSDBHANDLER_H
#define _CPMSDBHANDLER_H

#include <string.h>
#include <pthread.h>
#include <stdarg.h>
#include "ptrList.hpp"
#include "ctime.hpp"
#include "CPmsCommon.hpp"
#include "time.hpp"

// PMS������ DB handling�� ó�� �Ѵ�.
class CPmsDbHandler
{
private:
   char m_recvPipe[10]; // oracle pipe receive name
   char m_sendPipe[10]; // oracle pipe send    Name

public:
   // DB handling�� �ϱ����� �ʱ�ȭ�� �Ѵ�.
   CPmsDbHandler(int i=0, char* recv="recvpipe", char* send="sendpipe");

   void InsertTrapData(int i, int nTrapNo, int nValue, char* strDescF, ...);
      // Trap Data�� Insert �Ѵ�.
      

   // SQL error log�� �����.
   void SqlError(char *errMsg, struct sqlca sqlca);

   bool ConnectDBMultiThread(int i, char *id, char *passwd); // DB ����
   bool DisconnectDBMultiThread(int i);                 // DB ���� ����

   // Client(oracle pipe)�� ���� �޼����� �޴´�.
   bool RecvMsgFromPipe(int i, command_t &retCommand);

   // Client(oracle pipe)�� �޼����� ������.
   bool SendResultMsgToPipe(int i, int flag);

   // Linked list�� �����.
   void AddProcList(PPROCINFO pProcInfo, int cnt);

   // DB�� ���� process ������ �о�´�.
   bool FetchProcessInfoFromDb(int i, char type, char *strCondition);

   // pms�� �⵿�� �ý��۸��� db�� ������Ʈ �Ѵ�. (2001. 5. 7. �� �߰�. ky)
   bool UpdateHostName(int ct);

   // process�� ���� ������ ������Ʈ �Ѵ�.
   bool UpdateProcStatusTable(int i, PROCINFO &nodeData, char stFlag = 'P');

   // ���� �������� ������ ����Ѵ�.
   void PrintPipe(){/*cout << "pipe name :" << m_recvPipe <<","<<m_sendPipe<<endl;*/}

   // process list�� ������ DB�� ������ �´����� üũ�ϰ� Ʋ���� DB�� �����ͷ�
   // list�� ������Ʈ �Ѵ�.
   bool IsNewData(PROCINFO nodeData);

   // client�� ���� ���� ������ �з��Ѵ�.
   void SplitCommand(char* command, command_t &c);

   // config������ ��ȸ �Ѵ�.
   void _SelectCfgFile(command_t cfgFileInfo, char *f_path, char *f_name, int &i_count, char *f_id);

   // config������ ���� �Ѵ�.
   void _InsertCfgFile(command_t cfgFileInfo, int itemCnt, char flag);

   // Backup Table�� ������ �о� �´�.
   bool _SelectBackupInfo(BackInfo retInfo[100], int &retCnt);
};

#endif // _CPMSDBHANDLER_H