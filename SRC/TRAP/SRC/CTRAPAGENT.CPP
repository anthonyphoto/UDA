/*****************************************************************************************
* file : CTrapAgent.cpp
*
* Trap Agent.
* T_TRAP_LOG�� �ֱ������� üũ�Ͽ� TRAP �����Ͱ� �߻��ϸ� NMS�� TRAP�� �߻���Ŵ.
*
*                                                                 01-05-23 9:46���� ky.
*****************************************************************************************/
// Include
//----------------------------------------------------------
#define SQLCA_STORAGE_CLASS extern
#include "CTrapAgent.hpp"
#include "CTrapDbLib.hpp"  // for T_TRAP_LOG handling ...
#include <iostream.h>      // for cout ...
#include <stdlib.h>        // for atoi ...
#include <unistd.h>        // for sleep ...
#include <sqlca.h>         // for sqlca ...
#include <stdio.h>         // for popen ...




// Define
//----------------------------------------------------------
static       char *D_DBID = "uda";                    // oracle db id
static       char *D_DBPW = "uda";                    // oracle db password

static const char *D_TRAP_PROC = "/opt/OV/bin/snmptrap";    // trap process
static const char *D_TRAP_ARGV = "\"\" \"\" 6 2 0 1.3.6.1.4.1.9608.4.2.0 octetstringascii";
  // trap default argument

static const int D_CHECK_INTERVAL = 30;                     // default check interval
#define log CTrapAgent::g_log->Write                        // log macro
static const char *D_TRAP_CONTROL_FLAG_FILE = "/var/SNMP/CONF/TrapSwitch.conf";
static const char *D_TRAP_DEST_IP_FILE      = "/var/SNMP/CONF/TrapDestIp.conf";
  // trap start control flag file.

static const int D_TRAP = 1;     // trap flag value
static const int D_SKIP = 0;     // skip flag value




// Global variable
//----------------------------------------------------------
int           CTrapAgent::g_nStop = 1;
FileLogWrite *CTrapAgent::g_log;
int g_nTrapFlag[D_TRAP_MAX_CNT];          // �� Trap�� trap�߻� ������ȸ �迭
                                          // 0 : skip, 1 : trap




// Class function define
//----------------------------------------------------------
// Public
/*****************************************************************************************
* Function Name : CTrapAgent()
* Description   : CTrapAgent constructor. ���� �ʱ�ȭ.
* Parameters    : char **ptrArgv - main argument
* Return values : void
* Date          : 01-05-23 9:50����
* Last Update   :
* Author        : Ki Yon
*****************************************************************************************/
CTrapAgent::CTrapAgent(int nArgc, char **ptrArgv)
{
   log("==================================[ %s START ]==================================", ptrArgv[0]);
   log("");

   m_nArgc        = nArgc;
   m_ptrArgv      = ptrArgv;
   m_chkInterval  = atoi(m_ptrArgv[1]); // Check interval
   m_nDBstopFlag  = false;              // Database trap flag.
   m_nDBstartFlag = true;               // Database trap flag.
   m_nDestIpCnt   = 0;

   // interval�� 0�̰ų� ������ ����Ʈ interval ����.
   //----------------------------------------------------------
   if (m_chkInterval <= 0) m_chkInterval = D_CHECK_INTERVAL;
}//end CTrapAgent




/*****************************************************************************************
* Function Name : ~CTrapAgent()
* Description   : CTrapAgent destructor.
* Parameters    : void
* Return values : void
* Date          : 01-05-23 3:36����
* Last Update   :
* Author        : Ki Yon
*****************************************************************************************/
CTrapAgent::~CTrapAgent()
{
   // DB disconnect
   //----------------------------------------------------------
   if (m_IdbH.disconnect() != 0) {
      log("  [ERROR] DB disconnect fail ...");
      log("          sqlmsg : %.*s", sqlca.sqlerrm.sqlerrml-1, sqlca.sqlerrm.sqlerrmc);
   }
   else
      log("  DB disconnect success ...");

   log("");
   log("================================[ %s TERMINATE ]================================", m_ptrArgv[0]);
   log("");
}//end ~CTrapAgent




