/******************************************************************
* 파 일 명     
      : CollectorMain.cpp                
* 기   능                                                          
     : MQ로 들어온 Aggregation된 UDR을 임시 파일에 먼저 저장한다.
       그런 다음, 임시 파일내 각 UDR을 BDS 형식에 맞게 변환한다.
       변환된 UDR에 대해 File, Block Header를 붙인 다음 BDS File을 생성한다.
* 작 성 자     
     : 신미연                         
* first data  
     : 2000. 10.15                      
* last updata 
     : 2000. 12. 06                  
* 관 련 화 일  
     : 
* program history                                
                           
* Version
  : 1.1 ( 2001. 05. 21 )                                           
******************************************************************/


#include <time.h>

#include <BlockFile.hpp>
//#include <ConfigFile.hpp>
#include <time.hpp>
#include <StatusFile.hpp>
#include <MsgFormat.hpp>
#include <SendLogManage.hpp>
//#include <TempMsgQ_new.hpp>
#include <TempMsgQ_file.hpp>

int	SystemId;
int	ServiceId;
int	GroupId;
int	ProcessType;
int PserialNo;
int	PreMark;

//	PMS MQ
MessageQueue<MsgPmsStatus>* mqPMS;
MessageQueue<UdrFormat>*	MqUdr;

//	Log Manager
SendLogManage*				clsLogM;

void sigCapture(int sigNo);
void WritePMSMQ(int sigNo);
int  writeTempFile(CBlockData& clsBlockData, CFileData&	clsFiledata);
int writeBdsFile(CBlockData& clsBlockData, CFileData& clsFiledata);

TempUdrFile		clsTempFile(sizeof(UdrFormat));

int 			nBdsRecCount = 0;

char FILE_NAME_COL_BDS_UDR[50];
char PATH_NAME_BDS_BLOCK_CFG[50];
char PATH_NAME_BDS_FILE_CFG[50];


int main(int argc, char* argv[])
{

	if (argc < 7)
	{
		*clsLogM 	<< "Error No = [" << clsLogM->ErrorCode(6101) << "]"
					<< "Invalid Argument.." << endl;
 		clsLogM->put_EventHistData(FLAG_END, -1);
 		exit(-1);
 	}

	//	Allocate the SendLogManage
	clsLogM		= new SendLogManage		(	SystemId   	=	atoi(argv[1]),
											ServiceId	=	atoi(argv[2]),
											GroupId		=	atoi(argv[3]),
											ProcessType	=	atoi(argv[4]),
											PserialNo	=	atoi(argv[5]),
											PreMark		=	atoi(argv[6])
										);

	sprintf(FILE_NAME_COL_BDS_UDR  ,	"/SVC%d/BDS/BDS_UDR."		   ,	ServiceId);
	sprintf(PATH_NAME_BDS_BLOCK_CFG,	"/SVC%d/DATA/BDS_BlockInfo.cfg",	ServiceId);
	sprintf(PATH_NAME_BDS_FILE_CFG ,	"/SVC%d/DATA/BDS_FileConf.cfg" ,	ServiceId);

	*clsLogM 	<< clsLogM->ErrorCode(0) << "Collector is ready !!" << endl;
	clsLogM->put_EventHistData(FLAG_START, FLAG_PROC_START);

	//	signal(SIGNULL,sigCapture);	//0
	signal(SIGHUP,sigCapture);
	signal(SIGINT,sigCapture);
	signal(SIGQUIT,sigCapture);
	signal(SIGKILL,sigCapture);
	signal(SIGTERM,sigCapture);
	signal(SIGSTOP,sigCapture);
	signal(SIGTSTP,sigCapture);
	signal(SIGCONT,sigCapture);		//26
	signal(SIGUSR1,sigCapture);

	mqPMS 		= new MessageQueue<MsgPmsStatus>(	SystemId,
													0,
													1,
													1,
													0,
													10,
													1000);

	MqUdr		= new MessageQueue<UdrFormat>(	SystemId	,
												ServiceId	,
												GroupId		,
												ProcessType	,
												PserialNo	,
												10		,
												1000
											);


	//	Declare Variable
	CBlockData		clsBlockData(PATH_NAME_BDS_BLOCK_CFG);
	CFileData		clsFiledata(PATH_NAME_BDS_FILE_CFG);

	//	Notify the Process status To PMS
	WritePMSMQ(FLAG_PROC_START);//-1//프로세스 정상 start




	while(1)
	{
		//	Update Performance Data (Start Transaction)
		clsLogM->put_PerformanceData(FLAG_START, 1, clsFiledata.get_unFileSeq(), 0, 0, 0);

		//	Write Temp File
		if (writeTempFile(clsBlockData, clsFiledata) < 0)
		{
			//	Error Log
			break;
		}

		//	Write BDS File
		if (writeBdsFile(clsBlockData, clsFiledata) < 0)
		{
			//	Error Log
			break;
		}

		//	Update Performance Data (End Transaction)
		clsLogM->put_PerformanceData(FLAG_END,   1, clsFiledata.get_unFileSeq(), nBdsRecCount, 0, 0);
		nBdsRecCount = 0;
	}

}

