
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned long magic;
  };
  typedef struct sql_cursor sql_cursor;
  typedef struct sql_cursor SQL_CURSOR;
#endif /* SQL_CRSR */

/* Thread Safety */
typedef void * sql_context;
typedef void * SQL_CONTEXT;

/* Object support */
struct sqltvn
{
  unsigned char *tvnvsn; 
  unsigned short tvnvsnl; 
  unsigned char *tvnnm;
  unsigned short tvnnml; 
  unsigned char *tvnsnm;
  unsigned short tvnsnml;
};
typedef struct sqltvn sqltvn;

struct sqladts
{
  unsigned int adtvsn; 
  unsigned short adtmode; 
  unsigned short adtnum;  
  sqltvn adttvn[1];       
};
typedef struct sqladts sqladts;

static struct sqladts sqladt = {
  1,1,0,
};

/* Binding to PL/SQL Records */
struct sqltdss
{
  unsigned int tdsvsn; 
  unsigned short tdsnum; 
  unsigned char *tdsval[1]; 
};
typedef struct sqltdss sqltdss;
static struct sqltdss sqltds =
{
  1,
  0,
};

/* File name & Package Name */
struct sqlcxp
{
  unsigned short fillen;
           char  filnam[12];
};
static const struct sqlcxp sqlfpn =
{
    11,
    "RateRule.pc"
};


static unsigned long sqlctx = 157731;


static struct sqlexd {
   unsigned int   sqlvsn;
   unsigned int   arrsiz;
   unsigned int   iters;
   unsigned int   offset;
   unsigned short selerr;
   unsigned short sqlety;
   unsigned int   unused;
      const short *cud;
   unsigned char  *sqlest;
      const char  *stmt;
   sqladts *sqladtp;
   sqltdss *sqltdsp;
            void  **sqphsv;
   unsigned int   *sqphsl;
            int   *sqphss;
            void  **sqpind;
            int   *sqpins;
   unsigned int   *sqparm;
   unsigned int   **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
            void  *sqhstv[7];
   unsigned int   sqhstl[7];
            int   sqhsts[7];
            void  *sqindv[7];
            int   sqinds[7];
   unsigned int   sqharm[7];
   unsigned int   *sqharc[7];
   unsigned short  sqadto[7];
   unsigned short  sqtdso[7];
} sqlstm = {10,7};

// Prototypes
extern "C" {
  void sqlcxt (void **, unsigned long *,
               struct sqlexd *, const struct sqlcxp *);
  void sqlcx2t(void **, unsigned long *,
               struct sqlexd *, const struct sqlcxp *);
  void sqlbuft(void **, char *);
  void sqlgs2t(void **, char *);
  void sqlorat(void **, unsigned long *, void *);
}

// Forms Interface
static const int IAPSUCC = 0;
static const int IAPFAIL = 1403;
static const int IAPFTL  = 535;
extern "C" { void sqliem(char *, int *); }

 static const char *sq0001 = 
"select rate_type ,today_type ,start_time ,end_time ,time_rate ,packet_rate ,\
packet_size  from t_rate_rule            ";

 static const char *sq0002 = 
"select holiday_type  from t_holiday            ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{10,4130,0,0,0,
5,0,0,1,117,0,9,65,0,0,0,0,0,1,0,
20,0,0,1,0,0,15,80,0,0,0,0,0,1,0,
35,0,0,2,47,0,9,82,0,0,0,0,0,1,0,
50,0,0,2,0,0,15,92,0,0,0,0,0,1,0,
65,0,0,3,0,0,27,145,0,0,4,4,0,1,0,1,9,0,0,1,9,0,0,1,10,0,0,1,10,0,0,
96,0,0,4,0,0,30,152,0,0,0,0,0,1,0,
111,0,0,1,0,0,13,161,0,0,7,0,0,1,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,
0,0,2,3,0,0,
154,0,0,2,0,0,13,170,0,0,1,0,0,1,0,2,97,0,0,
};


/****************************************************************************/
/* 파 일 명 : RateRule.cpp													*/
/* 작 성 자 : KSI / 조상영								                    */
/* 기    능 : 평일,휴일, 시간별 10초당 이용요금 산출						*/
/* 작성일자 : 2000년 10월 30일                                              */
/* 변경일자 :     년   월   일                                              */
/* 관련화일 : rate_tbl.cfg, holiday.cfg 									*/
/* 기    타 :                                                               */
/****************************************************************************/

#include <MDB/RateRule.hpp>
//#include <SendLogManage.hpp>

