
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
    "DbConnectFail.pc"
};


static unsigned long sqlctx = 4493611;


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
5,0,0,1,0,0,27,50,0,0,4,4,0,1,0,1,97,0,0,1,10,0,0,1,10,0,0,1,10,0,0,
};


/*****************************************************************************************
* file : DbConnectFail.pc
*
* DB 접속 실패시 NMS로 직접 Trap을 발생시킨다.
* Trap 테스트용 프로그램.
*
*																						01-06-08 4:22오후 ky.
*****************************************************************************************/
#include <sqlca.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




// Define
//----------------------------------------------------------
static const char *D_TRAP_DEST_IP_FILE      = "/var/SNMP/CONF/TrapDestIp.conf";

static const char *D_TRAP_PROC = "/opt/OV/bin/snmptrap";    // trap process
static const char *D_TRAP_ARGV = "\"\" \"\" 6 2 0 1.3.6.1.4.1.9608.4.2.0 octetstringascii";
  // trap default argument




// Global variable
//----------------------------------------------------------
int  g_nDestIpCnt = 0;
char g_ptrDestIp[100][100];




// Function Prototype
//----------------------------------------------------------
bool GetTrapDestIp();
void DbConnectFail();




// Main
//----------------------------------------------------------
int main()
{
	if (!GetTrapDestIp()) return -1;		// Trap Destination ip read fail.
		
	/* EXEC SQL CONNECT :"iuda/uda"; */ 

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
 sqlstm.sqhstv[0] = (         void  *)"iuda/uda";
 sqlstm.sqhstl[0] = (unsigned int  )0;
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


	
   if (sqlca.sqlcode != 0) DbConnectFail();
	
	return 0;
}//end main




bool GetTrapDestIp()
{
   g_nDestIpCnt = 0;
   
   // trap nms ip 환경파일이 없으면 에러 발생.
   FILE *fd;
   if ((fd = fopen(D_TRAP_DEST_IP_FILE, "r")) == NULL) {
      printf("%s open fail.\n\n", D_TRAP_DEST_IP_FILE);
      return false;
   }

   // ip 갯수 체크.
   char strDestIp[100];
   while (fgets(strDestIp, sizeof(strDestIp), fd))
      g_nDestIpCnt++;

   fseek(fd, 0, SEEK_SET);
   for (int i=0; i<g_nDestIpCnt; i++)
   {
      fgets(strDestIp, sizeof(strDestIp), fd);
      strtok(strDestIp, "\n");
      strcpy(g_ptrDestIp[i], strDestIp);
   }
   fclose(fd);

   return true;
}//end GetTrapDestIp



void DbConnectFail()
{
	int nRet;
	char strCommand[200];
		
   for (int i=0; i<g_nDestIpCnt; i++) {
      sprintf(strCommand, "%s %s %s \"%s(%d) : [%d] - %s\"",
         D_TRAP_PROC, g_ptrDestIp[i], D_TRAP_ARGV,
         "udaTrapSqlConnect", 5000, 0, "DB Connect fail!!");

      printf("    Trap : %s\n", strCommand);
      nRet = system(strCommand);       // run trap process. 성공일때 0 return.
                                       //                   실패시 256 or 다른값 return.
      if (nRet != 0)
         printf("    [ERROR] Trap fail. return value : %d\n", nRet);
   }
}



