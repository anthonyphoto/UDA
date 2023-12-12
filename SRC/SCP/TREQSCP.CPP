
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
    "TReqScp.pc"
};


static unsigned long sqlctx = 82795;


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
            void  *sqhstv[19];
   unsigned int   sqhstl[19];
            int   sqhsts[19];
            void  *sqindv[19];
            int   sqinds[19];
   unsigned int   sqharm[19];
   unsigned int   *sqharc[19];
   unsigned short  sqadto[19];
   unsigned short  sqtdso[19];
} sqlstm = {10,19};

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
5,0,0,1,59,0,4,73,0,0,2,1,0,1,0,2,68,0,0,1,97,0,0,
28,0,0,2,250,0,3,132,0,0,17,17,0,1,0,1,97,0,0,1,68,0,0,1,3,0,0,1,97,0,0,1,68,0,
0,1,97,0,0,1,97,0,0,1,68,0,0,1,68,0,0,1,68,0,0,1,68,0,0,1,68,0,0,1,68,0,0,1,68,
0,0,1,68,0,0,1,68,0,0,1,68,0,0,
111,0,0,3,0,0,29,138,0,0,0,0,0,1,0,
126,0,0,4,0,0,31,143,0,0,0,0,0,1,0,
141,0,0,5,295,0,4,166,0,0,19,2,0,1,0,2,97,0,0,2,68,0,0,2,3,0,0,2,97,0,0,2,68,0,
0,2,97,0,0,2,97,0,0,2,68,0,0,2,68,0,0,2,68,0,0,2,68,0,0,2,68,0,0,2,68,0,0,2,68,
0,0,2,68,0,0,2,68,0,0,2,68,0,0,1,97,0,0,1,68,0,0,
232,0,0,6,295,0,4,196,0,0,19,2,0,1,0,2,97,0,0,2,68,0,0,2,3,0,0,2,97,0,0,2,68,0,
0,2,97,0,0,2,97,0,0,2,68,0,0,2,68,0,0,2,68,0,0,2,68,0,0,2,68,0,0,2,68,0,0,2,68,
0,0,2,68,0,0,2,68,0,0,2,68,0,0,1,97,0,0,1,68,0,0,
};


#include "TReqScp.hpp"
#include <SendLogManage.hpp>

extern 	SendLogManage*	LogM;

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

static char			 SysDate[8+1];		
static unsigned int  nTid;				
static int			 nCode;				
static char			 Msid[15+1];			
static unsigned int  nOper_Id;			
static char			 Account_Id[8+1];
static char			 Correl_Id[8+1];
static unsigned int  nStart_Time;
static unsigned int  nEnd_Time;
static unsigned int  nLimit_Money;
static unsigned int  nTop_Use;
static unsigned int  nServiceId;
static unsigned int  nCgClass;
static unsigned int  nPacket1;
static unsigned int  nPacket2;
static unsigned int  nPacket3;
static unsigned int  nUseMoney;
/* EXEC SQL END DECLARE SECTION; */ 



//------------------------------------------------------------------------
TReqScp::TReqScp()
{
	DBerror = clsDbConnect.connect();
	if(DBerror < 0)
	{
		cout << "[SCHANNEL] DB Connect Failure  [" << DBerror << "] : " << endl;
		*LogM << "[SCHANNEL] DB Connect Failure  [" << DBerror << "] : " <<  LogM->ErrorCode(8220) << endl;
	} else *LogM << "[SCHANNEL] DB Connect Success " << LogM->ErrorCode(0) << endl;
}



//------------------------------------------------------------------------
TReqScp::~TReqScp()
{
	if(DBerror) return;
	clsDbConnect.disconnect();	
}


//------------------------------------------------------------------------
char* TReqScp::Get_Day(void)
{
	//cout << "[TREQSCP] SysDate : " << SysDate << endl;
	return SysDate;	
}


