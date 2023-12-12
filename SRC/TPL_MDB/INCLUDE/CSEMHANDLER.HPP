/******************************************************************
* �� �� ��     : CSemHandler.hpp 		     					  
* ��   ��                                                          
* : Semaphore operations    								      
* �� �� ��     : �� �� ��               		      				  
* first data  :  				  	     						  
* last updata : 2000. 12. 23  by �� �� ��   	 					  
* �� �� ȭ ��  : CSemHandler.cpp             					  
* program history                        						  
*   . ���α׷� �ּ��߰�  ->  2000.12.26
*   . �ż��� UDA ���� & ����. -> 2001. 04. 23  
* bug fix                        	     						  
*                         	        	 						  
******************************************************************/
#ifndef _SEM_H
#define _SEM_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

const int SEMPERM = 0660;

typedef union _semun
{
	int 			val;
	struct semid_ds *buf;
	unsigned short 	*array;
}	semun;

class CSemHandler
{
	private :
		int		semid;  // ������� �����ߴ� �ֱ��� ���μ����� ���μ��� �ĺ���ȣ 
		key_t	semkey;  // semaphore key
		semun	sem_union;

		struct sembuf op_lock;    // p()������ ���� sembuf
		struct sembuf op_release; // v()������ ���� sembuf
	public :
		CSemHandler(){}
		CSemHandler(key_t key);
		CSemHandler(const char* strPathName, int nId);
		~CSemHandler(){}

		int CreateIPCSEM(int nsems = 1);
		int CloseIPCSEM(void);
		int lock();		//	p operation
		int unlock();	//	v operation
		void DisplayError(const char*	str);
};



#endif // _SEM_H

