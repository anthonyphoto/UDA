#include "CLimitMGR.hpp"

CLimitMGR	cLimitMgr;

//////////////////////////////////////////
//		Class CLimitMGR's Method		//
//////////////////////////////////////////

//------------------------------------------------------------------------------
// 생성자
CLimitMGR::CLimitMGR()
{
	;
}



//------------------------------------------------------------------------------
// 소멸자
CLimitMGR::~CLimitMGR()
{
	delete	LogM;
	delete	mqSCP;
	delete	mqLMGR;
	delete	mqPMS;
}



//------------------------------------------------------------------------------
// 한도 관리의 전 과정을 관리
int CLimitMGR::Start()
{
	while(1)
	{
		memset((char*)&stUdr, NULL, sizeof(UdrFormat));

		// Filter로 부터 한도 UDR과
		// SCP로 부터 한도 가입자 요금 정보 조회 결과 또는 가입자 요금 중간 정산 결과를 MQ로 부터 받음		
		if(mqLMGR->recvMsg(&stUdr) == FLAG_MQ_IS_EMPTY)
			sleep(2);
		else
		{ 
	   		// SCP로 부터 한도 가입자 요금 정보 조회 결과 또는 가입자 요금 중간 정산 결과일 때
	   		if(stUdr.stItemHead.get_nUdrType() == LIMIT_RESULT 
	   			|| stUdr.stItemHead.get_nUdrType() == ACCESS_RESULT )
	   		{
	   			#ifdef	__DEBUG
				*LogM<<LogM->ErrorCode(0)<< endl;
				*LogM<<LogM->ErrorCode(0)<< endl;
	   			*LogM<< "<< Recv Response from SCP >>"<<LogM->ErrorCode(0)<< endl;
				#endif
			}
			else 	// Filter로 부터 한도 UDR
			{
	   			#ifdef	__DEBUG
	   			*LogM<<LogM->ErrorCode(0)<< endl;
				*LogM<<LogM->ErrorCode(0)<< endl;
	   			*LogM<< "<< Recv Limit UDR from Filter>>"							 <<LogM->ErrorCode(0)<< endl;
	   			*LogM<< "[UDR_TYPE : "<<stUdr.stItemHead.get_nUdrType()			<<"]"<<LogM->ErrorCode(0)<< endl;
	   			*LogM<< "[MIN      : "<<stUdr.stItemA.get_unMsId()				<<"]"<<LogM->ErrorCode(0)<< endl;
	   			*LogM<< "[ACC_ID   : "<<stUdr.stItemC.get_strAccountSessionID()	<<"]"<<LogM->ErrorCode(0)<< endl;
	   			*LogM<< "[COR_ID   : "<<stUdr.stItemC.get_strCorrelationID()	<<"]"<<LogM->ErrorCode(0)<< endl;
	   			*LogM<< "[F_M_O    : "<<stUdr.stItemF.get_nForwardMUXOpt()		<<"]"<<LogM->ErrorCode(0)<< endl;
				#endif
			}
			
			switch(stUdr.stItemHead.get_nUdrType())
			{
				case	UDR_START:
						if(ProcStart() < 0)
							return -1;
						break;

				case	UDR_INTRIM:
						if(ProcIntrim() < 0)
							return -1;
						break;

				case	UDR_STOP:
						if(ProcStop() < 0)
							return -1;
						break;

				case	LIMIT_RESULT:
				case	ACCESS_RESULT:
						if(DoResProcess() < 0)
							return -1;
						break;
				default:
						#ifdef	__DEBUG
	   					*LogM << LogM->ErrorCode(9517)	<< "undefined udr ....["<<stUdr.stItemHead.get_nUdrType()
	   						<< "]" << endl;
						#endif
						break;
			}//switch end
   		}// else end
	}// while end
}



