/*****************************************************************************************
* file : CTrapAgent.hpp
*
* Trap Agent.
* T_TRAP_LOG를 주기적으로 체크하여 TRAP 데이터가 발생하면 NMS로 TRAP을 발생시킴.
*
*                                                                 01-05-23 9:38오전 ky.
*****************************************************************************************/
#if !defined(_C_TRAP_AGENT_HPP_)
#define _C_TRAP_AGENT_HPP_
// Include
//----------------------------------------------------------
#include "FileLogWrite.hpp"
#include "SqlCommon.hpp"   // for connectDB ...




// Define
//----------------------------------------------------------
const int D_TRAP_MAX_CNT  = 34;           // trap의 총 갯수
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

   void IsRunDatabase();             // database가 기동중인지 체크 함.
   void IsRunPms();                  // pms가 기동중인지 체크 함.
   void GetTrapControlFlagList();    // Trap Control Flag에 의해 특정 Trap은 skip하도록 함.
   int  GetControlFlag(int nTrapNo); // 해당 Trap number의 플래그값을 리턴한다.
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



