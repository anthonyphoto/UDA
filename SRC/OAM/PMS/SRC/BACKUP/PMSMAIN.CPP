#include "Cpms.hpp"
#include <time.h>

/*******************************************************************************
* PMS main program
*******************************************************************************/
int main(int argc, char *argv[])
{	
	CPMS pms;						// 객체 선언 및 초기화
	
	pms.Initialize(argc, argv);		// PMS 초기화 작업
	pms.ProcessCommand();			// Main Thread
}
