
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
5,0,0,1,117,0,9,154,0,0,0,0,0,1,0,
20,0,0,1,0,0,15,169,0,0,0,0,0,1,0,
35,0,0,2,47,0,9,173,0,0,0,0,0,1,0,
50,0,0,2,0,0,15,183,0,0,0,0,0,1,0,
65,0,0,3,0,0,27,232,0,0,4,4,0,1,0,1,9,0,0,1,9,0,0,1,10,0,0,1,10,0,0,
96,0,0,4,0,0,30,239,0,0,0,0,0,1,0,
111,0,0,1,0,0,13,248,0,0,7,0,0,1,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,
0,0,2,3,0,0,
154,0,0,2,0,0,13,257,0,0,1,0,0,1,0,2,97,0,0,
};


/****************************************************************************/
/* �� �� �� : RateRule.cpp													*/
/* �� �� �� : KSI / ����								                    */
/* ��    �� : ����,����, �ð��� 10�ʴ� �̿��� ����						*/
/* �ۼ����� : 2000�� 10�� 30��                                              */
/* �������� :     ��   ��   ��                                              */
/* ����ȭ�� : rate_tbl.cfg, holiday.cfg 									*/
/* ��    Ÿ :                                                               */
/****************************************************************************/
#ifndef _RATERULE_HPP
#define _RATERULE_HPP

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define  DB_USERNAME  "uda"
#define  DB_PASSWORD  "uda"

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
/* EXEC    SQL     INCLUDE SQLDA;
 */ 
/*
 * $Header: sqlda.h 08-apr-99.16:01:28 sylin Exp $ sqlda.h 
 */

/***************************************************************
*      The SQLDA descriptor definition                         *
*--------------------------------------------------------------*
*      VAX/3B Version                                          *
*                                                              *
*  Copyright (c) 1987, 1997, 1999 by Oracle Corporation                    *
***************************************************************/


/* NOTES
  **************************************************************
  ***                                                        ***
  *** This file is SOSD.  Porters must change the data types ***
  *** appropriately on their platform.  See notes/pcport.doc ***
  *** for more information.                                  ***
  ***                                                        ***
  **************************************************************
*/

/*  MODIFIED
    sylin      04/08/99 -  (860777) Change long to int
    apopat     03/16/99 -  [707588] TAB to blank for OCCS
    lvbcheng   08/15/97 -  Move sqlda protos to sqlcpr.h
    lvbcheng   06/25/97 -  Move sqlda protos to this file
    jbasu      01/29/95 -  correct typo
    jbasu      01/27/95 -  correct comment - ub2->sb2
    jbasu      12/12/94 - Bug 217878: note this is an SOSD file
    Morse      12/01/87 - undef L and S for v6 include files
    Richey     07/13/87 - change int defs to long 
    Clare      09/13/84 - Port: Ch types to match SQLLIB structs
    Clare      10/02/86 - Add ifndef SQLDA
*/

#ifndef SQLDA_
#define SQLDA_ 1
 
#ifdef T
# undef T
#endif
#ifdef F
# undef F
#endif

#ifdef S
# undef S
#endif
#ifdef L
# undef L
#endif
 
struct SQLDA {
  /* ub4    */ int       N; /* Descriptor size in number of entries        */
  /* text** */ char     **V; /* Ptr to Arr of addresses of main variables   */
  /* ub4*   */ int      *L; /* Ptr to Arr of lengths of buffers            */
  /* sb2*   */ short     *T; /* Ptr to Arr of types of buffers              */
  /* sb2**  */ short    **I; /* Ptr to Arr of addresses of indicator vars   */
  /* sb4    */ int       F; /* Number of variables found by DESCRIBE       */
  /* text** */ char     **S; /* Ptr to Arr of variable name pointers        */
  /* ub2*   */ short     *M; /* Ptr to Arr of max lengths of var. names     */
  /* ub2*   */ short     *C; /* Ptr to Arr of current lengths of var. names */
  /* text** */ char     **X; /* Ptr to Arr of ind. var. name pointers       */
  /* ub2*   */ short     *Y; /* Ptr to Arr of max lengths of ind. var. names */
  /* ub2*   */ short     *Z; /* Ptr to Arr of cur lengths of ind. var. names */
  };
 