//------------------------------------------------------------------------------
// START UDR이 들어왔을 때 처리
int CLimitMGR::ProcStart()
{
	int	nRet = 0;
	// [t_threshold_amt] table class에 set
	ILimitTbl.SetMin		(stUdr.stItemA.get_unMsId				());
	ILimitTbl.SetAccSId	(stUdr.stItemC.get_strAccountSessionID	());
	ILimitTbl.SetCorrId	(stUdr.stItemC.get_strCorrelationID		());

	// [t_threshold_amt] table의 한도 정보가 존재하지 않으면 SCP로 한도 정보 조회 요구한다.
	nRet = ILimitTbl.GetLimitInfo();
	if(nRet == 1403)
	{
		// SCP로 보낼 TTL 가입자 요금 정보 조회 요구 메세지를 만든다
		memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
		stAccReqMsg.stItemA2.set_Key		(0x01);								// 구분코드 
		stAccReqMsg.stItemA2.set_Min		(stUdr.stItemA.get_unMsId() );		// Min
		stAccReqMsg.stItemB.set_OperId		(0);								// 메시지구별 키 
		stAccReqMsg.stItemB.set_AccSessionId(stUdr.stItemC.get_strAccountSessionID());
		stAccReqMsg.stItemB.set_CorreId		(stUdr.stItemC.get_strCorrelationID());

		SndReqToSCP();

		#ifdef	__DEBUG
		*LogM << LogM->ErrorCode(0) << "###-한도정보 조회 요청 정보" <<  endl;
		*LogM << LogM->ErrorCode(0) << "-Key         =" << stAccReqMsg.get_ItemA2()->get_Key()			<<endl;
		*LogM << LogM->ErrorCode(0) << "-unMsId      =" << stAccReqMsg.get_ItemA2()->get_Min()			<<endl;
		*LogM << LogM->ErrorCode(0) << "-OperId      =" << stAccReqMsg.get_ItemB()->get_OperId()		<<endl;
		*LogM << LogM->ErrorCode(0) << "-AccSessionId=" << stAccReqMsg.get_ItemB()->get_AccSessionId()	<<endl;
		*LogM << LogM->ErrorCode(0) << "-CorreId     =" << stAccReqMsg.get_ItemB()->get_CorreId()		<<endl;
		#endif
	}	
	else if((nRet == 0) || (nRet == NOT_EXIST_SESSION)) 
	{
		#ifdef	__DEBUG
		*LogM << LogM->ErrorCode(0)	
		<< "-기존한도정보 존재 => 한도금액 :["<<ILimitTbl.GetLimitAmt()<<"]"<<  endl;
		#endif
	}
	else if(nRet == -1)
		return -1;
	
	return 0;	
}



