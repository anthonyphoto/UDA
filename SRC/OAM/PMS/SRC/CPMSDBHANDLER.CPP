
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
           char  filnam[17];
};
static const struct sqlcxp sqlfpn =
{
    16,
    "CPmsDbHandler.pc"
};


static unsigned long sqlctx = 4756219;


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
            void  *sqhstv[23];
   unsigned int   sqhstl[23];
            int   sqhsts[23];
            void  *sqindv[23];
            int   sqinds[23];
   unsigned int   sqharm[23];
   unsigned int   *sqharc[23];
   unsigned short  sqadto[23];
   unsigned short  sqtdso[23];
} sqlstm = {10,23};

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

 static const char *sq0016 = 
"select data_dir ,file_name ,bak_dir ,bak_fname ,bak_day ,bak_delday ,nvl(ba\
k_time,' ')  from t_backup_info where sys_id=:b0           ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{10,4138,0,0,0,
5,0,0,0,0,0,60,64,0,0,0,0,0,1,0,
20,0,0,0,0,0,58,66,0,0,1,1,0,1,0,3,110,0,0,
39,0,0,1,61,0,4,123,0,0,2,1,0,1,0,2,97,0,0,1,3,0,0,
62,0,0,2,101,0,3,142,0,0,5,5,0,1,0,1,97,0,0,1,3,0,0,1,97,0,0,1,3,0,0,1,97,0,0,
97,0,0,3,0,0,31,150,0,0,0,0,0,1,0,
112,0,0,4,0,0,29,153,0,0,0,0,0,1,0,
127,0,0,5,142,0,6,222,0,0,3,3,0,1,0,3,3,0,0,1,97,0,0,2,97,0,0,
154,0,0,6,105,0,6,317,0,0,3,3,0,1,0,1,3,0,0,2,3,0,0,1,97,0,0,
181,0,0,7,0,0,17,433,0,0,1,1,0,1,0,1,97,0,0,
200,0,0,7,0,0,45,435,0,0,0,0,0,1,0,
215,0,0,7,0,0,13,441,0,0,23,0,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,3,0,0,2,3,0,0,
2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,97,0,0,2,3,0,0,2,3,0,0,2,97,
0,0,2,3,0,0,2,3,0,0,2,1,0,0,2,1,0,0,2,3,0,0,2,97,0,0,2,3,0,0,2,3,0,0,
322,0,0,7,0,0,15,457,0,0,0,0,0,1,0,
337,0,0,8,127,0,4,667,0,0,6,2,0,1,0,2,97,0,0,2,97,0,0,2,3,0,0,2,97,0,0,1,97,0,
0,1,3,0,0,
376,0,0,9,103,0,3,723,0,0,5,5,0,1,0,1,97,0,0,1,68,0,0,1,97,0,0,1,68,0,0,1,1,0,
0,
411,0,0,10,113,0,3,728,0,0,6,6,0,1,0,1,97,0,0,1,68,0,0,1,97,0,0,1,68,0,0,1,68,
0,0,1,1,0,0,
450,0,0,11,123,0,3,733,0,0,7,7,0,1,0,1,97,0,0,1,68,0,0,1,97,0,0,1,68,0,0,1,68,
0,0,1,68,0,0,1,1,0,0,
493,0,0,12,133,0,3,738,0,0,8,8,0,1,0,1,97,0,0,1,68,0,0,1,97,0,0,1,68,0,0,1,68,
0,0,1,68,0,0,1,68,0,0,1,1,0,0,
540,0,0,13,143,0,3,743,0,0,9,9,0,1,0,1,97,0,0,1,68,0,0,1,97,0,0,1,68,0,0,1,68,
0,0,1,68,0,0,1,68,0,0,1,68,0,0,1,1,0,0,
591,0,0,14,153,0,3,748,0,0,10,10,0,1,0,1,97,0,0,1,68,0,0,1,97,0,0,1,68,0,0,1,
68,0,0,1,68,0,0,1,68,0,0,1,68,0,0,1,68,0,0,1,1,0,0,
646,0,0,15,0,0,29,765,0,0,0,0,0,1,0,
661,0,0,16,134,0,9,792,0,0,1,1,0,1,0,1,3,0,0,
680,0,0,16,0,0,13,797,0,0,7,0,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,3,0,
0,2,3,0,0,2,97,0,0,
723,0,0,16,0,0,15,823,0,0,0,0,0,1,0,
738,0,0,17,64,0,5,864,0,0,2,2,0,1,0,1,1,0,0,1,97,0,0,
761,0,0,18,0,0,31,867,0,0,0,0,0,1,0,
776,0,0,19,0,0,29,873,0,0,0,0,0,1,0,
791,0,0,20,0,0,31,879,0,0,0,0,0,1,0,
806,0,0,21,0,0,24,969,0,0,1,1,0,1,0,1,97,0,0,
825,0,0,22,0,0,29,971,0,0,0,0,0,1,0,
840,0,0,23,0,0,31,975,0,0,0,0,0,1,0,
855,0,0,24,0,0,27,1002,0,0,4,4,0,1,0,1,97,0,0,1,97,0,0,1,10,0,0,1,10,0,0,
886,0,0,25,0,0,30,1024,0,0,0,0,0,1,0,
901,0,0,0,0,0,59,1026,0,0,1,1,0,1,0,3,110,0,0,
};


/*******************************************************************************
*프로그램 설명:
*   multi threaded application을 개발할때 유의할점.
*   oracle developing multi-threaded application 참고.
*
*   1. 컴파일시에 thread=yes라는 옵션을 반드시 써야한다.
*      이것은 sql 문장이 정의한 runtime context의 범위내에서
*          실행되도록 하기 위한 것이다.
*      ex)proc iname=*.pc thread=yes
*
*   2.EXEC SQL ENABLE THREADS
*     multiple thread 를 지원하는 프로세스의 초기화작업.
*
*   3.EXEC SQL CONTEXT ALLOCATE :context_var
*     context variable을 초기화.
*         context variable을 초기화하기때문에 runtime context에서
*         발생한 모든 작업들이 초기화 되어진다.
*     connect의 정보도 context에 남겨지므로 connect한후에
*         allocate 한다면 not logged on 상태가 되므로 유의해한다.
*
*   4.EXEC SQL CONTEXT USE :context_var
*     sql 문장이 실행될 범위를 나타내는것으로 sql문장중 가장 첨에
*     나와야 한다.
* Version 1.1 ( 2001. 05. 21 )
*******************************************************************************/
#define SQLCA_STORAGE_CLASS extern
#include "CPmsDbHandler.hpp"
#include <unistd.h>

#define SQLCA_STORAGE_CLASS extern
/* EXEC SQL INCLUDE SQLCA;
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

    sql_context ctx[6];
/* EXEC SQL END DECLARE SECTION; */ 


extern pthread_mutex_t  mutex ;

extern char PMSNAME    [10];  // pms alias
extern char LOGMGR      [10]; // Log Manager
extern char USER       [10];    // DB ID
extern char PASSWD     [10];    // DB PASSWORD

extern int g_sysId      ;
extern int g_svcId      ;
extern int g_groupType  ;
extern int g_proc_type  ;
extern int g_proc_sn    ;
extern int g_proc_remark;

extern char LOGFILE[40];




