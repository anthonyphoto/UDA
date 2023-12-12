/******************************************************************
* 파 일 명     : SharedMemLegacy.cpp		     					  
* 기   능                                                          
* :shared memory에 대한 각종 class와 method를 정의한다.			      
* 작 성 자     : 권 종 욱               		      				  
* first data  :  				  	     						  
* last updata : 2000. 12. 26  by 한 근 희   	 					  
* 관 련 화 일  : SharedMemLegacy.hpp  
* Version	  : 1.1( 2001. 05. 21 )    							  
* program history                        						  
*  2000.12.26 -> 프로그램 주석추가        	 				 		  
******************************************************************/
#include <SharedMemLegacy.hpp>
//===============================================================
// error Method								   					=
//  : error msg를 받아서 출력해준다.					       		=
// argument     : errmsg -> Error Message						=
// return value : None											=
//																=
//===============================================================
void ShmInfo::error (const char* errmsg) const
{
    extern int 	errno;
    char		strErrNo[10];

    memset(strErrNo, NULL, 10);

    if (errno)
    {
    	switch(errno)
    	{
    		case	EACCES	:
    			strcpy(strErrNo, "EACCES");
    			break;
    		case	EINVAL	:
				strcpy(strErrNo, "EINVAL");
    			break;
           	case	ENOMEM	:
           		strcpy(strErrNo, "ENOMEM");
    			break;
           	case	EMFILE	:
           		strcpy(strErrNo, "EMFILE");
    			break;
           	default			:
           		sprintf(strErrNo, "%06d", errno);
        }
		cout << "ERR : [" << errno << "] = " << strErrNo << "==> " << errmsg << endl;
	}
}
//===============================================================
// Constructor  							   					=
//  : 입력받은 인자로 shared memory를 생성해준다.		       		=
// argument     : strPathName -> key path						=
//				  nId -> key Id									=
//				  nSize	-> memory size							=
//				  nFlag	-> flag									=
// return value : None											=
//																=
//===============================================================
ShmBuf::ShmBuf(const char* strPathName, int nId, int nSize, int nFlag)
{
	shm_key  = ::ftok(strPathName, nId);

	// key 생성하지 못했을때, key file을 생성한후 다시 key를 생성. 2001.05.09
	if( shm_key == -1 )
	{
		char	strTemp[128];
	
		memset(strTemp, NULL, sizeof(strTemp));
		sprintf(strTemp,"touch %s",strPathName);
		system(strTemp);
		shm_key  = ::ftok(strPathName, nId);
		
		if( shm_key == -1 )
		{
			cout << "[ShmBuf] shared Memory Key Create Error:ftok(strPathName, nId)" << endl;
			exit( 0 );
		}
	}
	
	shm_size = nSize;

	if ((shm_id   = ::shmget(shm_key, 0, 0)) == -1)	//	SHM is not exist
	{
		shm_id   = ::shmget(shm_key, shm_size, nFlag);
		if ((int)(shm_ptr = ::shmat(shm_id, 0, 0)) == -1)
		{
			cout << strPathName << " " << nId << " " << nSize << " " << nFlag << endl;
			error ("1shm access fail args = ");
		}
//		memset((char *)shm_ptr, NULL, shm_size);
	}												//	SHM is already exist
	else
		if ((int)(shm_ptr = ::shmat(shm_id, 0, 0)) == -1)
		{
			cout << strPathName << " " << nId << " " << nSize << " " << nFlag << endl;
			error ("2shm access fail args = ");
		}
}
//===============================================================
//Constructor								   					=
//  : shared memory key와 id로 해당 shared memroy를 attach.  		=
// argument     : nShmKey -> shm key	 						=
//				  nShmId -> shm Id								=
//				  nSize	-> memory size							=
//				  nFlag	-> flag									=
// return value : None											=
//																=
//===============================================================
ShmBuf::ShmBuf(int nShmKey, int nShmId, int nSize, int nFlag)
{
	shm_key		= nShmKey;
	shm_id		= nShmId;
	shm_size 	= nSize;

//	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
//	cout << "nShmId= " << nShmId << "nShmKey= " << nShmKey << "nFlag=" << nFlag << endl;
	if ((int)(shm_ptr = ::shmat(shm_id, 0, 0)) == -1)
	{
		cout 	<< "shm_key = [" << shm_key << "] "
				<< "shm_id  = [" << shm_id  << "] "
				<< "shm_size= [" << shm_size<< endl;
		error ("1shm access fail args = ");
	}
//	cout << "shm_ptr= " << shm_ptr << endl;
}
//===============================================================
// shmget Method							   					=
//  : Flag로 shared memory를 가져온다.					  		=
// argument     : nFlag	-> flag			 						=
// return value : shared meory id								=
//																=
//===============================================================
int ShmBuf::shmget(int nFlag) const
{
	return(::shmget(shm_key, shm_size, nFlag));
}
//===============================================================
// shmget Method							   					=
//  : Flag, memory size로 shared memory를 가져온다.		  		=
// argument     : nSize	-> memory size	 						=
//				  nFlag	-> flag									=
// return value : shared meory id								=
//																=
//===============================================================
int ShmBuf::shmget(int nSize=0, int nFlag=0)  const
{
	return(::shmget(shm_key, nSize, nFlag));
}
//===============================================================
// shmat Method							   						=
//  : shared memory address와 flag로 shared memroy를 attach.		=
// argument     : shmaddr -> shared memory address				=
//				  flag -> flag									=
// return value : shared memory pointer							=
//																=
//===============================================================
void*  ShmBuf::shmat(const void* shmaddr=NULL, int flag=0)
{
	return(::shmat(shm_id, shmaddr, flag));
}
//===============================================================
// shmdt Method							   						=
//  : shared memory address로 shared memroy를 detach.			=
// argument     : shmaddr -> shared memory address				=
// return value : 0-> 정상										=
//				  -1-> error									=
//===============================================================
int ShmBuf::shmdt(const void *shmaddr)
{
	return(::shmdt(shmaddr));
}
//===============================================================
// shmctl Method							   					=
//  : shared memory를 조작하기 위한 method.				  		=
// argument     : cmd -> 제어를 위한 명령어.						=
//				  ShmComp ->  shared memory 정보					=
// return value : 0-> 정상종료									=
//				  -1-> error									=
//===============================================================
int ShmBuf::shmctl(int cmd, ShmInfo& ShmComp)
{
	return( ::shmctl(shm_id, cmd, ShmComp) );
}
//===============================================================
// shm_release Method						   					=
//  : 잠긴 공유메모리를 해제시켜 준다.						 		=
// argument     : None					 						=
// return value : 0-> success									=
//				  -1-> fail										=
//===============================================================
int ShmMemory::shm_release()
{
	int 	nRet = unlock();

	if (nRet < 0)
		error("shm_release Fail");

	return(nRet);
}
//===============================================================
// shm_lock Method							   					=
//  : 공유메모리를 잠궈준다.								 		=
// argument     : None					 						=
// return value : 0-> success									=
//				  -1-> fail										=
//===============================================================
int ShmMemory::shm_lock()
{
	int 	nRet = lock();

	if (nRet < 0)
		error("shm_lock Fail");

	return(nRet);
}
//===============================================================
// shm_mode Method							   					=
//  : shmctl을 이용하여 모드를 바꿔준다.					  		=
// argument     : si ->  shared memory 정보 						=
//				  nCmd -> IPC의 상태								=
// return value : 0-> 정상종료									=
//				 -1-> error										=
//===============================================================
int	ShmMemory::shm_mode(ShmInfo& si, int nCmd = IPC_STAT)
{
	return(shmctl(nCmd, si));
}
//===============================================================
// shm_modeset Method						   					=
//  : shared memory mode를 setting해준다.					  		=
// argument     : nMode -> shm key	 							=
// return value : 0-> 정상종료									=
//				  -1-> error									=
//===============================================================
int	ShmMemory::shm_modeset(int nMode)
{
	ShmBuf	si;

	shm_mode(si);

	si.shm_perm.mode = (mode_t) nMode;

	return(shm_mode(si, IPC_SET));
}
//===============================================================
// shm_stat Method							   					=
//  : shared memory mode를 현재정보로 setting.			  		=
// argument     : None					 						=
// return value : 0-> 정상종료									=
//				  -1-> error									=
//===============================================================
int	ShmMemory::shm_stat()
{
	ShmBuf	si;

	return(shm_mode(si));
}
//===============================================================
// shm_destory Method						   					=
//  : shared memory를 삭제하도록 setting					  		=
// argument     : None					 						=
// return value : 0-> 정상종료									=
//				  -1-> error									=
//===============================================================
int	ShmMemory::shm_destory()
{
	ShmBuf	si;

	return(shm_mode(si, IPC_RMID));
}
//===============================================================
// shm_detach Method					   						=
//  : shmdt를 이용하여 shared memroy를 detach.					=
// argument     : shmaddr -> shared memory address				=
// return value : 0-> 정상										=
//				  -1-> error									=
//===============================================================
int	ShmMemory::shm_detach(const void *shmaddr)
{
	return(shmdt(shmaddr));
}





