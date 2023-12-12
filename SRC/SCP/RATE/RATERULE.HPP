/****************************************************************************/
/* 파 일 명 : RateRule.cpp													*/
/* 작 성 자 : KSI / 조상영								                    */
/* 기    능 : 평일,휴일, 시간별 10초당 이용요금 산출						*/
/* 작성일자 : 2000년 10월 30일                                              */
/* 변경일자 :     년   월   일                                              */
/* 관련화일 : rate_tbl.cfg, holiday.cfg 									*/
/* 기    타 :                                                               */
/****************************************************************************/
#ifndef _RATERULE_HPP
#define _RATERULE_HPP

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define  DB_USERNAME  "uda"
#define  DB_PASSWORD  "uda"

EXEC    SQL     INCLUDE SQLCA;
EXEC    SQL     INCLUDE SQLDA;
EXEC    SQL     INCLUDE ORACA;


EXEC SQL BEGIN DECLARE SECTION;
VARCHAR	username[32];
VARCHAR	passwd[32];
int		nRateType;
int		nTodayType;
int		nStartTime;
int		nEndTime;
int		nTimeRate;
int		nPacketRate;
int		nPacketSize;
char	sHoliDay[8+1];
EXEC SQL END DECLARE SECTION;


// Cusor declare 
EXEC SQL declare rate_cur cursor for
		select  rate_type, today_type, start_time, end_time, time_rate, packet_rate, packet_size 
		from    t_rate_rule;
		
EXEC SQL declare holiday_cur cursor for
		select  holiday_type 
		from    t_holiday;
				

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
	int		Rate;	// 10초당 통화료
	int		PRate;	// 패킷당 통화료
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
		int				GetTelRate(int amount, int select);	
		int				GetRate(int select);	// select : 계산기준 1(시작) 2(끝)
		
		int				ConnectToDB(void);
		int				Db_close(void);
		
		int 			FetchRateDB(void);
		int				FetchHolidayDB(void);
	
	public:
		RateRule(void);
		~RateRule(void){;}
		
		//시간갭이 1분30초를 넘을수는 없다.
		// rtype : 1(시간) 2(패킷) / stime: 시작시간  / etime: 끝시간  / parket : 패킷양
		int GetRate(int rtype, time_t stime, time_t etime, int packet=0); 
};


//--------------------------------------------------------------------
RateRule::RateRule(void)
{
	int			nSqlcode;
	time_t		today;
	FILE		*fp;
	
	time(&today);
	Tm = localtime(&today);	// TM 을 오늘날짜로 초기화
	
	chk_H=Rn=Hn=0;
	memset(strDay, 0x00, sizeof(strDay));
	
	DBerror=ConnectToDB();	// 오라클연결
	if(DBerror!=0) 
	{
		printf("DB Connect Error : [%d]", DBerror);
		return;	// error 발생
	}
	
	//--------------- Rate 테이블 읽기
	EXEC SQL OPEN rate_cur;
	for (;;) 
	{
		nSqlcode = FetchRateDB();
		if( nSqlcode != 0 || nSqlcode == 1403)	break;
		
		tblRate[Rn].Rtype = nRateType;
		tblRate[Rn].Today = nTodayType;
		tblRate[Rn].Stime = nStartTime;	
		tblRate[Rn].Ltime = nEndTime;	
		tblRate[Rn].Rate = nTimeRate;	
		tblRate[Rn].PRate = nPacketRate;	
		tblRate[Rn].PSize = nPacketSize;	
		Rn++;
	}
	EXEC SQL CLOSE rate_cur;
	
	
	//--------------- Holiday 테이블 읽기
	EXEC SQL OPEN holiday_cur;
	for (;;) 
	{
		nSqlcode = FetchHolidayDB();
		if( nSqlcode != 0 || nSqlcode == 1403)	break;
		
		memset(tblHoliday[Hn].Date, 0x00, 8+1);
		strncpy(tblHoliday[Hn].Date, sHoliDay, 8);
		Hn++;
	}
	EXEC SQL CLOSE holiday_cur;
	
	Db_close();
	
	/*
	fp = fopen(FNAME_RATE_TABLE, "r+b");   
	if(!fp) 
	{
		printf("FILE OPEN ERROR %s\n", FNAME_RATE_TABLE);
		return;
	}
	
	while(1)
	{
		if(!fread((void*)&tblRate[Rn], sizeof(RateTable), 1,  fp)) break;   
		//printf("tblRate[%d]=%d\n", Rn, tblRate[Rn].Today);
		Rn++;
	}
	fclose(fp);
	
	
	
	fp = fopen(FNAME_HOLIDAY_TABLE, "r+b");              
	if(!fp) 
	{
		printf("FILE OPEN ERROR %s\n", FNAME_HOLIDAY_TABLE);
		return;
	}
	
	while(1)
	{
		if(!fread((void*)&tblHoliday[Hn], sizeof(HolidayTable), 1,  fp)) break;   
		//printf("tblHoliday[%d]=%s\n", Hn, tblHoliday[Hn].Date);
		Hn++;
	}
	fclose(fp);	
	*/
	PSize=OnPacket=0;
}

//------------------------------------------------------------------------
int RateRule::ConnectToDB( void )
{
	strcpy((char *) username.arr, DB_USERNAME);
	username.len = (unsigned short) strlen(DB_USERNAME);

	strcpy((char *) passwd.arr, DB_PASSWORD);
	passwd.len = (unsigned short) strlen(DB_PASSWORD);
	
	EXEC SQL connect :username identified by :passwd;
	return(sqlca.sqlcode);
}

