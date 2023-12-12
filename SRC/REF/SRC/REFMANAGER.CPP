
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
    "refmanager.pc"
};


static unsigned long sqlctx = 623515;


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

/* EXEC SQL INCLUDE "../INCLUDE/refmanager.hpp";
 */ 
#ifndef 	_REFMANAGER_HPP
#define 	_REFMANAGER_HPP

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#define SQLCA_STORAGE_CLASS extern
#include <sqlca.h>
#include "../INCLUDE/FileLogWrite.hpp"
#include "../INCLUDE/collectorRule.hpp"
#include "../INCLUDE/pdsnRule.hpp"

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

#define 	OUT_MAX_BUFFER		480
#define		MAX_OUT_DATA_LEN 	128
#define		FAILREFMANAGER		-1
#define		OKREFMANAGER		1

//FileLogWrite*  	LogM;				// log 정의 
//////////////////////////////////////////////////////////////////////
//
//   Souce Name  	: refmanager.hpp
//   Usage          : data 형태의 변환을 하가 위한 class의 prototype
//   DATE           : 2001. 3 .
//   LAST UPDATE    : 2001. 5. 21
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//					  2001. 5. 21 Version update
//   Version 		: 1.1 (2001. 5. 21)
//
//////////////////////////////////////////////////////////////////////
class CRefMannager : public CLoadActTable
{
	private :
		int 	nActIndex;					// 	table에서 한 Record를 읽기 위한 Index (seq_no)
//		char*	strInputData;				//	Input을 저장한 data
		char*	strOutputData;				//	output을 저장할 data
		char*	strOutOneData;				// 	output one datas

		// 0번지에서 얻는 정보 저장변수 
		int 	nInTotLen;					//	input data 총 길이
		int 	nOutTotLen;					// 	output data 총 길이 
		int 	nZeroActionCode;			//	out mode를 결정하는 action code (0번지 값)

		int		nInOffset;					// 	해당 Input Field의 위치
		int 	nInOneLen;					//	input one data 길이
		int 	nOutOneLen;					//	변환될 한개의 field의 길이 
		int 	nOutOffset;					//	output data offset
		int 	nFieldCnt;					//	field의 갯수 
		
		C017Pdsn*	pdsn;					//	pdsn dbhandler 인스턴스 선언
				
	public :
        FileLogWrite*  	LogM;				// log 정의 
        
        //	accessor
        int 	get_InTotLen()		{	return 	nInTotLen;	}
		int 	get_OutTotLen()		{	return 	nOutTotLen;	}
		
		CRefMannager(char *strMappingName,char *strActVer);
		~CRefMannager();
		
		int 	collectorRule	(char* indata, char *outdata);
		int 	pdsnRule		(char* indata, char* outdata);

		void 	DataMove		(char *InBuf, char *OutBuf, int nTempOutOffset);
};

#endif 



//////////////////////////////////////////////////////////////////////
//
//   Function Name  : CRefMannager
//   Usage          : collector, RFC2138, 2139 변환 프로그램의 생성자
//	 Argument		: char* strMappingName (기본 meta table key)
//					  char* strActVer      (기본 meta table key)
//	 Return			: 
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
CRefMannager::CRefMannager(char *strMappingName,char *strActVer) : CLoadActTable(strMappingName, strActVer)
{
	char strTempLog[64];	
//  sprintf(strTempLog, "%s%s",strMappingName, "log");
//  LogM		 = new FileLogWrite(".", strTempLog);
//  LogM->Write("-------------------------\n");
	nActIndex = m_nActStartRecIndex;		//	action table에서 start 정보를 가져온다 (seq_no가 0이다)
	
	//	meta talbe seq_no = 0 정보 저장 
	nInTotLen 		= m_ActionTable[nActIndex].In_Field_Length;		//	input total length
	nZeroActionCode = atoi(m_ActionTable[nActIndex].Action_Code);	//	action code에 따른 DB선택
	nFieldCnt	 	= m_ActionTable[nActIndex].In_Field_Offset;		//	field의 갯수 

	switch(nZeroActionCode)											//	각 형태별로 output의 구성 정보를 얻는다.
	{
		case 1	:													//	field to field table 정의를 따른다.
			nOutTotLen = m_ActionTable[nActIndex].Out_Field_Length;
			break;
		case 2	:													// 	017 Pdsn (RFC2138, 2139)
			pdsn = new C017Pdsn;									//	pdsn 사용하기 위한 선언 (memory load)
			nOutTotLen = 0;											//	pdsn code는 총길이를 구하기 위해서 초기화 
			break;
		default :
//			LogM->Write("[refmanager]meta action table seq_no 0 error (action code)");
			exit(0);
			break;
	}
	++nActIndex;
/***
	LogM->Write("[refmanager]nActIndex = %d",nActIndex);
	LogM->Write("[refmanager]nInTotLen = %d",nInTotLen);
	LogM->Write("[refmanager]nZeroActionCode = %d",nZeroActionCode);
	LogM->Write("[refmanager]nFieldCnt = %d",nFieldCnt);
	LogM->Write("[refmanager]nOutTotLen = %d",nOutTotLen);
***/
}

