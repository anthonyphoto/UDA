
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
           char  filnam[11];
};
static const struct sqlcxp sqlfpn =
{
    10,
    "SysInfo.pc"
};


static unsigned long sqlctx = 85235;


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
            void  *sqhstv[9];
   unsigned int   sqhstl[9];
            int   sqhsts[9];
            void  *sqindv[9];
            int   sqinds[9];
   unsigned int   sqharm[9];
   unsigned int   *sqharc[9];
   unsigned short  sqadto[9];
   unsigned short  sqtdso[9];
} sqlstm = {10,9};

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

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{10,4130,0,0,0,
5,0,0,1,0,0,27,118,0,0,4,4,0,1,0,1,9,0,0,1,9,0,0,1,10,0,0,1,10,0,0,
36,0,0,2,165,0,3,165,0,0,9,9,0,1,0,1,97,0,0,1,3,0,0,1,97,0,0,1,3,0,0,1,3,0,0,1,
3,0,0,1,3,0,0,1,97,0,0,1,97,0,0,
87,0,0,3,0,0,29,170,0,0,0,0,0,1,0,
102,0,0,4,0,0,31,176,0,0,0,0,0,1,0,
117,0,0,5,25,0,2,187,0,0,0,0,0,1,0,
132,0,0,6,126,0,3,188,0,0,7,7,0,1,0,1,97,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,
3,0,0,1,97,0,0,
175,0,0,7,0,0,29,193,0,0,0,0,0,1,0,
190,0,0,8,0,0,31,199,0,0,0,0,0,1,0,
205,0,0,9,30,0,2,217,0,0,0,0,0,1,0,
};


/****************************************************************************/
/* 파 일 명 : SysInfo.pc													*/
/* 작 성 자 : KSI / 조상영								                    */
/* 기    능 : 시스템 디스크및 자원정보를 DB에 추가한다.						*/
/* 작성일자 : 2000년 11월  1일                                              */
/* 변경일자 :     년   월   일                                              */
/* 관련화일 :  																*/
/* 기    타 :                                                               */
/****************************************************************************/
#include <iostream.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>


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


#define  DB_USERNAME  "uda"
#define  DB_PASSWORD  "uda"


// Table변수에 접근하기 위한 변수.-----------
/* EXEC SQL BEGIN DECLARE SECTION; */ 

/* VARCHAR     username[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } username;

/* VARCHAR     passwd[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } passwd;


//------ 디스크 사용정보(bdf) 필드정의
char		DateTime[14+1];
int			SysID;	
char		VolumeName[40+1];
int			VolumeSize;
int			UsedSize;
int			AvailSize;
int			UsedRate;
char		Location[50+1];
char		Result[1+1];

//------ 자원 사용정보(sar) 필드정의
//char		DateTime[14+1];
//int		SysID;	
int			UserMode;
int			SystemMode;
int			WaitIO;
int			OtherIdle;
//char		Result;

/* EXEC SQL END DECLARE SECTION; */ 

// Table변수에 접근하기 위한 변수.-----------

/*
struct BDF {
	char 	Filesystem[40];
	int  	Kbytes;
	int	 	Used;
	int		Avail;
	int		used;
	char	Mounted[50];
};

struct SAR {
	int 	Usr;
	int  	Sys;
	int	 	Wio;
	int		Idle;
};

typedef BDF	BdfInfo;
typedef SAR	SarInfo;
*/

//--------------------------------------------------------------------
class SysInfo {
	private:
		int		DBConnect;
		char	Dhm[15];			//YYYYMMDDhhmmss
		
		int		InsertBdf(void);	//디스크정보를 DB에 추가한다.
		int		InsertSar(void);	//자원정보를 DB에 추가한다.
		
	public:
		SysInfo();
		~SysInfo(){;}
		void 	GetBdf(char *date, int sysid=1);	// 일시및 시스템ID를 받는다.
		void 	GetSar(char *date, int sysid=1);
		void 	GetBdf(int sysid=1)	{ GetBdf(Dhm, sysid);	}	// 현재 시스템일시 사용
		void 	GetSar(int sysid=1)	{ GetSar(Dhm, sysid);	}	
};


