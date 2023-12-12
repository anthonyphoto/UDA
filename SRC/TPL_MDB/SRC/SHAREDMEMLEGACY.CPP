/******************************************************************
* �� �� ��     : SharedMemLegacy.cpp		     					  
* ��   ��                                                          
* :shared memory�� ���� ���� class�� method�� �����Ѵ�.			      
* �� �� ��     : �� �� ��               		      				 
* first data  :  				  	     						  
* last updata : 2000. 12. 26  by �� �� ��   	 					  
* �� �� ȭ ��  : SharedMemLegacy.hpp     							  
* program history                        						  
*   . ���α׷� �ּ��߰�  ->  2000.12.26
*   . �ż��� UDA ���� & ����. -> 2001. 04. 23  
* bug fix                        	     						  
*                         	        	 						  
******************************************************************/
#include <SharedMemLegacy.hpp>

//===============================================================
// error Method								   					
//  : error msg�� �޾Ƽ� ������ش�.					       		
// argument     : errmsg -> Error Message						
// return value : None											
//																
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
// Constructor  							   					
//  : �Է¹��� ���ڷ� shared memory�� �������ش�.		       		
// argument     : strPathName -> key path						
//				  nId -> key Id									
//				  nSize	-> memory size							
//				  nFlag	-> flag									
// return value : None											
//																
//===============================================================
ShmBuf::ShmBuf(const char* strPathName, int nId, int nSize, int nFlag)
{
	shm_key  = ::ftok(strPathName, nId);

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
//Constructor								   					
//  : shared memory key�� id�� �ش� shared memroy�� attach.  		
// argument     : nShmKey -> shm key	 						
//				  nShmId -> shm Id								
//				  nSize	-> memory size							
//				  nFlag	-> flag									
// return value : None											
//																
//===============================================================
ShmBuf::ShmBuf(int nShmKey, int nShmId, int nSize, int nFlag)
{
	shm_key		= nShmKey;
	shm_id		= nShmId;
	shm_size 	= nSize;


	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	cout << "nShmId= " << nShmId << "nShmKey= " << nShmKey << "nSize=" << nSize << "nFlag=" << nFlag << endl;
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
// shmget Method							   					
//  : Flag�� shared memory�� �����´�.					  		
// argument     : nFlag	-> flag			 						
// return value : shared meory id								
//																
//===============================================================
int ShmBuf::shmget(int nFlag) const
{
	return(::shmget(shm_key, shm_size, nFlag));
}
//===============================================================
// shmget Method							   					
//  : Flag, memory size�� shared memory�� �����´�.		  		
// argument     : nSize	-> memory size	 						
//				  nFlag	-> flag									
// return value : shared meory id								
//																
//===============================================================
int ShmBuf::shmget(int nSize=0, int nFlag=0)  const
{
	return(::shmget(shm_key, nSize, nFlag));
}
//===============================================================
// shmat Method							   						
//  : shared memory address�� flag�� shared memroy�� attach.		
// argument     : shmaddr -> shared memory address				
//				  flag -> flag									
// return value : shared memory pointer							
//																
//===============================================================
void*  ShmBuf::shmat(const void* shmaddr=NULL, int flag=0)
{
	return(::shmat(shm_id, shmaddr, flag));
}
//===============================================================
// shmdt Method							   						
//  : shared memory address�� shared memroy�� detach.			
// argument     : shmaddr -> shared memory address				
// return value : 0-> ����										
//				  -1-> error									
//===============================================================
int ShmBuf::shmdt(const void *shmaddr)
{
	return(::shmdt(shmaddr));
}
//===============================================================
// shmctl Method							   					
//  : shared memory�� �����ϱ� ���� method.				  		
// argument     : cmd -> ��� ���� ���ɾ�.						
//				  ShmComp ->  shared memory ����					
// return value : 0-> ��������									
//				  -1-> error									
//===============================================================
int ShmBuf::shmctl(int cmd, ShmInfo& ShmComp)
{
	return( ::shmctl(shm_id, cmd, ShmComp) );
}
//===============================================================
// shm_release Method						   					
//  : ��� �����޸𸮸� �������� �ش�.						 		
// argument     : None					 						
// return value : 0-> success									
//				  -1-> fail										
//===============================================================
int ShmMemory::shm_release()
{
	int 	nRet = unlock();

	if (nRet < 0)
		error("shm_release Fail");

	return(nRet);
}
//===============================================================
// shm_lock Method							   					
//  : �����޸𸮸� ����ش�.								 		
// argument     : None					 						
// return value : 0-> success									
//				  -1-> fail										
//===============================================================
int ShmMemory::shm_lock()
{
	int 	nRet = lock();

	if (nRet < 0)
		error("shm_lock Fail");

	return(nRet);
}
//===============================================================
// shm_mode Method							   					
//  : shmctl�� �̿��Ͽ� ��带 �ٲ��ش�.					  		
// argument     : si ->  shared memory ���� 						
//				  nCmd -> IPC�� ����								
// return value : 0-> ��������									
//				 -1-> error										
//===============================================================
int	ShmMemory::shm_mode(ShmInfo& si, int nCmd = IPC_STAT)
{
	return(shmctl(nCmd, si));
}
//===============================================================
// shm_modeset Method						   					
//  : shared memory mode�� setting���ش�.					  		
// argument     : nMode -> shm key	 							
// return value : 0-> ��������									
//				  -1-> error									
//===============================================================
int	ShmMemory::shm_modeset(int nMode)
{
	ShmBuf	si;

	shm_mode(si);

	si.shm_perm.mode = (mode_t) nMode;

	return(shm_mode(si, IPC_SET));
}
//===============================================================
// shm_stat Method							   					
//  : shared memory mode�� ���������� setting.			  		
// argument     : None					 						
// return value : 0-> ��������									
//				  -1-> error									
//===============================================================
int	ShmMemory::shm_stat()
{
	ShmBuf	si;

	return(shm_mode(si));
}
//===============================================================
// shm_destory Method						   					
//  : shared memory�� �����ϵ��� setting					  		
// argument     : None					 						
// return value : 0-> ��������									
//				  -1-> error									
//===============================================================
int	ShmMemory::shm_destory()
{
	ShmBuf	si;

	return(shm_mode(si, IPC_RMID));
}
//===============================================================
// shm_detach Method					   						
//  : shmdt�� �̿��Ͽ� shared memroy�� detach.					
// argument     : shmaddr -> shared memory address				
// return value : 0-> ����										
//				  -1-> error									
//===============================================================
int	ShmMemory::shm_detach(const void *shmaddr)
{
	return(shmdt(shmaddr));
}




