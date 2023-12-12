#include "CLimitMGR.hpp"


CLimitMGR::CLimitMGR(int nSvcId)
{
/*	// ������� �ѵ������߰������� �ؾ��ϴ��� �о�´�
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
	// �ѵ�������û �޽����� �����
	memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
	stAccReqMsg.stItemA2.set_Key(0x01);								// �����ڵ� 
	stAccReqMsg.stItemA2.set_Min(stUdr.stItemA.get_unMsId() );	// Min
	stAccReqMsg.stItemB.set_OperId(0);								// �޽������� Ű 
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
		<< "�����ѵ����� ���� => �ѵ��ݾ� :["<<stTblMgr.GetLimitAmt()<<"]"<<  endl;
		return;
	}
	#ifdef	_DEBUG
	*LogM << LogM->ErrorCode(0) 
		<< "===== Access Request Data ���� ====" <<  endl;
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
	
	// �ΰ��� table���� ���꿡 �ʿ��� ������ �о�´�. 
	// �ѵ��ܾ�,������Ŷ��뷮, ���ݾ�, ���� �ݾ�, operation ID, nIntrimCnt
	// ��ȸ���� ��ȭ��ȣ, Account Session ID, Correlation ID�� stTblMgr class�� set�Ѵ�
	stTblMgr.SetMin(stUdr.stItemA.get_unMsId());
	stTblMgr.SetAccSId(stUdr.stItemC.get_strAccountSessionID());
	stTblMgr.SetCorrId(stUdr.stItemC.get_strCorrelationID());
	int nRet = stTblMgr.GetLimitInfo();
	
	// ��������ݾ�
	nPreviousRateAmt = stTblMgr.GetRateAmt();
				
	if( nRet == 1403)// session�� ���� ������ �������� �ʴ´�
	{
		#ifdef	_DEBUG
		*LogM << LogM->ErrorCode(0)<<"Start Not Exist...Send Access Request!" <<  endl;
		#endif
		ProcStart();//Start ���� Intrim udr�� recv���� ��
		return; 
	} 	
	// ������ ��Ŷ�� ���: ������� ������Ŷ�� - ���������� ������
	#ifdef	_DEBUG
	*LogM << LogM->ErrorCode(0)<< "<<���� ��Ŷ��>>" << endl;
	*LogM << LogM->ErrorCode(0)<< " ���� ��Ŷ��: "<<stTblMgr.GetLastDataOct()<< endl;
	*LogM << LogM->ErrorCode(0)<< " ���� ��Ŷ��: "<<stUdr.stItemG.get_nDataOctetCntTerminat() + stUdr.stItemG.get_nDataOctetCntOrigin()<<  endl;
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
	// ���� ��Ŷ������ ���ݾ��� ���.
	// ����ϰ� ���� ¥������ �����Ű�� �ʴ´�.
	nRatingPrice = stRate->GetRate(2, 0, 0, 0, nRateDataAmt );
	// FMO :�Ŀ� rating class�����Ǹ� �ٲ� �� 
	//nRatingPrice = stRate->GetRate(2, 0, 0, 0, nRateDataAmt, stUdr.stItemF.get_nForwardMUXOpt());
	// ����ϰ� ���� ¥���� ��Ŷ
	nRemain = stRate->GetLimitSec();
	//��ȸ�� ���������� ��� �ݾ� + ���� ���ݾ� 
	stTblMgr.SetTotalAmt(stTblMgr.GetTotalAmt()+nRatingPrice);
	
	#ifdef	_DEBUG
	*LogM << LogM->ErrorCode(0) 
		<< "============ �ѵ�ó����� ===========" <<  endl;
	*LogM << LogM->ErrorCode(0) 
		<< "���� �ѵ� �ݾ� : " << stTblMgr.GetLimitAmt() <<  endl;
	*LogM << LogM->ErrorCode(0) 
		<< "��� DataOctet : " << nRateDataAmt<<  endl;
	*LogM << LogM->ErrorCode(0) 
		<< "�� ���ݾ� : " << nRatingPrice<<  endl;
	*LogM << LogM->ErrorCode(0) 
		<< "�ѻ��ݾ� : " << stTblMgr.GetTotalAmt()<<  endl;
	*LogM << LogM->ErrorCode(0) 
		<< "������ ������ DataOctet : " << nRemain <<  endl;
	*LogM << LogM->ErrorCode(0) << "���� �ѵ� �ݾ� : " 
			<< stTblMgr.GetLimitAmt() - (stTblMgr.GetTotalAmt() - nPreviousRateAmt)<<  endl;
	#endif
	
	// T_THRESHOLD_AMT
	// Event Time
	stTblMgr.SetEventTime(stUdr.stItemG.get_nEndEventTime()); 
	// Intrim UDR Count
	stTblMgr.SetIntrimCnt(stTblMgr.GetIntrimCnt() + 1);
	// T_THRESHOLD_MID_AMT
	stTblMgr.SetCurrentAmt(nRatingPrice + stTblMgr.GetCurrentAmt());	// ������� ����� �ݾ�
	stTblMgr.SetLastDataOct(stUdr.stItemG.get_nDataOctetCntTerminat()	// �������ݷ� ��� �� 
							+ stUdr.stItemG.get_nDataOctetCntOrigin());	// �ʿ�
	
	// Session�� ���� ���� update
	if( stTblMgr.UpdateLimitInfo1() < 0)
	{
		#ifdef	_DEBUG
		*LogM << LogM->ErrorCode(0)<<"Limit Info Update fail...." <<  endl;
		#endif
	}
	
	// �ѵ��ݾ��� �Ѿ��� ��� : �ѵ��ݾ� - session�� ���� ���� ����(����ݾ�) 
	if( (stTblMgr.GetLimitAmt() - (stTblMgr.GetTotalAmt() - nPreviousRateAmt) ) <= 0 )
	{
		#ifdef	_DEBUG
			*LogM << LogM->ErrorCode(0)<< "�ѵ��ݾ� �ʰ� !!" << endl;
			*LogM << LogM->ErrorCode(0)<< "���� �ѵ��ݾ�	:["<<stTblMgr.GetLimitAmt()<<"]" << endl;
			*LogM << LogM->ErrorCode(0)<< "�ѻ��ݾ�	:["<<stTblMgr.GetTotalAmt()<<"]" << endl;
			*LogM << LogM->ErrorCode(0)<< "��������ݾ�	:["<<nPreviousRateAmt<<"]" << endl;
		#endif
		memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
		stAccReqMsg.stItemA2.set_Key(0x03);
		stAccReqMsg.stItemA2.set_Min(stUdr.stItemA.get_unMsId() );	// Min
		
		SndReqToSCP();
		//�ش� ��ȣ�� ���� �ѵ� ������ �����Ѵ�
		stTblMgr.DeleteAllLimitInfo();
	}
		
	// SCP �߰����꺸���� 
	if((stTblMgr.GetIntrimCnt() % gnAccReqCnt) == 0)
	{
		#ifdef	_DEBUG
			*LogM << LogM->ErrorCode(0)<< "send intrim Account Request !!" << endl;
		#endif
		
		// T_THRESHOLD_AMT �ѵ��ݾ� �� ����ݾ� Update
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
		// ����ݾ� = session�� �ѻ��� - ���� ����ݾ�
		stAccReqMsg.get_ItemD()->set_LimitT(stTblMgr.GetTotalAmt() - nPreviousRateAmt); //���� �ݾ�
		stAccReqMsg.get_ItemD()->set_NTuseT(0);
		stTblMgr.SetOId(stTblMgr.GetOId() + 1); // scp�۽� ������ +1 �����ؼ� ������
		stAccReqMsg.get_ItemB()->set_OperId(stTblMgr.GetOId());//������ OID�� select�ؼ� �־���Ѵ�
		
		if(stTblMgr.UpdateOId() < 0)
		{
			#ifdef	_DEBUG
			*LogM << LogM->ErrorCode(0)<<"Limit Info Update fail...." <<  endl;
			#endif
		}
		
		#ifdef	_DEBUG
		*LogM << LogM->ErrorCode(0)<< "<<���� �߰�����޽���>>" << endl;
		*LogM << LogM->ErrorCode(0)<< " ��ȣ      : "<<stAccReqMsg.get_ItemA2()->get_Min()<< endl;
		*LogM << LogM->ErrorCode(0)<< " ����ݾ�   : "<<stAccReqMsg.get_ItemD()->get_LimitT()<<  endl;
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
	
	// �ΰ��� table���� ���꿡 �ʿ��� ������ �о�´�. 
	// �ѵ��ܾ�,������Ŷ��뷮, ���ݾ�, ���� �ݾ�, operation ID, nIntrimCnt
	// ��ȸ���� ��ȭ��ȣ, Account Session ID, Correlation ID�� stTblMgr class�� set�Ѵ�
	stTblMgr.SetMin(stUdr.stItemA.get_unMsId());
	stTblMgr.SetAccSId(stUdr.stItemC.get_strAccountSessionID());
	stTblMgr.SetCorrId(stUdr.stItemC.get_strCorrelationID());
	int nRet = stTblMgr.GetLimitInfo();
	
	// ��������ݾ�
	nPreviousRateAmt = stTblMgr.GetRateAmt();
	
	if( nRet == 1403)// session�� ���� ������ �������� �ʴ´�
	{
		#ifdef	_DEBUG
		*LogM << LogM->ErrorCode(0)<<"Previus Data Not Exist...Send End Request!" <<  endl;
		#endif
	}

	// ������ ��Ŷ�� ���: ������� ������Ŷ�� - ���������� ������
	nRateDataAmt = (stUdr.stItemG.get_nDataOctetCntTerminat() + stUdr.stItemG.get_nDataOctetCntOrigin())
					- stTblMgr.GetLastDataOct();
	if( nRateDataAmt <= 0 )
	{
			#ifdef	_DEBUG
			*LogM << LogM->ErrorCode(0)<< "Small Rating Data Octet!!" << endl;
			#endif
			return ;
	}
	// �Ի�� ��Ŷ������ ���ݾ��� ���.
	// ����ϰ� ���� ¥������ �����Ű�� �ʴ´�.
	nRatingPrice = stRate->GetRate(2, 0, 0, 0, nRateDataAmt );
	// FMO :�Ŀ� rating class�����Ǹ� �ٲ� �� 
	//nRatingPrice = stRate->GetRate(2, 0, 0, 0, nRateDataAmt, stUdr.stItemF.get_nForwardMUXOpt());
	// ����ϰ� ���� ¥���� ��Ŷ
	nRemain = stRate->GetLimitSec();
	//��ȸ�� ���������� ��� �ݾ� + ���� ���ݾ� 
	stTblMgr.SetTotalAmt(stTblMgr.GetTotalAmt()+nRatingPrice);
	
	#ifdef	_DEBUG
	*LogM << LogM->ErrorCode(0) 
		<< "============ �ѵ�ó����� ===========" <<  endl;
	*LogM << LogM->ErrorCode(0) 
		<< "���� �ѵ� �ݾ� : " << stTblMgr.GetLimitAmt() <<  endl;
	*LogM << LogM->ErrorCode(0) 
		<< "��� DataOctet : " << nRateDataAmt<<  endl;
	*LogM << LogM->ErrorCode(0) 
		<< "���ݾ� : " << nRatingPrice<<endl;
	*LogM << LogM->ErrorCode(0) 
		<< "�ѻ��ݾ� : " << stTblMgr.GetTotalAmt()<<  endl;
	*LogM << LogM->ErrorCode(0) 
		<< "������ ������ DataOctet : " << nRemain <<  endl;
	*LogM << LogM->ErrorCode(0) << "���� �ѵ� �ݾ� : " 
			<< stTblMgr.GetLimitAmt() - (stTblMgr.GetTotalAmt() - nPreviousRateAmt)<<  endl;
	#endif
	
	stTblMgr.SetEventTime(stUdr.stItemG.get_nEndEventTime());

	if( (stTblMgr.GetLimitAmt() - (stTblMgr.GetTotalAmt() - nPreviousRateAmt ) ) <= 0 )
	{
		#ifdef	_DEBUG
		*LogM << LogM->ErrorCode(0)<< "�ѵ��ݾ� �ʰ� !!" << endl;
		*LogM << LogM->ErrorCode(0)<< "���� �ѵ��ݾ�	:["<<stTblMgr.GetLimitAmt()<<"]" << endl;
		*LogM << LogM->ErrorCode(0)<< "�ѻ��ݾ�	:["<<stTblMgr.GetTotalAmt()<<"]" << endl;
		*LogM << LogM->ErrorCode(0)<< "��������ݾ�	:["<<nPreviousRateAmt<<"]" << endl;
		#endif
		
		memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
		stAccReqMsg.get_ItemA2()->set_Key(0x03);
		stAccReqMsg.get_ItemA2()->set_Min(stUdr.stItemA.get_unMsId() );	// Min
		SndReqToSCP();
		//�ش� ��ȣ�� ���� �ѵ� ������ �����Ѵ�
		stTblMgr.DeleteAllLimitInfo();
	}

	memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
	stAccReqMsg.get_ItemA2()->set_Key(0x04);
	stAccReqMsg.get_ItemA2()->set_Min(stUdr.stItemA.get_unMsId() );	// Min
	stAccReqMsg.get_ItemC()->set_StartT(0);
	stAccReqMsg.get_ItemC()->set_EndT(0);
	// ����ݾ� = session�� �ѻ��� - ���� ����ݾ�
	stAccReqMsg.get_ItemD()->set_LimitT(stTblMgr.GetTotalAmt() - stTblMgr.GetRateAmt()); //���� �ݾ�
	stAccReqMsg.get_ItemD()->set_NTuseT(0);
	stTblMgr.SetOId(stTblMgr.GetOId() + 1); // scp�۽� ������ +1 �����ؼ� ������
	stAccReqMsg.get_ItemB()->set_OperId(stTblMgr.GetOId());//������ OID�� select�ؼ� �־���Ѵ�
	
	if(stTblMgr.UpdateOId() < 0)
	{
		#ifdef	_DEBUG
		*LogM << LogM->ErrorCode(0)<<"Limit Info Update fail...." <<  endl;
		#endif
	}
	
	SndReqToSCP();
	
	#ifdef	_DEBUG
	*LogM << LogM->ErrorCode(0)<< "<<���� �߰�����޽���(STOP UDR)>>" << endl;
	*LogM << LogM->ErrorCode(0)<< " ��ȣ      : "<<stAccReqMsg.get_ItemA2()->get_Min()<< endl;
	*LogM << LogM->ErrorCode(0)<< " ����ݾ�   : "<<stAccReqMsg.get_ItemD()->get_LimitT()<<  endl;
	#endif

	// T_THRESHOLD_MID_AMT
	stTblMgr.SetCurrentAmt(nRatingPrice + stTblMgr.GetCurrentAmt());	// ������� ����� �ݾ�
	stTblMgr.SetLastDataOct(stUdr.stItemG.get_nDataOctetCntTerminat()	// �������ݷ� ��� �� 
							+ stUdr.stItemG.get_nDataOctetCntOrigin());	// �ʿ�

	// T_THRESHOLD_AMT �ѵ��ݾ� �� ����ݾ� Update
	if(stTblMgr.UpdateLimitInfo1() < 0)
	{
		#ifdef	_DEBUG
		*LogM << LogM->ErrorCode(0)<<"Limt And Rate Amt Update fail...." <<  endl;
		#endif
	}

	// T_THRESHOLD_AMT �ѵ��ݾ� �� ����ݾ� Update
	stTblMgr.SetRateAmt(nPreviousRateAmt+stAccReqMsg.get_ItemD()->get_LimitT());
	stTblMgr.SetLimitAmt( stTblMgr.GetLimitAmt() - (stTblMgr.GetTotalAmt()-nPreviousRateAmt) );
	if(stTblMgr.UpdateLimitNRateAmt() < 0)
	{
		#ifdef	_DEBUG
		*LogM << LogM->ErrorCode(0)<<"Limt And Rate Amt Update fail...." <<  endl;
		#endif
	}

}


// scp�� ����
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


// SCP�κ��� ���� �ѵ���� ��ȸ ���, �߰����� ���ó��
// ���� OID�� �ѵ��ݾ��� UPDATE�Ѵ� 
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
	*LogM<<"�ѵ��ݾ�	: "<<stUdr.stLimitInfo.get_nLimitPrice()<<LogM->ErrorCode(0)<< endl;
	#endif
	
	if(stUdr.stItemHead.get_nUdrType() == 0x81) // �ѵ������ȸ ���
	{
		#ifdef	_DEBUG
	   		*LogM<< "RECV..�ѵ����� ��ȸ���"<<LogM->ErrorCode(0)<< endl;
		#endif
	}
	else if(stUdr.stItemHead.get_nUdrType() == 0x82) // �߰����� ���
	{
		#ifdef	_DEBUG
	   		*LogM<< "RECV..�߰����� ���"<<LogM->ErrorCode(0)<< endl;
		#endif
	}
	
	if(stUdr.stLimitInfo.get_nLimitPrice()<= 0)
	{
		#ifdef	_DEBUG
   		*LogM<< "�ѵ��ݾ��� ����...."<<LogM->ErrorCode(0)<< endl;
		#endif
		memset( ( char* )&stAccReqMsg, NULL, sizeof( SCPFormatAgg ) );
		stAccReqMsg.get_ItemA2()->set_Key(0x03);
		stAccReqMsg.get_ItemA2()->set_Min(stUdr.stItemA.get_unMsId() );	// Min
		SndReqToSCP();
		//�ش� ��ȣ�� ���� �ѵ� ������ �����Ѵ�
		stTblMgr.DeleteAllLimitInfo();
	}
	
	if(stUdr.stLimitInfo.get_nOperationID()  >= stTblMgr.SelectOId())
			stTblMgr.UpdateLimitAmt();  // �ѵ���û ����� ���� �ݾ��� insert
	return;
	#ifdef	_DEBUG
   		*LogM<< "�ѵ����� Not Update...Operation ID small..."<<LogM->ErrorCode(0)<< endl;
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
				WritePMSMQ(FLAG_PROC_KILL);//-1//���μ��� ������ stop (killed)
				delete mqPMS;
				exit(-1);
				break;
		case SIGUSR1:
				WritePMSMQ(FLAG_PROC_TERM);//0//���μ�������stop (Client��������)
				delete mqPMS;
				exit(0);
				break;
		default:
				WritePMSMQ(FLAG_PROC_KILL);//-1//���μ��� ������ stop (killed)
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




