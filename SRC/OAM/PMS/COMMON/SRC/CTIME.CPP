/* ---------------------------------------------------------
 *  Component Name : foundation
 *  Class Name     : CTime
 *  Programmer     : ParkDaeShin
 *  Purpose        : system date�� time�� �����ϰ� ����Ѵ�
 *			   �ð����̸� ���Ѵ�
 * --------------------------------------------------------- */

#define date_del 		"/"
#define time_del 		":"

// CTime class Header Include
#include "ctime.hpp"

#include <sys/time.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <iostream.h>

// Class CTime 
/* ---------------------------------------------------------
 *  Name     : CTime()
 *  Argument : void
 *  Return   : void
 *  Purpose  : CTime() constructor
 * --------------------------------------------------------- */
CTime::CTime()
{
}

/* ---------------------------------------------------------
 *  Name     : CTime(const CTime & source)
 *  Argument : CTime class
 *  Return   : void
 *  Purpose  : CTime(const CTime & source) constructor
 * --------------------------------------------------------- */
CTime::CTime(const CTime & source)
{
	m_year        = source.m_year;
	m_month       = source.m_month;
	m_day         = source.m_day;
	m_hour        = source.m_hour;
	m_minute      = source.m_minute;
	m_second      = source.m_second;
	m_microSecond = source.m_microSecond;
	
}

/* ---------------------------------------------------------
 *  Name     : CTime( int YYYY, int MM, int DD, int hh, int mm, int ss, int ms)
 *  Argument : YYYY -> m_year
 *             MM   -> m_month
 *             DD   -> m_day
 *             hh   -> m_hour
 *             mm   -> m_minute
 *             ss   -> m_second
 *             ms   -> micorSecond
 *  Return   : void
 *  Purpose  : CTime( int YYYY, int MM, int DD, int hh, int mm, int ss, int ms) constructor
 * --------------------------------------------------------- */
CTime::CTime( int YYYY, int MM, int DD, int hh, int mm, int ss, int ms)
{
	m_year = YYYY;
	m_month = MM;
	m_day = DD;
	m_hour = hh;
	m_minute = mm;
	m_second = ss;
	m_microSecond = ms;
}

/* ---------------------------------------------------------
 *  Name     : ~CTime()
 *  Argument : void
 *  Return   : void
 *  Purpose  : CTime destructor
 * --------------------------------------------------------- */
CTime::~CTime()
{
	
}


/* ---------------------------------------------------------
 *  Name     : operator =
 *  Argument : CTime 
 *  Return   : CTime 
 *  Purpose  : 
 * --------------------------------------------------------- */
const CTime & CTime::operator=(const CTime &source)
{
	this->m_year        = source.m_year;
	this->m_month       = source.m_month;
	this->m_day         = source.m_day;
	this->m_hour        = source.m_hour;
	this->m_minute      = source.m_minute;
	this->m_second      = source.m_second;
	this->m_microSecond = source.m_microSecond;

	return *this;
}


/* ---------------------------------------------------------
 *  Name     : operator >
 *  Argument : CTime
 *  Return   : int
 *  Purpose  : constructor of decompositionRuleC
 * --------------------------------------------------------- */
int CTime::operator > ( const CTime &source )
{
	if( this->m_year > source.m_year ||
	    this->m_month > source.m_month ||
	    this->m_day > source.m_day ||
	    this->m_hour > source.m_hour ||
	    this->m_minute > source.m_minute ||
	    this->m_second > source.m_second ||
	    this->m_microSecond > source.m_microSecond ) 
	    return True;
	else 
		return False;
}

/* ---------------------------------------------------------
 *  Name     : operator <=
 *  Argument : CTime
 *  Return   : int
 *  Purpose  : 
 * --------------------------------------------------------- */
int CTime::operator <= ( const CTime &source )
{
	if(*this > source)
	{
		return False;
	}
	else
	{
		return True;
	}
}

/* ---------------------------------------------------------
 *  Name     : operator < 
 *  Argument : CTime
 *  Return   : int
 *  Purpose  : 
 * --------------------------------------------------------- */
