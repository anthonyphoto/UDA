/******************************************************************
* �� �� ��     : ShmInfo.hpp 			     					  *
* ��   ��                                                          *
* :�Է¹��� ���󱸺����� �ش� shared memory key�� DB���� �����´�.       *
* �� �� ��     : �� �� ��               		      				  *
* first data  :  				  	     						  *
* last updata : 2000. 12. 26  by �� �� ��   	 					  *
* �� �� ȭ ��  : ShmInfo.pc             							  *
* program history                        						  *
*  2000.12.26 -> ���α׷� �ּ��߰�        	 				 		  *
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
        int				used_size;      // ����ϴ� memory size

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
