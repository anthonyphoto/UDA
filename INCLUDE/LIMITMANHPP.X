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
#include <fileSearchC.hpp>

char 	PATH_NAME_ACCOUNT_COUNT[80] 	= "/SVC#/DATA/AGG_AccountCnt.cfg";
//const int INTERIM_ACCOUNT_COUNT = 10;
#define LIMIT_DEBUG	1
enum LIMIT_TYPE      { NOT_LIMIT, LIMIT_PACKET, LIMIT_TIME };
// AGG -> Scp-if(S)
MessageQueue<SCPFormatAgg> *mqSCP;

class CLimitMan
{
	public:
		CLimitMan( int nServiceId );
		~CLimitMan();
		// int 			    LimitMan( UdrFormat* pUdrFormat );
		// �ش� UDR�� �ѵ������� �ؾ��ϴ��� ã�Ƴ��� �޼ҵ�
		int				 	   	CompareServiceType( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat, int nFlag );
		int						LimitAccessRequest( UdrFormat* pUdrFormat, int nCompareFlag );
		int						LimitServiceStopRequest( UdrFormat* pUdrFormat );
		int						LimitInterimAccountRequest( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat, int nCompareFlag );
		int						LimitServiceEndRequest( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat, int nCompareFlag );
		int						SendEndRequest( UdrFormat* pSearchUdrFormat );
		//int						AccessRequestResult( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat );
		//int						InterimAccountRequestResult( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat );

	private:
		short				    nServiceType;   //  Packet or nTop
        short			 	   	nSubsType;   //  PPS or TTL_School
        int						nForwardMUXOpt; // ���ۼӵ�
        unsigned int  			unMsId;
        unsigned int 			nAccountCount;
        AccountCnt*				pAccountCnt;
        char    				strAccountSessionID [LEN_ACCOUNT_SESSION_ID+1];
        char   					strCorrelationID    [LEN_CORRELATION_ID+1];
        SCPFormatAgg      	  	cRequestData;
        RateRule 				rate;
        
	
		// �ѵ������� A-S MQ�� ��û�ϴ� �޼ҵ�.
		int 					RequireLimitInfo( SCPFormatAgg* pRequestData );

};
CLimitMan::CLimitMan(int nServiceId )
{
	sprintf( PATH_NAME_ACCOUNT_COUNT, "/SVC%d/DATA/AGG_AccountCnt.cfg",nServiceId );
	pAccountCnt = new AccountCnt(PATH_NAME_ACCOUNT_COUNT);
	pAccountCnt->read_AccountCnt();
	nAccountCount = pAccountCnt->getAccountCnt();
}
CLimitMan::~CLimitMan()
{
}

