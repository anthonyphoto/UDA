#include <LogManageSM.hpp>


int main(int argc, char *argv[])
{
	
	if(argc<6) 
	{
		cout << " Usage:" << argv[0] << "[systemid] [serviceid] [groupid] [processtype] [serialno] [remarks]" << endl;
		exit(1);
	}
	
//	int			nRet;
//	DbConnect	clsDbConnect;
	LogManageSM	LogSM(1,1,1,2,0,0);

	//nRet = clsDbConnect.connect();
	/*
	if (nRet < 0)
	{
		cout << "DB Connect Failure [" << nRet << "]" << endl;
		return nRet;
	}else cout << "DB Connect Success " << endl;
*/
		
	LogSM.LogSMinit();
	
	//LogSM.LogInsert(1,1,1,1,1,2, 2, 0, 1, "aaaaaaaaaa");	
	//LogSM.LogInsert(1,1,1,1,1,3, 2, 1, 1, "bbbbbbbbbb");	
	
	//LogSM.Set_GroupSwitch(1,1,1, 5);	
	LogSM.Set_Switch(2,3,5,1,2,2,12,  0);	
	//LogSM.Set_SysSwitch(1,8);	
	//LogSM.LogDelete(1,1,1,1,1, 3, 2);
	
	//clsDbConnect.disconnect();
	exit(1);
}

