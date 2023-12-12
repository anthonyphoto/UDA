
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
    "t_meta_action.pc"
};


static unsigned long sqlctx = 5544859;


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
            void  *sqhstv[12];
   unsigned int   sqhstl[12];
            int   sqhsts[12];
            void  *sqindv[12];
            int   sqinds[12];
   unsigned int   sqharm[12];
   unsigned int   *sqharc[12];
   unsigned short  sqadto[12];
   unsigned short  sqtdso[12];
} sqlstm = {10,12};

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

 static const char *sq0002 = 
"select MAPPING_NAME ,VER ,nvl(ACTION_CODE,' ') ,nvl(SUB_ACTION_CODE,' ') ,nv\
l(DATA_TYPE,' ') ,nvl(IN_FIELD_NAME,' ') ,nvl(OUT_FIELD_NAME,' ') ,SEQ_NO ,nvl\
(OUT_FIELD_LENGTH,0) ,nvl(OUT_FIELD_OFFSET,0) ,nvl(IN_FIELD_LENGTH,0) ,nvl(IN_\
FIELD_OFFSET,0)  from T_META_ACTION where (MAPPING_NAME=:b0 and VER=:b1) order\
 by MAPPING_NAME,VER,SEQ_NO            ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{10,4130,0,0,0,
5,0,0,1,82,0,4,106,0,0,3,2,0,1,0,2,3,0,0,1,97,0,0,1,97,0,0,
32,0,0,2,349,0,9,159,0,0,2,2,0,1,0,1,97,0,0,1,97,0,0,
55,0,0,2,0,0,15,173,0,0,0,0,0,1,0,
70,0,0,2,0,0,13,185,0,0,12,0,0,1,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,9,0,0,2,9,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,
};


/******************************************************************
* 파 일 명     
      : t_meta_action.pc                
* 기   능                                                          
     : [t_meta_action] table에서 Udr 형식을 정의한 데이타를 메모리로 load한다.
* 작 성 자     
     : 신미연                          
* first data  
     : 2001. 03. 21                      
* last updata 
     : 2000. 03. 29               
* 관 련 화 일  
     : t_meta_action.hpp                     
* program history                                
           
* Version
  : 1.1 ( 2001. 05. 21 )                                           
******************************************************************/


#define SQLCA_STORAGE_CLASS extern
/* EXEC SQL INCLUDE    SQLCA;
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
#define ORACA_STORAGE_CLASS extern
/* EXEC SQL INCLUDE    ORACA;
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

// DECLARE SECTION을 hpp에 선언할때
/* EXEC SQL INCLUDE "../INCLUDE/t_meta_action.hpp";
 */ 
#define SQLCA_STORAGE_CLASS extern
#include <sqlca.h>

#include <iostream.h>
#include <stdlib.h>
#include "../INCLUDE/SqlErr.hpp"

#ifndef	  OK
#define   OK        1
#define   FAIL      -1
#endif

#ifndef _CLOADACTTABLE_
#define _CLOADACTTABLE_

//----------------------------------------
// Declare Host Variable
/* EXEC SQL BEGIN     DECLARE SECTION; */ 


    typedef struct    __ActionTable
    {
        char        mapping_name    [8+1];
        char        Version         [5+1];
        char        Action_Code     [2+1];
        char        Sub_Action_Code [2+1];
        char        Data_Type       [2+1];
        /* VARCHAR     In_Field_Name   [30+1]; */ 
struct { unsigned short len; unsigned char arr[31]; } In_Field_Name;

        /* VARCHAR     Out_Field_Name  [30+1]; */ 
struct { unsigned short len; unsigned char arr[31]; } Out_Field_Name;

        int         Seq_No;
        int         Out_Field_Length;
        int         Out_Field_Offset;
        int         In_Field_Length; 
        int         In_Field_Offset;
    }ActionTable;

/* EXEC SQL END     DECLARE SECTION; */ 

//----------------------------------------



///////////////////////////////////
//		Class LoadActTable		 //
///////////////////////////////////

class    CLoadActTable : public SqlErr
{
    private:

    public:
        /* EXEC SQL BEGIN     DECLARE SECTION; */ 

            char          m_strMappingName[8+1];   // Mapping Name
            char          m_strMapVersion[5+1];    // Mapping Version
            ActionTable*  m_ActionTable;           // Action Table
            int           m_nActRecCount;          // Action Record Count
        /* EXEC SQL END     DECLARE SECTION; */ 


        int m_nActStartRecIndex;// Action Start Record Index

        CLoadActTable(char *strMappingName,char *strVer);
		~CLoadActTable();

