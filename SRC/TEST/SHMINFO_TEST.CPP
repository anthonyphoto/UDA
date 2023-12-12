#include <ShmInfo.hpp>
#include <SharedMemLegacy.hpp>


int main(int argc, char* argv[])
{
	if (argc < 7)
	{
		cout << "arg check" << endl;
		exit(1);
	}
	
	
	char*			strTest;
	int				nRet;
	
	
	SharedMemory	clsShmInfo(	atoi(argv[1]),
								atoi(argv[2]),
								atoi(argv[3]),
								atoi(argv[4]),
								atoi(argv[5]),
								atoi(argv[6])	);

	strTest = (char*)clsShmInfo.get_ShmPtr();
	if(strTest == NULL)
		cout << "FAIL : get_ShmPtr()"	<< endl;
	
	if (clsShmInfo.mem_lock() != 0)
		cout << "FAIL : mem_lock()"	<< endl;

	strcpy(strTest, "Test, TEST, TEST");

	if (clsShmInfo.mem_unlock() != 0)
		cout << "FAIL : mem_unlock()"	<< endl;
	
	cout << "strTest  :"	<< strTest  << endl;
/*	
	if ( nRet != 0 )
	{
		cout << "DB Select Failure [" << nRet << "]" << endl;
		exit(-1);
	}
	else
	{
		cout << "\nResult"	<< endl;
		cout << "proc_alias :"	<< clsShmInfo.proc_alias << endl;
		cout << "shmtype    :"	<< clsShmInfo.shmtype	  << endl;
		cout << "keypath    :"	<< clsShmInfo.keypath    << endl;
		cout << "key        :"	<< clsShmInfo.key	      << endl;
		cout << "rsize      :"	<< clsShmInfo.rsize      << endl;
		cout << "rcount     :"	<< clsShmInfo.rcount     << endl;
		cout << "shmid      :"	<< clsShmInfo.shmid      << endl;
		cout << "shmkey     :"	<< clsShmInfo.shmkey     << endl;
		cout << "used_size  :"	<< clsShmInfo.used_size  << endl;
	}
*/		
}		
