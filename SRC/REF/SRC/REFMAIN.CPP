
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
    "RefMain.pc"
};


static unsigned long sqlctx = 77819;


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


/******************************************************************
* 파 일 명     
      : RefMain.pc                
* 기   능                                                          
     : Parser와 PDSN Interface로 부터 Service provider가 017인 UDR이 MQ로 들어온다.
       Parsing된 UDR운 Pdsn format에 맞게 변형한 다음, 변형된 UDR을 파일에 기록한다.
       PDSN Interface로 부터 들어온 UDR은 변형없이 파일에 기록한다. 
* 작 성 자     
     : 신미연                         
* first data  
     : 2001. 03. 21                      
* last updata 
     : 2000. 03. 29                  
* 관 련 화 일  
     : 
* program history                                
                           
* Version
  : 1.1 ( 2001. 05. 21 )                                           
******************************************************************/


#define SQLCA_STORAGE_CLASS extern
#include <sqlca.h>

#include <signal.h>
#include <StatusFile.hpp>
#include <MsgFormat.hpp>
#include <SendLogManage.hpp>
#include <TempMsgQ_file.hpp>
#include <reformat.hpp>
#include <ConfigCTRL.hpp>
#include <UDR.hpp>
#include <SqlCommon.hpp>

/* EXEC SQL INCLUDE  "../INCLUDE/refmanager.hpp";
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


#define REQUEST_CODE	4

MessageQueue<MsgPmsStatus>  *PMSMQ;		// PMS MQ
MessageQueue<CPdsnMsg>		*PDSNMQ;	// PDSN에서 Service Provider가 017인 원시Udr을 보내주는 MQ		
MessageQueue<UdrFormat>		*PARSERMQ;	// Parser에서 Service Provider가 017인 원시Udr만을 보내주는 MQ
SendLogManage				*clsLogM;	// Log Manager
ConfigCTRL					*clsCfgCTRL;// Reformat 구성정보를 Config File에 Write or Read하기 위함 
// [t_ref_action] table정보를 가지고 Input Record를 Output Type에 따라 변환한 후,
// Output format(Type, Length, Value)에 맞게 저장한다.
CRefMannager       			*clsRfObj;  
CPdsnMsg					clsPdsnMsg;
DbConnect					clsDbConnect;

char*	strInBuf;				// Input으로 한 레코드를 저장함
int		systemId;						// System ID
int		serviceId;						// Service ID
int		groupId;						// Process Type
int		processType;					// Process Type	
int		pserialNo;						// Serial Number
int		pRemark;						// Process Remark
char	gstrAtribut[SIZE_MAX_ATTRIBUTE];//Output format에 따라 저장된 값들 


int		ProcesingPdsn();			// Pdsn MQ로 들어오는 017 Udr을 파일로 만든 후, Config file에 구성정보를 write한다.
int		ProcesingParser(char* strInputBuf);
void	showPdsnHeader();
void	writePMSMQ(int sigNo);			// PMS에게 Process의 상태를 알려 준다.	
void	sigCapture(int sigNo);			// Signal 처리
void	Destructor();				// 동적 생성된 객체를 delete한다.



//==============================================================================
int main(int argc, char* argv[])
{

	if(argc != 7 ){
		cout << "Usage: Reformat [SysId][SvcId][GroupId][ProcId][ProcSn][ProcRemark]" <<endl;
    	return -1;
    }

    //signal(SIGNULL, sigCapture);
	signal(SIGHUP,  sigCapture);	
	signal(SIGINT,  sigCapture);
	signal(SIGQUIT, sigCapture);
	signal(SIGKILL, sigCapture);
	signal(SIGSTOP, sigCapture);
	signal(SIGTSTP, sigCapture);
	signal(SIGCONT, sigCapture);
	signal(SIGUSR1, sigCapture);
	
	systemId    = atoi(argv[1]);
	serviceId   = atoi(argv[2]);
	groupId     = atoi(argv[3]);
	processType = atoi(argv[4]);
	pserialNo   = atoi(argv[5]);
	pRemark     = atoi(argv[6]); 


    int			nInRecSize = 0;			// 한 Input Record의 크기
	UdrFormat	clsUdrFm;				// Parser MQ로 들어오는 Udr Format

	PMSMQ 	   = new MessageQueue<MsgPmsStatus>(systemId, 0, 1, 1, 0, 10, 1000);
    clsLogM	   = new SendLogManage(systemId, serviceId, groupId, processType, pserialNo, pRemark);

	*clsLogM << clsLogM->ErrorCode(0) << "=====START RFCMAIN=====" << endl;
	clsLogM->put_EventHistData(FLAG_START, FLAG_PROC_START);
	writePMSMQ(FLAG_PROC_START);

	clsCfgCTRL = new ConfigCTRL(serviceId, pserialNo, 1);
    if(clsCfgCTRL->set_iNum() < 0)
    {
		*clsLogM<<"Error No = ["<<clsLogM->ErrorCode(9008)<<"] Fail config file seek["<<errno<<"]"<<endl;	
		clsLogM->put_EventHistData(FLAG_END, -1);
 		delete PMSMQ; 		
 		delete clsLogM;
 		exit(-1);
	}			

	if(clsDbConnect.connect()!=0)
	{
		*clsLogM<<"Error No = ["<<clsLogM->ErrorCode(9001)<<"] Fail DB Connect"<<endl;	
		clsLogM->put_EventHistData(FLAG_END, -1);
 		delete PMSMQ; 		
 		delete clsLogM;
 		exit(-1);
	}			

	PDSNMQ     = new MessageQueue<CPdsnMsg>	(systemId, serviceId, groupId, processType, pserialNo, 11, 1000);
	PARSERMQ   = new MessageQueue<UdrFormat>(systemId, serviceId, groupId, processType, pserialNo, 10, 1000);

	char strProcName[50];
	memset(strProcName, 0x00, sizeof(strProcName));
	sprintf(strProcName, "%d%dREF%02dX", systemId, serviceId, pserialNo);

	clsRfObj   = new CRefMannager(strProcName, argv[2]);
	nInRecSize = clsRfObj->get_InTotLen(); //[t_clt_action]table에서 한 Input Record의 크기를 읽어옴
	strInBuf   = new char[nInRecSize+1];

	memset((char*)&clsPdsnMsg, 0x00, sizeof(clsPdsnMsg));
    
    
    /*--------------------------------------------------------------------*/
    while (1)
    {
	    if(PDSNMQ->recvMsg(&clsPdsnMsg) != FLAG_MQ_IS_EMPTY)//017 Pdsn Udr 처리
        {		
			*clsLogM<<clsLogM->ErrorCode(0)<<endl;
			*clsLogM<<clsLogM->ErrorCode(0)<<"------------------------------------"<<endl;
			*clsLogM<<clsLogM->ErrorCode(0)<<"-Received Udr from PDSN."        <<endl;

        	if(ProcesingPdsn() < 0)
        	{
        		clsLogM->put_EventHistData(FLAG_END, -1);
        		Destructor();
        		exit(-1);
        	}

			memset((char*)&clsPdsnMsg, 0x00, sizeof(clsPdsnMsg));
		}

		if(PARSERMQ->recvMsg(&clsUdrFm) != FLAG_MQ_IS_EMPTY)//017 Parser Udr 처리
		{
			*clsLogM<<clsLogM->ErrorCode(0)<<endl;
			*clsLogM<<clsLogM->ErrorCode(0)<<"------------------------------------"<<endl;
			*clsLogM<<clsLogM->ErrorCode(0)<<"-Received Udr from Parser."  <<endl;
			*clsLogM<<clsLogM->ErrorCode(0)<<"-Udr Type     : ["<<clsUdrFm.stItemHead.get_nUdrType()   <<"]"<<endl;
			*clsLogM<<clsLogM->ErrorCode(0)<<"-Udr File Seq : ["<<clsUdrFm.stItemHead.get_nUdrFileSeq()<<"]"<<endl;
			*clsLogM<<clsLogM->ErrorCode(0)<<"-Udr Seq      : ["<<clsUdrFm.stItemHead.get_nUdrSeqNo()  <<"]"<<endl;			
			*clsLogM<<clsLogM->ErrorCode(0)<<"-Svc Provider : ["<<clsUdrFm.stItemHead.get_nServiceProvider()<<"]"<<endl;			
			*clsLogM<<clsLogM->ErrorCode(0)<<"-MIN1         : ["<<clsUdrFm.stItemHead.get_nMin1()      <<"]"<<endl;			
			*clsLogM<<clsLogM->ErrorCode(0)<<"-MIN2         : ["<<clsUdrFm.stItemHead.get_nMin2()      <<"]"<<endl;			
			
			memset(strInBuf	  , 0x00, nInRecSize+1);
	   		memset(gstrAtribut, 0x00, sizeof(gstrAtribut));
			memcpy(strInBuf   , (char*)&clsUdrFm, nInRecSize);

			if(ProcesingParser(strInBuf) < 0)
			{
				clsLogM->put_EventHistData(FLAG_END, -1);
				Destructor();
				exit(-1);
			}

			memset((char*)&clsPdsnMsg, 0x00, sizeof(clsPdsnMsg));
		}
		
		usleep(1000);
	}	
	/*--------------------------------------------------------------------*/	

	clsLogM->put_EventHistData(FLAG_END, EXIT_SUCCESS);
	Destructor();
	return 0;
}		