        void init_ActionTable();// [t_clt_action] table의 레코드개수만큼 만들어진 배열을 초기화한다.

        void get_RecordCount()    throw(char *);// [t_clt_action] table에서 레코드 개수를 가져온다.
        void show_ActionTable();// [t_clt_action] Table의 내용을 Display하기 위한 함수

        void declare_Cursor()    throw(char *);
        void open_Cursor()       throw(char *);
        void close_Cursor();
        void fetch_Data()        throw(char *);
        
       	// [t_clt_action]table의 record count만큼 루프를 돌면서 Mapping Name, Mapping Version이
       	// 같은지 확인한 후 Sequence Number의 값이 0인 레코드의 시작 위치를 찾는다.
        int SearchActIndex();

};

#endif


/* EXEC ORACLE OPTION (ORACA=YES); */ 

//EXEC ORACLE OPTION (RELEASE_CURSOR=YES);

/* EXEC SQL BEGIN     DECLARE SECTION; */ 

char strMapName[8+1];
/* EXEC SQL END     DECLARE SECTION; */ 




/////////////////////////////////
// Class LoadActTable's Method //
/////////////////////////////////

//------------------------------------------------------------------------------
// 생성자
CLoadActTable::CLoadActTable(char *strMappingName,char *strVer)
{
    m_nActRecCount    = 0;

	memset(m_strMappingName, 0x00, sizeof(m_strMappingName));
	memset(m_strMapVersion , 0x00, sizeof(m_strMapVersion));
    strcpy(m_strMappingName,strMappingName);
    strcpy(m_strMapVersion ,strVer);

    try
    {
        get_RecordCount();	// GET Total Record Count
	    m_ActionTable = new ActionTable[m_nActRecCount+1]; // Allocate ActionTable Buffer

        init_ActionTable();	// Initialize Buffer
      
        declare_Cursor();	// OPEN Cursor

        open_Cursor();

	    fetch_Data();		// FETCH Data
    }
    catch(char *strTemp)
    {
        char strErrMsg[400];
        memset(strErrMsg , 0x00, 400);
        strcpy(strErrMsg,strTemp);
        SqlError(strTemp);
    }
	int nStartIdx = 0;
	if(nStartIdx  = SearchActIndex() < 0)		exit(-1);
	m_nActStartRecIndex = nStartIdx;
}




//------------------------------------------------------------------------------
CLoadActTable::~CLoadActTable()
{
	delete [] m_ActionTable;
}	




//------------------------------------------------------------------------------
// [t_meta_action] table의 레코드개수만큼 만들어진 배열을 초기화한다.
void CLoadActTable::init_ActionTable()
{
    int i;
    for(i = 0; i <= m_nActRecCount; i++)
    	memset(&m_ActionTable[i], 0x00, sizeof(ActionTable));
}




//------------------------------------------------------------------------------
// [t_meta_action] table에서 레코드 개수를 가져온다.
void CLoadActTable::get_RecordCount()    throw(char *)
{
	/* EXEC SQL SELECT COUNT(*)
    	     INTO  :m_nActRecCount
             FROM  T_META_ACTION
             WHERE MAPPING_NAME = :m_strMappingName AND
             	   VER          = :m_strMapVersion; */ 

{
 struct sqlexd sqlstm;
 sqlorat((void **)0, &sqlctx, &oraca);
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 3;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select count(*)  into :b0  from T_META_ACTION where (MAPPING\
_NAME=:b1 and VER=:b2)";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlstm.sqhstv[0] = (         void  *)&m_nActRecCount;
 sqlstm.sqhstl[0] = (unsigned int  )4;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)m_strMappingName;
 sqlstm.sqhstl[1] = (unsigned int  )9;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)m_strMapVersion;
 sqlstm.sqhstl[2] = (unsigned int  )6;
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
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


                  
    if (sqlca.sqlcode != 0){
   	 	strcpy(strSqlTemp,"!!!Error : [t_meta_action] table : Fail Get Record Count.");
  		throw (char *)strSqlTemp;
   	}	

}