// Access Request
// return value : 1 -> Success
//                0 -> Fail
int	CLimitMan::LimitAccessRequest( UdrFormat* pUdrFormat, int nCompareFlag )
{
	/*
	int 	nTatalDataOctet;
	int		nRatingPrice;
	int		nTempPrice;
	int 	nRatingTime;
	int		nTempTime;
	int		nRemainPrice;
*/
	
	if( LIMIT_DEBUG )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0)
				<< "CLimitMan : LimitAccessRequest!!" << endl;
	}	
	// UDR�� LIMIT_INFO�κ��� ä���ش�.
	// Start�� �ش��ϴ� ������.
	pUdrFormat->get_LimitInfo()->put_nOperationID( 0 );
	pUdrFormat->get_LimitInfo()->put_nSessionStartTime( pUdrFormat->get_ItemG()->get_nStartEventTime() );
	pUdrFormat->get_LimitInfo()->put_nSessionEndTime( pUdrFormat->get_ItemG()->get_nEndEventTime() );
	pUdrFormat->get_LimitInfo()->put_nSessionCount( 0 );
	pUdrFormat->get_LimitInfo()->put_nLimitPrice( 0 );
	pUdrFormat->get_LimitInfo()->put_nNtopFreeTime( 0 );
	pUdrFormat->get_LimitInfo()->put_nRemain( 0 );
	pUdrFormat->get_LimitInfo()->put_nQueryResult( 0 );
	
	memset( ( char* )&cRequestData, NULL, sizeof( SCPFormatAgg ) );
	// �ѵ������� ��û�� ����ü�� ����� �ش�.  
	cRequestData.get_ItemA2()->set_Key(0x01);
	cRequestData.get_ItemA2()->set_Min( pUdrFormat->get_ItemA()->get_unMsId() );	
	cRequestData.get_ItemB()->set_OperId(0);
	cRequestData.get_ItemB()->set_AccSessionId( pUdrFormat->get_ItemC()->get_strAccountSessionID() );
	cRequestData.get_ItemB()->set_CorreId( pUdrFormat->get_ItemC()->get_strCorrelationID() );
	if( 1 )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "============ Access Request ���� ===========" <<  endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "unMsId=" << cRequestData.get_ItemA2()->get_Min()<<  endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "AccSessionId=" << cRequestData.get_ItemB()->get_AccSessionId()<<  endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "CorreId=" << cRequestData.get_ItemB()->get_CorreId()<<  endl;
	}	
	// SCP I/F�� Access Request�� ������.
	if( !RequireLimitInfo( &cRequestData ) )
	{
		if( LIMIT_DEBUG )
		{
			*AggregatorLogM << AggregatorLogM->ErrorCode(4200) 
				<< "Error Access Request!!" << endl;
			return 0;
		}
	}
	else
	{
		if( LIMIT_DEBUG )
		{
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "Complete Access Request!!" << endl;
		}
	}
	
	return 1;
}
// Interim Account Request
// return value : 1 -> Success
//                0 -> Fail
// ¥���� �ð��� octet�� �����Ű�� �ʾ���.
int	CLimitMan::LimitInterimAccountRequest( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat, int nCompareFlag )
{
	int 	nTempSessionCount;
	int 	nTempOperationID;
	int		nRatedTatalDataOctet;
	int 	nTatalDataOctet;
	int 	nRatingDataOctet;
	int		nRatingPrice=0;
	//int 	nTotalRatingPrice;
	//int		nRatedTime;
	int 	nRatingTime;
	//int     nTotalRatingTime;
	int		nTempPrice;
	int		nTempTime;
	int		nRemainPrice=0;
	int		nRemainTime=0;
	int		nRemain=0;
	int		nQueryResultFlag=0;
	
	if( LIMIT_DEBUG )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "CLimitMan : LimitInterimAccountRequest!!" << endl;
	}
	
	nTempSessionCount = pSearchUdrFormat->get_LimitInfo()->get_nSessionCount();
	nTempOperationID = pSearchUdrFormat->get_LimitInfo()->get_nOperationID();
	nQueryResultFlag = pSearchUdrFormat->get_LimitInfo()->get_nQueryResult();
	
	// UDR�� LIMIT_INFO�κ��� ä���ش�.
	// OperationID�� ����.******************
	pUdrFormat->get_LimitInfo()->put_nOperationID( ++nTempOperationID );
	pUdrFormat->get_LimitInfo()->put_nSessionStartTime( pSearchUdrFormat->get_LimitInfo()->get_nSessionStartTime() );
	pUdrFormat->get_LimitInfo()->put_nSessionEndTime( pUdrFormat->get_ItemG()->get_nEndEventTime() );
	pUdrFormat->get_LimitInfo()->put_nSessionCount( ++nTempSessionCount );
	pUdrFormat->get_LimitInfo()->put_nLimitPrice( pSearchUdrFormat->get_LimitInfo()->get_nLimitPrice() );
	pUdrFormat->get_LimitInfo()->put_nNtopFreeTime( pSearchUdrFormat->get_LimitInfo()->get_nNtopFreeTime() );
		
	
	switch( nCompareFlag )
	{
		case LIMIT_PACKET:
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "******************>pSearchUdrFormat->get_LimitInfo()->get_nRemain()=" << pSearchUdrFormat->get_LimitInfo()->get_nRemain() << endl;
			// ������ ��Ŷ���� ���
			nTatalDataOctet = pUdrFormat->get_ItemG()->get_nDataOctetCntTerminat() + pUdrFormat->get_ItemG()->get_nDataOctetCntOrigin();
			nRatingDataOctet =  pSearchUdrFormat->get_LimitInfo()->get_nRemain();
			nTatalDataOctet = nTatalDataOctet - nRatingDataOctet;
		//	nRatedTatalDataOctet = pSearchUdrFormat->get_ItemG()->get_nDataOctetCntTerminat() + pSearchUdrFormat->get_ItemG()->get_nDataOctetCntOrigin();
		//	nRatingDataOctet = nTatalDataOctet - nRatedTatalDataOctet + pSearchUdrFormat->get_LimitInfo()->get_nRemain();
			if( nTatalDataOctet <= 0 )
			{
				*AggregatorLogM << AggregatorLogM->ErrorCode(4201) 
					<< "Small Rating Data Octet!!" << endl;
				return 0;
			}
			// �Ի걭 ��Ŷ������ ���ݾ��� ���.
			// ����ϰ� ���� ¥������ �����Ű�� �ʴ´�.
			nRatingPrice = rate.GetRate(2, pUdrFormat->get_LimitInfo()->get_nSessionStartTime(), pUdrFormat->get_LimitInfo()->get_nSessionEndTime(), 0, nTatalDataOctet );
			nRemain = rate.GetLimitSec();
			if( LIMIT_DEBUG )
			{
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "============ �ѵ�ó����� ===========" <<  endl;
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "�ѵ� �ݾ� : " << pUdrFormat->get_LimitInfo()->get_nLimitPrice() <<  endl;
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "��� DataOctet : " << nTatalDataOctet<<  endl;
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "���ݾ� : " << nRatingPrice<<  endl;
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "������ ������ DataOctet : " << nRemain <<  endl;
				
			}	
			pUdrFormat->get_LimitInfo()->put_nUsePrice( nRatingPrice );
			nTempPrice=pUdrFormat->get_LimitInfo()->get_nLimitPrice()-nRatingPrice;
			
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "���� �ѵ� �ݾ� : " << nTempPrice <<  endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "nQueryResultFlag : " << nQueryResultFlag <<  endl;
			// �ѹ��� �ѵ���û ������ ���� �ʾ������� stop request�� ���� �ʾƾ� �Ѵ�.
			// �ϴ� �ѵ� �ݾ��� 0�ΰ����� �Ǵ��Ѵ�.
			if( nQueryResultFlag )
			{
				// �ѵ� �ʰ��� Service Stop Request��û
				if( nTempPrice <= 0 )
				{
					LimitServiceStopRequest( pUdrFormat );
				}
			}
			break;
		case LIMIT_TIME:
			// ������ �ð��� ���
			nRatingTime = pUdrFormat->get_LimitInfo()->get_nSessionEndTime() - pUdrFormat->get_LimitInfo()->get_nSessionStartTime();
			if( nRatingTime < 0 )
			{
				*AggregatorLogM << AggregatorLogM->ErrorCode(4202) 
					<< "Small Rating Time!!" << endl;
				return 0;
			}
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "============ �ѵ�ó����� ===========" <<  endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "�ѵ� �ݾ� : " << pUdrFormat->get_LimitInfo()->get_nLimitPrice() <<  endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "N-top ������ð� : " << pUdrFormat->get_LimitInfo()->get_nNtopFreeTime()<<  endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "�� ���ð� : " << nRatingTime <<  endl;
			
			// N-top ������ð��� �ִٸ�, ���� ������ð����� ���ð��� ���ش�.
			nRemainTime=pUdrFormat->get_LimitInfo()->get_nNtopFreeTime()-nRatingTime;
			
			// �ѵ� �ʰ��� Service Stop Request��û
			if( nRemainTime <= 0 )			
			{
					nTempTime = nRemainTime*-1;
					// ���� �ð������� ���ݾ��� ���
					nTempPrice = rate.GetRate(1, pUdrFormat->get_LimitInfo()->get_nSessionStartTime()+(nRatingTime-nTempTime), pUdrFormat->get_LimitInfo()->get_nSessionEndTime(),0 );
					nRemain = rate.GetLimitSec();	
					pUdrFormat->get_LimitInfo()->put_nUseTime( nRatingTime-nTempTime );
					pUdrFormat->get_LimitInfo()->put_nUsePrice( nTempPrice );
					nRemainPrice = pUdrFormat->get_LimitInfo()->get_nLimitPrice()-nTempPrice;
					
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "N-top������ð��� �ʰ��� �ð� : " << ( nRemainTime * (-1) ) <<  endl;
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "���ݾ� : " << nTempPrice<<  endl;
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "���������� �ð� : " << nRemain <<  endl;	
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "���� �ѵ� �ݾ� : " << nRemainPrice <<  endl;
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "nQueryResultFlag : " << nQueryResultFlag <<  endl;
				// flag check ���.
				if( nQueryResultFlag )
				{
					if( nRemainPrice <= 0 )
					{
						LimitServiceStopRequest( pUdrFormat );
					}
				}
			}
			else
			{
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "����� �ð� : " << nRatingTime <<  endl;
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "���� N-top������ð� : " << nRemainTime<<  endl;
				pUdrFormat->get_LimitInfo()->put_nUseTime( nRatingTime );
			}
			break;
		default :
			*AggregatorLogM << AggregatorLogM->ErrorCode(4203) 
					<< "Error Compare Limit Info!!" << endl;
			break;
	}
	
	if( 0 )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "============Display InterimAccountRequest Result===========" <<  endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nOperationID=" << pUdrFormat->get_LimitInfo()->get_nOperationID() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nSessionStartTime=" << pUdrFormat->get_LimitInfo()->get_nSessionStartTime() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nSessionEndTime=" << pUdrFormat->get_LimitInfo()->get_nSessionEndTime() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nSessionCount=" << pUdrFormat->get_LimitInfo()->get_nSessionCount() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nLimitPrice=" << pUdrFormat->get_LimitInfo()->get_nLimitPrice() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nNtopFreeTime=" << pUdrFormat->get_LimitInfo()->get_nNtopFreeTime() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nUsePrice=" << pUdrFormat->get_LimitInfo()->get_nUsePrice() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nUseTime=" << pUdrFormat->get_LimitInfo()->get_nUseTime() <<endl;
	}
	
	*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nTempSessionCount=" << nTempSessionCount <<endl;	
	// ���� �߰������û�� Ƚ���� �ƴٸ�
	if( nTempSessionCount == nAccountCount )
	{
		memset( ( char* )&cRequestData, NULL, sizeof( SCPFormatAgg ) );
		// �ѵ������� ��û�� ����ü�� ����� �ش�.  
		cRequestData.get_ItemA2()->set_Key(0x02);
		cRequestData.get_ItemA2()->set_Min( pUdrFormat->get_ItemA()->get_unMsId() );	
		cRequestData.get_ItemB()->set_OperId( nTempOperationID );
		cRequestData.get_ItemB()->set_AccSessionId( pUdrFormat->get_ItemC()->get_strAccountSessionID() );
		cRequestData.get_ItemB()->set_CorreId( pUdrFormat->get_ItemC()->get_strCorrelationID() );
		cRequestData.get_ItemC()->set_StartT( pUdrFormat->get_LimitInfo()->get_nSessionStartTime() );
		cRequestData.get_ItemC()->set_EndT( pUdrFormat->get_LimitInfo()->get_nSessionEndTime() );
		cRequestData.get_ItemD()->set_LimitT( pUdrFormat->get_LimitInfo()->get_nUsePrice() );
		cRequestData.get_ItemD()->set_NTuseT( pUdrFormat->get_LimitInfo()->get_nUseTime() );
		
		if( LIMIT_DEBUG )
		{
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "============ Interim Account Request ���� ===========" <<  endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "Key=" << cRequestData.get_ItemA2()->get_Key() <<endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "unMsId=" << cRequestData.get_ItemA2()->get_Min() <<endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "OperId=" << cRequestData.get_ItemB()->get_OperId() <<endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "AccSessionId=" << cRequestData.get_ItemB()->get_AccSessionId() <<endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "CorreId=" << cRequestData.get_ItemB()->get_CorreId() <<endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "StartT=" << cRequestData.get_ItemC()->get_StartT() <<endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "EndT=" << cRequestData.get_ItemC()->get_EndT() <<endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "LimitT=" << cRequestData.get_ItemD()->get_LimitT() <<endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "NTuseT=" << cRequestData.get_ItemD()->get_NTuseT() <<endl;	
		}
		
		// SCP I/F�� �߰������û�� ������.
		if( !RequireLimitInfo( &cRequestData ) )
		{
			if( LIMIT_DEBUG )
			{
				*AggregatorLogM << AggregatorLogM->ErrorCode(4204) 
					<< "Error Interim Account Request!!" << endl;
				return 0;
			}
		}
		else
		{
			if( LIMIT_DEBUG )
			{
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "Complete Interim Account Request!!" << endl;
			}
		}
		// UDR�� LIMIT_INFO�κ��� �������� �ʱ�ȭ�� �ش�.
		pUdrFormat->get_LimitInfo()->put_nSessionCount( 0 );
		
		// UDR�� LIMIT_INFO�κ��� �������� ������ش�.
		// sessing count, nLimitPrice, nNtopFreeTime, nUsePrice, nUseTime
		switch( nCompareFlag )
		{
			case LIMIT_PACKET:
				pUdrFormat->get_LimitInfo()->put_nLimitPrice( nTempPrice );
				pUdrFormat->get_LimitInfo()->put_nUsePrice( 0 );
				// SessionStartTime�� Update���ش�.
				pUdrFormat->get_LimitInfo()->put_nSessionStartTime( pUdrFormat->get_LimitInfo()->get_nSessionEndTime() );
				pUdrFormat->get_LimitInfo()->put_nRemain( (nTatalDataOctet-nRemain)+nRatingDataOctet );
				break;
			case LIMIT_TIME:
				if( nRemainTime >= 0 )
				{
					pUdrFormat->get_LimitInfo()->put_nNtopFreeTime( nRemainTime );
				}
				else
				{
					pUdrFormat->get_LimitInfo()->put_nNtopFreeTime( 0 );
					pUdrFormat->get_LimitInfo()->put_nLimitPrice( nRemainPrice );
				}
				pUdrFormat->get_LimitInfo()->put_nUseTime( 0 );
				pUdrFormat->get_LimitInfo()->put_nUsePrice( 0 );
				// SessionStartTime�� Update���ش�.
				pUdrFormat->get_LimitInfo()->put_nSessionStartTime( ( pUdrFormat->get_LimitInfo()->get_nSessionEndTime()- nRemain ) );	
				// pUdrFormat->get_LimitInfo()->put_nRemain( 0 );
				break;
			default :
				*AggregatorLogM << AggregatorLogM->ErrorCode(4203) 
					<< "Error Compare Limit Info!!" << endl;
				break;
		}
	}
	
	return 1;
}

