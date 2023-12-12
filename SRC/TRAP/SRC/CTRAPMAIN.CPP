/*****************************************************************************************
* file : CTrapMain.cpp
*
* TrapAgent main.
* T_TRAP_LOG를 주기적으로 체크하여 TRAP 데이터가 발생하면 NMS로 TRAP을 발생시킴.
*
*                                                                 01-05-23 9:52오전 ky.
*****************************************************************************************/
// Include
//----------------------------------------------------------
#include "CTrapAgent.hpp"
#include <signal.h>        // for sigaction ...




// Define
//----------------------------------------------------------
static const int   D_QUIT_SIGNAL = SIGINT;            // 종료 시그널
//static const char *D_LOG_PATH  = "/UDA2UR/SVC1/LOG";  // log directory STI 2용
static const char *D_LOG_PATH  = "/SVC1/LOG";       // log directory SKT 1용
#define log CTrapAgent::g_log->Write




// Function prototype
//----------------------------------------------------------
void InitSignal();               // 시그널 초기화
void QuitProcess(int nSigNo);    // 종료 시그널 함수




// Main
//----------------------------------------------------------
int main(int argc, char *argv[])
{
   InitSignal();                                               // 시그널 초기화
   CTrapAgent::g_log = new FileLogWrite(D_LOG_PATH, argv[0]);  // 로그   초기화

   CTrapAgent ITrapH(argc, argv);

   log("  Init signal. quit signal : %02d", D_QUIT_SIGNAL);
   if (ITrapH.Initialize() == false) return -1;

   bool nRet;
   nRet = ITrapH.Start();

   return (nRet == true) ? 0 : -1;
}//end main




/*****************************************************************************************
* Function Name : InitSignal()
* Description   : 시그널 초기화.
* Parameters    : void
* Return values : void
* Date          : 01-05-23 10:18오전
* Last Update   :
* Author        : Ki Yon
*****************************************************************************************/
void InitSignal()
{
   static struct sigaction s_act;

   s_act.sa_handler = QuitProcess;
   sigaction(D_QUIT_SIGNAL, &s_act, NULL);
}//end InitSignal




/*****************************************************************************************
* Function Name : QuitProcess()
* Description   : 종료 시그널 함수.
* Parameters    : int nSigNo - 종료 시그널 번호
* Return values : void
* Date          : 01-05-23 10:21오전
* Last Update   :
* Author        : Ki Yon
*****************************************************************************************/
void QuitProcess(int nSigNo)
{
   log("---------------------------------[ catch signal  %02d ]---------------------------------", nSigNo);
   log("");
   CTrapAgent::g_nStop = 0;
}//end QuitProcess



