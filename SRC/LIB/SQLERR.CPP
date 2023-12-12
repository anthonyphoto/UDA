
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
           char  filnam[10];
};
static const struct sqlcxp sqlfpn =
{
    9,
    "SqlErr.pc"
};


static unsigned long sqlctx = 41067;


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
            void  *sqhstv[1];
   unsigned int   sqhstl[1];
            int   sqhsts[1];
            void  *sqindv[1];
            int   sqinds[1];
   unsigned int   sqharm[1];
   unsigned int   *sqharc[1];
   unsigned short  sqadto[1];
   unsigned short  sqtdso[1];
} sqlstm = {10,1};

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
5,0,0,1,0,0,544,54,0,0,0,0,0,1,0,
20,0,0,2,0,0,543,69,0,0,0,0,0,1,0,
};


/////////////////////////////////////////////////////////////
//
//   Class Name     : SqlErr
//   Usage          : Sql Error 처리를 위한 공용 클래스
//   DATE           : 2000. 12 .1
//   LAST UPDATE    :
//   AUTHOR         : Kwon Chong Wook (cwkwon@neomnt.co.kr)
//   History        :
//
/////////////////////////////////////////////////////////////

#define SQLCA_STORAGE_CLASS extern
#include <sqlca.h>
#include <oraca.h>

/* EXEC SQL INCLUDE "/home/uda1/include/SqlErr.hpp";
 */ 
/////////////////////////////////////////////////////////////
//
//   Class Name     : SqlErr
//   Usage          : Sql Error 처리를 위한 공용 클래스
//   DATE           : 2000. 12 .1
//   LAST UPDATE    :
//   AUTHOR         : Kwon Chong Wook (cwkwon@neomnt.co.kr)
//   History        :
//
/////////////////////////////////////////////////////////////
#ifndef _CSQLERR_
#define _CSQLERR_
#include <iostream.h>
#include <stdlib.h>
#include <string.h>

/////////////////////////////////////////////////////////////
//	Comment : SQL Error를 처리하기 위한 클래스
/////////////////////////////////////////////////////////////
class	SqlErr
{
	private:
		char	strSqlError[128];

	protected:
		static char	strSqlTemp[50];
		char        m_strSqlMsg[400];

	public:
		SqlErr();
		// Initiate SQLCA Option
		void	Init_SqlCa();
		// Error 발생후 program 종료할 필요가있을 때
		virtual void	SqlError(char* strMsg=NULL);
		// Error Message print and Return
		virtual char	*SqlErrCout(char* strMsg=NULL);

};

#endif


char	SqlErr::strSqlTemp[50];

/////////////////////////////////////////////////////////////
//	Comment : SQL Error를 처리하기 위한 클래스
/////////////////////////////////////////////////////////////
SqlErr::SqlErr()
{
	memset (strSqlError, NULL, 128);
	Init_SqlCa();
}

// Initiate SQLCA Option
void SqlErr::Init_SqlCa()
{
	// enable you to use the ORACA
	/* EXEC ORACLE OPTION (ORACA=YES); */ 

	
	// instructs Pro*C to release resources associated with embedded SQL
    // statements after they are executed
	/* EXEC ORACLE OPTION (RELEASE_CURSOR=YES); */ 

	
	// Save text of current SQL statement in the ORACA if an error occurs
	oraca.orastxtf = ORASTFERR;
	//cout << "Init_SqlCa!! " << endl;
}

// Error 발생후 program 종료할 필요가있을 때
void  SqlErr::SqlError(char* strMsg)
{
	printf("\nProcess abnormal done. SQL Error : \n%s\n%.*s",
				 strMsg,sqlca.sqlerrm.sqlerrml, sqlca.sqlerrm.sqlerrmc);
	printf("in \"%.*s...\"\n", oraca.orastxt.orastxtl,oraca.orastxt.orastxtc);
	printf("on line %d of %.*s.\n\n",
	 oraca.oraslnr, oraca.orasfnm.orasfnml, oraca.orasfnm.orasfnmc);

	/* EXEC SQL WHENEVER SQLERROR CONTINUE; */ 

	/* EXEC SQL ROLLBACK WORK RELEASE; */ 

{
 struct sqlexd sqlstm;
 sqlorat((void **)0, &sqlctx, &oraca);
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 0;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	exit(-1);
}

// Error Message print and Return
char *SqlErr::SqlErrCout(char* strMsg)
{
	printf("\nProcess abnormal done. SQL Error : \n%s\n%.*s",
				 strMsg,sqlca.sqlerrm.sqlerrml, sqlca.sqlerrm.sqlerrmc);
	printf("in \"%.*s...\"\n", oraca.orastxt.orastxtl,oraca.orastxt.orastxtc);
	printf("on line %d of %.*s.\n\n",
	 oraca.oraslnr, oraca.orasfnm.orasfnml, oraca.orasfnm.orasfnmc);
	 
	sprintf(m_strSqlMsg,"%.*s", sqlca.sqlerrm.sqlerrml, sqlca.sqlerrm.sqlerrmc);

	/* EXEC SQL ROLLBACK WORK; */ 

{
 struct sqlexd sqlstm;
 sqlorat((void **)0, &sqlctx, &oraca);
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


	
	return m_strSqlMsg;
}

/* Example
	Process abnormal done. SQL Error :
	Action Table : Select get_RecordCount Error!!
	ORA-00942: table or view does not exist
	in "select count(*)  into :b0  from T_META_FIEL where META_FILE_NAME=:b1..."
	on line 65 of LoadActTable.pc.
*/

