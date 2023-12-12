#ifndef _PROCEVENTHST_HPP
#define _PROCEVENTHST_HPP

#include <iostream.h>
#include <iomanip.h>
#include <string.h>
#include <stdlib.h>
#include <strstream.h>
#include <SqlCommon.hpp>

#define	DATE_LEN	14	//YYYYMMDDhhmmss

//--------------------------------------------------------------------
class ProcEventHist	: public ProcessID
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
        
        ProcEventHist(	int		nLSystemId,					\
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
										
										
		int Set_EventHist(char	*date, int exitcode);
};



#endif

