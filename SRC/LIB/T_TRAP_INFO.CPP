
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
    "t_trap_info.pc"
};


static unsigned long sqlctx = 1408499;


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

 static const char *sq0002 = 
"select TRAP_NO ,TRAP_NAME ,nvl(TRAP_TYPE,' ') ,nvl(TRAP_DESC,' ')  from T_TR\
AP_INFO            ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{10,4130,0,0,0,
5,0,0,1,44,0,4,106,0,0,1,0,0,1,0,2,3,0,0,
24,0,0,2,95,0,9,142,0,0,0,0,0,1,0,
39,0,0,2,0,0,15,156,0,0,0,0,0,1,0,
54,0,0,2,0,0,13,168,0,0,4,0,0,1,0,2,3,0,0,2,9,0,0,2,9,0,0,2,9,0,0,
};


#define SQLCA_STORAGE_CLASS extern
/* EXEC SQL INCLUDE    sqlca;
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

// DECLARE SECTION을 hpp에 선언할때
/* EXEC SQL INCLUDE "/home/uda1/include/t_trap_info.hpp";
 */ 
#ifndef _CLOADACTTABLE_
#define _CLOADACTTABLE_

//#define SQLCA_STORAGE_CLASS extern
//#include <sqlca.h>

#include <iostream.h>
#include <stdlib.h>
#include "/home/uda1/include/SqlErr.hpp"

#define   OKACTION        1
#define   FAILACTION      -1


//----------------------------------------
// Declare Host Variable
typedef struct    __TrapInfo
{
	int         Trap_No;
	char        Trap_name		[64+1];
	char        Trap_Type		[20+1];
	char        Trap_desc		[128+1];
}TRAPINFO;
//----------------------------------------

class    t_trap_info : public SqlErr
{
    private:

    public:
    	TRAPINFO*	m_TrapInfo;
        t_trap_info();
		~t_trap_info();
		int	nFlag;

		void init_TrapTbl();
        void get_RecordCount()    throw(char *);// table에서 레코드 개수를 가져온다.
        void show_TrapTbl();					// Table의 내용을 Display하기 위한 함수
		void show_TrapInfoTable();

        void declare_Cursor()    throw(char *);
        void open_Cursor()       throw(char *);
        void close_Cursor();
        void fetch_Data()        throw(char *);
        void fetch_all()		 throw(char *);	//	pms 요청에 의한 수정 ( jsj 5. 25 )
		int SearchIndex(int nCode);

};

#endif



//----------------------------------------
// Declare Host Variable

/* EXEC SQL BEGIN DECLARE SECTION; */ 

	typedef struct    __TrapTbl
	{
		int         Trap_No;
		/* VARCHAR     Trap_name		[64+1]; */ 
struct { unsigned short len; unsigned char arr[65]; } Trap_name;

		/* VARCHAR     Trap_Type		[20+1]; */ 
struct { unsigned short len; unsigned char arr[21]; } Trap_Type;

		/* VARCHAR     Trap_desc		[128+1]; */ 
struct { unsigned short len; unsigned char arr[129]; } Trap_desc;

	}TRAPTbl;

    TRAPTbl*      m_TrapTbl;           // Action Table
    int           m_nRecCount;          // Action Record Count

/* EXEC SQL END     DECLARE SECTION; */ 

//----------------------------------------


//------------------------------------------------------------------------------
// 생성자
t_trap_info::t_trap_info()
{
    m_nRecCount    = 0;
    nFlag		   = 0;	

}



//------------------------------------------------------------------------------
t_trap_info::~t_trap_info()
{
	if(nFlag == 1)
	{
		delete [] m_TrapTbl;
		delete [] m_TrapInfo;
	}	
}	


