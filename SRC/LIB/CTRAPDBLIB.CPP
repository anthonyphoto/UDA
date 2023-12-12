
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
           char  filnam[14];
};
static const struct sqlcxp sqlfpn =
{
    13,
    "CTrapDbLib.pc"
};


static unsigned long sqlctx = 601659;


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
            void  *sqhstv[6];
   unsigned int   sqhstl[6];
            int   sqhsts[6];
            void  *sqindv[6];
            int   sqinds[6];
   unsigned int   sqharm[6];
   unsigned int   *sqharc[6];
   unsigned short  sqadto[6];
   unsigned short  sqtdso[6];
} sqlstm = {10,6};

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
"select nvl(recv_time,' ') ,nvl(trap_no,0) ,nvl(trap_name,' ') ,nvl(trap_valu\
e,0) ,nvl(trap_desc,' ') ,nvl(lengthb(trap_desc),0)  from T_TRAP_LOG where sen\
d_time=' '           ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{10,4130,0,0,0,
5,0,0,1,175,0,9,98,0,0,0,0,0,1,0,
20,0,0,1,0,0,13,125,0,0,6,0,0,1,0,2,97,0,0,2,3,0,0,2,97,0,0,2,3,0,0,2,97,0,0,2,
3,0,0,
59,0,0,1,0,0,15,154,0,0,0,0,0,1,0,
74,0,0,2,56,0,5,187,0,0,2,2,0,1,0,1,97,0,0,1,97,0,0,
97,0,0,3,0,0,29,191,0,0,0,0,0,1,0,
112,0,0,4,0,0,31,192,0,0,0,0,0,1,0,
127,0,0,5,101,0,3,225,0,0,5,5,0,1,0,1,97,0,0,1,3,0,0,1,97,0,0,1,3,0,0,1,97,0,0,
162,0,0,6,0,0,29,229,0,0,0,0,0,1,0,
177,0,0,7,0,0,31,230,0,0,0,0,0,1,0,
};


/*****************************************************************************************
* file : CTrapDbLib.pc
*
* T_TRAP_LOG 테이블 핸들링 클래스.
*
*                                                                 01-05-22 5:06오후 ky.
*****************************************************************************************/
// Include
//----------------------------------------------------------
#define SQLCA_STORAGE_CLASS extern
#include "CTrapDbLib.hpp"
#include "time.hpp"       // for get_MDateTime ...
#include <sqlca.h>




// Structure
//----------------------------------------------------------
/* EXEC SQL BEGIN DECLARE SECTION; */ 

   typedef struct _trapT
   {
      char strRecvTime[ 17+1];   // receive time (삽입시간)
      int  nTrapNo;              // tran number
      char strTrapName[ 64+1];   // trap name
      int  nTrapValue;           // trap value
      char strTrapDesc[128+1];   // trap description
      int  strTrapDescLen;       // trap description length
   } h_trapT;
/* EXEC SQL END   DECLARE SECTION; */ 





// Class define
//----------------------------------------------------------
// Public member function
/*****************************************************************************************
* Function Name : CTrapDbLib()
* Description   : CTrapDbLib class constructor. 변수 초기화.
* Parameters    : void
* Return values : void
* Date          : 01-05-22 5:41오후
* Last Update   :
* Author        : Ki Yon
*****************************************************************************************/
CTrapDbLib::CTrapDbLib()
{
   m_nCursorFlag = false;
}//end CTrapDbLib




/*****************************************************************************************
* Function Name : ~CTrapDbLib()
* Description   : CTrapDbLib class destructor.
* Parameters    : void
* Return values : void
* Date          : 01-05-22 7:29오후
* Last Update   :
* Author        : Ki Yon
*****************************************************************************************/
CTrapDbLib::~CTrapDbLib()
{
   // 커서가 오픈되어 있으면 닫는다.
   //----------------------------------------------------------
   if (m_nCursorFlag == true) CloseCursor();
}//end ~CTrapDbLib




/*****************************************************************************************
* Function Name : OpenCursor()
* Description   : cursor open.
* Parameters    : void
* Return values : bool true  - cursor open success.
*                      false - cursor open fail.
* Date          : 01-05-22 6:07오후
* Last Update   :
* Author        : Ki Yon
*****************************************************************************************/
bool CTrapDbLib::OpenCursor()
{
   // Cursor declare
   //----------------------------------------------------------
   /* EXEC SQL DECLARE trap_cursor CURSOR FOR
      SELECT nvl(recv_time, ' '), nvl(trap_no, 0), nvl(trap_name, ' '),
             nvl(trap_value, 0), nvl(trap_desc, ' '), nvl(lengthb(trap_desc), 0)
             from  T_TRAP_LOG
             where send_time = ' '; */ 
         // sendtime을 플래그로 사용.

   if (sqlca.sqlcode != 0) return false;

   // Cursor open
   //----------------------------------------------------------
   /* EXEC SQL OPEN trap_cursor; */ 

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



   m_nCursorFlag = true;   // cursor open

   return (sqlca.sqlcode == 0) ? true : false;
}//end OpenCursor