// Service End Request
// return value : 1 -> Success
//                0 -> Fail
int	CLimitMan::LimitServiceEndRequest( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat, int nCompareFlag )
{
	//int 	nTempSessionCount;
	//int 	nTempOperationID;
	int		nRatedTatalDataOctet;
	int 	nTatalDataOctet;
	int 	nRatingDataOctet;
	int		nRatingPrice=0;
	//int 	nTotalRatingPrice;
	//int		nRatedTime;
	int 	nRatingTime;
	int     nTotalRatingTime;
	int		nTempPrice;
	int		nTempTime;
	int		nRemainTime;
	int		nRemainPrice;
	int		nQueryResultFlag=0;
		
	if( LIMIT_DEBUG )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "CLimitMan : LimitServiceEndRequest!!" << endl;
	}	
	//nTempSessionCount = pSearchUdrFormat->get_LimitInfo()->get_nSessionCount();
	//nTempOperationID = pSearchUdrFormat->get_LimitInfo()->get_nOperationID();
	
	// UDR�� LIMIT_INFO�κ��� ä���ش�.
	// OperationID�� ����.******************
	pUdrFormat->get_LimitInfo()->put_nSessionStartTime( pSearchUdrFormat->get_LimitInfo()->get_nSessionStartTime() );
	pUdrFormat->get_LimitInfo()->put_nSessionEndTime( pUdrFormat->get_ItemG()->get_nEndEventTime() );
	pUdrFormat->get_LimitInfo()->put_nLimitPrice( pSearchUdrFormat->get_LimitInfo()->get_nLimitPrice() );
	pUdrFormat->get_LimitInfo()->put_nNtopFreeTime( pSearchUdrFormat->get_LimitInfo()->get_nNtopFreeTime() );

	switch( nCompareFlag )
	{
		case LIMIT_PACKET:
			// ������ ��Ŷ���� ���
			nTatalDataOctet = pUdrFormat->get_ItemG()->get_nDataOctetCntTerminat() + pUdrFormat->get_ItemG()->get_nDataOctetCntOrigin();
			nRatingDataOctet =  pSearchUdrFormat->get_LimitInfo()->get_nRemain();
			nTatalDataOctet = nTatalDataOctet - nRatingDataOctet;
		//	nRatedTatalDataOctet = pSearchUdrFormat->get_ItemG()->get_nDataOctetCntTerminat() + pSearchUdrFormat->get_ItemG()->get_nDataOctetCntOrigin();
		//	nRatingDataOctet = nTatalDataOctet - nRatedTatalDataOctet + pSearchUdrFormat->get_LimitInfo()->get_nRemain();
			if( nTatalDataOctet <= 0 )
			{
				*AggregatorLogM << AggregatorLogM->ErrorCode(4201) 
					<< "Small Rating Data Octet!!" << endl;
				return 0;
			}
			// �Ի걭 ��Ŷ������ ���ݾ��� ���.
			// ����ϰ� ���� ¥������ �����Ű�� �ʴ´�.
			nRatingPrice = rate.GetRate(2, pUdrFormat->get_LimitInfo()->get_nSessionStartTime(), pUdrFormat->get_LimitInfo()->get_nSessionEndTime(), 1, nTatalDataOctet);
			if( LIMIT_DEBUG )
			{
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "============ �ѵ�ó����� ===========" <<  endl;
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "�ѵ� �ݾ� : " << pUdrFormat->get_LimitInfo()->get_nLimitPrice() <<  endl;
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "��� DataOctet : " << nTatalDataOctet<<  endl;
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "���ݾ� : " << nRatingPrice<<  endl;
			}	
			pUdrFormat->get_LimitInfo()->put_nUsePrice( nRatingPrice );
			nTempPrice=pUdrFormat->get_LimitInfo()->get_nLimitPrice()-nRatingPrice;
			
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "���� �ѵ� �ݾ� : " << nTempPrice <<  endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "nQueryResultFlag : " << nQueryResultFlag <<  endl;
			// �ѹ��� �ѵ���û ������ ���� �ʾ������� stop request�� ���� �ʾƾ� �Ѵ�.
			// �ϴ� �ѵ� �ݾ��� 0�ΰ����� �Ǵ��Ѵ�.
			if( nQueryResultFlag )
			{
				// �ѵ� �ʰ��� Service Stop Request��û
				if( nTempPrice <= 0 )
				{
					LimitServiceStopRequest( pUdrFormat );
				}
			}
			break;
		case LIMIT_TIME:
			// ������ �ð��� ���
			nRatingTime = pUdrFormat->get_LimitInfo()->get_nSessionEndTime() - pUdrFormat->get_LimitInfo()->get_nSessionStartTime();
			if( nRatingTime < 0 )
			{
				*AggregatorLogM << AggregatorLogM->ErrorCode(4202) 
					<< "Small Rating Time!!" << endl;
				return 0;
			}
			//nTotalRatingTime = nRatingTime + pSearchUdrFormat->get_LimitInfo()->get_nUseTime();
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "============ �ѵ�ó����� ===========" <<  endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "�ѵ� �ݾ� : " << pUdrFormat->get_LimitInfo()->get_nLimitPrice() <<  endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "N-top ������ð� : " << pUdrFormat->get_LimitInfo()->get_nNtopFreeTime()<<  endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "�� ���ð� : " << nRatingTime <<  endl;
			
			// N-top ������ð��� �ִٸ�, ���� ������ð����� ���ð��� ���ش�.
			nRemainTime=pUdrFormat->get_LimitInfo()->get_nNtopFreeTime()-nRatingTime;
			
			// �ѵ� �ʰ��� Service Stop Request��û
			if( nRemainTime <= 0 )			
			{
					nTempTime = nRemainTime*-1;
					// ���� �ð������� ���ݾ��� ���
					nTempPrice = rate.GetRate(1, pUdrFormat->get_LimitInfo()->get_nSessionStartTime()+(nRatingTime-nTempTime), pUdrFormat->get_LimitInfo()->get_nSessionEndTime(), 1 );
						
					pUdrFormat->get_LimitInfo()->put_nUseTime( nRatingTime-nTempTime );
					pUdrFormat->get_LimitInfo()->put_nUsePrice( nTempPrice );
					nRemainPrice = pUdrFormat->get_LimitInfo()->get_nLimitPrice()-nTempPrice;
					
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "N-top������ð��� �ʰ��� �ð� : " << ( nRemainTime * (-1) ) <<  endl;
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "���ݾ� : " << nTempPrice <<  endl;
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "���� �ݾ� : " << nRemainPrice <<  endl;
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "nQueryResultFlag : " << nQueryResultFlag <<  endl;
				if( nQueryResultFlag )
				{
					if( nRemainPrice <= 0 )
					{
						LimitServiceStopRequest( pUdrFormat );
					}
				}
			}
			else
			{
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "����� �ð� : " << nRatingTime<<  endl;
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "���� N-top������ð� : " << nRemainTime<<  endl;
				pUdrFormat->get_LimitInfo()->put_nUseTime( nRatingTime );
			}
			break;
		default :
			*AggregatorLogM << AggregatorLogM->ErrorCode(4203) 
					<< "Error Compare Limit Info!!" << endl;
			break;
	}

	memset( ( char* )&cRequestData, NULL, sizeof( SCPFormatAgg ) );
	// �ѵ������� ��û�� ����ü�� ����� �ش�.  
	cRequestData.get_ItemA2()->set_Key(0x04);
	cRequestData.get_ItemA2()->set_Min( pUdrFormat->get_ItemA()->get_unMsId() );	
	cRequestData.get_ItemC()->set_StartT( pUdrFormat->get_LimitInfo()->get_nSessionStartTime() );
	cRequestData.get_ItemC()->set_EndT( pUdrFormat->get_LimitInfo()->get_nSessionEndTime() );
	cRequestData.get_ItemD()->set_LimitT( pUdrFormat->get_LimitInfo()->get_nUsePrice() );
	cRequestData.get_ItemD()->set_NTuseT( pUdrFormat->get_LimitInfo()->get_nUseTime() );
	
	if( LIMIT_DEBUG )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "============ Service End Request ���� ===========" <<  endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "Key=" << cRequestData.get_ItemA2()->get_Key() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "unMsId=" << cRequestData.get_ItemA2()->get_Min() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "StartT=" << cRequestData.get_ItemC()->get_StartT() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "EndT=" << cRequestData.get_ItemC()->get_EndT() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "LimitT=" << cRequestData.get_ItemD()->get_LimitT() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "NTuseT=" << cRequestData.get_ItemD()->get_NTuseT() <<endl;	
	}
	
	// SCP I/F�� �߰������û�� ������.
	if( !RequireLimitInfo( &cRequestData ) )
	{
		if( LIMIT_DEBUG )
		{
			*AggregatorLogM << AggregatorLogM->ErrorCode(4205) 
					<< "Error Service End Request!!" << endl;
			return 0;
		}
	}
	else
	{
		if( LIMIT_DEBUG )
		{
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "Complete Service End Request!!" << endl;
		}
	}

	// UDR�� LIMIT_INFO�κ��� �������� �ʱ�ȭ�� �ش�.
		pUdrFormat->get_LimitInfo()->put_nSessionCount( 0 );
		
		// UDR�� LIMIT_INFO�κ��� �������� ������ش�.
		// sessing count, nLimitPrice, nNtopFreeTime, nUsePrice, nUseTime
		switch( nCompareFlag )
		{
			case LIMIT_PACKET:
				pUdrFormat->get_LimitInfo()->put_nLimitPrice( nTempPrice );
				pUdrFormat->get_LimitInfo()->put_nUsePrice( 0 );
				break;
			case LIMIT_TIME:
				if( nRemainTime >= 0 )
				{
					pUdrFormat->get_LimitInfo()->put_nNtopFreeTime( nRemainTime );
				}
				else
				{
					pUdrFormat->get_LimitInfo()->put_nNtopFreeTime( 0 );
					pUdrFormat->get_LimitInfo()->put_nLimitPrice( nRemainPrice );
				}
				pUdrFormat->get_LimitInfo()->put_nUseTime( 0 );
				pUdrFormat->get_LimitInfo()->put_nUsePrice( 0 );
				//pUdrFormat->get_LimitInfo()->put_nRemain( 0 );
				break;
			default :
				*AggregatorLogM << AggregatorLogM->ErrorCode(4203) 
					<< "Error Compare Limit Info!!" << endl;
				break;
		}

	return 1;
}