//------------------------------------------------------------------------------
// INTERIM UDR이 들어왔을 때 처리
int CLimitMGR::ProcIntrim()
{
	int		nRet			= 0;
	int		nRateDataAmt	= 0;						// 현재 사용한 패킷량
	int		nRemain			= 0;						// 계산하고 남은 짜투리 패킷
	int		nPreviousRateAmt= 0;						// 기존 정산 금액
	int		nPreRemainAmt	= 0;						// 기존 짜투리 패킷
	int		nPreLimitAmt	= 0;						// 기존 한도 금액
	int		nSpeedFlag		= 0;						// '1':저속, '2':고속
		
	nRatingPrice			= 0;						// 현재 사용한 금액
		
	ILimitTbl.SetMin(stUdr.stItemA.get_unMsId());
	ILimitTbl.SetAccSId(stUdr.stItemC.get_strAccountSessionID());
	ILimitTbl.SetCorrId(stUdr.stItemC.get_strCorrelationID());

	// [t_threshold_amt], [t_threshold_mid_amt]에서 정보를 가져온다
	nRet = ILimitTbl.GetLimitInfo();
	
	nPreviousRateAmt = ILimitTbl.GetRateAmt	();		
	nPreLimitAmt	 = ILimitTbl.GetLimitAmt();
	nPreRemainAmt	 = ILimitTbl.GetRemainAmt();
				
	if(nRet == 1403)									// MIN별 한도정보가 존재하지 않는다
	{
		#ifdef	__DEBUG
		*LogM << LogM->ErrorCode(0)<<"-Start Not Exist...Send Access Request!" <<  endl;
		#endif

		if(ProcStart() < 0)								//Start 없는 Intrim udr을 recv했을 때
			return -1;

		return 0; 
	} 	
	else if(nRet == -1)
		return -1;

	#ifdef	__DEBUG
	*LogM << LogM->ErrorCode(0) << "<<과금 패킷량>>"	<< endl;
	*LogM << LogM->ErrorCode(0) << " 과거 패킷량	: "	<< ILimitTbl.GetLastDataOct() << endl;
	*LogM << LogM->ErrorCode(0) << " 현재 패킷량	: "	
			<< stUdr.stItemG.get_nDataOctetCntTerminat() + stUdr.stItemG.get_nDataOctetCntOrigin() <<  endl;
	#endif

	// 과금할 패킷량 계산: 현재들어온 누적패킷량 - 이전까지의 누적량
	nRateDataAmt = (stUdr.stItemG.get_nDataOctetCntTerminat() + stUdr.stItemG.get_nDataOctetCntOrigin())
					- ILimitTbl.GetLastDataOct();
					
	if( nRateDataAmt <= 0 )
	{
		#ifdef	__DEBUG
		*LogM << LogM->ErrorCode(0)<< "Small Rating Data Octet!!" << endl;
		#endif

		return 0;
	}
	
	// 저속(1), 고속(2337) 확인 
	if(stUdr.stItemF.get_nForwardMUXOpt() == 1)
		nSpeedFlag = 1;
	else
		nSpeedFlag = 2;
			
	// (현재 패킷량 + 이전 계산하고 남은 짜투리 패킷량)으로 사용금액을 계산.
	nRatingPrice = stRate->GetRate(nSpeedFlag, 0, 0, 0, nRateDataAmt + ILimitTbl.GetRemainAmt());
	
	// FMO :후에 rating class수정되면 바꿀 것 
	// nRatingPrice = stRate->GetRate(2, 0, 0, 0, nRateDataAmt, stUdr.stItemF.get_nForwardMUXOpt());

	nRemain = stRate->GetLimitSec();					// 계산하고 남은 짜투리 패킷

	//총 사용금액 = 조회된 이전까지의 토달 금액 + 현재 사용 금액 
	ILimitTbl.SetTotalAmt(ILimitTbl.GetTotalAmt() + nRatingPrice);
	ILimitTbl.SetRemainAmt(nRemain);			

	#ifdef	__DEBUG
	*LogM << LogM->ErrorCode(0) << "###한도처리결과" 	<<  endl;
	*LogM << LogM->ErrorCode(0) << "기존 한도 금액 : " 	<< ILimitTbl.GetLimitAmt() 	<<  endl;
	*LogM << LogM->ErrorCode(0) << "사용 DataOctet : " 	<< nRateDataAmt				<<  endl;
	*LogM << LogM->ErrorCode(0) << "현 사용금액    : " 	<< nRatingPrice				<<  endl;
	*LogM << LogM->ErrorCode(0) << "총사용금액     : " 	<< ILimitTbl.GetTotalAmt()	<<  endl;
	*LogM << LogM->ErrorCode(0) << "과금후 나머지 DataOctet : " << nRemain <<  endl;
	*LogM << LogM->ErrorCode(0) << "남은 한도 금액 : " 
			<< ILimitTbl.GetLimitAmt() - (ILimitTbl.GetTotalAmt() - nPreviousRateAmt)<<  endl;
	#endif

	ILimitTbl.SetIntrimCnt(ILimitTbl.GetIntrimCnt() + 1);	// Intrim UDR Count 증가

	if(InterimAccountReq(nPreviousRateAmt) < 0)			// 중간 정산 요청
		return -1;
	
	if(LimitMGR(nPreLimitAmt, nPreviousRateAmt) < 0)	// 한도 초과 확인
		return -1;
	
	ILimitTbl.Commit();		
	return 0;
}



