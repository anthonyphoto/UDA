#ifndef _CUSTHIST_HPP
#define _CUSTHIST_HPP

#include <iostream.h>
#include <iomanip.h>
#include <string.h>
#include <stdlib.h>
#include <strstream.h>
#include <SqlCommon.hpp>

#define	DATE_LEN	14	//YYYYMMDDhhmmss

//--------------------------------------------------------------------
class CustHist	
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
        
        CustHist(void){;}
		int SetCustHist(int svctype, int custtype, char *strdate);
};



#endif

