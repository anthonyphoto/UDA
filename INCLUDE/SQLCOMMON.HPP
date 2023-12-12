/******************************************************************
* �� �� ��     : SqlCommon.hpp 			     					  *
* ��   ��                                                          *
* : DB connection, close, �׸��� ���ڷ� ���� ���μ��� ���󱸺���        *
*   ������ �ִ� ����.                     	 				 		  *
* �� �� ��     : �� �� ��               		      				  *
* first data  :  				  	     						  *
* last updata : 2000. 12. 23  by �� �� ��   	 					  *
* �� �� ȭ ��  : SqlCommon.pc              						  *
* program history                        						  *
*  2000.12.23 -> ���α׷� �ּ��߰�        	 				 		  *
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
//  : DB�� ����� ���Ḧ ����ϴ� class						   		=
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
//  : ���ڷ� �Է¹��� ���μ��� ���󱸺��� ������ �ִ� class	   		=
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