//------------------------------------------------------------------------------
// STOP UDR이 들어왔을 때 처리
int CLimitMGR::ProcStop()
{
	int		nRet			= 0;
	int		nRateDataAmt	= 0;				// 현재 사용한 패킷량
	int		nRemain			= 0;				// 계산하고 남은 짜투리 패킷
	int		nPreviousRateAmt= 0;				// 기존 정산 금액
	int		nPreRemainAmt	= 0;				// 기존 짜투리 패킷
	int		nSessionCnt		= 0;
	int		nPreLimitAmt	= 0;				// 기존 한도 금액
	int		nSpeedFlag		= 0;				// '1':저속, '2':고속
		
	nRatingPrice			= 0;				// 현재 사용한 금액
	
	ILimitTbl.SetMin(stUdr.stItemA.get_unMsId());
	ILimitTbl.SetAccSId(stUdr.stItemC.get_strAccountSessionID());
	ILimitTbl.SetCorrId(stUdr.stItemC.get_strCorrelationID());

	// [t_threshold_amt], [t_threshold_mid_amt]에서 정보를 가져온다
	nRet = ILimitTbl.GetLimitInfo();

	if(nRet == -1)
		return -1;						

	nPreviousRateAmt = ILimitTbl.GetRateAmt();
	nPreLimitAmt	 = ILimitTbl.GetLimitAmt();
	nPreRemainAmt	 = ILimitTbl.GetRemainAmt();
		
	#ifdef	__DEBUG
	*LogM << LogM->ErrorCode(0) << "<<과금 패킷량>>"	<< endl;
	*LogM << LogM->ErrorCode(0) << " 과거 패킷량	: "	<< ILimitTbl.GetLastDataOct() << endl;
	*LogM << LogM->ErrorCode(0) << " 현재 패킷량	: "	
			<< stUdr.stItemG.get_nDataOctetCntTerminat() + stUdr.stItemG.get_nDataOctetCntOrigin() <<  endl;
	#endif

	// 과금할 패킷량 계산: 현재들어온 누적패킷량 - 이전까지의 누적량
	nRateDataAmt = (stUdr.stItemG.get_nDataOctetCntTerminat() + stUdr.stItemG.get_nDataOctetCntOrigin())
					- ILimitTbl.GetLastDataOct();

	if( nRateDataAmt <= 0 )
	{
		#ifdef	__DEBUG
		*LogM << LogM->ErrorCode(0)<< "Small Rating Data Octet!!" << endl;
		#endif

		return 0;
	}

	// 저속(1), 고속(2337) 확인 
	if(stUdr.stItemF.get_nForwardMUXOpt() == 1)
		nSpeedFlag = 1;
	else
		nSpeedFlag = 2;

	// (현재 패킷량 + 이전 계산하고 남은 짜투리 패킷량)으로 사용금액을 계산.
	nRatingPrice = stRate->GetRate(nSpeedFlag, 0, 0, 1, nRateDataAmt + ILimitTbl.GetRemainAmt());

	// FMO :후에 rating class수정되면 바꿀 것 
	//nRatingPrice = stRate->GetRate(2, 0, 0, 0, nRateDataAmt, stUdr.stItemF.get_nForwardMUXOpt());

	// 계산하고 남은 짜투리 패킷
	nRemain = stRate->GetLimitSec();

	//총 사용금액 = 조회된 이전까지의 토달 금액 + 현재 사용 금액 
	ILimitTbl.SetTotalAmt(ILimitTbl.GetTotalAmt() + nRatingPrice);
	ILimitTbl.SetRemainAmt(nRemain);
	
	#ifdef	__DEBUG
	*LogM << LogM->ErrorCode(0) << "-----한도처리결과-----"				 			<<  endl;
	*LogM << LogM->ErrorCode(0) << "-기존 한도 금액 : " 	<< ILimitTbl.GetLimitAmt() 	<<  endl;
	*LogM << LogM->ErrorCode(0) << "-사용 DataOctet : " 	<< nRateDataAmt				<<  endl;
	*LogM << LogM->ErrorCode(0) << "-현 사용금액    : " 	<< nRatingPrice				<<  endl;
	*LogM << LogM->ErrorCode(0) << "-총사용금액     : " 	<< ILimitTbl.GetTotalAmt()	<<  endl;
	*LogM << LogM->ErrorCode(0) << "-과금후 나머지 DataOctet : " << nRemain <<  endl;
	*LogM << LogM->ErrorCode(0) << "-남은 한도 금액 : " 
			<< ILimitTbl.GetLimitAmt() - (ILimitTbl.GetTotalAmt() - nPreviousRateAmt)<<  endl;
	#endif
	
	ILimitTbl.SetEventTime(stUdr.stItemG.get_nEndEventTime());

	// 자기 MIN과 같고, 세션 id 또는  collation id가 다른 [t_threshold_mid_amt] 레코드의 개수 
	nSessionCnt = ILimitTbl.SelectAmtTbl();

	if((nRet == 1403) || (nRet == NOT_EXIST_SESSION))
	{
		// [t_threshold_amt], [t_threshold_mid_amt] table에 데이타 존재하지 않음
		if(nRet == 1403)
		{
			*LogM << LogM->ErrorCode(0)<<"-Send End Request!" <<  endl;
			SndServiceEndReq();
			return 0;
		}	
		else// 해당 세션이 [t_threshold_mid_amt] table에 존재하지 않음
		{
			// 자기 MIN과 같고, 세션 id 또는  collation id가 다른 [t_threshold_mid_amt] 레코드가 존재하지 않음
			if(nSessionCnt == 0)
			{
				*LogM << LogM->ErrorCode(0)<<"-Send End Request!" <<  endl;
				SndServiceEndReq();
				*LogM << LogM->ErrorCode(0)<<"-한도관리 테이블 삭제!" <<  endl;
				if(ILimitTbl.DeleteAmtTbl() < 0)
					return -1;

				ILimitTbl.Commit();		
				return 0;
			}
			else// 자기 MIN과 같고, 세션 id 또는  collation id가 다른 [t_threshold_mid_amt] 레코드가 존재함
			{
				*LogM << LogM->ErrorCode(0)<<"-중간 정산!" <<  endl;
				if(InterimAccountReq(nPreviousRateAmt, 1) < 0)
					return -1;

				*LogM << LogM->ErrorCode(0)<<"-한도관리!" <<  endl;
				if(LimitMGR(nPreLimitAmt, nPreviousRateAmt) < 0)
					return -1;

				ILimitTbl.Commit();		
				return 0;
			}
		}
	}

	// 해당 세션이 [t_threshold_mid_amt]에 존재하고, 
	// 자기 MIN과 같고, 세션 id 또는  collation id가 다른 [t_threshold_mid_amt] 레코드가 존재하지 않음
	if(nSessionCnt == 0)
	{
		*LogM << LogM->ErrorCode(0)<<"-Send End Request!" <<  endl;
		SndServiceEndReq();

		*LogM << LogM->ErrorCode(0)<<"-세션 테이블 삭제!" <<  endl;
		//해당 번호, Account session id, Collation id에 대한 한도 정보를 삭제한다
		if(ILimitTbl.DeleteMidAmtTbl() < 0)
			return -1;

		*LogM << LogM->ErrorCode(0)<<"-한도관리 테이블 삭제!" <<  endl;
		if(ILimitTbl.DeleteAmtTbl() < 0)
			return -1;
	}
	else
	{
		// 해당 세션이 [t_threshold_mid_amt]에 존재하고, 
		// 자기 MIN과 같고, 세션 id 또는  collation id가 다른 [t_threshold_mid_amt] 레코드가 존재하지 않음
		*LogM << LogM->ErrorCode(0)<<"-중간정산!" <<  endl;
		if(InterimAccountReq(nPreviousRateAmt, 1) < 0)
			return -1;
		
		*LogM << LogM->ErrorCode(0)<<"-세션 테이블 삭제!" <<  endl;
		if(ILimitTbl.DeleteMidAmtTbl() < 0)
			return -1;
		
		*LogM << LogM->ErrorCode(0)<<"-한도관리!" <<  endl;
		if(LimitMGR(nPreLimitAmt, nPreviousRateAmt) < 0)
			return -1;

		if(ILimitTbl.UpdateRateAmt() < 0)
			return -1;
	}	

	ILimitTbl.Commit();		
	return 0;
}



