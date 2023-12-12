#ifndef _RATERULE_HPP
#define _RATERULE_HPP

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <SendLogManage.hpp>

//#define  DB_USERNAME  "uda"
//#define  DB_PASSWORD  "uda"

/* 패킷과금 방법 */
/*
1. 패킷개수 = (패킷량 / 패킷size)
2. 요금 = (해당시간패킷당통화료) * (패킷개수)
*/


struct RATE_TABLE {
	int 	Rtype;	// 구분 (일반:1 할인:2 심야:3) => 서비스타입 TTL, N-TOP 인지 향후고려대상
	int		Today;	// 요일 (평일(월-토):1 휴일:2)
	int		Stime;	// 시작시간 = (HH*100)+mm
	int		Ltime;	// 끝시간  = (HH*100)+mm
	int		Rate;	// 10초당 통화료	(저속)
	int		PRate;	// 패킷당 통화료	(고속)
	int		PSize;	// 패킷Size
};

/* 구분코드(일반/할인/심야, 요일구분(평일,토요일,공휴일), 시작시각(HHmm), 끝시각, 10초당통화료, 패킷당통화료, 패킷Size 
RateTable 	tblRate[6]=	{{1,1,  800, 2100, 17, 17, 512},
						 {2,1,  600,  800, 12, 12, 512},
						 {2,1, 2100, 2400, 12, 12, 512},
						 {2,2,  600, 2400, 12, 12, 512},
						 {3,2,    0,  600,  8,  8, 512},
						 {3,1,    0,  600,  8,  8, 512} };

*/

struct HOLIDAY_TABLE {
	char 	Date[10];	// YYYYMMDD
	char	Name[10];	// 휴일명
};

typedef RATE_TABLE		RateTable;
typedef HOLIDAY_TABLE	HolidayTable;

#define		FNAME_RATE_TABLE		"/SVC1/DATA/rate_tbl.cfg"	// 현재파일 => DB변경필요
#define		FNAME_HOLIDAY_TABLE		"/SVC1/DATA/holiday.cfg"
#define		RATE_MAX	30			// 요금테이블 최대개수
#define		HOLIDAY_MAX	50			// 법정공휴일 최대개수

//--------------------------------------------------------------------
class RateRule {
	
	private:
		int				Rn;		// 요금테이블 개수
		int				Hn;		// 공휴일 개수
		int				chk_H;	// 1.평일 2.토요일 3.휴일
		int				OnPacket;	// 패킷과금여부
		int				PSize;
		int				LimitSec;
		int				fgHI;
		char			strDay[14];
		int				DBerror;
		
		time_t			STime;
		time_t			ETime;
		struct  tm 		*Tm;	
		RateTable		tblRate[RATE_MAX];		// 요금테이블
		HolidayTable	tblHoliday[HOLIDAY_MAX];// 법정공유일
		
		int				CHK_Holyday(void);		// 공유일체크
		int				GetRate(void);			// 요금테이블을 검색해 10초당요금 리턴
						// 전화요금을 계산한다(시간단위) amount : 시간양 select : 계산기준 1(시작) 2(끝)
		//int				GetTelRate(int amount, int select);	
		int				GetTelRate(int amount, int select, int last);	
		int				GetRate(int select);	// select : 계산기준 1(시작) 2(끝)
		
//		int				ConnectToDB(void);
//		int				Db_close(void);
		
		int 			FetchRateDB(void);
		int				FetchHolidayDB(void);
	
	public:
		RateRule(void);
		~RateRule(void){;}
		
		//int GetRate(int rtype, time_t stime, time_t etime, int packet=0); 
		// rtype : 1(저속) 2(고속) / stime: 시작시간  / etime: 끝시간  /last: 마지막계산여부 / parket : 패킷양
		int GetRate(int rtype, time_t stime, time_t etime, int last, int packet=1); 
		int	GetLimitSec(void)	{ return LimitSec; } // 계산하고 남은 시간 & 패킷
};

#endif