//	소멸자 
CRefMannager::~CRefMannager()
{
//	delete LogM;

	if (nZeroActionCode == 2)
	{
		delete pdsn;
	}
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : collectorRule
//   Usage          : UDR을 file 만들어 주는 collector에서 data를 변형
//					  해 주는 함수 
//	 Argument		: char* indata 		(input data record)
//					  char* outdata     (변형을 한 output data record)
//	 Return			: int (FAILREFMANAGER : -1 , OKREFMANAGER : 1)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
int CRefMannager::collectorRule (char* indata, char *outdata)
{
	CCollectorRule	cltRule;
	int 			nTempNullCnt;
	int 			nDataType;
	int 			nActionCode;
	char 			strSubAction;

	strOutOneData 		= new char[MAX_OUT_DATA_LEN];

//	LogM->Write("[refmanager||collectorRule]nActIndex = %d",nActIndex);
    for(;nActIndex<=nFieldCnt;nActIndex++)
    {
		// loop에서 필요한 정보 초기화 
		nTempNullCnt = 0;
		memset (strOutOneData, NULL, MAX_OUT_DATA_LEN);
		
		//	구성정보 얻는 부분
		nInOffset   = m_ActionTable[nActIndex].In_Field_Offset;
		nOutOffset  = m_ActionTable[nActIndex].Out_Field_Offset;
	    nDataType   = atoi(m_ActionTable[nActIndex].Data_Type);
	    nActionCode = atoi(m_ActionTable[nActIndex].Action_Code);
	    memcpy(&strSubAction, m_ActionTable[nActIndex].Sub_Action_Code, 1);
	    nOutOneLen  = m_ActionTable[nActIndex].Out_Field_Length;
	    nInOneLen   = m_ActionTable[nActIndex].In_Field_Length;
/***		
		LogM->Write("[refmanager||collectorRule]nInOffset    = %d",nInOffset);
		LogM->Write("[refmanager||collectorRule]nDataType    = %d",nDataType);
		LogM->Write("[refmanager||collectorRule]nActionCode  = %d",nActionCode);
		LogM->Write("[refmanager||collectorRule]strSubAction = %d",strSubAction);
		LogM->Write("[refmanager||collectorRule]nOutOneLen   = %d",nOutOneLen);
***/		
        //	one field 처리 
        memcpy(strOutOneData, cltRule.oneField(indata,nInOffset,nInOneLen,nOutOneLen,nDataType,nActionCode,strSubAction), 
        						cltRule.get_OneFieldLen());
        
//		LogM->Write("[refmanager||collectorRule]strOutOneData   = %s",strOutOneData);
        //	one field의 data가 없을 경우 처리 (false 처리)
        for(int i = 0; i < MAX_OUT_DATA_LEN ; i++)
        {
        	if (strOutOneData[i] == NULL)
        	{
        		nTempNullCnt++;
        	}
        }
        if (nTempNullCnt == MAX_OUT_DATA_LEN)
        {
			delete[]	strOutOneData;
//			LogM->Write("[refmanager||pdsnRule]FAILREFMANAGER   = %d",FAILREFMANAGER);
        	return FAILREFMANAGER;
        }
       	DataMove(strOutOneData, outdata, nOutOffset);			
    }
	nActIndex = 1;	
	delete[]	strOutOneData;
    return OKREFMANAGER;
}	

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : pdsnRule
//   Usage          : UDR을 017 data format인 pdsn으로 변형해 주는 함수
//	 Argument		: char* indata 		(input data record)
//					  char* outdata     (변형을 한 output data record)
//	 Return			: int (FAILREFMANAGER : -1 , OKREFMANAGER : 1)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
int CRefMannager::pdsnRule		(char* indata, char* outdata)
{
	CPdsnRule		ptrPdsnRule;					
	int 			nTempNullCnt;
	int				nPdsnCode;
	int 			nVendorCode;
	int 			outoffset= 0;
	nOutTotLen  = 0;

	strOutOneData 		= new char[MAX_OUT_DATA_LEN];
    for(;nActIndex<=nFieldCnt;nActIndex++)
    {
		// loop에서 필요한 정보 초기화 
		nTempNullCnt = 0;
		memset (strOutOneData, NULL, MAX_OUT_DATA_LEN);
		
		//	구성정보 얻는 부분 (input은 meta table에서 output은 pdsn table에서)
		nInOffset   = m_ActionTable[nActIndex].In_Field_Offset;
	    nInOneLen   = m_ActionTable[nActIndex].In_Field_Length;
	    
	    nPdsnCode	= pdsn->m_PdsnTable[nActIndex].Pdsn_Code;
	    nVendorCode	= pdsn->m_PdsnTable[nActIndex].Vendor_Code;
	    if (nPdsnCode == 0)
		{
			delete[] strOutOneData;
	    	continue;
		}
/***				
		LogM->Write("[refmanager||pdsnRule]nActIndex    = %d",nActIndex);
		LogM->Write("[refmanager||pdsnRule]nPdsnCode 	= %d",nPdsnCode);
		LogM->Write("[refmanager||pdsnRule]nVendorCode 	= %d",nVendorCode);
***/		
        //	one field 처리 
        memcpy(strOutOneData, ptrPdsnRule.pdsnOneField(indata,nInOffset,nInOneLen,nPdsnCode,nVendorCode), 
        						ptrPdsnRule.get_pdsnOneFieldLen());

		//	총 data size
		nOutOneLen = (int)strOutOneData[1];
		nOutTotLen  = nOutTotLen + nOutOneLen;
/***
		LogM->Write("[refmanager||pdsnRule]ptrPdsnRule.get_OneFieldLen()   = %d",nOutOneLen);
		LogM->Write("[refmanager||pdsnRule]nOutTotLen   = %d",nOutTotLen);
        
		LogM->Write("[refmanager||pdsnRule]strOutOneData   = %s",strOutOneData);
***/
        //	one field의 data가 없을 경우 처리 (false 처리)
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
//			LogM->Write("[refmanager||pdsnRule]FAILREFMANAGER   = %d",FAILREFMANAGER);
        	return FAILREFMANAGER;
        }


       	DataMove(strOutOneData, outdata, outoffset);			
       	outoffset = outoffset + nOutOneLen;
    }
	nActIndex = 1;	
	delete[] strOutOneData;
    return OKREFMANAGER;
}	
	



//////////////////////////////////////////////////////////////////////
//
//   Function Name  : DataMove
//   Usage          : 하나의 처리된 결과를 out buffer에 쌓아주는 부분 
//	 Argument		: char* indata 		(input data record)
//					  char* outdata     (변형을 한 output data record)
//	 Return			: 
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
void CRefMannager::DataMove(char *InBuf, char *OutBuf, int nTempOutOffset)
{
//	LogM->Write("[refmanager||DataMove]nOutOffSet   = %d",nTempOutOffset);
//	LogM->Write("[refmanager||DataMove]nOutLen      = %d\n",nOutOneLen);
    memcpy(OutBuf+nTempOutOffset,InBuf,nOutOneLen);
}