//------------------------------------------------------------------------------
// 한도관리 관련 데이블을 update한 후, SCP로 보낼 중간 정산 요청 메세지를 만들고, SCP MQ로 메세지를 보냄
int CLimitMGR::InterimAccountReq(int nPreviousRate, int nStopFlag)
{
	// [t_threshold_amt], [t_threshold_mid_amt] table update
	// Event time
	ILimitTbl.SetEventTime(stUdr.stItemG.get_nEndEventTime()); 

	// 해당 세션이 현재까지 사용한 총 금액
	ILimitTbl.SetCurrentAmt(nRatingPrice + ILimitTbl.GetCurrentAmt());
	// 현재들어온 누적패킷량
	ILimitTbl.SetLastDataOct(stUdr.stItemG.get_nDataOctetCntTerminat()
							+ stUdr.stItemG.get_nDataOctetCntOrigin());
	if( ILimitTbl.UpdateLimitInfo1() < 0)
		return -1;


	// SCP 중간정산보내기 (nStopFlag-'0':INTERIM UDR, '1':STOP UDR)
	if(((ILimitTbl.GetIntrimCnt() % ACCESS_REQ_CNT) == 0) || (nStopFlag == 1))
	{
		// 총 정산 금액
		ILimitTbl.SetRateAmt(ILimitTbl.GetTotalAmt());
		// 정산 후 한도 금액
		ILimitTbl.SetLimitAmt( ILimitTbl.GetLimitAmt() - (ILimitTbl.GetTotalAmt()-nPreviousRate) );

		// [t_threshold_amt] table의 한도 금액, 정산 금액 update
		if(ILimitTbl.UpdateLimitNRateAmt() < 0)
			return -1;
		
		memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
		stAccReqMsg.get_ItemA2()->set_Key(0x02);
		stAccReqMsg.get_ItemA2()->set_Min(stUdr.stItemA.get_unMsId() );	// Min
		stAccReqMsg.get_ItemB()->set_AccSessionId( stUdr.stItemC.get_strAccountSessionID() );
		stAccReqMsg.get_ItemB()->set_CorreId( stUdr.stItemC.get_strCorrelationID() );
		stAccReqMsg.get_ItemC()->set_StartT(0);
		stAccReqMsg.get_ItemC()->set_EndT(0);
		// 정산금액 = session별 총사용액 - 기존 정산금액
		stAccReqMsg.get_ItemD()->set_LimitT(ILimitTbl.GetTotalAmt() - nPreviousRate);
		stAccReqMsg.get_ItemD()->set_NTuseT(0);
		ILimitTbl.SetOId(ILimitTbl.GetOId() + 1); // scp송신 때마다 +1 증가해서 보낸다
		stAccReqMsg.get_ItemB()->set_OperId(ILimitTbl.GetOId());//증가된 OID를 select해서 넣어야한다
		
		// Operation ID update
		if(ILimitTbl.UpdateOId() < 0)
			return -1;
		
		#ifdef	__DEBUG
		*LogM << LogM->ErrorCode(0)<< "<<보낸 중간정산메시지>>" << endl;
		*LogM << LogM->ErrorCode(0)<< " 번호      : " << stAccReqMsg.get_ItemA2()->get_Min()  << endl;
		*LogM << LogM->ErrorCode(0)<< " 정산금액  : " << stAccReqMsg.get_ItemD()->get_LimitT()<< endl;
		#endif

		SndReqToSCP();
		
		if(ILimitTbl.UpdateInterimCnt() < 0)				// Interim count update
			return -1;
	}	
	
	return 0;
}



