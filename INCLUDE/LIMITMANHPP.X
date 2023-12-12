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
		// 해당 UDR이 한도관리를 해야하는지 찾아내는 메소드
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
        int						nForwardMUXOpt; // 전송속도
        unsigned int  			unMsId;
        unsigned int 			nAccountCount;
        AccountCnt*				pAccountCnt;
        char    				strAccountSessionID [LEN_ACCOUNT_SESSION_ID+1];
        char   					strCorrelationID    [LEN_CORRELATION_ID+1];
        SCPFormatAgg      	  	cRequestData;
        RateRule 				rate;
        
	
		// 한도정보를 A-S MQ에 요청하는 메소드.
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
	// UDR의 LIMIT_INFO부분을 채워준다.
	// Start에 해당하는 변수만.
	pUdrFormat->get_LimitInfo()->put_nOperationID( 0 );
	pUdrFormat->get_LimitInfo()->put_nSessionStartTime( pUdrFormat->get_ItemG()->get_nStartEventTime() );
	pUdrFormat->get_LimitInfo()->put_nSessionEndTime( pUdrFormat->get_ItemG()->get_nEndEventTime() );
	pUdrFormat->get_LimitInfo()->put_nSessionCount( 0 );
	pUdrFormat->get_LimitInfo()->put_nLimitPrice( 0 );
	pUdrFormat->get_LimitInfo()->put_nNtopFreeTime( 0 );
	pUdrFormat->get_LimitInfo()->put_nRemain( 0 );
	pUdrFormat->get_LimitInfo()->put_nQueryResult( 0 );
	
	memset( ( char* )&cRequestData, NULL, sizeof( SCPFormatAgg ) );
	// 한도정보를 요청할 구조체를 만들어 준다.  
	cRequestData.get_ItemA2()->set_Key(0x01);
	cRequestData.get_ItemA2()->set_Min( pUdrFormat->get_ItemA()->get_unMsId() );	
	cRequestData.get_ItemB()->set_OperId(0);
	cRequestData.get_ItemB()->set_AccSessionId( pUdrFormat->get_ItemC()->get_strAccountSessionID() );
	cRequestData.get_ItemB()->set_CorreId( pUdrFormat->get_ItemC()->get_strCorrelationID() );
	if( 1 )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "============ Access Request 정보 ===========" <<  endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "unMsId=" << cRequestData.get_ItemA2()->get_Min()<<  endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "AccSessionId=" << cRequestData.get_ItemB()->get_AccSessionId()<<  endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "CorreId=" << cRequestData.get_ItemB()->get_CorreId()<<  endl;
	}	
	// SCP I/F로 Access Request를 보낸다.
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
// 짜투리 시간과 octet은 적용시키지 않았음.
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
	
	// UDR의 LIMIT_INFO부분을 채워준다.
	// OperationID를 증가.******************
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
			// 과금할 패킷량을 계산
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
			// 게산괸 패킷량으로 사용금액을 계산.
			// 계산하고 남은 짜투리는 적용시키지 않는다.
			nRatingPrice = rate.GetRate(2, pUdrFormat->get_LimitInfo()->get_nSessionStartTime(), pUdrFormat->get_LimitInfo()->get_nSessionEndTime(), 0, nTatalDataOctet );
			nRemain = rate.GetLimitSec();
			if( LIMIT_DEBUG )
			{
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "============ 한도처리결과 ===========" <<  endl;
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "한도 금액 : " << pUdrFormat->get_LimitInfo()->get_nLimitPrice() <<  endl;
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "사용 DataOctet : " << nTatalDataOctet<<  endl;
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "사용금액 : " << nRatingPrice<<  endl;
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "과금후 나머지 DataOctet : " << nRemain <<  endl;
				
			}	
			pUdrFormat->get_LimitInfo()->put_nUsePrice( nRatingPrice );
			nTempPrice=pUdrFormat->get_LimitInfo()->get_nLimitPrice()-nRatingPrice;
			
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "남은 한도 금액 : " << nTempPrice <<  endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "nQueryResultFlag : " << nQueryResultFlag <<  endl;
			// 한번도 한도요청 응답을 받지 않았을때는 stop request를 하지 않아야 한다.
			// 일단 한도 금액이 0인것으로 판단한다.
			if( nQueryResultFlag )
			{
				// 한도 초과시 Service Stop Request요청
				if( nTempPrice <= 0 )
				{
					LimitServiceStopRequest( pUdrFormat );
				}
			}
			break;
		case LIMIT_TIME:
			// 과금할 시간을 계산
			nRatingTime = pUdrFormat->get_LimitInfo()->get_nSessionEndTime() - pUdrFormat->get_LimitInfo()->get_nSessionStartTime();
			if( nRatingTime < 0 )
			{
				*AggregatorLogM << AggregatorLogM->ErrorCode(4202) 
					<< "Small Rating Time!!" << endl;
				return 0;
			}
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "============ 한도처리결과 ===========" <<  endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "한도 금액 : " << pUdrFormat->get_LimitInfo()->get_nLimitPrice() <<  endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "N-top 무료사용시간 : " << pUdrFormat->get_LimitInfo()->get_nNtopFreeTime()<<  endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "총 사용시간 : " << nRatingTime <<  endl;
			
			// N-top 무료사용시간이 있다면, 먼저 무료사용시간에서 사용시간을 빼준다.
			nRemainTime=pUdrFormat->get_LimitInfo()->get_nNtopFreeTime()-nRatingTime;
			
			// 한도 초과시 Service Stop Request요청
			if( nRemainTime <= 0 )			
			{
					nTempTime = nRemainTime*-1;
					// 남은 시간양으로 사용금액을 계산
					nTempPrice = rate.GetRate(1, pUdrFormat->get_LimitInfo()->get_nSessionStartTime()+(nRatingTime-nTempTime), pUdrFormat->get_LimitInfo()->get_nSessionEndTime(),0 );
					nRemain = rate.GetLimitSec();	
					pUdrFormat->get_LimitInfo()->put_nUseTime( nRatingTime-nTempTime );
					pUdrFormat->get_LimitInfo()->put_nUsePrice( nTempPrice );
					nRemainPrice = pUdrFormat->get_LimitInfo()->get_nLimitPrice()-nTempPrice;
					
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "N-top무료사용시간을 초과한 시간 : " << ( nRemainTime * (-1) ) <<  endl;
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "사용금액 : " << nTempPrice<<  endl;
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "계산되지않은 시간 : " << nRemain <<  endl;	
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "남은 한도 금액 : " << nRemainPrice <<  endl;
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "nQueryResultFlag : " << nQueryResultFlag <<  endl;
				// flag check 요망.
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
					<< "사용한 시간 : " << nRatingTime <<  endl;
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "남은 N-top무료사용시간 : " << nRemainTime<<  endl;
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
	// 만약 중간정산요청할 횟수가 됐다면
	if( nTempSessionCount == nAccountCount )
	{
		memset( ( char* )&cRequestData, NULL, sizeof( SCPFormatAgg ) );
		// 한도정보를 요청할 구조체를 만들어 준다.  
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
					<< "============ Interim Account Request 정보 ===========" <<  endl;
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
		
		// SCP I/F로 중간정산요청을 보낸다.
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
		// UDR의 LIMIT_INFO부분의 변수들을 초기화해 준다.
		pUdrFormat->get_LimitInfo()->put_nSessionCount( 0 );
		
		// UDR의 LIMIT_INFO부분의 변수들을 계산해준다.
		// sessing count, nLimitPrice, nNtopFreeTime, nUsePrice, nUseTime
		switch( nCompareFlag )
		{
			case LIMIT_PACKET:
				pUdrFormat->get_LimitInfo()->put_nLimitPrice( nTempPrice );
				pUdrFormat->get_LimitInfo()->put_nUsePrice( 0 );
				// SessionStartTime을 Update해준다.
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
				// SessionStartTime을 Update해준다.
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
	
	// UDR의 LIMIT_INFO부분을 채워준다.
	// OperationID를 증가.******************
	pUdrFormat->get_LimitInfo()->put_nSessionStartTime( pSearchUdrFormat->get_LimitInfo()->get_nSessionStartTime() );
	pUdrFormat->get_LimitInfo()->put_nSessionEndTime( pUdrFormat->get_ItemG()->get_nEndEventTime() );
	pUdrFormat->get_LimitInfo()->put_nLimitPrice( pSearchUdrFormat->get_LimitInfo()->get_nLimitPrice() );
	pUdrFormat->get_LimitInfo()->put_nNtopFreeTime( pSearchUdrFormat->get_LimitInfo()->get_nNtopFreeTime() );

	switch( nCompareFlag )
	{
		case LIMIT_PACKET:
			// 과금할 패킷량을 계산
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
			// 게산괸 패킷량으로 사용금액을 계산.
			// 계산하고 남은 짜투리는 적용시키지 않는다.
			nRatingPrice = rate.GetRate(2, pUdrFormat->get_LimitInfo()->get_nSessionStartTime(), pUdrFormat->get_LimitInfo()->get_nSessionEndTime(), 1, nTatalDataOctet);
			if( LIMIT_DEBUG )
			{
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "============ 한도처리결과 ===========" <<  endl;
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "한도 금액 : " << pUdrFormat->get_LimitInfo()->get_nLimitPrice() <<  endl;
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "사용 DataOctet : " << nTatalDataOctet<<  endl;
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "사용금액 : " << nRatingPrice<<  endl;
			}	
			pUdrFormat->get_LimitInfo()->put_nUsePrice( nRatingPrice );
			nTempPrice=pUdrFormat->get_LimitInfo()->get_nLimitPrice()-nRatingPrice;
			
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "남은 한도 금액 : " << nTempPrice <<  endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "nQueryResultFlag : " << nQueryResultFlag <<  endl;
			// 한번도 한도요청 응답을 받지 않았을때는 stop request를 하지 않아야 한다.
			// 일단 한도 금액이 0인것으로 판단한다.
			if( nQueryResultFlag )
			{
				// 한도 초과시 Service Stop Request요청
				if( nTempPrice <= 0 )
				{
					LimitServiceStopRequest( pUdrFormat );
				}
			}
			break;
		case LIMIT_TIME:
			// 과금할 시간을 계산
			nRatingTime = pUdrFormat->get_LimitInfo()->get_nSessionEndTime() - pUdrFormat->get_LimitInfo()->get_nSessionStartTime();
			if( nRatingTime < 0 )
			{
				*AggregatorLogM << AggregatorLogM->ErrorCode(4202) 
					<< "Small Rating Time!!" << endl;
				return 0;
			}
			//nTotalRatingTime = nRatingTime + pSearchUdrFormat->get_LimitInfo()->get_nUseTime();
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "============ 한도처리결과 ===========" <<  endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "한도 금액 : " << pUdrFormat->get_LimitInfo()->get_nLimitPrice() <<  endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "N-top 무료사용시간 : " << pUdrFormat->get_LimitInfo()->get_nNtopFreeTime()<<  endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "총 사용시간 : " << nRatingTime <<  endl;
			
			// N-top 무료사용시간이 있다면, 먼저 무료사용시간에서 사용시간을 빼준다.
			nRemainTime=pUdrFormat->get_LimitInfo()->get_nNtopFreeTime()-nRatingTime;
			
			// 한도 초과시 Service Stop Request요청
			if( nRemainTime <= 0 )			
			{
					nTempTime = nRemainTime*-1;
					// 남은 시간양으로 사용금액을 계산
					nTempPrice = rate.GetRate(1, pUdrFormat->get_LimitInfo()->get_nSessionStartTime()+(nRatingTime-nTempTime), pUdrFormat->get_LimitInfo()->get_nSessionEndTime(), 1 );
						
					pUdrFormat->get_LimitInfo()->put_nUseTime( nRatingTime-nTempTime );
					pUdrFormat->get_LimitInfo()->put_nUsePrice( nTempPrice );
					nRemainPrice = pUdrFormat->get_LimitInfo()->get_nLimitPrice()-nTempPrice;
					
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "N-top무료사용시간을 초과한 시간 : " << ( nRemainTime * (-1) ) <<  endl;
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "사용금액 : " << nTempPrice <<  endl;
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "남은 금액 : " << nRemainPrice <<  endl;
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
					<< "사용한 시간 : " << nRatingTime<<  endl;
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "남은 N-top무료사용시간 : " << nRemainTime<<  endl;
				pUdrFormat->get_LimitInfo()->put_nUseTime( nRatingTime );
			}
			break;
		default :
			*AggregatorLogM << AggregatorLogM->ErrorCode(4203) 
					<< "Error Compare Limit Info!!" << endl;
			break;
	}

	memset( ( char* )&cRequestData, NULL, sizeof( SCPFormatAgg ) );
	// 한도정보를 요청할 구조체를 만들어 준다.  
	cRequestData.get_ItemA2()->set_Key(0x04);
	cRequestData.get_ItemA2()->set_Min( pUdrFormat->get_ItemA()->get_unMsId() );	
	cRequestData.get_ItemC()->set_StartT( pUdrFormat->get_LimitInfo()->get_nSessionStartTime() );
	cRequestData.get_ItemC()->set_EndT( pUdrFormat->get_LimitInfo()->get_nSessionEndTime() );
	cRequestData.get_ItemD()->set_LimitT( pUdrFormat->get_LimitInfo()->get_nUsePrice() );
	cRequestData.get_ItemD()->set_NTuseT( pUdrFormat->get_LimitInfo()->get_nUseTime() );
	
	if( LIMIT_DEBUG )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "============ Service End Request 정보 ===========" <<  endl;
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
	
	// SCP I/F로 중간정산요청을 보낸다.
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

	// UDR의 LIMIT_INFO부분의 변수들을 초기화해 준다.
		pUdrFormat->get_LimitInfo()->put_nSessionCount( 0 );
		
		// UDR의 LIMIT_INFO부분의 변수들을 계산해준다.
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
	// 한도정보를 요청할 구조체를 만들어 준다.  
	cRequestData.get_ItemA2()->set_Key(0x04);
	cRequestData.get_ItemA2()->set_Min( pSearchUdrFormat->get_ItemA()->get_unMsId() );	
	cRequestData.get_ItemC()->set_StartT( pSearchUdrFormat->get_LimitInfo()->get_nSessionStartTime() );
	cRequestData.get_ItemC()->set_EndT( pSearchUdrFormat->get_LimitInfo()->get_nSessionEndTime() );
	cRequestData.get_ItemD()->set_LimitT( pSearchUdrFormat->get_LimitInfo()->get_nUsePrice() );
	cRequestData.get_ItemD()->set_NTuseT( pSearchUdrFormat->get_LimitInfo()->get_nUseTime() );
	
	if( LIMIT_DEBUG )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "============ Service End Request 정보 ===========" <<  endl;
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
	
	// SCP I/F로 중간정산요청을 보낸다.
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
	// 한도정보를 요청할 구조체를 만들어 준다.  
	cRequestData.get_ItemA2()->set_Key(0x03);
	cRequestData.get_ItemA2()->set_Min( pUdrFormat->get_ItemA()->get_unMsId() );	

	if( 0 )
	{
		cout << "***********************" << endl;
		cout << "unMsId= " << cRequestData.get_ItemA2()->get_Min() << endl;
		cout << "***********************" << endl;
	}	
	// SCP I/F로 Access Request를 보낸다.
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
			<< "============ 가입자 정보조회 결과 ===========" <<  endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "MsId=" << pUdrFormat->get_ItemA()->get_unMsId() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nOperationID=" << pUdrFormat->get_LimitInfo()->get_nOperationID() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "strAccountSessionID=" << pUdrFormat->get_ItemC()->get_strAccountSessionID() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "strCorrelationID=" << pUdrFormat->get_ItemC()->get_strCorrelationID() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "한도잔액=" << pUdrFormat->get_LimitInfo()->get_nLimitPrice() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "N-top 무료사용시간=" << pUdrFormat->get_LimitInfo()->get_nNtopFreeTime() <<endl;
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
	// 한도 잔액과 n-Top무료사용 잔액시간을 넣어준다.
	pSearchUdrFormat->get_LimitInfo()->put_nQueryResult( 1 );
	pSearchUdrFormat->get_LimitInfo()->put_nLimitPrice( nTempNewPrice );
	pSearchUdrFormat->get_LimitInfo()->put_nNtopFreeTime( nTempNewTime );

	if( 0 )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "============ UPDATE된 결과 ===========" <<  endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "한도잔액=" << pSearchUdrFormat->get_LimitInfo()->get_nLimitPrice() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "N-top 무료사용시간=" << pSearchUdrFormat->get_LimitInfo()->get_nNtopFreeTime() <<endl;
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
			<< "============가입자요금 중간정산 결과===========" <<  endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "MsId=" << pUdrFormat->get_ItemA()->get_unMsId() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "nOperationID=" << pUdrFormat->get_LimitInfo()->get_nOperationID() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "strAccountSessionID=" << pUdrFormat->get_ItemC()->get_strAccountSessionID() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "strCorrelationID=" << pUdrFormat->get_ItemC()->get_strCorrelationID() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "한도잔액=" << pUdrFormat->get_LimitInfo()->get_nLimitPrice() <<endl;
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "N-top 무료사용시간=" << pUdrFormat->get_LimitInfo()->get_nNtopFreeTime() <<endl;
		
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
		// 한도 잔액과 n-Top무료사용 잔액시간을 update
		pSearchUdrFormat->get_LimitInfo()->put_nLimitPrice( pUdrFormat->get_LimitInfo()->get_nLimitPrice() );
		pSearchUdrFormat->get_LimitInfo()->put_nNtopFreeTime( pUdrFormat->get_LimitInfo()->get_nNtopFreeTime() );
		pSearchUdrFormat->get_LimitInfo()->put_nQueryResult( 1 );
		if( LIMIT_DEBUG )
		{
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "============ UPDATE된 결과 ===========" <<  endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "한도잔액=" << pSearchUdrFormat->get_LimitInfo()->get_nLimitPrice() <<endl;
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "N-top 무료사용시간=" << pSearchUdrFormat->get_LimitInfo()->get_nNtopFreeTime() <<endl;
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
// return value : NOT_LIMIT->일반 UDR
//                LIMIT_PACKET-> Packet방식 한도관리
//				  LIMIT_TIME -> 시간방식 한도관리 
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
	
	// 이전 UDR의 한도정보와 현재 UDR의 한도정보가 같은지 비교한다.
	// 만약 같지 않다면 과거의 정보를 유지한다.
	// START UDR일때는 제외
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
		// 일반 packet 과금방식
		case PACKET :
			// TTL School요금방식이면 Packet으로 한도관리.
			if( nSubsType == 1 )
			{
				if( LIMIT_DEBUG )
				{
					*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
						<< "Packet Limit UDR!!" << endl;
				}
				return 1;
		    }
		    // 일반 UDR
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
			// N_Top이면서 TTL School이면 
			if( nSubsType == 1 )
			{
				// if Forward Mux Option이 1 or 2이면
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
				<< "SCP에 정보요청" << endl;
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





