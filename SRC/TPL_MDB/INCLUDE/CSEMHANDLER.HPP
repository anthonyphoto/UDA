/******************************************************************
* 파 일 명     : CSemHandler.hpp 		     					  
* 기   능                                                          
* : Semaphore operations    								      
* 작 성 자     : 권 종 욱               		      				  
* first data  :  				  	     						  
* last updata : 2000. 12. 23  by 한 근 희   	 					  
* 관 련 화 일  : CSemHandler.cpp             					  
* program history                        						  
*   . 프로그램 주석추가  ->  2000.12.26
*   . 신세기 UDA 적용 & 정리. -> 2001. 04. 23  
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
		int		semid;  // 세마포어에 접근했던 최근의 프로세스의 프로세스 식별번호 
		key_t	semkey;  // semaphore key
		semun	sem_union;

		struct sembuf op_lock;    // p()연산을 위한 sembuf
		struct sembuf op_release; // v()연산을 위한 sembuf
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


