#include <SharedMemLegacy.hpp>

void ShmInfo::error (const char* errmsg) const
{
    extern int errno;

    if (errno)
		cout << "ERR " << errno << ": " << errmsg << endl;
}

ShmBuf::ShmBuf(const char* strPathName, int nId, int nSize, int nFlag)
{
	shm_key  = ::ftok(strPathName, nId);
	
	shm_size = nSize;
	
	if ((shm_id   = ::shmget(shm_key, 0, 0)) == -1)	//	SHM is not exist
	{
		shm_id   = ::shmget(shm_key, shm_size, nFlag);
		if ((long)(shm_ptr = ::shmat(shm_id, 0, 0)) == -1)
		{	
			cout << strPathName << " " << nId << " " << nSize << " " << nFlag << endl;
			error ("1shm access fail args = ");
		}
//		memset((char *)shm_ptr, NULL, shm_size);
	}												//	SHM is already exist
	else
		if ((long)(shm_ptr = ::shmat(shm_id, 0, 0)) == -1)
		{	
			cout << strPathName << " " << nId << " " << nSize << " " << nFlag << endl;
			error ("2shm access fail args = ");
		}
}

ShmBuf::ShmBuf(int nShmKey, int nShmId, int nSize, int nFlag)
{
	shm_key		= nShmKey;
	shm_id		= nShmId;
	shm_size 	= nSize;
	
	if ((long)(shm_ptr = ::shmat(shm_id, 0, 0)) == -1)
	{	
		cout 	<< "shm_key = [" << shm_key << "] "
				<< "shm_id  = [" << shm_id  << "] "
				<< "shm_size= [" << shm_size<< endl;
		error ("1shm access fail args = ");
	}
}

int ShmBuf::shmget(int nFlag) const
{
	return(::shmget(shm_key, shm_size, nFlag));
}

int ShmBuf::shmget(int nSize=0, int nFlag=0)  const
{
	return(::shmget(shm_key, nSize, nFlag));
}

void*  ShmBuf::shmat(const void* shmaddr=NULL, int flag=0)
{
	return(::shmat(shm_id, shmaddr, flag));
}

int ShmBuf::shmdt(const void *shmaddr)
{
	return(::shmdt(shmaddr));
}

int ShmBuf::shmctl(int cmd, ShmInfo& ShmComp)
{
	return( ::shmctl(shm_id, cmd, ShmComp) );
}
 
int ShmMemory::shm_release()
{
	int 	nRet = unlock();
	
	if (nRet < 0)
		error("shm_lock Fail");
		
	return(nRet);
} 

int ShmMemory::shm_lock()
{
	int 	nRet = lock();
	
	if (nRet < 0)
		error("shm_lock Fail");
		
	return(nRet);
} 

int	ShmMemory::shm_mode(ShmInfo& si, int nCmd = IPC_STAT)
{
	return(shmctl(nCmd, si));
}

int	ShmMemory::shm_modeset(int nMode)
{
	ShmBuf	si;
	
	shm_mode(si);
	
	si.shm_perm.mode = (mode_t) nMode;
	
	return(shm_mode(si, IPC_SET));
}

int	ShmMemory::shm_stat()
{
	ShmBuf	si;
	
	return(shm_mode(si));
}

int	ShmMemory::shm_destory()
{
	ShmBuf	si;
	
	return(shm_mode(si, IPC_RMID));
}





