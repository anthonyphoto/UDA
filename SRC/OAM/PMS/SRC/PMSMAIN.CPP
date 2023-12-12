/*******************************************************************************
* file : pmsMain.cpp
*
* PMS (Process Management System)
* process run, stop 그외 업무처리를 하는 프로세스이다.
* client와 연동되어 명령을 받고 처리하는 식으로 프로세스가 동작하게 된다.
* 업무로는 파일백업, config file 조회 및 수정, disk, cpu사용량 조회,
* Log switch 변경, 프로세스 및 네트웍 비정상 체크를 한다.
* Version 1.1 ( 2001. 05. 21 )
*******************************************************************************/
#include "Cpms.hpp"




// function prototype
//----------------------------------------------------------
bool IsProcessRun(int ac, char** av);     // 동일한 프로세스가 떠있는지 체크한다.




/*******************************************************************************
* PMS main
*******************************************************************************/
int main(int argc, char *argv[])
{
   if (IsProcessRun(argc, argv) == true) {
      cout << argv[0] << " is running!!" << endl << endl;
      return -1;
   }
   
    CPMS pms;                       // 객체 선언 및 초기화

    pms.Initialize(argc, argv);     // PMS 초기화 작업 (Thread생성..)
    pms.ProcessCommand();           // Main Thread

    return 0;
}




/*****************************************************************************************
* Desc : process alias에 해당하는 프로세스가 떠있는지 체크하는 함수.
*        프로세스 체크는 ps명령을 사용하며, 프로세스의 alias는 유일해야 한다.
*                                                                   00-12-22 1:01오전 ky.
* Argu : int    ac - main의 argc
*        char** av - main의 argv. 프로세스의 alias를 만들기 위해 받는다.
* Retn : bool true  - 프로세스가 기동중이다.
*             false - 프로세스가 기동중이 아니다.
*             -1    - popen error
*****************************************************************************************/
bool IsProcessRun(int ac, char** av)
{
    // process alias를 만든다.
    //----------------------------------------------------------
    char processName[100];
    memset(processName, 0x00, sizeof(processName));
    sprintf(processName, "%s", av[0]);
    for(int i=1; i < ac; i++)   sprintf(processName, "%s %s", processName, av[i]);

    // ps 명령 정의
    //----------------------------------------------------------
    char sysCmd[512];
    sprintf(sysCmd, "ps -ef | grep '%s' | grep -v grep | grep -v tail | grep -v vi | grep -v cat | grep -v webadmin", processName);

    // pipe open처리.
    //----------------------------------------------------------
    FILE *pFd;
    if ((pFd = popen(sysCmd, "r")) == NULL) return -1;  // popen에러시 -1을 리턴한다.

    // process가 떠있는지를 check한다.
    //----------------------------------------------------------
    char compName[100]; // 비교 문자열. (ps에서 얻어온 process name)
    char strOut  [100]; // popen으로 부터 받은 출력 메세지.
    bool isRun = false; // 프로세스의 동작 여부.

    while (fgets(strOut, 100, pFd))
    {
        memset(compName, 0x00, sizeof(compName));
        memcpy(compName, strstr(strOut, processName), strlen(processName));

        if (strcmp(compName, processName) == 0)
        {
            // 같은 형상의 프로세스가 동작중이다.
            if (isRun == true) { pclose(pFd); return true; }
            isRun = true;
        }
    }

    pclose(pFd);
    return false;   // 같은 형상의 프로세스가 동작중이지 않다.
}