//------------------------------------------------------------------------------
// Pdsn MQ로 들어오는 017 Udr을 파일로 만든 후, Config file에 구성정보를 write한다.
int ProcesingPdsn()
{
	if(clsCfgCTRL->write_Udr(&clsPdsnMsg) < 0)
	{
		*clsLogM << "Error No = [" << clsLogM->ErrorCode(9002)<<"] Cannot open or write  file for Write PDSN Udr.[" << errno << "]" << endl;
		return -1;
	}

	*clsLogM<<clsLogM->ErrorCode(0)<<"-Header Id    : ["<< (int)clsPdsnMsg.get_ucIdentifier()<<"]"<<endl;
	*clsLogM<<clsLogM->ErrorCode(0)<<"-Success Write to File PDSN Udr."    << endl;
	*clsLogM<<clsLogM->ErrorCode(0)<<"------------------------------------"<<endl;		
	
	return 1;

}	 




//------------------------------------------------------------------------------
// Parser MQ로 들어오는 017 Udr을 원시Pdsn Udr로 변환하여 파일로 만든 후, 
// Config file에 구성정보를 write한다.
int ProcesingParser(char* strInputBuf)
{
	if(clsRfObj->pdsnRule(strInputBuf, gstrAtribut)<0)
	{
		*clsLogM<<"Error No = ["<<clsLogM->ErrorCode(9003)<<"] Fail Conversion from Parsing Udr to Pdsn Udr format." << endl;
		return -1;
	}	

	clsPdsnMsg.set_ucCode(REQUEST_CODE);
	clsPdsnMsg.set_usLength(clsRfObj->get_OutTotLen()+ SIZE_HEADER);

	memcpy(clsPdsnMsg.strAttribute, gstrAtribut, sizeof(gstrAtribut));

	if(clsCfgCTRL->write_Udr(&clsPdsnMsg) < 0)
	{
		*clsLogM<<"Error No = ["<<clsLogM->ErrorCode(9004)<<"] Cannot open or write for Write Reformat Udr.[" << errno << "]" << endl;
		return -1;
	}

	*clsLogM<<clsLogM->ErrorCode(0)<<"-Header Id    : ["<< (int)clsPdsnMsg.get_ucIdentifier()<<"]"<<endl;
	*clsLogM<<clsLogM->ErrorCode(0)<<"-Success Write to File Reformat Udr."<< endl;
	*clsLogM<<clsLogM->ErrorCode(0)<<"------------------------------------"<<endl;		

	return 1;
}	