/*****************************************************************************************
* Function Name : Select()
* Description   : T_TRAP_LOG. fetch.
* Parameters    : void
* Return values : int 0   - fetch fail.
*                     > 0 - fetch sucess. 조회한 갯수.
* Date          : 01-05-22 7:40오후
* Last Update   :
* Author        : Ki Yon
*****************************************************************************************/
int CTrapDbLib::Select(trapT *data)
{
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      h_trapT h_data[D_LOG_RECORD_CNT];                    // fetch data저장 변수.
   /* EXEC SQL END   DECLARE SECTION; */ 


	memset(h_data, 0x00, sizeof(h_data));
   /* EXEC SQL FETCH trap_cursor INTO :h_data; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 6;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )100;
   sqlstm.offset = (unsigned int  )20;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)0;
   sqlstm.sqhstv[0] = (         void  *)h_data->strRecvTime;
   sqlstm.sqhstl[0] = (unsigned int  )18;
   sqlstm.sqhsts[0] = (         int  )sizeof(struct _trapT);
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqharc[0] = (unsigned int   *)0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)&h_data->nTrapNo;
   sqlstm.sqhstl[1] = (unsigned int  )4;
   sqlstm.sqhsts[1] = (         int  )sizeof(struct _trapT);
   sqlstm.sqindv[1] = (         void  *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned int  )0;
   sqlstm.sqharc[1] = (unsigned int   *)0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (         void  *)h_data->strTrapName;
   sqlstm.sqhstl[2] = (unsigned int  )65;
   sqlstm.sqhsts[2] = (         int  )sizeof(struct _trapT);
   sqlstm.sqindv[2] = (         void  *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned int  )0;
   sqlstm.sqharc[2] = (unsigned int   *)0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (         void  *)&h_data->nTrapValue;
   sqlstm.sqhstl[3] = (unsigned int  )4;
   sqlstm.sqhsts[3] = (         int  )sizeof(struct _trapT);
   sqlstm.sqindv[3] = (         void  *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned int  )0;
   sqlstm.sqharc[3] = (unsigned int   *)0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (         void  *)h_data->strTrapDesc;
   sqlstm.sqhstl[4] = (unsigned int  )129;
   sqlstm.sqhsts[4] = (         int  )sizeof(struct _trapT);
   sqlstm.sqindv[4] = (         void  *)0;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned int  )0;
   sqlstm.sqharc[4] = (unsigned int   *)0;
   sqlstm.sqadto[4] = (unsigned short )0;
   sqlstm.sqtdso[4] = (unsigned short )0;
   sqlstm.sqhstv[5] = (         void  *)&h_data->strTrapDescLen;
   sqlstm.sqhstl[5] = (unsigned int  )4;
   sqlstm.sqhsts[5] = (         int  )sizeof(struct _trapT);
   sqlstm.sqindv[5] = (         void  *)0;
   sqlstm.sqinds[5] = (         int  )0;
   sqlstm.sqharm[5] = (unsigned int  )0;
   sqlstm.sqharc[5] = (unsigned int   *)0;
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
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



   // 조회 데이터 반환.
   //----------------------------------------------------------
   for (int i=0; i<sqlca.sqlerrd[2]; i++)
   {
      strtok(h_data[i].strRecvTime, " ");
      strtok(h_data[i].strTrapName, " ");
      h_data[i].strTrapDesc[h_data[i].strTrapDescLen] = 0x00;
   }
   memcpy(data, h_data, sizeof(trapT)*D_LOG_RECORD_CNT);
   return sqlca.sqlerrd[2];   // 조회한 갯수를 return한다.
}//end Select




/*****************************************************************************************
* Function Name : CloseCursor()
* Description   : cursor close.
* Parameters    : void
* Return values : bool true  - cursor close success.
*                      false - cursor close fail.
* Date          : 01-05-22 7:28오후
* Last Update   :
* Author        : Ki Yon
*****************************************************************************************/
bool CTrapDbLib::CloseCursor()
{
   /* EXEC SQL CLOSE trap_cursor; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 6;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )59;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



   m_nCursorFlag = false;     // cursor close

   return (sqlca.sqlcode == 0) ? true : false;
}//end CloseCursor




/*****************************************************************************************
* Function Name : UpdateSendTime()
* Description   : NMS로 trap을 전송한 시간(현재시간)을 T_TRAP_LOG ptrKey레코드의
*                 sendtime field에 기록.
* Parameters    : const char *ptrKey - record key
* Return values : bool true  - update success.
*                      false - update fail.
* Date          : 01-05-22 9:19오후
* Last Update   :
* Author        : Ki Yon
*****************************************************************************************/
bool CTrapDbLib::UpdateSendTime(const char *ptrKey)
{
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      const char *h_ptrKey  = ptrKey;     // key
      char *h_strTime;                    // send time
   /* EXEC SQL END   DECLARE SECTION; */ 


   DateTime Itime;                        // time class object
   h_strTime = Itime.get_MDateTime(1);    // 년월일시분초천분의일초 ex) '20010522211523332'

   // trap send time update
   //----------------------------------------------------------
   /* EXEC SQL UPDATE T_TRAP_LOG
                   SET   send_time = :h_strTime
                   WHERE recv_time = :h_ptrKey; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 6;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "update T_TRAP_LOG  set send_time=:b0 where recv_time=:b1";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )74;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)0;
   sqlstm.sqhstv[0] = (         void  *)h_strTime;
   sqlstm.sqhstl[0] = (unsigned int  )0;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)h_ptrKey;
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
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



   if (sqlca.sqlcode == 0) { /* EXEC SQL COMMIT WORK; */ 

{
                             struct sqlexd sqlstm;
                             sqlstm.sqlvsn = 10;
                             sqlstm.arrsiz = 6;
                             sqlstm.sqladtp = &sqladt;
                             sqlstm.sqltdsp = &sqltds;
                             sqlstm.iters = (unsigned int  )1;
                             sqlstm.offset = (unsigned int  )97;
                             sqlstm.cud = sqlcud0;
                             sqlstm.sqlest = (unsigned char  *)&sqlca;
                             sqlstm.sqlety = (unsigned short)0;
                             sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 return true; }
   else { /* EXEC SQL ROLLBACK WORK; */ 

{
          struct sqlexd sqlstm;
          sqlstm.sqlvsn = 10;
          sqlstm.arrsiz = 6;
          sqlstm.sqladtp = &sqladt;
          sqlstm.sqltdsp = &sqltds;
          sqlstm.iters = (unsigned int  )1;
          sqlstm.offset = (unsigned int  )112;
          sqlstm.cud = sqlcud0;
          sqlstm.sqlest = (unsigned char  *)&sqlca;
          sqlstm.sqlety = (unsigned short)0;
          sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 return false; }
}//end UpdateSendTime




/*****************************************************************************************
* Function Name : Insert()
* Description   : T_TRAP_LOG insert.
* Parameters    : const int   nTrapNo     - trap number
*                 const char *ptrTrapName - trap name
*                 const int   nTrapValue  - trap value
*                 const char *ptrTrapDesc - trap description
* Return values : bool true  - insert success.
*                      false - insert fail.
* Date          : 01-05-22 9:33오후
* Last Update   :
* Author        : Ki Yon
*****************************************************************************************/
bool CTrapDbLib::Insert(const int nTrapNo, const char *ptrTrapName, const int nTrapValue,
                        const char *ptrTrapDesc)
{
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      const char *h_ptrRecvTime;                  // recvtime
      const int   h_nTrapNo     = nTrapNo;        // trap number
      const char *h_ptrTrapName = ptrTrapName;    // trap name
      const int   h_nTrapValue  = nTrapValue;     // trap value
      const char *h_ptrTrapDesc = ptrTrapDesc;    // trap description
   /* EXEC SQL END   DECLARE SECTION; */ 


   DateTime Itime;                                // time class object
   h_ptrRecvTime = Itime.get_MDateTime(1);        // 년월일시분초천분의일초 ex) '20010522211523332'

   /* EXEC SQL INSERT INTO T_TRAP_LOG (recv_time, trap_no, trap_name, trap_value, trap_desc)
                   VALUES (:h_ptrRecvTime, :h_nTrapNo, :h_ptrTrapName,
                           :h_nTrapValue, :h_ptrTrapDesc); */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 6;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "insert into T_TRAP_LOG(recv_time,trap_no,trap_name,trap_va\
lue,trap_desc) values (:b0,:b1,:b2,:b3,:b4)";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )127;
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
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



   if (sqlca.sqlcode == 0) { /* EXEC SQL COMMIT WORK; */ 

{
                             struct sqlexd sqlstm;
                             sqlstm.sqlvsn = 10;
                             sqlstm.arrsiz = 6;
                             sqlstm.sqladtp = &sqladt;
                             sqlstm.sqltdsp = &sqltds;
                             sqlstm.iters = (unsigned int  )1;
                             sqlstm.offset = (unsigned int  )162;
                             sqlstm.cud = sqlcud0;
                             sqlstm.sqlest = (unsigned char  *)&sqlca;
                             sqlstm.sqlety = (unsigned short)0;
                             sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 return true; }
   else { /* EXEC SQL ROLLBACK WORK; */ 

{
          struct sqlexd sqlstm;
          sqlstm.sqlvsn = 10;
          sqlstm.arrsiz = 6;
          sqlstm.sqladtp = &sqladt;
          sqlstm.sqltdsp = &sqltds;
          sqlstm.iters = (unsigned int  )1;
          sqlstm.offset = (unsigned int  )177;
          sqlstm.cud = sqlcud0;
          sqlstm.sqlest = (unsigned char  *)&sqlca;
          sqlstm.sqlety = (unsigned short)0;
          sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 return false; }
}//end Insert



