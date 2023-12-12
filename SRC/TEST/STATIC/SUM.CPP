#include <iostream.h>
#include <ShmInfo.hpp>
#include <SendLogManage.hpp>

extern SendLogManage*  clsLogM;

int sum(int a, int b)
{
	*clsLogM << "a = " << a << clsLogM->ErrorCode(0) <<endl;
	*clsLogM << "b = " << b << clsLogM->ErrorCode(0) <<endl;
	*clsLogM << "SUM = " << (a + b) << clsLogM->ErrorCode(0) <<endl;

	return (a+b);
}
