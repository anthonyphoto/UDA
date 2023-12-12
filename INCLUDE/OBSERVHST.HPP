#ifndef _OBSERVHST_HPP
#define _OBSERVHST_HPP

#include <iostream.h>
#include <iomanip.h>
#include <string.h>
#include <stdlib.h>
#include <strstream.h>
#include <SqlCommon.hpp>

#define	DATE_LEN	14	//YYYYMMDDhhmmss

//--------------------------------------------------------------------
class ObservHist	
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
        
        ObservHist(void){;}
		int SetObservHist(int trantype, int trancount, char *sdate, char *edate);
};



#endif

