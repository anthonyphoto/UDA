#include "Cpms.hpp"
#include <time.h>

/*******************************************************************************
* PMS main program
*******************************************************************************/
int main(int argc, char *argv[])
{	
	CPMS pms;						// ��ü ���� �� �ʱ�ȭ
	
	pms.Initialize(argc, argv);		// PMS �ʱ�ȭ �۾�
	pms.ProcessCommand();			// Main Thread
}