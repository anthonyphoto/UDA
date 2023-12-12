#include "CLimitMGR.hpp"


CLimitMGR::CLimitMGR(int nSvcId)
{
/*	// 몇번만에 한도정보중간정산을 해야하는지 읽어온다
	char strFileName[100];
	
	sprintf(strFileName, "/SVC%d/DATA/AGG_AccountCnt.cfg",nSvcId);
	pAccReqCnt = new AccountCnt(PATH_NAME_ACCOUNT_COUNT);
	pAccReqCnt->read_AccountCnt();
	gnAccReqCnt = pAccountCnt->getAccountCnt();
*/	
	gnAccReqCnt = 2;

	stRate = new RateRule();
	
}

CLimitMGR::~CLimitMGR()
{
//	delete	pAccReqCnt;
	delete	LogM;
	delete	mqSCP;
	delete	mqLMGR;
	delete	mqPMS;
	

}

void CLimitMGR::Start()
{
	while(1)
	{
		memset((char*)&stUdr, NULL, sizeof(UdrFormat));
		memset((char*)&stUdr, NULL, sizeof(UdrFormat));
		
		if(mqLMGR->recvMsg(&stUdr) == FLAG_MQ_IS_EMPTY)
			sleep(2);
		else
		{ 
	   		if(stUdr.stItemHead.get_nUdrType()== LIMIT_RESULT 
	   			|| stUdr.stItemHead.get_nUdrType()== ACCESS_RESULT )
	   		{
	   			#ifdef	_DEBUG
				*LogM<<LogM->ErrorCode(0)<< endl;
				*LogM<<LogM->ErrorCode(0)<< endl;
	   			*LogM<< "<< Recv Response from SCP >>"<<LogM->ErrorCode(0)<< endl;
				#endif
			}
			else 
			{
	   			#ifdef	_DEBUG
	   			*LogM<<LogM->ErrorCode(0)<< endl;
				*LogM<<LogM->ErrorCode(0)<< endl;
	   			*LogM<< "<< Recv Limit UDR from Filter>>"<<LogM->ErrorCode(0)<< endl;
	   			*LogM<< "[UDR_TYPE : "<<stUdr.stItemHead.get_nUdrType()<<"]"<<LogM->ErrorCode(0)<< endl;
	   			*LogM<< "[MIN      : "<<stUdr.stItemA.get_unMsId()<<"]"<<LogM->ErrorCode(0)<< endl;
	   			*LogM<< "[ACC_ID   : "<<stUdr.stItemC.get_strAccountSessionID()<<"]"<<LogM->ErrorCode(0)<< endl;
	   			*LogM<< "[COR_ID   : "<<stUdr.stItemC.get_strCorrelationID()<<"]"<<LogM->ErrorCode(0)<< endl;
	   			*LogM<< "[F_M_O    : "<<stUdr.stItemF.get_nForwardMUXOpt()<<"]"<<LogM->ErrorCode(0)<< endl;
				#endif
			}
			
			switch(stUdr.stItemHead.get_nUdrType())
			{
				case	UDR_START:
						ProcStart();	break;
				case	UDR_INTRIM:
						ProcIntrim();	break;
				case	UDR_STOP:
						ProcStop();		break;
				case	LIMIT_RESULT:
				case	ACCESS_RESULT:
						
						DoResProcess();
						
						break;
				default:
						#ifdef	_DEBUG
	   					*LogM<< "undefined udr ....["<<stUdr.stItemHead.get_nUdrType()
	   						<<"]"<<LogM->ErrorCode(0)<< endl;
						#endif
						break;
			}
				
   		}// else
	}// while
}