//------------------------------------------------------------------------------
// 총사용 금액이 한도 초과했는지 확인한 후,
// 초과했다면 SCP로 보낼 Service stop request 메세지를 만들고, SCP MQ로 메세지를 보냄
int CLimitMGR::LimitMGR(int nPreLimitAmt, int nPreviousRate)
{
	// 한도 초과 확인 : 기존 한도금액 - (세션별 현재 총 사용금액 - 기존 정산 금액)
	if( (nPreLimitAmt - (ILimitTbl.GetTotalAmt() - nPreviousRate) ) <= 0 )
	{
		#ifdef	__DEBUG
			*LogM << LogM->ErrorCode(0) << "한도금액 초과 !!" << endl;
			*LogM << LogM->ErrorCode(0) << "기존 한도금액:["  << nPreLimitAmt			<< "]" << endl;
			*LogM << LogM->ErrorCode(0) << "총사용금액	 :["  << ILimitTbl.GetTotalAmt() << "]" << endl;
			*LogM << LogM->ErrorCode(0) << "기존정산금액 :["  << nPreviousRate			<< "]" << endl;
		#endif
		
		// Service stop request 메세지 만든 후, SCP로 보냄
		memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
		stAccReqMsg.stItemA2.set_Key(0x03);
		stAccReqMsg.stItemA2.set_Min(stUdr.stItemA.get_unMsId() );
		
		SndReqToSCP();

		// Operation ID update
		ILimitTbl.SetOId(ILimitTbl.GetOId() + 1);			// scp송신 때마다 Operation id 1증가
		if(ILimitTbl.UpdateOId() < 0)
			return -1;

/*
		// [t_threshold_amt] table의 한도 금액, 정산 금액 update
		// 총 정산 금액
		ILimitTbl.SetRateAmt(ILimitTbl.GetTotalAmt());
		// 정산 후 한도 금액
		ILimitTbl.SetLimitAmt( nPreLimitAmt - (ILimitTbl.GetTotalAmt() - nPreviousRate) );
		if(ILimitTbl.UpdateLimitNRateAmt() < 0)
			return -1;
*/
	}

	return 0;
}	



