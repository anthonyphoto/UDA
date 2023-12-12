
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
           char  filnam[15];
};
static const struct sqlcxp sqlfpn =
{
    14,
    "ProcPerform.pc"
};


static unsigned long sqlctx = 1300099;


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

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{10,4130,0,0,0,
5,0,0,1,166,0,4,55,0,0,7,2,0,1,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,1,97,
0,0,1,97,0,0,
48,0,0,2,156,0,5,90,0,0,7,7,0,1,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,97,
0,0,1,97,0,0,
91,0,0,3,0,0,29,99,0,0,0,0,0,1,0,
106,0,0,4,0,0,31,100,0,0,0,0,0,1,0,
121,0,0,5,154,0,3,138,0,0,7,7,0,1,0,1,97,0,0,1,97,0,0,1,3,0,0,1,3,0,0,1,3,0,0,
1,3,0,0,1,3,0,0,
164,0,0,6,0,0,29,142,0,0,0,0,0,1,0,
179,0,0,7,0,0,31,143,0,0,0,0,0,1,0,
};


/******************************************************************
* 파 일 명     
      : ProcPerform.pc                
* 기   능                                                          
     : UDR 정보(정상 UDR , 비정상 UDR 개수, 한도 질의 회수)를 
       [t_proc_perform] table에 insert or update한다.                      
* 작 성 자     
     : 조상영                         
* first data  
     :                    
* last updata 
     :                 
* 관 련 화 일  
     : ProcPerform.hpp                     
* program history  
	   2001.06.14 파일별 성능 통계를 시간별 성능 통계로 바꿈                              

* Version
  : 1.1 ( 2001. 05. 21 )                                           
******************************************************************/


#include <ProcPerform.hpp>

#define D_AGGREGATOR_ID			"21"				// smy 2001.06.14 (Aggregator 통계 정보를 이외의 process와 다르게 처리하기 위한 구분자)

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

static char		sProcessID[PROCESS_ID_LEN+1];
static char		sProcessTime[PROCESS_TIME_LEN+1];
static int		nNormal_udr_cnt;
static int		nFail_udr_cnt;
static int		nAggr_udr_cnt;
static int		nNormal_file_cnt;
static int		nQuery_count;
/* EXEC SQL END DECLARE SECTION; */ 



