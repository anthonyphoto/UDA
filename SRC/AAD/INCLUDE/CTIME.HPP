/* ---------------------------------------------------------
 *  Component Name : foundation
 *  Class Name     : CTime
 *  Programmer   : Park Dae Shin
 *  Purpose        : system date와 time을 생성하고 출력한다
 *			   시간차이를 구한다
 * --------------------------------------------------------- */

#ifndef _CTIME_
#define _CTIME_

#define		True		1
#define		False	0

// System Define Header File Include
#include <sys/time.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <iostream.h>

class CTime 
{

  private :
		int m_year;
        int m_month;
        int m_day;
        int m_hour;
        int m_minute;
        int m_second;
        int m_microSecond;
        long m_nanoSecond;
        long m_epochSecond;
        
        struct tm	fromtp;
		struct tm	totp;
		struct timespec fromnanosec;
		struct timespec tonanosec;

  public:
    //## Constructors (generated)
		CTime();
		CTime( const CTime &source );
		CTime( int YYYY, int MM, int DD, int hh, int mm, int ss, int ms);

    //## Destructor (generated)
		~CTime();

    //## Assignment Operation (generated)
	const CTime & operator=(const CTime &source);

	int operator > (const CTime &source);
	int operator >= (const CTime &source);
	int operator < (const CTime &source);
	int operator <= (const CTime &source);

	// Updated by cloud2000  99.09.04
	friend ostream & operator << (ostream & os, const CTime & t);
	// Updated by cloud2000  99.09.27
	friend istream & operator >> (istream & is, CTime & t);

    //## Equality Operations (generated)
    int operator==(const CTime &source) const;
    int operator!=(const CTime &source) const;

	time_t MakeTm ( struct tm tp, char *strTime );

	//## Time Display
	void Time_Display (char *dest, int mod);
	
	//## Time Duration
	void Duration (char *dest, char *fromtime, char *totime);
	void TimeDuration (char *dest, char *fromtime, char *totime, int mod);
	void DateTimeDuration (char *dest, char *fromdatetime, char *todatetime, int mod);
	void CalToTime ( char *dest, char *fromtime, char *strDuration );

  protected:
    // Additional Protected Declarations


  public:
		//## system date 및 time을 가져온다
		const int get_year () const { return m_year; }
		void set_year ( int value ) { m_year = value; }
		
		const int get_month () const { return m_month; }
		void set_month ( int value ) { m_month = value; }
		
		const int get_day () const { return m_day;	}
		void set_day ( int value ) { m_day = value; }
		
		const int get_hour () const { return m_hour; }
		void set_hour ( int value ) { m_hour = value; }
		
		const int get_minute () const { return m_minute; }
		void set_minute ( int value ) { m_minute = value; }
		
		const int get_second () const { return m_second; }
		void set_second ( int value ) { m_second = value; }
		
		const int get_microSecond () const { return m_microSecond; }
		void set_microSecond (int value)	{ m_microSecond = value; }
		
		const int get_nanoSecond () const { return m_nanoSecond; }
		void set_nanoSecond (int value)	{ m_nanoSecond = value; }
};

#endif
