#ifndef _PROCPERFORM_HPP
#define _PROCPERFORM_HPP

#include <iostream.h>
#include <iomanip.h>
#include <string.h>
#include <stdlib.h>
#include <strstream.h>
#include <SqlCommon.hpp>

#define	PROC_ID_LEN	8
#define	DATE_LEN	14	//YYYYMMDDhhmmss

//--------------------------------------------------------------------
class ProcPerform	: public ProcessID
{
	private:
	/*
		int			nBillType;
		int			nFileSeq;
        char		ProcId[PROC_ID_LEN+1];
        int			nUdrCount;
        int			nQueryCount;
        char		SDate[DATE_LEN+1];
        char		EDate[DATE_LEN+1];
    */
	
	public:
        
        ProcPerform	(	int		nLSystemId,					\
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
										){}
										
										
		//								get_nFailUdrCount
		int Set_PerForm(int	billtype,int fileseq, int udrcount, int failcount, int querycount, char	*sdate,char *edate);
};



#endif