#define SQLCA_STORAGE_CLASS extern
/* EXEC    SQL     INCLUDE SQLCA;
 */ 
/*
 * $Header: sqlca.h,v 1.3 1994/12/12 19:27:27 jbasu Exp $ sqlca.h 
 */

/* Copyright (c) 1985,1986, 1998 by Oracle Corporation. */
 
/*
NAME
  SQLCA : SQL Communications Area.
FUNCTION
  Contains no code. Oracle fills in the SQLCA with status info
  during the execution of a SQL stmt.
NOTES
  **************************************************************
  ***                                                        ***
  *** This file is SOSD.  Porters must change the data types ***
  *** appropriately on their platform.  See notes/pcport.doc ***
  *** for more information.                                  ***
  ***                                                        ***
  **************************************************************

  If the symbol SQLCA_STORAGE_CLASS is defined, then the SQLCA
  will be defined to have this storage class. For example:
 
    #define SQLCA_STORAGE_CLASS extern
 
  will define the SQLCA as an extern.
 
  If the symbol SQLCA_INIT is defined, then the SQLCA will be
  statically initialized. Although this is not necessary in order
  to use the SQLCA, it is a good pgming practice not to have
  unitialized variables. However, some C compilers/OS's don't
  allow automatic variables to be init'd in this manner. Therefore,
  if you are INCLUDE'ing the SQLCA in a place where it would be
  an automatic AND your C compiler/OS doesn't allow this style
  of initialization, then SQLCA_INIT should be left undefined --
  all others can define SQLCA_INIT if they wish.

  If the symbol SQLCA_NONE is defined, then the SQLCA variable will
  not be defined at all.  The symbol SQLCA_NONE should not be defined
  in source modules that have embedded SQL.  However, source modules
  that have no embedded SQL, but need to manipulate a sqlca struct
  passed in as a parameter, can set the SQLCA_NONE symbol to avoid
  creation of an extraneous sqlca variable.
 
MODIFIED
    lvbcheng   10/28/98 -  Undo long to int
    lvbcheng   08/03/98 -  Change sqlca long attrs to ints
    jbasu      12/12/94 -  Bug 217878: note this is an SOSD file
    losborne   08/11/92 -  No sqlca var if SQLCA_NONE macro set 
  Clare      12/06/84 - Ch SQLCA to not be an extern.
  Clare      10/21/85 - Add initialization.
  Bradbury   01/05/86 - Only initialize when SQLCA_INIT set
  Clare      06/12/86 - Add SQLCA_STORAGE_CLASS option.
*/
 
#ifndef SQLCA
#define SQLCA 1
 
struct   sqlca
         {
         /* ub1 */ char    sqlcaid[8];
         /* b4  */ long    sqlabc;
         /* b4  */ long    sqlcode;
         struct
           {
           /* ub2 */ unsigned short sqlerrml;
           /* ub1 */ char           sqlerrmc[70];
           } sqlerrm;
         /* ub1 */ char    sqlerrp[8];
         /* b4  */ long    sqlerrd[6];
         /* ub1 */ char    sqlwarn[8];
         /* ub1 */ char    sqlext[8];
         };

#ifndef SQLCA_NONE 
#ifdef   SQLCA_STORAGE_CLASS
SQLCA_STORAGE_CLASS struct sqlca sqlca
#else
         struct sqlca sqlca
#endif
 
#ifdef  SQLCA_INIT
         = {
         {'S', 'Q', 'L', 'C', 'A', ' ', ' ', ' '},
         sizeof(struct sqlca),
         0,
         { 0, {0}},
         {'N', 'O', 'T', ' ', 'S', 'E', 'T', ' '},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0}
         }
#endif
         ;
#endif
 
#endif
 
/* end SQLCA */


/* EXEC SQL BEGIN DECLARE SECTION; */ 

/* VARCHAR	rateusername[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } rateusername;

/* VARCHAR	ratepasswd[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } ratepasswd;

int		nRateType;
int		nTodayType;
int		nStartTime;
int		nEndTime;
int		nTimeRate;
int		nPacketRate;
int		nPacketSize;
char	sHoliDay[8+1];
/* EXEC SQL END DECLARE SECTION; */ 



// Cusor declare 
/* EXEC SQL declare rate_cur cursor for
		select  rate_type, today_type, start_time, end_time, time_rate, packet_rate, packet_size 
		from    t_rate_rule; */ 

		
/* EXEC SQL declare holiday_cur cursor for
		select  holiday_type 
		from    t_holiday; */ 

				
//	Log Manager
extern SendLogManage*				AggregatorLogM;


