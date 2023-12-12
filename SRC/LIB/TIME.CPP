#include <time.h>
#include <sys/timeb.h>
#include "time.hpp"

//-----------------------------------------------------------------------------
int	DateTime::get_Year(void)
{
	return nYear;	
}

//-----------------------------------------------------------------------------
int	DateTime::get_Month(void)
{
	return nMonth;
}

//-----------------------------------------------------------------------------
int	DateTime::get_Day(void)
{
	return nDay;
}


//--------------------------------------------------------------------------
DateTime::DateTime()
{
	memset(strDate, 	NULL, LEN_DATE+1);
	memset(strTime,		NULL, LEN_TIME+1);
	memset(strDateTime, NULL, LEN_TIME+LEN_TIME+1);
	nYear=nMonth=nDay=0;
}


//--------------------------------------------------------------------------
char*	DateTime::get_date(int nType)
{
	time(&t);
	s = (struct tm *) localtime( &t );

	nYear = s->tm_year+1900;
	nMonth = s->tm_mon+1;
	nDay = s->tm_mday;
	
	switch(nType)
	{
	    case    0:
	        sprintf(strDate,"%04d%02d%02d", s->tm_year+1900, s->tm_mon+1, s->tm_mday);
	        break;
        case    1:
	        sprintf(strDate,"%04d/%02d/%02d", s->tm_year+1900, s->tm_mon+1, s->tm_mday);
	        break;
	    default:
	        strcpy(strDate,"Unknown Type");
	        break;
    }
    return strDate;
}


//--------------------------------------------------------------------------
char*	DateTime::get_time(int nType)
{
	time(&t);
	s = (struct tm *) localtime( &t );

	nYear = s->tm_year+1900;
	nMonth = s->tm_mon+1;
	nDay = s->tm_mday;
	
	switch(nType)
	{
	    case    0:
	        sprintf(strTime,"%02d%02d%02d", s->tm_hour, s->tm_min, s->tm_sec);
        	break;
        case    1:
	        sprintf(strTime,"%02d:%02d:%02d", s->tm_hour, s->tm_min, s->tm_sec);
        	break;
	    default:
	        strcpy(strTime,"Unknown Type");
	        break;
    }
    return strTime;
}


//--------------------------------------------------------------------------

char*	DateTime::get_DateTime(int nType)
{
   	time(&t);
	s = (struct tm *) localtime( &t );

	nYear = s->tm_year+1900;
	nMonth = s->tm_mon+1;
	nDay = s->tm_mday;
	
	switch(nType)
	{
		case    0:
	        sprintf(strDateTime, "%04d%02d%02d%02d%02d%02d",		s->tm_year+1900,
    	        													s->tm_mon+1,
    	        													s->tm_mday,
    	        													s->tm_hour,
    	        													s->tm_min,
    	        													s->tm_sec);
	       break;

		case    1:
	        sprintf(strDateTime, "%04d/%02d/%02d %02d:%02d:%02d",	s->tm_year+1900,
    	        													s->tm_mon+1,
    	        													s->tm_mday,
    	        													s->tm_hour,
    	        													s->tm_min,
    	        													s->tm_sec);
	       break;

	    default:
	        strcpy(strDateTime,"Unknown Type");
	        break;
    }
    return strDateTime;
}
// 현재 쥴리언스 Time을 넣어주면, 원하는 형태의 시간으로 변경시켜 준다.
// 2001. 05. 28 by Han Guen Hee
char*	DateTime::get_DateTime2(int nTime, int nType)
{
  	t=(time_t)nTime;
  	//time(&t);
	s = (struct tm *) localtime( &t );

	switch(nType)
	{
		case    0:
	        sprintf(strDateTime, "%04d%02d%02d%02d%02d%02d",		s->tm_year+1900,
    	        													s->tm_mon+1,
    	        													s->tm_mday,
    	        													s->tm_hour,
    	        													s->tm_min,
    	        													s->tm_sec);
	       break;

		case    1:
	        sprintf(strDateTime, "%04d/%02d/%02d %02d:%02d:%02d",	s->tm_year+1900,
    	        													s->tm_mon+1,
    	        													s->tm_mday,
    	        													s->tm_hour,
    	        													s->tm_min,
    	        													s->tm_sec);
	       break;

	    default:
	        strcpy(strDateTime,"Unknown Type");
	        break;
    }
    return strDateTime;
}

