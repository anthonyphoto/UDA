/******************************************************************
* 파 일 명     : ShmInfo.hpp 			     					  *
* 기   능                                                          *
* :입력받은 형상구분으로 해당 shared memory key를 DB에서 가져온다.       *
* 작 성 자     : 권 종 욱               		      				  *
* first data  :  				  	     						  *
* last updata : 2000. 12. 26  by 한 근 희   	 					  *
* 관 련 화 일  : ShmInfo.pc             							  *
* program history                        						  *
*  2000.12.26 -> 프로그램 주석추가        	 				 		  *
* bug fix                        	     						  *
*                         	        	 						  *
******************************************************************/
#ifndef SHMINFO_HPP
#define SHMINFO_HPP

#include <iostream.h>
#include <iomanip.h>
#include <string.h>
#include <stdlib.h>
#include <strstream.h>

#ifndef	SQLCOMMON_HPP
#include <SqlCommon.hpp>
#endif

class ShmConfInfo	: public ProcessID
{
	public:
		char			proc_alias[9];  // process alias
        char			shmtype[3];     // MQ or MDB
        char			keypath[33];    // Key path
        char			key[2];         // Key ID
        int				rsize;          // record size
        int				rcount;         // record count
        int				shmid;          // shared memory ID
        int				shmkey;         // shared memory Key
        int				used_size;      // 사용하는 memory size

        ShmConfInfo	(	int		nLSystemId,					\
						int		nLServiceId,				\
						int		nLGroupId,					\
						int		nLProcessType,				\
						int		nLSerialNo,					\
						int		nLRemark					\
					)	: 	ProcessID 	(	nLSystemId,		\
											nLServiceId,	\
											nLGroupId,		\
											nLProcessType,	\
											nLSerialNo,		\
											nLRemark		\
										)
										{
											//cout<< "shmInfo:nLRemark:"<<nLRemark<<endl;
										}

//		operator ProcessID& const ()	{	return (ProcessID&)this;	}

		int get_t_shm_info();
};
#endif