int writeTempFile(CBlockData& clsBlockData, CFileData&	clsFiledata)
{
	UdrFormat		Udr;
	time_t			tTimeNow, tLastWritedTime = 0;	//	Declare Time Variable
	int				nRecordCount = 0,
					nMaxRecCount = 0;

	char			strTempNameBuf[64];
	
	sprintf( strTempNameBuf, "/SVC%d/BDS/UDR%d.TEMP", ServiceId, clsFiledata.get_unFileSeqInConf());

	ifstream	TempConfigFile(strTempNameBuf, ios::binary);
	
	//	Temp File이 존재하면 이전에 비정상 종료인 것으로 보고 
	//	Temp File을 BDS UDR File로 Write하고 이후 정상 처리한다.
	if (TempConfigFile)
	{
		*clsLogM	<< clsLogM->ErrorCode(0)
					<< "Temp FileSeq [" << clsFiledata.get_unFileSeqInConf()
					<< "] write to File Now....." << endl;
		clsTempFile.put_strFileName(strTempNameBuf);
		TempConfigFile.close();
		
		return 0;
	}

	clsTempFile.put_strFileName(strTempNameBuf);

	//	Get Max Record Count
	nMaxRecCount = 	clsFiledata.get_nMaxBlockCount() *
					clsBlockData.get_nMaxRecCount();

	tLastWritedTime = time(NULL);	// Last Writeing Time Initialize.

	while(1)
	{
		tTimeNow = time(NULL);	// Current Time Setting.

		memset ((char*)&Udr, NULL, sizeof(UdrFormat));

		if (	(((tTimeNow - tLastWritedTime) > 120) ||
//		if (	(((tTimeNow - tLastWritedTime) > 30) ||
				(nRecordCount >= nMaxRecCount)) && (nRecordCount != 0)	)
		{
					////////////////////////////////////////////////////////////////
					*clsLogM	<< clsLogM->ErrorCode(0)
								<< "Temp FileSeq [" 		<< clsFiledata.get_unFileSeqInConf() << "] is complete."
								<< "\n"
								<< "Temp File Record Count = " 	<< nRecordCount
								<< ",	Time Limit = "		<< (tTimeNow - tLastWritedTime) << endl;
					////////////////////////////////////////////////////////////////
					break;
		}

		if (MqUdr->recvMsg(&Udr) != FLAG_MQ_IS_EMPTY)
		{
			if (Udr.get_ItemHead() == NULL)
			{
				////////////////////////////////////////////////////////////////
				*clsLogM	<< clsLogM->ErrorCode(0)
							<< "Garbage Data is received, so continue" << endl;
				////////////////////////////////////////////////////////////////
				continue;
			}

			//	Write the Temp File
			clsTempFile.write_TempUdrFile(&Udr);

			// Last Writeing Time Resetting.

			tLastWritedTime = time(NULL);

			nRecordCount++;
			if(nRecordCount == 1)
			{
				////////////////////////////////////////////////////////////////
				*clsLogM 	<< clsLogM->ErrorCode(0)
							<< "======Temp FileSeq [" << clsFiledata.get_unFileSeqInConf()
							<< "] Writing Now=====" << endl;
				////////////////////////////////////////////////////////////////
			}
		}
		else
			usleep(100);
	}	//end while

	return 0;
}