/*******************************************************************************
* Desc : 생성자. DB Handler 초기화 작업
* Argu : i        - context number      default = 0
*        recvName - Receive pipe Name   default = "recvpipe"
*        sendName - Send pipe Name      default = 'sendpipe"
*******************************************************************************/
CPmsDbHandler::CPmsDbHandler(int i, char* recvName, char* sendName)
{
    /* EXEC SQL ENABLE THREADS; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 0;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )5;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

        // Thread를 사용함을 선언

    /* EXEC SQL CONTEXT ALLOCATE :ctx[i]; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 1;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )20;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlstm.sqhstv[0] = (         void  *)&ctx[i];
    sqlstm.sqhstl[0] = (unsigned int  )4;
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

 // DB context 선언

    strcpy(m_recvPipe, recvName);       // receive pipe name 지정
    strcpy(m_sendPipe, sendName);       // send    pipe name 지정
}




/*****************************************************************************************
* Desc : Trap Data를 T_TRAP_INFO에서 조회하여 T_TRAP_LOG로 insert한다.
*        PMS에서 Trap의 종류로는 프로세스 실행, 프로세스 종료가 발생할때마다 Trap Data를
*        삽입한다.
*                                                                 01-05-26 3:38오후 ky.
* Argu : int nTrapNo - Trap Number (key)
*        int nValue  - Trap Value
*        char* strDescF - trap description
*        ... - sub description
* Retn : void
* etc. : none
*****************************************************************************************/
void CPmsDbHandler::InsertTrapData(int i, int nTrapNo, int nValue, char* strDescF, ...)
{
    /* EXEC SQL CONTEXT USE :ctx[i]; */ 

    
//    DebugLog(LOGFILE, "  [InsertTrapData]");

    // make description.
    //----------------------------------------------------------
    va_list var;
    va_start(var, strDescF);
    char strDesc[128+1];
    vsprintf(strDesc, strDescF, var);
    
//    DebugLog(LOGFILE, "    %d. %s", nValue, strDesc);

    int      nIndex; // T_TRAP_INFO list index.
    int      nKey;   // T_TRAP_INFO search key.

    // T_TRAP_INFO에서 데이터를 읽어와 T_TRAP_LOG에 로그를 남긴다.
    //----------------------------------------------------------
    switch (nTrapNo)
    {
        case D_UDA_TRAP_SERVICE_START    :
        case D_UDA_TRAP_SERVICE_STOP     :
        case D_UDA_TRAP_SERVICE_TAKE_OVER:
        case D_UDA_TRAP_PROCESS_UP       :
        case D_UDA_TRAP_PROCESS_DOWN     : 
        case D_UDA_TRAP_PROXY_FAIL       : nKey = nTrapNo; break;
        default : return ;
    }

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    	 int h_trap_no = nTrapNo;
    	 char h_trap_name [64+1];
    /* EXEC SQL END   DECLARE SECTION; */ 

    
    /* EXEC SQL SELECT trap_name INTO :h_trap_name from t_trap_info where TRAP_NO = :h_trap_no; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 2;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "select trap_name into :b0  from t_trap_info where TRAP_N\
O=:b1";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )39;
    sqlstm.selerr = (unsigned short)1;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlstm.sqhstv[0] = (         void  *)h_trap_name;
    sqlstm.sqhstl[0] = (unsigned int  )65;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (         void  *)&h_trap_no;
    sqlstm.sqhstl[1] = (unsigned int  )4;
    sqlstm.sqhsts[1] = (         int  )0;
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
    sqlcxt(&ctx[i], &sqlctx, &sqlstm, &sqlfpn);
}


    
    if (sqlca.sqlcode != 0) {
//     	  DebugLog(LOGFILE, "    [ERROR] search key : %d", h_trap_no);
//     	  DebugLog(LOGFILE, "            sql msg : %.*s", sqlca.sqlerrm.sqlerrml, sqlca.sqlerrm.sqlerrmc);
	 	  return ;
	 }

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

       const char *h_ptrRecvTime;                  // recvtime
       const int   h_nTrapNo     = nTrapNo;        // trap number
       const char *h_ptrTrapName = h_trap_name;    // trap name
       const int   h_nTrapValue  = nValue;         // trap value
       const char *h_ptrTrapDesc = strDesc;        // trap description
    /* EXEC SQL END   DECLARE SECTION; */ 

 
    DateTime Itime;                                // time class object
    h_ptrRecvTime = Itime.get_MDateTime(1);        // 년월일시분초천분의일초 ex) '20010522211523332'
 
    /* EXEC SQL INSERT INTO T_TRAP_LOG (recv_time, trap_no, trap_name, trap_value, trap_desc)
                    VALUES (:h_ptrRecvTime, :h_nTrapNo, :h_ptrTrapName,
                            :h_nTrapValue, :h_ptrTrapDesc); */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 5;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "insert into T_TRAP_LOG(recv_time,trap_no,trap_name,trap_\
value,trap_desc) values (:b0,:b1,:b2,:b3,:b4)";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )62;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlstm.sqhstv[0] = (         void  *)h_ptrRecvTime;
    sqlstm.sqhstl[0] = (unsigned int  )0;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (         void  *)&h_nTrapNo;
    sqlstm.sqhstl[1] = (unsigned int  )4;
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         void  *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned int  )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqhstv[2] = (         void  *)h_ptrTrapName;
    sqlstm.sqhstl[2] = (unsigned int  )0;
    sqlstm.sqhsts[2] = (         int  )0;
    sqlstm.sqindv[2] = (         void  *)0;
    sqlstm.sqinds[2] = (         int  )0;
    sqlstm.sqharm[2] = (unsigned int  )0;
    sqlstm.sqadto[2] = (unsigned short )0;
    sqlstm.sqtdso[2] = (unsigned short )0;
    sqlstm.sqhstv[3] = (         void  *)&h_nTrapValue;
    sqlstm.sqhstl[3] = (unsigned int  )4;
    sqlstm.sqhsts[3] = (         int  )0;
    sqlstm.sqindv[3] = (         void  *)0;
    sqlstm.sqinds[3] = (         int  )0;
    sqlstm.sqharm[3] = (unsigned int  )0;
    sqlstm.sqadto[3] = (unsigned short )0;
    sqlstm.sqtdso[3] = (unsigned short )0;
    sqlstm.sqhstv[4] = (         void  *)h_ptrTrapDesc;
    sqlstm.sqhstl[4] = (unsigned int  )0;
    sqlstm.sqhsts[4] = (         int  )0;
    sqlstm.sqindv[4] = (         void  *)0;
    sqlstm.sqinds[4] = (         int  )0;
    sqlstm.sqharm[4] = (unsigned int  )0;
    sqlstm.sqadto[4] = (unsigned short )0;
    sqlstm.sqtdso[4] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&ctx[i], &sqlctx, &sqlstm, &sqlfpn);
}



    if (sqlca.sqlcode != 0) {                     
        DebugLog(LOGFILE, "    [ERROR] T_TRAP_LOG Insert fail. no:%d, name:%s, value:%d, desc:%s",
            nTrapNo, nValue, nValue, strDesc);
        DebugLog(LOGFILE, "            sqlmsg : %.*s", sqlca.sqlerrm.sqlerrml-1, sqlca.sqlerrm.sqlerrmc);
        /* EXEC SQL ROLLBACK WORK; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 10;
        sqlstm.arrsiz = 5;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )97;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)0;
        sqlcxt(&ctx[i], &sqlctx, &sqlstm, &sqlfpn);
}


    }
    else {
        /* EXEC SQL COMMIT WORK; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 10;
        sqlstm.arrsiz = 5;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )112;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)0;
        sqlcxt(&ctx[i], &sqlctx, &sqlstm, &sqlfpn);
}


        DebugLog(LOGFILE, "");
    }
}//end InsertTrapData






/*******************************************************************************
* Desc : 파일에 SQL ERROR LOG 남김
* Argu : errMsg - 에러 메세지
*        sqlca  - 에러 정보 구조체
*******************************************************************************/
void CPmsDbHandler::SqlError(char* errMsg, struct sqlca sqlca)
{
    // ORA-01405: fetched column value is NULL
    if (sqlca.sqlcode != abs(1405))
        DebugLog(LOGFILE, "%s (%d) : %s",
                 errMsg, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc);

    //error log (SQL Error)
    ErrorLog(7100, "%s(%d) : %s",
             errMsg, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc);
}

/*******************************************************************************
* Desc: client가 전송한 msg를 oracle pipe를 이용하여
*   수신한다.
*
*   command
*       T - terminate process
*       P - stop process (suspen)
*       S - start process
*
*   type
*       O - one (개별적)
*       G - group 단위로 command 실행
*       A - all (전체)
*
*   subContent (선택한 type에 따라 내용의 의미가 달라짐.)
*       O 일경우 - 명령을 실행할 프로세스 이름
*       G 일경우 - group code (1,2,3,...)
*       A 일경우 - 스페이스
*
* Argu :
*    retCommand     - pipe로부터 수신한 command 정보.
*    retType    - pipe로부터 수신한 type 정보
*    retSubContent  - pipe로부터 수신한 subContent 정보
*******************************************************************************/
bool CPmsDbHandler::RecvMsgFromPipe(int i, command_t &retCommand)
{
    /* EXEC SQL CONTEXT USE :ctx[i]; */ 


    struct sqlca sqlca;
    sqlca.sqlcode = 0;

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

        int status;
        char *recvPipe;
        char command[87+1];
    /* EXEC SQL END DECLARE SECTION; */ 


    recvPipe = m_recvPipe;

    /* EXEC SQL WHENEVER SQLERROR goto _sqlerr_; */ 


    /* Wait for a message to be received, using pipe daemon. */
    /* EXEC SQL EXECUTE
    begin
            :status := dbms_pipe.receive_message(:recvPipe, 60);
            if :status = 0 then
            dbms_pipe.unpack_message(:command);
            end if;
        end;
    END-EXEC; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 5;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "begin :status := dbms_pipe . receive_message ( :recvPipe\
 , 60 ) ; if :status = 0 then dbms_pipe . unpack_message ( :command ) ; end i\
f ; end ;";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )127;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlstm.sqhstv[0] = (         void  *)&status;
    sqlstm.sqhstl[0] = (unsigned int  )4;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (         void  *)recvPipe;
    sqlstm.sqhstl[1] = (unsigned int  )0;
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         void  *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned int  )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqhstv[2] = (         void  *)command;
    sqlstm.sqhstl[2] = (unsigned int  )88;
    sqlstm.sqhsts[2] = (         int  )0;
    sqlstm.sqindv[2] = (         void  *)0;
    sqlstm.sqinds[2] = (         int  )0;
    sqlstm.sqharm[2] = (unsigned int  )0;
    sqlstm.sqadto[2] = (unsigned short )0;
    sqlstm.sqtdso[2] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&ctx[i], &sqlctx, &sqlstm, &sqlfpn);
    if (sqlca.sqlcode < 0) goto _sqlerr_;
}



    if (status != 0)
    {
//      DebugLog(LOGFILE,"fail to receive message from pipe %d", status);
        return false;
    }

    DebugLog(LOGFILE, "Success to Receive Message from pipe %d", status);
    DebugLog(LOGFILE, "COMMAND = %s", command );

    SplitCommand(command, retCommand);

    DebugLog(LOGFILE, "");
    DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
    DebugLog(LOGFILE, "  command    : %c"              , retCommand.command );
    DebugLog(LOGFILE, "  type       : %c"              , retCommand.type );
    DebugLog(LOGFILE, "  subContent : %s"              , retCommand.subContent );
    DebugLog(LOGFILE, "  date       : %s"              , retCommand.date );
    DebugLog(LOGFILE, "  filePart   : %c"              , retCommand.filePart );
    DebugLog(LOGFILE, "  item1      : %s"              , retCommand.item1 );
    DebugLog(LOGFILE, "  item2      : %s"              , retCommand.item2 );
    DebugLog(LOGFILE, "  item3      : %s"              , retCommand.item3 );
    DebugLog(LOGFILE, "  item4      : %s"              , retCommand.item4 );
    DebugLog(LOGFILE, "  item5      : %s"              , retCommand.item5 );
    DebugLog(LOGFILE, "  item6      : %s"              , retCommand.item6 );
    DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
   DebugLog(LOGFILE, "");

    return true;

_sqlerr_:
    // ctrl+c를 눌렀을경우 1013오류가 발생하는데 더 이상 작업을
    // 할 수 없기때문에 종료한다.
    if ( abs(sqlca.sqlcode) == 1013 )
    {
        sleep(1);
        return false;
    }
    else
    {
        DebugLog(LOGFILE, "Oracle Error. Sql error : %s", sqlca.sqlerrm.sqlerrmc);
        //error log (oracle error)
        ErrorLog(7101, "oracle error %s", sqlca.sqlerrm.sqlerrmc);
        return false;
    }
}

void CPmsDbHandler::SplitCommand(char* command, command_t &c)
{
    c.command = toupper(command[0]);
    c.type    = toupper(command[1]);
    sprintf(c.subContent, "%10.10s", &command[2]);
    strtok (c.subContent, " ");
    sprintf(c.date      , "%14.14s", &command[12]);
    strtok (c.date      , " ");
    c.filePart = command[26];
    sprintf(c.item1, "%10.10s", &command[27]);
    strtok (c.item1, " ");
    sprintf(c.item2, "%10.10s", &command[37]);
    strtok (c.item2, " ");
    sprintf(c.item3, "%10.10s", &command[47]);
    strtok (c.item3, " ");
    sprintf(c.item4, "%10.10s", &command[57]);
    strtok (c.item4, " ");
    sprintf(c.item5, "%10.10s", &command[67]);
    strtok (c.item5, " ");
    sprintf(c.item6, "%10.10s", &command[77]);
    strtok (c.item6, " ");
}

/*******************************************************************************
* Desc : client가 요청한 작업을 완료하고 성공여부를 전송함.
*    성공 - 1
*    실패 - 0
*******************************************************************************/
bool CPmsDbHandler::SendResultMsgToPipe(int i, int flag)
{
    /* EXEC SQL CONTEXT USE :ctx[i]; */ 


    /* EXEC SQL BEGIN DECLARE SECTION; */ 

        int sendValue = flag;
        char *sendPipe = m_sendPipe;
        int status;
    /* EXEC SQL END DECLARE SECTION; */ 


    /* EXEC SQL WHENEVER SQLERROR do SqlError("Fail to pipe.send_message", sqlca); */ 


    /* EXEC SQL EXECUTE
    begin
        dbms_pipe.pack_message(:sendValue);
        :status := dbms_pipe.send_message(:sendPipe);
    end;
    END-EXEC; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 5;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "begin dbms_pipe . pack_message ( :sendValue ) ; :status \
:= dbms_pipe . send_message ( :sendPipe ) ; end ;";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )154;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlstm.sqhstv[0] = (         void  *)&sendValue;
    sqlstm.sqhstl[0] = (unsigned int  )4;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (         void  *)&status;
    sqlstm.sqhstl[1] = (unsigned int  )4;
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         void  *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned int  )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqhstv[2] = (         void  *)sendPipe;
    sqlstm.sqhstl[2] = (unsigned int  )0;
    sqlstm.sqhsts[2] = (         int  )0;
    sqlstm.sqindv[2] = (         void  *)0;
    sqlstm.sqinds[2] = (         int  )0;
    sqlstm.sqharm[2] = (unsigned int  )0;
    sqlstm.sqadto[2] = (unsigned short )0;
    sqlstm.sqtdso[2] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&ctx[i], &sqlctx, &sqlstm, &sqlfpn);
    if (sqlca.sqlcode < 0) SqlError("Fail to pipe.send_message",sqlca);
}



    if(status != 0 )
    {
//        cout << "fail to pipe.send_message" << endl;
        return false;
    }

    return true;
}

/*******************************************************************************
* Desc : 원하는 조건에 따라 여러개 또는 한개의 record를
*    fetch할 수 있다.

* Argu : type - 조회 조건의 종료
*               G:그룹명
*               N:프로세스명
*               A:프로세스 alias
*               E:모든 프로세스
*    strCondition - 조회조건
*******************************************************************************/
extern CPtrList procInfoList; /* running process info */

bool CPmsDbHandler::FetchProcessInfoFromDb(int i, char type, char *strCondition)
{
    /* EXEC SQL CONTEXT USE :ctx[i]; */ 

    struct sqlca sqlca;

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

        char query[2000];
        char condition[50];
        struct _ProcInfo
        {
            char    prog_name   [12+1]; // program name
            char    prog_path   [50+1]; // program path
            char    proc_name   [12+1]; // process name
            int     sys_id;             // system  id   uda1 or uda2
            int     svc_id;             // service id  1 or 2 or ...
            int     group_type;         // group type
            int     proc_type;          // process type
            int     proc_sn;            // process serial
            int     proc_remark;        // process remark
            int     ip_cnt;             // ip count
            int     channel_cnt;        // channel count
            char    p_ipaddr1   [16+1]; // IP ADDRESS 1
            int     p_send1;            // SEND 1
            int     p_recv1;            // RECIEVE 1
            char    p_ipaddr2   [16+1]; // IP ADDRESS 2
            int     p_send2;            // SEND 2
            int     p_recv2;            // RECV 2
            char    proc_status;        // process status S:Running, T:Stopped, K:killed, A:Abnormal
         char  net_status;       // network status E:Error network, R:regular network
            int     sys_pid;            // process pid
         char  proc_run_time[14+1]; // process run time
            int      runCnt;           // start cnt
         int     exitFlag;       // 0일경우 외부종료, 1일경우 PMS종료.
        } stProc[20];
    /* EXEC SQL END DECLARE SECTION; */ 


    int totCnt = 0;

    memset(query    , NULL, sizeof(query));
    memset(condition, NULL, sizeof(condition));

    sprintf(query,
    "select pi.prog_name, pi.prog_path, pi.proc_alias, \n\
                pi.sys_id, pi.svc_id, pi.group_type, pi.proc_type, pi.proc_sn, pi.proc_remark, \n\
                nvl(ni.ip_cnt, 0), nvl(ni.channel_cnt, 0), nvl(ni.p_ipaddr1, ' '), \n\
                nvl(ni.p_send1, 0), nvl(ni.p_recv1, 0), nvl(ni.p_ipaddr2, ' '), \n\
                nvl(ni.p_send2, 0), nvl(ni.p_recv2, 0),  \n\
                nvl(ps.proc_status, 'T'), nvl(ps.net_status, ' '), nvl(ps.sys_pid, 0),\n\
                nvl(ps.proc_run_time, '0'),\n\
                nvl(to_char(sysdate, ''), 0), nvl(to_char(sysdate, ''), 0) \n\
         from   t_process_info pi, t_ne_info ni, t_process_status ps \n\
         where  pi.sys_id = ps.sys_id and \n\
                pi.svc_id = ps.svc_id and  \n\
                pi.group_type = ps.group_type and \n\
                pi.proc_type = ps.proc_type and  \n\
                pi.proc_sn  = ps.proc_sn  and  \n\
                pi.proc_remark = ps.proc_remark and \n\
                pi.proc_alias = ps.proc_alias and  \n\
                ni.ne_id(+) = ps.proc_alias and \n\
                pi.sys_id = %d", g_sysId );

    switch(type)
    {
        case NAME:
            sprintf(condition, " and pi.proc_alias = '%s' ",
            strCondition);
            break;
        case ID:
            sprintf(condition, " and pi.proc_id = '%s' ",
            strCondition);
            break;
        case GROUP:
            sprintf(condition, " and pi.group_id = '%s' ",
            strCondition);
            break;
        case ALL:
            memset(condition, NULL, sizeof(condition));
            break;
        default:
            DebugLog(LOGFILE, "Wrong type for selecting process info");
    }

    strcat(query, condition);

//    DebugLog(LOGFILE, "%s", query);

    /* EXEC SQL WHENEVER SQLERROR do SqlError("Fail to Fetch data", sqlca); */ 

    /* EXEC SQL PREPARE QUERY FROM :query; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 5;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )181;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlstm.sqhstv[0] = (         void  *)query;
    sqlstm.sqhstl[0] = (unsigned int  )2000;
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
    sqlcxt(&ctx[i], &sqlctx, &sqlstm, &sqlfpn);
    if (sqlca.sqlcode < 0) SqlError("Fail to Fetch data",sqlca);
}


    /* EXEC SQL DECLARE Cur CURSOR FOR QUERY; */ 

    /* EXEC SQL OPEN  Cur; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 5;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )200;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlcxt(&ctx[i], &sqlctx, &sqlstm, &sqlfpn);
    if (sqlca.sqlcode < 0) SqlError("Fail to Fetch data",sqlca);
}



    /* EXEC SQL WHENEVER NOT FOUND do break; */ 


    for(;;)
    {
        /* EXEC SQL FETCH Cur INTO :stProc; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 10;
        sqlstm.arrsiz = 23;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )20;
        sqlstm.offset = (unsigned int  )215;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)0;
        sqlstm.sqhstv[0] = (         void  *)stProc->prog_name;
        sqlstm.sqhstl[0] = (unsigned int  )13;
        sqlstm.sqhsts[0] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqharc[0] = (unsigned int   *)0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)stProc->prog_path;
        sqlstm.sqhstl[1] = (unsigned int  )51;
        sqlstm.sqhsts[1] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqharc[1] = (unsigned int   *)0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)stProc->proc_name;
        sqlstm.sqhstl[2] = (unsigned int  )13;
        sqlstm.sqhsts[2] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqharc[2] = (unsigned int   *)0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)&stProc->sys_id;
        sqlstm.sqhstl[3] = (unsigned int  )4;
        sqlstm.sqhsts[3] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqharc[3] = (unsigned int   *)0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)&stProc->svc_id;
        sqlstm.sqhstl[4] = (unsigned int  )4;
        sqlstm.sqhsts[4] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqharc[4] = (unsigned int   *)0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)&stProc->group_type;
        sqlstm.sqhstl[5] = (unsigned int  )4;
        sqlstm.sqhsts[5] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqharc[5] = (unsigned int   *)0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)&stProc->proc_type;
        sqlstm.sqhstl[6] = (unsigned int  )4;
        sqlstm.sqhsts[6] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqharc[6] = (unsigned int   *)0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)&stProc->proc_sn;
        sqlstm.sqhstl[7] = (unsigned int  )4;
        sqlstm.sqhsts[7] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqharc[7] = (unsigned int   *)0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)&stProc->proc_remark;
        sqlstm.sqhstl[8] = (unsigned int  )4;
        sqlstm.sqhsts[8] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqharc[8] = (unsigned int   *)0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)&stProc->ip_cnt;
        sqlstm.sqhstl[9] = (unsigned int  )4;
        sqlstm.sqhsts[9] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqharc[9] = (unsigned int   *)0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)&stProc->channel_cnt;
        sqlstm.sqhstl[10] = (unsigned int  )4;
        sqlstm.sqhsts[10] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqharc[10] = (unsigned int   *)0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
        sqlstm.sqhstv[11] = (         void  *)stProc->p_ipaddr1;
        sqlstm.sqhstl[11] = (unsigned int  )17;
        sqlstm.sqhsts[11] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[11] = (         void  *)0;
        sqlstm.sqinds[11] = (         int  )0;
        sqlstm.sqharm[11] = (unsigned int  )0;
        sqlstm.sqharc[11] = (unsigned int   *)0;
        sqlstm.sqadto[11] = (unsigned short )0;
        sqlstm.sqtdso[11] = (unsigned short )0;
        sqlstm.sqhstv[12] = (         void  *)&stProc->p_send1;
        sqlstm.sqhstl[12] = (unsigned int  )4;
        sqlstm.sqhsts[12] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[12] = (         void  *)0;
        sqlstm.sqinds[12] = (         int  )0;
        sqlstm.sqharm[12] = (unsigned int  )0;
        sqlstm.sqharc[12] = (unsigned int   *)0;
        sqlstm.sqadto[12] = (unsigned short )0;
        sqlstm.sqtdso[12] = (unsigned short )0;
        sqlstm.sqhstv[13] = (         void  *)&stProc->p_recv1;
        sqlstm.sqhstl[13] = (unsigned int  )4;
        sqlstm.sqhsts[13] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[13] = (         void  *)0;
        sqlstm.sqinds[13] = (         int  )0;
        sqlstm.sqharm[13] = (unsigned int  )0;
        sqlstm.sqharc[13] = (unsigned int   *)0;
        sqlstm.sqadto[13] = (unsigned short )0;
        sqlstm.sqtdso[13] = (unsigned short )0;
        sqlstm.sqhstv[14] = (         void  *)stProc->p_ipaddr2;
        sqlstm.sqhstl[14] = (unsigned int  )17;
        sqlstm.sqhsts[14] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[14] = (         void  *)0;
        sqlstm.sqinds[14] = (         int  )0;
        sqlstm.sqharm[14] = (unsigned int  )0;
        sqlstm.sqharc[14] = (unsigned int   *)0;
        sqlstm.sqadto[14] = (unsigned short )0;
        sqlstm.sqtdso[14] = (unsigned short )0;
        sqlstm.sqhstv[15] = (         void  *)&stProc->p_send2;
        sqlstm.sqhstl[15] = (unsigned int  )4;
        sqlstm.sqhsts[15] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[15] = (         void  *)0;
        sqlstm.sqinds[15] = (         int  )0;
        sqlstm.sqharm[15] = (unsigned int  )0;
        sqlstm.sqharc[15] = (unsigned int   *)0;
        sqlstm.sqadto[15] = (unsigned short )0;
        sqlstm.sqtdso[15] = (unsigned short )0;
        sqlstm.sqhstv[16] = (         void  *)&stProc->p_recv2;
        sqlstm.sqhstl[16] = (unsigned int  )4;
        sqlstm.sqhsts[16] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[16] = (         void  *)0;
        sqlstm.sqinds[16] = (         int  )0;
        sqlstm.sqharm[16] = (unsigned int  )0;
        sqlstm.sqharc[16] = (unsigned int   *)0;
        sqlstm.sqadto[16] = (unsigned short )0;
        sqlstm.sqtdso[16] = (unsigned short )0;
        sqlstm.sqhstv[17] = (         void  *)&stProc->proc_status;
        sqlstm.sqhstl[17] = (unsigned int  )1;
        sqlstm.sqhsts[17] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[17] = (         void  *)0;
        sqlstm.sqinds[17] = (         int  )0;
        sqlstm.sqharm[17] = (unsigned int  )0;
        sqlstm.sqharc[17] = (unsigned int   *)0;
        sqlstm.sqadto[17] = (unsigned short )0;
        sqlstm.sqtdso[17] = (unsigned short )0;
        sqlstm.sqhstv[18] = (         void  *)&stProc->net_status;
        sqlstm.sqhstl[18] = (unsigned int  )1;
        sqlstm.sqhsts[18] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[18] = (         void  *)0;
        sqlstm.sqinds[18] = (         int  )0;
        sqlstm.sqharm[18] = (unsigned int  )0;
        sqlstm.sqharc[18] = (unsigned int   *)0;
        sqlstm.sqadto[18] = (unsigned short )0;
        sqlstm.sqtdso[18] = (unsigned short )0;
        sqlstm.sqhstv[19] = (         void  *)&stProc->sys_pid;
        sqlstm.sqhstl[19] = (unsigned int  )4;
        sqlstm.sqhsts[19] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[19] = (         void  *)0;
        sqlstm.sqinds[19] = (         int  )0;
        sqlstm.sqharm[19] = (unsigned int  )0;
        sqlstm.sqharc[19] = (unsigned int   *)0;
        sqlstm.sqadto[19] = (unsigned short )0;
        sqlstm.sqtdso[19] = (unsigned short )0;
        sqlstm.sqhstv[20] = (         void  *)stProc->proc_run_time;
        sqlstm.sqhstl[20] = (unsigned int  )15;
        sqlstm.sqhsts[20] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[20] = (         void  *)0;
        sqlstm.sqinds[20] = (         int  )0;
        sqlstm.sqharm[20] = (unsigned int  )0;
        sqlstm.sqharc[20] = (unsigned int   *)0;
        sqlstm.sqadto[20] = (unsigned short )0;
        sqlstm.sqtdso[20] = (unsigned short )0;
        sqlstm.sqhstv[21] = (         void  *)&stProc->runCnt;
        sqlstm.sqhstl[21] = (unsigned int  )4;
        sqlstm.sqhsts[21] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[21] = (         void  *)0;
        sqlstm.sqinds[21] = (         int  )0;
        sqlstm.sqharm[21] = (unsigned int  )0;
        sqlstm.sqharc[21] = (unsigned int   *)0;
        sqlstm.sqadto[21] = (unsigned short )0;
        sqlstm.sqtdso[21] = (unsigned short )0;
        sqlstm.sqhstv[22] = (         void  *)&stProc->exitFlag;
        sqlstm.sqhstl[22] = (unsigned int  )4;
        sqlstm.sqhsts[22] = (         int  )sizeof(struct _ProcInfo);
        sqlstm.sqindv[22] = (         void  *)0;
        sqlstm.sqinds[22] = (         int  )0;
        sqlstm.sqharm[22] = (unsigned int  )0;
        sqlstm.sqharc[22] = (unsigned int   *)0;
        sqlstm.sqadto[22] = (unsigned short )0;
        sqlstm.sqtdso[22] = (unsigned short )0;
        sqlstm.sqphsv = sqlstm.sqhstv;
        sqlstm.sqphsl = sqlstm.sqhstl;
        sqlstm.sqphss = sqlstm.sqhsts;
        sqlstm.sqpind = sqlstm.sqindv;
        sqlstm.sqpins = sqlstm.sqinds;
        sqlstm.sqparm = sqlstm.sqharm;
        sqlstm.sqparc = sqlstm.sqharc;
        sqlstm.sqpadto = sqlstm.sqadto;
        sqlstm.sqptdso = sqlstm.sqtdso;
        sqlcxt(&ctx[i], &sqlctx, &sqlstm, &sqlfpn);
        if (sqlca.sqlcode == 1403) break;
        if (sqlca.sqlcode < 0) SqlError("Fail to Fetch data",sqlca);
}



        AddProcList((PPROCINFO )stProc, sqlca.sqlerrd[2] - totCnt);

        totCnt = sqlca.sqlerrd[2];

//        DebugLog(LOGFILE, "sqlca = %d, totCnt = %d", sqlca.sqlerrd[2], totCnt);
    }

    if ((sqlca.sqlerrd[2] - totCnt) > 0)
        AddProcList((PPROCINFO)stProc, sqlca.sqlerrd[2] - totCnt);
    else
    if (totCnt == 0)
        SqlError("No data in t_process_status table", sqlca);

    /* EXEC SQL WHENEVER NOT FOUND CONTINUE; */ 

    /* EXEC SQL CLOSE Cur; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 23;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )322;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlcxt(&ctx[i], &sqlctx, &sqlstm, &sqlfpn);
    if (sqlca.sqlcode < 0) SqlError("Fail to Fetch data",sqlca);
}



    DebugLog(LOGFILE, "List count : (%d)", procInfoList.GetCount());

    return true;
}

/*******************************************************************************
* Desc : table로부터 fetch한 정보들을 linked list에 추가.
* Argu : 1. pProcInfo - fetch한 record 구조체 배열
*        2. cnt       - 배열에 insert되어진 레코드 수.
* 주의사항 : strtok를 써서 반드시 space를 null로 바꿔주어야 한다.
*******************************************************************************/
void CPmsDbHandler::AddProcList(PPROCINFO pProcInfo, int cnt)
{
    PPROCINFO pNewNodeData;

    for(int i=0; i < cnt; i++)
    {
        strtok(pProcInfo[i].prog_name     , " ");
        strtok(pProcInfo[i].prog_path     , " ");
        strtok(pProcInfo[i].proc_name     , " ");
        strtok(pProcInfo[i].p_ipaddr1     , " ");
        strtok(pProcInfo[i].p_ipaddr2     , " ");
        strtok(pProcInfo[i].proc_run_time, " ");

        if(IsNewData(pProcInfo[i]) == false)
            continue;

        pNewNodeData = (PPROCINFO)malloc(sizeof(PROCINFO));

        DebugLog(LOGFILE, "");
        DebugLog(LOGFILE, "@[Create process list (%2d)]@@@@@@@@@@@@@@@@@@", i);
        DebugLog(LOGFILE, "  prog_name      = %s", pProcInfo[i].prog_name    );
        DebugLog(LOGFILE, "  prog_path      = %s", pProcInfo[i].prog_path    );
        DebugLog(LOGFILE, "  proc_name      = %s", pProcInfo[i].proc_name    );
        DebugLog(LOGFILE, "  sys_id         = %d", pProcInfo[i].sys_id       );
        DebugLog(LOGFILE, "  svc_id         = %d", pProcInfo[i].svc_id       );
        DebugLog(LOGFILE, "  group_type     = %d", pProcInfo[i].group_type   );
        DebugLog(LOGFILE, "  proc_type      = %d", pProcInfo[i].proc_type    );
        DebugLog(LOGFILE, "  proc_sn        = %d", pProcInfo[i].proc_sn      );
        DebugLog(LOGFILE, "  proc_remark    = %d", pProcInfo[i].proc_remark  );
        DebugLog(LOGFILE, "  ip_cnt         = %d", pProcInfo[i].ip_cnt       );
        DebugLog(LOGFILE, "  channel_cnt    = %d", pProcInfo[i].channel_cnt  );
        DebugLog(LOGFILE, "  p_ipaddr1      = %s", pProcInfo[i].p_ipaddr1    );
        DebugLog(LOGFILE, "  p_send1        = %d", pProcInfo[i].p_send1      );
        DebugLog(LOGFILE, "  p_recv1        = %d", pProcInfo[i].p_recv1      );
        DebugLog(LOGFILE, "  p_ipaddr2      = %s", pProcInfo[i].p_ipaddr2    );
        DebugLog(LOGFILE, "  p_send2        = %d", pProcInfo[i].p_send2      );
        DebugLog(LOGFILE, "  p_recv2        = %d", pProcInfo[i].p_recv2      );
        DebugLog(LOGFILE, "  proc_status    = %c", pProcInfo[i].proc_status  );
        DebugLog(LOGFILE, "  net_status     = %c", pProcInfo[i].net_status   );
        DebugLog(LOGFILE, "  sys_pid        = %d", pProcInfo[i].sys_pid      );
        DebugLog(LOGFILE, "  proc_run_time  = %s", pProcInfo[i].proc_run_time);
        DebugLog(LOGFILE, "  runCnt         = %d", pProcInfo[i].runCnt       );
        DebugLog(LOGFILE, "  exitFlag       = %d", pProcInfo[i].exitFlag     );
        DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"   );
      DebugLog(LOGFILE, "");

        *pNewNodeData = pProcInfo[i];
        procInfoList.AddTail(pNewNodeData);
    }

    DebugLog(LOGFILE,"List count %d", procInfoList.GetCount());
}


/*******************************************************************************
* return false : 1. 기존에 있는 데이타
*                2. list의 정보와 다를때..
* true  : 새로운 정보일경우 추가
*******************************************************************************/
bool CPmsDbHandler::IsNewData(PROCINFO nodeData)
{
    if (pthread_mutex_lock(&mutex))
        DebugLog(LOGFILE,"Can't lock mutex");

    CNode* pNode = procInfoList.GetHeadPosition();

    while (pNode)
    {
        PPROCINFO pData = (PPROCINFO)pNode->data;

        // 동일 id에 대해서 db와 list가 다른 정보를 가지고 있을경우
        // list의 정보를 변경한다.
        if (strcmp(pData->proc_name, nodeData.proc_name) == 0 )
        {
         strcpy(pData->prog_name, nodeData.prog_name);
         strcpy(pData->prog_path, nodeData.prog_path);
         strcpy(pData->p_ipaddr1, nodeData.p_ipaddr1);
         strcpy(pData->p_ipaddr2, nodeData.p_ipaddr2);
         strcpy(pData->proc_run_time, nodeData.proc_run_time);
         pData->sys_id          = nodeData.sys_id;
         pData->svc_id          = nodeData.svc_id;
         pData->group_type      = nodeData.group_type;
         pData->proc_type       = nodeData.proc_type;
         pData->proc_sn         = nodeData.proc_sn;
         pData->proc_remark     = nodeData.proc_remark;
         pData->ip_cnt          = nodeData.ip_cnt;
         pData->channel_cnt     = nodeData.channel_cnt;
         pData->p_send1         = nodeData.p_send1;
         pData->p_recv1         = nodeData.p_recv1;
         pData->p_send2         = nodeData.p_send2;
         pData->p_recv2         = nodeData.p_recv2;
         pData->proc_status     = nodeData.proc_status;
         pData->net_status      = nodeData.net_status;
         pData->sys_pid         = nodeData.sys_pid;

         if (pthread_mutex_unlock(&mutex))
            DebugLog(LOGFILE,"Can't unlock mutex");

         return false;
        }//end if
        pNode = procInfoList.GetNextPosition(pNode);
    }//end while

    if(pNode == NULL)
    {
        if (pthread_mutex_unlock(&mutex))
                DebugLog(LOGFILE,"Can't unlock mutex");
            return true;
    }
}

#if 0
bool CPmsDbHandler::IsNewData(PROCINFO nodeData)
{
    if (pthread_mutex_lock(&mutex))
        DebugLog(LOGFILE,"Can't lock mutex");

    CNode* pNode = procInfoList.GetHeadPosition();

    while (pNode)
    {
        PPROCINFO pData = (PPROCINFO)pNode->data;

        // 동일 id에 대해서 db와 list가 다른 정보를 가지고 있을경우
        // list의 정보를 변경한다.
        if (strcmp(pData->proc_name, nodeData.proc_name) == 0 )
        {
            if(pData->proc_status != nodeData.proc_status)
            {
                DebugLog(LOGFILE, "@@@@@@@@@@[Linked list]@@@@@@@@@@");
                DebugLog(LOGFILE, "  prog name   : %10.10s", pData->prog_name);
                DebugLog(LOGFILE, "  proc name   : %10.10s", pData->proc_name);
                DebugLog(LOGFILE, "  proc pid    : %10d"   , pData->sys_pid);
                DebugLog(LOGFILE, "  proc status : %10.10c", pData->proc_status);
            DebugLog(LOGFILE, "  runCnt      : %d"     , pData->runCnt);
                DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
                DebugLog(LOGFILE, "                 |                ");
                DebugLog(LOGFILE, "@@@@@@@@@@@@[DB list]@@@@@@@@@@@@");
                DebugLog(LOGFILE, "  prog name   : %10.10s", nodeData.prog_name);
                DebugLog(LOGFILE, "  proc name   : %10.10s", nodeData.proc_name);
                DebugLog(LOGFILE, "  proc pid    : %10d"   , nodeData.sys_pid);
                DebugLog(LOGFILE, "  proc status : %10.10c", nodeData.proc_status);
            DebugLog(LOGFILE, "  runCnt      : %d"     , nodeData.runCnt);
                DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");

                pData->sys_pid = nodeData.sys_pid;
                strcpy(pData->proc_name, nodeData.proc_name);

                pData->proc_status = nodeData.proc_status;
                strcpy(pData->prog_name, nodeData.prog_name);

                if (pthread_mutex_unlock(&mutex))
                    DebugLog(LOGFILE,"Can't unlock mutex");
                return false;
            }
            else
            {
                if (pthread_mutex_unlock(&mutex))
                    DebugLog(LOGFILE,"Can't unlock mutex");
                return false;
            }//end if
        }//end if
        pNode = procInfoList.GetNextPosition(pNode);
    }//end while

    if(pNode == NULL)
    {
        if (pthread_mutex_unlock(&mutex))
                DebugLog(LOGFILE,"Can't unlock mutex");
            return true;
    }
}
#endif
/*******************************************************************************
* Desc : PMS업무 처리를 위한 메소드
*        config file정보를 DB로 부터 조회해 온다.
*******************************************************************************/
void CPmsDbHandler::_SelectCfgFile(command_t cfgFileInfo, char *f_path, char *f_name, int &i_count, char *f_id)
{
    /* EXEC SQL CONTEXT USE :ctx[0]; */ 


   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      char  proc_alias  [ 8+1];
      int      file_part;
      char  file_path   [50+1];
      char  file_name   [30+1];
      int      item_count;
      char  file_id     [ 4+1];
   /* EXEC SQL END   DECLARE SECTION; */ 


   memset(proc_alias, 0x00, sizeof(proc_alias));
   strcpy(proc_alias, cfgFileInfo.subContent);

   char conv[2];
   sprintf(conv, "%c", cfgFileInfo.filePart);
   file_part = atoi(conv);

   /* EXEC SQL SELECT FILE_PATH, FILE_NAME, ITEM_COUNT, FILE_ID
                   INTO :file_path, :file_name, :item_count, :file_id
                  FROM  T_CFG_FILE
               WHERE PROC_ALIAS = :proc_alias and
                    FILE_PART  = :file_part; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 23;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select FILE_PATH ,FILE_NAME ,ITEM_COUNT ,FILE_ID into :b0\
,:b1,:b2,:b3  from T_CFG_FILE where (PROC_ALIAS=:b4 and FILE_PART=:b5)";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )337;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)0;
   sqlstm.sqhstv[0] = (         void  *)file_path;
   sqlstm.sqhstl[0] = (unsigned int  )51;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)file_name;
   sqlstm.sqhstl[1] = (unsigned int  )31;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         void  *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned int  )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (         void  *)&item_count;
   sqlstm.sqhstl[2] = (unsigned int  )4;
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         void  *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned int  )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (         void  *)file_id;
   sqlstm.sqhstl[3] = (unsigned int  )5;
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         void  *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned int  )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (         void  *)proc_alias;
   sqlstm.sqhstl[4] = (unsigned int  )9;
   sqlstm.sqhsts[4] = (         int  )0;
   sqlstm.sqindv[4] = (         void  *)0;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned int  )0;
   sqlstm.sqadto[4] = (unsigned short )0;
   sqlstm.sqtdso[4] = (unsigned short )0;
   sqlstm.sqhstv[5] = (         void  *)&file_part;
   sqlstm.sqhstl[5] = (unsigned int  )4;
   sqlstm.sqhsts[5] = (         int  )0;
   sqlstm.sqindv[5] = (         void  *)0;
   sqlstm.sqinds[5] = (         int  )0;
   sqlstm.sqharm[5] = (unsigned int  )0;
   sqlstm.sqadto[5] = (unsigned short )0;
   sqlstm.sqtdso[5] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&ctx[0], &sqlctx, &sqlstm, &sqlfpn);
   if (sqlca.sqlcode < 0) SqlError("Fail to Fetch data",sqlca);
}



   if (sqlca.sqlcode != 0)
   {
      DebugLog(LOGFILE, "T_CFG_FILE select error - %s", sqlca.sqlerrm.sqlerrmc);
      //error log (DB Select error)
      ErrorLog(7102, "T_CFG_FILE select error - %s", sqlca.sqlerrm.sqlerrmc);
   }

   i_count = item_count;
   strtok(file_path, " ");
   strtok(file_name, " ");
   strtok(file_id  , " ");
   strcpy(f_path, file_path);
   strcpy(f_name, file_name);
   strcpy(f_id  , file_id);
}

void CPmsDbHandler::_InsertCfgFile(command_t cfgFileInfo, int itemCnt, char flag)
{
   /* EXEC SQL CONTEXT USE :ctx[0]; */ 


   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      char  proc_alias  [ 8+1];
      unsigned int      file_part;
      char  date     [14+1];
      unsigned int      item1;
      unsigned int      item2;
      unsigned int      item3;
      unsigned int      item4;
      unsigned int      item5;
      unsigned int      item6;
      char  result;
   /* EXEC SQL END   DECLARE SECTION; */ 


   char conv[2];
   sprintf(conv, "%c", cfgFileInfo.filePart);

   strcpy(proc_alias, cfgFileInfo.subContent);
   file_part = atoi(conv);
   strcpy(date      , cfgFileInfo.date      );
   item1 = atoi(cfgFileInfo.item1);
   item2 = atoi(cfgFileInfo.item2);
   item3 = atoi(cfgFileInfo.item3);
   item4 = atoi(cfgFileInfo.item4);
   item5 = atoi(cfgFileInfo.item5);
   item6 = atoi(cfgFileInfo.item6);
   result = flag;

   switch(itemCnt)
   {
      case 1:
            /* EXEC SQL INSERT INTO T_CFG_FILE_DETAIL
                            ( PROC_ALIAS,  FILE_PART,  DATE_TIME,  ITEM1,  RESULT)
                      VALUES (:proc_alias, :file_part, :date     , :item1, :result); */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 23;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "insert into T_CFG_FILE_DETAIL(PROC_ALIAS,FILE_PA\
RT,DATE_TIME,ITEM1,RESULT) values (:b0,:b1,:b2,:b3,:b4)";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )376;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)0;
            sqlstm.sqhstv[0] = (         void  *)proc_alias;
            sqlstm.sqhstl[0] = (unsigned int  )9;
            sqlstm.sqhsts[0] = (         int  )0;
            sqlstm.sqindv[0] = (         void  *)0;
            sqlstm.sqinds[0] = (         int  )0;
            sqlstm.sqharm[0] = (unsigned int  )0;
            sqlstm.sqadto[0] = (unsigned short )0;
            sqlstm.sqtdso[0] = (unsigned short )0;
            sqlstm.sqhstv[1] = (         void  *)&file_part;
            sqlstm.sqhstl[1] = (unsigned int  )4;
            sqlstm.sqhsts[1] = (         int  )0;
            sqlstm.sqindv[1] = (         void  *)0;
            sqlstm.sqinds[1] = (         int  )0;
            sqlstm.sqharm[1] = (unsigned int  )0;
            sqlstm.sqadto[1] = (unsigned short )0;
            sqlstm.sqtdso[1] = (unsigned short )0;
            sqlstm.sqhstv[2] = (         void  *)date;
            sqlstm.sqhstl[2] = (unsigned int  )15;
            sqlstm.sqhsts[2] = (         int  )0;
            sqlstm.sqindv[2] = (         void  *)0;
            sqlstm.sqinds[2] = (         int  )0;
            sqlstm.sqharm[2] = (unsigned int  )0;
            sqlstm.sqadto[2] = (unsigned short )0;
            sqlstm.sqtdso[2] = (unsigned short )0;
            sqlstm.sqhstv[3] = (         void  *)&item1;
            sqlstm.sqhstl[3] = (unsigned int  )4;
            sqlstm.sqhsts[3] = (         int  )0;
            sqlstm.sqindv[3] = (         void  *)0;
            sqlstm.sqinds[3] = (         int  )0;
            sqlstm.sqharm[3] = (unsigned int  )0;
            sqlstm.sqadto[3] = (unsigned short )0;
            sqlstm.sqtdso[3] = (unsigned short )0;
            sqlstm.sqhstv[4] = (         void  *)&result;
            sqlstm.sqhstl[4] = (unsigned int  )1;
            sqlstm.sqhsts[4] = (         int  )0;
            sqlstm.sqindv[4] = (         void  *)0;
            sqlstm.sqinds[4] = (         int  )0;
            sqlstm.sqharm[4] = (unsigned int  )0;
            sqlstm.sqadto[4] = (unsigned short )0;
            sqlstm.sqtdso[4] = (unsigned short )0;
            sqlstm.sqphsv = sqlstm.sqhstv;
            sqlstm.sqphsl = sqlstm.sqhstl;
            sqlstm.sqphss = sqlstm.sqhsts;
            sqlstm.sqpind = sqlstm.sqindv;
            sqlstm.sqpins = sqlstm.sqinds;
            sqlstm.sqparm = sqlstm.sqharm;
            sqlstm.sqparc = sqlstm.sqharc;
            sqlstm.sqpadto = sqlstm.sqadto;
            sqlstm.sqptdso = sqlstm.sqtdso;
            sqlcxt(&ctx[0], &sqlctx, &sqlstm, &sqlfpn);
            if (sqlca.sqlcode < 0) SqlError("Fail to Fetch data",sqlca);
}


               break;
      case 2:
            /* EXEC SQL INSERT INTO T_CFG_FILE_DETAIL
                            ( PROC_ALIAS,  FILE_PART,  DATE_TIME,  ITEM1,  ITEM2,  RESULT)
                      VALUES (:proc_alias, :file_part, :date     , :item1, :item2, :result); */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 23;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "insert into T_CFG_FILE_DETAIL(PROC_ALIAS,FILE_PA\
RT,DATE_TIME,ITEM1,ITEM2,RESULT) values (:b0,:b1,:b2,:b3,:b4,:b5)";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )411;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)0;
            sqlstm.sqhstv[0] = (         void  *)proc_alias;
            sqlstm.sqhstl[0] = (unsigned int  )9;
            sqlstm.sqhsts[0] = (         int  )0;
            sqlstm.sqindv[0] = (         void  *)0;
            sqlstm.sqinds[0] = (         int  )0;
            sqlstm.sqharm[0] = (unsigned int  )0;
            sqlstm.sqadto[0] = (unsigned short )0;
            sqlstm.sqtdso[0] = (unsigned short )0;
            sqlstm.sqhstv[1] = (         void  *)&file_part;
            sqlstm.sqhstl[1] = (unsigned int  )4;
            sqlstm.sqhsts[1] = (         int  )0;
            sqlstm.sqindv[1] = (         void  *)0;
            sqlstm.sqinds[1] = (         int  )0;
            sqlstm.sqharm[1] = (unsigned int  )0;
            sqlstm.sqadto[1] = (unsigned short )0;
            sqlstm.sqtdso[1] = (unsigned short )0;
            sqlstm.sqhstv[2] = (         void  *)date;
            sqlstm.sqhstl[2] = (unsigned int  )15;
            sqlstm.sqhsts[2] = (         int  )0;
            sqlstm.sqindv[2] = (         void  *)0;
            sqlstm.sqinds[2] = (         int  )0;
            sqlstm.sqharm[2] = (unsigned int  )0;
            sqlstm.sqadto[2] = (unsigned short )0;
            sqlstm.sqtdso[2] = (unsigned short )0;
            sqlstm.sqhstv[3] = (         void  *)&item1;
            sqlstm.sqhstl[3] = (unsigned int  )4;
            sqlstm.sqhsts[3] = (         int  )0;
            sqlstm.sqindv[3] = (         void  *)0;
            sqlstm.sqinds[3] = (         int  )0;
            sqlstm.sqharm[3] = (unsigned int  )0;
            sqlstm.sqadto[3] = (unsigned short )0;
            sqlstm.sqtdso[3] = (unsigned short )0;
            sqlstm.sqhstv[4] = (         void  *)&item2;
            sqlstm.sqhstl[4] = (unsigned int  )4;
            sqlstm.sqhsts[4] = (         int  )0;
            sqlstm.sqindv[4] = (         void  *)0;
            sqlstm.sqinds[4] = (         int  )0;
            sqlstm.sqharm[4] = (unsigned int  )0;
            sqlstm.sqadto[4] = (unsigned short )0;
            sqlstm.sqtdso[4] = (unsigned short )0;
            sqlstm.sqhstv[5] = (         void  *)&result;
            sqlstm.sqhstl[5] = (unsigned int  )1;
            sqlstm.sqhsts[5] = (         int  )0;
            sqlstm.sqindv[5] = (         void  *)0;
            sqlstm.sqinds[5] = (         int  )0;
            sqlstm.sqharm[5] = (unsigned int  )0;
            sqlstm.sqadto[5] = (unsigned short )0;
            sqlstm.sqtdso[5] = (unsigned short )0;
            sqlstm.sqphsv = sqlstm.sqhstv;
            sqlstm.sqphsl = sqlstm.sqhstl;
            sqlstm.sqphss = sqlstm.sqhsts;
            sqlstm.sqpind = sqlstm.sqindv;
            sqlstm.sqpins = sqlstm.sqinds;
            sqlstm.sqparm = sqlstm.sqharm;
            sqlstm.sqparc = sqlstm.sqharc;
            sqlstm.sqpadto = sqlstm.sqadto;
            sqlstm.sqptdso = sqlstm.sqtdso;
            sqlcxt(&ctx[0], &sqlctx, &sqlstm, &sqlfpn);
            if (sqlca.sqlcode < 0) SqlError("Fail to Fetch data",sqlca);
}


               break;
      case 3:
            /* EXEC SQL INSERT INTO T_CFG_FILE_DETAIL
                            ( PROC_ALIAS,  FILE_PART,  DATE_TIME,  ITEM1,  ITEM2,  ITEM3,  RESULT)
                      VALUES (:proc_alias, :file_part, :date     , :item1, :item2, :item3, :result); */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 23;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "insert into T_CFG_FILE_DETAIL(PROC_ALIAS,FILE_PA\
RT,DATE_TIME,ITEM1,ITEM2,ITEM3,RESULT) values (:b0,:b1,:b2,:b3,:b4,:b5,:b6)";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )450;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)0;
            sqlstm.sqhstv[0] = (         void  *)proc_alias;
            sqlstm.sqhstl[0] = (unsigned int  )9;
            sqlstm.sqhsts[0] = (         int  )0;
            sqlstm.sqindv[0] = (         void  *)0;
            sqlstm.sqinds[0] = (         int  )0;
            sqlstm.sqharm[0] = (unsigned int  )0;
            sqlstm.sqadto[0] = (unsigned short )0;
            sqlstm.sqtdso[0] = (unsigned short )0;
            sqlstm.sqhstv[1] = (         void  *)&file_part;
            sqlstm.sqhstl[1] = (unsigned int  )4;
            sqlstm.sqhsts[1] = (         int  )0;
            sqlstm.sqindv[1] = (         void  *)0;
            sqlstm.sqinds[1] = (         int  )0;
            sqlstm.sqharm[1] = (unsigned int  )0;
            sqlstm.sqadto[1] = (unsigned short )0;
            sqlstm.sqtdso[1] = (unsigned short )0;
            sqlstm.sqhstv[2] = (         void  *)date;
            sqlstm.sqhstl[2] = (unsigned int  )15;
            sqlstm.sqhsts[2] = (         int  )0;
            sqlstm.sqindv[2] = (         void  *)0;
            sqlstm.sqinds[2] = (         int  )0;
            sqlstm.sqharm[2] = (unsigned int  )0;
            sqlstm.sqadto[2] = (unsigned short )0;
            sqlstm.sqtdso[2] = (unsigned short )0;
            sqlstm.sqhstv[3] = (         void  *)&item1;
            sqlstm.sqhstl[3] = (unsigned int  )4;
            sqlstm.sqhsts[3] = (         int  )0;
            sqlstm.sqindv[3] = (         void  *)0;
            sqlstm.sqinds[3] = (         int  )0;
            sqlstm.sqharm[3] = (unsigned int  )0;
            sqlstm.sqadto[3] = (unsigned short )0;
            sqlstm.sqtdso[3] = (unsigned short )0;
            sqlstm.sqhstv[4] = (         void  *)&item2;
            sqlstm.sqhstl[4] = (unsigned int  )4;
            sqlstm.sqhsts[4] = (         int  )0;
            sqlstm.sqindv[4] = (         void  *)0;
            sqlstm.sqinds[4] = (         int  )0;
            sqlstm.sqharm[4] = (unsigned int  )0;
            sqlstm.sqadto[4] = (unsigned short )0;
            sqlstm.sqtdso[4] = (unsigned short )0;
            sqlstm.sqhstv[5] = (         void  *)&item3;
            sqlstm.sqhstl[5] = (unsigned int  )4;
            sqlstm.sqhsts[5] = (         int  )0;
            sqlstm.sqindv[5] = (         void  *)0;
            sqlstm.sqinds[5] = (         int  )0;
            sqlstm.sqharm[5] = (unsigned int  )0;
            sqlstm.sqadto[5] = (unsigned short )0;
            sqlstm.sqtdso[5] = (unsigned short )0;
            sqlstm.sqhstv[6] = (         void  *)&result;
            sqlstm.sqhstl[6] = (unsigned int  )1;
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
            sqlcxt(&ctx[0], &sqlctx, &sqlstm, &sqlfpn);
            if (sqlca.sqlcode < 0) SqlError("Fail to Fetch data",sqlca);
}


               break;
      case 4:
            /* EXEC SQL INSERT INTO T_CFG_FILE_DETAIL
                            ( PROC_ALIAS,  FILE_PART,  DATE_TIME,  ITEM1,  ITEM2,  ITEM3,  ITEM4,  RESULT)
                      VALUES (:proc_alias, :file_part, :date     , :item1, :item2, :item3, :item4, :result); */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 23;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "insert into T_CFG_FILE_DETAIL(PROC_ALIAS,FILE_PA\
RT,DATE_TIME,ITEM1,ITEM2,ITEM3,ITEM4,RESULT) values (:b0,:b1,:b2,:b3,:b4,:b5,\
:b6,:b7)";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )493;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)0;
            sqlstm.sqhstv[0] = (         void  *)proc_alias;
            sqlstm.sqhstl[0] = (unsigned int  )9;
            sqlstm.sqhsts[0] = (         int  )0;
            sqlstm.sqindv[0] = (         void  *)0;
            sqlstm.sqinds[0] = (         int  )0;
            sqlstm.sqharm[0] = (unsigned int  )0;
            sqlstm.sqadto[0] = (unsigned short )0;
            sqlstm.sqtdso[0] = (unsigned short )0;
            sqlstm.sqhstv[1] = (         void  *)&file_part;
            sqlstm.sqhstl[1] = (unsigned int  )4;
            sqlstm.sqhsts[1] = (         int  )0;
            sqlstm.sqindv[1] = (         void  *)0;
            sqlstm.sqinds[1] = (         int  )0;
            sqlstm.sqharm[1] = (unsigned int  )0;
            sqlstm.sqadto[1] = (unsigned short )0;
            sqlstm.sqtdso[1] = (unsigned short )0;
            sqlstm.sqhstv[2] = (         void  *)date;
            sqlstm.sqhstl[2] = (unsigned int  )15;
            sqlstm.sqhsts[2] = (         int  )0;
            sqlstm.sqindv[2] = (         void  *)0;
            sqlstm.sqinds[2] = (         int  )0;
            sqlstm.sqharm[2] = (unsigned int  )0;
            sqlstm.sqadto[2] = (unsigned short )0;
            sqlstm.sqtdso[2] = (unsigned short )0;
            sqlstm.sqhstv[3] = (         void  *)&item1;
            sqlstm.sqhstl[3] = (unsigned int  )4;
            sqlstm.sqhsts[3] = (         int  )0;
            sqlstm.sqindv[3] = (         void  *)0;
            sqlstm.sqinds[3] = (         int  )0;
            sqlstm.sqharm[3] = (unsigned int  )0;
            sqlstm.sqadto[3] = (unsigned short )0;
            sqlstm.sqtdso[3] = (unsigned short )0;
            sqlstm.sqhstv[4] = (         void  *)&item2;
            sqlstm.sqhstl[4] = (unsigned int  )4;
            sqlstm.sqhsts[4] = (         int  )0;
            sqlstm.sqindv[4] = (         void  *)0;
            sqlstm.sqinds[4] = (         int  )0;
            sqlstm.sqharm[4] = (unsigned int  )0;
            sqlstm.sqadto[4] = (unsigned short )0;
            sqlstm.sqtdso[4] = (unsigned short )0;
            sqlstm.sqhstv[5] = (         void  *)&item3;
            sqlstm.sqhstl[5] = (unsigned int  )4;
            sqlstm.sqhsts[5] = (         int  )0;
            sqlstm.sqindv[5] = (         void  *)0;
            sqlstm.sqinds[5] = (         int  )0;
            sqlstm.sqharm[5] = (unsigned int  )0;
            sqlstm.sqadto[5] = (unsigned short )0;
            sqlstm.sqtdso[5] = (unsigned short )0;
            sqlstm.sqhstv[6] = (         void  *)&item4;
            sqlstm.sqhstl[6] = (unsigned int  )4;
            sqlstm.sqhsts[6] = (         int  )0;
            sqlstm.sqindv[6] = (         void  *)0;
            sqlstm.sqinds[6] = (         int  )0;
            sqlstm.sqharm[6] = (unsigned int  )0;
            sqlstm.sqadto[6] = (unsigned short )0;
            sqlstm.sqtdso[6] = (unsigned short )0;
            sqlstm.sqhstv[7] = (         void  *)&result;
            sqlstm.sqhstl[7] = (unsigned int  )1;
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
            sqlcxt(&ctx[0], &sqlctx, &sqlstm, &sqlfpn);
            if (sqlca.sqlcode < 0) SqlError("Fail to Fetch data",sqlca);
}


               break;
      case 5:
            /* EXEC SQL INSERT INTO T_CFG_FILE_DETAIL
                            ( PROC_ALIAS,  FILE_PART,  DATE_TIME,  ITEM1,  ITEM2,  ITEM3,  ITEM4,  ITEM5,  RESULT)
                      VALUES (:proc_alias, :file_part, :date     , :item1, :item2, :item3, :item4, :item5, :result); */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 23;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "insert into T_CFG_FILE_DETAIL(PROC_ALIAS,FILE_PA\
RT,DATE_TIME,ITEM1,ITEM2,ITEM3,ITEM4,ITEM5,RESULT) values (:b0,:b1,:b2,:b3,:b\
4,:b5,:b6,:b7,:b8)";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )540;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)0;
            sqlstm.sqhstv[0] = (         void  *)proc_alias;
            sqlstm.sqhstl[0] = (unsigned int  )9;
            sqlstm.sqhsts[0] = (         int  )0;
            sqlstm.sqindv[0] = (         void  *)0;
            sqlstm.sqinds[0] = (         int  )0;
            sqlstm.sqharm[0] = (unsigned int  )0;
            sqlstm.sqadto[0] = (unsigned short )0;
            sqlstm.sqtdso[0] = (unsigned short )0;
            sqlstm.sqhstv[1] = (         void  *)&file_part;
            sqlstm.sqhstl[1] = (unsigned int  )4;
            sqlstm.sqhsts[1] = (         int  )0;
            sqlstm.sqindv[1] = (         void  *)0;
            sqlstm.sqinds[1] = (         int  )0;
            sqlstm.sqharm[1] = (unsigned int  )0;
            sqlstm.sqadto[1] = (unsigned short )0;
            sqlstm.sqtdso[1] = (unsigned short )0;
            sqlstm.sqhstv[2] = (         void  *)date;
            sqlstm.sqhstl[2] = (unsigned int  )15;
            sqlstm.sqhsts[2] = (         int  )0;
            sqlstm.sqindv[2] = (         void  *)0;
            sqlstm.sqinds[2] = (         int  )0;
            sqlstm.sqharm[2] = (unsigned int  )0;
            sqlstm.sqadto[2] = (unsigned short )0;
            sqlstm.sqtdso[2] = (unsigned short )0;
            sqlstm.sqhstv[3] = (         void  *)&item1;
            sqlstm.sqhstl[3] = (unsigned int  )4;
            sqlstm.sqhsts[3] = (         int  )0;
            sqlstm.sqindv[3] = (         void  *)0;
            sqlstm.sqinds[3] = (         int  )0;
            sqlstm.sqharm[3] = (unsigned int  )0;
            sqlstm.sqadto[3] = (unsigned short )0;
            sqlstm.sqtdso[3] = (unsigned short )0;
            sqlstm.sqhstv[4] = (         void  *)&item2;
            sqlstm.sqhstl[4] = (unsigned int  )4;
            sqlstm.sqhsts[4] = (         int  )0;
            sqlstm.sqindv[4] = (         void  *)0;
            sqlstm.sqinds[4] = (         int  )0;
            sqlstm.sqharm[4] = (unsigned int  )0;
            sqlstm.sqadto[4] = (unsigned short )0;
            sqlstm.sqtdso[4] = (unsigned short )0;
            sqlstm.sqhstv[5] = (         void  *)&item3;
            sqlstm.sqhstl[5] = (unsigned int  )4;
            sqlstm.sqhsts[5] = (         int  )0;
            sqlstm.sqindv[5] = (         void  *)0;
            sqlstm.sqinds[5] = (         int  )0;
            sqlstm.sqharm[5] = (unsigned int  )0;
            sqlstm.sqadto[5] = (unsigned short )0;
            sqlstm.sqtdso[5] = (unsigned short )0;
            sqlstm.sqhstv[6] = (         void  *)&item4;
            sqlstm.sqhstl[6] = (unsigned int  )4;
            sqlstm.sqhsts[6] = (         int  )0;
            sqlstm.sqindv[6] = (         void  *)0;
            sqlstm.sqinds[6] = (         int  )0;
            sqlstm.sqharm[6] = (unsigned int  )0;
            sqlstm.sqadto[6] = (unsigned short )0;
            sqlstm.sqtdso[6] = (unsigned short )0;
            sqlstm.sqhstv[7] = (         void  *)&item5;
            sqlstm.sqhstl[7] = (unsigned int  )4;
            sqlstm.sqhsts[7] = (         int  )0;
            sqlstm.sqindv[7] = (         void  *)0;
            sqlstm.sqinds[7] = (         int  )0;
            sqlstm.sqharm[7] = (unsigned int  )0;
            sqlstm.sqadto[7] = (unsigned short )0;
            sqlstm.sqtdso[7] = (unsigned short )0;
            sqlstm.sqhstv[8] = (         void  *)&result;
            sqlstm.sqhstl[8] = (unsigned int  )1;
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
            sqlcxt(&ctx[0], &sqlctx, &sqlstm, &sqlfpn);
            if (sqlca.sqlcode < 0) SqlError("Fail to Fetch data",sqlca);
}


               break;
      case 6:
            /* EXEC SQL INSERT INTO T_CFG_FILE_DETAIL
                            ( PROC_ALIAS,  FILE_PART,  DATE_TIME,  ITEM1,  ITEM2,  ITEM3,  ITEM4,  ITEM5,  ITEM6,  RESULT)
                      VALUES (:proc_alias, :file_part, :date     , :item1, :item2, :item3, :item4, :item5, :item6, :result); */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 23;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "insert into T_CFG_FILE_DETAIL(PROC_ALIAS,FILE_PA\
RT,DATE_TIME,ITEM1,ITEM2,ITEM3,ITEM4,ITEM5,ITEM6,RESULT) values (:b0,:b1,:b2,\
:b3,:b4,:b5,:b6,:b7,:b8,:b9)";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )591;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)0;
            sqlstm.sqhstv[0] = (         void  *)proc_alias;
            sqlstm.sqhstl[0] = (unsigned int  )9;
            sqlstm.sqhsts[0] = (         int  )0;
            sqlstm.sqindv[0] = (         void  *)0;
            sqlstm.sqinds[0] = (         int  )0;
            sqlstm.sqharm[0] = (unsigned int  )0;
            sqlstm.sqadto[0] = (unsigned short )0;
            sqlstm.sqtdso[0] = (unsigned short )0;
            sqlstm.sqhstv[1] = (         void  *)&file_part;
            sqlstm.sqhstl[1] = (unsigned int  )4;
            sqlstm.sqhsts[1] = (         int  )0;
            sqlstm.sqindv[1] = (         void  *)0;
            sqlstm.sqinds[1] = (         int  )0;
            sqlstm.sqharm[1] = (unsigned int  )0;
            sqlstm.sqadto[1] = (unsigned short )0;
            sqlstm.sqtdso[1] = (unsigned short )0;
            sqlstm.sqhstv[2] = (         void  *)date;
            sqlstm.sqhstl[2] = (unsigned int  )15;
            sqlstm.sqhsts[2] = (         int  )0;
            sqlstm.sqindv[2] = (         void  *)0;
            sqlstm.sqinds[2] = (         int  )0;
            sqlstm.sqharm[2] = (unsigned int  )0;
            sqlstm.sqadto[2] = (unsigned short )0;
            sqlstm.sqtdso[2] = (unsigned short )0;
            sqlstm.sqhstv[3] = (         void  *)&item1;
            sqlstm.sqhstl[3] = (unsigned int  )4;
            sqlstm.sqhsts[3] = (         int  )0;
            sqlstm.sqindv[3] = (         void  *)0;
            sqlstm.sqinds[3] = (         int  )0;
            sqlstm.sqharm[3] = (unsigned int  )0;
            sqlstm.sqadto[3] = (unsigned short )0;
            sqlstm.sqtdso[3] = (unsigned short )0;
            sqlstm.sqhstv[4] = (         void  *)&item2;
            sqlstm.sqhstl[4] = (unsigned int  )4;
            sqlstm.sqhsts[4] = (         int  )0;
            sqlstm.sqindv[4] = (         void  *)0;
            sqlstm.sqinds[4] = (         int  )0;
            sqlstm.sqharm[4] = (unsigned int  )0;
            sqlstm.sqadto[4] = (unsigned short )0;
            sqlstm.sqtdso[4] = (unsigned short )0;
            sqlstm.sqhstv[5] = (         void  *)&item3;
            sqlstm.sqhstl[5] = (unsigned int  )4;
            sqlstm.sqhsts[5] = (         int  )0;
            sqlstm.sqindv[5] = (         void  *)0;
            sqlstm.sqinds[5] = (         int  )0;
            sqlstm.sqharm[5] = (unsigned int  )0;
            sqlstm.sqadto[5] = (unsigned short )0;
            sqlstm.sqtdso[5] = (unsigned short )0;
            sqlstm.sqhstv[6] = (         void  *)&item4;
            sqlstm.sqhstl[6] = (unsigned int  )4;
            sqlstm.sqhsts[6] = (         int  )0;
            sqlstm.sqindv[6] = (         void  *)0;
            sqlstm.sqinds[6] = (         int  )0;
            sqlstm.sqharm[6] = (unsigned int  )0;
            sqlstm.sqadto[6] = (unsigned short )0;
            sqlstm.sqtdso[6] = (unsigned short )0;
            sqlstm.sqhstv[7] = (         void  *)&item5;
            sqlstm.sqhstl[7] = (unsigned int  )4;
            sqlstm.sqhsts[7] = (         int  )0;
            sqlstm.sqindv[7] = (         void  *)0;
            sqlstm.sqinds[7] = (         int  )0;
            sqlstm.sqharm[7] = (unsigned int  )0;
            sqlstm.sqadto[7] = (unsigned short )0;
            sqlstm.sqtdso[7] = (unsigned short )0;
            sqlstm.sqhstv[8] = (         void  *)&item6;
            sqlstm.sqhstl[8] = (unsigned int  )4;
            sqlstm.sqhsts[8] = (         int  )0;
            sqlstm.sqindv[8] = (         void  *)0;
            sqlstm.sqinds[8] = (         int  )0;
            sqlstm.sqharm[8] = (unsigned int  )0;
            sqlstm.sqadto[8] = (unsigned short )0;
            sqlstm.sqtdso[8] = (unsigned short )0;
            sqlstm.sqhstv[9] = (         void  *)&result;
            sqlstm.sqhstl[9] = (unsigned int  )1;
            sqlstm.sqhsts[9] = (         int  )0;
            sqlstm.sqindv[9] = (         void  *)0;
            sqlstm.sqinds[9] = (         int  )0;
            sqlstm.sqharm[9] = (unsigned int  )0;
            sqlstm.sqadto[9] = (unsigned short )0;
            sqlstm.sqtdso[9] = (unsigned short )0;
            sqlstm.sqphsv = sqlstm.sqhstv;
            sqlstm.sqphsl = sqlstm.sqhstl;
            sqlstm.sqphss = sqlstm.sqhsts;
            sqlstm.sqpind = sqlstm.sqindv;
            sqlstm.sqpins = sqlstm.sqinds;
            sqlstm.sqparm = sqlstm.sqharm;
            sqlstm.sqparc = sqlstm.sqharc;
            sqlstm.sqpadto = sqlstm.sqadto;
            sqlstm.sqptdso = sqlstm.sqtdso;
            sqlcxt(&ctx[0], &sqlctx, &sqlstm, &sqlfpn);
            if (sqlca.sqlcode < 0) SqlError("Fail to Fetch data",sqlca);
}


               break;
      default:
         DebugLog(LOGFILE, "Wrong ITEM_COUNT. ITEM COUNT = %d", itemCnt);
         //error log (wrong item count)
         ErrorLog(7103, "wrong ITEM_COUNT. ITEM COUNT = %d", itemCnt);
   }

   if (sqlca.sqlcode != 0)
   {
      DebugLog(LOGFILE, "T_CFG_FILE_DETAIL Insert error - %s", sqlca.sqlerrm.sqlerrmc);
      //error log (DB Insert error)
      ErrorLog(7104, "T_CFG_FILE_DETAIL Insert error - %s", sqlca.sqlerrm.sqlerrmc);
   }

   /* EXEC SQL COMMIT WORK; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 23;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )646;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)0;
   sqlcxt(&ctx[0], &sqlctx, &sqlstm, &sqlfpn);
   if (sqlca.sqlcode < 0) SqlError("Fail to Fetch data",sqlca);
}


}

bool CPmsDbHandler::_SelectBackupInfo(BackInfo retInfo[100], int &retCnt)
{
    /* EXEC SQL CONTEXT USE :ctx[4]; */ 

   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      int  sys_id;            // system id (uda1, uda2)
      char backup_id [  5+1]; // backup id
      char srcDir     [100+1];
      char srcFile   [ 20+1]; // source       file path & name
      char dstDir     [100+1];
      char dstFile   [ 20+1]; // destination  file path & name
      int  bak_day;           // backup day
      int  bak_delday;        // backup delete day
      char bak_time  [6+1];      // backup time
   /* EXEC SQL END   DECLARE SECTION; */ 


   sys_id = g_sysId;

   /* EXEC SQL WHENEVER SQLERROR goto _sqlerr_; */ 

   /* EXEC SQL DECLARE selectbackupinfo CURSOR FOR
   select data_dir, file_name, bak_dir, bak_fname,
          bak_day, bak_delday, nvl(bak_time, ' ')
     from t_backup_info
    where sys_id = :sys_id; */ 


   /* EXEC SQL OPEN    selectbackupinfo; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 23;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = sq0016;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )661;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)0;
   sqlstm.sqhstv[0] = (         void  *)&sys_id;
   sqlstm.sqhstl[0] = (unsigned int  )4;
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
   sqlcxt(&ctx[4], &sqlctx, &sqlstm, &sqlfpn);
   if (sqlca.sqlcode < 0) goto _sqlerr_;
}



   int x;
   for(x = 0 ;;x++)
   {
      /* EXEC SQL FETCH selectbackupinfo INTO :srcDir,  :srcFile,
                                           :dstDir,  :dstFile,
                                           :bak_day, :bak_delday, :bak_time; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 23;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )680;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)0;
      sqlstm.sqhstv[0] = (         void  *)srcDir;
      sqlstm.sqhstl[0] = (unsigned int  )101;
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (         void  *)srcFile;
      sqlstm.sqhstl[1] = (unsigned int  )21;
      sqlstm.sqhsts[1] = (         int  )0;
      sqlstm.sqindv[1] = (         void  *)0;
      sqlstm.sqinds[1] = (         int  )0;
      sqlstm.sqharm[1] = (unsigned int  )0;
      sqlstm.sqadto[1] = (unsigned short )0;
      sqlstm.sqtdso[1] = (unsigned short )0;
      sqlstm.sqhstv[2] = (         void  *)dstDir;
      sqlstm.sqhstl[2] = (unsigned int  )101;
      sqlstm.sqhsts[2] = (         int  )0;
      sqlstm.sqindv[2] = (         void  *)0;
      sqlstm.sqinds[2] = (         int  )0;
      sqlstm.sqharm[2] = (unsigned int  )0;
      sqlstm.sqadto[2] = (unsigned short )0;
      sqlstm.sqtdso[2] = (unsigned short )0;
      sqlstm.sqhstv[3] = (         void  *)dstFile;
      sqlstm.sqhstl[3] = (unsigned int  )21;
      sqlstm.sqhsts[3] = (         int  )0;
      sqlstm.sqindv[3] = (         void  *)0;
      sqlstm.sqinds[3] = (         int  )0;
      sqlstm.sqharm[3] = (unsigned int  )0;
      sqlstm.sqadto[3] = (unsigned short )0;
      sqlstm.sqtdso[3] = (unsigned short )0;
      sqlstm.sqhstv[4] = (         void  *)&bak_day;
      sqlstm.sqhstl[4] = (unsigned int  )4;
      sqlstm.sqhsts[4] = (         int  )0;
      sqlstm.sqindv[4] = (         void  *)0;
      sqlstm.sqinds[4] = (         int  )0;
      sqlstm.sqharm[4] = (unsigned int  )0;
      sqlstm.sqadto[4] = (unsigned short )0;
      sqlstm.sqtdso[4] = (unsigned short )0;
      sqlstm.sqhstv[5] = (         void  *)&bak_delday;
      sqlstm.sqhstl[5] = (unsigned int  )4;
      sqlstm.sqhsts[5] = (         int  )0;
      sqlstm.sqindv[5] = (         void  *)0;
      sqlstm.sqinds[5] = (         int  )0;
      sqlstm.sqharm[5] = (unsigned int  )0;
      sqlstm.sqadto[5] = (unsigned short )0;
      sqlstm.sqtdso[5] = (unsigned short )0;
      sqlstm.sqhstv[6] = (         void  *)bak_time;
      sqlstm.sqhstl[6] = (unsigned int  )7;
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
      sqlcxt(&ctx[4], &sqlctx, &sqlstm, &sqlfpn);
      if (sqlca.sqlcode < 0) goto _sqlerr_;
}



      if (sqlca.sqlcode == 1403) break;

      strtok(backup_id, " ");
      strtok(srcDir   , " ");
      strtok(srcFile  , " ");
      strtok(dstDir   , " ");
      strtok(dstFile  , " ");
      strtok(bak_time , " ");

//    retInfo[x].sys_id       =   sys_id    ;
      retInfo[x].bak_day      =   bak_day   ;
      retInfo[x].bak_delday   =   bak_delday;
//    strcpy(retInfo[x].backup_id, backup_id);
      strcpy(retInfo[x].srcDir   , srcDir   );
      strcpy(retInfo[x].srcFile  , srcFile  );
      strcpy(retInfo[x].dstDir   , dstDir   );
      strcpy(retInfo[x].dstFile  , dstFile  );
      strcpy(retInfo[x].bak_time , bak_time );
   }

   retCnt = sqlca.sqlerrd[2];

   /* EXEC SQL CLOSE   selectbackupinfo; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 23;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )723;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)0;
   sqlcxt(&ctx[4], &sqlctx, &sqlstm, &sqlfpn);
   if (sqlca.sqlcode < 0) goto _sqlerr_;
}

                    // close

_sqlerr_:
   if (sqlca.sqlcode != 0 && sqlca.sqlcode != 1403)
   {
      DebugLog(LOGFILE, "sql message = %s\n", sqlca.sqlerrm.sqlerrmc);
      //error log (DB Select error)
      ErrorLog(7102, "sql message = %s\n", sqlca.sqlerrm.sqlerrmc);
      return false;
   }
}

/*****************************************************************************************
* Desc : PMS와 HA의 연동으로 시스템이 절체되었을 경우 Client에서 절체되었는지를 확인하기
*        위해 PMS는 기동시 자신의 호스트명을 T_PROCESS_STATUS의 NET_STATUS에 업데이트를 하면
*        Client에서 시스템명을 보고 절체를 확인 할 수 있다.
*        이 메소드는 단지 gethostname을 사용하여 현시스템명을 db로 업데이트 한다.
*        UDA1 : 1, UDA2 : 2
*                                                                 01-05-07 10:29오전 ky.
* Argu : int ctx    - context id
* Retn : bool true  - Update 성공.
*             false - Update 실패.
* etc. : none
*****************************************************************************************/
#define L_HOST_NUM_OFFSET 3      // hostname에서 번호 위치. (fix)
bool CPmsDbHandler::UpdateHostName(int ct)
{
    // 사용 context 선언
    //----------------------------------------------------------
    /* EXEC SQL CONTEXT USE :ctx[ct]; */ 


    /* EXEC SQL BEGIN DECLARE SECTION; */ 

       char pmsName [20];
       char hostName[20];
       char hostNum;
    /* EXEC SQL END   DECLARE SECTION; */ 


    strcpy(pmsName, PMSNAME);
    gethostname(hostName, sizeof(hostName));
    hostNum = hostName[L_HOST_NUM_OFFSET];

    /* EXEC SQL update t_process_status set net_status = :hostNum where proc_alias = :pmsName; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 23;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "update t_process_status  set net_status=:b0 where proc_a\
lias=:b1";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )738;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlstm.sqhstv[0] = (         void  *)&hostNum;
    sqlstm.sqhstl[0] = (unsigned int  )1;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (         void  *)pmsName;
    sqlstm.sqhstl[1] = (unsigned int  )20;
    sqlstm.sqhsts[1] = (         int  )0;
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
    sqlcxt(&ctx[ct], &sqlctx, &sqlstm, &sqlfpn);
    if (sqlca.sqlcode < 0) goto _sqlerr_;
}



    if (sqlca.sqlcode != 0) {
       /* EXEC SQL ROLLBACK WORK; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 10;
       sqlstm.arrsiz = 23;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )761;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)0;
       sqlcxt(&ctx[ct], &sqlctx, &sqlstm, &sqlfpn);
       if (sqlca.sqlcode < 0) goto _sqlerr_;
}


       DebugLog(LOGFILE, "    [SQL-ERROR] %s", sqlca.sqlerrm.sqlerrmc);
       DebugLog(LOGFILE, "      hostname update fail. hostname:%s, hostNum:%c, error line:%d", hostName, hostNum, __LINE__);
       return false;
    }
    else {
       /* EXEC SQL COMMIT WORK; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 10;
       sqlstm.arrsiz = 23;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )776;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)0;
       sqlcxt(&ctx[ct], &sqlctx, &sqlstm, &sqlfpn);
       if (sqlca.sqlcode < 0) goto _sqlerr_;
}


       DebugLog(LOGFILE, "      hostname : %s", hostName);
       return true;
    }

_sqlerr_:
    /* EXEC SQL ROLLBACK WORK; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 23;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )791;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlcxt(&ctx[ct], &sqlctx, &sqlstm, &sqlfpn);
    if (sqlca.sqlcode < 0) goto _sqlerr_;
}


    DebugLog(LOGFILE, "    [SQL-ERROR] %s", sqlca.sqlerrm.sqlerrmc);
    DebugLog(LOGFILE, "      hostname update fail. hostname:%s, hostNum:%c, error line:%d", hostName, hostNum, __LINE__);
    return false;
}//end UpdateHostName
#undef L_HOST_NUM_OFFSET         // define 해제.

/*******************************************************************************
* Desc : 프로세스의 상태가 변할때 table의 정보를 변경한다.
* Argu : 1. tmpId     - 상태가 변한 프로세스 명
*        2. tmpStatus - 변한 상태
*        3. stFlag    - 상태 플래그 (N : network status, 그외 process status)
*******************************************************************************/
bool CPmsDbHandler::UpdateProcStatusTable(int i, PROCINFO &nodeData, char stFlag)
{
    /* EXEC SQL CONTEXT USE :ctx[i]; */ 

    // struct sqlca sqlca;

    /* EXEC SQL BEGIN DECLARE SECTION; */ 

        char date       [ 8+1];
        char time       [ 6+1];
        char strDate    [14+1];
        char strQuery   [200];
        char strSubQuery[100];
    /* EXEC SQL END DECLARE SECTION; */ 


    CTime tm;

    memset(strQuery,    NULL, sizeof(strQuery));
    memset(strSubQuery, NULL, sizeof(strSubQuery));

    /* EXEC SQL WHENEVER SQLERROR GOTO _sqlerr_; */ 


    tm.Time_Display(date, 4);
    tm.Time_Display(time, 5);

    sprintf(strDate, "%s%s", date, time);

    DebugLog(LOGFILE, "@@@@@@@[Update %d]@@@@@@@", i);
    DebugLog(LOGFILE, "  id     : [%s]"          , nodeData.proc_name);
    DebugLog(LOGFILE, "  status : [%c]"          , nodeData.proc_status);
    DebugLog(LOGFILE, "  pid    : [%d]"          , nodeData.sys_pid);
   DebugLog(LOGFILE, "  runCnt : [%d]"          , nodeData.runCnt);
    DebugLog(LOGFILE, "  date   : [%s]"          , strDate);
    DebugLog(LOGFILE, "@@@@@@@@@@@@@@@@@@@@@@@@@");

    if (stFlag == 'N') // network status
   {
      sprintf(strQuery,
               "UPDATE t_process_status set net_status = '%c', sys_pid = %d ",
               nodeData.net_status, nodeData.sys_pid);
    }
   else
   {
      sprintf(strQuery,
            "UPDATE t_process_status set proc_status = '%c', sys_pid = %d ",
            nodeData.proc_status, nodeData.sys_pid);
    }

    switch(nodeData.proc_status)
    {
        case RUNNING:
            if (stFlag == 'P') // process status
         {
               sprintf(strSubQuery, ", proc_run_time = '%s'  ", strDate);
               sprintf(nodeData.proc_run_time, "%s", strDate);
            }
            break;
        case STOPPED:
        case ABNORMAL:
        case KILLED:
            sprintf(strSubQuery, ", proc_exit_time = '%s' ", strDate);
            break;
        case PAUSED:
            sprintf(strSubQuery, ", proc_stop_time = '%s' ", strDate);
            break;
        default:
            sprintf(strSubQuery,"");
    }
    strcat(strQuery, strSubQuery);

    sprintf(strSubQuery, " where rtrim(proc_alias,' ') = '%s' ",
            nodeData.proc_name);
    strcat(strQuery, strSubQuery);

   DebugLog(LOGFILE, "");
    DebugLog(LOGFILE, "┌cxt[%d]. Query = "   , i);
    DebugLog(LOGFILE, "└%s", strQuery);
   DebugLog(LOGFILE, "");

    /* EXEC SQL EXECUTE IMMEDIATE :strQuery; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 23;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )806;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlstm.sqhstv[0] = (         void  *)strQuery;
    sqlstm.sqhstl[0] = (unsigned int  )200;
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
    sqlcxt(&ctx[i], &sqlctx, &sqlstm, &sqlfpn);
    if (sqlca.sqlcode < 0) goto _sqlerr_;
}



    /* EXEC SQL COMMIT; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 23;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )825;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlcxt(&ctx[i], &sqlctx, &sqlstm, &sqlfpn);
    if (sqlca.sqlcode < 0) goto _sqlerr_;
}


    return true;

_sqlerr_:
    /* EXEC SQL ROLLBACK; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 23;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )840;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlcxt(&ctx[i], &sqlctx, &sqlstm, &sqlfpn);
    if (sqlca.sqlcode < 0) goto _sqlerr_;
}


    DebugLog(LOGFILE, "Fail to UPDATE t_process_status. Sql Error = %s", sqlca.sqlerrm.sqlerrmc);
    //error log (DB Update error)
    ErrorLog(7105, "Fail to UPDATE t_process_status. Sql Error = %s",
             sqlca.sqlerrm.sqlerrmc);
    return false;
}

/*******************************************************************************
* Desc : DB Multi Connect
* Argu : i         - context number
*        tmpid     - DB connection id
*        tmpPasswd - DB connection password
* Retn : true  - ok connect.
*        false - no connect.
*******************************************************************************/
bool CPmsDbHandler::ConnectDBMultiThread(int i, char *tmpId, char *tmpPasswd)
{
    /* EXEC SQL CONTEXT USE :ctx[i]; */ 


    /* EXEC SQL WHENEVER SQLERROR GOTO _sqlerr_; */ 


    /* EXEC SQL BEGIN DECLARE SECTION; */ 

        char* id   = tmpId;
        char* pass = tmpPasswd;
    /* EXEC SQL END DECLARE SECTION; */ 


    /* EXEC SQL CONNECT :id IDENTIFIED BY :pass; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 23;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )10;
    sqlstm.offset = (unsigned int  )855;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlstm.sqhstv[0] = (         void  *)id;
    sqlstm.sqhstl[0] = (unsigned int  )0;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (         void  *)pass;
    sqlstm.sqhstl[1] = (unsigned int  )0;
    sqlstm.sqhsts[1] = (         int  )0;
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
    sqlcxt(&ctx[i], &sqlctx, &sqlstm, &sqlfpn);
    if (sqlca.sqlcode < 0) goto _sqlerr_;
}



    return true;

_sqlerr_:
	 cout << "sqlmsg : " << sqlca.sqlerrm.sqlerrmc << endl;
    return false;
}

/*******************************************************************************
* Desc : DB disconnect
* Argu : i - context number
* Retn : true  - ok disconnect.
*        false - no disconnect.
*******************************************************************************/
bool CPmsDbHandler::DisconnectDBMultiThread(int i)
{
    /* EXEC SQL CONTEXT USE :ctx[i]; */ 

    struct sqlca sqlca;

    /* EXEC SQL WHENEVER SQLERROR GOTO _sqlerr_; */ 


    /* EXEC SQL COMMIT WORK RELEASE; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 23;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )886;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlcxt(&ctx[i], &sqlctx, &sqlstm, &sqlfpn);
    if (sqlca.sqlcode < 0) goto _sqlerr_;
}



    /* EXEC SQL CONTEXT FREE :ctx[i]; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 23;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )901;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlstm.sqhstv[0] = (         void  *)&ctx[i];
    sqlstm.sqhstl[0] = (unsigned int  )4;
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
    if (sqlca.sqlcode < 0) goto _sqlerr_;
}



    return true;

_sqlerr_:
    return false;
}



