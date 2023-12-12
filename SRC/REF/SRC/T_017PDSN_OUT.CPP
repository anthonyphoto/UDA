
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
    "t_017pdsn_out.pc"
};


static unsigned long sqlctx = 5174571;


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
            void  *sqhstv[3];
   unsigned int   sqhstl[3];
            int   sqhsts[3];
            void  *sqindv[3];
            int   sqinds[3];
   unsigned int   sqharm[3];
   unsigned int   *sqharc[3];
   unsigned short  sqadto[3];
   unsigned short  sqtdso[3];
} sqlstm = {10,3};

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
"select SEQ_NO ,nvl(PDSN_CODE,0) ,nvl(VENDOR_CODE,0)  from T_017PDSN_OUT  ord\
er by SEQ_NO            ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{10,4130,0,0,0,
5,0,0,1,46,0,4,92,0,0,1,0,0,1,0,2,3,0,0,
24,0,0,2,100,0,9,130,0,0,0,0,0,1,0,
39,0,0,2,0,0,15,145,0,0,0,0,0,1,0,
54,0,0,2,0,0,13,157,0,0,3,0,0,1,0,2,3,0,0,2,3,0,0,2,3,0,0,
};


/******************************************************************
* 파 일 명     
      : t_017pdsn_out.pc                
* 기   능                                                          
     : Parsing된 Pdsn format으로 바꾸기 위해 [t_017pdsn_out]table의 data를 Memory로 Load한다.
* 작 성 자     
     : 신미연                          
* first data  
     : 2001. 03. 21                      
* last updata 
     : 2000. 03. 29                     
* 관 련 화 일  
     : t_017pdsn_out.hpp                     
* program history                                
           
* Version
  : 1.1 ( 2001. 05. 21 )                                           
******************************************************************/


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

/* EXEC SQL INCLUDE "../INCLUDE/t_017pdsn_out.hpp";
 */ 
#define SQLCA_STORAGE_CLASS extern
#include <sqlca.h>

#include <iostream.h>
#include <stdlib.h>
#include "../INCLUDE/SqlErr.hpp"

#ifndef	  OKPDSN
#define   OKPDSN        1
#define   FAILPDSN      -1
#endif


#ifndef _C017Pdsn_
#define _C017Pdsn_

//----------------------------------------
// Declare Host Variable
/* EXEC SQL BEGIN     DECLARE SECTION; */ 


    typedef struct    __PdsnTable
    {
        int         Seq_No;
        int    	    Pdsn_Code;
        int   	    Vendor_Code;
    }PdsnTable;

/* EXEC SQL END     DECLARE SECTION; */ 

//----------------------------------------



///////////////////////////////
//		Class C017Pdsn		 //
///////////////////////////////

class C017Pdsn : public SqlErr
{
    private:

    public:
        /* EXEC SQL BEGIN		DECLARE SECTION; */ 

			int				m_nSeq_no;
            PdsnTable*  	m_PdsnTable;           // Action Table
            int           	m_nOutRecCount;        // Action Record Count
        /* EXEC SQL END		DECLARE SECTION; */ 


        int m_nOutStartRecIndex;// Action Start Record Index

        C017Pdsn();
		~C017Pdsn();

        void init_PdsnTable();// [t_017pdsn_out] table의 레코드개수만큼 만들어진 배열을 초기화한다.

        void get_RecordCount()    throw(char *);// [t_017pdsn_out] table에서 레코드 개수를 가져온다.
        void show_PdsnTable();// [t_017pdsn_out] Table의 내용을 Display하기 위한 함수

        void declare_Cursor()    throw(char *);
        void open_Cursor()       throw(char *);
        void close_Cursor();
        void fetch_Data()        throw(char *);
        
       	// [t_017pdsn_out]table의 record count만큼 루프를 돌면서 Mapping Name, Mapping Version이
       	// 같은지 확인한 후 Sequence Number의 값이 0인 레코드의 시작 위치를 찾는다.
        int SearchPdsnIndex();

};

#endif




///////////////////////////////
// Class C017Pdsn's Method	 //
///////////////////////////////