int writeBdsFile(CBlockData& clsBlockData, CFileData& clsFiledata)
{
	CBDS_UdrFormat	Udr_Bds;
	UdrFormat		Udr;
	int				nRet = 0;
	int				nCompleteCode = 0;

	//	Open Temp File
	ifstream		UdrTempFile(clsTempFile.get_strFileName(), ios::in | ios::binary);

	if (!UdrTempFile)
	{
		////////////////////////////////////////////////////////////////
		*clsLogM 	<< clsLogM->ErrorCode(6102)
					<< "Err Code#" << "\tUDR Temp File [" << clsTempFile.get_strFileName()
					<< "] couldn't open in writeBdsFile" << endl;
		////////////////////////////////////////////////////////////////
		return -1;
	}

	//	Get Udr Count from Temp File
	clsTempFile.get_nUdrCountFromFile();
	
	//	Make the BdsUdrFileName
	clsFiledata.make_BdsUdrFileName(clsFiledata.get_unFileSeqInConf());
	
	////////////////////////////////////////////////////////////////
	*clsLogM 	<< clsLogM->ErrorCode(0)<< "======BDS UDR FileSeq [" << clsFiledata.get_unFileSeqInConf()
				<< "] Making Block Now=====" << "\n"
				<< "Udr Count in Temp File = "		 << clsTempFile.get_nUdrCount()
				<< endl;
	////////////////////////////////////////////////////////////////

	int i = 0;

	while(1)
	{
		i++;

		memset ((char*)&Udr, NULL, sizeof(Udr));

		//	Read UDR Temp File
		UdrTempFile.read(reinterpret_cast<char*>(&Udr), sizeof(UdrFormat));

		//	Conversion UDR to BDS Format
		Udr_Bds.conv_BDS_UDR(Udr);

		nBdsRecCount++;

		//	put UDR to Block
		nRet = clsBlockData.put_strBdsBlock(Udr_Bds.get_strBdsUdr());

		if( i >= clsTempFile.get_nUdrCount() )
			nCompleteCode = 1;

		if (nRet == FLAG_BLOCK_NO_MADE)
		{
			if (nCompleteCode == 1)
			{
				//	Fill Dummy Record
				nRet = clsBlockData.put_strBdsBlock();
				////////////////////////////////////////////////////////////////
				*clsLogM 	<< clsLogM->ErrorCode(0)
							<< "BDS UDR File is on writing Block Seq #"
							<< clsBlockData.get_unBlockSeq() << endl;
				////////////////////////////////////////////////////////////////
			}
			else
				continue;
		}

		if (nRet == FLAG_BLOCK_MADE)	//	if a UDR Block is made.
		{
			//	write UDR to BDS UDR File.
			nRet = clsFiledata.put_strBdsFileBuffer(clsBlockData.get_unBlockSeq(),
													clsBlockData.get_strOneBlock());

			if ( 	(nRet == FLAG_FILE_NO_MADE) && (nCompleteCode != 1) )
			{
				////////////////////////////////////////////////////////////////
				*clsLogM 	<< clsLogM->ErrorCode(0)
							<< "BDS UDR File is on writing Block Seq #"
							<< clsBlockData.get_unBlockSeq() << endl;
				////////////////////////////////////////////////////////////////
				continue;
			}
			else if ( 	(nCompleteCode == 1) || (nRet == FLAG_FILE_MADE)	)
			{
				//	Initialize Total of Record Count in Data File
				clsBlockData.put_unTotalRecCount(0);

				//	Update Performance Data (End Transaction)
//				clsLogM->put_PerformanceData(	FLAG_END,   1,
//												clsFiledata.get_unFileSeq(),
//												clsTempFile.get_nUdrCountFromFile(),
//												0, 0);
				break;
			}
			else
			{
				*clsLogM 	<< clsLogM->ErrorCode(0)
							<< "Error No = [" << "]"
							<< "Error is occured, so exit !!" << endl;
				break;
			}
		}
	}	//end of while

	//	Write BDS UDR Data
	if ( (nRet = clsFiledata.put_strBdsFile()) < 0 )
	{
		////////////////////////////////////////////////////////////////
		*clsLogM 	<< clsLogM->ErrorCode(6103)
					<< "ERROR ====> UDR File#" << clsFiledata.get_unFileSeq()
					<< " couldn't writed !!" << endl;
		////////////////////////////////////////////////////////////////
		return -1;
	}

	//	Update : Last Block Seq & File Seq
	clsBlockData.write_unBlockSeqInConf();
	clsFiledata.write_unFileSeqInConf(clsFiledata.get_unFileSeqInConf());

	//	unlink	Tempfile
	if (clsTempFile.unlink_TempUdrFile() < 0)
	{
		////////////////////////////////////////////////////////////////
		*clsLogM 	<< clsLogM->ErrorCode(0)	<< "UDR Temp File UNLINK FAIL !!!" << endl;
		////////////////////////////////////////////////////////////////;
	}

	UdrTempFile.close();

	////////////////////////////////////////////////////////////////
	*clsLogM 	<< clsLogM->ErrorCode(0)
				<< "SUCCESS ====> BDS UDR FileSeq[" << clsFiledata.get_unFileSeq()
				<< "] is writed !!" << endl;
	////////////////////////////////////////////////////////////////

	return 0;
}


