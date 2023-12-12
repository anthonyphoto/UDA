/*******************************************************************************
* file : pmsMain.cpp
*
* PMS (Process Management System)
* process run, stop �׿� ����ó���� �ϴ� ���μ����̴�.
* client�� �����Ǿ� ������ �ް� ó���ϴ� ������ ���μ����� �����ϰ� �ȴ�.
* �����δ� ���Ϲ��, config file ��ȸ �� ����, disk, cpu��뷮 ��ȸ,
* Log switch ����, ���μ��� �� ��Ʈ�� ������ üũ�� �Ѵ�.
* Version 1.1 ( 2001. 05. 21 )
*******************************************************************************/
#include "Cpms.hpp"




// function prototype
//----------------------------------------------------------
bool IsProcessRun(int ac, char** av);     // ������ ���μ����� ���ִ��� üũ�Ѵ�.




/*******************************************************************************
* PMS main
*******************************************************************************/
int main(int argc, char *argv[])
{
   if (IsProcessRun(argc, argv) == true) {
      cout << argv[0] << " is running!!" << endl << endl;
      return -1;
   }
   
    CPMS pms;                       // ��ü ���� �� �ʱ�ȭ

    pms.Initialize(argc, argv);     // PMS �ʱ�ȭ �۾� (Thread����..)
    pms.ProcessCommand();           // Main Thread

    return 0;
}




/*****************************************************************************************
* Desc : process alias�� �ش��ϴ� ���μ����� ���ִ��� üũ�ϴ� �Լ�.
*        ���μ��� üũ�� ps������ ����ϸ�, ���μ����� alias�� �����ؾ� �Ѵ�.
*                                                                   00-12-22 1:01���� ky.
* Argu : int    ac - main�� argc
*        char** av - main�� argv. ���μ����� alias�� ����� ���� �޴´�.
* Retn : bool true  - ���μ����� �⵿���̴�.
*             false - ���μ����� �⵿���� �ƴϴ�.
*             -1    - popen error
*****************************************************************************************/
bool IsProcessRun(int ac, char** av)
{
    // process alias�� �����.
    //----------------------------------------------------------
    char processName[100];
    memset(processName, 0x00, sizeof(processName));
    sprintf(processName, "%s", av[0]);
    for(int i=1; i < ac; i++)   sprintf(processName, "%s %s", processName, av[i]);

    // ps ���� ����
    //----------------------------------------------------------
    char sysCmd[512];
    sprintf(sysCmd, "ps -ef | grep '%s' | grep -v grep | grep -v tail | grep -v vi | grep -v cat | grep -v webadmin", processName);

    // pipe openó��.
    //----------------------------------------------------------
    FILE *pFd;
    if ((pFd = popen(sysCmd, "r")) == NULL) return -1;  // popen������ -1�� �����Ѵ�.

    // process�� ���ִ����� check�Ѵ�.
    //----------------------------------------------------------
    char compName[100]; // �� ���ڿ�. (ps���� ���� process name)
    char strOut  [100]; // popen���� ���� ���� ��� �޼���.
    bool isRun = false; // ���μ����� ���� ����.

    while (fgets(strOut, 100, pFd))
    {
        memset(compName, 0x00, sizeof(compName));
        memcpy(compName, strstr(strOut, processName), strlen(processName));

        if (strcmp(compName, processName) == 0)
        {
            // ���� ������ ���μ����� �������̴�.
            if (isRun == true) { pclose(pFd); return true; }
            isRun = true;
        }
    }

    pclose(pFd);
    return false;   // ���� ������ ���μ����� ���������� �ʴ�.
}