typedef struct SQLDA SQLDA;
 
#endif

/* ----------------- */
/* defines for sqlda */
/* ----------------- */

#define SQLSQLDAAlloc(arg1, arg2, arg3, arg4) sqlaldt(arg1, arg2, arg3, arg4) 

#define SQLSQLDAFree(arg1, arg2) sqlclut(arg1, arg2) 



/* EXEC    SQL     INCLUDE ORACA;
 */ 
/*
 * $Header: oraca.h 08-apr-99.16:01:35 sylin Exp $ oraca.h 
 */

/* Copyright (c) 1985, 1996, 1999 by Oracle Corporation. */
 
/*
NAME
  ORACA : Oracle Communications Area.
FUNCTION
  Contains no code. Provides supplementary communications to/from
  Oracle (in addition to standard SQLCA).
NOTES
  **************************************************************
  ***                                                        ***
  *** This file is SOSD.  Porters must change the data types ***
  *** appropriately on their platform.  See notes/pcport.doc ***
  *** for more information.                                  ***
  ***                                                        ***
  **************************************************************

  oracchf : Check cursor cache consistency flag. If set AND oradbgf
            is set, then directs SQLLIB to perform cursor cache
            consistency checks before every cursor operation
            (OPEN, FETCH, SELECT, INSERT, etc.).
  oradbgf : Master DEBUG flag. Used to turn all DEBUG options
            on or off.
  orahchf : Check Heap consistency flag. If set AND oradbgf is set,
            then directs SQLLIB to perform heap consistency checks
            everytime memory is dynamically allocated/free'd via
            sqlalc/sqlfre/sqlrlc. MUST BE SET BEFORE 1ST CONNECT
            and once set cannot be cleared (subsequent requests
            to change it are ignored).
  orastxtf: Save SQL stmt text flag. If set, then directs SQLLIB
            to save the text of the current SQL stmt in orastxt
            (in VARCHAR format).
  orastxt : Saved len and text of current SQL stmt (in VARCHAR
            format).
  orasfnm : Saved len and text of filename containing current SQL
            stmt (in VARCHAR format).
  oraslnr : Saved line nr within orasfnm of current SQL stmt.
 
  Cursor cache statistics. Set after COMMIT or ROLLBACK. Each
  CONNECT'd DATABASE has its own set of statistics.
 
  orahoc  : Highest Max Open OraCursors requested. Highest value
            for MAXOPENCURSORS by any CONNECT to this DATABASE.
  oramoc  : Max Open OraCursors required. Specifies the max nr
            of OraCursors required to run this pgm. Can be higher
            than orahoc if working set (MAXOPENCURSORS) was set
            too low, thus forcing the PCC to expand the cache.
  oracoc  : Current nr of OraCursors used.
  oranor  : Nr of OraCursor cache reassignments. Can show the
            degree of "thrashing" in the cache. Optimally, this
            nr should be kept as low as possible (time vs space
            optimization).
  oranpr  : Nr of SQL stmt "parses".
  oranex  : Nr of SQL stmt "executes". Optimally, the relation-
            ship of oranex to oranpr should be kept as high as
            possible.
 
 
  If the symbol ORACA_NONE is defined, then there will be no ORACA
  *variable*, although there will still be a struct defined.  This
  macro should not normally be defined in application code.

  If the symbol ORACA_INIT is defined, then the ORACA will be
  statically initialized. Although this is not necessary in order
  to use the ORACA, it is a good pgming practice not to have
  unitialized variables. However, some C compilers/OS's don't
  allow automatic variables to be init'd in this manner. Therefore,
  if you are INCLUDE'ing the ORACA in a place where it would be
  an automatic AND your C compiler/OS doesn't allow this style
  of initialization, then ORACA_INIT should be left undefined --
  all others can define ORACA_INIT if they wish.
 
OWNER
  Clare
DATE
  10/19/85
MODIFIED
    sylin      04/08/99 -  (860777) Change long to int
    apopat     03/16/99 -  [707588] TAB to blank for OCCS
    pccint     10/03/96 -  Add IS_OSD for linting
    jbasu      12/12/94 -  Bug 217878: note this is an SOSD file
    losborne   09/04/92 -  Make oraca variable optional 
    Osborne    05/24/90 - Add ORACA_STORAGE_CLASS construct
  Clare      02/20/86 - PCC [10101l] Feature: Heap consistency check.
  Clare      03/04/86 - PCC [10101r] Port: ORACA init ifdef.
  Clare      03/12/86 - PCC [10101ab] Feature: ORACA cuc statistics.
*/
/* IS_OSD */ 
#ifndef  ORACA
#define  ORACA     1
 
