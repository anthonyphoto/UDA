
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
    "headermanager.pc"
};


static unsigned long sqlctx = 4600731;


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
};


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

/* EXEC SQL INCLUDE "../INCLUDE/headermanager.hpp";
 */ 
#ifndef 	_HEADERMANAGER_HPP
#define 	_HEADERMANAGER_HPP

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#define SQLCA_STORAGE_CLASS extern
#include <sqlca.h>
#include "../INCLUDE/FileLogWrite.hpp"
#include "../INCLUDE/collectorRule.hpp"

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
        char        mapping_name    [7+1];
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

            char          m_strMappingName[7+1];   // Mapping Name
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

#define		MAX_OUT_DATA_LEN 	128
#define		MAX_IN_LEN			64
#define		FAILHEADERMANAGER	-1
#define		OKHEADERMANAGER		1

//////////////////////////////////////////////////////////////////////
//
//   Souce Name  	: headermanager.hpp
//   Usage          : data 형태의 변환을 하고 header를 생성하는 
//					  class의 prototype
//   DATE           : 2001. 4 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
class CHeaderMannager : public CLoadActTable
{
	private :
		int 	nActIndex;					// 	table에서 한 Record를 읽기 위한 Index (seq_no)
		char*	strOutputData;				//	output을 저장할 data
		char*	strOutOneData;				// 	output one datas
		char*	strHeaderData;				//	header data 저장 변수 

		// 0번지에서 얻는 정보 저장변수 
		int 	nOutTotLen;					// 	output data 총 길이 

		int 	nFieldCnt;					//	field의 갯수 
		
		int* 	nInOneLen;					//	one data length (내부 저장값)
		int* 	nInOffset;					//	one data offset (내부 저장값)
		int*	nDataType;					// 	DataType(input의 형을 알기 위해서 생성자에서 저장)
		int 	nInTotLen;					//	header data 생성시 총 길이 
		int 	nInputCnt;					//	input data의 count (내부 순차 증가값)
		int 	nOutOneLen;
		
	public :
        FileLogWrite*  	LogHeader;			// log 정의 
        
		CHeaderMannager(char *strMappingName,char *strActVer);
		~CHeaderMannager();
        //	accessor
		int 	get_OutTotLen()		{	return 	nOutTotLen;	}

		//	put 정의 
		void 	put_Data		(void* value);	//	data를 입력하는 부분 

		int 	headerRule		(char* indata, char *outdata);

		void 	DataMove		(char *InBuf, char *OutBuf, int nTempOutOffset);
};

#endif 
//////////////////////////////////////////////////////////////////////
//
//   Function Name  : CHeaderMannager
//   Usage          : header(file,block)을 생성하는 함수의 생성자
//	 Argument		: char* strMappingName (기본 meta table key)
//					  char* strActVer      (기본 meta table key)
//	 Return			: 
//   DATE           : 2001. 4 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
CHeaderMannager::CHeaderMannager(char *strMappingName,char *strActVer) : CLoadActTable(strMappingName, strActVer)
{
	nInputCnt = 0;		
	nInTotLen = 0;			
	
	//	header를 저장 변수 선언 및 초기화 
	strHeaderData 	= new char[MAX_OUT_DATA_LEN];
	memset (strHeaderData, NULL, MAX_OUT_DATA_LEN);
	
	char strTempLog[64];	
    sprintf(strTempLog, "%s%s",strMappingName, "log");
    LogHeader	= new FileLogWrite(".", strTempLog);
    LogHeader->Write("-------------------------\n");
	nActIndex 	= m_nActStartRecIndex;		//	action table에서 start 정보를 가져온다 (seq_no가 0이다)
	
	//	meta talbe seq_no = 0 정보 저장 
	nFieldCnt	= m_ActionTable[nActIndex].In_Field_Offset;		//	field의 갯수 
	nOutTotLen 	= m_ActionTable[nActIndex].Out_Field_Length;	// 	out data length 

	//	indata 정보 저장값 선언
	nInOneLen 	= new int[nFieldCnt+1];		// 	0번지는 사용을 하지 않으므로 +1을 한다 
	nInOffset 	= new int[nFieldCnt+1];		// 	0번지는 사용을 하지 않으므로 +1을 한다 
	nDataType	= new int[nFieldCnt+1];		// 	0번지는 사용을 하지 않으므로 +1을 한다 

	++nActIndex;
	++nInputCnt;

	for(; nInputCnt <= nFieldCnt ; nInputCnt++)
	{
	    nDataType[nInputCnt]   = atoi(m_ActionTable[nInputCnt].Data_Type);
	}
	
	nInputCnt = 1;
	
	LogHeader->Write("[headerManager]nActIndex = %d",nActIndex);
	LogHeader->Write("[headerManager]nFieldCnt = %d",nFieldCnt);
	LogHeader->Write("[headerManager]nOutTotLen = %d",nOutTotLen);
}

