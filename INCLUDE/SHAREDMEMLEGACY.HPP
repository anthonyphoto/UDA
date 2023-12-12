/******************************************************************
* 파 일 명     : SharedMemLegacy.hpp		     					  
* 기   능                                                         
* :shared memory에 대한 각종 class와 method를 정의한다.			      
*   shared memory와 semaphore를 외부에서 사용할수 있게 해준다.         
* 작 성 자     : 권 종 욱               		      				 
* first data  :  				  	     						  
* last updata : 2000. 12. 26  by 한 근 희   	 					  
* 관 련 화 일  : SharedMemLegacy.cpp  
* Version	  : 1.1( 2001. 05. 21 )    							  
* program history                        						  
*  2000.12.26 -> 프로그램 주석추가        	 				 		  
******************************************************************/
#ifndef SHAREDMEMLEGACY_HPP
#define SHAREDMEMLEGACY_HPP

//#include <sys/types.h>
#include <iostream.h>
#include <sys/shm.h>
#include <CSemHandler.hpp>

#include <ShmInfo.hpp>

// C의 shared memory system function을 extern으로 추가.
extern "C"
{
	int 	shmget(key_t key, size_t size, int oflag);
	void 	*shmat(int shmid, const void* shmaddr, int flag);
	int		shmdt(const void *shmaddr);
	int 	shmctl(int shmid, int cmd, struct shmid_ds *buff);
	key_t	ftok(const char *pathname, int id);
}
//===============================================================
// ShmInfo Class												=
//  : shared memory 정보를 가상함수로 초기화 하는 부분.		   		=
//																=
//===============================================================
class	ShmInfo
{
	private:

	public:
		ShmInfo(){}

		virtual operator struct shmid_ds* () const = 0;

		virtual int		ShmKey() const = 0;
		virtual int		ShmSize() const = 0;
		virtual int		ShmId() const = 0;

		void error (const char* errmsg) const;
};
//===============================================================
// ShmBuf Class													=
//  : shared memory를 실제 생성하거나, attach, detach시키는 class.	=
//																=
//===============================================================
class	ShmBuf	: public ShmInfo, public shmid_ds
{
	private:
		key_t	shm_key;
		size_t	shm_size;
		int		shm_id;
		void*	shm_ptr;

	public:
	 	ShmBuf(const char*, int, int, int);
	 	ShmBuf(int nShmKey, int nShmId, int nSize, int nFlag);
	 	ShmBuf(){}

	 	operator struct shmid_ds* () const { return (shmid_ds*)this; }

		int		ShmKey() const	{ return shm_key; }
		int		ShmSize() const	{ return shm_size; }
		int		ShmId() const 	{ return shm_id; }
		void*	ShmPtr() const 	{ return shm_ptr; }

		int		shmget(int) const;
		int		shmget(int nSize, int nFlag)  const;
		void* 	shmat(const void*, int);
		int		shmdt(const void *);
		int 	shmctl(int, ShmInfo&);

		void	displayShmBuf()
		{
			cout << "shm_key = " << shm_key << endl;
			cout << "shm_size = " << shm_size << endl;
			cout << "shm_id = " << shm_id << endl;
		//	cout << "shm_ptr = " << shm_ptr << endl;
		}
};
//===============================================================
// ShmMemory Class												=
//  : shared memory를 실제 생성하고, 그 밖의 method들을 정의     	=
//																=
//===============================================================
class	ShmMemory	: public ShmBuf, public CSemHandler
{
	public:
		ShmMemory(){}
		ShmMemory(const char* strPathName, int nId, int nSize = 0, int nFlag = 0600|IPC_CREAT)	\
			: CSemHandler(strPathName, nId),										\
			  ShmBuf(strPathName, nId, nSize, nFlag){}

		ShmMemory(int nShmKey, int nShmId, int nSize, int nFlag = 0)				\
			: CSemHandler((key_t)nShmKey),											\
			  ShmBuf(nShmKey, nShmId, nSize, nFlag){}

		int	shm_release();
		int shm_lock();
		int shm_modeset(int);
		int	shm_mode(ShmInfo& si, int nCmd);
		int	shm_stat();
		int	shm_destory();
		int	shm_detach(const void*);
};
//===============================================================
// SharedMemory Class											=
//  : 프로세스 형상구분이 들어왔을때, 그것을 이용해서 shared memory를   =
//	  생성하고, 다룰수 있는 class들을 생성하는 class.				=
//===============================================================
class	SharedMemory	: public ShmConfInfo
{
	private:
		ShmMemory*		clsShmMem;

	public:
		SharedMemory(	int		nLSystemId,					\
						int		nLServiceId,				\
						int		nLGroupId,					\
						int		nLProcessType,				\
						int		nLSerialNo,					\
						int		nLRemark					\
					)										\
			:	ShmConfInfo	(	nLSystemId,					\
								nLServiceId,				\
								nLGroupId,					\
								nLProcessType,				\
								nLSerialNo,					\
								nLRemark					\
							)
		{
			if (get_t_shm_info() != 0)
			{
				cout << "FAIL : get_t_shm_info(), so EXIT !!" << endl;
				exit(-1);
			}
			clsShmMem = new ShmMemory(shmkey, shmid, rsize * rcount);
		}

		SharedMemory(	int		nLSystemId,					\
						int		nLServiceId,				\
						int		nLGroupId,					\
						int		nLProcessType,				\
						int		nLSerialNo,					\
						int		nLRemark,					\
						int		nLSize						\
					)										\
			:	ShmConfInfo	(	nLSystemId,					\
								nLServiceId,				\
								nLGroupId,					\
								nLProcessType,				\
								nLSerialNo,					\
								nLRemark					\
							)
		{
			if (get_t_shm_info() != 0)
			{
				cout << "FAIL : get_t_shm_info(), so EXIT !!" << endl;
				exit(-1);
			}
			clsShmMem = new ShmMemory(shmkey, shmid, nLSize);
		}

		~SharedMemory()			{	delete	clsShmMem;				}
		void* 	get_ShmPtr()	{	return	clsShmMem->ShmPtr();	}
		int		mem_unlock()	{	return	clsShmMem->unlock();	}
		int 	mem_lock()		{	return	clsShmMem->lock();		}
		
		int		free_ShmPtr()	{	return	clsShmMem->shm_destory();}
		int		detach_ShmPtr(const void* detach_key)	{	return	clsShmMem->shm_detach( detach_key ); }

		int		get_rcount()	{	return	rcount;					}
};
//===============================================================
// Semaphore Class												=
//  : shared memory를 여러 프로세스가 동시에 사용하기 위해서 lock과   =
//	  unlock을 할수있는 semaphore를 정의해주는 class				=
//===============================================================
class	Semaphore	: public ShmConfInfo
{
	private:
		CSemHandler*	CSema;

	public:
		Semaphore(	int		nLSystemId,					\
					int		nLServiceId,				\
					int		nLGroupId,					\
					int		nLProcessType,				\
					int		nLSerialNo,					\
					int		nLRemark					\
				)										\
		:	ShmConfInfo	(	nLSystemId,					\
							nLServiceId,				\
							nLGroupId,					\
							nLProcessType,				\
							nLSerialNo,					\
							nLRemark					\
						)
		{
			if (get_t_shm_info() != 0)
			{
				cout << "FAIL : get_t_shm_info(), so EXIT !!" << endl;
				exit(-1);
			}
			CSema = new CSemHandler((key_t)shmkey);
		}
		~Semaphore()			{	delete	CSema;				}
		int		mem_unlock()	{	return	CSema->unlock();	}
		int 	mem_lock()		{	return	CSema->lock();		}
};


#endif