struct   oraca
         {
    /* text */ char oracaid[8];        /* Reserved                            */
    /* ub4  */ int oracabc;            /* Reserved                            */
 
/*       Flags which are setable by User. */
 
   /* ub4 */ int  oracchf;             /* <> 0 if "check cur cache consistncy"*/
   /* ub4 */ int  oradbgf;             /* <> 0 if "do DEBUG mode checking"    */
   /* ub4 */ int  orahchf;             /* <> 0 if "do Heap consistency check" */
   /* ub4 */ int  orastxtf;            /* SQL stmt text flag                  */
#define  ORASTFNON 0                   /* = don't save text of SQL stmt       */
#define  ORASTFERR 1                   /* = only save on SQLERROR             */
#define  ORASTFWRN 2                   /* = only save on SQLWARNING/SQLERROR  */
#define  ORASTFANY 3                   /* = always save                       */
         struct
           {
  /* ub2  */ unsigned short orastxtl;
  /* text */ char  orastxtc[70];
           } orastxt;                  /* text of last SQL stmt               */
         struct
           {
  /* ub2  */   unsigned short orasfnml;
  /* text */   char       orasfnmc[70];
           } orasfnm;                  /* name of file containing SQL stmt    */
  /* ub4 */ int   oraslnr;             /* line nr-within-file of SQL stmt     */
 
  /* ub4 */ int   orahoc;              /* highest max open OraCurs requested  */
  /* ub4 */ int   oramoc;              /* max open OraCursors required        */
  /* ub4 */ int   oracoc;              /* current OraCursors open             */
  /* ub4 */ int   oranor;              /* nr of OraCursor re-assignments      */
  /* ub4 */ int   oranpr;              /* nr of parses                        */
  /* ub4 */ int   oranex;              /* nr of executes                      */
         };

#ifndef ORACA_NONE

#ifdef ORACA_STORAGE_CLASS
ORACA_STORAGE_CLASS struct oraca oraca
#else
struct oraca oraca
#endif
#ifdef ORACA_INIT
         =
         {
         {'O','R','A','C','A',' ',' ',' '},
         sizeof(struct oraca),
         0,0,0,0,
         {0,{0}},
         {0,{0}},
         0,
         0,0,0,0,0,0
         }
#endif
         ;

#endif

#endif
/* end oraca.h */



/* EXEC SQL BEGIN DECLARE SECTION; */ 

/* VARCHAR	username[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } username;

/* VARCHAR	passwd[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } passwd;

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

				

/* ��Ŷ���� ��� */
/*
1. ��Ŷ���� = (��Ŷ�� / ��Ŷsize)
2. ��� = (�ش�ð���Ŷ����ȭ��) * (��Ŷ����)
*/


struct RATE_TABLE {
	int 	Rtype;	// ���� (�Ϲ�:1 ����:2 �ɾ�:3) => ����Ÿ�� TTL, N-TOP ���� ���İ������
	int		Today;	// ���� (����(��-��):1 ����:2)
	int		Stime;	// ���۽ð� = (HH*100)+mm
	int		Ltime;	// ���ð�  = (HH*100)+mm
	int		Rate;	// 10�ʴ� ��ȭ��
	int		PRate;	// ��Ŷ�� ��ȭ��
	int		PSize;	// ��ŶSize
};

/* �����ڵ�(�Ϲ�/����/�ɾ�, ���ϱ���(����,�����,������), ���۽ð�(HHmm), ���ð�, 10�ʴ���ȭ��, ��Ŷ����ȭ��, ��ŶSize 
RateTable 	tblRate[6]=	{{1,1,  800, 2100, 17, 17, 512},
						 {2,1,  600,  800, 12, 12, 512},
						 {2,1, 2100, 2400, 12, 12, 512},
						 {2,2,  600, 2400, 12, 12, 512},
						 {3,2,    0,  600,  8,  8, 512},
						 {3,1,    0,  600,  8,  8, 512} };

*/