/*****************************************************************************************
* Function Name : Initialize()
* Description   : �ʱ�ȭ.
* Parameters    : void
* Return values : bool true  - ��������
*                      false - ������ ����
* Date          : 01-05-23 9:58����
* Last Update   :
* Author        : Ki Yon
*****************************************************************************************/
bool CTrapAgent::Initialize()
{
   cout << "Usage : " << m_ptrArgv[0] << " [check interval](sec)"
        << endl << endl;

   // DB connect
   //----------------------------------------------------------
   if (m_IdbH.connect() != 0) {
      log("  [ERROR] DB connect fail ...");
      log("          sqlmsg : %.*s", sqlca.sqlerrm.sqlerrml-1, sqlca.sqlerrm.sqlerrmc);
      return false;
   }
   log("  DB connect success ...");
   log("");

   if (GetTrapDestIp() == false) return false;     // trap destination ip load.

   for (int i=0; i<m_nDestIpCnt; i++)
      log("  NMS ip address[%d] : %s", i, m_ptrDestIp[i]);

   log("  check interval : %d", m_chkInterval);
   log("");

   return true;
}//end Initialize




/*****************************************************************************************
* Function Name : Start()
* Description   : Trap Agent�⵿.
* Parameters    : void
* Return values : bool true  - ��������
*                      false - ������ ����
* Date          : 01-05-23 9:51����
* Last Update   :
* Author        : Ki Yon
*****************************************************************************************/
bool CTrapAgent::Start()
{
   log("  [Start]");

   int  i, j;               // for index
   int  nRet;               // return value
   int  nTrapFlag;          // Trap flag value
   int  nComRecCnt;         // ��ȸ�� ������ ���ڵ� ����
   char strCommand[1024];   // trap command

   trapT data[D_LOG_RECORD_CNT];
   CTrapDbLib ItrapH;

   while (g_nStop)
   {
      (void) GetTrapDestIp();          // trap destination ip load.
      GetTrapControlFlagList();        // trap switch load
      IsRunDatabase();                 // database check
      IsRunPms();                      // pms check

      // cursor open
      //----------------------------------------------------------
      if (ItrapH.OpenCursor() == false) {
         log("    [ERROR] OpenCursor fail.");
         log("            sqlmsg : %.*s", sqlca.sqlerrm.sqlerrml-1, sqlca.sqlerrm.sqlerrmc);
         sleep(m_chkInterval);      // interval
         continue;
      }

      if (ItrapH.Select(data) > 0 ) {
         nComRecCnt = sqlca.sqlerrd[2];      // ��ȸ�� ������ ������ ���ڵ� ����.
         for (i=0; i<nComRecCnt; i++) {
            nTrapFlag = GetControlFlag(data[i].nTrapNo);
            if (nTrapFlag == D_TRAP)
            {
               for (j=0; j<m_nDestIpCnt; j++) {
                  sprintf(strCommand, "%s %s %s \"%s(%d) : [%d] - %s\"",
                     D_TRAP_PROC, m_ptrDestIp[j], D_TRAP_ARGV,
                     data[i].strTrapName, data[i].nTrapNo, data[i].nTrapValue, data[i].strTrapDesc);

                  log("    Trap : %s", strCommand);
                  nRet = system(strCommand);       // run trap process. �����϶� 0 return.
                                                   //                   ���н� 256 or �ٸ��� return.
                  if (nRet != 0)
                     log("    [ERROR] Trap fail. return value : %d", nRet);
               }
               log("");
               continue;
            }
            log("    Trap Skip. %s(%d). flag:%d", data[i].strTrapName, data[i].nTrapNo, nTrapFlag);
         }

         for (i=0; i<nComRecCnt; i++) {
            if (ItrapH.UpdateSendTime(data[i].strRecvTime) == false) {
               log("    [ERROR] UpdateSendTime fail. key : %s", data[i].strRecvTime);
               log("            sqlmsg : %.*s", sqlca.sqlerrm.sqlerrml-1, sqlca.sqlerrm.sqlerrmc);
            }
         }
         continue;
      }

      if (sqlca.sqlcode != 1403) {
         log("    sqlmsg : %.*s", sqlca.sqlerrm.sqlerrml-1, sqlca.sqlerrm.sqlerrmc);
         log("    wait interverl. interval : %d", m_chkInterval);
      }

      // cursor close
      //----------------------------------------------------------
      if (ItrapH.CloseCursor() == false) {
         log("    [ERROR] CloseCursor fail.");
         log("            sqlmsg : %.*s", sqlca.sqlerrm.sqlerrml-1, sqlca.sqlerrm.sqlerrmc);
      }

      sleep(m_chkInterval);      // interval
   }

   return true;
}//end Start