//------------------------------------------------------------------------------
// STOP UDR이 들어 왔을 때 SCP로 보낼 Service end request 메세지를 만들고, SCP MQ로 메세지를 보냄
void CLimitMGR::SndServiceEndReq()
{
	memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
	stAccReqMsg.get_ItemA2()->set_Key(0x04);
	stAccReqMsg.get_ItemA2()->set_Min(stUdr.stItemA.get_unMsId() );	// Min
	stAccReqMsg.get_ItemC()->set_StartT(0);
	stAccReqMsg.get_ItemC()->set_EndT(0);
	// 정산금액 = session별 총사용액 - 기존 정산금액
	stAccReqMsg.get_ItemD()->set_LimitT(ILimitTbl.GetTotalAmt() - ILimitTbl.GetRateAmt()); //계산된 금액
	stAccReqMsg.get_ItemD()->set_NTuseT(0);
	ILimitTbl.SetOId(ILimitTbl.GetOId() + 1); // scp송신 때마다 +1 증가해서 보낸다
	stAccReqMsg.get_ItemB()->set_OperId(ILimitTbl.GetOId());//증가된 OID를 select해서 넣어야한다
	
	SndReqToSCP();
		
	#ifdef	__DEBUG
	*LogM << LogM->ErrorCode(0)<< "<<Service end req>>" << endl;
	*LogM << LogM->ErrorCode(0)<< " 번호      : "<<stAccReqMsg.get_ItemA2()->get_Min()<< endl;
	*LogM << LogM->ErrorCode(0)<< " 정산금액  : "<<stAccReqMsg.get_ItemD()->get_LimitT()<<  endl;
	#endif
}	



//------------------------------------------------------------------------------
// SCP MQ로 메세지를 보냄
void CLimitMGR::SndReqToSCP()
{
	// Send request Aggregator - Scp I/F MQ
    while(1)
    {
        if( mqSCP->sendMsg( &stAccReqMsg ) == FLAG_MQ_IS_FULL )
        {
	   		*LogM << LogM->ErrorCode(9518) << "Limit Manager -> SCP MQ is full..." << endl;

			usleep(100000);
        }
        else
        {
        	#ifdef	__DEBUG
	   		*LogM<< "Send Success to SCP..."<<LogM->ErrorCode(0)<< endl;
			#endif
        	break;
        }//else
    }//while
}



//------------------------------------------------------------------------------
// SCP로부터 받은 한도요금 조회 결과, 중간정산 결과처리
// 최종 OID의 한도금액을 UPDATE한다 
int CLimitMGR::DoResProcess()
{
	ILimitTbl.SetMin(stUdr.stItemA.get_unMsId());
	ILimitTbl.SetOId(stUdr.stLimitInfo.get_nOperationID());
	ILimitTbl.SetAccSId(stUdr.stItemC.get_strAccountSessionID());
	ILimitTbl.SetCorrId(stUdr.stItemC.get_strCorrelationID());
	ILimitTbl.SetLimitAmt(stUdr.stLimitInfo.get_nLimitPrice());

	if(stUdr.stItemHead.get_nUdrType() == 0x81) // 한도요금조회 결과
	{
		#ifdef	__DEBUG
	   		*LogM<< "RECV..한도정보 조회결과"<<LogM->ErrorCode(0)<< endl;
		#endif
	}
	else if(stUdr.stItemHead.get_nUdrType() == 0x82) // 중간정산 결과
	{
		#ifdef	__DEBUG
	   		*LogM<< "RECV..중간정산 결과"<<LogM->ErrorCode(0)<< endl;
		#endif
	}

	#ifdef	__DEBUG
	*LogM<<"-UDR TYPE: "<<stUdr.stItemHead.get_nUdrType()<<LogM->ErrorCode(0)<< endl;
	*LogM<<"-MIN		: "<<stUdr.stItemA.get_unMsId()<<LogM->ErrorCode(0)<< endl;
	*LogM<<"-ACC_ID	: "<<stUdr.stItemC.get_strAccountSessionID()<<LogM->ErrorCode(0)<< endl;
	*LogM<<"-CORR_ID	: "<<stUdr.stItemC.get_strCorrelationID()<<LogM->ErrorCode(0)<< endl;
	*LogM<<"-OP_ID	: "<<stUdr.stLimitInfo.get_nOperationID()<<LogM->ErrorCode(0)<< endl;
	*LogM<<"-한도금액: "<<stUdr.stLimitInfo.get_nLimitPrice()<<LogM->ErrorCode(0)<< endl;
	#endif
	
	if(stUdr.stLimitInfo.get_nLimitPrice() <= 0)
	{
		#ifdef	__DEBUG
   		*LogM<< "한도금액이 없음...."<<LogM->ErrorCode(0)<< endl;
		#endif

		memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
		stAccReqMsg.get_ItemA2()->set_Key(0x03);
		stAccReqMsg.get_ItemA2()->set_Min(stUdr.stItemA.get_unMsId() );	// Min

		SndReqToSCP();
	}
	
	// 한도 금액 update
	if(stUdr.stLimitInfo.get_nOperationID()  >= ILimitTbl.SelectOId())
	{
		if(ILimitTbl.UpdateLimitAmt() < 0)
			return -1;
		else
			ILimitTbl.Commit();	
	}
		
	return 0;
}



