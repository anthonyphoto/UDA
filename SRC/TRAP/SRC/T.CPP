#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
   char strCommand[512];
   sprintf(strCommand, "ps -ef | grep CPmsMain | grep -v grep | grep -v vi | grep -v tail | grep -v cat");

	bool flag=false;

for(;;)
{
   flag = false;

   FILE *pfd;
   if ((pfd = popen(strCommand, "r")) == NULL) {
      printf("      [ERROR] popen fail. command : %s", strCommand);
      return -1;
   }

   char strBuff[100];
   memset(strBuff, 0x00, sizeof(strBuff));
   while (fgets(strBuff, sizeof(strBuff), pfd)){
      if (strstr(strBuff, "CPmsMain") != NULL) { 
		 printf(".");
		 fflush(stdout);
		 pclose(pfd);
		 flag = true;
	   }
	}

	if (flag) continue;

   int nTrapFlag;
   int i;

	printf("PMS Stop!!\n");
   pclose(pfd);
	usleep(100000);
}
#if 0
   nTrapFlag = GetControlFlag(101);    // 101=udaTrapServiceStop UDA System A>7a =C :83?.
   if (nTrapFlag == D_TRAP)
   {
      for (i=0; i<m_nDestIpCnt; i++) {
         sprintf(strCommand, "%s %s %s \"%s(%d) : [%d] - %s\"",
            D_TRAP_PROC, m_ptrDestIp[i], D_TRAP_ARGV,
            "udaTrapServiceStop", 101, 1, "PMS Stopped!!");
         printf("    Trap : %s", strCommand);
         system(strCommand);
      }
      printf("");
   }
   else
      printf("    Trap Skip. %s(%d). flag:%d", "udaTrapServiceStop", 101, nTrapFlag);
#endif

	return 0;
}