int CTime::operator < ( const CTime &source )
{
	if( this->m_year < source.m_year ||
		this->m_month < source.m_month ||
		this->m_day < source.m_day ||
		this->m_hour < source.m_hour ||
		this->m_minute < source.m_minute ||
		this->m_second < source.m_second ||
		this->m_microSecond < source.m_microSecond ) 
		return True;
	else 
		return False;
}

/* ---------------------------------------------------------
 *  Name     : operator >=
 *  Argument : CTime
 *  Return   : int
 *  Purpose  : 
 * --------------------------------------------------------- */
int CTime::operator >= ( const CTime &source )
{
	if(*this < source)
	{
		return False;
	}
	else
	{
		return True;
	}
}

/* ---------------------------------------------------------
 *  Name     : operator ==
 *  Argument : CTime
 *  Return   : int
 *  Purpose  : 
 * --------------------------------------------------------- */
int CTime::operator==(const CTime &source) const
{
	if( this->m_year == source.m_year &&
		this->m_month == source.m_month &&
		this->m_day == source.m_day &&
		this->m_hour == source.m_hour &&
		this->m_minute == source.m_minute &&
		this->m_second == source.m_second &&
		this->m_microSecond == source.m_microSecond ) 
		return True;
	else 
		return False;
}

/* ---------------------------------------------------------
 *  Name     : operator !=
 *  Argument : CTime
 *  Return   : int
 *  Purpose  : 
 * --------------------------------------------------------- */
int CTime::operator!=(const CTime &source) const
{
	if ( *this == source )
		return True;

	return False;
}


 /* ---------------------------------------------------------
 *  Name     : Time_Display
 *  Argument : dest     -> �ð��� ������ string
 *             source   -> CTime
 *             date_del -> ��,��,�� �� ���� �ϱ� ���� ������
 *             time_del -> ��,��,�� �� ���� �ϱ� ���� ������
 *  Return   : void
 *  Purpose  : �Է� ���� CTime�� ������ �����ϰ� �����ڸ� del�� 
 *             ����Ͽ� ��Ʈ������ �����Ͽ� dest�� �Ѱ� �ش�.
 *  mod : display ����
 *	1 : date display(��/��/��)
 *	2 : time display(��:��:��:����ũ����)
 *	3 : date time display(��/��/��/��:��:��:����ũ����)
 *	4 : date time display(�����)
 *	5 : date time display(�ú���)
 *	6 : date time display(�ú��ʸ���ũ����)
 *	7 : date time display(����Ͻú�)
 *	8 : date time display(����Ͻú���)
 *	9 : date time display(����Ͻú��ʸ���ũ����)
 *	default : date time display(��/��/��/��:��:��:����ũ����)
 * --------------------------------------------------------- */