//------------------------------------------------------------------------------
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
			*LogM<<"PmpMQ IS FULL.. sleep(1)!"<<LogM->ErrorCode(2005)<< endl;
			sleep(1);
			if (i >= 10)
			{
				kill(0,SIGINT);
			}
		}else break;
	}
}



//------------------------------------------------------------------------------
void sigCapture(int sigNo)
{
	LogM->put_EventHistData(FLAG_END,0);
	*LogM<<"=====STOP LIMIT MANAGER====="<<LogM->ErrorCode(0)<<endl;

	cLimitMgr.IDbConnect.disconnect();
	delete stRate;

	switch(sigNo)
	{
		case SIGINT :
		case SIGQUIT :
		case SIGKILL :
		case SIGTERM :
				WritePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				exit(-1);
				break;
		case SIGUSR1:
				WritePMSMQ(FLAG_PROC_TERM);//0//프로세스정상stop (Client업무명령)
				exit(0);
				break;
		default:
				WritePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				exit(-1);
				break;
	}
}


//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	if(argc != 7 )
    {
    	cout <<"Usage: LimitMGR [sysId][svcId][gId][pId][pSn][pRemark]"<<endl;
    	exit(-1);
    }
    
    SystemId	= atoi(argv[1]);   
	ServiceId	= atoi(argv[2]);  
	GroupId		= atoi(argv[3]);    
	ProcessType	= atoi(argv[4]);
	PserialNo	= atoi(argv[5]);  
	PreMark		= atoi(argv[6]);    
   
	signal(SIGUSR1,sigCapture); 	signal(SIGTERM,sigCapture);
	signal(SIGTERM,sigCapture);
	signal(SIGINT,sigCapture); 	signal(SIGSTOP,sigCapture);
	signal(SIGQUIT,sigCapture);	signal(SIGTSTP,sigCapture);
	signal(SIGKILL,sigCapture);	signal(SIGCONT,sigCapture);	//26
	
	LogM		= new SendLogManage(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),\
								 atoi(argv[4]),atoi(argv[5]),atoi(argv[6]));
	mqPMS		= new MessageQueue<MsgPmsStatus>(atoi(argv[1]), 0,1,1,0,10, 3000);
	mqSCP		= new MessageQueue<SCPFormatAgg>(atoi(argv[1]), ServiceId,4,1,0,10, 3000);
	mqLMGR		= new MessageQueue<UdrFormat>(atoi(argv[1]), ServiceId,4,4,0,10, 3000);
	
	*LogM<<"=====START LIMIT MANAGER====="<<LogM->ErrorCode(0)<<endl;
	LogM->put_EventHistData(FLAG_START,FLAG_PROC_START);
	WritePMSMQ(FLAG_PROC_START);
    
    // DB connect
	if(cLimitMgr.IDbConnect.connect() != 0)
	{
	   	*LogM<<"Fail DB connect"<<LogM->ErrorCode(0)<<endl;
	    exit(-1);
	}			

	stRate = new RateRule();			// 주어진 패킷량에 따라 금액을 계산하기 위한 인스턴스
	
	if(cLimitMgr.Start() < 0)
	{
		cLimitMgr.IDbConnect.disconnect();
		delete stRate;
	    exit(-1);
	}

	cLimitMgr.IDbConnect.disconnect();
    delete stRate;
    return 0;
}
