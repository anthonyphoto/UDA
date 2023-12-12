
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
            void  *sqhstv[8];
   unsigned int   sqhstl[8];
            int   sqhsts[8];
            void  *sqindv[8];
            int   sqinds[8];
   unsigned int   sqharm[8];
   unsigned int   *sqharc[8];
   unsigned short  sqadto[8];
   unsigned short  sqtdso[8];
} sqlstm = {10,8};

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
5,0,0,1,223,0,3,67,0,0,8,8,0,1,0,1,3,0,0,1,3,0,0,1,97,0,0,1,3,0,0,1,3,0,0,1,3,
0,0,1,97,0,0,1,97,0,0,
52,0,0,2,0,0,29,71,0,0,0,0,0,1,0,
67,0,0,3,0,0,31,75,0,0,0,0,0,1,0,
82,0,0,4,226,0,5,82,0,0,8,8,0,1,0,1,3,0,0,1,3,0,0,1,3,0,0,1,97,0,0,1,97,0,0,1,
3,0,0,1,3,0,0,1,97,0,0,
129,0,0,5,0,0,29,90,0,0,0,0,0,1,0,
144,0,0,6,0,0,31,94,0,0,0,0,0,1,0,
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

* Version
  : 1.1 ( 2001. 05. 21 )                                           
******************************************************************/


#include <ProcPerform.hpp>

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

static int		nBillType;
static int		nFileSeq;
static char		ProcId[PROC_ID_LEN+1];
static int		nUdrCount;
static int		nFailCount;
static int		nQueryCount;
static char		SDate[DATE_LEN+1];
static char		EDate[DATE_LEN+1];
/* EXEC SQL END DECLARE SECTION; */ 



//--------------------------------------------------------------------
int ProcPerform::Set_PerForm(int billtype,int fileseq, int udrcount, int failcount, int querycount, char *sdate,char *edate)
{
	int	nRet;
	
	/*
	DbConnect	clsDbConnect;

	nRet = clsDbConnect.connect();
	if (nRet < 0)
	{
		cout << "DB Connect Failure [" << nRet << "]" << endl;
		return nRet;
	}else cout << "DB Connect Success " << endl;
	*/
	
	//---------------- 변수할당
	nBillType=billtype;
	nFileSeq=fileseq;
	sprintf(ProcId, "%d%d%d%d%02d%02d",	sys_id, svc_id, group_type, proc_type, proc_sn, proc_remark);
	nUdrCount = udrcount;
	nFailCount = failcount;
	nQueryCount = querycount;
	strcpy(SDate, sdate);
	strcpy(EDate, edate);
	//---------------- 변수할당
	
	cout << "INSERT.....T_PROC_PERFORM" << endl;
	/* EXEC SQL INSERT INTO t_proc_perform(bill_data_type, file_seq, proc_id, udr_count, fail_udr_count, query_count, start_date_time, end_date_time)
	values(:nBillType, :nFileSeq, :ProcId, :nUdrCount, :nFailCount, :nQueryCount, TO_DATE(:SDate, 'YYYYMMDDHH24MISS'), TO_DATE(:EDate, 'YYYYMMDDHH24MISS')); */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 8;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "insert into t_proc_perform(bill_data_type,file_seq,proc_id,u\
dr_count,fail_udr_count,query_count,start_date_time,end_date_time) values (:b0\
,:b1,:b2,:b3,:b4,:b5,TO_DATE(:b6,'YYYYMMDDHH24MISS'),TO_DATE(:b7,'YYYYMMDDHH24\
MISS'))";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlstm.sqhstv[0] = (         void  *)&nBillType;
 sqlstm.sqhstl[0] = (unsigned int  )4;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&nFileSeq;
 sqlstm.sqhstl[1] = (unsigned int  )4;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)ProcId;
 sqlstm.sqhstl[2] = (unsigned int  )9;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)&nUdrCount;
 sqlstm.sqhstl[3] = (unsigned int  )4;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&nFailCount;
 sqlstm.sqhstl[4] = (unsigned int  )4;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)&nQueryCount;
 sqlstm.sqhstl[5] = (unsigned int  )4;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)SDate;
 sqlstm.sqhstl[6] = (unsigned int  )15;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         void  *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned int  )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (         void  *)EDate;
 sqlstm.sqhstl[7] = (unsigned int  )15;
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         void  *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned int  )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
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


	
	nRet = sqlca.sqlcode;
	if (sqlca.sqlcode == 0)  /* EXEC SQL COMMIT WORK; */ 

