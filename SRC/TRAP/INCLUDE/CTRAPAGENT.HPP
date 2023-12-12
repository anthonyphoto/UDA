/*****************************************************************************************
* file : CTrapAgent.hpp
*
* Trap Agent.
* T_TRAP_LOG�� �ֱ������� üũ�Ͽ� TRAP �����Ͱ� �߻��ϸ� NMS�� TRAP�� �߻���Ŵ.
*
*                                                                 01-05-23 9:38���� ky.
*****************************************************************************************/
#if !defined(_C_TRAP_AGENT_HPP_)
#define _C_TRAP_AGENT_HPP_
// Include
//----------------------------------------------------------
#include "FileLogWrite.hpp"
#include "SqlCommon.hpp"   // for connectDB ...




// Define
//----------------------------------------------------------
const int D_TRAP_MAX_CNT  = 34;           // trap�� �� ����
const int D_TRAP_DEST_CNT = 100;          // trap destination ip count




// Class
//----------------------------------------------------------
class CTrapAgent
{
private:
   DbConnect m_IdbH;

   int    m_nArgc;                   // main argument count
   char **m_ptrArgv;                 // main argument value
   int    m_chkInterval;             // check interval
   bool   m_nDBstopFlag;             // Database trap flag.
   bool   m_nDBstartFlag;            // Database trap flag.
   int    m_index[D_TRAP_MAX_CNT];
   int    m_nDestIpCnt;              // trap NMS ip address count.
   char   m_ptrDestIp[D_TRAP_DEST_CNT][100];    // trap NMS ip address. max size 100

   void IsRunDatabase();             // database�� �⵿������ üũ ��.
   void IsRunPms();                  // pms�� �⵿������ üũ ��.
   void GetTrapControlFlagList();    // Trap Control Flag�� ���� Ư�� Trap�� skip�ϵ��� ��.
   int  GetControlFlag(int nTrapNo); // �ش� Trap number�� �÷��װ��� �����Ѵ�.
   bool GetTrapDestIp();             // trap destination ip load.

public:
   static FileLogWrite *g_log;       // file log handler
   static int           g_nStop;     // process stop flag

    CTrapAgent(int nArgc, char **ptrArgv);
   ~CTrapAgent();

   bool Initialize();
   bool Start();
};//end CTrapAgent




#endif//end _C_TRAP_AGENT_HPP_