/*****************************************************************************************
* Function Name : IsRunOracle()
* Description   : oracle database�� ���������� üũ�Ѵ�. �������̸� �ƹ��ൿ�� ���� �ʴ´�.
*                 �������� �ƴϸ� trap�� �߻��Ѵ�.
* Parameters    : void
* Return values : void
* Date          : 01-05-29 7:05����
* Last Update   :
* Author        : Ki Yon
*****************************************************************************************/
void CTrapAgent::IsRunDatabase()
{
   char strCommand[100];
   sprintf(strCommand, "ps -ef | egrep 'ora_ckpt|ora_pmon|ora_smon|ora_reco|ora_dbw0' | grep -v grep | wc -l");

   FILE *pfd;
   if ((pfd = popen(strCommand, "r")) == NULL) {
      log("      [ERROR] popen fail. command : %s", strCommand);
      return ;
   }

   char strBuff[100];
   int  nProcCnt;
   while (fgets(strBuff, sizeof(strBuff), pfd))
      nProcCnt = atoi(strBuff);

   if (nProcCnt < 0) {
      log("      [ERROR] abnormally nProcCnt. nProcCnt : %d", nProcCnt);
      pclose(pfd);
      return ;
   }

   int nTrapFlag;
   int i;

   if (nProcCnt >= 0 && nProcCnt < 5) {
      if (m_nDBstopFlag == false)
      {
         m_IdbH.disconnect();
         nTrapFlag = GetControlFlag(10006);     // 10006=udaTrapDbFail. �����ͺ��̽� �����߻�.
         if (nTrapFlag == D_TRAP)
         {
            for (i=0; i<m_nDestIpCnt; i++) {
               sprintf(strCommand, "%s %s %s \"%s(%d) : [%d] - %s\"",
                  D_TRAP_PROC, m_ptrDestIp[i], D_TRAP_ARGV,
                  "udaTrapDbFail", 10006, 1, "database stopped!!");

               log("    Trap : %s", strCommand);
               system(strCommand);
            }
            log("");
         }
         else
            log("    Trap Skip. %s(%d). flag:%d", "udaTrapDbFail", 10006, nTrapFlag);

         m_nDBstopFlag  = true;
         m_nDBstartFlag = false;
      }
   }
   else if (nProcCnt >= 5) {
      if (m_nDBstartFlag == false)
      {
         // DB connect
         //----------------------------------------------------------
         if (m_IdbH.connect() != 0) {
            log("  [ERROR] DB connect fail ...");
            log("          sqlmsg : %.*s", sqlca.sqlerrm.sqlerrml-1, sqlca.sqlerrm.sqlerrmc);
         }
         log("  DB connect success ...");
         log("");

         nTrapFlag = GetControlFlag(112);    // 112=udaTrapDbFail. ������ ���̽� ���� ����.
         if (nTrapFlag == D_TRAP)
         {
            for (i=0; i<m_nDestIpCnt; i++) {
               sprintf(strCommand, "%s %s %s \"%s(%d) : [%d] - %s\"",
                  D_TRAP_PROC, m_ptrDestIp[i], D_TRAP_ARGV,
                  "udaTrapDbNormal", 112, 0, "database running!!");

               log("    Trap : %s", strCommand);
               system(strCommand);
            }
            log("");
         }
         else
            log("    Trap Skip. %s(%d). flag:%d", "udaTrapDbNormal", 112, nTrapFlag);

         m_nDBstartFlag = true;
         m_nDBstopFlag  = false;
      }
   }

   pclose(pfd);
}//end IsRunOracle