//------------------------------------------------------------------------------
// PMS에게 Process의 상태를 알려 준다.
void writePMSMQ(int sigNo)
{
	int i = 0;
	char PMSMQ_NO[2];
	MsgPmsStatus	OneRec(MSG_STATUS, systemId, serviceId, groupId, processType, pserialNo, 10);

	OneRec.set_nStatus(sigNo);
	
	sprintf(PMSMQ_NO, "%d", systemId);
	
	while(1)
	{
		i++;
		if (PMSMQ->sendMsg(&OneRec) == FLAG_MQ_IS_FULL)
		{
			*clsLogM<<"Error No = ["<<clsLogM->ErrorCode(9005)<<"] PMSMQIS FULL sleep(1)...!"<<endl;
			sleep(1);
			if (i >= 10)
			{
				kill(0,SIGINT);
			}
		}
		else
		{
			break;
		}
	}
}



//------------------------------------------------------------------------------
// Signal 처리
void sigCapture(int sigNo)
{
	*clsLogM<<"Error No = ["<<clsLogM->ErrorCode(9006)<<"] Got Signal...Process End!!!" << endl;
	
	switch(sigNo)
	{
		case SIGINT :
		case SIGQUIT :
		case SIGKILL :
		case SIGTERM :
				writePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				clsLogM->put_EventHistData(FLAG_END, -1);
				Destructor();
				exit(-1);
				break;
		case SIGUSR1:
				writePMSMQ(FLAG_PROC_TERM);//0//프로세스정상stop (Client업무명령)
				clsLogM->put_EventHistData(FLAG_END, 0);
				Destructor();
				exit(0);
				break;
		default:
				writePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				clsLogM->put_EventHistData(FLAG_END, -1);
				Destructor();
				exit(-1);
				break;
	}
}



//------------------------------------------------------------------------------
// 동적 생성된 객체를 delete한다.
void Destructor()
{
	clsDbConnect.disconnect();
	delete [] strInBuf;
	delete PDSNMQ;
	delete PARSERMQ;
	delete PMSMQ;				
	delete clsLogM;
	delete clsCfgCTRL;
	delete clsRfObj;
}