void CTime::Time_Display ( char *dest, int mod)
{
	time_t		t;
	struct tm	*tp;
	struct timeb tb;
	struct timespec cur_time;
	char temp[128];
	char temp2[100];


	memset(temp2, 0x00 , sizeof(temp2));
	memset(temp, 0x00, sizeof temp);

	time(&t);
	tp = localtime(&t);
	ftime(&tb);
	clock_gettime(CLOCK_REALTIME,&cur_time);
	
	strftime( temp, sizeof temp, "%Y", tp );
	
	m_year		= atoi( temp );	/* years since 1900 */
	m_month		= tp->tm_mon + 1;	/* month of year - [0,11] */
	m_day			= tp->tm_mday;	/* day of month - [1,31] */
	m_hour		= tp->tm_hour;	/* hours - [0,23] */
	m_minute		= tp->tm_min;	/* minutes after the hour - [0,59] */
	m_second		= tp->tm_sec;	/* seconds after the minute - [0,61] */
	m_microSecond	= tb.millitm;	/* micor Second 100/1 =[0,100]*/
	m_nanoSecond	= cur_time.tv_nsec/100; /* (nano Second/100) */
	
	switch( mod )
	{
		// mod1 : date display(��/��/��)
		case 1 : sprintf( temp2, "%04d%s%02d%s%02d", 	get_year(), 
												date_del,
												get_month(), 
												date_del,
												get_day());
				break;
		
		// mod2 :  time display(��:��:��:����ũ����)
		case 2 : sprintf( temp2, "%02d%s%02d%s%02d%s%03d", 	get_hour(), 
													time_del,
													get_minute(), 
													time_del,
													get_second(), 
													time_del,
													get_microSecond());
				break;
		
		// mod3 :  date time display(��/��/��/��:��:��:����ũ����)
		case 3 : sprintf( temp2, "%04d%s%02d%s%02d%s %02d%s%02d%s%02d%s%03d", 	get_year(), 
																date_del,
															    get_month(), 
															    date_del,
															    get_day(), 
															    date_del,
																get_hour(), 
																time_del,
																get_minute(), 
																time_del,
																get_second(), 
																time_del,
																get_microSecond());
				break;
		
		// mod4 :  date time display(�����)
		case 4 : sprintf( temp2, "%04d%02d%02d", 	get_year(), 
																get_month(), 
															    get_day());
				break;
		
		// mod5 :  date time display(�ú���)
		case 5 : sprintf( temp2, "%02d%02d%02d", 	get_hour(), 
																get_minute(), 
																get_second());
				break;
				
		// mod6 :  date time display(�ú��ʸ���ũ����)
	//	case 6 : sprintf( temp2, "%02d%02d%02d%03d", 	get_hour(), 
		case 6 : sprintf( temp2, "%02d%02d%02d%03d", 	get_hour(),
																get_minute(), 
																get_second(), 
																get_microSecond());
				break;
		
		// mod7 :  date time display(����Ͻú�)
		case 7 : sprintf( temp2, "%04d%02d%02d%02d%02d%", 	get_year(), 
																get_month(), 
															    get_day(), 
															    get_hour(), 
																get_minute());
				break;
		
		// mod8 :  date time display(����Ͻú���)
		case 8 : sprintf( temp2, "%04d%02d%02d%02d%02d%02d", 	get_year(), 
																get_month(), 
															    get_day(), 
															    get_hour(), 
																get_minute(),
																get_second());
				break;
		
		// mod9 :  date time display(����Ͻú��ʸ���ũ����)
		case 9 : sprintf( temp2, "%04d%02d%02d%02d%02d%02d%03d", 	get_year(), 
																get_month(), 
															    get_day(), 
															    get_hour(), 
																get_minute(),
																get_second(),
																get_microSecond());
				break;
				
			// mod10 :  date time display(�ú���10-7��)	
		case 10 : sprintf( temp2, "%02d%02d%02d%07d", 	get_hour(),
																get_minute(), 
																get_second(), 
																get_nanoSecond());
				break;		
		
		// default :  date time display(��/��/��/��:��:��:����ũ����)
		default : sprintf( temp2, "%04d%s%02d%s%02d%s %02d%s%02d%s%02d%s%03d", 	get_year(), 
																date_del,
															    get_month(), 
															    date_del,
															    get_day(), 
															    date_del,
																get_hour(), 
																time_del,
																get_minute(), 
																time_del,
																get_second(), 
																time_del,
																get_microSecond());
				break;
	}
	
	strcpy ( dest, temp2 );
}

 /* ---------------------------------------------------------
 *  Name     : TimeDuration
 *  Argument : 	dest     -> �ð��� ������ string
 *             		fromtime   -> ���۽ð�("12:32:20:123"�� ���·� �Է��Ͽ��� �Ѵ�)
 *             		totime -> ���ð�("12:32:20:123"�� ���·� �Է��Ͽ��� �Ѵ�)
 *  Return   : void
 *  Purpose  : �Է� ����fromtime��  totime�� �ð����� second������ �ٲپ� ����� ���ϴ� ���·� 
 * 		     dest�� �Ѱ� �ش�.
 *  mod : display ����
 *	1 : second������ display ( ��> 200.123 )
 *	2 : ��:��:��:����ũ���� ���·� display ( ��>00:00:12:123 )
 * --------------------------------------------------------- */
