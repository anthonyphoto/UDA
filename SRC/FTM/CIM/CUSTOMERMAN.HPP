/******************************************************************
* 파 일 명     : CustmerMan.hpp			     					  *
* 기   능                                                          *
* : CCustomerMan class 정의.										  *
* 작 성 자     : Han Guen Hee                    				  *
* first data  : 2000. 11. 07       	     						  *
* last updata : 2000. 12. 21        		 					  *
* 관 련 화 일  : CustmerMan.pc                					  *
* program history                        						  *
*                         	        	 				 		  *
* bug fix                        	     						  *
*                         	        	 						  *
******************************************************************/
#ifndef CUSTOMERMAN_HPP
#define CUSTOMERMAN_HPP
                        
#include <iostream.h>   
#include <TempMsgQ_file.hpp>
#include <CustMdbMan.hpp>

#define  EVENT_DATE_LEN  8 
#define  EVENT_TIME_LEN  6
#define  PRICE_PLAN_LEN  5
#define  DB_USERNAME  "uda"
#define  DB_PASSWORD  "uda"

static int	SystemId;
static int	ServiceId;
static int	GroupId;
static int	ProcessType;
static int 	PserialNo;
static int	PreMark;


// CDS I/F -> customer 
MessageQueue<CdsCustFormat> *mqCUST;
// Scp-if(R) to 0m040|8.
//MessageQueue<CustFormat> *mqCUST;
// 0m040|8. to SCP-IF(S)
//MessageQueue<ReCustFormat> *mqReCUST;
// PMS MQ
MessageQueue<MsgPmsStatus>* mqPMS;
//	Log Manager
SendLogManage*				CustomerManLogM;

char 	PATH_NAME_CUSTOMER[80] 	= "/SVC#/DATA/SubsShm.cfg";
const    int				CUSTOMER_DEBUG=1;
void sigCapture(int sigNo );
void WritePMSMQ(int sigNo);
class CCustomerMan
{
	public:
								CCustomerMan(); 
								CCustomerMan( int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark );
								~CCustomerMan();
		int            		    TransactInsert( CustFormat stCustFormat );
		int						TransactDelete( CustFormat stCustFormat );
		void 					ReadDataToMQ( void );
		//int        	        ReponseToMQ( ReCustFormat* pSendUdr );
		int						SearchMdbTemp( int nSearchMin );
		int 					ConnectToDB( void );
		int						UploadDb( void );
		int						Db_close( void );

	private:
		CdsCustFormat			cRecvData;  // CDS I/F MQ로부터 읽어들여온 DATA를 저장하는 변수
//		ReCustFormat    		cSendData;
		CustFormatMdb 			cCustFormatMdb;  // 고객관리 MDB Format
		CCustMdbMan*			cCustMdbMan;  // 고객관리MDB를 Handling해주는 class pointer
		
		int 					InsertCustDb( CustFormat stCustFormat );
		int 					deleteCustDb( CustFormat stCustFormat );
		int 					UpdateCustDb( CustFormat stCustFormat );
		int						FetchData( void );
		int 					MatchToDb( CustFormat stCustFormat );
		void					PrintCustFormat( ostream& output );
};

#endif