void CLimitMGR::ProcStart()
{
	// 한도정보요청 메시지를 만든다
	memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
	stAccReqMsg.stItemA2.set_Key(0x01);								// 구분코드 
	stAccReqMsg.stItemA2.set_Min(stUdr.stItemA.get_unMsId() );	// Min
	stAccReqMsg.stItemB.set_OperId(0);								// 메시지구별 키 
	stAccReqMsg.stItemB.set_AccSessionId(stUdr.stItemC.get_strAccountSessionID());
	stAccReqMsg.stItemB.set_CorreId(stUdr.stItemC.get_strCorrelationID());
	
	stTblMgr.SetMin(stUdr.stItemA.get_unMsId());
	stTblMgr.SetAccSId(stUdr.stItemC.get_strAccountSessionID());
	stTblMgr.SetCorrId(stUdr.stItemC.get_strCorrelationID());
	if( stTblMgr.GetLimitInfo() == 1403)
		SndReqToSCP();
	else 
	{
		*LogM << LogM->ErrorCode(0)	
		<< "기존한도정보 존재 => 한도금액 :["<<stTblMgr.GetLimitAmt()<<"]"<<  endl;
		return;
	}
	#ifdef	_DEBUG
	*LogM << LogM->ErrorCode(0) 
		<< "===== Access Request Data 정보 ====" <<  endl;
	*LogM << LogM->ErrorCode(0) 
		<< "Key=" << stAccReqMsg.get_ItemA2()->get_Key() <<endl;
	*LogM << LogM->ErrorCode(0) 
		<< "unMsId=" << stAccReqMsg.get_ItemA2()->get_Min() <<endl;
	*LogM << LogM->ErrorCode(0) 
		<< "OperId=" << stAccReqMsg.get_ItemB()->get_OperId() <<endl;
	*LogM << LogM->ErrorCode(0) 
		<< "AccSessionId=" << stAccReqMsg.get_ItemB()->get_AccSessionId() <<endl;
	*LogM << LogM->ErrorCode(0) 
		<< "CorreId=" << stAccReqMsg.get_ItemB()->get_CorreId() <<endl;
	#endif
}