//------------------------------------------------------------------------------
void CLoadActTable::declare_Cursor()    throw(char *)
{
	strcpy(strMapName,m_strMappingName);

    /* EXEC SQL DECLARE CUR_TABLE CURSOR FOR
                SELECT      MAPPING_NAME,    
                            VER,         
                            nvl(ACTION_CODE,' '),
                            nvl(SUB_ACTION_CODE,' '),
                            nvl(DATA_TYPE,' '),
                            nvl(IN_FIELD_NAME,' '),
                            nvl(OUT_FIELD_NAME,' '),
                            SEQ_NO,
                            nvl(OUT_FIELD_LENGTH,0),
                            nvl(OUT_FIELD_OFFSET,0),
                            nvl(IN_FIELD_LENGTH,0),
                            nvl(IN_FIELD_OFFSET,0)
                FROM        T_META_ACTION
                WHERE       MAPPING_NAME = :strMapName
                AND         VER          = :m_strMapVersion
                ORDER BY    MAPPING_NAME,
                            VER,
                            SEQ_NO; */ 


    if (sqlca.sqlcode != 0){
	    strcpy(strSqlTemp,"!!!Error : [t_meta_action] table : Fail Declare Cursor.");
        throw (char *)strSqlTemp;
    }    
}




//------------------------------------------------------------------------------
void CLoadActTable::open_Cursor()    throw(char *)
{
    /* EXEC SQL OPEN CUR_TABLE; */ 

{
    struct sqlexd sqlstm;
    sqlorat((void **)0, &sqlctx, &oraca);
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 3;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = sq0002;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )32;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlstm.sqhstv[0] = (         void  *)strMapName;
    sqlstm.sqhstl[0] = (unsigned int  )9;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (         void  *)m_strMapVersion;
    sqlstm.sqhstl[1] = (unsigned int  )6;
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
}



    if (sqlca.sqlcode != 0){
	    strcpy(strSqlTemp,"!!!Error : [t_meta_action] table : Fail Open Cursor.");
        throw (char *)strSqlTemp;
    }
}




//------------------------------------------------------------------------------
void CLoadActTable::close_Cursor()
{
    /* EXEC SQL CLOSE CUR_TABLE; */ 

{
    struct sqlexd sqlstm;
    sqlorat((void **)0, &sqlctx, &oraca);
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 3;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )55;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


}




//------------------------------------------------------------------------------
void CLoadActTable::fetch_Data()    throw(char *)
{
    
    for(int i = 0; i <= m_nActRecCount; i++)
    {
        /* EXEC SQL FETCH CUR_TABLE INTO :m_ActionTable[i]; */ 

{
        struct sqlexd sqlstm;
        sqlorat((void **)0, &sqlctx, &oraca);
        sqlstm.sqlvsn = 10;
        sqlstm.arrsiz = 12;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )70;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)0;
        sqlstm.sqhstv[0] = (         void  *)m_ActionTable[i].mapping_name;
        sqlstm.sqhstl[0] = (unsigned int  )9;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)m_ActionTable[i].Version;
        sqlstm.sqhstl[1] = (unsigned int  )6;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)m_ActionTable[i].Action_Code;
        sqlstm.sqhstl[2] = (unsigned int  )3;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)m_ActionTable[i].Sub_Action_Code;
        sqlstm.sqhstl[3] = (unsigned int  )3;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (         void  *)m_ActionTable[i].Data_Type;
        sqlstm.sqhstl[4] = (unsigned int  )3;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         void  *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned int  )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (         void  *)&m_ActionTable[i].In_Field_Name;
        sqlstm.sqhstl[5] = (unsigned int  )33;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         void  *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned int  )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (         void  *)&m_ActionTable[i].Out_Field_Name;
        sqlstm.sqhstl[6] = (unsigned int  )33;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         void  *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned int  )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqhstv[7] = (         void  *)&m_ActionTable[i].Seq_No;
        sqlstm.sqhstl[7] = (unsigned int  )4;
        sqlstm.sqhsts[7] = (         int  )0;
        sqlstm.sqindv[7] = (         void  *)0;
        sqlstm.sqinds[7] = (         int  )0;
        sqlstm.sqharm[7] = (unsigned int  )0;
        sqlstm.sqadto[7] = (unsigned short )0;
        sqlstm.sqtdso[7] = (unsigned short )0;
        sqlstm.sqhstv[8] = (         void  *)&m_ActionTable[i].Out_Field_Length;
        sqlstm.sqhstl[8] = (unsigned int  )4;
        sqlstm.sqhsts[8] = (         int  )0;
        sqlstm.sqindv[8] = (         void  *)0;
        sqlstm.sqinds[8] = (         int  )0;
        sqlstm.sqharm[8] = (unsigned int  )0;
        sqlstm.sqadto[8] = (unsigned short )0;
        sqlstm.sqtdso[8] = (unsigned short )0;
        sqlstm.sqhstv[9] = (         void  *)&m_ActionTable[i].Out_Field_Offset;
        sqlstm.sqhstl[9] = (unsigned int  )4;
        sqlstm.sqhsts[9] = (         int  )0;
        sqlstm.sqindv[9] = (         void  *)0;
        sqlstm.sqinds[9] = (         int  )0;
        sqlstm.sqharm[9] = (unsigned int  )0;
        sqlstm.sqadto[9] = (unsigned short )0;
        sqlstm.sqtdso[9] = (unsigned short )0;
        sqlstm.sqhstv[10] = (         void  *)&m_ActionTable[i].In_Field_Length;
        sqlstm.sqhstl[10] = (unsigned int  )4;
        sqlstm.sqhsts[10] = (         int  )0;
        sqlstm.sqindv[10] = (         void  *)0;
        sqlstm.sqinds[10] = (         int  )0;
        sqlstm.sqharm[10] = (unsigned int  )0;
        sqlstm.sqadto[10] = (unsigned short )0;
        sqlstm.sqtdso[10] = (unsigned short )0;
        sqlstm.sqhstv[11] = (         void  *)&m_ActionTable[i].In_Field_Offset;
        sqlstm.sqhstl[11] = (unsigned int  )4;
        sqlstm.sqhsts[11] = (         int  )0;
        sqlstm.sqindv[11] = (         void  *)0;
        sqlstm.sqinds[11] = (         int  )0;
        sqlstm.sqharm[11] = (unsigned int  )0;
        sqlstm.sqadto[11] = (unsigned short )0;
        sqlstm.sqtdso[11] = (unsigned short )0;
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



        // Type이 varchar2인 변수의 스트링 끝에 NULL 처리
        m_ActionTable[i].In_Field_Name.arr[m_ActionTable[i].In_Field_Name.len] = NULL;
        m_ActionTable[i].Out_Field_Name.arr[m_ActionTable[i].Out_Field_Name.len] = NULL;
        
        if(sqlca.sqlcode == 1403){
            close_Cursor();
            break;
        }
        else if(sqlca.sqlcode != 0){
            strcpy(strSqlTemp,"!!!Error : [t_meta_action] table : Fail Data fetch.");
            close_Cursor();
            throw (char *)strSqlTemp;
        }
    }
    
