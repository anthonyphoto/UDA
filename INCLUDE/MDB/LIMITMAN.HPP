/****************************************************************************/
/* �� �� �� : LimitMan.hpp                                                  */
/* �� �� �� : KSI / �ѱ���                                                  */
/* ��    �� : UDR�� ���ڷ� �޾�, �ѵ���������� �˻�����, �ѵ����� ��û��   */
/*        	  ���ִ� ���α׷�	     										*/
/* �ۼ����� : 2000�� 10�� 31��                                              */
/* �������� :     ��   ��   ��                                              */
/* ����ȭ�� : Aggregator.hpp                                                */
/* ��    Ÿ :                                                               */
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
		// �ش� UDR�� �ѵ������� �ؾ��ϴ��� ã�Ƴ��� �޼ҵ�
		int				 	   	CompareServiceType( UdrFormat* pUdrFormat );
		int						LimitAccessRequest( UdrFormat* pUdrFormat, int nCompareFlag );
		int						LimitInterimAccountRequest( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat, int nCompareFlag );
		int						LimitServiceEndRequest( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat, int nCompareFlag );
		int						MakeAndSendRequest( UdrFormat* pUdrFormat, int nFlag );
		void 					CompareLimitInfo( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat );
	private:
		short				    nServiceType;   //  Packet or nTop
        short			 	   	nSubsType;   //  PPS or TTL_School
        int						nForwardMUXOpt; // ���ۼӵ�
        unsigned int  			unMsId;
        unsigned int 			nAccountCount;   // �ѵ�������û count�� �����ϴ� ����.
        AccountCnt*				pAccountCnt;     // �ѵ�������û count�� �������� ���� �ν��Ͻ�
        char    				strAccountSessionID [LEN_ACCOUNT_SESSION_ID+1];
        char   					strCorrelationID    [LEN_CORRELATION_ID+1];
        SCPFormatAgg      	  	cRequestData;	 // SCP I/F�� ����ϱ� ���� Data Format
        RateRule 				rate;	         // �־��� �ð��� ��Ŷ���� ���� �ݾ��� ����ϱ� ���� �ν��Ͻ�
        
	
		// �ѵ������� A-S MQ�� ��û�ϴ� �޼ҵ�.
		int 					RequireLimitInfo( SCPFormatAgg* pRequestData );

};

#endif