// Send End Request
// return value : 1 -> Success
//                0 -> Fail
int	CLimitMan::SendEndRequest( UdrFormat* pSearchUdrFormat )
{
	if( LIMIT_DEBUG )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "CLimitMan : SendEndRequest!!" << endl;
	}	

	memset( ( char* )&cRequestData, NULL, sizeof( SCPFormatAgg ) );
	// �ѵ������� ��û�� ����ü�� ����� �ش�.  
	cRequestData.get_ItemA2()->set_Key(0x04);
	cRequestData.get_ItemA2()->set_Min( pSearchUdrFormat->get_ItemA()->get_unMsId() );	
	cRequestData.get_ItemC()->set_StartT( pSearchUdrFormat->get_LimitInfo()->get_nSessionStartTime() );
	cRequestData.get_ItemC()->set_EndT( pSearchUdrFormat->get_LimitInfo()->get_nSessionEndTime() );
	cRequestData.get_ItemD()->set_LimitT( pSearchUdrFormat->get_LimitInfo()->get_nUsePrice() );
	cRequestData.get_ItemD()->set_NTuseT( pSearchUdrFormat->get_LimitInfo()->get_nUseTime() );
	
	if( LIMIT_DEBUG )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "============ Service End Request ���� ===========" <<  endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "Key=" << cRequestData.get_ItemA2()->get_Key() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "unMsId=" << cRequestData.get_ItemA2()->get_Min() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "StartT=" << cRequestData.get_ItemC()->get_StartT() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "EndT=" << cRequestData.get_ItemC()->get_EndT() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "LimitT=" << cRequestData.get_ItemD()->get_LimitT() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "NTuseT=" << cRequestData.get_ItemD()->get_NTuseT() <<endl;	
	}
	
	// SCP I/F�� �߰������û�� ������.
	if( !RequireLimitInfo( &cRequestData ) )
	{
		if( LIMIT_DEBUG )
		{
			*AggregatorLogM << AggregatorLogM->ErrorCode(4205) 
					<< "Error Service End Request!!" << endl;
			return 0;
		}
	}
	else
	{
		if( LIMIT_DEBUG )
		{
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "Complete Service End Request!!" << endl;
		}
	}

	return 1;
}



