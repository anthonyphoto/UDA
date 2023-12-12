#include <RecvLogManage.hpp>


//==============================================================================
RecvLogManage::RecvLogManage(void)
{
	perForm	= new ProcPerformData(1, 1, 1, 1, 1, 1);
	performMQ = new MessageQueue<ProcPerformData>(1, 1, 1, 2, 0, 11, 1000);
	
	eventHst = new ProcEventData(1, 1, 1, 1, 1, 1);  
	eventHstMQ = new MessageQueue<ProcEventData>(1, 1, 1, 2, 0, 12, 1000);
    
	errorLog = new ProcErrInfo(1, 1, 1, 1, 1, 1);
	errorLogMQ = new MessageQueue<ProcErrInfo>(1, 1, 1, 2, 0, 13, 1000);
    /*        
	custHst = new CustHist(..);
	custHstMQ = new MessageQueue<CustHist>(1, 1, 1, 2, 0, 10, 1000);
            
	observHst = new ObservHist(..);
	observHstMQ = new MessageQueue<ObservHist>(1, 1, 1, 2, 0, 10, 1000);
	*/
	EmptyN=0;
}



//--------------------------------------------------------------------
RecvLogManage::~RecvLogManage(void)
{
	delete errorLog;
	delete errorLogMQ;
	
	delete eventHst;
	delete eventHstMQ;
	
	delete perForm;
	delete performMQ;
	
	
    /*       
	delete custHst;
	delete custHstMQ;
           
	delete observHst;
	delete observHstMQ;
	*/
}

//--------------------------------------------------------------------
void RecvLogManage::WritePerForm(void)
{
	if(performMQ->recvMsg(perForm)==FLAG_MQ_IS_EMPTY) 
	{
		cout << "[LogWrite] <perforMQ> NO DATA IN MessageQueue" << endl;
		EmptyN++;
		return;
	}
		
	dbPerForm = new ProcPerform(perForm->get_nSystemId(), perForm->get_nSvcId(), perForm->get_nGrpType(),
						perForm->get_nProcType(), perForm->get_nProcSerial(), perForm->get_nProcRemark());
	
	//int	billtype,int fileseq, int udrcount,int querycount, char	*sdate,char *edate);

	dbPerForm->Set_PerForm(perForm->get_nBillDataType(), perForm->get_unFileSeq(), perForm->get_nUdrCount(), 
					perForm->get_nQuery_Count(),perForm->get_strStartDateTime(), perForm->get_strEndDateTime());
	
	delete dbPerForm;
	EmptyN=0;
	
}


//--------------------------------------------------------------------
void RecvLogManage::WriteEventHst(void)
{
	if(eventHstMQ->recvMsg(eventHst)==FLAG_MQ_IS_EMPTY) 
	{
		cout << "[LogWrite] <eventHstMQ> NO DATA IN MessageQueue" << endl;
		EmptyN++;
		return;
	}
		
	dbeventHst = new ProcEventHist(eventHst->get_nSystemId(), eventHst->get_nSvcId(), eventHst->get_nGrpType(),
						eventHst->get_nProcType(), eventHst->get_nProcSerial(), eventHst->get_nProcRemark());
	
	dbeventHst->Set_EventHist(eventHst->get_strProcAlias(), eventHst->get_strStartDateTime(),
					eventHst->get_strEndDateTime(), eventHst->get_nExitCode());
	
	delete dbeventHst;
	EmptyN=0;
}




//--------------------------------------------------------------------
void RecvLogManage::WriteErrorLog(void)
{
	if(errorLogMQ->recvMsg(errorLog)==FLAG_MQ_IS_EMPTY) 
	{
		cout << "[LogWrite] <errorLogMQ> NO DATA IN MessageQueue" << endl;
		EmptyN++;
		return;
	}
		
	if(atoi(errorLog->get_nErrCode())==0)
	{	// FILE Write
		errorLog->write_ErrorLog();
		
	}else
	{
		dberrorLog = new ErrorLog(errorLog->get_nSystemId(), errorLog->get_nSvcId(), errorLog->get_nGrpType(),
						errorLog->get_nProcType(), errorLog->get_nProcSerial(), errorLog->get_nProcRemark());
	
		dberrorLog->Set_ErrorLog(errorLog->get_strEventDate(),errorLog->get_strEventTime(),
					errorLog->get_nErrCode() ,errorLog->get_strFileName(), errorLog->get_strErrDetail());
				
		delete dberrorLog;
	}
	EmptyN=0;
}


/*
//--------------------------------------------------------------------
void RecvLogManage::WriteObservHst(void)
{
	if(observHstMQ->recvMsg(observHst)==FLAG_MQ_IS_EMPTY) 
	{
		cout << "[LogWrite] <observHstMQ> NO DATA IN MessageQueue" << endl;
		return;
	}
		
	dbobservHst = new ObservHist();
	
	//int SetObservHist(int trantype, int trancount, char *sdate, char *edate);
	dbobservHst->SetObservHist();
	
	delete dbobservHst;	
}


//--------------------------------------------------------------------
void RecvLogManage::WriteCustHst(void)
{

	if(custHstMQ->recvMsg(custHst)==FLAG_MQ_IS_EMPTY) 
	{
		cout << "[LogWrite] <custHstMQ> NO DATA IN MessageQueue" << endl;
		return;
	}
		
	dbcustHst = new CustHist();
	
	//int SetCustHist(int svctype, int custtype, char *strdate);
	dbcustHst->SetCustHist();
	
	delete dbcustHst;	
}
*/

//--------------------------------------------------------------------
int RecvLogManage::WriteLog(void)
{
	WritePerForm();
	WriteEventHst();
	WriteErrorLog();
	if(EmptyN==3) sleep(1);
	//WriteObservHst();
	//WriteCustHst();
}



//--------------------------------------------------------------------
int main(int argc, char *argv[])
{
	
	if(argc < 7)
	{
		cout << "Check your arg !!!" << endl;
		exit(-1);
	}
	int				nRet;
	DbConnect		clsDbConnect;
	RecvLogManage	clsLogM;
	

	nRet = clsDbConnect.connect();
	if (nRet < 0)
	{
		cout << "DB Connect Failure [" << nRet << "]" << endl;
		return nRet;
	}else cout << "DB Connect Success " << endl;
	
	while(1) clsLogM.WriteLog();
	
	clsDbConnect.disconnect();
	exit(1);
}