//	소멸자 
CHeaderMannager::~CHeaderMannager()
{
	delete LogHeader;
	delete[] nInOneLen;
	delete[] nInOffset;
	delete[] nDataType;
	
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : headerRule
//   Usage          : UDR을 file 만들어 주는 collector에서 data를 변형
//					  해 주는 함수 
//	 Argument		: char* indata 		(input data record)
//					  char* outdata     (변형을 한 output data record)
//	 Return			: int (FAILheaderManager : -1 , OKheaderManager : 1)
//   DATE           : 2001. 4 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
int CHeaderMannager::headerRule (char* indata, char *outdata)
{
	CCollectorRule		hdRule;
	int 			nTempNullCnt;
	int 			nActionCode;
	char 			strSubAction;
	int 			nOutOffset;
	

	strOutOneData 		= new char[MAX_OUT_DATA_LEN];

	LogHeader->Write("[headerManager||headerRule]nActIndex = %d",nActIndex);
    for(;nActIndex<=nFieldCnt;nActIndex++)
    {
		// loop에서 필요한 정보 초기화 
		nTempNullCnt = 0;
		memset (strOutOneData, NULL, MAX_OUT_DATA_LEN);
		
		//	구성정보 얻는 부분
		nOutOffset  = m_ActionTable[nActIndex].Out_Field_Offset;
	    nActionCode = atoi(m_ActionTable[nActIndex].Action_Code);
	    memcpy(&strSubAction, m_ActionTable[nActIndex].Sub_Action_Code, 1);
	    nOutOneLen  = m_ActionTable[nActIndex].Out_Field_Length;
		
		LogHeader->Write("[headerManager||headerRule]nInOffset    = %d",nInOffset);
		LogHeader->Write("[headerManager||headerRule]nDataType    = %d",nDataType);
		LogHeader->Write("[headerManager||headerRule]nActionCode  = %d",nActionCode);
		LogHeader->Write("[headerManager||headerRule]strSubAction = %d",strSubAction);
		LogHeader->Write("[headerManager||headerRule]nOutOneLen   = %d",nOutOneLen);
		
        //	one field 처리 
        memcpy(strOutOneData, hdRule.oneField(strHeaderData,nInOffset[nActIndex],nInOneLen[nActIndex],
        					  nOutOneLen,nDataType[nActIndex],nActionCode,strSubAction), hdRule.get_OneFieldLen());
        
		LogHeader->Write("[headerManager||headerRule]strOutOneData   = %s",strOutOneData);

/*        //	one field의 data가 없을 경우 처리 (false 처리)
        for(int i = 0; i < MAX_OUT_DATA_LEN ; i++)
        {
        	if (strOutOneData[i] == NULL)
        	{
        		nTempNullCnt++;
        	}
        }
        if (nTempNullCnt == MAX_OUT_DATA_LEN)
        {
			delete[] strOutOneData;
        	return FAILHEADERMANAGER;
        } */
       	DataMove(strOutOneData, outdata, nOutOffset);			//	변형된 header one data를 out buffer에 저장
    }
   	DataMove(indata, outdata, nOutTotLen);						// 	header data에 input data를 덧붙이기 

	//	다음 data를 위한 초기화 
	nActIndex = 1;	
	nInputCnt = 1;
	nInTotLen = 0;
   	memset(strHeaderData, NULL, MAX_OUT_DATA_LEN);
	delete[] strOutOneData;
    return OKHEADERMANAGER;
}	

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : DataMove
//   Usage          : 하나의 처리된 결과를 out buffer에 쌓아주는 부분 
//	 Argument		: char* indata 		(input data record)
//					  char* outdata     (변형을 한 output data record)
//	 Return			: 
//   DATE           : 2001. 4 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
void CHeaderMannager::DataMove(char *InBuf, char *OutBuf, int nTempOutOffset)
{
	LogHeader->Write("[headerManager||DataMove]nOutOffSet   = %d",nTempOutOffset);
	LogHeader->Write("[headerManager||DataMove]nOutLen      = %d\n",nOutOneLen);
    memcpy(OutBuf+nTempOutOffset,InBuf,nOutOneLen);
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : put_Data
//   Usage          : header에 필요한 input정보를 저장하는 함수 
//	 Argument		: void* value 		(header에 들어갈 value)
//	 Return			: 
//   DATE           : 2001. 4 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
void CHeaderMannager::put_Data(void* value)
{
	int 	nInDataType;

	nInDataType = nDataType[nInputCnt] / 10;
	switch (nInDataType)
	{
		case  0 :  
		case  1 :  
		case  2 :  
		case  3 :  
		case  9 :  
					nInOneLen[nInputCnt] = sizeof(int);
					nInTotLen += nInOneLen[nInputCnt] ; 
					nInOffset[nInputCnt] = nInTotLen - sizeof(int);
					LogHeader->Write("[headerManager||put_Data]nInputCnt              = %d",nInputCnt);
					LogHeader->Write("[headerManager||put_Data]nInOneLen[nInputCnt]   = %d",nInOneLen[nInputCnt]);
					LogHeader->Write("[headerManager||put_Data]nInTotLen              = %d",nInTotLen);
					LogHeader->Write("[headerManager||put_Data]nInOffset[nInputCnt]   = %d",nInOffset[nInputCnt]);
					break;
		case  5 :
		case  6 :
					nInOneLen[nInputCnt] = sizeof(short);
					nInTotLen += nInOneLen[nInputCnt] ; 
					nInOffset[nInputCnt] = nInTotLen - sizeof(short);
					LogHeader->Write("[headerManager||put_Data]nInputCnt              = %d",nInputCnt);
					LogHeader->Write("[headerManager||put_Data]nInOneLen[nInputCnt]   = %d",nInOneLen[nInputCnt]);
					LogHeader->Write("[headerManager||put_Data]nInTotLen              = %d",nInTotLen);
					LogHeader->Write("[headerManager||put_Data]nInOffset[nInputCnt]   = %d",nInOffset[nInputCnt]);
					break;
		case  7 :
					nInOneLen[nInputCnt] = sizeof(char);
					nInTotLen += nInOneLen[nInputCnt] ; 
					nInOffset[nInputCnt] = nInTotLen - sizeof(char);
					LogHeader->Write("[headerManager||put_Data]nInputCnt              = %d",nInputCnt);
					LogHeader->Write("[headerManager||put_Data]nInOneLen[nInputCnt]   = %d",nInOneLen[nInputCnt]);
					LogHeader->Write("[headerManager||put_Data]nInTotLen              = %d",nInTotLen);
					LogHeader->Write("[headerManager||put_Data]nInOffset[nInputCnt]   = %d",nInOffset[nInputCnt]);
					break;
		case  8 : 
					nInOneLen[nInputCnt] = strlen((char*)value);
					nInTotLen += nInOneLen[nInputCnt] ; 
					nInOffset[nInputCnt] = nInTotLen - strlen((char*)value);
					LogHeader->Write("[headerManager||put_Data]nInputCnt              = %d",nInputCnt);
					LogHeader->Write("[headerManager||put_Data]nInOneLen[nInputCnt]   = %d",nInOneLen[nInputCnt]);
					LogHeader->Write("[headerManager||put_Data]nInTotLen              = %d",nInTotLen);
					LogHeader->Write("[headerManager||put_Data]nInOffset[nInputCnt]   = %d",nInOffset[nInputCnt]);
					break;
	}
	DataMove((char*)value, strHeaderData, nInOffset[nInputCnt]);
	++nInputCnt;
}

