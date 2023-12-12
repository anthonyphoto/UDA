/*******************************************************************************
*프로그램 설명:
*   multi threaded application을 개발할때 유의할점.
*   oracle developing multi-threaded application 참고.
*
*   1. 컴파일시에 thread=yes라는 옵션을 반드시 써야한다.
*      이것은 sql 문장이 정의한 runtime context의 범위내에서
*          실행되도록 하기 위한 것이다.
*      ex)proc iname=*.pc thread=yes
*
*   2.EXEC SQL ENABLE THREADS
*     multiple thread 를 지원하는 프로세스의 초기화작업.
*
*   3.EXEC SQL CONTEXT ALLOCATE :context_var
*     context variable을 초기화.
*         context variable을 초기화하기때문에 runtime context에서
*         발생한 모든 작업들이 초기화 되어진다.
*     connect의 정보도 context에 남겨지므로 connect한후에
*         allocate 한다면 not logged on 상태가 되므로 유의해한다.
*
*   4.EXEC SQL CONTEXT USE :context_var
*     sql 문장이 실행될 범위를 나타내는것으로 sql문장중 가장 첨에
*     나와야 한다.
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

// PMS에서의 DB handling을 처리 한다.
class CPmsDbHandler
{
private:
   char m_recvPipe[10]; // oracle pipe receive name
   char m_sendPipe[10]; // oracle pipe send    Name

public:
   // DB handling을 하기위한 초기화를 한다.
   CPmsDbHandler(int i=0, char* recv="recvpipe", char* send="sendpipe");

   void InsertTrapData(int i, int nTrapNo, int nValue, char* strDescF, ...);
      // Trap Data를 Insert 한다.
      

   // SQL error log를 남긴다.
   void SqlError(char *errMsg, struct sqlca sqlca);

   bool ConnectDBMultiThread(int i, char *id, char *passwd); // DB 접속
   bool DisconnectDBMultiThread(int i);                 // DB 접속 해제

   // Client(oracle pipe)로 부터 메세지를 받는다.
   bool RecvMsgFromPipe(int i, command_t &retCommand);

   // Client(oracle pipe)로 메세지를 보낸다.
   bool SendResultMsgToPipe(int i, int flag);

   // Linked list를 만든다.
   void AddProcList(PPROCINFO pProcInfo, int cnt);

   // DB로 부터 process 정보를 읽어온다.
   bool FetchProcessInfoFromDb(int i, char type, char *strCondition);

   // pms가 기동된 시스템명을 db로 업데이트 한다. (2001. 5. 7. 월 추가. ky)
   bool UpdateHostName(int ct);

   // process의 상태 정보를 업데이트 한다.
   bool UpdateProcStatusTable(int i, PROCINFO &nodeData, char stFlag = 'P');

   // 현재 파이프의 내용을 출력한다.
   void PrintPipe(){/*cout << "pipe name :" << m_recvPipe <<","<<m_sendPipe<<endl;*/}

   // process list의 정보가 DB의 정보와 맞는지를 체크하고 틀릴시 DB의 데이터로
   // list를 업데이트 한다.
   bool IsNewData(PROCINFO nodeData);

   // client로 부터 받은 명령을 분류한다.
   void SplitCommand(char* command, command_t &c);

   // config파일을 조회 한다.
   void _SelectCfgFile(command_t cfgFileInfo, char *f_path, char *f_name, int &i_count, char *f_id);

   // config파일을 수정 한다.
   void _InsertCfgFile(command_t cfgFileInfo, int itemCnt, char flag);

   // Backup Table의 내용을 읽어 온다.
   bool _SelectBackupInfo(BackInfo retInfo[100], int &retCnt);
};

#endif // _CPMSDBHANDLER_H