void CLimitMGR::ProcIntrim()
{
	int		nRateDataAmt=0;
	int		nRatingPrice=0, nRemain=0;
	int		nPreviousRateAmt;
	
	// 두개의 table에서 정산에 필요한 정보를 읽어온다. 
	// 한도잔액,최종패킷사용량, 사용금액, 남은 금액, operation ID, nIntrimCnt
	// 조회전에 전화번호, Account Session ID, Correlation ID를 stTblMgr class에 set한다
	stTblMgr.SetMin(stUdr.stItemA.get_unMsId());
	stTblMgr.SetAccSId(stUdr.stItemC.get_strAccountSessionID());
	stTblMgr.SetCorrId(stUdr.stItemC.get_strCorrelationID());
	int nRet = stTblMgr.GetLimitInfo();
	
	// 기존정산금액
	nPreviousRateAmt = stTblMgr.GetRateAmt();
				
	if( nRet == 1403)// session에 대한 정보가 존재하지 않는다
	{
		#ifdef	_DEBUG
		*LogM << LogM->ErrorCode(0)<<"Start Not Exist...Send Access Request!" <<  endl;
		#endif
		ProcStart();//Start 없는 Intrim udr을 recv했을 때
		return; 
	} 	
	// 과금할 패킷량 계산: 현재들어온 누적패킷량 - 이전까지의 누적량
	#ifdef	_DEBUG
	*LogM << LogM->ErrorCode(0)<< "<<과금 패킷량>>" << endl;
	*LogM << LogM->ErrorCode(0)<< " 과거 패킷량: "<<stTblMgr.GetLastDataOct()<< endl;
	*LogM << LogM->ErrorCode(0)<< " 현재 패킷량: "<<stUdr.stItemG.get_nDataOctetCntTerminat() + stUdr.stItemG.get_nDataOctetCntOrigin()<<  endl;
	#endif
	nRateDataAmt = (stUdr.stItemG.get_nDataOctetCntTerminat() + stUdr.stItemG.get_nDataOctetCntOrigin())
					- stTblMgr.GetLastDataOct();
	if( nRateDataAmt <= 0 )
	{
			#ifdef	_DEBUG
			*LogM << LogM->ErrorCode(0)<< "Small Rating Data Octet!!" << endl;
			#endif
			return;
	}
	// 계산된 패킷량으로 사용금액을 계산.
	// 계산하고 남은 짜투리는 적용시키지 않는다.
	nRatingPrice = stRate->GetRate(2, 0, 0, 0, nRateDataAmt );
	// FMO :후에 rating class수정되면 바꿀 것 
	//nRatingPrice = stRate->GetRate(2, 0, 0, 0, nRateDataAmt, stUdr.stItemF.get_nForwardMUXOpt());
	// 계산하고 남은 짜투리 패킷
	nRemain = stRate->GetLimitSec();
	//조회된 이전까지의 토달 금액 + 현재 과금액 
	stTblMgr.SetTotalAmt(stTblMgr.GetTotalAmt()+nRatingPrice);
	
	#ifdef	_DEBUG
	*LogM << LogM->ErrorCode(0) 
		<< "============ 한도처리결과 ===========" <<  endl;
	*LogM << LogM->ErrorCode(0) 
		<< "기존 한도 금액 : " << stTblMgr.GetLimitAmt() <<  endl;
	*LogM << LogM->ErrorCode(0) 
		<< "사용 DataOctet : " << nRateDataAmt<<  endl;
	*LogM << LogM->ErrorCode(0) 
		<< "현 사용금액 : " << nRatingPrice<<  endl;
	*LogM << LogM->ErrorCode(0) 
		<< "총사용금액 : " << stTblMgr.GetTotalAmt()<<  endl;
	*LogM << LogM->ErrorCode(0) 
		<< "과금후 나머지 DataOctet : " << nRemain <<  endl;
	*LogM << LogM->ErrorCode(0) << "남은 한도 금액 : " 
			<< stTblMgr.GetLimitAmt() - (stTblMgr.GetTotalAmt() - nPreviousRateAmt)<<  endl;
	#endif
	
	// T_THRESHOLD_AMT
	// Event Time
	stTblMgr.SetEventTime(stUdr.stItemG.get_nEndEventTime()); 
	// Intrim UDR Count
	stTblMgr.SetIntrimCnt(stTblMgr.GetIntrimCnt() + 1);
	// T_THRESHOLD_MID_AMT
	stTblMgr.SetCurrentAmt(nRatingPrice + stTblMgr.GetCurrentAmt());	// 현재까지 사용한 금액
	stTblMgr.SetLastDataOct(stUdr.stItemG.get_nDataOctetCntTerminat()	// 다음과금량 계산 시 
							+ stUdr.stItemG.get_nDataOctetCntOrigin());	// 필요
	
	// Session별 현재 사용액 update
	if( stTblMgr.UpdateLimitInfo1() < 0)
	{
		#ifdef	_DEBUG
		*LogM << LogM->ErrorCode(0)<<"Limit Info Update fail...." <<  endl;
		#endif
	}
	
	// 한도금액이 넘었을 경우 : 한도금액 - session별 현재 사용액 총합(정산금액) 
	if( (stTblMgr.GetLimitAmt() - (stTblMgr.GetTotalAmt() - nPreviousRateAmt) ) <= 0 )
	{
		#ifdef	_DEBUG
			*LogM << LogM->ErrorCode(0)<< "한도금액 초과 !!" << endl;
			*LogM << LogM->ErrorCode(0)<< "기존 한도금액	:["<<stTblMgr.GetLimitAmt()<<"]" << endl;
			*LogM << LogM->ErrorCode(0)<< "총사용금액	:["<<stTblMgr.GetTotalAmt()<<"]" << endl;
			*LogM << LogM->ErrorCode(0)<< "기존정산금액	:["<<nPreviousRateAmt<<"]" << endl;
		#endif
		memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
		stAccReqMsg.stItemA2.set_Key(0x03);
		stAccReqMsg.stItemA2.set_Min(stUdr.stItemA.get_unMsId() );	// Min
		
		SndReqToSCP();
		//해당 번호에 대한 한도 정보를 삭제한다
		stTblMgr.DeleteAllLimitInfo();
	}
		
	// SCP 중간정산보내기 
	if((stTblMgr.GetIntrimCnt() % gnAccReqCnt) == 0)
	{
		#ifdef	_DEBUG
			*LogM << LogM->ErrorCode(0)<< "send intrim Account Request !!" << endl;
		#endif
		
		// T_THRESHOLD_AMT 한도금액 및 정산금액 Update
		stTblMgr.SetRateAmt(stTblMgr.GetTotalAmt());
		stTblMgr.SetLimitAmt( stTblMgr.GetLimitAmt() - (stTblMgr.GetTotalAmt()-nPreviousRateAmt) );
		if(stTblMgr.UpdateLimitNRateAmt() < 0)
		{
			#ifdef	_DEBUG
			*LogM << LogM->ErrorCode(0)<<"Limt And Rate Amt Update fail...." <<  endl;
			#endif
		}
		
		memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
		stAccReqMsg.get_ItemA2()->set_Key(0x02);
		stAccReqMsg.get_ItemA2()->set_Min(stUdr.stItemA.get_unMsId() );	// Min
		stAccReqMsg.get_ItemB()->set_AccSessionId( stUdr.stItemC.get_strAccountSessionID() );
		stAccReqMsg.get_ItemB()->set_CorreId( stUdr.stItemC.get_strCorrelationID() );
		stAccReqMsg.get_ItemC()->set_StartT(0);
		stAccReqMsg.get_ItemC()->set_EndT(0);
		// 정산금액 = session별 총사용액 - 기존 정산금액
		stAccReqMsg.get_ItemD()->set_LimitT(stTblMgr.GetTotalAmt() - nPreviousRateAmt); //계산된 금액
		stAccReqMsg.get_ItemD()->set_NTuseT(0);
		stTblMgr.SetOId(stTblMgr.GetOId() + 1); // scp송신 때마다 +1 증가해서 보낸다
		stAccReqMsg.get_ItemB()->set_OperId(stTblMgr.GetOId());//증가된 OID를 select해서 넣어야한다
		
		if(stTblMgr.UpdateOId() < 0)
		{
			#ifdef	_DEBUG
			*LogM << LogM->ErrorCode(0)<<"Limit Info Update fail...." <<  endl;
			#endif
		}
		
		#ifdef	_DEBUG
		*LogM << LogM->ErrorCode(0)<< "<<보낸 중간정산메시지>>" << endl;
		*LogM << LogM->ErrorCode(0)<< " 번호      : "<<stAccReqMsg.get_ItemA2()->get_Min()<< endl;
		*LogM << LogM->ErrorCode(0)<< " 정산금액   : "<<stAccReqMsg.get_ItemD()->get_LimitT()<<  endl;
		#endif
		SndReqToSCP();
		
	}
	return;
}