//==============================================================================
void WritePMSMQ(int sigNo )
{
	int 			i=0;
	char 			PMSMQ_NO[2];
	MsgPmsStatus	OneRec	(	MSG_STATUS,
								SystemId,
								ServiceId,
								GroupId,
								ProcessType,
								PserialNo,
								PreMark
							);

	sprintf(PMSMQ_NO, "%d", SystemId);
	OneRec.set_nStatus(sigNo);

	while(1)
	{
		i++;
		if (mqPMS->sendMsg(&OneRec) == FLAG_MQ_IS_FULL)
		{
			*clsLogM << "Error No = [" << clsLogM->ErrorCode(6104) << "]"
					<< "[ERROR] PMSMQ " << PMSMQ_NO << " IS FULL usleep(100)...!"	<< endl;
			usleep(100);
			if (i >= 10)
			{
				kill(0,SIGINT);
			}
		}else break;
	}
}

//==============================================================================
void sigCapture(int sigNo )
{
	*clsLogM << "Error No = [" << clsLogM->ErrorCode(6105) << "]"
			<< "SIGNAL :: Collector is got sigint = [" << sigNo << "]"
			<< PserialNo << "Process End " << endl;

	sleep(1);
	switch(sigNo)
	{
		case SIGINT :
		case SIGQUIT :
		case SIGKILL :
		case SIGTERM :
				WritePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				clsLogM->put_EventHistData(FLAG_END, -1);
				delete	mqPMS;
				delete	clsLogM;
				exit(-1);
				break;
		case SIGUSR1:
				WritePMSMQ(FLAG_PROC_TERM);//0//프로세스정상stop (Client업무명령)
				clsLogM->put_EventHistData(FLAG_END, 0);
				delete mqPMS;
				exit(0);
				break;
		default:
				WritePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				clsLogM->put_EventHistData(FLAG_END, -1);
				delete	mqPMS;
				delete	clsLogM;
				exit(-1);
				break;
	}
}



