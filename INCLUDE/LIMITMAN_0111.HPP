/******************************************************************
* �� �� ��     : LimitMan.hpp 			     					  *
* ��   ��                                                          *
* : UDR�� ���ڷ� �޾�, �ѵ���������� �˻�����, �ѵ������� ����.          *
* �� �� ��     : Han Guen Hee                    				  *
* first data  : 2000. 10. 31       	     						  *
* last updata : 2000. 12. 22        		 					  *
* �� �� ȭ ��  : Aggregator.hpp, AGG_AccountCnt.cfg				  *
* program history                        						  *
*                         	        	 				 		  *
* bug fix                        	     						  *
*                         	        	 						  *
******************************************************************/
#ifndef LIMITMAN_HPP
#define LIMITMAN_HPP

#include <scpformat.hpp>
#include <RateRule.hpp>
#include <fileSearchC.hpp>

char 	PATH_NAME_ACCOUNT_COUNT[80] 	= "/SVC#/DATA/AGG_AccountCnt.cfg";
#define LIMIT_DEBUG	1
enum LIMIT_TYPE      { NOT_LIMIT, LIMIT_PACKET, LIMIT_TIME };
// AGG -> Scp-if(S)
MessageQueue<SCPFormatAgg> *mqSCP;

class CLimitMan
{
	public:
		CLimitMan( int nServiceId );
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
//===============================================================
// Constructor Method											=
//  : �ѵ������� �ʿ��� Config������ �о�´�.				  		=
// argument : nServiceId -> Service ID							=
// return value : None											=
//																=
//===============================================================
CLimitMan::CLimitMan(int nServiceId )
{
	// ������� �ѵ������߰������� �ؾ��ϴ����� ��Ÿ���� 
	// Config data�� �о���� �κ�.
	sprintf( PATH_NAME_ACCOUNT_COUNT, "/SVC%d/DATA/AGG_AccountCnt.cfg",nServiceId );
	pAccountCnt = new AccountCnt(PATH_NAME_ACCOUNT_COUNT);
	pAccountCnt->read_AccountCnt();
	nAccountCount = pAccountCnt->getAccountCnt();
}
// destructor
CLimitMan::~CLimitMan()
{
	delete pAccountCnt;
}
//===============================================================
// LimitAccessRequest Method				   					=
//  : �ѵ�������û�� ���ִ� Method						       		=
// argument     :  pUdrFormat -> ó���ϱ� ���� ���� UdrFormat	=
//				   nCompareFlag -> Packet����, or �ð� ����		=
// return value : 1-> success									=
//                0-> fail										=
//																=
//===============================================================
int	CLimitMan::LimitAccessRequest( UdrFormat* pUdrFormat, int nCompareFlag )
{
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
	
	if( !MakeAndSendRequest( pUdrFormat, 0 ) )
	{
		if( LIMIT_DEBUG )
		{
			// Error Code�� �߰����־�� �Ѵ�.
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "Error Make And Send Request!!" << endl;
			return 0;
		}
	}
	else
	{
		if( LIMIT_DEBUG )
		{
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "Complete Make And Send Request!!" << endl;
		}
	}
	
	return 1;
}
//===============================================================
// LimitInterimAccountRequest Method		   					=
//  : �ѵ����� �߰������ ���ִ� Method.				       		=
//	  ¥���� �ð��� octet�� �����Ű�� �ʾ���.						=
// argument     :  pUdrFormat -> ó���ϱ� ���� ���� UdrFormat	=
//				   pSearchUdrFormat -> ������ MDB�� �ִ� DATA		=
//				   nCompareFlag -> Packet����, or �ð� ����		=
// return value : 1-> success									=
//                0-> fail										=
//																=
//===============================================================
int	CLimitMan::LimitInterimAccountRequest( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat, int nCompareFlag )
{
	int 	nTempSessionCount;
	int 	nTempOperationID;
	int		nRatedTatalDataOctet;
	int 	nTatalDataOctet;
	int 	nRatingDataOctet;
	int		nRatingPrice=0;
	int 	nRatingTime;
	int		nTempPrice;
	int		nTempTime;
	int		nRemainPrice=0;
	int		nRemainTime=0;
	int		nRemain=0;
	int		nQueryResultFlag=0;
	
	// ������ ������ ���´�.
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
	pUdrFormat->get_LimitInfo()->put_nQueryResult( nQueryResultFlag );
	
	switch( nCompareFlag )
	{
		case LIMIT_PACKET:
			// ������ ��Ŷ���� ���
			nTatalDataOctet = pUdrFormat->get_ItemG()->get_nDataOctetCntTerminat() + pUdrFormat->get_ItemG()->get_nDataOctetCntOrigin();
			// ���ſ� ���ݵ� ����.
			nRatingDataOctet =  pSearchUdrFormat->get_LimitInfo()->get_nRemain();
			// ���ſ� ���ݵ� ������ �����ؾ� �Ѵ�.
			pUdrFormat->get_LimitInfo()->put_nRemain( nRatingDataOctet );
			nTatalDataOctet = nTatalDataOctet - nRatingDataOctet;
			if( nTatalDataOctet <= 0 )
			{
				*AggregatorLogM << AggregatorLogM->ErrorCode(4201) 
					<< "Small Rating Data Octet!!" << endl;
				return 0;
			}
			// �Ի걭 ��Ŷ������ ���ݾ��� ���.
			// ����ϰ� ���� ¥������ �����Ű�� �ʴ´�.
			nRatingPrice = rate.GetRate(2, pUdrFormat->get_LimitInfo()->get_nSessionStartTime(), pUdrFormat->get_LimitInfo()->get_nSessionEndTime(), 0, nTatalDataOctet );
			// ����ϰ� ���� ¥���� ��Ŷ
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
					// Service Stop Request
					if( !MakeAndSendRequest( pUdrFormat, 2 ) )
					{
						if( LIMIT_DEBUG )
						{
							// Error Code�� �߰����־�� �Ѵ�.
							*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
								<< "Error Make And Send Request!!" << endl;
							return 0;
						}
					}
					else
					{
						if( LIMIT_DEBUG )
						{
							*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
								<< "Complete Make And Send Request!!" << endl;
						}
					}
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
					// ���������� n-Top����ð��� �� ���ĺ��� �����Ѵ�.
					nTempPrice = rate.GetRate(1, pUdrFormat->get_LimitInfo()->get_nSessionStartTime()+(nRatingTime-nTempTime), pUdrFormat->get_LimitInfo()->get_nSessionEndTime(),0 );
					// ����ϰ� ���� ¥���� �ð�.
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
						// Service Stop Request
						if( !MakeAndSendRequest( pUdrFormat, 2 ) )
						{
							if( LIMIT_DEBUG )
							{
								// Error Code�� �߰����־�� �Ѵ�.
								*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
									<< "Error Make And Send Request!!" << endl;
								return 0;
							}
						}
						else
						{
							if( LIMIT_DEBUG )
							{
								*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
									<< "Complete Make And Send Request!!" << endl;
							}
						}
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
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "Error Compare Limit Info!!" << endl;
			break;
	}
	
	*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "nTempSessionCount=" << nTempSessionCount <<endl;	
	// ���� �߰������û�� Ƚ���� �ƴٸ�
	if( nTempSessionCount == nAccountCount )
	{
		if( !MakeAndSendRequest( pUdrFormat, 1 ) )
		{
			if( LIMIT_DEBUG )
			{
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "Error Make And Send Request!!" << endl;
				return 0;
			}
		}
		else
		{
			if( LIMIT_DEBUG )
			{
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "Complete Make And Send Request!!" << endl;
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
				// ���ݵ� ��Ŷ���� ���.( (����Ŷ��-¥����) + ���ſ� ���ݵ� ��Ŷ�� )
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
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "Error Compare Limit Info!!" << endl;
				break;
		}
	}
	
	return 1;
}
//===============================================================
// LimitServiceEndRequest Method		  	 					=
//  : �ѵ����� END��û�� ���ִ� Method.					       		=
// argument     :  pUdrFormat -> ó���ϱ� ���� ���� UdrFormat	=
//				   pSearchUdrFormat -> ������ MDB�� �ִ� DATA		=
//				   nCompareFlag -> Packet����, or �ð� ����		=
// return value : 1-> success									=
//                0-> fail										=
//																=
//===============================================================
int	CLimitMan::LimitServiceEndRequest( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat, int nCompareFlag )
{
	int		nRatedTatalDataOctet;
	int 	nTatalDataOctet;
	int 	nRatingDataOctet;
	int		nRatingPrice=0;
	int 	nRatingTime;
	int     nTotalRatingTime;
	int		nTempPrice;
	int		nTempTime;
	int		nRemainTime;
	int		nRemainPrice;
	int		nQueryResultFlag=0;
		
	// UDR�� LIMIT_INFO�κ��� ä���ش�.
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
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "Error Compare Limit Info!!" << endl;
			break;
	}

	if( !MakeAndSendRequest( pUdrFormat, 3 ) )
	{
		if( LIMIT_DEBUG )
		{
			// Error Code�� �߰����־�� �Ѵ�.
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "Error Make And Send Request!!" << endl;
			return 0;
		}
	}
	else
	{
		if( LIMIT_DEBUG )
		{
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "Complete Make And Send Request!!" << endl;
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
				pUdrFormat->get_LimitInfo()->put_nRemain( 0 );
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
				pUdrFormat->get_LimitInfo()->put_nSessionStartTime( pUdrFormat->get_LimitInfo()->get_nSessionEndTime() );	
				pUdrFormat->get_LimitInfo()->put_nRemain( 0 );
				break;
			default :
				*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "Error Compare Limit Info!!" << endl;
				break;
		}

	return 1;
}
//===============================================================
// MakeAndSendRequest Method		  	 						=
//  :SCP I/F�� �ش� ��û�� ������ Method.				       		=
// argument     :  pUdrFormat -> ó���ϱ� ���� ���� UdrFormat	=
//				   nFlag -> � ��û�� �ش��ϴ����� ��Ÿ���� Flag	=
// return value : 1-> success									=
//                0-> fail										=
//																=
//===============================================================
int	CLimitMan::MakeAndSendRequest( UdrFormat* pUdrFormat, int nFlag )
{
	memset( ( char* )&cRequestData, NULL, sizeof( SCPFormatAgg ) );
	
	// �ѵ������� ��û�� ����ü�� ����� �ش�.
	cRequestData.get_ItemA2()->set_Min( pUdrFormat->get_ItemA()->get_unMsId() );  
	switch( nFlag )
	{
		// Access Request
		case 0 : 
			cRequestData.get_ItemA2()->set_Key(0x01);
			cRequestData.get_ItemB()->set_OperId( pUdrFormat->get_LimitInfo()->get_nOperationID() );
			cRequestData.get_ItemB()->set_AccSessionId( pUdrFormat->get_ItemC()->get_strAccountSessionID() );
			cRequestData.get_ItemB()->set_CorreId( pUdrFormat->get_ItemC()->get_strCorrelationID() );
			break;
		// Interim Account Request
		case 1 :
			cRequestData.get_ItemA2()->set_Key(0x02);
			cRequestData.get_ItemB()->set_OperId( pUdrFormat->get_LimitInfo()->get_nOperationID() );
			cRequestData.get_ItemB()->set_AccSessionId( pUdrFormat->get_ItemC()->get_strAccountSessionID() );
			cRequestData.get_ItemB()->set_CorreId( pUdrFormat->get_ItemC()->get_strCorrelationID() );
			cRequestData.get_ItemC()->set_StartT( pUdrFormat->get_LimitInfo()->get_nSessionStartTime() );
			cRequestData.get_ItemC()->set_EndT( pUdrFormat->get_LimitInfo()->get_nSessionEndTime() );
			cRequestData.get_ItemD()->set_LimitT( pUdrFormat->get_LimitInfo()->get_nUsePrice() );
			cRequestData.get_ItemD()->set_NTuseT( pUdrFormat->get_LimitInfo()->get_nUseTime() );
			break;
		// Service Stop Request
		case 2 : 
			cRequestData.get_ItemA2()->set_Key(0x03);
			break;
		// Service End Request
		case 3 :
			cRequestData.get_ItemA2()->set_Key(0x04);
			cRequestData.get_ItemC()->set_StartT( pUdrFormat->get_LimitInfo()->get_nSessionStartTime() );
			cRequestData.get_ItemC()->set_EndT( pUdrFormat->get_LimitInfo()->get_nSessionEndTime() );
			cRequestData.get_ItemD()->set_LimitT( pUdrFormat->get_LimitInfo()->get_nUsePrice() );
			cRequestData.get_ItemD()->set_NTuseT( pUdrFormat->get_LimitInfo()->get_nUseTime() );
			break;
		default :
			// Error Code�� �߰������ ��.
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
				<< "CLimitMan : MakeAndSendRequest!!" << endl;
	}
	
	if( LIMIT_DEBUG )
	{
		*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
			<< "============ Request Data ���� ===========" <<  endl;
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
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "SCP�� �������۽���" << endl;
			return 0;
		}
	}
	else
	{
		if( LIMIT_DEBUG )
		{
			*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "SCP�� �������ۿϷ�" << endl;
		}
	}

	return 1;
}
//===============================================================
// CompareLimitInfo Method		  	 							=
//  : ������ ������ ���Ͽ�, ������ �ϰ��ǰ� �����ϱ� ���� Method.		=
//	 �Ϲ�UDR->�ѵ�����UDR, Packet -> Time, Time -> Packet���  	=
// argument     :  pUdrFormat -> ó���ϱ� ���� ���� UdrFormat	=
//				   pSearchUdrFormat -> ������ MDB�� �ִ� DATA		=
// return value : None											=
//																=
//===============================================================
void CLimitMan::CompareLimitInfo( UdrFormat* pUdrFormat, UdrFormat* pSearchUdrFormat )
{
	int TempServiceType;
	int TempSubsType;
	
	nServiceType = pUdrFormat->get_ItemHead()->get_nServiceType();
	nSubsType = pUdrFormat->get_ItemHead()->get_nSubsType();
	nForwardMUXOpt = pUdrFormat->get_ItemF()->get_nForwardMUXOpt();
	
	// ���� UDR�� �ѵ������� ���� UDR�� �ѵ������� ������ ���Ѵ�.
	// ���� ���� �ʴٸ� ������ ������ �����Ѵ�.
	// START UDR�϶��� ����
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
//===============================================================
// CompareServiceType Method		  	 						=
//  : ���� UDR�� �Ϲ�UDR����, �ѵ������� �ؾ��ϴ��� �Ǵ��ϴ� Method.	=
// argument     :  pUdrFormat -> ó���ϱ� ���� ���� UdrFormat	=
// return value : 0->�Ϲ� UDR									=
//				  1-> Packet��� �ѵ�����							=
//				  2 -> �ð���� �ѵ����� 							=
//===============================================================	
int CLimitMan::CompareServiceType( UdrFormat* pUdrFormat )
{
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
			/*
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
			*/
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
//===============================================================
// RequireLimitInfo Method		  	 							=
//  :SCP I/F MQ�� �ش� ��û�� ������ Method.			       		=
// argument     :  pRequestData -> ��û�ϱ�����  SCPFormatAgg		=
// return value : 1-> success									=
//                0-> fail										=
//																=
//===============================================================
int CLimitMan::RequireLimitInfo( SCPFormatAgg* pRequestData )
{
    // Send request Aggregator - Scp I/F MQ
    while(1)
    {
        if( mqSCP->sendMsg( pRequestData ) == FLAG_MQ_IS_FULL )
        {
        	*AggregatorLogM << AggregatorLogM->ErrorCode(0) 
					<< "Wait  MSG Queue FULL!!" << endl;
			usleep(100);
  //         return 0;
        }
        else break;
    }
    return 1;
}
#endif