void CLimitMGR::ProcStop()
{
	int		nRateDataAmt=0;
	int		nRatingPrice=0, nRemain=0;
	int		nPreviousRateAmt;
	
	// 두개의 table에서 정산에 필요한 정보를 읽어온다. 
	// 한도잔액,최종패킷사용량, 사용금액, 남은 금액, operation ID, nIntrimCnt
	// 조회전에 전화번호, Account Session ID, Correlation ID를 stTblMgr class에 set한다
	stTblMgr.SetMin(stUdr.stItemA.get_unMsId());
	stTblMgr.SetAccSId(stUdr.stItemC.get_strAccountSessionID());
	stTblMgr.SetCorrId(stUdr.stItemC.get_strCorrelationID());
	int nRet = stTblMgr.GetLimitInfo();
	
	// 기존정산금액
	nPreviousRateAmt = stTblMgr.GetRateAmt();
	
	if( nRet == 1403)// session에 대한 정보가 존재하지 않는다
	{
		#ifdef	_DEBUG
		*LogM << LogM->ErrorCode(0)<<"Previus Data Not Exist...Send End Request!" <<  endl;
		#endif
	}

	// 과금할 패킷량 계산: 현재들어온 누적패킷량 - 이전까지의 누적량
	nRateDataAmt = (stUdr.stItemG.get_nDataOctetCntTerminat() + stUdr.stItemG.get_nDataOctetCntOrigin())
					- stTblMgr.GetLastDataOct();
	if( nRateDataAmt <= 0 )
	{
			#ifdef	_DEBUG
			*LogM << LogM->ErrorCode(0)<< "Small Rating Data Octet!!" << endl;
			#endif
			return ;
	}
	// 게산된 패킷량으로 사용금액을 계산.
	// 계산하고 남은 짜투리는 적용시키지 않는다.
	nRatingPrice = stRate->GetRate(2, 0, 0, 0, nRateDataAmt );
	// FMO :후에 rating class수정되면 바꿀 것 
	//nRatingPrice = stRate->GetRate(2, 0, 0, 0, nRateDataAmt, stUdr.stItemF.get_nForwardMUXOpt());
	// 계산하고 남은 짜투리 패킷
	nRemain = stRate->GetLimitSec();
	//조회된 이전까지의 토달 금액 + 현재 과금액 
	stTblMgr.SetTotalAmt(stTblMgr.GetTotalAmt()+nRatingPrice);
	
	#ifdef	_DEBUG
	*LogM << LogM->ErrorCode(0) 
		<< "============ 한도처리결과 ===========" <<  endl;
	*LogM << LogM->ErrorCode(0) 
		<< "기존 한도 금액 : " << stTblMgr.GetLimitAmt() <<  endl;
	*LogM << LogM->ErrorCode(0) 
		<< "사용 DataOctet : " << nRateDataAmt<<  endl;
	*LogM << LogM->ErrorCode(0) 
		<< "사용금액 : " << nRatingPrice<<endl;
	*LogM << LogM->ErrorCode(0) 
		<< "총사용금액 : " << stTblMgr.GetTotalAmt()<<  endl;
	*LogM << LogM->ErrorCode(0) 
		<< "과금후 나머지 DataOctet : " << nRemain <<  endl;
	*LogM << LogM->ErrorCode(0) << "남은 한도 금액 : " 
			<< stTblMgr.GetLimitAmt() - (stTblMgr.GetTotalAmt() - nPreviousRateAmt)<<  endl;
	#endif
	
	stTblMgr.SetEventTime(stUdr.stItemG.get_nEndEventTime());

	if( (stTblMgr.GetLimitAmt() - (stTblMgr.GetTotalAmt() - nPreviousRateAmt ) ) <= 0 )
	{
		#ifdef	_DEBUG
		*LogM << LogM->ErrorCode(0)<< "한도금액 초과 !!" << endl;
		*LogM << LogM->ErrorCode(0)<< "기존 한도금액	:["<<stTblMgr.GetLimitAmt()<<"]" << endl;
		*LogM << LogM->ErrorCode(0)<< "총사용금액	:["<<stTblMgr.GetTotalAmt()<<"]" << endl;
		*LogM << LogM->ErrorCode(0)<< "기존정산금액	:["<<nPreviousRateAmt<<"]" << endl;
		#endif
		
		memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
		stAccReqMsg.get_ItemA2()->set_Key(0x03);
		stAccReqMsg.get_ItemA2()->set_Min(stUdr.stItemA.get_unMsId() );	// Min
		SndReqToSCP();
		//해당 번호에 대한 한도 정보를 삭제한다
		stTblMgr.DeleteAllLimitInfo();
	}

	memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
	stAccReqMsg.get_ItemA2()->set_Key(0x04);
	stAccReqMsg.get_ItemA2()->set_Min(stUdr.stItemA.get_unMsId() );	// Min
	stAccReqMsg.get_ItemC()->set_StartT(0);
	stAccReqMsg.get_ItemC()->set_EndT(0);
	// 정산금액 = session별 총사용액 - 기존 정산금액
	stAccReqMsg.get_ItemD()->set_LimitT(stTblMgr.GetTotalAmt() - stTblMgr.GetRateAmt()); //계산된 금액
	stAccReqMsg.get_ItemD()->set_NTuseT(0);
	stTblMgr.SetOId(stTblMgr.GetOId() + 1); // scp송신 때마다 +1 증가해서 보낸다
	stAccReqMsg.get_ItemB()->set_OperId(stTblMgr.GetOId());//증가된 OID를 select해서 넣어야한다
	
	if(stTblMgr.UpdateOId() < 0)
	{
		#ifdef	_DEBUG
		*LogM << LogM->ErrorCode(0)<<"Limit Info Update fail...." <<  endl;
		#endif
	}
	
	SndReqToSCP();
	
	#ifdef	_DEBUG
	*LogM << LogM->ErrorCode(0)<< "<<보낸 중간정산메시지(STOP UDR)>>" << endl;
	*LogM << LogM->ErrorCode(0)<< " 번호      : "<<stAccReqMsg.get_ItemA2()->get_Min()<< endl;
	*LogM << LogM->ErrorCode(0)<< " 정산금액   : "<<stAccReqMsg.get_ItemD()->get_LimitT()<<  endl;
	#endif

	// T_THRESHOLD_MID_AMT
	stTblMgr.SetCurrentAmt(nRatingPrice + stTblMgr.GetCurrentAmt());	// 현재까지 사용한 금액
	stTblMgr.SetLastDataOct(stUdr.stItemG.get_nDataOctetCntTerminat()	// 다음과금량 계산 시 
							+ stUdr.stItemG.get_nDataOctetCntOrigin());	// 필요

	// T_THRESHOLD_AMT 한도금액 및 정산금액 Update
	if(stTblMgr.UpdateLimitInfo1() < 0)
	{
		#ifdef	_DEBUG
		*LogM << LogM->ErrorCode(0)<<"Limt And Rate Amt Update fail...." <<  endl;
		#endif
	}

	// T_THRESHOLD_AMT 한도금액 및 정산금액 Update
	stTblMgr.SetRateAmt(nPreviousRateAmt+stAccReqMsg.get_ItemD()->get_LimitT());
	stTblMgr.SetLimitAmt( stTblMgr.GetLimitAmt() - (stTblMgr.GetTotalAmt()-nPreviousRateAmt) );
	if(stTblMgr.UpdateLimitNRateAmt() < 0)
	{
		#ifdef	_DEBUG
		*LogM << LogM->ErrorCode(0)<<"Limt And Rate Amt Update fail...." <<  endl;
		#endif
	}

}