void  CTime::TimeDuration ( char *dest, char *fromtime, char *totime, int mod)
{
	char Temp[100];
	char Temp2[100];
	char tmp[2];
	char tmp2[3];
	char tmp3[2];
	char tmp4[3];
	
	int h,m,s,ms,ftime,ttime,timeduration=0;
	float fs;
	
	// initialize
	memset(Temp, 0x00 , sizeof(Temp));
	memset(Temp2, 0x00 , sizeof(Temp2));
	memset( tmp, 0x00, sizeof tmp );
	memset( tmp2, 0x00, sizeof tmp2 );
	memset( tmp3, 0x00, sizeof tmp3 );
	memset( tmp4, 0x00, sizeof tmp4 );
	
	strcpy ( Temp, fromtime );
	strcpy ( Temp2, totime );
	
	// string type�� �ð��� int type�� second������ ��ȯ 
	tmp[0]=Temp[0];
	tmp[1]=Temp[1];
	h = atoi(tmp);
	ftime=h*60*60*1000;
	
	tmp[0]=Temp[3];
	tmp[1]=Temp[4];
	m = atoi(tmp);
	ftime=ftime+(m*60*1000);
	
	tmp[0]=Temp[6];
	tmp[1]=Temp[7];
	s = atoi(tmp);
	ftime = ftime+(s*1000);
	
	tmp2[0]=Temp[9];
	tmp2[1]=Temp[10];
	tmp2[2]=Temp[11];
	ms = atoi(tmp2);
	ftime = ftime+ms;
	
	tmp3[0]=Temp2[0];
	tmp3[1]=Temp2[1];
	h = atoi(tmp3);
	ttime=h*60*60*1000;
	
	tmp3[0]=Temp2[3];
	tmp3[1]=Temp2[4];
	m = atoi(tmp3);
	ttime=ttime+m*60*1000;
	
	tmp3[0]=Temp2[6];
	tmp3[1]=Temp2[7];
	s = atoi(tmp3);
	ttime = ttime+s*1000;
	
	tmp4[0]=Temp2[9];
	tmp4[1]=Temp2[10];
	tmp4[2]=Temp2[11];
	ms = atoi(tmp4);
	ttime = ttime+ms;
	
	if(ttime<ftime)
		timeduration = ftime - ttime;
	else
		timeduration = ttime - ftime;
	
	
	if(mod==1)
	{
		fs = timeduration;
		fs = fs/1000;
		sprintf(dest,"%.3f", fs);
	}
	else
	{
		// ��:��:��:mili�� ���·� ��ȯ
		int tmh, tmm, tms, tmms;
		tmh = timeduration/3600/1000;
		tmm = ((timeduration%3600000)/60)/1000;
		tms = ((timeduration%3600000)%60000)/1000;
		tmms = ((timeduration%3600000)%60000)%1000;
		sprintf(dest,"%d:%d:%d:%03d",tmh,tmm,tms,tmms);
	}
}

/* ---------------------------------------------------------
 *  Name     : DateTimeDuration
 *  Argument : 	dest     -> �ð��� ������ string
 *             		fromtime   -> ���۽ð�("12:32:20:123"�� ���·� �Է��Ͽ��� �Ѵ�)
 *             		totime -> ���ð�("12:32:20:123"�� ���·� �Է��Ͽ��� �Ѵ�)
 *  Return   : void
 *  Purpose  : �Է� ����fromtime��  totime�� �ð����� second������ �ٲپ� ����� ���ϴ� ���·� 
 * 		     dest�� �Ѱ� �ش�.
 *		     30�� �̳��� ��¥���̸� ����� �����ϴ�
 *  mod : display ����
 *	1 : second������ display ( ��> 200.123 )
 *	2 : ��:��:��:����ũ���� ���·� display ( ��>00:00:12:123 )
 * --------------------------------------------------------- */

