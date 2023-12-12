#include <SharedMemLegacy.hpp>
#include <ShmInfo.hpp>

SharedMemory::SharedMemory	(	int		nLSystemId,				\
								int		nLServiceId,			\
								int		nLGroupId,				\
								int		nLProcessType,			\
								int		nLSerialNo,				\
								int		nLRemark				\
							)
{
	if (get_t_shm_info() != 0)
	{
		cout << "FAIL : get_t_shm_info(), so EXIT !!" << endl;
		exit(-1);
	}
	clsShmMem = new ShmMemory(shmkey, shmid, rsize * rcount);
}

SharedMemory::~SharedMemory()
{
	delete clsShmMem;
}