//--------------------------------------------------------------------
int ProcPerform::Set_PerForm(int fileflag, int udrcount, int failcount, int querycount, char *proctime)
{
	memset(sProcessID  , 0x00, sizeof(sProcessID));
	memset(sProcessTime, 0x00, sizeof(sProcessTime));

	//---------------- 변수할당
	sprintf(sProcessID , "%d%d%d%d%02d%02d",	sys_id, svc_id, group_type, proc_type, proc_sn, proc_remark);
	strcpy(sProcessTime, proctime);
	//---------------- 변수할당//
	
	//---------------- select
	cout <<"[Set_PerForm] SELECT ------------ " << endl;
	/* EXEC SQL SELECT normal_udr_cnt, fail_udr_cnt, aggr_udr_cnt, normal_file_cnt, query_count
	          INTO  :nNormal_udr_cnt, :nFail_udr_cnt, :nAggr_udr_cnt, :nNormal_file_cnt, :nQuery_count
	          FROM  t_perform_test
	          WHERE proc_id=:sProcessID AND process_time=:sProcessTime; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 7;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select normal_udr_cnt ,fail_udr_cnt ,aggr_udr_cnt ,normal_fi\
le_cnt ,query_count into :b0,:b1,:b2,:b3,:b4  from t_perform_test where (proc_\
id=:b5 and process_time=:b6)";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlstm.sqhstv[0] = (         void  *)&nNormal_udr_cnt;
 sqlstm.sqhstl[0] = (unsigned int  )4;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&nFail_udr_cnt;
 sqlstm.sqhstl[1] = (unsigned int  )4;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&nAggr_udr_cnt;
 sqlstm.sqhstl[2] = (unsigned int  )4;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)&nNormal_file_cnt;
 sqlstm.sqhstl[3] = (unsigned int  )4;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&nQuery_count;
 sqlstm.sqhstl[4] = (unsigned int  )4;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)sProcessID;
 sqlstm.sqhstl[5] = (unsigned int  )9;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)sProcessTime;
 sqlstm.sqhstl[6] = (unsigned int  )11;
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


	          
	if(sqlca.sqlcode == 0)
	{
		//------------- 변수할당
		if(strncmp(&sProcessID[2], D_AGGREGATOR_ID, 2) == 0)
			nNormal_udr_cnt += udrcount - failcount;
		else
			nNormal_udr_cnt	+= udrcount; 
		
		nFail_udr_cnt	+= failcount;   
		nAggr_udr_cnt	= nNormal_udr_cnt + nFail_udr_cnt;   

		if(fileflag == 1)
			nNormal_file_cnt++;
		else
			nNormal_file_cnt = 0;

		nQuery_count	+= querycount;
		//------------- 변수할당//    

		cout << "[Set_PerForm] UPDATE ------------ "		<< endl;
//		cout << "sProcessID       : [" << &sProcessID[2]    << "]" << endl;
//		cout << "sProcessID       : [" << sProcessID		<< "]" << endl; 
//		cout << "sProcessTime     : [" << sProcessTime		<< "]" << endl; 
//		cout << "nNormal_udr_cnt  : [" << nNormal_udr_cnt	<< "]" << endl;
//		cout << "nFail_udr_cnt    : [" << nFail_udr_cnt		<< "]" << endl;
//		cout << "nAggr_udr_cnt    : [" << nAggr_udr_cnt		<< "]" << endl;
//		cout << "nNormal_file_cnt : [" << nNormal_file_cnt	<< "]" << endl;
//		cout << "nQuery_count     : [" << nQuery_count 		<< "]" << endl;
		
		//------------- update
		/* EXEC SQL UPDATE t_perform_test
		         SET    normal_udr_cnt=:nNormal_udr_cnt, 
		                fail_udr_cnt=:nFail_udr_cnt,
		                aggr_udr_cnt=:nAggr_udr_cnt,
		                normal_file_cnt=:nNormal_file_cnt,
		                query_count=:nQuery_count
		          WHERE proc_id=:sProcessID AND process_time=:sProcessTime; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 7;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "update t_perform_test  set normal_udr_cnt=:b0,fail_udr_cnt=\
:b1,aggr_udr_cnt=:b2,normal_file_cnt=:b3,query_count=:b4 where (proc_id=:b5 an\
d process_time=:b6)";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )48;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)0;
  sqlstm.sqhstv[0] = (         void  *)&nNormal_udr_cnt;
  sqlstm.sqhstl[0] = (unsigned int  )4;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&nFail_udr_cnt;
  sqlstm.sqhstl[1] = (unsigned int  )4;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)&nAggr_udr_cnt;
  sqlstm.sqhstl[2] = (unsigned int  )4;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)&nNormal_file_cnt;
  sqlstm.sqhstl[3] = (unsigned int  )4;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         void  *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned int  )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (         void  *)&nQuery_count;
  sqlstm.sqhstl[4] = (unsigned int  )4;
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         void  *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned int  )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (         void  *)sProcessID;
  sqlstm.sqhstl[5] = (unsigned int  )9;
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         void  *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned int  )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (         void  *)sProcessTime;
  sqlstm.sqhstl[6] = (unsigned int  )11;
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


		
		
		if(sqlca.sqlcode == 0)  /* EXEC SQL COMMIT WORK; */ 

{
                          struct sqlexd sqlstm;
                          sqlstm.sqlvsn = 10;
                          sqlstm.arrsiz = 7;
                          sqlstm.sqladtp = &sqladt;
                          sqlstm.sqltdsp = &sqltds;
                          sqlstm.iters = (unsigned int  )1;
                          sqlstm.offset = (unsigned int  )91;
                          sqlstm.cud = sqlcud0;
                          sqlstm.sqlest = (unsigned char  *)&sqlca;
                          sqlstm.sqlety = (unsigned short)0;
                          sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		else /* EXEC SQL ROLLBACK; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 10;
       sqlstm.arrsiz = 7;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )106;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)0;
       sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		
		return ((int)sqlca.sqlcode);
		
	}
	else 
	{
		
		cout <<"[Set_PerForm] SELECT ERROR : " <<  (int)sqlca.sqlcode << endl;
		cout <<"[Set_PerForm] INSERT ------------- "<< endl;
		nNormal_udr_cnt=nFail_udr_cnt=nAggr_udr_cnt=nNormal_file_cnt=nQuery_count=0;

		//------------- 변수할당
		if(strncmp(&sProcessID[2], D_AGGREGATOR_ID, 2) == 0)
			nNormal_udr_cnt = udrcount - failcount;
		else
			nNormal_udr_cnt	= udrcount; 
		
		nFail_udr_cnt	= failcount;   
		nAggr_udr_cnt	= nNormal_udr_cnt + nFail_udr_cnt;   


		if(fileflag == 1)
			nNormal_file_cnt++;
		else
			nNormal_file_cnt = 0;

		nQuery_count	= querycount;    
		//------------- 변수할당//
//		cout << "sProcessID       : [" << &sProcessID[2]    << "]" << endl;
//		cout << "sProcessID       : [" << sProcessID		<< "]" << endl; 
//		cout << "sProcessTime     : [" << sProcessTime		<< "]" << endl; 
//		cout << "nNormal_udr_cnt  : [" << nNormal_udr_cnt	<< "]" << endl;
//		cout << "nFail_udr_cnt    : [" << nFail_udr_cnt		<< "]" << endl;
//		cout << "nAggr_udr_cnt    : [" << nAggr_udr_cnt		<< "]" << endl;
//		cout << "nNormal_file_cnt : [" << nNormal_file_cnt	<< "]" << endl;
//		cout << "nQuery_count     : [" << nQuery_count 		<< "]" << endl;
		
		/* EXEC SQL INSERT INTO t_perform_test(proc_id, process_time, normal_udr_cnt, fail_udr_cnt, aggr_udr_cnt, normal_file_cnt, query_count)
		values(:sProcessID, :sProcessTime, :nNormal_udr_cnt, :nFail_udr_cnt, :nAggr_udr_cnt, :nNormal_file_cnt, :nQuery_count); */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 7;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "insert into t_perform_test(proc_id,process_time,normal_udr_\
cnt,fail_udr_cnt,aggr_udr_cnt,normal_file_cnt,query_count) values (:b0,:b1,:b2\
,:b3,:b4,:b5,:b6)";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )121;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)0;
  sqlstm.sqhstv[0] = (         void  *)sProcessID;
  sqlstm.sqhstl[0] = (unsigned int  )9;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)sProcessTime;
  sqlstm.sqhstl[1] = (unsigned int  )11;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)&nNormal_udr_cnt;
  sqlstm.sqhstl[2] = (unsigned int  )4;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)&nFail_udr_cnt;
  sqlstm.sqhstl[3] = (unsigned int  )4;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         void  *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned int  )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (         void  *)&nAggr_udr_cnt;
  sqlstm.sqhstl[4] = (unsigned int  )4;
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         void  *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned int  )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (         void  *)&nNormal_file_cnt;
  sqlstm.sqhstl[5] = (unsigned int  )4;
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         void  *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned int  )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (         void  *)&nQuery_count;
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


	
		cout << "sqlca.sqlcode : [" << sqlca.sqlcode << "]" << endl;
		if (sqlca.sqlcode == 0)  /* EXEC SQL COMMIT WORK; */ 