void t_trap_info::fetch_all()		 throw(char *)	//	pms 요청에 의한 수정 ( jsj 5. 25 )
{
   	nFlag = 1;
    try
    {
        get_RecordCount();	// GET Total Record Count
	    m_TrapTbl = new TRAPTbl[m_nRecCount+1]; // Allocate MetaTbl Buffer

        init_TrapTbl();		// Initialize Buffer
      
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

	int i;
	m_TrapInfo = new TRAPINFO[m_nRecCount+1];
    for(i = 0; i <= m_nRecCount; i++)
    	memset(&m_TrapInfo[i], 0x00, sizeof(TRAPINFO));
	for(i = 0; i <= m_nRecCount; i++)
	{
    	m_TrapInfo[i].Trap_No =  m_TrapTbl[i].Trap_No;

    	memcpy(m_TrapInfo[i].Trap_name	, m_TrapTbl[i].Trap_name.arr  	, sizeof(m_TrapTbl[i].Trap_name));
    	memcpy(m_TrapInfo[i].Trap_Type  , m_TrapTbl[i].Trap_Type.arr	, sizeof(m_TrapTbl[i].Trap_Type));
    	memcpy(m_TrapInfo[i].Trap_desc  , m_TrapTbl[i].Trap_desc.arr	, sizeof(m_TrapTbl[i].Trap_desc));
    }	
    	
// 	cout << "[t_trap_info] Constructor Success " << endl;
//	show_TrapInfoTable();
}

//------------------------------------------------------------------------------
// [t_trap_info] table의 레코드개수만큼 만들어진 배열을 초기화한다.
void t_trap_info::init_TrapTbl()
{
    int i;
    for(i = 0; i <= m_nRecCount; i++)
    	memset(&m_TrapTbl[i], 0x00, sizeof(TRAPTbl));
}




//------------------------------------------------------------------------------
// [t_trap_info] table에서 레코드 개수를 가져온다.
void t_trap_info::get_RecordCount()    throw(char *)
{
	/* EXEC SQL SELECT COUNT(*)
    	     INTO  :m_nRecCount
             FROM  T_TRAP_INFO; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 1;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select count(*)  into :b0  from T_TRAP_INFO ";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlstm.sqhstv[0] = (         void  *)&m_nRecCount;
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



    if (sqlca.sqlcode != 0){
   	 	strcpy(strSqlTemp,"!!!Error : [t_trap_info] table : Fail Get Record Count.");
  		throw (char *)strSqlTemp;
   	}	

}




//------------------------------------------------------------------------------
void t_trap_info::declare_Cursor()    throw(char *)
{
    /* EXEC SQL DECLARE CUR_TABLE CURSOR FOR
                SELECT      TRAP_NO,    
                            TRAP_NAME,         
                            nvl(TRAP_TYPE,' '),
                            nvl(TRAP_DESC,' ')
                FROM        T_TRAP_INFO; */ 


    if (sqlca.sqlcode != 0){
	    strcpy(strSqlTemp,"!!!Error : [t_trap_info]] table : Fail Declare Cursor.");
        throw (char *)strSqlTemp;
    }    
}




//------------------------------------------------------------------------------
void t_trap_info::open_Cursor()    throw(char *)
{
    /* EXEC SQL OPEN CUR_TABLE; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 1;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = sq0002;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )24;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



    if (sqlca.sqlcode != 0){
	    strcpy(strSqlTemp,"!!!Error : [t_trap_info] table : Fail Open Cursor.");
        throw (char *)strSqlTemp;
    }
}




//------------------------------------------------------------------------------
void t_trap_info::close_Cursor()
{
    /* EXEC SQL CLOSE CUR_TABLE; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 1;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )39;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


}




//------------------------------------------------------------------------------
void t_trap_info::fetch_Data()    throw(char *)
{
    
    for(int i = 0; i <= m_nRecCount; i++)
    {
        /* EXEC SQL FETCH CUR_TABLE INTO :m_TrapTbl[i]; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 10;
        sqlstm.arrsiz = 4;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )54;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)0;
        sqlstm.sqhstv[0] = (         void  *)&m_TrapTbl[i].Trap_No;
        sqlstm.sqhstl[0] = (unsigned int  )4;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)&m_TrapTbl[i].Trap_name;
        sqlstm.sqhstl[1] = (unsigned int  )67;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)&m_TrapTbl[i].Trap_Type;
        sqlstm.sqhstl[2] = (unsigned int  )23;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)&m_TrapTbl[i].Trap_desc;
        sqlstm.sqhstl[3] = (unsigned int  )131;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         void  *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned int  )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
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
        m_TrapTbl[i].Trap_name.arr[m_TrapTbl[i].Trap_name.len] = NULL;
        m_TrapTbl[i].Trap_Type.arr[m_TrapTbl[i].Trap_Type.len] = NULL;
        m_TrapTbl[i].Trap_desc.arr[m_TrapTbl[i].Trap_desc.len] = NULL;
        
        if(sqlca.sqlcode == 1403){
            close_Cursor();
            break;
        }
        else if(sqlca.sqlcode != 0){
            strcpy(strSqlTemp,"!!!Error : [t_trap_info] table : Fail Data fetch.");
            close_Cursor();
            throw (char *)strSqlTemp;
        }
    }
    
//    show_TrapTbl();	// [t_trap_info] table Display

}

int t_trap_info::SearchIndex(int nCode)
{
	for(int i = 0; i < m_nRecCount; i++)
	{
		if (nCode == m_TrapTbl[i].Trap_No)
		{
		   	return i;
		}
	   
	}//for
    return FAILACTION;
}



//------------------------------------------------------------------------------
// [t_trap_info] Table의 내용을 Display하기 위한 함수
void t_trap_info::show_TrapTbl()
{
	cout << "########### T_TRAP_INFO Table ###########" << endl;
	cout << "All Record   Cnt           => " << m_nRecCount << endl;

	cout << "======================================================================================" << endl;
	cout << "Trap_No    Trap_name               Trap_Type                     Trap_desc" << endl;
	cout << "--------------------------------------------------------------------------------------" << endl;
	for(int c=0;c<m_nRecCount;c++)
	{
		printf(" %d 	%s	%s	%s\n",
			                         m_TrapTbl[c].Trap_No,
			                         m_TrapTbl[c].Trap_name.arr,
			                         m_TrapTbl[c].Trap_Type.arr,
			                         m_TrapTbl[c].Trap_desc.arr);
	}
	cout << "----------------------------------------------------------------------------------------------" << endl << endl;

}
//------------------------------------------------------------------------------
// [t_meta_action] Table의 내용을 Display하기 위한 함수
void t_trap_info::show_TrapInfoTable()
{
	cout << "########### T_TRAP_INFO Table ###########" << endl;
	cout << "All Record   Cnt           => " << m_nRecCount << endl;

	cout << "======================================================================================" << endl;
	cout << "Trap_No    Trap_name               Trap_Type                     Trap_desc" << endl;
	cout << "--------------------------------------------------------------------------------------" << endl;
	for(int c=0;c<m_nRecCount;c++)
	{
		printf(" %d 	%s	%s	%s\n",
			                         m_TrapTbl[c].Trap_No,
			                         m_TrapTbl[c].Trap_name.arr,
			                         m_TrapTbl[c].Trap_Type.arr,
			                         m_TrapTbl[c].Trap_desc.arr);
	}
	cout << "----------------------------------------------------------------------------------------------" << endl << endl;
}
