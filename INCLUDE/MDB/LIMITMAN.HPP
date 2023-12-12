/****************************************************************************/
/* 파 일 명 : LimitMan.hpp                                                  */
/* 작 성 자 : KSI / 한근희                                                  */
/* 기    능 : UDR을 인자로 받아, 한도관리요건을 검색한후, 한도관리 요청을   */
/*        	  해주는 프로그램	     										*/
/* 작성일자 : 2000년 10월 31일                                              */
/* 변경일자 :     년   월   일                                              */
/* 관련화일 : Aggregator.hpp                                                */
/* 기    타 :                                                               */
/*        																	*/
/****************************************************************************/
#ifndef LIMITMAN_HPP
#define LIMITMAN_HPP

#include <scpformat.hpp>
#include <MDB/RateRule.hpp>
//#include <fileSearchC.hpp>
#include <SendLogManage.hpp>

char 	PATH_NAME_ACCOUNT_COUNT[80] 	= "/SVC#/DATA/AGG_AccountCnt.cfg";
#define LIMIT_DEBUG	1
enum LIMIT_TYPE      { NOT_LIMIT, LIMIT_PACKET, LIMIT_TIME };


class CLimitMan
{
	public:
		CLimitMan( int nServiceId );
		CLimitMan();
		~CLimitMan();
		// 해당 UDR이 한도관리를 해야하는지 찾아내는 메소드
		int				 	   	CompareServiceType( UdrFormat* pUdrFormat );
		int						LimitAccessRequest( UdrFormat* pUdrFormat, int nCompareFlag );
		int						LimitInterimAccountRequest( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat, int nCompareFlag );
		int						LimitServiceEndRequest( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat, int nCompareFlag );
		int						MakeAndSendRequest( UdrFormat* pUdrFormat, int nFlag );
		void 					CompareLimitInfo( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat );
	private:
		short				    nServiceType;   //  Packet or nTop
        short			 	   	nSubsType;   //  PPS or TTL_School
        int						nForwardMUXOpt; // 전송속도
        unsigned int  			unMsId;
        unsigned int 			nAccountCount;   // 한도정보요청 count를 저장하는 변수.
        AccountCnt*				pAccountCnt;     // 한도정보요청 count를 가져오기 위한 인스턴스
        char    				strAccountSessionID [LEN_ACCOUNT_SESSION_ID+1];
        char   					strCorrelationID    [LEN_CORRELATION_ID+1];
        SCPFormatAgg      	  	cRequestData;	 // SCP I/F와 통신하기 위한 Data Format
        RateRule 				rate;	         // 주어진 시간과 패킷량에 따라 금액을 계산하기 위한 인스턴스
        
	
		// 한도정보를 A-S MQ에 요청하는 메소드.
		int 					RequireLimitInfo( SCPFormatAgg* pRequestData );

};

#endif