/*****************************************************************************************
* Desc : PMS�� �⵿������ üũ�Ͽ� ������ �����϶��� trap�� �߻���Ų��.
*                                                                    01-05-29 5:04���� ky.
* Argu : void
* Retn : void
* etc. : none
*****************************************************************************************/
void CTrapAgent::IsRunPms()
{
   char strCommand[512];
   sprintf(strCommand, "export UNIX95=XPG4 ; ps -o comm -e | grep CPmsMain | grep -v grep | grep -v vi | grep -v tail | grep -v cat");

   FILE *pfd;
   if ((pfd = popen(strCommand, "r")) == NULL) {
      log("      [ERROR] popen fail. command : %s", strCommand);
      return ;
   }

   sleep(1);
   
   char strBuff[512];
   memset(strBuff, 0x00, sizeof(strBuff));
   while (fgets(strBuff, sizeof(strBuff), pfd)) {
      if (strncmp(strBuff, "CPmsMain", 8) == 0) { 
      	pclose(pfd);
      	return;
      }
	}
   pclose(pfd);

   int nTrapFlag;
   int i;

   nTrapFlag = GetControlFlag(101);    // 101=udaTrapServiceStop UDA System ���� �� ����.
   if (nTrapFlag == D_TRAP)
   {
      for (i=0; i<m_nDestIpCnt; i++) {
         sprintf(strCommand, "%s %s %s \"%s(%d) : [%d] - %s\"",
            D_TRAP_PROC, m_ptrDestIp[i], D_TRAP_ARGV,
            "udaTrapServiceStop", 101, 1, "PMS Stopped!!");
         log("    Trap : %s", strCommand);
         system(strCommand);
      }
      log("");
   }
   else
      log("    Trap Skip. %s(%d). flag:%d", "udaTrapServiceStop", 101, nTrapFlag);
}//end IsRunPms




/*****************************************************************************************
* Desc : �� Trap�� �߻����θ� ������ �� �ֵ��� trap no�� flag�� �Ҵ��Ѵ�.
*        flag������ ~/SNMP/CONF/TrapStartControl.conf�� ��ϵǾ� ������ 0�� 1�� �����Ѵ�.
*        0�� skip�̸� 1�� trap�߻��̴�.
*                                                                 01-05-29 3:56���� ky.
* Argu : void
* Retn : void
* etc. : none
*****************************************************************************************/
void CTrapAgent::GetTrapControlFlagList()
{
   int i = D_TRAP_MAX_CNT;
   memset((char *)&g_nTrapFlag, 0x00, sizeof(g_nTrapFlag));

   FILE *fd;
   if ((fd = fopen(D_TRAP_CONTROL_FLAG_FILE, "r")) == NULL)
   {
      cout << "    [ERROR] " << D_TRAP_CONTROL_FLAG_FILE << " file open error." << endl;
      return ;
   }

   fscanf(fd, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
   &g_nTrapFlag[0] , &g_nTrapFlag[1] , &g_nTrapFlag[2] , &g_nTrapFlag[3] , &g_nTrapFlag[4] , &g_nTrapFlag[5],
   &g_nTrapFlag[6] , &g_nTrapFlag[7] , &g_nTrapFlag[8] , &g_nTrapFlag[9] , &g_nTrapFlag[10], &g_nTrapFlag[11],
   &g_nTrapFlag[12], &g_nTrapFlag[13], &g_nTrapFlag[14], &g_nTrapFlag[15], &g_nTrapFlag[16], &g_nTrapFlag[17],
   &g_nTrapFlag[18], &g_nTrapFlag[19], &g_nTrapFlag[20], &g_nTrapFlag[21], &g_nTrapFlag[22], &g_nTrapFlag[23],
   &g_nTrapFlag[24], &g_nTrapFlag[25], &g_nTrapFlag[26], &g_nTrapFlag[27], &g_nTrapFlag[28], &g_nTrapFlag[29],
   &g_nTrapFlag[30], &g_nTrapFlag[31], &g_nTrapFlag[32], &g_nTrapFlag[33]);

   fclose(fd);
}//end GetTrapControlFlagList