//    show_ActionTable();	// [t_meta_action] table Display

}




//------------------------------------------------------------------------------
// [t_meta_action]table의 record count만큼 루프를 돌면서 Mapping Name, Mapping Version이 같은지 확인한 후
// Sequence Number의 값이 0인 레코드의 시작 위치를 찾는다
int CLoadActTable::SearchActIndex()
{
	for(int i = 0; i < m_nActRecCount; i++){
//	 cout << " i=> " << i << endl;
		if(!strcmp(m_ActionTable[i].mapping_name,strMapName)){
//			if(!strcmp(m_ActionTable[i].Version,m_strMapVersion)){
			if(!strncmp(m_ActionTable[i].Version,m_strMapVersion, 1)){
				if(m_ActionTable[i].Seq_No==0){
//				    cout << " i=> " << i << endl;
				    return i;
				}
		    }
		}//outside for
	}//for
//    cout << "!!!Error : [t_meta_action] table : Sequence Number != 0" << endl;
    return FAIL;
}




//------------------------------------------------------------------------------
// [t_meta_action] Table의 내용을 Display하기 위한 함수
void CLoadActTable::show_ActionTable()
{
	cout << "########### T_META_ACTION Table ###########" << endl;
	cout << "All Record   Cnt           => " << m_nActRecCount << endl;

	cout << "==============================================================================================" << endl;
	cout << "SeqNo OutName Ver StFlag      FieldName                 SeqNo DataType FieldLen FieldOffset" << endl;
	cout << "----------------------------------------------------------------------------------------------" << endl;
	for(int c=0;c<m_nActRecCount;c++)
	{
		printf("  %d	%s	%s	%d	%d	%s	%d	%d	%s	%s	%s  \n",
							                         m_ActionTable[c].Seq_No,
							                         m_ActionTable[c].mapping_name,
			 				                         m_ActionTable[c].In_Field_Name.arr,
			 				                         m_ActionTable[c].In_Field_Length,
			 				                         m_ActionTable[c].In_Field_Offset,
			 				                         m_ActionTable[c].Out_Field_Name.arr,
			 				                         m_ActionTable[c].Out_Field_Length,
			 				                         m_ActionTable[c].Out_Field_Offset,
			 				                         m_ActionTable[c].Action_Code,
			 				                         m_ActionTable[c].Sub_Action_Code,
			 				                         m_ActionTable[c].Data_Type);
	}
	cout << "----------------------------------------------------------------------------------------------" << endl << endl;

}