// scp로 전송
void CLimitMGR::SndReqToSCP()
{
	// Send request Aggregator - Scp I/F MQ
    while(1)
    {
        if( mqSCP->sendMsg( &stAccReqMsg ) == FLAG_MQ_IS_FULL )
        {
			#ifdef	_DEBUG
	   		*LogM<< "Limit Manager -> SCP MQ is full..."<<LogM->ErrorCode(0)<< endl;
			#endif
			usleep(100000);
        } else {
        	#ifdef	_DEBUG
	   		*LogM<< "Send Success to SCP..."<<LogM->ErrorCode(0)<< endl;
			#endif
        	break;
        }//else
    }//while
}


// SCP로부터 받은 한도요금 조회 결과, 중간정산 결과처리
// 최종 OID의 한도금액을 UPDATE한다 
void CLimitMGR::DoResProcess()
{
	stTblMgr.SetMin(stUdr.stItemA.get_unMsId());
	stTblMgr.SetOId(stUdr.stLimitInfo.get_nOperationID());
	stTblMgr.SetAccSId(stUdr.stItemC.get_strAccountSessionID());
	stTblMgr.SetCorrId(stUdr.stItemC.get_strCorrelationID());
	stTblMgr.SetLimitAmt(stUdr.stLimitInfo.get_nLimitPrice());

	#ifdef	_DEBUG
	*LogM<<"UDR TYPE: "<<stUdr.stItemHead.get_nUdrType()<<LogM->ErrorCode(0)<< endl;
	*LogM<<"MIN		: "<<stUdr.stItemA.get_unMsId()<<LogM->ErrorCode(0)<< endl;
	*LogM<<"ACC_ID	: "<<stUdr.stItemC.get_strAccountSessionID()<<LogM->ErrorCode(0)<< endl;
	*LogM<<"CORR_ID	: "<<stUdr.stItemC.get_strCorrelationID()<<LogM->ErrorCode(0)<< endl;
	*LogM<<"OP_ID	: "<<stUdr.stLimitInfo.get_nOperationID()<<LogM->ErrorCode(0)<< endl;
	*LogM<<"한도금액	: "<<stUdr.stLimitInfo.get_nLimitPrice()<<LogM->ErrorCode(0)<< endl;
	#endif
	
	if(stUdr.stItemHead.get_nUdrType() == 0x81) // 한도요금조회 결과
	{
		#ifdef	_DEBUG
	   		*LogM<< "RECV..한도정보 조회결과"<<LogM->ErrorCode(0)<< endl;
		#endif
	}
	else if(stUdr.stItemHead.get_nUdrType() == 0x82) // 중간정산 결과
	{
		#ifdef	_DEBUG
	   		*LogM<< "RECV..중간정산 결과"<<LogM->ErrorCode(0)<< endl;
		#endif
	}
	
	if(stUdr.stLimitInfo.get_nLimitPrice()<= 0)
	{
		#ifdef	_DEBUG
   		*LogM<< "한도금액이 없음...."<<LogM->ErrorCode(0)<< endl;
		#endif
		memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
		stAccReqMsg.get_ItemA2()->set_Key(0x03);
		stAccReqMsg.get_ItemA2()->set_Min(stUdr.stItemA.get_unMsId() );	// Min
		SndReqToSCP();
		//해당 번호에 대한 한도 정보를 삭제한다
		stTblMgr.DeleteAllLimitInfo();
	}
	
	if(stUdr.stLimitInfo.get_nOperationID()  >= stTblMgr.SelectOId())
			stTblMgr.UpdateLimitAmt();  // 한도요청 결과가 오면 금액을 insert
	return;
	#ifdef	_DEBUG
   		*LogM<< "한도정보 Not Update...Operation ID small..."<<LogM->ErrorCode(0)<< endl;
	#endif
}


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


