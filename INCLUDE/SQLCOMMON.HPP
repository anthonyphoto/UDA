/******************************************************************
* 파 일 명     : SqlCommon.hpp 			     					  *
* 기   능                                                          *
* : DB connection, close, 그리고 인자로 받은 프로세스 형상구분을        *
*   가지고 있는 파일.                     	 				 		  *
* 작 성 자     : 권 종 욱               		      				  *
* first data  :  				  	     						  *
* last updata : 2000. 12. 23  by 한 근 희   	 					  *
* 관 련 화 일  : SqlCommon.pc              						  *
* program history                        						  *
*  2000.12.23 -> 프로그램 주석추가        	 				 		  *
* bug fix                        	     						  *
*                         	        	 						  *
******************************************************************/
#ifndef	SQLCOMMON_HPP
#define	SQLCOMMON_HPP

#include <iostream.h>
#include <iomanip.h>
#include <string.h>

#include <stdlib.h>
#include <strstream.h>

//===============================================================
// DbConnect Class												=
//  : DB의 연결과 종료를 담당하는 class						   		=
//																=
//===============================================================
class DbConnect
{
	public:
		DbConnect();
		int connect();
		int	disconnect();
};
//===============================================================
// ProcessID Class												=
//  : 인자로 입력받은 프로세스 형상구분을 가지고 있는 class	   		=
//																=
//===============================================================
class ProcessID
{
	public:
		int		sys_id;			//	SYSTEM ID
		int		svc_id;			//	SERVICE ID
		int		group_type;		//	GROUP ID
		int		proc_type;		//	PROCESS TYPE
		int		proc_sn;		//	PROCESS SERIAL NUMBER
		int		proc_remark;	//	PROCESS REMARK

		ProcessID(	int		nLSystemId,		\
					int		nLServiceId,	\
					int		nLGroupId,		\
					int		nLProcessType,	\
					int		nLSerialNo,		\
					int		nLRemark		\
				);
/*
		strstream operator << (ProcessID& clsPID);

		char*		makeStrProcessID();
		int			makeIntProcessID();
*/
		ProcessID*	get_ProcessID()		{	return (ProcessID*)this;	}
};

#endif
