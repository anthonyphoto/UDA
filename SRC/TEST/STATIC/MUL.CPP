#include <iostream.h>
#include <ShmInfo.hpp>
#include <SendLogManage.hpp>

extern SendLogManage*  clsLogM;

int mul(int a, int b)
{
	*clsLogM << "a = " << a << clsLogM->ErrorCode(0) <<endl;
	*clsLogM << "b = " << b << clsLogM->ErrorCode(0) <<endl;
	*clsLogM << "MUL = " << (a * b) << clsLogM->ErrorCode(0) <<endl;

	return (a*b);
}