//--------------------------------------------------------------------------
void	DateTime::putStrDateTime(char*	strDateTime)
{
	memcpy(strDate, strDateTime, 8);
	strDate[8] = NULL;

	memcpy(strTime, strDateTime+8, 6);
	strTime[6] = NULL;
}


//--------------------------------------------------------------------------
time_t	DateTime::get_time_t(char*	strDateTime)
{
	struct tm	stTM;

	putStrDateTime(strDateTime);

	moveStrDate2StructTime(&stTM);
	
	cout << "[TIME]------------------------------------"<<endl;
	cout << "[stTM.tm_hour] " << stTM.tm_hour << endl;
	cout << "[stTM.tm_min] " << stTM.tm_min << endl;
	cout << "[stTM.tm_sec] " << stTM.tm_sec << endl;

	return (mktime (&stTM));
}


//--------------------------------------------------------------------------
void	DateTime::moveStrDate2StructTime(struct tm* stTM)
{
	int			nTempValue;
	char		strTempValue[10];

	memcpy (strTempValue, strDate, 4);
	strTempValue[4] = NULL;
	nTempValue = atoi(strTempValue);

	stTM->tm_year = nTempValue - 1900;

	memcpy (strTempValue, strDate+4, 2);
	strTempValue[2] = NULL;
	nTempValue = atoi(strTempValue);

	stTM->tm_mon = nTempValue - 1;

	memcpy (strTempValue, strDate+6, 2);
	strTempValue[2] = NULL;
	nTempValue = atoi(strTempValue);

	stTM->tm_mday = nTempValue;

	memcpy (strTempValue, strTime, 2);
	strTempValue[2] = NULL;
	nTempValue = atoi(strTempValue);
	stTM->tm_hour = nTempValue;

	memcpy (strTempValue, strTime+2, 2);
	strTempValue[2] = NULL;
	nTempValue = atoi(strTempValue);
	stTM->tm_min = nTempValue;

	memcpy (strTempValue, strTime+4, 2);
	strTempValue[2] = NULL;
	nTempValue = atoi(strTempValue);
	stTM->tm_sec = nTempValue;
}



//--------------------------------------------------------------------------
// sycho------------------------------------
char*	DateTime::get_DateTimeT(time_t tTime, int nType)
{
	s = (struct tm *) localtime( &tTime );

	switch(nType)
	{
		case    0:
	        sprintf(strDateTime, "%04d%02d%02d%02d%02d%02d",		s->tm_year+1900,
    	        													s->tm_mon+1,
    	        													s->tm_mday,
    	        													s->tm_hour,
    	        													s->tm_min,
    	        													s->tm_sec);
	       break;

		case    1:
	        sprintf(strDateTime, "%04d/%02d/%02d %02d:%02d:%02d",	s->tm_year+1900,
    	        													s->tm_mon+1,
    	        													s->tm_mday,
    	        													s->tm_hour,
    	        													s->tm_min,
    	        													s->tm_sec);
	       break;

	    default:
	        strcpy(strDateTime,"Unknown Type");
	        break;
    }
    return strDateTime;	
	
}


//--------------------------------------------------------------------------
void	DateTime::calu_addMonth(int mon)
{
	int y, m;
	
	y=(mon/12);
	m=(mon%12);
	
	nYear+=y;
	nMonth+=m; 
}


//--------------------------------------------------------------------------
char*	DateTime::get_Mtime(int nType)
{
	struct 	timeb tb;
	struct  tm	  sTm;
	
	ftime(&tb);
	memcpy((char *)&sTm, (char *)localtime(&tb.time), sizeof(sTm));
	
	
	sprintf(strTime,"%02d%02d%02d%03d", sTm.tm_hour, sTm.tm_min, sTm.tm_sec, tb.millitm);
	
	if(!nType) strTime[8]=0x00;
	return strTime;
}


//--------------------------------------------------------------------------
char*	DateTime::get_MDateTime(int nType)
{
	struct 	timeb tb;
	struct  tm	  sTm;
	
	ftime(&tb);
	memcpy((char *)&sTm, (char *)localtime(&tb.time), sizeof(sTm));
	
	sprintf(strTime,"%04d%02d%02d%02d%02d%02d%03d", sTm.tm_year+1900, sTm.tm_mon+1, sTm.tm_mday,
					sTm.tm_hour, sTm.tm_min, sTm.tm_sec, tb.millitm);
	
	if(!nType) strTime[16]=0x00;
	return strTime;
}