{
                           struct sqlexd sqlstm;
                           sqlstm.sqlvsn = 10;
                           sqlstm.arrsiz = 7;
                           sqlstm.sqladtp = &sqladt;
                           sqlstm.sqltdsp = &sqltds;
                           sqlstm.iters = (unsigned int  )1;
                           sqlstm.offset = (unsigned int  )164;
                           sqlstm.cud = sqlcud0;
                           sqlstm.sqlest = (unsigned char  *)&sqlca;
                           sqlstm.sqlety = (unsigned short)0;
                           sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		else /* EXEC SQL ROLLBACK; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 10;
       sqlstm.arrsiz = 7;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )179;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)0;
       sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		
		return ((int)sqlca.sqlcode);
	}
}


/*
//--------------------------------------------------------------------
int main(int argc, char *argv[])
{
	
	if(argc<6) 
	{
		cout << " Usage:" << argv[0] << "[systemid] [serviceid] [groupid] [processtype] [serialno] [remarks]" << endl;
		exit(1);
	}
	
	int			nRet;
	DbConnect	clsDbConnect;

	nRet = clsDbConnect.connect();
	if (nRet < 0)
	{
		cout << "DB Connect Failure [" << nRet << "]" << endl;
		return nRet;
	}else cout << "DB Connect Success " << endl;
	
	ProcPerform Perform(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]), atoi(argv[6]));
	
	//Perform.Set_PerForm(1, 20, 6, 20, "20001111121220", "20001111121250");
	Perform.Set_PerForm(1, 20, 6, 100, "20001111121220", "20001115121250");
	
	clsDbConnect.disconnect();
	exit(1);
}
*/