//--------------------------------------------------------------------
RateRule::RateRule(void)
{
	int			nSqlcode;
	time_t		today;
	FILE		*fp;
	
	time(&today);
	Tm = localtime(&today);	// TM 을 오늘날짜로 초기화
	
	LimitSec=chk_H=Rn=Hn=0;
	memset(strDay, 0x00, sizeof(strDay));
	
	DBerror=ConnectToDB();	// 오라클연결
	if(DBerror!=0) 
	{
		printf("DB Connect Error : [%d]", DBerror);
		return;	// error 발생
	}
	
	//--------------- Rate 테이블 읽기
	/* EXEC SQL OPEN rate_cur; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 0;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0001;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


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
	/* EXEC SQL CLOSE rate_cur; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 0;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )20;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	//--------------- Holiday 테이블 읽기
	/* EXEC SQL OPEN holiday_cur; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 0;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0002;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )35;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	for (;;) 
	{
		nSqlcode = FetchHolidayDB();
		if( nSqlcode != 0 || nSqlcode == 1403)	break;
		
		memset(tblHoliday[Hn].Date, 0x00, 8+1);
		strncpy(tblHoliday[Hn].Date, sHoliDay, 8);
		Hn++;
	}
	/* EXEC SQL CLOSE holiday_cur; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 0;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )50;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


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
	
	*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "[Rating Rule] Rn= " << Rn << endl;
	*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "[Rating Rule] Hn= " << Hn << endl;
}

//------------------------------------------------------------------------
int RateRule::ConnectToDB( void )
{
	strcpy((char *) rateusername.arr, DB_USERNAME);
	rateusername.len = (unsigned short) strlen(DB_USERNAME);

	strcpy((char *) ratepasswd.arr, DB_PASSWORD);
	ratepasswd.len = (unsigned short) strlen(DB_PASSWORD);
	
	/* EXEC SQL connect :rateusername identified by :ratepasswd; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )10;
 sqlstm.offset = (unsigned int  )65;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlstm.sqhstv[0] = (         void  *)&rateusername;
 sqlstm.sqhstl[0] = (unsigned int  )34;
 sqlstm.sqhsts[0] = (         int  )34;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&ratepasswd;
 sqlstm.sqhstl[1] = (unsigned int  )34;
 sqlstm.sqhsts[1] = (         int  )34;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	return(sqlca.sqlcode);
}

//------------------------------------------------------------------------
int RateRule::Db_close()
{
	/* EXEC	SQL	COMMIT	WORK	RELEASE; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )96;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



	return(sqlca.sqlcode); 
}


//------------------------------------------------------------------------
int RateRule::FetchRateDB(void)
{
	/* EXEC SQL	fetch	rate_cur 
				into 	:nRateType, nTodayType, :nStartTime, :nEndTime, :nTimeRate, :nPacketRate, :nPacketSize; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 7;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )111;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlstm.sqhstv[0] = (         void  *)&nRateType;
 sqlstm.sqhstl[0] = (unsigned int  )4;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&nTodayType;
 sqlstm.sqhstl[1] = (unsigned int  )4;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&nStartTime;
 sqlstm.sqhstl[2] = (unsigned int  )4;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)&nEndTime;
 sqlstm.sqhstl[3] = (unsigned int  )4;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&nTimeRate;
 sqlstm.sqhstl[4] = (unsigned int  )4;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)&nPacketRate;
 sqlstm.sqhstl[5] = (unsigned int  )4;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)&nPacketSize;
 sqlstm.sqhstl[6] = (unsigned int  )4;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         void  *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned int  )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	printf("fetch_rate sqlcode:[%d]\n",sqlca.sqlcode);
	return(sqlca.sqlcode);
}	

//------------------------------------------------------------------------
int RateRule::FetchHolidayDB(void)
{
	/* EXEC SQL	fetch	holiday_cur 
				into 	:sHoliDay; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 7;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )154;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlstm.sqhstv[0] = (         void  *)sHoliDay;
 sqlstm.sqhstl[0] = (unsigned int  )9;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


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
	/*
	*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "[Rating Rule] j= " << j << endl;
	*/
	*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "[Rating Rule] Rn= " << Rn << endl;
	/*
	*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "[Rating Rule] chk_H= " << chk_H << endl;
	*/
	
	// 임시로 테이블수를 넣어주었습니다.
	// 테이블수를 수정해야 합니다.
	if( Rn == 0 ) Rn = 8;
	for( i= 0; i < Rn; i++ )
		if(tblRate[i].Today==chk_H){
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "[Rating Rule] tblRate[i].Stime= " << tblRate[i].Stime << endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "[Rating Rule] tblRate[i].Ltime= " << tblRate[i].Ltime << endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "[Rating Rule] tblRate[i].Rate= " << tblRate[i].Rate << endl;
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
int	RateRule::GetTelRate(int amount, int select,  int last)	// 전화요금을 계산한다(시간단위)
{
	*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "[Rating Rule] amount= " << amount << endl;
	if(amount<=0) return 0;
	
	int i, n = (amount * 0.1);
	int rate;
	
	rate=GetRate(select);
	*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "[Rating Rule] rate= " << rate << endl;
	if(last)
	{
		if(amount%10) n++;
		// i=(amount%10) * ((double)rate*0.1);
		// printf("n=%d rate=%d amount=%d  [%d]\n", n, rate, amount, i);
		n = (n*rate);
	}else n *= rate;
	
	*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "[Rating Rule] n= " << n << endl;
	return n;
}

	

//--------------------------------------------------------------------
// 시간갭은 1분30초를 넘을수없다. (2개를 초과한 요금율이 적용되지 않는다)
int RateRule::GetRate(int rtype, time_t stime, time_t etime, int last, int packet)
{
	if(DBerror!=0) return -1;
	
	int				n;
	int				ret;
	unsigned long	difft;
	struct  tm 		tm1;	
	struct  tm 		tm2;	
	time_t 			tmImsi;
	char			imsi[20];
	
	if(rtype==2) OnPacket = 1;	// 패킷과금여부지정
	
	STime = stime;
	ETime = etime;
	
	memcpy((void *)&tm1, localtime(&stime), sizeof(struct tm));	
	memcpy((void *)&tm2, localtime(&etime), sizeof(struct tm));			
	
	/*
	sprintf(imsi, "%04d%02d%02d %02d:%02d:%02d", tm1.tm_year+1900, tm1.tm_mon+1, tm1.tm_mday,
				tm1.tm_hour, tm1.tm_min, tm1.tm_sec);
	*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "[Rating Rule] STime= " << imsi << endl;
							
	sprintf(imsi, "%04d%02d%02d %02d:%02d:%02d", tm2.tm_year+1900, tm2.tm_mon+1, tm2.tm_mday,
				tm2.tm_hour, tm2.tm_min, tm2.tm_sec);
	*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "[Rating Rule] ETime= " << imsi << endl;				
	*/
	LimitSec=0;
	if(rtype==1) //시간과금이면
	{
		if(tm1.tm_hour==tm2.tm_hour)
		{	//시간이 같으면 동일한 요금적용
			if(last) return GetTelRate(difftime(etime, stime),1, last);
			else
			{
				difft=difftime(etime, stime);
				LimitSec = difft%10;
				if(difft<10) return 0;
				else return GetTelRate(difftime(etime, stime),1, last);
			}
			
		}else
		{	//시간이 다르면 
			n=GetRate(1);
			if(n==GetRate(2)) // 시간은 다르나 동일한 Rate가 적용되면 나누지않고 계산 2000.11.23
			{
				if(last) return GetTelRate(difftime(etime, stime),1, last);
				else
				{
					difft=difftime(etime, stime);
					LimitSec = difft%10;
					if(difft<10) return 0;
					else return GetTelRate(difftime(etime, stime),1, last);
				}  
			}
			
			
			tmImsi = stime + ( 59 - tm1.tm_min )*60 + (60-tm1.tm_sec);
		//	tmImsi = stime + (60 - tm1.tm_sec);	// 바로다음시간 00분00초로 만든다.
			
			ret = GetTelRate(difftime(tmImsi, stime),1, last);
			if(last) 
			{
				ret += GetTelRate(difftime(etime, tmImsi),2, last);
			}else
			{
				difft=difftime(etime, tmImsi);
				LimitSec = difft%10;
				if(difft>=10) ret += GetTelRate(difftime(etime, tmImsi),2, last);
			}
			return ret;
		}
	}else
	{	// 패킷과금이면..
		
		ret = GetRate(1);
		if(!PSize) PSize=512;
		
		//printf("rate: %d / pszie: %d / [%d]\n", ret, PSize, (packet / PSize));
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "[Rating Rule] ret= " << ret << endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "[Rating Rule] PSize= " << PSize << endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "[Rating Rule] packet= " << packet << endl;
			
		if(last) 
		{
			n = (int)(packet/PSize);
			if(packet % PSize) n++;
			ret *= n;
		}else 
		{
			ret *= 	(int)(packet / PSize);
			LimitSec = packet % PSize;
		}
		
		return (ret);
	}
}