/*****************************************************************************************
* Desc : nTrapNo�� �ش��ϴ� Flag���� �����Ѵ�.
*                                                                 01-05-29 4:21���� ky.
* Argu : int nTrapNo - trap number
* Retn : int - Flag 0:skip, 1:trap �߻�
*            - -1 �ش� trap number ����.
* etc. : none
*****************************************************************************************/
int CTrapAgent::GetControlFlag(int nTrapNo)
{
   switch (nTrapNo)
   {
      case 100  : return g_nTrapFlag[ 0]; break;
      case 101  : return g_nTrapFlag[ 1]; break;
      case 102  : return g_nTrapFlag[ 2]; break;
      case 103  : return g_nTrapFlag[ 3]; break;
      case 104  : return g_nTrapFlag[ 4]; break;
      case 105  : return g_nTrapFlag[ 5]; break;
      case 106  : return g_nTrapFlag[ 6]; break;
      case 107  : return g_nTrapFlag[ 7]; break;
      case 108  : return g_nTrapFlag[ 8]; break;
      case 109  : return g_nTrapFlag[ 9]; break;
      case 110  : return g_nTrapFlag[10]; break;
      case 111  : return g_nTrapFlag[11]; break;
      case 112  : return g_nTrapFlag[12]; break;
      case 113  : return g_nTrapFlag[13]; break;
      case 5000 : return g_nTrapFlag[14]; break;
      case 5001 : return g_nTrapFlag[15]; break;
      case 5002 : return g_nTrapFlag[16]; break;
      case 5003 : return g_nTrapFlag[17]; break;
      case 5004 : return g_nTrapFlag[18]; break;
      case 5005 : return g_nTrapFlag[19]; break;
      case 5006 : return g_nTrapFlag[20]; break;
      case 5007 : return g_nTrapFlag[21]; break;
      case 5008 : return g_nTrapFlag[22]; break;
      case 5009 : return g_nTrapFlag[23]; break;
      case 5010 : return g_nTrapFlag[24]; break;
      case 5011 : return g_nTrapFlag[25]; break;
      case 10000: return g_nTrapFlag[26]; break;
      case 10001: return g_nTrapFlag[27]; break;
      case 10002: return g_nTrapFlag[28]; break;
      case 10003: return g_nTrapFlag[29]; break;
      case 10004: return g_nTrapFlag[30]; break;
      case 10005: return g_nTrapFlag[31]; break;
      case 10006: return g_nTrapFlag[32]; break;
      case 10007: return g_nTrapFlag[33]; break;
      default : return -1;
   }
}//end GetControlFlag




/*****************************************************************************************
* Desc : D_TRAP_DEST_IP_FILE ���Ͽ��� Trap�� ���� ������ IP�� �о� ����Ʈȭ �Ѵ�.
*        ����Ʈ�� �迭�� �����Ǹ� D_TRAP_DEST_CNT���� ������ �� �ִ�.
* Argu : void
* Retn : bool true  - list load success.
*             false - list load fail.
* etc. : none
*****************************************************************************************/
bool CTrapAgent::GetTrapDestIp()
{
   m_nDestIpCnt = 0;
   
   // trap nms ip ȯ�������� ������ ���� �߻�.
   FILE *fd;
   if ((fd = fopen(D_TRAP_DEST_IP_FILE, "r")) == NULL) {
      cout << D_TRAP_DEST_IP_FILE << " open fail." << endl << endl;
      return false;
   }

   // ip ���� üũ.
   char strDestIp[100];
   while (fgets(strDestIp, sizeof(strDestIp), fd))
      m_nDestIpCnt++;

   fseek(fd, 0, SEEK_SET);
   for (int i=0; i<m_nDestIpCnt; i++)
   {
      fgets(strDestIp, sizeof(strDestIp), fd);
      strtok(strDestIp, "\n");
      strcpy(m_ptrDestIp[i], strDestIp);
   }
   fclose(fd);

   return true;
}//end GetTrapDestIp


