/******************************************************************
* 파 일 명     : CSemHandler.cpp 		     					  
* 기   능                                                          
* : Semaphore operations    								      
* 작 성 자     : 권 종 욱               		      				  
* first data  :  				  	     						  
* last updata : 2000. 12. 23  by 한 근 희   	 					  
* 관 련 화 일  : CSemHandler.hpp             						  
* program history                        						 
*   . 프로그램 주석추가  ->  2000.12.26
*   . 신세기 UDA 적용 & 정리. -> 2001. 04. 23        	 				 		  
* bug fix                        	     						  
*                         	        	 						  
******************************************************************/
#include <iostream.h>
#include <stdio.h>
#include <CSemHandler.hpp>

//===============================================================
// Constructor Method											
//  :두개의 sembuf구조체를 초기화하고, semaphore를 생성한다.	  		
// argument : strPathName -> Key Path							
//			  nId -> Key ID										
// return value : None											
//																
//===============================================================
CSemHandler::CSemHandler(const char* strPathName, int nId)
{
	op_lock.sem_num		= 0;
	op_lock.sem_op		= -1;
	op_lock.sem_flg		= SEM_UNDO;
//	op_lock.sem_flg		= 0;

	op_release.sem_num	= 0;   // 세마포어 number
	op_release.sem_op	= 1;   // 세마포어 operation
	op_release.sem_flg	= SEM_UNDO;  // operation flag
//	op_release.sem_flg	= 0;

	semkey = ftok(strPathName, nId);

	semid = CreateIPCSEM(1);
}
//===============================================================
// Constructor Method											
//  :두개의 sembuf구조체를 초기화하고, semaphore를 생성한다.	  		
// argument : key -> 유일한 Key									
// return value : None											
//																
//===============================================================
CSemHandler::CSemHandler(key_t key)
{
	op_lock.sem_num		= 0;
	op_lock.sem_op		= -1;
	op_lock.sem_flg		= SEM_UNDO;
//	op_lock.sem_flg		= 0;

	op_release.sem_num	= 0;   // 세마포어 number
	op_release.sem_op	= 1;   // 세마포어 operation
	op_release.sem_flg	= SEM_UNDO;  // operation flag
//	op_release.sem_flg	= 0;

	semkey = key;

	semid = CreateIPCSEM(1);
}
//===============================================================
// CreateIPCSEM Method					   						
//  : semaphore집합을 생성. 							       		
// argument     : nsems -> semaphore 갯수.						
// return value : 0-> success									
//                -1-> fail										
//																
//===============================================================
int CSemHandler::CreateIPCSEM(int nsems)	//	nsems = 1 (default)
{
    int	m_semid, status;

    if((m_semid = semget(semkey, nsems, SEMPERM | IPC_CREAT | IPC_EXCL)) == -1)
	{
		//	if already exist m_semid
		m_semid = semget(semkey, nsems, 0);
	}
	else
	{
		sem_union.val = 1;
		status = semctl(m_semid, 0, SETVAL, sem_union);
	}

	if (m_semid == -1 || status == -1)
	{
#ifdef	__DEBUG
		cout << "== CreateIPCSEM ==========================" << endl;
		cout << "m_semid = " << m_semid << "status = " << status << "errno = " << errno << endl;
		cout << "== CreateIPCSEM ==========================" << endl;
		perror("\tCSemHandler::CreateIPCSEM()");
		cout << "== CreateIPCSEM ==========================" << endl;
#endif
		return -1;
	}
	return m_semid;
}
//===============================================================
// CloseIPCSEM Method					   						
//  : semaphore집합을 제거 							       		
// argument     : None											
// return value : 0-> success									
//                -1-> fail										
//																
//===============================================================
int CSemHandler::CloseIPCSEM(void)
{
	if (semctl(semid, 0, IPC_RMID, 0) < 0)
	{
#ifdef	__DEBUG
		perror("CloseIPCSEM");
		DisplayError("CloseIPCSEM()");
#endif
        return -1;
	}

    return 0;
}
//===============================================================
// lock Method							   						
//  : semaphore p()연산을 수행.						       		
// argument     : None											
// return value : 0-> success									
//                -1-> fail										
//																
//===============================================================
int CSemHandler::lock()
{
	if(semop(semid, &op_lock, 1) == -1)
	{
#ifdef	__DEBUG
		cout << "== lock ==========================" << endl;
		cout << "errno = " << errno ;
		perror("\tCSemHandler::lock()");
		cout << "== lock ==========================" << endl;
#endif
		return -1;
	}
	return 0;
}
//===============================================================
// unlock Method							   					
//  : semaphore v()연산을 수행.						       		
// argument     : None											
// return value : 0-> success									
//                -1-> fail										
//																
//===============================================================
int CSemHandler::unlock()
{
	if(semop(semid, &op_release, 1) == -1)
	{
#ifdef	__DEBUG
		cout << "== unlock ==========================" << endl;
		cout << "errno = " << errno ;
		perror("\tCSemHandler::unlock()");
		cout << "== unlock ==========================" << endl;
#endif
		return -1;
	}
	return 0;
}
//===============================================================
// DisplayError Method						   					
//  : Error Number에 해당하는 Error Message를 출력.	       		
// argument     : str -> Error난 장소.							
// return value : None											
//																
//===============================================================
void CSemHandler::DisplayError(const char*	str)
{
	cout << "Position => " << str << " ERR No #" << errno << endl;
	switch(errno)
	{
		case EFBIG	:
			cout 	<< "sem_num is less than zero or greater than or equal "
					<< "to the number of semaphores in the set associated "
					<< "with semid."
					<< endl;
			break;

		case E2BIG	:
			cout	<< "nsops is greater than the system-imposed maximum."
					<< endl;
			break;

		case EACCES	:
			cout	<< "Operation permission is denied to the calling "
					<< "process (see glossary(9))."
					<< endl;
			break;

		case EAGAIN	:
			cout	<< "The operation would result in suspension of the "
					<< "calling process but (sem_flg & IPC_NOWAIT) is ``true''."
					<< endl;
			break;

		case ENOSPC	:
			cout	<< "The limit on the number of individual  processes "
					<< "requesting an SEM_UNDO would be exceeded."
					<< endl;
			break;

		case EINVAL	:
			cout	<< "The number of individual semaphores for which the "
					<< "calling process requests a SEM_UNDO would exceed the limit"
					<< endl;
			break;

		case ERANGE	:
			cout	<< "An operation would cause a semadj value to "
					<< "overflow the system-imposed limit."
					<< endl;

			cout	<< "An operation would cause a semval to overflow the "
					<< "system-imposed limit."
					<< endl;
			break;

		case EFAULT	:
			cout	<< "sops points to an illegal address.  The reliable "
					<< "detection of this error will be implementation "
					<< "dependent."
					<< endl;
		default	:
			cout	<< "Error Occured when Semaphore handling. errno #"
					<< errno	<< endl;
	}
}