void sigCapture(int sigNo)
{
	LogM->put_EventHistData(FLAG_END,0);
	
	switch(sigNo)
	{
		case SIGINT :
		case SIGQUIT :
		case SIGKILL :
		case SIGTERM :
				WritePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				delete mqPMS;
				exit(-1);
				break;
		case SIGUSR1:
				WritePMSMQ(FLAG_PROC_TERM);//0//프로세스정상stop (Client업무명령)
				delete mqPMS;
				exit(0);
				break;
		default:
				WritePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				delete mqPMS;
				exit(-1);
				break;
	}
}


int main(int argc, char **argv)
{
	CLimitMGR	*cLimitMgr;
	
	if(argc != 7 )
    {
    	cout <<"Usage: LimitMGR [sysId][svcId][gId][pId][pSn][pRemark]"<<endl;
    	exit(-1);
    }
    
    SystemId=atoi(argv[1]);   
	ServiceId=atoi(argv[2]);  
	GroupId=atoi(argv[3]);    
	ProcessType=atoi(argv[4]);
	PserialNo=atoi(argv[5]);  
	PreMark=atoi(argv[6]);    
   
	signal(SIGUSR1,sigCapture); 	signal(SIGTERM,sigCapture);
	signal(SIGTERM,sigCapture);
	signal(SIGINT,sigCapture); 	signal(SIGSTOP,sigCapture);
	signal(SIGQUIT,sigCapture);	signal(SIGTSTP,sigCapture);
	signal(SIGKILL,sigCapture);	signal(SIGCONT,sigCapture);	//26
	
	cLimitMgr	= new CLimitMGR(SystemId);
	LogM		= new SendLogManage(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),\
								 atoi(argv[4]),atoi(argv[5]),atoi(argv[6]));
	mqPMS		= new MessageQueue<MsgPmsStatus>(atoi(argv[1]), 0,1,1,0,10, 3000);
	mqSCP		= new MessageQueue<SCPFormatAgg>(atoi(argv[1]), ServiceId,4,1,0,10, 3000);
	mqLMGR		= new MessageQueue<UdrFormat>(atoi(argv[1]), ServiceId,4,4,0,10, 3000);
	
	LogM->put_EventHistData(FLAG_START,FLAG_PROC_START);
	*LogM<<"Limit Manager Start!"<<LogM->ErrorCode(0)<<endl;
	WritePMSMQ(FLAG_PROC_START);
    
    if(stTblMgr.ConnectToDB() < 0)
	{
		#ifdef	_DEBUG
		*LogM<<"DataBase Connect error.."<<LogM->ErrorCode(0)<<endl;
		#endif	
	}
	
	cLimitMgr->Start();
	stTblMgr.DbClose();
	
	delete cLimitMgr;
	
    return 0;
}





