
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
    "CustomerMan.pc"
};


static unsigned long sqlctx = 1231803;


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
            void  *sqhstv[9];
   unsigned int   sqhstl[9];
            int   sqhsts[9];
            void  *sqindv[9];
            int   sqinds[9];
   unsigned int   sqharm[9];
   unsigned int   *sqharc[9];
   unsigned short  sqadto[9];
   unsigned short  sqtdso[9];
} sqlstm = {10,9};

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
"select provider ,min ,event_date ,event_time ,svc_type ,cust_status  from t_\
cust_info            ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{10,4130,0,0,0,
5,0,0,2,58,0,2,332,0,0,2,2,0,1,0,1,3,0,0,1,3,0,0,
28,0,0,3,0,0,29,334,0,0,0,0,0,1,0,
43,0,0,4,0,0,27,453,0,0,4,4,0,1,0,1,9,0,0,1,9,0,0,1,10,0,0,1,10,0,0,
74,0,0,5,0,0,30,472,0,0,0,0,0,1,0,
89,0,0,6,113,0,3,514,0,0,6,6,0,1,0,1,3,0,0,1,3,0,0,1,97,0,0,1,97,0,0,1,3,0,0,1,
3,0,0,
128,0,0,7,0,0,29,519,0,0,0,0,0,1,0,
143,0,0,8,151,0,5,750,0,0,9,9,0,1,0,1,3,0,0,1,3,0,0,1,97,0,0,1,97,0,0,1,3,0,0,
1,97,0,0,1,3,0,0,1,3,0,0,1,3,0,0,
194,0,0,9,0,0,29,755,0,0,0,0,0,1,0,
209,0,0,10,128,0,3,757,0,0,7,7,0,1,0,1,3,0,0,1,3,0,0,1,97,0,0,1,97,0,0,1,3,0,0,
1,97,0,0,1,3,0,0,
252,0,0,11,0,0,29,759,0,0,0,0,0,1,0,
267,0,0,12,39,0,2,762,0,0,1,1,0,1,0,1,3,0,0,
286,0,0,13,0,0,29,763,0,0,0,0,0,1,0,
301,0,0,1,0,0,13,782,0,0,6,0,0,1,0,2,3,0,0,2,3,0,0,2,97,0,0,2,97,0,0,2,3,0,0,2,
3,0,0,
340,0,0,1,97,0,9,815,0,0,0,0,0,1,0,
355,0,0,14,0,0,29,852,0,0,0,0,0,1,0,
370,0,0,1,0,0,15,853,0,0,0,0,0,1,0,
};


/******************************************************************
* 파 일 명     : CustmerMan.pc			     					  *
* 기   능                                                          *
* : 고객정보를 CDS I/F MQ로부터 읽어들여, 고객정보MDB와 t_cust_info     *
*    table을 변경시켜주는 역할을 하는 프로그램.	   					  *
* 작 성 자     : Han Guen Hee                    				  *
* first data  : 2000. 11. 07       	     						  *
* last updata : 2000. 12. 21        		 					  *
* 관 련 화 일  : CustmerMan.hpp                					  *
* program history                        						  *
*                         	        	 				 		  *
* bug fix                        	     						  *
*                         	        	 						  *
******************************************************************/
#include "CustomerMan.hpp"

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


// T_CUST_INFO Table변수에 접근하기 위한 변수.
/* EXEC SQL BEGIN DECLARE SECTION; */ 

/* VARCHAR          custusername[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } custusername;

/* VARCHAR          custpasswd[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } custpasswd;

int              nProvider;
int 			 nMin;
int 			 nMin2;
char             strEvent_Date[EVENT_DATE_LEN+1];
char             strEvent_Time[EVENT_TIME_LEN+1];
int				 nSvc_Type;
char             strPrice_Plan[PRICE_PLAN_LEN+1]; 
int              nCust_Status;
/* EXEC SQL END DECLARE SECTION; */ 


// Oracle DB에서 record를 fetch해오기 위한 cursor를 정의.
/* EXEC SQL declare shm_cur cursor for
		select  provider,min,event_date,event_time,svc_type,cust_status
		from    t_cust_info; */ 

	//	where   provider=:nProvider and min=:nMin;

