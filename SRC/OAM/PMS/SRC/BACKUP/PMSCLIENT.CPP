
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
           char  filnam[13];
};
static const struct sqlcxp sqlfpn =
{
    12,
    "pmsClient.pc"
};


static unsigned long sqlctx = 321275;


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
            void  *sqhstv[4];
   unsigned int   sqhstl[4];
            int   sqhsts[4];
            void  *sqindv[4];
            int   sqinds[4];
   unsigned int   sqharm[4];
   unsigned int   *sqharc[4];
   unsigned short  sqadto[4];
   unsigned short  sqtdso[4];
} sqlstm = {10,4};

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
5,0,0,1,0,0,27,40,0,0,4,4,0,1,0,1,9,0,0,1,10,0,0,1,10,0,0,1,10,0,0,
36,0,0,2,136,0,6,68,0,0,2,2,0,1,0,1,97,0,0,2,3,0,0,
};


/******************************************************************************* 
* Database Pipes
* 12-14 Your Product Name/BookTitle as a Variable
* This example shows the Pro*C code that receives messages from the PLSQL_DEBUG
* pipe in the PL/SQL example above, and displays the messages.
* If the Pro*C session is run in a separate window,
* it can be used to display any messages that are sent to
* the debug procedure from a PL/SQL program executing in a separate session.
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>
#include <time.h>

/* EXEC SQL BEGIN DECLARE SECTION; */ 

    /* varchar username[20]; */ 
struct { unsigned short len; unsigned char arr[20]; } username;

    int status;
    int msg_length;
    char retval[2000];
    char cmd;
    char type;
    char subContent[10];
    char cmdd[87+1];
/* EXEC SQL END DECLARE SECTION; */ 


/* EXEC SQL INCLUDE sqlca;
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


void sql_error();
void CreateCommand(char command, char type, char* subContent, char* retCommand);

int main()
{
    /* prepare username */

    strcpy((char *)username.arr,"uda/uda");
    username.len = strlen((char *)username.arr);

    /* EXEC SQL WHENEVER SQLERROR GOTO _sqlerr_; */ 

    /* EXEC SQL CONNECT :username; */ 

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
    sqlstm.sqhstl[0] = (unsigned int  )22;
    sqlstm.sqhsts[0] = (         int  )22;
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


    printf("connect ok\n");

    /* start an endless loop to look for and print messages on the pipe */
    for (;;)
    {
        cout << "====================================================" << endl;
        cout << " 'X' is EXIT"                                         << endl;
        cout << " Command    : S(tart), T(erminate), O(peration)"      << endl;
        cout << " Type       : G(roup), O(ne), A(ll) | S(elete), U(pdate) | D(isk), C(pu)" << endl;
        cout << " SubContent : detail content "                        << endl;
        cout << "====================================================="<< endl <<endl;

        cout << "Command : ";
        cin >> cmd;
        if (cmd == 'X') return 0;   
        cout << cmd << endl;

        cout << "Type : ";
        cin  >> type;
        cout << type << endl;

        cout << "SubContent : ";
        cin  >> subContent;
        cout << subContent << endl;
       
        CreateCommand(cmd, type, subContent, cmdd);

        /* EXEC SQL EXECUTE
        DECLARE
            sta INTEGER;
        BEGIN
            dbms_pipe.pack_message(:cmdd);
//          dbms_pipe.pack_message(:cmd);
//          dbms_pipe.pack_message(:type);
//          dbms_pipe.pack_message(:subContent);
            sta := dbms_pipe.send_message('recvpipe');
            :status := sta;
        END;
        END-EXEC; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 10;
        sqlstm.arrsiz = 4;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "declare sta INTEGER ; BEGIN dbms_pipe . pack_message\
 ( :cmdd ) ; sta := dbms_pipe . send_message ( 'recvpipe' ) ; :status := sta \
; END ;";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )36;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)0;
        sqlstm.sqhstv[0] = (         void  *)cmdd;
        sqlstm.sqhstl[0] = (unsigned int  )88;
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



        cout << "cmd = " << cmdd << endl;

        if (status == 0)
            printf("success to send message \n");
        else
            printf("abnormal status, value is %d\n", status);
    }//end for
    
    return 0;

_sqlerr_:
    printf("ORACLE ERROR [%d]\n", sqlca.sqlcode);
    cout << cmd  <<endl;
    cout << type <<endl;
    cout << subContent<<endl;               
    return 0;
}

void sql_error()
{
    printf("ORACLE ERROR [%d]\n", sqlca.sqlcode);
    cout << cmd  << endl;
    cout << type << endl;
    cout << subContent << endl; 
    exit(1);
}

void CreateCommand(char command, char type, char* subContent, char* retCommand)
{
    char cTime[14+1]; 					   // 명령 일자/시간
    char fFlag = '3';   				   // 파일구분
    char fItem1[10+1] = { "         |" };  // 파일 ITEM 1
    char fItem2[10+1] = { "         |" };  // 파일 ITEM 2
    char fItem3[10+1] = { "         |" };  // 파일 ITEM 3
    char fItem4[10+1] = { "         |" };  // 파일 ITEM 4
    char fItem5[10+1] = { "         |" };  // 파일 ITEM 5
    char fItem6[10+1] = { "         |" };  // 파일 ITEM 6
    
    time_t t;       time(&t);
    struct tm *tp;  tp = localtime(&t);

    // 현재 날짜구함  (년월일시분초)
    sprintf(cTime, "%04d%02d%02d%02d%02d%02d",
            tp->tm_year+1900, tp->tm_mon+1, tp->tm_mday,
            tp->tm_hour, tp->tm_min, tp->tm_sec);
    
    switch(command)
    {
        // Start, Terminate
        case 'S':
        case 'T':
            break;
        
        // Operation
        case 'O':
            switch(type)
            {
                // Select, Update
                case 'S':
                case 'U':
                    fFlag = '3';
                    break;
                
                // Disk, Cpu
                case 'D':
                case 'C':
                    break;
                default:
                    printf("Worng Command !!");
            }
            break;
        default:
            printf("Worng Command !!");
    }
    
    sprintf(retCommand, "%c%c%-10.10s%s%c%s%s%s%s%s%s",
            command, type, subContent, cTime, fFlag,
            fItem1, fItem2, fItem3, fItem4, fItem5, fItem6);
            
    cout << " retCommand = " << retCommand << endl;
    cout << command << endl;
    cout << type << endl;
    cout << subContent << endl;
    cout << cTime << endl;
    cout << fFlag << endl;
    cout << fItem1 << endl;
    cout << fItem2 << endl;
    cout << fItem3 << endl;
    cout << fItem4 << endl;
    cout << fItem5 << endl;
    cout << fItem6 << endl;
}
