#include "CLimitMGR.hpp"

CLimitMGR	cLimitMgr;

//////////////////////////////////////////
//		Class CLimitMGR's Method		//
//////////////////////////////////////////

//------------------------------------------------------------------------------
// ������
CLimitMGR::CLimitMGR()
{
	;
}



//------------------------------------------------------------------------------
// �Ҹ���
CLimitMGR::~CLimitMGR()
{
	delete	LogM;
	delete	mqSCP;
	delete	mqLMGR;
	delete	mqPMS;
}



//------------------------------------------------------------------------------
// �ѵ� ������ �� ������ ����
int CLimitMGR::Start()
{
	while(1)
	{
		memset((char*)&stUdr, NULL, sizeof(UdrFormat));

		// Filter�� ���� �ѵ� UDR��
		// SCP�� ���� �ѵ� ������ ��� ���� ��ȸ ��� �Ǵ� ������ ��� �߰� ���� ����� MQ�� ���� ����		
		if(mqLMGR->recvMsg(&stUdr) == FLAG_MQ_IS_EMPTY)
			sleep(2);
		else
		{ 
	   		// SCP�� ���� �ѵ� ������ ��� ���� ��ȸ ��� �Ǵ� ������ ��� �߰� ���� ����� ��
	   		if(stUdr.stItemHead.get_nUdrType() == LIMIT_RESULT 
	   			|| stUdr.stItemHead.get_nUdrType() == ACCESS_RESULT )
	   		{
	   			#ifdef	__DEBUG
				*LogM<<LogM->ErrorCode(0)<< endl;
				*LogM<<LogM->ErrorCode(0)<< endl;
	   			*LogM<< "<< Recv Response from SCP >>"<<LogM->ErrorCode(0)<< endl;
				#endif
			}
			else 	// Filter�� ���� �ѵ� UDR
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
// START UDR�� ������ �� ó��
int CLimitMGR::ProcStart()
{
	int	nRet = 0;
	// [t_threshold_amt] table class�� set
	ILimitTbl.SetMin		(stUdr.stItemA.get_unMsId				());
	ILimitTbl.SetAccSId	(stUdr.stItemC.get_strAccountSessionID	());
	ILimitTbl.SetCorrId	(stUdr.stItemC.get_strCorrelationID		());

	// [t_threshold_amt] table�� �ѵ� ������ �������� ������ SCP�� �ѵ� ���� ��ȸ �䱸�Ѵ�.
	nRet = ILimitTbl.GetLimitInfo();
	if(nRet == 1403)
	{
		// SCP�� ���� TTL ������ ��� ���� ��ȸ �䱸 �޼����� �����
		memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
		stAccReqMsg.stItemA2.set_Key		(0x01);								// �����ڵ� 
		stAccReqMsg.stItemA2.set_Min		(stUdr.stItemA.get_unMsId() );		// Min
		stAccReqMsg.stItemB.set_OperId		(0);								// �޽������� Ű 
		stAccReqMsg.stItemB.set_AccSessionId(stUdr.stItemC.get_strAccountSessionID());
		stAccReqMsg.stItemB.set_CorreId		(stUdr.stItemC.get_strCorrelationID());

		SndReqToSCP();

		#ifdef	__DEBUG
		*LogM << LogM->ErrorCode(0) << "###-�ѵ����� ��ȸ ��û ����" <<  endl;
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
		<< "-�����ѵ����� ���� => �ѵ��ݾ� :["<<ILimitTbl.GetLimitAmt()<<"]"<<  endl;
		#endif
	}
	else if(nRet == -1)
		return -1;
	
	return 0;	
}



//------------------------------------------------------------------------------
// INTERIM UDR�� ������ �� ó��
int CLimitMGR::ProcIntrim()
{
	int		nRet			= 0;
	int		nRateDataAmt	= 0;						// ���� ����� ��Ŷ��
	int		nRemain			= 0;						// ����ϰ� ���� ¥���� ��Ŷ
	int		nPreviousRateAmt= 0;						// ���� ���� �ݾ�
	int		nPreRemainAmt	= 0;						// ���� ¥���� ��Ŷ
	int		nPreLimitAmt	= 0;						// ���� �ѵ� �ݾ�
	int		nSpeedFlag		= 0;						// '1':����, '2':����
		
	nRatingPrice			= 0;						// ���� ����� �ݾ�
		
	ILimitTbl.SetMin(stUdr.stItemA.get_unMsId());
	ILimitTbl.SetAccSId(stUdr.stItemC.get_strAccountSessionID());
	ILimitTbl.SetCorrId(stUdr.stItemC.get_strCorrelationID());

	// [t_threshold_amt], [t_threshold_mid_amt]���� ������ �����´�
	nRet = ILimitTbl.GetLimitInfo();
	
	nPreviousRateAmt = ILimitTbl.GetRateAmt	();		
	nPreLimitAmt	 = ILimitTbl.GetLimitAmt();
	nPreRemainAmt	 = ILimitTbl.GetRemainAmt();
				
	if(nRet == 1403)									// MIN�� �ѵ������� �������� �ʴ´�
	{
		#ifdef	__DEBUG
		*LogM << LogM->ErrorCode(0)<<"-Start Not Exist...Send Access Request!" <<  endl;
		#endif

		if(ProcStart() < 0)								//Start ���� Intrim udr�� recv���� ��
			return -1;

		return 0; 
	} 	
	else if(nRet == -1)
		return -1;

	#ifdef	__DEBUG
	*LogM << LogM->ErrorCode(0) << "<<���� ��Ŷ��>>"	<< endl;
	*LogM << LogM->ErrorCode(0) << " ���� ��Ŷ��	: "	<< ILimitTbl.GetLastDataOct() << endl;
	*LogM << LogM->ErrorCode(0) << " ���� ��Ŷ��	: "	
			<< stUdr.stItemG.get_nDataOctetCntTerminat() + stUdr.stItemG.get_nDataOctetCntOrigin() <<  endl;
	#endif

	// ������ ��Ŷ�� ���: ������� ������Ŷ�� - ���������� ������
	nRateDataAmt = (stUdr.stItemG.get_nDataOctetCntTerminat() + stUdr.stItemG.get_nDataOctetCntOrigin())
					- ILimitTbl.GetLastDataOct();
					
	if( nRateDataAmt <= 0 )
	{
		#ifdef	__DEBUG
		*LogM << LogM->ErrorCode(0)<< "Small Rating Data Octet!!" << endl;
		#endif

		return 0;
	}
	
	// ����(1), ����(2337) Ȯ�� 
	if(stUdr.stItemF.get_nForwardMUXOpt() == 1)
		nSpeedFlag = 1;
	else
		nSpeedFlag = 2;
			
	// (���� ��Ŷ�� + ���� ����ϰ� ���� ¥���� ��Ŷ��)���� ���ݾ��� ���.
	nRatingPrice = stRate->GetRate(nSpeedFlag, 0, 0, 0, nRateDataAmt + ILimitTbl.GetRemainAmt());
	
	// FMO :�Ŀ� rating class�����Ǹ� �ٲ� �� 
	// nRatingPrice = stRate->GetRate(2, 0, 0, 0, nRateDataAmt, stUdr.stItemF.get_nForwardMUXOpt());

	nRemain = stRate->GetLimitSec();					// ����ϰ� ���� ¥���� ��Ŷ

	//�� ���ݾ� = ��ȸ�� ���������� ��� �ݾ� + ���� ��� �ݾ� 
	ILimitTbl.SetTotalAmt(ILimitTbl.GetTotalAmt() + nRatingPrice);
	ILimitTbl.SetRemainAmt(nRemain);			

	#ifdef	__DEBUG
	*LogM << LogM->ErrorCode(0) << "###�ѵ�ó�����" 	<<  endl;
	*LogM << LogM->ErrorCode(0) << "���� �ѵ� �ݾ� : " 	<< ILimitTbl.GetLimitAmt() 	<<  endl;
	*LogM << LogM->ErrorCode(0) << "��� DataOctet : " 	<< nRateDataAmt				<<  endl;
	*LogM << LogM->ErrorCode(0) << "�� ���ݾ�    : " 	<< nRatingPrice				<<  endl;
	*LogM << LogM->ErrorCode(0) << "�ѻ��ݾ�     : " 	<< ILimitTbl.GetTotalAmt()	<<  endl;
	*LogM << LogM->ErrorCode(0) << "������ ������ DataOctet : " << nRemain <<  endl;
	*LogM << LogM->ErrorCode(0) << "���� �ѵ� �ݾ� : " 
			<< ILimitTbl.GetLimitAmt() - (ILimitTbl.GetTotalAmt() - nPreviousRateAmt)<<  endl;
	#endif

	ILimitTbl.SetIntrimCnt(ILimitTbl.GetIntrimCnt() + 1);	// Intrim UDR Count ����

	if(InterimAccountReq(nPreviousRateAmt) < 0)			// �߰� ���� ��û
		return -1;
	
	if(LimitMGR(nPreLimitAmt, nPreviousRateAmt) < 0)	// �ѵ� �ʰ� Ȯ��
		return -1;
	
	ILimitTbl.Commit();		
	return 0;
}



//------------------------------------------------------------------------------
// STOP UDR�� ������ �� ó��
int CLimitMGR::ProcStop()
{
	int		nRet			= 0;
	int		nRateDataAmt	= 0;				// ���� ����� ��Ŷ��
	int		nRemain			= 0;				// ����ϰ� ���� ¥���� ��Ŷ
	int		nPreviousRateAmt= 0;				// ���� ���� �ݾ�
	int		nPreRemainAmt	= 0;				// ���� ¥���� ��Ŷ
	int		nSessionCnt		= 0;
	int		nPreLimitAmt	= 0;				// ���� �ѵ� �ݾ�
	int		nSpeedFlag		= 0;				// '1':����, '2':����
		
	nRatingPrice			= 0;				// ���� ����� �ݾ�
	
	ILimitTbl.SetMin(stUdr.stItemA.get_unMsId());
	ILimitTbl.SetAccSId(stUdr.stItemC.get_strAccountSessionID());
	ILimitTbl.SetCorrId(stUdr.stItemC.get_strCorrelationID());

	// [t_threshold_amt], [t_threshold_mid_amt]���� ������ �����´�
	nRet = ILimitTbl.GetLimitInfo();

	if(nRet == -1)
		return -1;						

	nPreviousRateAmt = ILimitTbl.GetRateAmt();
	nPreLimitAmt	 = ILimitTbl.GetLimitAmt();
	nPreRemainAmt	 = ILimitTbl.GetRemainAmt();
		
	#ifdef	__DEBUG
	*LogM << LogM->ErrorCode(0) << "<<���� ��Ŷ��>>"	<< endl;
	*LogM << LogM->ErrorCode(0) << " ���� ��Ŷ��	: "	<< ILimitTbl.GetLastDataOct() << endl;
	*LogM << LogM->ErrorCode(0) << " ���� ��Ŷ��	: "	
			<< stUdr.stItemG.get_nDataOctetCntTerminat() + stUdr.stItemG.get_nDataOctetCntOrigin() <<  endl;
	#endif

	// ������ ��Ŷ�� ���: ������� ������Ŷ�� - ���������� ������
	nRateDataAmt = (stUdr.stItemG.get_nDataOctetCntTerminat() + stUdr.stItemG.get_nDataOctetCntOrigin())
					- ILimitTbl.GetLastDataOct();

	if( nRateDataAmt <= 0 )
	{
		#ifdef	__DEBUG
		*LogM << LogM->ErrorCode(0)<< "Small Rating Data Octet!!" << endl;
		#endif

		return 0;
	}

	// ����(1), ����(2337) Ȯ�� 
	if(stUdr.stItemF.get_nForwardMUXOpt() == 1)
		nSpeedFlag = 1;
	else
		nSpeedFlag = 2;

	// (���� ��Ŷ�� + ���� ����ϰ� ���� ¥���� ��Ŷ��)���� ���ݾ��� ���.
	nRatingPrice = stRate->GetRate(nSpeedFlag, 0, 0, 1, nRateDataAmt + ILimitTbl.GetRemainAmt());

	// FMO :�Ŀ� rating class�����Ǹ� �ٲ� �� 
	//nRatingPrice = stRate->GetRate(2, 0, 0, 0, nRateDataAmt, stUdr.stItemF.get_nForwardMUXOpt());

	// ����ϰ� ���� ¥���� ��Ŷ
	nRemain = stRate->GetLimitSec();

	//�� ���ݾ� = ��ȸ�� ���������� ��� �ݾ� + ���� ��� �ݾ� 
	ILimitTbl.SetTotalAmt(ILimitTbl.GetTotalAmt() + nRatingPrice);
	ILimitTbl.SetRemainAmt(nRemain);
	
	#ifdef	__DEBUG
	*LogM << LogM->ErrorCode(0) << "-----�ѵ�ó�����-----"				 			<<  endl;
	*LogM << LogM->ErrorCode(0) << "-���� �ѵ� �ݾ� : " 	<< ILimitTbl.GetLimitAmt() 	<<  endl;
	*LogM << LogM->ErrorCode(0) << "-��� DataOctet : " 	<< nRateDataAmt				<<  endl;
	*LogM << LogM->ErrorCode(0) << "-�� ���ݾ�    : " 	<< nRatingPrice				<<  endl;
	*LogM << LogM->ErrorCode(0) << "-�ѻ��ݾ�     : " 	<< ILimitTbl.GetTotalAmt()	<<  endl;
	*LogM << LogM->ErrorCode(0) << "-������ ������ DataOctet : " << nRemain <<  endl;
	*LogM << LogM->ErrorCode(0) << "-���� �ѵ� �ݾ� : " 
			<< ILimitTbl.GetLimitAmt() - (ILimitTbl.GetTotalAmt() - nPreviousRateAmt)<<  endl;
	#endif
	
	ILimitTbl.SetEventTime(stUdr.stItemG.get_nEndEventTime());

	// �ڱ� MIN�� ����, ���� id �Ǵ�  collation id�� �ٸ� [t_threshold_mid_amt] ���ڵ��� ���� 
	nSessionCnt = ILimitTbl.SelectAmtTbl();

	if((nRet == 1403) || (nRet == NOT_EXIST_SESSION))
	{
		// [t_threshold_amt], [t_threshold_mid_amt] table�� ����Ÿ �������� ����
		if(nRet == 1403)
		{
			*LogM << LogM->ErrorCode(0)<<"-Send End Request!" <<  endl;
			SndServiceEndReq();
			return 0;
		}	
		else// �ش� ������ [t_threshold_mid_amt] table�� �������� ����
		{
			// �ڱ� MIN�� ����, ���� id �Ǵ�  collation id�� �ٸ� [t_threshold_mid_amt] ���ڵ尡 �������� ����
			if(nSessionCnt == 0)
			{
				*LogM << LogM->ErrorCode(0)<<"-Send End Request!" <<  endl;
				SndServiceEndReq();
				*LogM << LogM->ErrorCode(0)<<"-�ѵ����� ���̺� ����!" <<  endl;
				if(ILimitTbl.DeleteAmtTbl() < 0)
					return -1;

				ILimitTbl.Commit();		
				return 0;
			}
			else// �ڱ� MIN�� ����, ���� id �Ǵ�  collation id�� �ٸ� [t_threshold_mid_amt] ���ڵ尡 ������
			{
				*LogM << LogM->ErrorCode(0)<<"-�߰� ����!" <<  endl;
				if(InterimAccountReq(nPreviousRateAmt, 1) < 0)
					return -1;

				*LogM << LogM->ErrorCode(0)<<"-�ѵ�����!" <<  endl;
				if(LimitMGR(nPreLimitAmt, nPreviousRateAmt) < 0)
					return -1;

				ILimitTbl.Commit();		
				return 0;
			}
		}
	}

	// �ش� ������ [t_threshold_mid_amt]�� �����ϰ�, 
	// �ڱ� MIN�� ����, ���� id �Ǵ�  collation id�� �ٸ� [t_threshold_mid_amt] ���ڵ尡 �������� ����
	if(nSessionCnt == 0)
	{
		*LogM << LogM->ErrorCode(0)<<"-Send End Request!" <<  endl;
		SndServiceEndReq();

		*LogM << LogM->ErrorCode(0)<<"-���� ���̺� ����!" <<  endl;
		//�ش� ��ȣ, Account session id, Collation id�� ���� �ѵ� ������ �����Ѵ�
		if(ILimitTbl.DeleteMidAmtTbl() < 0)
			return -1;

		*LogM << LogM->ErrorCode(0)<<"-�ѵ����� ���̺� ����!" <<  endl;
		if(ILimitTbl.DeleteAmtTbl() < 0)
			return -1;
	}
	else
	{
		// �ش� ������ [t_threshold_mid_amt]�� �����ϰ�, 
		// �ڱ� MIN�� ����, ���� id �Ǵ�  collation id�� �ٸ� [t_threshold_mid_amt] ���ڵ尡 �������� ����
		*LogM << LogM->ErrorCode(0)<<"-�߰�����!" <<  endl;
		if(InterimAccountReq(nPreviousRateAmt, 1) < 0)
			return -1;
		
		*LogM << LogM->ErrorCode(0)<<"-���� ���̺� ����!" <<  endl;
		if(ILimitTbl.DeleteMidAmtTbl() < 0)
			return -1;
		
		*LogM << LogM->ErrorCode(0)<<"-�ѵ�����!" <<  endl;
		if(LimitMGR(nPreLimitAmt, nPreviousRateAmt) < 0)
			return -1;

		if(ILimitTbl.UpdateRateAmt() < 0)
			return -1;
	}	

	ILimitTbl.Commit();		
	return 0;
}



//------------------------------------------------------------------------------
// �ѵ����� ���� ���̺��� update�� ��, SCP�� ���� �߰� ���� ��û �޼����� �����, SCP MQ�� �޼����� ����
int CLimitMGR::InterimAccountReq(int nPreviousRate, int nStopFlag)
{
	// [t_threshold_amt], [t_threshold_mid_amt] table update
	// Event time
	ILimitTbl.SetEventTime(stUdr.stItemG.get_nEndEventTime()); 

	// �ش� ������ ������� ����� �� �ݾ�
	ILimitTbl.SetCurrentAmt(nRatingPrice + ILimitTbl.GetCurrentAmt());
	// ������� ������Ŷ��
	ILimitTbl.SetLastDataOct(stUdr.stItemG.get_nDataOctetCntTerminat()
							+ stUdr.stItemG.get_nDataOctetCntOrigin());
	if( ILimitTbl.UpdateLimitInfo1() < 0)
		return -1;


	// SCP �߰����꺸���� (nStopFlag-'0':INTERIM UDR, '1':STOP UDR)
	if(((ILimitTbl.GetIntrimCnt() % ACCESS_REQ_CNT) == 0) || (nStopFlag == 1))
	{
		// �� ���� �ݾ�
		ILimitTbl.SetRateAmt(ILimitTbl.GetTotalAmt());
		// ���� �� �ѵ� �ݾ�
		ILimitTbl.SetLimitAmt( ILimitTbl.GetLimitAmt() - (ILimitTbl.GetTotalAmt()-nPreviousRate) );

		// [t_threshold_amt] table�� �ѵ� �ݾ�, ���� �ݾ� update
		if(ILimitTbl.UpdateLimitNRateAmt() < 0)
			return -1;
		
		memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
		stAccReqMsg.get_ItemA2()->set_Key(0x02);
		stAccReqMsg.get_ItemA2()->set_Min(stUdr.stItemA.get_unMsId() );	// Min
		stAccReqMsg.get_ItemB()->set_AccSessionId( stUdr.stItemC.get_strAccountSessionID() );
		stAccReqMsg.get_ItemB()->set_CorreId( stUdr.stItemC.get_strCorrelationID() );
		stAccReqMsg.get_ItemC()->set_StartT(0);
		stAccReqMsg.get_ItemC()->set_EndT(0);
		// ����ݾ� = session�� �ѻ��� - ���� ����ݾ�
		stAccReqMsg.get_ItemD()->set_LimitT(ILimitTbl.GetTotalAmt() - nPreviousRate);
		stAccReqMsg.get_ItemD()->set_NTuseT(0);
		ILimitTbl.SetOId(ILimitTbl.GetOId() + 1); // scp�۽� ������ +1 �����ؼ� ������
		stAccReqMsg.get_ItemB()->set_OperId(ILimitTbl.GetOId());//������ OID�� select�ؼ� �־���Ѵ�
		
		// Operation ID update
		if(ILimitTbl.UpdateOId() < 0)
			return -1;
		
		#ifdef	__DEBUG
		*LogM << LogM->ErrorCode(0)<< "<<���� �߰�����޽���>>" << endl;
		*LogM << LogM->ErrorCode(0)<< " ��ȣ      : " << stAccReqMsg.get_ItemA2()->get_Min()  << endl;
		*LogM << LogM->ErrorCode(0)<< " ����ݾ�  : " << stAccReqMsg.get_ItemD()->get_LimitT()<< endl;
		#endif

		SndReqToSCP();
		
		if(ILimitTbl.UpdateInterimCnt() < 0)				// Interim count update
			return -1;
	}	
	
	return 0;
}



//------------------------------------------------------------------------------
// �ѻ�� �ݾ��� �ѵ� �ʰ��ߴ��� Ȯ���� ��,
// �ʰ��ߴٸ� SCP�� ���� Service stop request �޼����� �����, SCP MQ�� �޼����� ����
int CLimitMGR::LimitMGR(int nPreLimitAmt, int nPreviousRate)
{
	// �ѵ� �ʰ� Ȯ�� : ���� �ѵ��ݾ� - (���Ǻ� ���� �� ���ݾ� - ���� ���� �ݾ�)
	if( (nPreLimitAmt - (ILimitTbl.GetTotalAmt() - nPreviousRate) ) <= 0 )
	{
		#ifdef	__DEBUG
			*LogM << LogM->ErrorCode(0) << "�ѵ��ݾ� �ʰ� !!" << endl;
			*LogM << LogM->ErrorCode(0) << "���� �ѵ��ݾ�:["  << nPreLimitAmt			<< "]" << endl;
			*LogM << LogM->ErrorCode(0) << "�ѻ��ݾ�	 :["  << ILimitTbl.GetTotalAmt() << "]" << endl;
			*LogM << LogM->ErrorCode(0) << "��������ݾ� :["  << nPreviousRate			<< "]" << endl;
		#endif
		
		// Service stop request �޼��� ���� ��, SCP�� ����
		memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
		stAccReqMsg.stItemA2.set_Key(0x03);
		stAccReqMsg.stItemA2.set_Min(stUdr.stItemA.get_unMsId() );
		
		SndReqToSCP();

		// Operation ID update
		ILimitTbl.SetOId(ILimitTbl.GetOId() + 1);			// scp�۽� ������ Operation id 1����
		if(ILimitTbl.UpdateOId() < 0)
			return -1;

/*
		// [t_threshold_amt] table�� �ѵ� �ݾ�, ���� �ݾ� update
		// �� ���� �ݾ�
		ILimitTbl.SetRateAmt(ILimitTbl.GetTotalAmt());
		// ���� �� �ѵ� �ݾ�
		ILimitTbl.SetLimitAmt( nPreLimitAmt - (ILimitTbl.GetTotalAmt() - nPreviousRate) );
		if(ILimitTbl.UpdateLimitNRateAmt() < 0)
			return -1;
*/
	}

	return 0;
}	



//------------------------------------------------------------------------------
// STOP UDR�� ��� ���� �� SCP�� ���� Service end request �޼����� �����, SCP MQ�� �޼����� ����
void CLimitMGR::SndServiceEndReq()
{
	memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
	stAccReqMsg.get_ItemA2()->set_Key(0x04);
	stAccReqMsg.get_ItemA2()->set_Min(stUdr.stItemA.get_unMsId() );	// Min
	stAccReqMsg.get_ItemC()->set_StartT(0);
	stAccReqMsg.get_ItemC()->set_EndT(0);
	// ����ݾ� = session�� �ѻ��� - ���� ����ݾ�
	stAccReqMsg.get_ItemD()->set_LimitT(ILimitTbl.GetTotalAmt() - ILimitTbl.GetRateAmt()); //���� �ݾ�
	stAccReqMsg.get_ItemD()->set_NTuseT(0);
	ILimitTbl.SetOId(ILimitTbl.GetOId() + 1); // scp�۽� ������ +1 �����ؼ� ������
	stAccReqMsg.get_ItemB()->set_OperId(ILimitTbl.GetOId());//������ OID�� select�ؼ� �־���Ѵ�
	
	SndReqToSCP();
		
	#ifdef	__DEBUG
	*LogM << LogM->ErrorCode(0)<< "<<Service end req>>" << endl;
	*LogM << LogM->ErrorCode(0)<< " ��ȣ      : "<<stAccReqMsg.get_ItemA2()->get_Min()<< endl;
	*LogM << LogM->ErrorCode(0)<< " ����ݾ�  : "<<stAccReqMsg.get_ItemD()->get_LimitT()<<  endl;
	#endif
}	



//------------------------------------------------------------------------------
// SCP MQ�� �޼����� ����
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
// SCP�κ��� ���� �ѵ���� ��ȸ ���, �߰����� ���ó��
// ���� OID�� �ѵ��ݾ��� UPDATE�Ѵ� 
int CLimitMGR::DoResProcess()
{
	ILimitTbl.SetMin(stUdr.stItemA.get_unMsId());
	ILimitTbl.SetOId(stUdr.stLimitInfo.get_nOperationID());
	ILimitTbl.SetAccSId(stUdr.stItemC.get_strAccountSessionID());
	ILimitTbl.SetCorrId(stUdr.stItemC.get_strCorrelationID());
	ILimitTbl.SetLimitAmt(stUdr.stLimitInfo.get_nLimitPrice());

	if(stUdr.stItemHead.get_nUdrType() == 0x81) // �ѵ������ȸ ���
	{
		#ifdef	__DEBUG
	   		*LogM<< "RECV..�ѵ����� ��ȸ���"<<LogM->ErrorCode(0)<< endl;
		#endif
	}
	else if(stUdr.stItemHead.get_nUdrType() == 0x82) // �߰����� ���
	{
		#ifdef	__DEBUG
	   		*LogM<< "RECV..�߰����� ���"<<LogM->ErrorCode(0)<< endl;
		#endif
	}

	#ifdef	__DEBUG
	*LogM<<"-UDR TYPE: "<<stUdr.stItemHead.get_nUdrType()<<LogM->ErrorCode(0)<< endl;
	*LogM<<"-MIN		: "<<stUdr.stItemA.get_unMsId()<<LogM->ErrorCode(0)<< endl;
	*LogM<<"-ACC_ID	: "<<stUdr.stItemC.get_strAccountSessionID()<<LogM->ErrorCode(0)<< endl;
	*LogM<<"-CORR_ID	: "<<stUdr.stItemC.get_strCorrelationID()<<LogM->ErrorCode(0)<< endl;
	*LogM<<"-OP_ID	: "<<stUdr.stLimitInfo.get_nOperationID()<<LogM->ErrorCode(0)<< endl;
	*LogM<<"-�ѵ��ݾ�: "<<stUdr.stLimitInfo.get_nLimitPrice()<<LogM->ErrorCode(0)<< endl;
	#endif
	
	if(stUdr.stLimitInfo.get_nLimitPrice() <= 0)
	{
		#ifdef	__DEBUG
   		*LogM<< "�ѵ��ݾ��� ����...."<<LogM->ErrorCode(0)<< endl;
		#endif

		memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
		stAccReqMsg.get_ItemA2()->set_Key(0x03);
		stAccReqMsg.get_ItemA2()->set_Min(stUdr.stItemA.get_unMsId() );	// Min

		SndReqToSCP();
	}
	
	// �ѵ� �ݾ� update
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
				WritePMSMQ(FLAG_PROC_KILL);//-1//���μ��� ������ stop (killed)
				exit(-1);
				break;
		case SIGUSR1:
				WritePMSMQ(FLAG_PROC_TERM);//0//���μ�������stop (Client��������)
				exit(0);
				break;
		default:
				WritePMSMQ(FLAG_PROC_KILL);//-1//���μ��� ������ stop (killed)
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

	stRate = new RateRule();			// �־��� ��Ŷ���� ���� �ݾ��� ����ϱ� ���� �ν��Ͻ�
	
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