//------------------------------------------------------------------------------
// 생성자
C017Pdsn::C017Pdsn()
{
    m_nOutRecCount    = 0;

    try
    {
        get_RecordCount();	// GET Total Record Count

	    m_PdsnTable = new PdsnTable[m_nOutRecCount+1]; // Allocate ActionTable Buffer

        init_PdsnTable();	// Initialize Buffer
      
        declare_Cursor();	// Declare Cursor

        open_Cursor();		// OPEN Cursor

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
	if(nStartIdx  = SearchPdsnIndex() < 0)		exit(-1);
	m_nOutStartRecIndex = nStartIdx;
}




//------------------------------------------------------------------------------
C017Pdsn::~C017Pdsn()
{
	delete [] m_PdsnTable;
}




//------------------------------------------------------------------------------
// [t_017pdsn_out] table의 레코드개수만큼 만들어진 배열을 초기화한다.
void C017Pdsn::init_PdsnTable()
{
    int i;
    for(i = 0; i <= m_nOutRecCount; i++)
    	memset(&m_PdsnTable[i], 0x00, sizeof(PdsnTable));
}




//------------------------------------------------------------------------------
// [t_017pdsn_out] table에서 레코드 개수를 가져온다.
void C017Pdsn::get_RecordCount()    throw(char *)
{
	/* EXEC SQL SELECT COUNT(*)
    	     INTO  :m_nOutRecCount
             FROM  T_017PDSN_OUT; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 1;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select count(*)  into :b0  from T_017PDSN_OUT ";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlstm.sqhstv[0] = (         void  *)&m_nOutRecCount;
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
   	 	strcpy(strSqlTemp,"!!!Error : [t_017pdsn_out] table : Fail Get Record Count.");
  		throw (char *)strSqlTemp;
   	}	

}




//------------------------------------------------------------------------------
// Cursor 선언
void C017Pdsn::declare_Cursor()    throw(char *)
{
    /* EXEC SQL DECLARE CUR_TABLE CURSOR FOR
                SELECT      SEQ_NO,
                            nvl(PDSN_CODE,0),
                            nvl(VENDOR_CODE,0)
                FROM        T_017PDSN_OUT
                ORDER BY    SEQ_NO; */ 


    if (sqlca.sqlcode != 0){
	    strcpy(strSqlTemp,"!!!Error : [t_017pdsn_out] table : Fail Declare Cursor.");
        throw (char *)strSqlTemp;
    }    
}




//------------------------------------------------------------------------------
// Cursor Open
void C017Pdsn::open_Cursor()    throw(char *)
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
	    strcpy(strSqlTemp,"!!!Error : [t_017pdsn_out] table : Fail Open Cursor.");
        throw (char *)strSqlTemp;
    }
}




//------------------------------------------------------------------------------
// Cursor Close
void C017Pdsn::close_Cursor()
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
// DB에 있는 Data를 메모리로 가져온다.
void C017Pdsn::fetch_Data()    throw(char *)
{
    for(int i = 0; i <= m_nOutRecCount; i++)
    {
        /* EXEC SQL FETCH CUR_TABLE INTO :m_PdsnTable[i]; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 10;
        sqlstm.arrsiz = 3;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )54;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)0;
        sqlstm.sqhstv[0] = (         void  *)&m_PdsnTable[i].Seq_No;
        sqlstm.sqhstl[0] = (unsigned int  )4;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)&m_PdsnTable[i].Pdsn_Code;
        sqlstm.sqhstl[1] = (unsigned int  )4;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)&m_PdsnTable[i].Vendor_Code;
        sqlstm.sqhstl[2] = (unsigned int  )4;
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



        if(sqlca.sqlcode == 1403){
            close_Cursor();
            break;
        }
        else if(sqlca.sqlcode != 0){
            strcpy(strSqlTemp,"!!!Error : [t_017pdsn_out] table : Fail Data fetch.");
            close_Cursor();
            throw (char *)strSqlTemp;
        }
    }
    
//    show_PdsnTable();	// [t_017pdsn_out] table Display

}




//------------------------------------------------------------------------------
// [t_017pdsn_out]table의 record count만큼 루프를 돌면서 Mapping Name, Mapping Version이 같은지 확인한 후
// Sequence Number의 값이 0인 레코드의 시작 위치를 찾는다
int C017Pdsn::SearchPdsnIndex()
{
	for(int i = 0; i < m_nOutRecCount; i++){
		if(m_PdsnTable[i].Seq_No==0){
//		    cout << " i=> " << i << endl;
			return i;
		}//if
	}//for

//    cout << "!!!Error : [t_017pdsn_out] table : Sequence Number != 0" << endl;
    return FAILPDSN;
}




//------------------------------------------------------------------------------
// [t_017pdsn_out] Table의 내용을 Display하기 위한 함수
void C017Pdsn::show_PdsnTable()
{
	cout << "########### T_017PDSN_OUT Table ###########" << endl;
	cout << "All Record   Cnt           => " << m_nOutRecCount << endl;

	cout << "======================================================" << endl;
	cout << "SeqNo	PdsnCode	VendorCode" << endl;
	cout << "------------------------------------------------------" << endl;
	for(int c=0;c<m_nOutRecCount;c++)
	{
		printf( "  %d	%d	%d\n", m_PdsnTable[c].Seq_No, m_PdsnTable[c].Pdsn_Code, m_PdsnTable[c].Vendor_Code);
	}
	cout << "------------------------------------------------------" << endl;

}