struct HOLIDAY_TABLE {
	char 	Date[10];	// YYYYMMDD
	char	Name[10];	// ���ϸ�
};

typedef RATE_TABLE		RateTable;
typedef HOLIDAY_TABLE	HolidayTable;

#define		FNAME_RATE_TABLE		"/SVC1/DATA/rate_tbl.cfg"	// �������� => DB�����ʿ�
#define		FNAME_HOLIDAY_TABLE		"/SVC1/DATA/holiday.cfg"
#define		RATE_MAX	30			// ������̺� �ִ밳��
#define		HOLIDAY_MAX	50			// ���������� �ִ밳��

//--------------------------------------------------------------------
class RateRule {
	
	private:
		int				Rn;		// ������̺� ����
		int				Hn;		// ������ ����
		int				chk_H;	// 1.���� 2.����� 3.����
		int				OnPacket;	// ��Ŷ���ݿ���
		int				PSize;
		int				LimitSec;
		char			strDay[14];
		int				DBerror;
		
		
		time_t			STime;
		time_t			ETime;
		struct  tm 		*Tm;	
		RateTable		tblRate[RATE_MAX];		// ������̺�
		HolidayTable	tblHoliday[HOLIDAY_MAX];// ����������
		
		int				CHK_Holyday(void);		// ������üũ
		int				GetRate(void);			// ������̺��� �˻��� 10�ʴ��� ����
						// ��ȭ����� ����Ѵ�(�ð�����) amount : �ð��� select : ������ 1(����) 2(��)
		int				GetTelRate(int amount, int select, int last);	
		int				GetRate(int select);	// select : ������ 1(����) 2(��)
		
		int				ConnectToDB(void);
		int				Db_close(void);
		
		int 			FetchRateDB(void);
		int				FetchHolidayDB(void);
	
	public:
		RateRule(void);
		~RateRule(void){;}
		
		//�ð����� 1��30�ʸ� �������� ����.
		// rtype : 1(�ð�) 2(��Ŷ) / stime: ���۽ð�  / etime: ���ð�  /last: ��������꿩�� / parket : ��Ŷ��
		int GetRate(int rtype, time_t stime, time_t etime, int last, int packet=0); 
		int	GetLimitSec(void)	{ return LimitSec; } // ����ϰ� ���� �ð� & ��Ŷ
};


//--------------------------------------------------------------------
RateRule::RateRule(void)
{
	int			nSqlcode;
	time_t		today;
	FILE		*fp;
	
	time(&today);
	Tm = localtime(&today);	// TM �� ���ó�¥�� �ʱ�ȭ
	
	LimitSec=chk_H=Rn=Hn=0;
	memset(strDay, 0x00, sizeof(strDay));
	
	DBerror=ConnectToDB();	// ����Ŭ����
	if(DBerror!=0) 
	{
		printf("DB Connect Error : [%d]", DBerror);
		return;	// error �߻�
	}
	
	//--------------- Rate ���̺� �б�
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


	
	
	//--------------- Holiday ���̺� �б�
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
}