// Constructor
CCustomerMan::CCustomerMan()
{
}
//===============================================================
// Constructor Method											=
//  : Customer MDB Object를 할당한다.                        		=
// argument : 프로세스 형상구분 6가지								=
// return value : None											=
//																=
//===============================================================
CCustomerMan::CCustomerMan( int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark )
{
	char   cId='A';
	
	*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
		<< "CCustomerMan Contructor!!" << endl;
	
	memset( strEvent_Date, 0x00, sizeof( strEvent_Date) );
    memset( strEvent_Time, 0x00, sizeof( strEvent_Time) );
    memset( strPrice_Plan, 0x00, sizeof( strPrice_Plan) );
    
	sprintf( PATH_NAME_CUSTOMER, "/SVC%d/DATA/SubsShm.cfg",nServiceId );
    cCustMdbMan = new CCustMdbMan( PATH_NAME_CUSTOMER, cId, nSystemId,nServiceId,nGroupId,nProcessType,nPserialNo,nPreMark );
}
// destructor
CCustomerMan::~CCustomerMan()
{
	delete cCustMdbMan;
}
//===============================================================
// ReadDataToMQ Method						   					=
//  : 외부에서 최초로 Customer Manager를 시작하는 method.      		=
// argument     : None											=
// return value : None											=
//																=
//===============================================================
void CCustomerMan::ReadDataToMQ( void )
{
	int 		nErrorNo;
	char		cJobCode[JOBCODE_LEN+1];	// A1:신규가입 Z1:가입해지 D1:변경
	CustFormat 	stTempCustFormat;
	
	*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
					<< "CCustomerMan : ReadDataToMQ!!" << endl;
	while( 1 )
	{
		//SCP I/F로부터 한도관리고객정보를 읽어온다.
		memset( ( char* )&cRecvData, NULL, sizeof( CdsCustFormat ) );
		if ( mqCUST->recvMsg( &cRecvData ) == FLAG_MQ_IS_EMPTY )
		{
	//	    cout << "==Recv=NO DATA IN MessageQueue====="   << endl;
			sleep( 1 );
		}
		else 
		{
			memcpy( cJobCode, cRecvData.get_CustFormat()->get_JobCode(), JOBCODE_LEN );
			memcpy( (char *)&stTempCustFormat, cRecvData.get_CustFormat(), sizeof( CustFormat ) );
			
			// 신규가입이라면..
			if( !memcmp( cJobCode, "A1", 2 ) )
			{
				// 읽어온 정보를 DB에 넣어준다.
				if( TransactInsert( stTempCustFormat ) )
				{
					// 응답하기위한 구조체를 만들어 준다.
				/*
					cSendData.set_Result(1);
					cSendData.set_UTransactionID( cRecvData.get_UTransactionID() );
				*/
					// 처리결과를 SCP I/F에 보내준다.	
					/*
					if( ReponseToMQ( &cSendData ) )
					{
						cout << "Reponse Success!!" << endl;
					}
					else
					{
						cout << "Error Reponse in ReponseToMQ" << endl;
					}
					*/
					if( CUSTOMER_DEBUG )
					{
						*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
							<< "Insert Receive Data Complete!!" << endl;
					}
					
				}
				else
				{
				//	cSendData.set_Result(0);
				//	cSendData.set_UTransactionID( cRecvData.get_UTransactionID() );
				/*
					if( ReponseToMQ( &cSendData ) )
					{
						cout << "Reponse Success!!" << endl;
					}
					else
					{
						cout << "Error Reponse in ReponseToMQ" << endl;
					}
					*/
					if( CUSTOMER_DEBUG )
					{
						*CustomerManLogM << CustomerManLogM->ErrorCode(5001) 
							<< "Error DB Insert in ReadDataToMQ!!" << endl;
					}
				}
			} // else
			// 가입해지라면..
			else if( !memcmp( cJobCode, "Z1", 2 ) )
			{
				// 읽어온 정보를 DB에서 삭제한다.
				if( TransactDelete( stTempCustFormat ) )
				{
					// 응답하기위한 구조체를 만들어 준다.
				//	cSendData.set_Result(1);
				//	cSendData.set_UTransactionID( cRecvData.get_UTransactionID() );

					// 처리결과를 SCP I/F에 보내준다.	
					/*
					if( ReponseToMQ( &cSendData ) )
					{
						cout << "Reponse Success!!" << endl;
					}
					else
					{
						cout << "Error Reponse in ReponseToMQ" << endl;
					}
					*/
					if( CUSTOMER_DEBUG )
					{
						*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
							<< "TransactDelete Receive Data Complete!!" << endl;
					}
					
				}
				else
				{
				//	cSendData.set_Result(0);
				//	cSendData.set_UTransactionID( cRecvData.get_UTransactionID() );

					/*
					if( ReponseToMQ( &cSendData ) )
					{
						cout << "Reponse Success!!" << endl;
					}
					else
					{
						cout << "Error Reponse in ReponseToMQ" << endl;
					}
					*/
					if( CUSTOMER_DEBUG )
					{
						*CustomerManLogM << CustomerManLogM->ErrorCode(5002) 
							<< "Error TransactDelete in ReadDataToMQ!!" << endl;
					}
					
				}
			} // else if
			// 정보변경이라면...
			else if( !memcmp( cJobCode, "D1", 2 ) )
			{
				if( cCustMdbMan->UpdateCustMdb( stTempCustFormat ) )
				{
					*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
						<< "Complete!! Update Memory DB !!" << endl;
				}
				else
				{
					*CustomerManLogM << CustomerManLogM->ErrorCode(5007) 
						<< "fail!! Update Memory DB!!" << endl;
				}
				if( UpdateCustDb( stTempCustFormat ) )
				{
					*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
						<< "Complete!! Update Oracle DB !!" << endl;
				}
				else
				{
					*CustomerManLogM << CustomerManLogM->ErrorCode(5008) 
						<< "fail!! Update Oracle DB!!" << endl;
				}
			} // else if
			else
			{
				*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
							<< "Different customer status!!" << endl;
			} // else
		} // else
	}// while
}
//===============================================================
// ReponseToMQ Method						   					=
//  : 처리된 결과를 SCP I/F에게 보내준다. 				       		=
// argument     :  pSendUdr -> 응답을 하기위한 format				=
// return value : 1-> success									=
//                0-> fail										=
//																=
//===============================================================
/*
int CCustomerMan::ReponseToMQ( ReCustFormat* pSendUdr )
{
	
	*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
					<< "CCustomerMan : ReponseToMQ!!" << endl;
	while(1)
    {
		if( mqReCUST->sendMsg( pSendUdr ) == FLAG_MQ_IS_FULL )
	    {
	    	cout << "MSG Queue FULL!!" <<  endl;
			return 0;
	    }
		else break;
    }
	return 1;
}
*/
//===============================================================
// TransactDelete Method					   					=
//  : 해당 data를 MDB와 Oracle DB에서 삭제해 준다.		       		=
// argument     :  stCustFormat -> 삭제하기위한 format.			=
// return value : 1-> success									=
//                0-> fail										=
//																=
//===============================================================
int	CCustomerMan::TransactDelete( CustFormat stCustFormat )
{
	
	*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
			<< "CCustomerMan : TransactDelete!!" << endl;
	// Memory DB에서 삭제해 준다.
	if( cCustMdbMan->DeleteCustMdb( stCustFormat ) )
	{
		*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
			<< "Complete!! Delete Memory DB !!" << endl;
	}
	else
	{
		*CustomerManLogM << CustomerManLogM->ErrorCode(5004) 
			<< "fail!! Delete Memory DB!!" << endl;
		return 0;
	}
	// Oracle DB에서 삭제해 준다.
	if( deleteCustDb( stCustFormat ) )
	{
		*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
			<< "Complete!! Delete Oracle DB!!" << endl;
	}
	else
	{
		*CustomerManLogM << CustomerManLogM->ErrorCode(5005) 
			<< "fail!! Delete Oracle DB!!" << endl;
		return 0;
	}
	return 1;
}
//===============================================================
// deleteCustDb Method						   					=
//  : 해당 data를 실제로 Oracle DB에서 삭제해 준다.		       		=
// argument     :  stCustFormat -> 삭제하기위한 format.			=
// return value : 1-> success									=
//                0-> fail										=
//																=
//===============================================================
int CCustomerMan::deleteCustDb( CustFormat stCustFormat )
{
	*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
			<< "CCustomerMan : deleteCustDb!!" << endl;
	// CustFormat형식을 DB Data형식으로 바꾸어 준다.
	if( MatchToDb( stCustFormat ) )
    {
		if( CUSTOMER_DEBUG )
		{
			*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
				<< "Success Conversion CustFormat -> DB Format!!" << endl;
		}
    }
    else
	{
		if( CUSTOMER_DEBUG )
		{
			*CustomerManLogM << CustomerManLogM->ErrorCode(5006) 
				<< "Error Conversion CustFormat->DB Format!!" << endl;
		}
		return 0;
	}  
	if( 0 )
	{
		PrintCustFormat( cout );
	}
    
    /* EXEC SQL DELETE FROM t_cust_info
        where  provider=:nProvider and min=:nMin; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 2;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "delete  from t_cust_info  where (provider=:b0 and min=:b1\
)";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )5;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlstm.sqhstv[0] = (         void  *)&nProvider;
    sqlstm.sqhstl[0] = (unsigned int  )4;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (         void  *)&nMin;
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
}


    /* EXEC SQL COMMIT WORK; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 2;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )28;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


    
    return 1;
}
//===============================================================
// TransactInsert Method					   					=
//  : 해당 data를 MDB와 Oracle DB에 Insert해준다.  	       		=
// argument     :  stCustFormat -> 삽입하기위한 format.			=
// return value : 1-> success									=
//                0-> fail( Memory overflow )					=
//																=
//===============================================================
int CCustomerMan::TransactInsert( CustFormat stCustFormat )
{
	unsigned long 	divtemp;
	char			cMin[MIN_LEN+1];
	int				nCount;
	unsigned int	nMin = 0;
	int				nTempFlag;
	unsigned int    temp;

	*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
			<< "CCustomerMan : TransactInsert!!" << endl;
	// Min을 변환한다.	
	memcpy( cMin, stCustFormat.get_Min(), MIN_LEN );
	
	nCount=9;	
	divtemp=1000000000;
	for( int i=0; i<10; i++ )
	{
		temp = (unsigned int)( cMin[i]-48 );
		nMin += (unsigned int)( temp * divtemp );
		divtemp /= 10;
		nCount--;
	}
	if ( 0 )
	{
		cout << "========================" << endl;
		cout << "nMin=" << nMin << endl;
		cout << "========================" << endl;
	}
	// 1이면 Update, 0이면 Insert
	nTempFlag=cCustMdbMan->SearchMdb( nMin );
	
	// Update
	if( nTempFlag)
	{
		if( cCustMdbMan->UpdateCustMdb( stCustFormat ) )
		{
			*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
				<< "Complete!! Update Memory DB !!" << endl;
		}
		else
		{
			*CustomerManLogM << CustomerManLogM->ErrorCode(5007) 
				<< "fail!! Update Memory DB!!" << endl;
			return 0;
		}
		//if( UpdateCustDb( stCustFormat ) )
		if( InsertCustDb( stCustFormat ) )//jjk
		{
			*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
				<< "Complete!! Update Oracle DB !!" << endl;
		}
		else
		{
			*CustomerManLogM << CustomerManLogM->ErrorCode(5008) 
				<< "fail!! Update Oracle DB!!" << endl;
			return 0;
		}
	} // if
	// Insert
	else
	{
		cout << "TransactInsert : TransactInsert Insert" << endl;
		if( cCustMdbMan->InsertCustMdb( stCustFormat ) )
		{
			*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
				<< "Complete!! Insert Memory DB !!" << endl;
		}
		else
		{
			*CustomerManLogM << CustomerManLogM->ErrorCode(5009) 
				<< "fail!! Insert Memory  DB!!" << endl;
			return 0;
		}
		if( InsertCustDb( stCustFormat ) )
		{
			*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
				<< "Complete!! Insert Oracle DB !!" << endl;
		}
		else
		{
			*CustomerManLogM << CustomerManLogM->ErrorCode(5010) 
				<< "fail!! Insert Oracle  DB!!" << endl;
			return 0;
		}
	} // else

	return 1;
}
//===============================================================
// ConnectToDB Method					   						=
//  : Db Connection								 	       		=
// argument     :  None											=
// return value : 0-> success									=
//                sql code-> fail           					=
//																=
//===============================================================
int CCustomerMan::ConnectToDB( void )
{
	*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
			<< "CCustomerMan : ConnectToDB!!" << endl;
	strcpy((char *) custusername.arr, DB_USERNAME);
	custusername.len = (unsigned short) strlen(DB_USERNAME);

	strcpy((char *) custpasswd.arr, DB_PASSWORD);
	custpasswd.len = (unsigned short) strlen(DB_PASSWORD);

	/* EXEC SQL connect :custusername identified by :custpasswd; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )10;
 sqlstm.offset = (unsigned int  )43;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlstm.sqhstv[0] = (         void  *)&custusername;
 sqlstm.sqhstl[0] = (unsigned int  )34;
 sqlstm.sqhsts[0] = (         int  )34;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&custpasswd;
 sqlstm.sqhstl[1] = (unsigned int  )34;
 sqlstm.sqhsts[1] = (         int  )34;
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



	if( CUSTOMER_DEBUG )
	{
		*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
				<< "ConnectToDB sqlcode: " << sqlca.sqlcode << endl;
	}

	return(0);
}
//===============================================================
// Db_close Method						   						=
//  : Db Close									 	       		=
// argument     :  None											=
// return value : sql code		 								=
//																=
//===============================================================
int CCustomerMan::Db_close()
{
	/* EXEC	SQL	COMMIT	WORK	RELEASE; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )74;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



	return(sqlca.sqlcode); 
}
//===============================================================
// InsertCustDb Method						   					=
//  : 해당 data를 실제로 Oracle DB에 Insert해준다.  	       		=
// argument     :  stCustFormat -> 삽입하기위한 format.			=
// return value : 1-> success									=
//                0-> fail( Memory overflow )					=
//																=
//===============================================================
int CCustomerMan::InsertCustDb( CustFormat stUdr )
{
	*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
			<< "CCustomerMan : InsertCustDb!!" << endl;
    if( MatchToDb( stUdr ) )
    {
		if( CUSTOMER_DEBUG )
		{
			*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
				<< "Success Conversion CustFormat -> DB Format!!" << endl;
		}
    }
    else
	{
		if( CUSTOMER_DEBUG )
		{
			*CustomerManLogM << CustomerManLogM->ErrorCode(5006) 
				<< "Error Conversion CustFormat->DB Format!!" << endl;
		}
		return 0;
	}  

	if( 0 )
	{
		PrintCustFormat( cout );
	}

	nSvc_Type = 0;
	nCust_Status = 1;
	
    /* EXEC SQL INSERT INTO t_cust_info(provider,min,event_date,event_time,svc_type,cust_status)
		values(:nProvider,:nMin,:strEvent_Date,:strEvent_Time,:nSvc_Type,:nCust_Status); */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 6;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "insert into t_cust_info(provider,min,event_date,event_tim\
e,svc_type,cust_status) values (:b0,:b1,:b2,:b3,:b4,:b5)";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )89;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)0;
    sqlstm.sqhstv[0] = (         void  *)&nProvider;
    sqlstm.sqhstl[0] = (unsigned int  )4;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (         void  *)&nMin;
    sqlstm.sqhstl[1] = (unsigned int  )4;
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         void  *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned int  )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqhstv[2] = (         void  *)strEvent_Date;
    sqlstm.sqhstl[2] = (unsigned int  )9;
    sqlstm.sqhsts[2] = (         int  )0;
    sqlstm.sqindv[2] = (         void  *)0;
    sqlstm.sqinds[2] = (         int  )0;
    sqlstm.sqharm[2] = (unsigned int  )0;
    sqlstm.sqadto[2] = (unsigned short )0;
    sqlstm.sqtdso[2] = (unsigned short )0;
    sqlstm.sqhstv[3] = (         void  *)strEvent_Time;
    sqlstm.sqhstl[3] = (unsigned int  )7;
    sqlstm.sqhsts[3] = (         int  )0;
    sqlstm.sqindv[3] = (         void  *)0;
    sqlstm.sqinds[3] = (         int  )0;
    sqlstm.sqharm[3] = (unsigned int  )0;
    sqlstm.sqadto[3] = (unsigned short )0;
    sqlstm.sqtdso[3] = (unsigned short )0;
    sqlstm.sqhstv[4] = (         void  *)&nSvc_Type;
    sqlstm.sqhstl[4] = (unsigned int  )4;
    sqlstm.sqhsts[4] = (         int  )0;
    sqlstm.sqindv[4] = (         void  *)0;
    sqlstm.sqinds[4] = (         int  )0;
    sqlstm.sqharm[4] = (unsigned int  )0;
    sqlstm.sqadto[4] = (unsigned short )0;
    sqlstm.sqtdso[4] = (unsigned short )0;
    sqlstm.sqhstv[5] = (         void  *)&nCust_Status;
    sqlstm.sqhstl[5] = (unsigned int  )4;
    sqlstm.sqhsts[5] = (         int  )0;
    sqlstm.sqindv[5] = (         void  *)0;
    sqlstm.sqinds[5] = (         int  )0;
    sqlstm.sqharm[5] = (unsigned int  )0;
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



	if (sqlca.sqlcode == 0) 
	{
		/* EXEC SQL COMMIT WORK; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 6;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )128;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		return 1;
	}
	*CustomerManLogM << CustomerManLogM->ErrorCode(0)<< "Insert sqlcode:"<<sqlca.sqlcode << endl;
}
//===============================================================
// MatchToDb Method						   						=
//  : 해당 data를 실제로 Oracle DB Format에 맞게 Reformat     		=
// argument     :  stCustFormat									=
// return value : 1-> success									=
//                0-> fail( Memory overflow )					=
//																=
//===============================================================
int CCustomerMan::MatchToDb( CustFormat stCustFormat )
{
    unsigned char 	cCode;		// 0x11
	char			cJobCode[JOBCODE_LEN+1];	// A1:신규가입 Z1:가입해지 D1:번호변경
	char			cMin[MIN_LEN+1];	
	char			cMin2[MIN_LEN+1];
	char			cAccInfo[ACCINFO_LEN+1];	// 01:TTL	
	char 			cDate[20];  
    char 			cTime[20];
	unsigned int    temp;
	unsigned long 	divtemp;
    time_t 			curtime;
    struct 			tm *loctime;
    int				nCount,i,j;

	*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
							<< "CCustomerMan : MatchToDb!!" << endl;
    curtime = time( NULL );
    loctime = localtime( &curtime);
 
	strftime( cDate,20, "%Y%m%d", loctime );              
    strftime( cTime,20, "%H%M%S", loctime);                 

	cCode = stCustFormat.get_Code();
	
	memcpy( cJobCode, stCustFormat.get_JobCode(), JOBCODE_LEN );
	memcpy( cMin, stCustFormat.get_Min(), MIN_LEN );
	memcpy( cMin2, stCustFormat.get_Min2(), MIN_LEN );
	memcpy( cAccInfo, stCustFormat.get_AccInfo(), ACCINFO_LEN );

	if( 0 )
	{
		cout << "cCode: " << cCode << endl;
		cout << "cJobCode: " << cJobCode << endl;
		cout << "cMin: " << cMin << endl;
		cout << "cMin2: " << cMin2 << endl;
		cout << "cAccInfo: " << cAccInfo << endl;
	}
	
	memcpy( strEvent_Date, cDate, EVENT_DATE_LEN );
	memcpy( strEvent_Time, cTime, EVENT_TIME_LEN );
	// 아직 정의되지 않아서 NULL을 유지한다.
	memcpy( strPrice_Plan, 0x00, PRICE_PLAN_LEN );
	
	if( !memcmp( cAccInfo, "01", 2 ) )
		nSvc_Type = 1;
	else
		nSvc_Type = 0;
		
	// Min을 DB에 넣을수 있도록 하나의 unsigned int형으로 만들어 준다.
	nProvider = 0;
	nMin = 0;
	if( !memcmp( &cMin[0], "0", 1 ) )
	{
		nCount = 2;
		divtemp=100;
		for( i=0; i<3; i++ )
		{
			temp = (unsigned int)( cMin[i]-48 );
			nProvider += (unsigned int)( temp * divtemp );
			divtemp /= 10;
			nCount--;
		}
		nCount = 6;
		divtemp=1000000;
		for( j=3; j<10; j++ )
		{
			temp = (unsigned int)( cMin[j]-48 );
			nMin += (unsigned int)( temp * divtemp );
			divtemp /= 10;
			nCount--;
		}
	}
	else
	{
		nCount = 1;
		divtemp=10;
		for( i=0; i<2; i++ )
		{
			temp = (unsigned int)( cMin[i]-48 );
			nProvider += (unsigned int)( temp * divtemp );
			divtemp /= 10;
			nCount--;
		}
		nCount = 7;
		divtemp=10000000;
		for( j=2; j<10; j++ )
		{
			temp = (unsigned int)( cMin[j]-48 );
			nMin += (unsigned int)( temp * divtemp );
			divtemp /= 10;
			nCount--;
		}
	}
	// A1: 신규가입
	if( !memcmp( cJobCode, "A1", 2 ) )
	{
		nCust_Status = 1;
	}
	// Z1: 가입해지
	else if( !memcmp( cJobCode, "Z1", 2 ) )
	{
		nCust_Status = 2;
	}
	// D1 : 번호변경 
/*	else if( !memcmp( cJobCode, "D1", 2 ) )
	{
		nCust_Status = 1;
		nProvider = 0;
		nMin  = 0;
		if( !memcmp( &cMin[0], "0", 1 ) )
		{
			nCount = 2;
			divtemp=100;
			for( i=0; i<3; i++ )
			{
				temp = (unsigned int)( cMin[i]-48 );
				nProvider += (unsigned int)( temp * divtemp );
				divtemp /= 10;
				nCount--;
			}
			nCount = 6;
			divtemp=1000000;
			for( int j=3; j<10; j++ )
			{
				temp = (unsigned int)( cMin[j]-48 );
				nMin += (unsigned int)( temp * divtemp );
				divtemp /= 10;
				nCount--;
			}
		}
		else
		{
			nCount = 1;
			divtemp=10;
			for( int i=0; i<2; i++ )
			{
				temp = (unsigned int)( cMin[i]-48 );
				nProvider += (unsigned int)( temp * divtemp );
				divtemp /= 10;
				nCount--;
			}
			nCount = 7;
			divtemp=10000000;
			for( int j=2; j<10; j++ )
			{
				temp = (unsigned int)( cMin[j]-48 );
				nMin += (unsigned int)( temp * divtemp );
				divtemp /= 10;
				nCount--;
			}
		}
	}*/
	else if( !memcmp( cJobCode, "D1", 2 ) )
	{
		char strMin[8];
		nCust_Status = 1;
		nProvider = 11;
		nMin2 = 0;
		
		memset(strMin,0x00,8);
		
		if( !memcmp( &cMin2[0], "0", 1 ) )
		{
			memcpy(strMin,&cMin2[3],7);
			nMin2 = (unsigned int) atoi(strMin);
		}
		else
		{
			 memcpy(strMin,&cMin2[2],8);
			 nMin2 = (unsigned int) atoi(strMin);
		}
		
	}
	else
	{
		*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
					<< "Different customer status!!" << endl;
		return 0;
	}
	return 1;
}
//===============================================================
// UpdateCustDb Method						   					=
//  : 해당 data를 실제로 Oracle DB에 Update해준다.  	       		=
// argument     :  stCustFormat -> 갱신하기위한 format.			=
// return value : 1-> success									=
//                0-> fail 										=
//																=
//===============================================================
int CCustomerMan::UpdateCustDb( CustFormat stCustFormat )
{
	*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
			<< "CCustomerMan : UpdateCustDb!!" << endl;
	
	if( MatchToDb( stCustFormat ) )
    {
		if( CUSTOMER_DEBUG )
		{
			*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
				<< "Success Conversion CustFormat -> DB Format!!" << endl;
		}
    }
    else
	{
		if( CUSTOMER_DEBUG )
		{
			*CustomerManLogM << CustomerManLogM->ErrorCode(5006) 
				<< "Error Conversion CustFormat->DB Format!!" << endl;
		}
		return 0;
	}  
	if( 0 )
	{
		PrintCustFormat( cout );
	}
	*CustomerManLogM << CustomerManLogM->ErrorCode(0) << "D3 min:"<< nMin<<"NewMin:"<<nMin2<< endl;
	
	/* EXEC SQL update t_cust_info
		 set    provider=:nProvider,min=:nMin2,event_date=:strEvent_Date,event_time=:strEvent_Time,svc_type=:nSvc_Type,price_plan=:strPrice_Plan,cust_status=:nCust_Status
		 where   provider=:nProvider and min=:nMin; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 9;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "update t_cust_info  set provider=:b0,min=:b1,event_date=:b2,\
event_time=:b3,svc_type=:b4,price_plan=:b5,cust_status=:b6 where (provider=:b0\
 and min=:b8)";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )143;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlstm.sqhstv[0] = (         void  *)&nProvider;
 sqlstm.sqhstl[0] = (unsigned int  )4;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&nMin2;
 sqlstm.sqhstl[1] = (unsigned int  )4;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)strEvent_Date;
 sqlstm.sqhstl[2] = (unsigned int  )9;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)strEvent_Time;
 sqlstm.sqhstl[3] = (unsigned int  )7;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&nSvc_Type;
 sqlstm.sqhstl[4] = (unsigned int  )4;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)strPrice_Plan;
 sqlstm.sqhstl[5] = (unsigned int  )6;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)&nCust_Status;
 sqlstm.sqhstl[6] = (unsigned int  )4;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         void  *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned int  )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (         void  *)&nProvider;
 sqlstm.sqhstl[7] = (unsigned int  )4;
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         void  *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned int  )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
 sqlstm.sqhstv[8] = (         void  *)&nMin;
 sqlstm.sqhstl[8] = (unsigned int  )4;
 sqlstm.sqhsts[8] = (         int  )0;
 sqlstm.sqindv[8] = (         void  *)0;
 sqlstm.sqinds[8] = (         int  )0;
 sqlstm.sqharm[8] = (unsigned int  )0;
 sqlstm.sqadto[8] = (unsigned short )0;
 sqlstm.sqtdso[8] = (unsigned short )0;
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


	
	if( sqlca.sqlcode == 0 ) 
		/* EXEC SQL COMMIT WORK; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 9;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )194;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	else if( sqlca.sqlcode == 1403 ) {
		/* EXEC SQL INSERT INTO t_cust_info(provider,min,event_date,event_time,svc_type,price_plan,cust_status)
		values(:nProvider,:nMin2,:strEvent_Date,:strEvent_Time,:nSvc_Type,:strPrice_Plan,:nCust_Status); */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 9;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "insert into t_cust_info(provider,min,event_date,event_time,\
svc_type,price_plan,cust_status) values (:b0,:b1,:b2,:b3,:b4,:b5,:b6)";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )209;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)0;
  sqlstm.sqhstv[0] = (         void  *)&nProvider;
  sqlstm.sqhstl[0] = (unsigned int  )4;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&nMin2;
  sqlstm.sqhstl[1] = (unsigned int  )4;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)strEvent_Date;
  sqlstm.sqhstl[2] = (unsigned int  )9;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)strEvent_Time;
  sqlstm.sqhstl[3] = (unsigned int  )7;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         void  *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned int  )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (         void  *)&nSvc_Type;
  sqlstm.sqhstl[4] = (unsigned int  )4;
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         void  *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned int  )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (         void  *)strPrice_Plan;
  sqlstm.sqhstl[5] = (unsigned int  )6;
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         void  *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned int  )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (         void  *)&nCust_Status;
  sqlstm.sqhstl[6] = (unsigned int  )4;
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         void  *)0;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned int  )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
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


		/* EXEC SQL COMMIT WORK; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 9;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )252;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		
	}else if( sqlca.sqlcode == -1 ) {
		/* EXEC SQL delete from t_cust_info where min=:nMin; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 9;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "delete  from t_cust_info  where min=:b0";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )267;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)0;
  sqlstm.sqhstv[0] = (         void  *)&nMin;
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


		/* EXEC SQL COMMIT WORK; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 9;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )286;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	}
	
	*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
				<< "update select sqlcode = " << sqlca.sqlcode << endl;
    return 1;
}
//===============================================================
// FetchData Method						   						=
//  : Db Fetch									 	       		=
// argument     :  None											=
// return value : sql code		 								=
//																=
//===============================================================
int CCustomerMan::FetchData()
{
	*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
			<< "CCustomerMan : FetchData!!" << endl;
			
	/* EXEC SQL	fetch	shm_cur 
				into 	:nProvider, :nMin, :strEvent_Date, :strEvent_Time, :nSvc_Type, :nCust_Status; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 9;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )301;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlstm.sqhstv[0] = (         void  *)&nProvider;
 sqlstm.sqhstl[0] = (unsigned int  )4;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&nMin;
 sqlstm.sqhstl[1] = (unsigned int  )4;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)strEvent_Date;
 sqlstm.sqhstl[2] = (unsigned int  )9;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)strEvent_Time;
 sqlstm.sqhstl[3] = (unsigned int  )7;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&nSvc_Type;
 sqlstm.sqhstl[4] = (unsigned int  )4;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)&nCust_Status;
 sqlstm.sqhstl[5] = (unsigned int  )4;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
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


	*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
				<< "fetch sqlcode = " << sqlca.sqlcode << endl;
	return(sqlca.sqlcode);
}
//===============================================================
// SearchMdbTemp Method					   						=
//  : MDB를 검색해주는 Method.					 	       		=
// argument     :  nSearchMin -> 찾고자 하는 Min					=
// return value : 0-> Not Found	 								=
//				  1-> Found										=
//===============================================================
int	CCustomerMan::SearchMdbTemp( int nSearchMin )
{
	return cCustMdbMan->SearchMdb( nSearchMin );
}
//===============================================================
// UploadDb Method							   					=
//  : DB의 내용을 MDB로 복구시켜주는 메소드.  	     		  		=
// argument     :  None											=
// return value : 1-> success									=
//                0-> fail 										=
//																=
//===============================================================
int	CCustomerMan::UploadDb( )
{
	CustFormatMdb		cTempCustFormatMdb;
	int					nTempMin;
	int					nSqlcode;
	
	*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
			<< "CCustomerMan : UploadDb!!" << endl;
	/* EXEC SQL OPEN shm_cur; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 9;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0001;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )340;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	for (;;) 
	{
		nSqlcode = FetchData();
		if( nSqlcode != 0 || nSqlcode == 1403)
		{
			*CustomerManLogM << CustomerManLogM->ErrorCode(5011) 
				<< "T_CUST_INFO : Sql Fetch Error " << endl;
			return 0;
		}
		if( nMin > 9999999 )
		{
			nTempMin=(nProvider*100000000)+nMin;
		}
		else
		{
			nTempMin=(nProvider*10000000)+nMin;
		} 
		cTempCustFormatMdb.set_MsID( nTempMin );
		cTempCustFormatMdb.set_EventDate(strEvent_Date);
		cTempCustFormatMdb.set_EventTime(strEvent_Time);
		cTempCustFormatMdb.set_ServiceType(nSvc_Type);
		//cTempCustFormatMdb.set_PricePlan(strPrice_Plan);
		cTempCustFormatMdb.set_CustStatus(nCust_Status);
		
		if( cCustMdbMan->UploadDbToMdb( cTempCustFormatMdb ) )
		{
			*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
				<< "Complete Lode DB!!" << endl;
		}
		else
		{
			*CustomerManLogM << CustomerManLogM->ErrorCode(5012) 
				<< "Error Upload Db To Mdb" << endl;
			return 0;
		}
	}
	/* EXEC SQL COMMIT WORK; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 9;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )355;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	/* EXEC SQL CLOSE shm_cur; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 9;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )370;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	
	return 1;
}
//===============================================================
// PrintCustFormat Method					   					=
//  : CustFormat을 echo print해주기 위한 method.     		  		=
// argument     :  output -> 출력되는 대상.						=
// return value : None											=
//																=
//===============================================================
void CCustomerMan::PrintCustFormat( ostream& output )
{
	output << setiosflags( ios::left) 
		   << setw( 11 ) << "Provider"
		   << setw( 11) << "Min"
		   << setw( 11) << "EventDate"
		   << setw( 11) << "EventTime"
		   << setw( 11) << "ServiceType"
		   << setw( 11) << "PricePlan"
		   << setw( 11) << "CustStatus" << '\n';
	output << setiosflags( ios::left) 
		   << setw( 11) << nProvider
		   << setw( 11) << nMin
		   << setw( 11) << strEvent_Date
		   << setw( 11) << strEvent_Time
		   << setw( 11) << nSvc_Type
		   << setw( 11) << strPrice_Plan
		   << setw( 11) << nCust_Status << '\n';
}

int main( int argc, char* argv[] )
{
	CCustomerMan* CustomerMan;
	int nErrorNo;

	if( argc != 7 )
	{
		cout << "Usage CustomerMan [SysId][SerId][GroupId][ProcessId][SerialNumber][Remark]" << endl;
		exit( 1 );
	}
	
	SystemId=atoi(argv[1]);
	ServiceId=atoi(argv[2]);
	GroupId=atoi(argv[3]);
	ProcessType=atoi(argv[4]);
	PserialNo=atoi(argv[5]);
	PreMark=atoi(argv[6]);
	
	CustomerManLogM	= new SendLogManage(SystemId,ServiceId,GroupId ,ProcessType ,PserialNo, PreMark );
	CustomerManLogM->put_EventHistData(FLAG_START, FLAG_PROC_START);
	
	
	*CustomerManLogM << "[" << CustomerManLogM->ErrorCode(0) << "]"
			<< "Start Customer Db Manager!!" << endl;
	
	//signal(SIGNULL,sigCapture);	//0
	signal(SIGHUP,sigCapture);
	signal(SIGINT,sigCapture);
	signal(SIGQUIT,sigCapture);
	signal(SIGKILL,sigCapture);
	signal(SIGTERM,sigCapture);
	signal(SIGSTOP,sigCapture);
	signal(SIGTSTP,sigCapture);
	signal(SIGCONT,sigCapture);	//26
	signal(SIGUSR1,sigCapture);

	CustomerMan= new CCustomerMan( SystemId,ServiceId,GroupId,ProcessType,PserialNo,PreMark );
	
	mqCUST = new MessageQueue<CdsCustFormat>(SystemId,ServiceId,GroupId ,ProcessType ,PserialNo ,10 ,1000);
	//mqCUST = new MessageQueue<CustFormat>(SystemId,ServiceId,GroupId ,ProcessType ,PserialNo ,10 ,1000);
	//mqReCUST = new MessageQueue<ReCustFormat>(SystemId,ServiceId,4,1,0,11,1000 );
	mqPMS = new MessageQueue<MsgPmsStatus>(SystemId, 0,1,1,0,10, 1000);
	WritePMSMQ(FLAG_PROC_START);
	
	
	if( nErrorNo=CustomerMan->ConnectToDB() )
	{
		*CustomerManLogM << CustomerManLogM->ErrorCode(5013) 
				<< "DB Connection Error!! Error Number= " << nErrorNo <<endl;
	}
	CustomerMan->ReadDataToMQ();
	
	*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
			<< "End Customer DB Manager!!" << endl;
	
	if( nErrorNo=CustomerMan->Db_close() )
	{
		*CustomerManLogM << CustomerManLogM->ErrorCode(5013) 
				<< "DB Connection Error!! Error Number= " << nErrorNo <<endl;
	}		
	delete mqCUST;
	//delete mqReCUST;
	delete mqPMS;
	delete CustomerMan;
	CustomerManLogM->put_EventHistData(FLAG_END, 0);
	delete CustomerManLogM;
	return 0;
}
//===============================================================
// WritePMSMQ Method		     			   					=
//  : PMS에게 Process의 상태를 보고하기 위한 Method					=
//	                                    						=
// argument     :  sigNo -> Signal Number						=
// return value : None											=
//																=
//===============================================================
void WritePMSMQ(int sigNo)
{
	int 			i=0;
	char 			PMSMQ_NO[2];
	MsgPmsStatus	OneRec(MSG_STATUS, SystemId, ServiceId, GroupId, ProcessType, PserialNo, PreMark);

	
	sprintf(PMSMQ_NO, "%d", SystemId);
	OneRec.set_nStatus(sigNo);
	
	while(1)
	{
		i++;
		if (mqPMS->sendMsg(&OneRec) == FLAG_MQ_IS_FULL)
		{
			cout << "[ERROR] PMSMQ " << PMSMQ_NO << " IS FULL sleep(1)...!"	<< endl;
			sleep(1);
			if (i >= 10)
			{
				kill(0,SIGINT);
			}
		}else break;
	}
}
//===============================================================
// sigCapture Method		     			   					=
//  : Process에서 signal capture	하는 Method      				=
//	                                    						=
// argument     :  sigNo -> Signal Number						=
// return value : None											=
//																=
//===============================================================
void sigCapture(int sigNo)
{
	*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
			<< "SIGNAL :: CustomerMan Process End!!" << endl;
	delete mqCUST;
//	delete mqReCUST;
	
	
	switch(sigNo)
	{
		case SIGINT :
		case SIGQUIT :
		case SIGKILL :
		case SIGTERM :
				*CustomerManLogM << CustomerManLogM->ErrorCode(5014) 
					<< "CustomerMan Process Killed!! " <<endl;
				WritePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				delete mqPMS;
				CustomerManLogM->put_EventHistData(FLAG_END, -1);
				delete CustomerManLogM;
				exit(-1);
				break;
		case SIGUSR1:
				*CustomerManLogM << CustomerManLogM->ErrorCode(0) 
					<< "CustomerMan Process Normal Stop!! " <<endl;
				WritePMSMQ(FLAG_PROC_TERM);//0//프로세스정상stop (Client업무명령)
				delete mqPMS;
				CustomerManLogM->put_EventHistData(FLAG_END,0);
				delete CustomerManLogM;
				exit(0);
				break;
		default:
				*CustomerManLogM << CustomerManLogM->ErrorCode(5014) 
					<< "CustomerMan Process Killed!! " <<endl;
				WritePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				delete mqPMS;
				CustomerManLogM->put_EventHistData(FLAG_END, -1);
				delete CustomerManLogM;
				exit(-1);
				break;
	}
}