void  CTime::DateTimeDuration ( char *dest, char *fromdatetime, char *todatetime, int mod)
{
	char Temp[100];
	char Temp2[100];
	char tmp[2];
	char tmp2[3];
	char tmp3[2];
	char tmp4[3];
	int d,d2,h,m,s,ms,dftime,dttime,datetimeduration=0;
	float fs;
	
	// initialize
	memset(Temp, 0x00 , sizeof(Temp));
	memset(Temp2, 0x00 , sizeof(Temp2));
	memset( tmp, 0x00, sizeof tmp );
	memset( tmp2, 0x00, sizeof tmp2 );
	memset( tmp3, 0x00, sizeof tmp3 );
	memset( tmp4, 0x00, sizeof tmp4 );
	
	strcpy ( Temp, fromdatetime );
	strcpy ( Temp2, todatetime );
	
	tmp[0]=Temp[12];
	tmp[1]=Temp[13];
	h = atoi(tmp);
	dftime=h*60*60*1000;
	
	// string type�� �ð��� int type�� second������ ��ȯ 
	tmp[0]=Temp[15];
	tmp[1]=Temp[16];
	m = atoi(tmp);
	dftime=dftime+m*60*1000;
	
	tmp[0]=Temp[18];
	tmp[1]=Temp[19];
	s = atoi(tmp);
	dftime = dftime+s*1000;
	
	tmp2[0]=Temp[21];
	tmp2[1]=Temp[22];
	tmp2[2]=Temp[23];
	ms = atoi(tmp2);
	dftime = dftime+ms;
	
	tmp3[0]=Temp2[12];
	tmp3[1]=Temp2[13];
	h = atoi(tmp3);
	dttime=h*60*60*1000;
	
	tmp3[0]=Temp2[15];
	tmp3[1]=Temp2[16];
	m = atoi(tmp3);
	dttime=dttime+m*60*1000;
	
	tmp3[0]=Temp2[18];
	tmp3[1]=Temp2[19];
	s = atoi(tmp3);
	dttime = dttime+s*1000;
	
	tmp3[0]=Temp[8];
	tmp3[1]=Temp[9];
	d = atoi(tmp3);
	
	tmp3[0]=Temp2[8];
	tmp3[1]=Temp2[9];
	d2 = atoi(tmp3);
	
	tmp4[0]=Temp2[21];
	tmp4[1]=Temp2[22];
	tmp4[2]=Temp2[23];
	ms = atoi(tmp4);
	dttime = dttime+ms;
	
	dttime =dttime+((d2-d)*3600*24*1000);
		
	datetimeduration = dttime - dftime;
		
	if(mod==1)
	{
		fs = datetimeduration;
		fs = fs/1000;
		sprintf(dest,"%.3f", fs);
	}
	else
	{
		// ��:��:��:mili�� ���·� ��ȯ
		int tmh, tmm,tms,tmms;
		tmh = datetimeduration/3600/1000;
		tmm = ((datetimeduration%3600)/60)/1000;
		tms = ((datetimeduration%3600)%60000)/1000;
		tmms = ((datetimeduration%3600000)%60000)%1000;
		sprintf(dest,"%d:%d:%d:%03d",tmh,tmm,tms,tmms);
	}
}

ostream &operator << (ostream & os, const CTime & t)
{
//	os << t.m_year << "/" << t.m_month << "/" << t.m_day << " " << t.m_hour << ":" << t.m_minute << ":" << t.m_second << ":" << t.m_microSecond;
	os << t.m_year << " " << t.m_month << " " << t.m_day << " " << t.m_hour << " " << t.m_minute << " " << t.m_second << " " << t.m_microSecond;
	return os;
}

// Update by cloud2000
istream & operator >> (istream & is, CTime &t)
{
	is >> t.m_year >> t.m_month >> t.m_day >> t.m_hour >> t.m_minute >> t.m_second >> t.m_microSecond;

	return is;
}