{
                          struct sqlexd sqlstm;
                          sqlstm.sqlvsn = 10;
                          sqlstm.arrsiz = 8;
                          sqlstm.sqladtp = &sqladt;
                          sqlstm.sqltdsp = &sqltds;
                          sqlstm.iters = (unsigned int  )1;
                          sqlstm.offset = (unsigned int  )52;
                          sqlstm.cud = sqlcud0;
                          sqlstm.sqlest = (unsigned char  *)&sqlca;
                          sqlstm.sqlety = (unsigned short)0;
                          sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	else
	{
		cout << "[INSERT] T_PROC_PERFORM ERROR : " << (int)sqlca.sqlcode << endl;
		/* EXEC SQL ROLLBACK; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 8;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )67;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	}	
	
	
	if(nRet == -1) 
	{	// 추가에러 UPdate 하기 
		cout << "UPDATE....." << endl;
		/* EXEC SQL UPDATE t_proc_perform SET udr_count=:nUdrCount, 
										failcount = :nFailCount,
										query_count=:nQueryCount, 
										start_date_time=TO_DATE(:SDate, 'YYYYMMDDHH24MISS'),
										end_date_time=TO_DATE(:EDate, 'YYYYMMDDHH24MISS')
					WHERE bill_data_type =:nBillType and file_seq=:nFileSeq and proc_id=:ProcId; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 8;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "update t_proc_perform  set udr_count=:b0,failcount=:b1,quer\
y_count=:b2,start_date_time=TO_DATE(:b3,'YYYYMMDDHH24MISS'),end_date_time=TO_D\
ATE(:b4,'YYYYMMDDHH24MISS') where ((bill_data_type=:b5 and file_seq=:b6) and p\
roc_id=:b7)";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )82;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)0;
  sqlstm.sqhstv[0] = (         void  *)&nUdrCount;
  sqlstm.sqhstl[0] = (unsigned int  )4;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&nFailCount;
  sqlstm.sqhstl[1] = (unsigned int  )4;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)&nQueryCount;
  sqlstm.sqhstl[2] = (unsigned int  )4;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)SDate;
  sqlstm.sqhstl[3] = (unsigned int  )15;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         void  *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned int  )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (         void  *)EDate;
  sqlstm.sqhstl[4] = (unsigned int  )15;
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         void  *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned int  )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (         void  *)&nBillType;
  sqlstm.sqhstl[5] = (unsigned int  )4;
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         void  *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned int  )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (         void  *)&nFileSeq;
  sqlstm.sqhstl[6] = (unsigned int  )4;
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         void  *)0;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned int  )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
  sqlstm.sqhstv[7] = (         void  *)ProcId;
  sqlstm.sqhstl[7] = (unsigned int  )9;
  sqlstm.sqhsts[7] = (         int  )0;
  sqlstm.sqindv[7] = (         void  *)0;
  sqlstm.sqinds[7] = (         int  )0;
  sqlstm.sqharm[7] = (unsigned int  )0;
  sqlstm.sqadto[7] = (unsigned short )0;
  sqlstm.sqtdso[7] = (unsigned short )0;
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


		
		
		if (sqlca.sqlcode == 0)  /* EXEC SQL COMMIT WORK; */ 

{
                           struct sqlexd sqlstm;
                           sqlstm.sqlvsn = 10;
                           sqlstm.arrsiz = 8;
                           sqlstm.sqladtp = &sqladt;
                           sqlstm.sqltdsp = &sqltds;
                           sqlstm.iters = (unsigned int  )1;
                           sqlstm.offset = (unsigned int  )129;
                           sqlstm.cud = sqlcud0;
                           sqlstm.sqlest = (unsigned char  *)&sqlca;
                           sqlstm.sqlety = (unsigned short)0;
                           sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		else
		{
			cout << "[UPDATE] T_PROC_PERFORM ERROR : " << sqlca.sqlcode << endl;
			/* EXEC SQL ROLLBACK; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 8;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )144;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		}	
	}
	

	cout << "nBillType   = " << nBillType << endl;
	cout << "nFileSeq    = " << nFileSeq << endl;
	cout << "ProcId      = " << ProcId << endl;
	cout << "nUdrCount   = " << nUdrCount << endl;
	cout << "nQueryCount = " << nQueryCount << endl;
	cout << "SDate 	     = " << SDate << endl;
	cout << "EDate	     = " << EDate << endl;

	nRet = (int)sqlca.sqlcode;

 	//clsDbConnect.disconnect();

 	return (nRet);
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