//------------------------------------------------------------------------
int RateRule::Db_close()
{
	EXEC	SQL	COMMIT	WORK	RELEASE;

	return(sqlca.sqlcode); 
}


//------------------------------------------------------------------------
int RateRule::FetchRateDB(void)
{
	EXEC SQL	fetch	rate_cur 
				into 	:nRateType, nTodayType, :nStartTime, :nEndTime, :nTimeRate, :nPacketRate, :nPacketSize;
	printf("fetch_rate sqlcode:[%d]\n",sqlca.sqlcode);
	return(sqlca.sqlcode);
}	

//------------------------------------------------------------------------
int RateRule::FetchHolidayDB(void)
{
	EXEC SQL	fetch	holiday_cur 
				into 	:sHoliDay;
	printf("fetch_holiday sqlcode:[%d]\n",sqlca.sqlcode);
	return(sqlca.sqlcode);
}	
	

//-------------------------------------------------------------------------
int	RateRule::CHK_Holyday(void)	
{
	int i=Hn;
	
	while(i--) if(!strncmp(tblHoliday[i].Date, strDay, 8)) return 1;

	return 0;	
	
}


//--------------------------------------------------------------------
int	RateRule::GetRate(void)
{
	int i=Rn;
	int j=(Tm->tm_hour*100) + Tm->tm_min;
	
	while(i--)	
		if(tblRate[i].Today==chk_H){
			if((tblRate[i].Stime <= j) && (tblRate[i].Ltime > j)) {
				if(OnPacket) {
					PSize = tblRate[i].PSize;
					return tblRate[i].PRate;
				}else return tblRate[i].Rate;
			}
		}
	
	return 0;
	
}


//--------------------------------------------------------------------
// select : 적용시각기준
int	RateRule::GetRate(int select)
{
			
	if(select==1) Tm = localtime(&STime);		//주일얻기
	else Tm = localtime(&ETime);				
	
	sprintf(strDay, "%04d%02d%02d", Tm->tm_year+1900, Tm->tm_mon+1, Tm->tm_mday);
	//printf("%04d%02d%02d %2d:%2d [%d]\n", Tm->tm_year+1900, Tm->tm_mon+1, Tm->tm_mday, Tm->tm_hour, Tm->tm_min, Tm->tm_wday);
	
	if(!Tm->tm_wday || CHK_Holyday()) chk_H=3;	//공휴일
	else if(!Tm->tm_wday==6) chk_H=2;	//토요일
	else chk_H = 1;		//평일
	
	return GetRate();
}


//--------------------------------------------------------------------
int	RateRule::GetTelRate(int amount, int select)	// 전화요금을 계산한다(시간단위)
{
	if(amount<0) return 0;
	
	int i, n = (amount * 0.1);
	int rate;
	
	rate=GetRate(select);
	
	i=(amount%10) * ((double)rate*0.1);
	//printf("n=%d rate=%d amount=%d  [%d]\n", n, rate, amount, i);
	n = (n*rate) + i;
	
	return n;
}

	
	
//--------------------------------------------------------------------
// 시간갭은 1분30초를 넘을수없다. (2개를 초과한 요금율이 적용되지 않는다)
int RateRule::GetRate(int rtype, time_t stime, time_t etime, int packet)
{
	if(DBerror!=0) return -1;
	
	int				ret;
	struct  tm 		tm1;	
	struct  tm 		tm2;	
	time_t 			tmImsi;
	
	if(rtype==2) OnPacket = 1;	// 패킷과금여부지정
	
	STime = stime;
	ETime = etime;
	
	memcpy((void *)&tm1, localtime(&stime), sizeof(struct tm));	
	memcpy((void *)&tm2, localtime(&etime), sizeof(struct tm));			
	
	
	if(rtype==1) //시간과금이면
	{
		if(tm1.tm_hour==tm2.tm_hour)
		{	//시간이 같으면 동일한 요금적용
			return GetTelRate(difftime(etime, stime),1);
		}else
		{	//시간이 다르면 
			tmImsi = stime + (60 - tm1.tm_sec);	// 바로다음시간 00분00초로 만든다.
		
			ret = GetTelRate(difftime(tmImsi, stime),1);
			ret += GetTelRate(difftime(etime, tmImsi),2);
			return ret;
		}
	}else
	{	// 패킷과금이면..
		
		ret = GetRate(2);

		if(!PSize) PSize=0;

		ret *= (packet / PSize);
		return (ret);
	}
}



//--------------------------------------------------------------------
//사용예
int main(void)
{
	struct  tm 	*Tm;
	time_t		today;
	time_t		onet, twot;
	
	time(&today);	// 오늘날자 얻기
	Tm = localtime(&today);
	
	Tm->tm_mday=29;
	Tm->tm_hour=5;
	Tm->tm_min=59;
	Tm->tm_sec=35;
	
	onet = mktime(Tm);
	
	Tm->tm_hour=6;
	Tm->tm_min=0;
	//Tm->tm_hour=7;
	//Tm->tm_min=59;
	Tm->tm_sec=40;
	
	twot = mktime(Tm);
	
	RateRule rate;
	
	
	printf("[[ %d ]]\n", rate.GetRate(1, onet, twot, 10));
	printf("[[ %d ]]\n", rate.GetRate(2, onet, twot, 10));
	/*
	printf("%d\n",	rate.GetRate("200011272350")); //12
	printf("%d\n",	rate.GetRate("200011281030")); //17
	printf("%d\n",	rate.GetRate("200012250520")); //8
	printf("%d\n",	rate.GetRate("200012241110")); //12
	*/
}




#endif
