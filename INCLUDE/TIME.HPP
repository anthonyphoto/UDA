#ifndef	CTIME_HPP
#define CTIME_HPP

#include <iostream.h>
#include <time.h>
#include <stdlib.h>
#include <fstream.h>

const int       LEN_DATE    = 20;
const int       LEN_TIME    = 20;

class   DateTime
{
    private:
    	int			nYear, nMonth, nDay;
    	char		strDate[LEN_DATE+1];
    	char		strTime[LEN_TIME+1];
    	char		strDateTime[LEN_DATE+LEN_TIME+1];
        time_t	    t;
	    struct tm   *s;
    public:
        DateTime();
		~DateTime(){;}
       	char*	get_date(int nType=0);
       	char*	get_time(int nType=0);
       	char*	get_DateTime(int nType=0);
       	char*	get_DateTime2(int nTime, int nType=0);
       	void	putStrDateTime(char*	strDateTime);
       	time_t	get_time_t(char*	strDateTime);
       	void	moveStrDate2StructTime(struct tm* stTM);
       	
       	//sycho-----------------------------------------
       	char*	get_DateTimeT(time_t tTime, int nType=0);
       	int		get_Year(void);
       	int		get_Month(void);
       	int		get_Day(void);
       	void	calu_addMonth(int mon); 		// 월을 Plus시킨다
       	char*	get_Mtime(int nType=0);			// 1/100 초 2001.0409
       	char*	get_MDateTime(int nType=0);
       	
};

#endif