//--------------------------------------------------------------------
SysInfo::SysInfo()
{

	memset(DateTime, 0x00, sizeof(DateTime));
	SysID = 0;
	memset(VolumeName, 0x00, sizeof(VolumeName));
	VolumeSize=UsedSize=AvailSize=UsedRate=0;
	memset(Location, 0x00, sizeof(Location));
	memset(Result, 0x00, sizeof(Result));
	Result[0]='S';
	UserMode=SystemMode=WaitIO=OtherIdle=0;
	
	DBConnect=1;
	
	strcpy((char *)username.arr, DB_USERNAME);
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
 sqlstm.offset = (unsigned int  )5;
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


	if( sqlca.sqlcode != 0 )
	{
		cout << " sqlcaid: " << sqlca.sqlcaid << endl;
		printf (" (haxa)sql abc: %x\n",sqlca.sqlabc );
		cout << " sqlabc: " << sqlca.sqlabc << endl;
		printf (" (haxa)sql error code: %x\n",sqlca.sqlcode );
		cout << " sql error code: " << sqlca.sqlcode << endl;
		cout << " sqlerrml: " << sqlca.sqlerrm.sqlerrml << endl;
		cout << " sqlerrmc: " << (char *)&sqlca.sqlerrm.sqlerrmc-8 << endl;
		cout << " sqlerrp: " << sqlca.sqlerrp << endl;
		cout << " sqlerd: " << sqlca.sqlerrd << endl;
		cout << " sqlwarn: " << sqlca.sqlwarn << endl;
		cout << " sqlext: " << sqlca.sqlext << endl;
		//-------------------------------------------- 연결실패
		DBConnect=0;
		cout << "DB Connection Error!! : " << sqlca.sqlcode << endl;
	}

	cout << " sql error code: " << sqlca.sqlcode << endl;
	cout << "ConnectToDB sqlcode:" << sqlca.sqlcode << endl;
	
	//-------------------------
	struct  tm 	*Tm;
	time_t		today;
	
	time(&today);	// 오늘날자 얻기
	Tm = localtime(&today);
	sprintf(Dhm,"%04d%02d%02d%02d%02d%02d", Tm->tm_year+1900, Tm->tm_mon+1, Tm->tm_mday,  
				Tm->tm_hour, Tm->tm_min, Tm->tm_sec);
}