// Service Stop Request
// return value : 1 -> Success
//                0 -> Fail
int	CLimitMan::LimitServiceStopRequest( UdrFormat* pUdrFormat )
{
	if( LIMIT_DEBUG )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "CLimitMan : LimitServiceStopRequest!!" << endl;
	}	
	memset( ( char* )&cRequestData, NULL, sizeof( SCPFormatAgg ) );
	// �ѵ������� ��û�� ����ü�� ����� �ش�.  
	cRequestData.get_ItemA2()->set_Key(0x03);
	cRequestData.get_ItemA2()->set_Min( pUdrFormat->get_ItemA()->get_unMsId() );	

	if( 0 )
	{
		cout << "***********************" << endl;
		cout << "unMsId= " << cRequestData.get_ItemA2()->get_Min() << endl;
		cout << "***********************" << endl;
	}	
	// SCP I/F�� Access Request�� ������.
	if( !RequireLimitInfo( &cRequestData ) )
	{
		if( LIMIT_DEBUG )
		{
			*AggregatorLogM << AggregatorLogM->ErrorCode(4200) 
				<< "Error Access Request!!" << endl;
			return 0;
		}
	}
	else
	{
		if( LIMIT_DEBUG )
		{
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "Complete Access Request!!" << endl;
			return 1;
		}
	}
}
/*
// Access Request Result
// return value : 1 -> Success
//                0 -> Fail
int	CLimitMan::AccessRequestResult( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat )
{
	int 	nNewOperationID;
	int 	nLastOperationID;
	int 	nTempNewPrice;
	int 	nTempNewTime;
	
	if( LIMIT_DEBUG )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "CLimitMan : AccessRequestResult!!" << endl;
	}	
	if( LIMIT_DEBUG )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "============ ������ ������ȸ ��� ===========" <<  endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "MsId=" << pUdrFormat->get_ItemA()->get_unMsId() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nOperationID=" << pUdrFormat->get_LimitInfo()->get_nOperationID() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "strAccountSessionID=" << pUdrFormat->get_ItemC()->get_strAccountSessionID() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "strCorrelationID=" << pUdrFormat->get_ItemC()->get_strCorrelationID() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "�ѵ��ܾ�=" << pUdrFormat->get_LimitInfo()->get_nLimitPrice() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "N-top ������ð�=" << pUdrFormat->get_LimitInfo()->get_nNtopFreeTime() <<endl;
	}	
	
	if( 0 )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "============Display Old Udr===========" <<  endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nOperationID=" << pSearchUdrFormat->get_LimitInfo()->get_nOperationID() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nLimitPrice=" << pSearchUdrFormat->get_LimitInfo()->get_nLimitPrice() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nNtopFreeTime=" << pSearchUdrFormat->get_LimitInfo()->get_nNtopFreeTime() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "strAccountSessionID=" << pSearchUdrFormat->get_ItemC()->get_strAccountSessionID() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "strCorrelationID=" << pSearchUdrFormat->get_ItemC()->get_strCorrelationID() <<endl;
	}
	
	nNewOperationID = pUdrFormat->get_LimitInfo()->get_nOperationID();
	nLastOperationID = pSearchUdrFormat->get_LimitInfo()->get_nOperationID();
	
	if( nNewOperationID > nLastOperationID )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(4206) 
				<< "Error Operation ID!!" << endl;
		return 0;
	}
	nTempNewPrice=pUdrFormat->get_LimitInfo()->get_nLimitPrice();
	nTempNewTime=pUdrFormat->get_LimitInfo()->get_nNtopFreeTime();
	// �ѵ� �ܾװ� n-Top������ �ܾ׽ð��� �־��ش�.
	pSearchUdrFormat->get_LimitInfo()->put_nQueryResult( 1 );
	pSearchUdrFormat->get_LimitInfo()->put_nLimitPrice( nTempNewPrice );
	pSearchUdrFormat->get_LimitInfo()->put_nNtopFreeTime( nTempNewTime );

	if( 0 )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "============ UPDATE�� ��� ===========" <<  endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "�ѵ��ܾ�=" << pSearchUdrFormat->get_LimitInfo()->get_nLimitPrice() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "N-top ������ð�=" << pSearchUdrFormat->get_LimitInfo()->get_nNtopFreeTime() <<endl;
	}

	return 1;
}

// Interim Account Request Result
// return value : 1 -> Success
//                0 -> Fail
int	CLimitMan::InterimAccountRequestResult( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat )
{
	int 	nNewOperationID;
	int 	nLastOperationID;
	
	if( LIMIT_DEBUG )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "CLimitMan : InterimAccountRequestResult!!" << endl;
	}	
	if( LIMIT_DEBUG )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "============�����ڿ�� �߰����� ���===========" <<  endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "MsId=" << pUdrFormat->get_ItemA()->get_unMsId() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "nOperationID=" << pUdrFormat->get_LimitInfo()->get_nOperationID() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "strAccountSessionID=" << pUdrFormat->get_ItemC()->get_strAccountSessionID() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "strCorrelationID=" << pUdrFormat->get_ItemC()->get_strCorrelationID() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "�ѵ��ܾ�=" << pUdrFormat->get_LimitInfo()->get_nLimitPrice() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "N-top ������ð�=" << pUdrFormat->get_LimitInfo()->get_nNtopFreeTime() <<endl;
		
	}	
	
	nNewOperationID = pUdrFormat->get_LimitInfo()->get_nOperationID();
	nLastOperationID = pSearchUdrFormat->get_LimitInfo()->get_nOperationID();
	
	if(  nLastOperationID < nNewOperationID )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(4206) 
				<< "Error Operation ID!!" << endl;
		return 0;
	}
	else if( nLastOperationID = nNewOperationID )
	{
		// �ѵ� �ܾװ� n-Top������ �ܾ׽ð��� update
		pSearchUdrFormat->get_LimitInfo()->put_nLimitPrice( pUdrFormat->get_LimitInfo()->get_nLimitPrice() );
		pSearchUdrFormat->get_LimitInfo()->put_nNtopFreeTime( pUdrFormat->get_LimitInfo()->get_nNtopFreeTime() );
		pSearchUdrFormat->get_LimitInfo()->put_nQueryResult( 1 );
		if( LIMIT_DEBUG )
		{
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "============ UPDATE�� ��� ===========" <<  endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "�ѵ��ܾ�=" << pSearchUdrFormat->get_LimitInfo()->get_nLimitPrice() <<endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "N-top ������ð�=" << pSearchUdrFormat->get_LimitInfo()->get_nNtopFreeTime() <<endl;
		}
	}
	else
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "passing UDR!!" << endl;
	}
	return 1;
}
*/
// return value : NOT_LIMIT->�Ϲ� UDR
//                LIMIT_PACKET-> Packet��� �ѵ�����
//				  LIMIT_TIME -> �ð���� �ѵ����� 
int CLimitMan::CompareServiceType( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat, int nFlag )
{
	int TempServiceType;
	int TempSubsType;
	
	if( LIMIT_DEBUG )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "CLimitMan : CompareServiceType!!" << endl;
	}	
	
	nServiceType = pUdrFormat->get_ItemHead()->get_nServiceType();
	nSubsType = pUdrFormat->get_ItemHead()->get_nSubsType();
	nForwardMUXOpt = pUdrFormat->get_ItemF()->get_nForwardMUXOpt();
	
	if( 0 )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "==========CompareServiceType============" <<  endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nServiceType=" <<nServiceType <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nSubsType=" << nSubsType <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nForwardMUXOpt=" << nForwardMUXOpt <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "========================================" <<  endl;
	}	
	
	// ���� UDR�� �ѵ������� ���� UDR�� �ѵ������� ������ ���Ѵ�.
	// ���� ���� �ʴٸ� ������ ������ �����Ѵ�.
	// START UDR�϶��� ����
	if( nFlag )
	{
		TempServiceType = pSearchUdrFormat->get_ItemHead()->get_nServiceType();
		TempSubsType = pSearchUdrFormat->get_ItemHead()->get_nSubsType();
		if( TempServiceType != nServiceType )
		{
			nServiceType = TempServiceType;
			pUdrFormat->get_ItemHead()->put_nServiceType(nServiceType);
		}
		if( TempSubsType != nSubsType )
		{
			nSubsType = TempSubsType;
			pUdrFormat->get_ItemHead()->put_nSubsType(nSubsType);
		}
	}
	
	if( LIMIT_DEBUG )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "==========CompareServiceType============" <<  endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nServiceType=" <<nServiceType <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nSubsType=" << nSubsType <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nForwardMUXOpt=" << nForwardMUXOpt <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "========================================" <<  endl;
	}	
	
	switch( nServiceType )
	{
		// �Ϲ� packet ���ݹ��
		case PACKET :
			// TTL School��ݹ���̸� Packet���� �ѵ�����.
			if( nSubsType == 1 )
			{
				if( LIMIT_DEBUG )
				{
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "Packet Limit UDR!!" << endl;
				}
				return 1;
		    }
		    // �Ϲ� UDR
		    else
		    {
		    	if( LIMIT_DEBUG )
				{
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "Normal UDR!!" << endl;
				}
				return 0;
		    }
		    
			break;
		// N-Top Service
		case NTOP :
			// N_Top�̸鼭 TTL School�̸� 
			if( nSubsType == 1 )
			{
				// if Forward Mux Option�� 1 or 2�̸�
				if( nForwardMUXOpt== 1 || nForwardMUXOpt == 2 )
				{
					if( LIMIT_DEBUG )
					{
						*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
							<< "TIME Limit UDR!!" << endl;
					}
					return 2;
				}
				else
				{
					if( LIMIT_DEBUG )
					{
						*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
							<< "Normal UDR!!" << endl;
					}
					return 0;
				}
		    }
		    else
		    {
		    	if( LIMIT_DEBUG )
				{
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "Normal UDR!!" << endl;
				}
				return 0;
		    }
			break;
		default :
			if( LIMIT_DEBUG )
			{
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "Normal UDR!!" << endl;
			}
			return 0;
			break;
	}
}

// return value : 0 -> fail send MQ
//                1 -> success require info
int CLimitMan::RequireLimitInfo( SCPFormatAgg* pRequestData )
{
    if( LIMIT_DEBUG )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "SCP�� ������û" << endl;
	}	
    // Send request Aggregator - Scp I/F MQ
    while(1)
    {
        if( mqSCP->sendMsg( pRequestData ) == FLAG_MQ_IS_FULL )
        {
        	*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "Wait  MSG Queue FULL!!" << endl;
  //         return 0;
        }
        else break;
    }
    return 1;
}
#endif




