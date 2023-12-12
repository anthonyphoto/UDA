#ifndef _ERRORLOG_HPP
#define _ERRORLOG_HPP

#include <iostream.h>
#include <iomanip.h>
#include <string.h>
#include <stdlib.h>
#include <strstream.h>
#include <SqlCommon.hpp>

//--------------------------------------------------------------------
class ErrorLog	: public ProcessID
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

        ErrorLog(		int		nLSystemId,					\
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


		int Set_ErrorLog(char *sdate, char *stime, char *ecode, char *fname, char *strerr);
};



#endif