//--------------------------------------------------------------------
int SysInfo::InsertBdf(void)
{
	printf("%s\n", DateTime);
	printf("%d\n", SysID);
	printf("%s\n", VolumeName);
	printf("%d\n", VolumeSize);
	printf("%d\n", UsedSize);
	printf("%d\n", AvailSize);
	printf("%d\n", UsedRate);
	printf("%s\n", Location);
	printf("%s\n", Result);
	
	/* EXEC SQL INSERT INTO t_use_filesystem(date_time, sys_id, volume_name, volume_size, used_size, avail_size, used_rate, mounted_loc, result)
	values(:DateTime, :SysID, :VolumeName, :VolumeSize, :UsedSize, :AvailSize, :UsedRate, :Location, :Result); */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 9;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "insert into t_use_filesystem(date_time,sys_id,volume_name,vo\
lume_size,used_size,avail_size,used_rate,mounted_loc,result) values (:b0,:b1,:\
b2,:b3,:b4,:b5,:b6,:b7,:b8)";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )36;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlstm.sqhstv[0] = (         void  *)DateTime;
 sqlstm.sqhstl[0] = (unsigned int  )15;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&SysID;
 sqlstm.sqhstl[1] = (unsigned int  )4;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)VolumeName;
 sqlstm.sqhstl[2] = (unsigned int  )41;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)&VolumeSize;
 sqlstm.sqhstl[3] = (unsigned int  )4;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&UsedSize;
 sqlstm.sqhstl[4] = (unsigned int  )4;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)&AvailSize;
 sqlstm.sqhstl[5] = (unsigned int  )4;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)&UsedRate;
 sqlstm.sqhstl[6] = (unsigned int  )4;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         void  *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned int  )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (         void  *)Location;
 sqlstm.sqhstl[7] = (unsigned int  )51;
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         void  *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned int  )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
 sqlstm.sqhstv[8] = (         void  *)Result;
 sqlstm.sqhstl[8] = (unsigned int  )2;
 sqlstm.sqhsts[8] = (         int  )0;
 sqlstm.sqindv[8] = (         void  *)0;
 sqlstm.sqinds[8] = (         int  )0;
 sqlstm.sqharm[8] = (unsigned int  )0;
 sqlstm.sqadto[8] = (unsigned short )0;
 sqlstm.sqtdso[8] = (unsigned short )0;
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



	if (sqlca.sqlcode == 0) 
	{
		/* EXEC SQL COMMIT WORK; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 9;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )87;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		return 1;
	}
	else 
	{
		printf("InsertBdf Error: [%d]\n", sqlca.sqlcode);
		/* EXEC SQL ROLLBACK; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 9;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )102;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		return 0;
	}	
	
}


//--------------------------------------------------------------------
int SysInfo::InsertSar(void)
{
	
	/* EXEC SQL DELETE t_cpu_usage; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 9;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "delete  from t_cpu_usage ";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )117;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	/* EXEC SQL INSERT INTO t_cpu_usage(date_time, sys_id, user_mode, system_mode, wait_io, other_idle, result)
	values(:DateTime, :SysID, :UserMode, :SystemMode, :WaitIO, :OtherIdle, :Result); */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 9;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "insert into t_cpu_usage(date_time,sys_id,user_mode,system_mo\
de,wait_io,other_idle,result) values (:b0,:b1,:b2,:b3,:b4,:b5,:b6)";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )132;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlstm.sqhstv[0] = (         void  *)DateTime;
 sqlstm.sqhstl[0] = (unsigned int  )15;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&SysID;
 sqlstm.sqhstl[1] = (unsigned int  )4;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&UserMode;
 sqlstm.sqhstl[2] = (unsigned int  )4;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)&SystemMode;
 sqlstm.sqhstl[3] = (unsigned int  )4;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&WaitIO;
 sqlstm.sqhstl[4] = (unsigned int  )4;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)&OtherIdle;
 sqlstm.sqhstl[5] = (unsigned int  )4;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)Result;
 sqlstm.sqhstl[6] = (unsigned int  )2;
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



	if (sqlca.sqlcode == 0) 
	{
		/* EXEC SQL COMMIT WORK; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 9;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )175;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		return 1;
	}
	else 
	{
		printf("InsertSar Error: [%d]\n", sqlca.sqlcode);
		/* EXEC SQL ROLLBACK; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 9;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )190;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		return 0;
	}	
	
}



//--------------------------------------------------------------------
void SysInfo::GetBdf(char *date, int sysid)
{
	if(!DBConnect) return;
	
	int		i;
	char 	strCmd[200];
	FILE	*fp;
	
	
	/* EXEC SQL DELETE t_use_filesystem; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 9;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "delete  from t_use_filesystem ";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )205;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	
	strcpy(DateTime, date);
	SysID = sysid;
	if((fp = popen("bdf","r")) == NULL)
	{
		perror("popen err:");
		Result[0]='F';	
		InsertBdf();
		return;
	}else
	{
		fgets(strCmd, 200, fp);	// 한줄건너뛰기
		while(fgets(strCmd, 200, fp))
		{
			i=strlen(strCmd);
			i--;
			strCmd[i]=0x00;
			while(i--) 	if(strCmd[i]=='%') { strCmd[i]=' '; break;}
			sscanf(strCmd,"%s %d %d %d %d %s", VolumeName, &VolumeSize, &UsedSize, &AvailSize, &UsedRate, Location);
			printf("%s %d %d %d %d %s\n", VolumeName, VolumeSize, UsedSize, AvailSize, UsedRate, Location);
			Result[0]='S';	
			if(!InsertBdf()) break;
		}
	}
	pclose( fp );
}



//--------------------------------------------------------------------
void SysInfo::GetSar(char *date, int sysid)
{
	if(!DBConnect) return;
	
	int		i;
	char 	strCmd[200];
	char 	imsi[20];
	FILE	*fp;
	
	
	strcpy(DateTime, date);
	SysID = sysid;
	if((fp = popen("sar 1 1","r")) == NULL)
	{
		perror("popen err:");
		Result[0]='F';
		InsertSar();
		return;
	}else
	{
		i=4; while(i--) fgets(strCmd, 100, fp); // 4줄건너뛰기
		fgets(strCmd, 100, fp);
		strCmd[strlen(strCmd)-1]=0x00;
		sscanf(strCmd, "%s %d %d %d %d", imsi, &UserMode, &SystemMode, &WaitIO, &OtherIdle);
		printf("%d %d %d %d\n", UserMode, SystemMode, WaitIO, OtherIdle);
		Result[0]='S';
		InsertSar();
	}
	pclose( fp );
}





//--------------------------------------------------------------------
// infotype : 1(filesystem) 2.(cpu)
// date : YYYYMMDDhhmmss (14)
// systemid : UDA1, UDA2
int main(int argc, char *argv[])
{
	if(argc<4) 
	{
		printf("\nUSE : SysInfo <infotype> <date> <systemid> \n");
		exit(1);
	}
	
	SysInfo	sysInfo;
	
	if(atoi(argv[1])==1) sysInfo.GetBdf(argv[2], atoi(argv[3]));
	else sysInfo.GetSar(argv[2], atoi(argv[3]));	
	
	exit(1);
}

/*
int main()
{
	FILE 		*pid;
	int 		i, j;
	BdfInfo		bdf;
	SarInfo		sarif;
	
	char str[100];
	char cmd[100];
	
	char psCommandTime[]="ps -ef | grep              | grep -v grep | grep -v ftp | grep -v tail | awk '{ print $5 }'";
	char psCommand[]="ps -ef | grep              | grep -v grep | grep -v ftp | grep -v tail | awk '{ print $2 }'";
	//char bdfCommand[]="bdf | grep -e SVC1/RADIUS";  // | cut -f 1 -d %  | tail -c 3";
	//char bdfCommand[100]="bdf | grep -e SVC1/RADIUS | awk '{print $%d}'";
	char bdfCommand[100]="bdf | grep -e SVC1/RADIUS | awk '{print $%d}'";
	char sarifCommand[]="sarif 1 1 | tail -n 1 |awk '{ print $5 }'";
	
	
	
	
	if((pid = popen("bdf","r")) == NULL)
	{
			perror("popen err:");
	}else
	{
		fgets(str, 100, pid);	// 한줄건너뛰기
		while(fgets(str, 100, pid))
		{
			i=strlen(str);
			i--;
			str[i]=0x00;
			while(i--) 	if(str[i]=='%') { str[i]=' '; break;}
			sscanf(str,"%s %d %d %d %d %s", bdf.Filesystem, &bdf.Kbytes, &bdf.Used, &bdf.Avail, &bdf.Used, bdf.Mounted);
			printf("%s %d %d %d %d %s\n", bdf.Filesystem, bdf.Kbytes, bdf.Used, bdf.Avail, bdf.Used, bdf.Mounted);
		}
	}
	pclose( pid );
	
	
	if((pid = popen("sar 1 1","r")) == NULL)
	{
			perror("popen err:");
	}else
	{
		i=4; while(i--) fgets(str, 100, pid); // 4줄건너뛰기
		while(fgets(str, 100, pid))
		{
			str[strlen(str)-1]=0x00;
			sscanf(str, "%d %d %d %d", &sarif.Usr, &sarif.Sys, &sarif.Wio, &sarif.Idle);
			printf("%d %d %d %d\n", sarif.Usr, sarif.Sys, sarif.Wio, sarif.Idle);
			//printf("[%s]\n", str);
		}
	}
	pclose( pid );
	
	exit(1);	
}
*/