//------------------------------------------------------------------------
int RateRule::ConnectToDB( void )
{
	strcpy((char *) username.arr, DB_USERNAME);
	username.len = (unsigned short) strlen(DB_USERNAME);

	strcpy((char *) passwd.arr, DB_PASSWORD);
	passwd.len = (unsigned short) strlen(DB_PASSWORD);
	
	/* EXEC SQL connect :username identified by :passwd; */ 

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
 sqlstm.sqhstv[0] = (         void  *)&username;
 sqlstm.sqhstl[0] = (unsigned int  )34;
 sqlstm.sqhsts[0] = (         int  )34;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&passwd;
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
// select : ����ð�����
int	RateRule::GetRate(int select)
{
			
	if(select==1) Tm = localtime(&STime);		//���Ͼ��
	else Tm = localtime(&ETime);				
	
	sprintf(strDay, "%04d%02d%02d", Tm->tm_year+1900, Tm->tm_mon+1, Tm->tm_mday);
	//printf("%04d%02d%02d %2d:%2d [%d]\n", Tm->tm_year+1900, Tm->tm_mon+1, Tm->tm_mday, Tm->tm_hour, Tm->tm_min, Tm->tm_wday);
	
	if(!Tm->tm_wday || CHK_Holyday()) chk_H=3;	//������
	else if(!Tm->tm_wday==6) chk_H=2;	//�����
	else chk_H = 1;		//����
	
	return GetRate();
}



//--------------------------------------------------------------------
int	RateRule::GetTelRate(int amount, int select, int last)	// ��ȭ����� ����Ѵ�(�ð�����)
{
	printf("amount: %d\n", amount);//
	if(amount<=0) return 0;
	
	int i, n = (amount * 0.1);
	int rate;
	
	
	rate=GetRate(select);
	printf("rate: %d\n", rate);//
	
	if(last)
	{
		if(amount%10) n++;
		// i=(amount%10) * ((double)rate*0.1);
		// printf("n=%d rate=%d amount=%d  [%d]\n", n, rate, amount, i);
		n = (n*rate);
	}else n *= rate;
	
	printf("money: %d\n", n);//
	return n;
}

	

	
//--------------------------------------------------------------------
// �ð����� 1��30�ʸ� ����������. (2���� �ʰ��� ������� ������� �ʴ´�)
int RateRule::GetRate(int rtype, time_t stime, time_t etime, int last, int packet)
{
	if(DBerror!=0) return -1;
	
	int				n;
	int				ret;
	unsigned long	difft;
	struct  tm 		tm1;	
	struct  tm 		tm2;	
	time_t 			tmImsi;
	
	if(rtype==2) OnPacket = 1;	// ��Ŷ���ݿ�������
	
	STime = stime;
	ETime = etime;
	
	memcpy((void *)&tm1, localtime(&stime), sizeof(struct tm));	
	memcpy((void *)&tm2, localtime(&etime), sizeof(struct tm));			
	
	LimitSec=0;
	if(rtype==1) //�ð������̸�
	{
		if(tm1.tm_hour==tm2.tm_hour)
		{	//�ð��� ������ ������ �������
			if(last) return GetTelRate(difftime(etime, stime),1, last);
			else
			{
				difft=difftime(etime, stime);
				LimitSec = difft%10;
				if(difft<10) return 0;
				else return GetTelRate(difftime(etime, stime),1, last);
			}
			
		}else
		{	//�ð��� �ٸ��� 
			n=GetRate(1);
			if(n==GetRate(2)) // �ð��� �ٸ��� ������ Rate�� ����Ǹ� �������ʰ� ��� 2000.11.23
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
			
			tmImsi = stime + (59 - tm1.tm_min)*60 + (60-tm1.tm_sec);	// �ٷδ����ð� 00��00�ʷ� �����.
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
	{	// ��Ŷ�����̸�..
		
		ret = GetRate(1);
		if(!PSize) PSize=512;
		
		//printf("rate: %d / pszie: %d / [%d]\n", ret, PSize, (packet / PSize));

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



//--------------------------------------------------------------------
//��뿹
int main(void)
{
	struct  tm 	*Tm;
	time_t		today;
	time_t		onet, twot;
	
	time(&today);	// ���ó��� ���
	Tm = localtime(&today);
	
	Tm->tm_mday=13;
	Tm->tm_hour=23;
	Tm->tm_min=59;
	Tm->tm_sec=55;
	
	onet = mktime(Tm);
	
	Tm->tm_mday=14;
	Tm->tm_hour=00;
	Tm->tm_min=01;
	//Tm->tm_hour=7;
	//Tm->tm_min=59;
	Tm->tm_sec=15;
	
	twot = mktime(Tm);
	
	RateRule rate;
	
	
	printf("[[ %d ]]\n", rate.GetRate(1, onet, twot, 1));
	//printf("[[ %d ]]\n", rate.GetRate(2, onet, twot, 0, 10000));
	//printf("[[ %d ]]\n", rate.GetLimitSec());
	
	//printf("[[ %d ]]\n", rate.GetRate(2, onet, twot, 10240));
	/*
	printf("%d\n",	rate.GetRate("200011272350")); //12
	printf("%d\n",	rate.GetRate("200011281030")); //17
	printf("%d\n",	rate.GetRate("200012250520")); //8
	printf("%d\n",	rate.GetRate("200012241110")); //12
	*/
}




#endif