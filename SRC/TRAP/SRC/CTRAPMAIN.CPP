/*****************************************************************************************
* file : CTrapMain.cpp
*
* TrapAgent main.
* T_TRAP_LOG�� �ֱ������� üũ�Ͽ� TRAP �����Ͱ� �߻��ϸ� NMS�� TRAP�� �߻���Ŵ.
*
*                                                                 01-05-23 9:52���� ky.
*****************************************************************************************/
// Include
//----------------------------------------------------------
#include "CTrapAgent.hpp"
#include <signal.h>        // for sigaction ...




// Define
//----------------------------------------------------------
static const int   D_QUIT_SIGNAL = SIGINT;            // ���� �ñ׳�
//static const char *D_LOG_PATH  = "/UDA2UR/SVC1/LOG";  // log directory STI 2��
static const char *D_LOG_PATH  = "/SVC1/LOG";       // log directory SKT 1��
#define log CTrapAgent::g_log->Write




// Function prototype
//----------------------------------------------------------
void InitSignal();               // �ñ׳� �ʱ�ȭ
void QuitProcess(int nSigNo);    // ���� �ñ׳� �Լ�




// Main
//----------------------------------------------------------
int main(int argc, char *argv[])
{
   InitSignal();                                               // �ñ׳� �ʱ�ȭ
   CTrapAgent::g_log = new FileLogWrite(D_LOG_PATH, argv[0]);  // �α�   �ʱ�ȭ

   CTrapAgent ITrapH(argc, argv);

   log("  Init signal. quit signal : %02d", D_QUIT_SIGNAL);
   if (ITrapH.Initialize() == false) return -1;

   bool nRet;
   nRet = ITrapH.Start();

   return (nRet == true) ? 0 : -1;
}//end main




/*****************************************************************************************
* Function Name : InitSignal()
* Description   : �ñ׳� �ʱ�ȭ.
* Parameters    : void
* Return values : void
* Date          : 01-05-23 10:18����
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
* Description   : ���� �ñ׳� �Լ�.
* Parameters    : int nSigNo - ���� �ñ׳� ��ȣ
* Return values : void
* Date          : 01-05-23 10:21����
* Last Update   :
* Author        : Ki Yon
*****************************************************************************************/
void QuitProcess(int nSigNo)
{
   log("---------------------------------[ catch signal  %02d ]---------------------------------", nSigNo);
   log("");
   CTrapAgent::g_nStop = 0;
}//end QuitProcess