//------------------------------------------------------------------------	
unsigned long TReqScp::Get_Tid(void)
{
	//cout << "[TREQSCP] nTid : " << nTid << endl;
	return nTid;	
	
}

		
//------------------------------------------------------------------------	
int TReqScp::Get_MaxTid(void)
{
	if(DBerror) return 0;
	
	/* EXEC SQL 	SELECT max(tid) into :nTid
				FROM t_req_scp
				WHERE sys_date =:SysDate; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 2;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select max(tid) into :b0  from t_req_scp where sys_date=:b1";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlstm.sqhstv[0] = (         void  *)&nTid;
 sqlstm.sqhstl[0] = (unsigned int  )4;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)SysDate;
 sqlstm.sqhstl[1] = (unsigned int  )9;
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


	
	if (sqlca.sqlcode != 0) 
	{
		cout << "[TREQSCP] Get_MaxTid ERROR : " << sqlca.sqlcode << endl;
		*LogM << "[TREQSCP] Get_MaxTid ERROR : " <<  sqlca.sqlcode  <<  LogM->ErrorCode(0) << endl;
		nTid=0;
	}
	
	cout << "[TREQSCP] Get_MaxTid : " << nTid << endl;
	return nTid;
}




//------------------------------------------------------------------------
int	TReqScp::Add_ScpRecord(SCPFormatAgg Aggrec)
{	
	if(DBerror) return 0;
	
	//-------------------------------------변수할당 -----------------
	int			nRet;
	time_t		Time;	
	struct  tm *Tm;
	
	time(&Time);	
	Tm = localtime(&Time);	
	sprintf(SysDate, "%04d%02d%02d",Tm->tm_year+1900, Tm->tm_mon+1, Tm->tm_mday);
	
	sprintf(Msid, "%ld", Aggrec.get_ItemA2()->get_Min());
	if(strlen(Msid)==9) sprintf(Msid, "0%-14ld", Aggrec.get_ItemA2()->get_Min());
	else sprintf(Msid, "%-15ld", Aggrec.get_ItemA2()->get_Min());
			
	nCode=Aggrec.get_ItemA2()->get_Key();
	nOper_Id=Aggrec.get_ItemB()->get_OperId();
	memcpy(Account_Id, Aggrec.get_ItemB()->get_AccSessionId(), 8);  
	Account_Id[9]=NULL;
	memcpy(Correl_Id, Aggrec.get_ItemB()->get_CorreId(), 8);		
	Correl_Id[9]=NULL;
	nStart_Time = Aggrec.get_ItemC()->get_StartT();
	nEnd_Time = Aggrec.get_ItemC()->get_EndT();
	nLimit_Money = Aggrec.get_ItemD()->get_LimitT();
	nTop_Use = Aggrec.get_ItemD()->get_NTuseT();
	nServiceId = Aggrec.get_ItemE()->get_ServiceId();
	nCgClass = Aggrec.get_ItemE()->get_CgClass(); 
	nPacket1 = Aggrec.get_ItemE()->get_PacketN1();
	nPacket2 = Aggrec.get_ItemE()->get_PacketN2(); 
	nPacket3 = Aggrec.get_ItemE()->get_PacketN3(); 
	nUseMoney = Aggrec.get_ItemE()->get_UseMoney();
	
	nTid=Get_MaxTid()+1;
	//-------------------------------------변수할당 -----------------
	
	cout << "[TREQSCP] [INSERT] T_REQ_SCP : " << SysDate << ": " << nTid << endl;
	*LogM << "[TREQSCP] [INSERT] T_REQ_SCP : " << SysDate << " : " << nTid << LogM->ErrorCode(0) << endl;
	
	/* EXEC SQL INSERT INTO t_req_scp(sys_date, tid, code, msid, oper_id, 
									account_id, correl_id, start_time, end_time,
									limit_money, ntop_use, serviceid, cgclass, packet1, packet2, packet3, usemoney)
					values(:SysDate, :nTid, :nCode, :Msid, :nOper_Id, :Account_Id,
							:Correl_Id, :nStart_Time, :nEnd_Time, :nLimit_Money, :nTop_Use, :nServiceId, :nCgClass, :nPacket1, :nPacket2, :nPacket3, :nUseMoney); */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 17;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "insert into t_req_scp(sys_date,tid,code,msid,oper_id,account\
_id,correl_id,start_time,end_time,limit_money,ntop_use,serviceid,cgclass,packe\
t1,packet2,packet3,usemoney) values (:b0,:b1,:b2,:b3,:b4,:b5,:b6,:b7,:b8,:b9,:\
b10,:b11,:b12,:b13,:b14,:b15,:b16)";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )28;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlstm.sqhstv[0] = (         void  *)SysDate;
 sqlstm.sqhstl[0] = (unsigned int  )9;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&nTid;
 sqlstm.sqhstl[1] = (unsigned int  )4;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&nCode;
 sqlstm.sqhstl[2] = (unsigned int  )4;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)Msid;
 sqlstm.sqhstl[3] = (unsigned int  )16;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&nOper_Id;
 sqlstm.sqhstl[4] = (unsigned int  )4;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)Account_Id;
 sqlstm.sqhstl[5] = (unsigned int  )9;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)Correl_Id;
 sqlstm.sqhstl[6] = (unsigned int  )9;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         void  *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned int  )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (         void  *)&nStart_Time;
 sqlstm.sqhstl[7] = (unsigned int  )4;
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         void  *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned int  )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
 sqlstm.sqhstv[8] = (         void  *)&nEnd_Time;
 sqlstm.sqhstl[8] = (unsigned int  )4;
 sqlstm.sqhsts[8] = (         int  )0;
 sqlstm.sqindv[8] = (         void  *)0;
 sqlstm.sqinds[8] = (         int  )0;
 sqlstm.sqharm[8] = (unsigned int  )0;
 sqlstm.sqadto[8] = (unsigned short )0;
 sqlstm.sqtdso[8] = (unsigned short )0;
 sqlstm.sqhstv[9] = (         void  *)&nLimit_Money;
 sqlstm.sqhstl[9] = (unsigned int  )4;
 sqlstm.sqhsts[9] = (         int  )0;
 sqlstm.sqindv[9] = (         void  *)0;
 sqlstm.sqinds[9] = (         int  )0;
 sqlstm.sqharm[9] = (unsigned int  )0;
 sqlstm.sqadto[9] = (unsigned short )0;
 sqlstm.sqtdso[9] = (unsigned short )0;
 sqlstm.sqhstv[10] = (         void  *)&nTop_Use;
 sqlstm.sqhstl[10] = (unsigned int  )4;
 sqlstm.sqhsts[10] = (         int  )0;
 sqlstm.sqindv[10] = (         void  *)0;
 sqlstm.sqinds[10] = (         int  )0;
 sqlstm.sqharm[10] = (unsigned int  )0;
 sqlstm.sqadto[10] = (unsigned short )0;
 sqlstm.sqtdso[10] = (unsigned short )0;
 sqlstm.sqhstv[11] = (         void  *)&nServiceId;
 sqlstm.sqhstl[11] = (unsigned int  )4;
 sqlstm.sqhsts[11] = (         int  )0;
 sqlstm.sqindv[11] = (         void  *)0;
 sqlstm.sqinds[11] = (         int  )0;
 sqlstm.sqharm[11] = (unsigned int  )0;
 sqlstm.sqadto[11] = (unsigned short )0;
 sqlstm.sqtdso[11] = (unsigned short )0;
 sqlstm.sqhstv[12] = (         void  *)&nCgClass;
 sqlstm.sqhstl[12] = (unsigned int  )4;
 sqlstm.sqhsts[12] = (         int  )0;
 sqlstm.sqindv[12] = (         void  *)0;
 sqlstm.sqinds[12] = (         int  )0;
 sqlstm.sqharm[12] = (unsigned int  )0;
 sqlstm.sqadto[12] = (unsigned short )0;
 sqlstm.sqtdso[12] = (unsigned short )0;
 sqlstm.sqhstv[13] = (         void  *)&nPacket1;
 sqlstm.sqhstl[13] = (unsigned int  )4;
 sqlstm.sqhsts[13] = (         int  )0;
 sqlstm.sqindv[13] = (         void  *)0;
 sqlstm.sqinds[13] = (         int  )0;
 sqlstm.sqharm[13] = (unsigned int  )0;
 sqlstm.sqadto[13] = (unsigned short )0;
 sqlstm.sqtdso[13] = (unsigned short )0;
 sqlstm.sqhstv[14] = (         void  *)&nPacket2;
 sqlstm.sqhstl[14] = (unsigned int  )4;
 sqlstm.sqhsts[14] = (         int  )0;
 sqlstm.sqindv[14] = (         void  *)0;
 sqlstm.sqinds[14] = (         int  )0;
 sqlstm.sqharm[14] = (unsigned int  )0;
 sqlstm.sqadto[14] = (unsigned short )0;
 sqlstm.sqtdso[14] = (unsigned short )0;
 sqlstm.sqhstv[15] = (         void  *)&nPacket3;
 sqlstm.sqhstl[15] = (unsigned int  )4;
 sqlstm.sqhsts[15] = (         int  )0;
 sqlstm.sqindv[15] = (         void  *)0;
 sqlstm.sqinds[15] = (         int  )0;
 sqlstm.sqharm[15] = (unsigned int  )0;
 sqlstm.sqadto[15] = (unsigned short )0;
 sqlstm.sqtdso[15] = (unsigned short )0;
 sqlstm.sqhstv[16] = (         void  *)&nUseMoney;
 sqlstm.sqhstl[16] = (unsigned int  )4;
 sqlstm.sqhsts[16] = (         int  )0;
 sqlstm.sqindv[16] = (         void  *)0;
 sqlstm.sqinds[16] = (         int  )0;
 sqlstm.sqharm[16] = (unsigned int  )0;
 sqlstm.sqadto[16] = (unsigned short )0;
 sqlstm.sqtdso[16] = (unsigned short )0;
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
                          sqlstm.arrsiz = 17;
                          sqlstm.sqladtp = &sqladt;
                          sqlstm.sqltdsp = &sqltds;
                          sqlstm.iters = (unsigned int  )1;
                          sqlstm.offset = (unsigned int  )111;
                          sqlstm.cud = sqlcud0;
                          sqlstm.sqlest = (unsigned char  *)&sqlca;
                          sqlstm.sqlety = (unsigned short)0;
                          sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	else
	{
		cout << "[TREQSCP] [INSERT] T_REQ_SCP ERROR : " << (int)sqlca.sqlcode << endl;
		*LogM << "[TREQSCP] [INSERT] T_REQ_SCP ERROR : " << (int)sqlca.sqlcode <<  LogM->ErrorCode(0) << endl;
		/* EXEC SQL ROLLBACK; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 17;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )126;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		return 0;
	}
	return 1;
}



//------------------------------------------------------------------------
int	TReqScp::Get_ScpRecord(SCPFormat *Scprec, char *day, unsigned long n)
{
	if(DBerror) return 0;
	
	cout << "[TREQSCP] Get_ScpRecord()  : " << day << " : " << n << endl;
	//*LogM << "[TREQSCP] Get_ScpRecord() : " << day << " : " << n << LogM->ErrorCode(0) << endl;
	
	int		recFind=0;
	
	strcpy(SysDate, day);
	
	for(nTid=n+1; nTid < n+3; nTid++)
	{
		
		/* EXEC SQL SELECT sys_date, tid, code, msid, oper_id, 
						account_id, correl_id, start_time, end_time,
						limit_money, ntop_use, serviceid, cgclass, packet1, packet2, packet3, usemoney
				INTO	:SysDate, :nTid, :nCode, :Msid, :nOper_Id, :Account_Id,
						:Correl_Id, :nStart_Time, :nEnd_Time, :nLimit_Money, :nTop_Use,
						:nServiceId, :nCgClass, :nPacket1, :nPacket2, :nPacket3, :nUseMoney
			 	FROM 	t_req_scp
			 	WHERE 	sys_date=:SysDate and tid=:nTid; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 19;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select sys_date ,tid ,code ,msid ,oper_id ,account_id ,corr\
el_id ,start_time ,end_time ,limit_money ,ntop_use ,serviceid ,cgclass ,packet\
1 ,packet2 ,packet3 ,usemoney into :b0,:b1,:b2,:b3,:b4,:b5,:b6,:b7,:b8,:b9,:b1\
0,:b11,:b12,:b13,:b14,:b15,:b16  from t_req_scp where (sys_date=:b0 and tid=:b\
1)";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )141;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)0;
  sqlstm.sqhstv[0] = (         void  *)SysDate;
  sqlstm.sqhstl[0] = (unsigned int  )9;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&nTid;
  sqlstm.sqhstl[1] = (unsigned int  )4;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)&nCode;
  sqlstm.sqhstl[2] = (unsigned int  )4;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)Msid;
  sqlstm.sqhstl[3] = (unsigned int  )16;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         void  *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned int  )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (         void  *)&nOper_Id;
  sqlstm.sqhstl[4] = (unsigned int  )4;
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         void  *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned int  )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (         void  *)Account_Id;
  sqlstm.sqhstl[5] = (unsigned int  )9;
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         void  *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned int  )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (         void  *)Correl_Id;
  sqlstm.sqhstl[6] = (unsigned int  )9;
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         void  *)0;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned int  )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
  sqlstm.sqhstv[7] = (         void  *)&nStart_Time;
  sqlstm.sqhstl[7] = (unsigned int  )4;
  sqlstm.sqhsts[7] = (         int  )0;
  sqlstm.sqindv[7] = (         void  *)0;
  sqlstm.sqinds[7] = (         int  )0;
  sqlstm.sqharm[7] = (unsigned int  )0;
  sqlstm.sqadto[7] = (unsigned short )0;
  sqlstm.sqtdso[7] = (unsigned short )0;
  sqlstm.sqhstv[8] = (         void  *)&nEnd_Time;
  sqlstm.sqhstl[8] = (unsigned int  )4;
  sqlstm.sqhsts[8] = (         int  )0;
  sqlstm.sqindv[8] = (         void  *)0;
  sqlstm.sqinds[8] = (         int  )0;
  sqlstm.sqharm[8] = (unsigned int  )0;
  sqlstm.sqadto[8] = (unsigned short )0;
  sqlstm.sqtdso[8] = (unsigned short )0;
  sqlstm.sqhstv[9] = (         void  *)&nLimit_Money;
  sqlstm.sqhstl[9] = (unsigned int  )4;
  sqlstm.sqhsts[9] = (         int  )0;
  sqlstm.sqindv[9] = (         void  *)0;
  sqlstm.sqinds[9] = (         int  )0;
  sqlstm.sqharm[9] = (unsigned int  )0;
  sqlstm.sqadto[9] = (unsigned short )0;
  sqlstm.sqtdso[9] = (unsigned short )0;
  sqlstm.sqhstv[10] = (         void  *)&nTop_Use;
  sqlstm.sqhstl[10] = (unsigned int  )4;
  sqlstm.sqhsts[10] = (         int  )0;
  sqlstm.sqindv[10] = (         void  *)0;
  sqlstm.sqinds[10] = (         int  )0;
  sqlstm.sqharm[10] = (unsigned int  )0;
  sqlstm.sqadto[10] = (unsigned short )0;
  sqlstm.sqtdso[10] = (unsigned short )0;
  sqlstm.sqhstv[11] = (         void  *)&nServiceId;
  sqlstm.sqhstl[11] = (unsigned int  )4;
  sqlstm.sqhsts[11] = (         int  )0;
  sqlstm.sqindv[11] = (         void  *)0;
  sqlstm.sqinds[11] = (         int  )0;
  sqlstm.sqharm[11] = (unsigned int  )0;
  sqlstm.sqadto[11] = (unsigned short )0;
  sqlstm.sqtdso[11] = (unsigned short )0;
  sqlstm.sqhstv[12] = (         void  *)&nCgClass;
  sqlstm.sqhstl[12] = (unsigned int  )4;
  sqlstm.sqhsts[12] = (         int  )0;
  sqlstm.sqindv[12] = (         void  *)0;
  sqlstm.sqinds[12] = (         int  )0;
  sqlstm.sqharm[12] = (unsigned int  )0;
  sqlstm.sqadto[12] = (unsigned short )0;
  sqlstm.sqtdso[12] = (unsigned short )0;
  sqlstm.sqhstv[13] = (         void  *)&nPacket1;
  sqlstm.sqhstl[13] = (unsigned int  )4;
  sqlstm.sqhsts[13] = (         int  )0;
  sqlstm.sqindv[13] = (         void  *)0;
  sqlstm.sqinds[13] = (         int  )0;
  sqlstm.sqharm[13] = (unsigned int  )0;
  sqlstm.sqadto[13] = (unsigned short )0;
  sqlstm.sqtdso[13] = (unsigned short )0;
  sqlstm.sqhstv[14] = (         void  *)&nPacket2;
  sqlstm.sqhstl[14] = (unsigned int  )4;
  sqlstm.sqhsts[14] = (         int  )0;
  sqlstm.sqindv[14] = (         void  *)0;
  sqlstm.sqinds[14] = (         int  )0;
  sqlstm.sqharm[14] = (unsigned int  )0;
  sqlstm.sqadto[14] = (unsigned short )0;
  sqlstm.sqtdso[14] = (unsigned short )0;
  sqlstm.sqhstv[15] = (         void  *)&nPacket3;
  sqlstm.sqhstl[15] = (unsigned int  )4;
  sqlstm.sqhsts[15] = (         int  )0;
  sqlstm.sqindv[15] = (         void  *)0;
  sqlstm.sqinds[15] = (         int  )0;
  sqlstm.sqharm[15] = (unsigned int  )0;
  sqlstm.sqadto[15] = (unsigned short )0;
  sqlstm.sqtdso[15] = (unsigned short )0;
  sqlstm.sqhstv[16] = (         void  *)&nUseMoney;
  sqlstm.sqhstl[16] = (unsigned int  )4;
  sqlstm.sqhsts[16] = (         int  )0;
  sqlstm.sqindv[16] = (         void  *)0;
  sqlstm.sqinds[16] = (         int  )0;
  sqlstm.sqharm[16] = (unsigned int  )0;
  sqlstm.sqadto[16] = (unsigned short )0;
  sqlstm.sqtdso[16] = (unsigned short )0;
  sqlstm.sqhstv[17] = (         void  *)SysDate;
  sqlstm.sqhstl[17] = (unsigned int  )9;
  sqlstm.sqhsts[17] = (         int  )0;
  sqlstm.sqindv[17] = (         void  *)0;
  sqlstm.sqinds[17] = (         int  )0;
  sqlstm.sqharm[17] = (unsigned int  )0;
  sqlstm.sqadto[17] = (unsigned short )0;
  sqlstm.sqtdso[17] = (unsigned short )0;
  sqlstm.sqhstv[18] = (         void  *)&nTid;
  sqlstm.sqhstl[18] = (unsigned int  )4;
  sqlstm.sqhsts[18] = (         int  )0;
  sqlstm.sqindv[18] = (         void  *)0;
  sqlstm.sqinds[18] = (         int  )0;
  sqlstm.sqharm[18] = (unsigned int  )0;
  sqlstm.sqadto[18] = (unsigned short )0;
  sqlstm.sqtdso[18] = (unsigned short )0;
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


			 	
		if(sqlca.sqlcode == 0 || sqlca.sqlcode == -1405) 
		{
			//cout << "[TREQSCP] FIND : " << SysDate << " : " << nTid << endl;
			recFind=1;
			break;
		}//else cout << "[TREQSCP] Not FIND : " << sqlca.sqlcode << ": " << SysDate << " : " << nTid << endl;
	}
	
	
	if(!recFind)
	{
		int			nRet;
		time_t		Time;	
		struct  tm *Tm;
	
		time(&Time);	
		Tm = localtime(&Time);	
		sprintf(SysDate, "%04d%02d%02d",Tm->tm_year+1900, Tm->tm_mon+1, Tm->tm_mday);	
		if(!strncmp(SysDate, day, 8)) return 0;	// 일자가 오늘일자이면 No Data
	
		nTid=1;
		/* EXEC SQL SELECT sys_date, tid, code, msid, oper_id, 
						account_id, correl_id, start_time, end_time,
						limit_money, ntop_use, serviceid, cgclass, packet1, packet2, packet3, usemoney
			 	INTO	:SysDate, :nTid, :nCode, :Msid, :nOper_Id, :Account_Id,
						:Correl_Id, :nStart_Time, :nEnd_Time, :nLimit_Money, :nTop_Use,
						:nServiceId, :nCgClass, :nPacket1, :nPacket2, :nPacket3, :nUseMoney
			 	FROM 	t_req_scp
			 	WHERE 	sys_date=:SysDate and tid=:nTid; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 19;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select sys_date ,tid ,code ,msid ,oper_id ,account_id ,corr\
el_id ,start_time ,end_time ,limit_money ,ntop_use ,serviceid ,cgclass ,packet\
1 ,packet2 ,packet3 ,usemoney into :b0,:b1,:b2,:b3,:b4,:b5,:b6,:b7,:b8,:b9,:b1\
0,:b11,:b12,:b13,:b14,:b15,:b16  from t_req_scp where (sys_date=:b0 and tid=:b\
1)";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )232;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)0;
  sqlstm.sqhstv[0] = (         void  *)SysDate;
  sqlstm.sqhstl[0] = (unsigned int  )9;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&nTid;
  sqlstm.sqhstl[1] = (unsigned int  )4;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)&nCode;
  sqlstm.sqhstl[2] = (unsigned int  )4;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)Msid;
  sqlstm.sqhstl[3] = (unsigned int  )16;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         void  *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned int  )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (         void  *)&nOper_Id;
  sqlstm.sqhstl[4] = (unsigned int  )4;
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         void  *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned int  )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (         void  *)Account_Id;
  sqlstm.sqhstl[5] = (unsigned int  )9;
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         void  *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned int  )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (         void  *)Correl_Id;
  sqlstm.sqhstl[6] = (unsigned int  )9;
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         void  *)0;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned int  )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
  sqlstm.sqhstv[7] = (         void  *)&nStart_Time;
  sqlstm.sqhstl[7] = (unsigned int  )4;
  sqlstm.sqhsts[7] = (         int  )0;
  sqlstm.sqindv[7] = (         void  *)0;
  sqlstm.sqinds[7] = (         int  )0;
  sqlstm.sqharm[7] = (unsigned int  )0;
  sqlstm.sqadto[7] = (unsigned short )0;
  sqlstm.sqtdso[7] = (unsigned short )0;
  sqlstm.sqhstv[8] = (         void  *)&nEnd_Time;
  sqlstm.sqhstl[8] = (unsigned int  )4;
  sqlstm.sqhsts[8] = (         int  )0;
  sqlstm.sqindv[8] = (         void  *)0;
  sqlstm.sqinds[8] = (         int  )0;
  sqlstm.sqharm[8] = (unsigned int  )0;
  sqlstm.sqadto[8] = (unsigned short )0;
  sqlstm.sqtdso[8] = (unsigned short )0;
  sqlstm.sqhstv[9] = (         void  *)&nLimit_Money;
  sqlstm.sqhstl[9] = (unsigned int  )4;
  sqlstm.sqhsts[9] = (         int  )0;
  sqlstm.sqindv[9] = (         void  *)0;
  sqlstm.sqinds[9] = (         int  )0;
  sqlstm.sqharm[9] = (unsigned int  )0;
  sqlstm.sqadto[9] = (unsigned short )0;
  sqlstm.sqtdso[9] = (unsigned short )0;
  sqlstm.sqhstv[10] = (         void  *)&nTop_Use;
  sqlstm.sqhstl[10] = (unsigned int  )4;
  sqlstm.sqhsts[10] = (         int  )0;
  sqlstm.sqindv[10] = (         void  *)0;
  sqlstm.sqinds[10] = (         int  )0;
  sqlstm.sqharm[10] = (unsigned int  )0;
  sqlstm.sqadto[10] = (unsigned short )0;
  sqlstm.sqtdso[10] = (unsigned short )0;
  sqlstm.sqhstv[11] = (         void  *)&nServiceId;
  sqlstm.sqhstl[11] = (unsigned int  )4;
  sqlstm.sqhsts[11] = (         int  )0;
  sqlstm.sqindv[11] = (         void  *)0;
  sqlstm.sqinds[11] = (         int  )0;
  sqlstm.sqharm[11] = (unsigned int  )0;
  sqlstm.sqadto[11] = (unsigned short )0;
  sqlstm.sqtdso[11] = (unsigned short )0;
  sqlstm.sqhstv[12] = (         void  *)&nCgClass;
  sqlstm.sqhstl[12] = (unsigned int  )4;
  sqlstm.sqhsts[12] = (         int  )0;
  sqlstm.sqindv[12] = (         void  *)0;
  sqlstm.sqinds[12] = (         int  )0;
  sqlstm.sqharm[12] = (unsigned int  )0;
  sqlstm.sqadto[12] = (unsigned short )0;
  sqlstm.sqtdso[12] = (unsigned short )0;
  sqlstm.sqhstv[13] = (         void  *)&nPacket1;
  sqlstm.sqhstl[13] = (unsigned int  )4;
  sqlstm.sqhsts[13] = (         int  )0;
  sqlstm.sqindv[13] = (         void  *)0;
  sqlstm.sqinds[13] = (         int  )0;
  sqlstm.sqharm[13] = (unsigned int  )0;
  sqlstm.sqadto[13] = (unsigned short )0;
  sqlstm.sqtdso[13] = (unsigned short )0;
  sqlstm.sqhstv[14] = (         void  *)&nPacket2;
  sqlstm.sqhstl[14] = (unsigned int  )4;
  sqlstm.sqhsts[14] = (         int  )0;
  sqlstm.sqindv[14] = (         void  *)0;
  sqlstm.sqinds[14] = (         int  )0;
  sqlstm.sqharm[14] = (unsigned int  )0;
  sqlstm.sqadto[14] = (unsigned short )0;
  sqlstm.sqtdso[14] = (unsigned short )0;
  sqlstm.sqhstv[15] = (         void  *)&nPacket3;
  sqlstm.sqhstl[15] = (unsigned int  )4;
  sqlstm.sqhsts[15] = (         int  )0;
  sqlstm.sqindv[15] = (         void  *)0;
  sqlstm.sqinds[15] = (         int  )0;
  sqlstm.sqharm[15] = (unsigned int  )0;
  sqlstm.sqadto[15] = (unsigned short )0;
  sqlstm.sqtdso[15] = (unsigned short )0;
  sqlstm.sqhstv[16] = (         void  *)&nUseMoney;
  sqlstm.sqhstl[16] = (unsigned int  )4;
  sqlstm.sqhsts[16] = (         int  )0;
  sqlstm.sqindv[16] = (         void  *)0;
  sqlstm.sqinds[16] = (         int  )0;
  sqlstm.sqharm[16] = (unsigned int  )0;
  sqlstm.sqadto[16] = (unsigned short )0;
  sqlstm.sqtdso[16] = (unsigned short )0;
  sqlstm.sqhstv[17] = (         void  *)SysDate;
  sqlstm.sqhstl[17] = (unsigned int  )9;
  sqlstm.sqhsts[17] = (         int  )0;
  sqlstm.sqindv[17] = (         void  *)0;
  sqlstm.sqinds[17] = (         int  )0;
  sqlstm.sqharm[17] = (unsigned int  )0;
  sqlstm.sqadto[17] = (unsigned short )0;
  sqlstm.sqtdso[17] = (unsigned short )0;
  sqlstm.sqhstv[18] = (         void  *)&nTid;
  sqlstm.sqhstl[18] = (unsigned int  )4;
  sqlstm.sqhsts[18] = (         int  )0;
  sqlstm.sqindv[18] = (         void  *)0;
  sqlstm.sqinds[18] = (         int  )0;
  sqlstm.sqharm[18] = (unsigned int  )0;
  sqlstm.sqadto[18] = (unsigned short )0;
  sqlstm.sqtdso[18] = (unsigned short )0;
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


			 	
		if(sqlca.sqlcode != 0 && sqlca.sqlcode != -1405)  
		{
			//cout << "[TREQSCP] Not FIND : " << sqlca.sqlcode << ": " << SysDate << " : " << nTid << endl;
			return 0;	// No Data
		}//else cout << "[TREQSCP] FIND : " << SysDate << " : " << nTid << endl;
	}
			 
	
	Scprec->get_ItemA()->set_Key(nCode);
	Scprec->get_ItemA()->set_MSID(Msid);
	
	Scprec->get_ItemB()->set_OperId(nOper_Id);
	Scprec->get_ItemB()->set_AccSessionId(Account_Id);
	Scprec->get_ItemB()->set_CorreId(Correl_Id);
	
	Scprec->get_ItemC()->set_StartT(nStart_Time);
	Scprec->get_ItemC()->set_EndT(nEnd_Time);
	
	Scprec->get_ItemD()->set_LimitT(nLimit_Money);
	Scprec->get_ItemD()->set_NTuseT(nTop_Use);
	
	Scprec->get_ItemE()->set_ServiceId(nServiceId);
	Scprec->get_ItemE()->set_CgClass(nCgClass);
	Scprec->get_ItemE()->set_PacketN1(nPacket1);
	Scprec->get_ItemE()->set_PacketN2(nPacket2);
	Scprec->get_ItemE()->set_PacketN3(nPacket3);
	Scprec->get_ItemE()->set_UseMoney(nUseMoney);
	
	/*
	cout << "[TREQSCP] nCgClass : " << nCgClass <<  endl;
	cout << "[TREQSCP] nPacket1 : " << nPacket1 <<  endl;
	cout << "[TREQSCP] nPacket2 : " << nPacket2 <<  endl;
	cout << "[TREQSCP] nPacket3 : " << nPacket3 <<  endl;
	cout << "[TREQSCP] nUseMoney : " << nUseMoney <<  endl;
	*/
	
	return 1;
}





//--------------------------------------------------------------------
/*
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
	
	
	
	TReqScp EventHst(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]), atoi(argv[6]));
	EventHst.Set_TReqScp(4, 0, 0, "aaaaaaaaaaaaaaaaaaaaa");
	EventHst.Set_TReqScp(4, 1, 1, "bbbbbbbbbbbbbbbbbbbbb");
	
	clsDbConnect.disconnect();
	exit(1);
}
*/
