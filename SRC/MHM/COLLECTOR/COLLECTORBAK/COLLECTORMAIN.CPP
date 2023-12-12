#include <time.h>

#include <BlockFile.hpp>
#include <ConfigFile.hpp>
#include <time.hpp>
#include <StatusFile.hpp>
#include <MsgFormat.hpp>
#include <SendLogManage.hpp>
#include <TempMsgQ_new.hpp>

int	SystemId;
int	ServiceId;
int	GroupId;
int	ProcessType;
int PserialNo;
int	PreMark;

//	PMS MQ
MessageQueue<MsgPmsStatus>* mqPMS;

//	Log Manager
SendLogManage*				clsLogM;

void sigCapture(int sigNo);
void WritePMSMQ(int sigNo);
void checkSeqNo(CBlockData& clsBlock, CFileData& clsFile);

int main(int argc, char* argv[])
{
	if (argc < 7)
	{
//		*clsLogM 	<< "Error No = [" << clsLogM->ErrorCode(6102) << "]"
//					<< "Invalid Argument.." << endl;
// 		clsLogM->put_EventHistData(FLAG_END, -1);
 		exit(-1);
 	}

	clsLogM		= new SendLogManage		(	SystemId   	=	atoi(argv[1]),
											ServiceId	=	atoi(argv[2]),
											GroupId		=	atoi(argv[3]),
											ProcessType	=	atoi(argv[4]),
											PserialNo	=	atoi(argv[5]),
											PreMark		=	atoi(argv[6])
										);

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

	UdrFormat		Udr;
	CBDS_UdrFormat	Udr_Bds;
	CBlockData		clsBlockData(PATH_NAME_BDS_BLOCK_CFG);
	CFileData		clsFiledata(PATH_NAME_BDS_FILE_CFG);
	CFileInfoData	clsFileInfo(PATH_NAME_BDS_FILE_INFO);
	int				nRet = 0;
	time_t			tTimeNow, tLastWritedTime = 0;

	mqPMS 		= new MessageQueue<MsgPmsStatus>(	SystemId,
													0,
													1,
													1,
													0,
													10,
													1000);

	MessageQueue<UdrFormat>		MqUdr	(	SystemId	,
											ServiceId	,
											GroupId		,
											ProcessType	,
											PserialNo	,
											10		,
											1000
										);


	WritePMSMQ(FLAG_PROC_START);//-1//프로세스 정상 start

	//	check Current Block Seq
	
	*clsLogM 	<< clsLogM->ErrorCode(0) << "checkSeqNo before !!" << endl;	
	
	checkSeqNo(clsBlockData, clsFiledata);

	*clsLogM 	<< clsLogM->ErrorCode(0) << "checkSeqNo after !!" << endl;

	tLastWritedTime = time(NULL);	// Last Writeing Time Initialize.

	
	*clsLogM 	<< clsLogM->ErrorCode(0) << "While ready !!" << endl;

	while(1)
	{
		tTimeNow = time(NULL);	// Current Time Setting.

		memset ((char*)&Udr, NULL, sizeof(UdrFormat));

			*clsLogM 	<< clsLogM->ErrorCode(0) << "While after !!" << endl;


		if (MqUdr.recvMsg(&Udr) == FLAG_MQ_IS_EMPTY)
		{
			sleep (1);
			//////////////////////////////////////////////
			//
			//	한도 시간 초과인 경우 처리 루틴 : 시작
			//
			//////////////////////////////////////////////
//			if ((tTimeNow - tLastWritedTime) > 300)
			if ((tTimeNow - tLastWritedTime) > 120)
			{
				//	UDR 파일 생성 한도 시간이 초과할 경우
				//	현재까지 버퍼내용을 파일에 Write한다.
				switch(clsBlockData.put_strBdsBlock())
				{
					case	FLAG_BLOCK_MADE:
						//	write UDR to BDS UDR File.
						////////////////////////////////////////////////////////////////
						*clsLogM << clsLogM->ErrorCode(0)
								<< "UDR File(1-1) is on writing Block Seq #"
								<< clsBlockData.get_unBlockSeq() << endl;
						////////////////////////////////////////////////////////////////
						nRet = clsFiledata.put_strBdsFile(	clsBlockData.get_unBlockSeq(),
															clsBlockData.get_strOneBlock());
					//	cout << "clsBlockData.get_unBlockSeq() = " << clsBlockData.get_unBlockSeq() << endl;
						if (nRet == FLAG_FILE_NO_MADE)
						{
							while(1)
							{
								clsBlockData.put_strBdsBlock();

								////////////////////////////////////////////////////////////////
								*clsLogM << clsLogM->ErrorCode(0)
										 << "UDR File(1) is on writing Block Seq #"
										 << clsBlockData.get_unBlockSeq() << endl;
								////////////////////////////////////////////////////////////////

								nRet = clsFiledata.put_strBdsFile(	clsBlockData.get_unBlockSeq(),
																	clsBlockData.get_strOneBlock());
								if (nRet == FLAG_FILE_NO_MADE)
									continue;
								else
								{
									tLastWritedTime = time(NULL);	// Last Writeing Time Resetting.

									//	Total of Record Count in Data File
									clsBlockData.put_unTotalRecCount(0);

									break;
								}
							}
						}

						tLastWritedTime = time(NULL);	// Last Writeing Time Resetting.
						clsFileInfo.put_FileInfoData (clsFiledata.get_clsFileHeader(),\
													  FLAG_FILE_MADE);
						////////////////////////////////////////////////////////////////
						*clsLogM << clsLogM->ErrorCode(0)
								<< "UDR File#"<< clsFiledata.get_unFileSeq()
								<< " is writed !!" << endl;
						////////////////////////////////////////////////////////////////

						break;

					case	FLAG_EMPTY_BLOCK_MADE:
						//	버퍼 내용이 없을 경우에는 FileInfo.log에 상태값(EMPTY_DATA)를 WRITE한다.
						//	2000. 9. 29 EMPTY_DATA일때는 FileInfo.log에 업데이트하지 않는다.
						//	clsFileInfo.put_FileInfoData (clsFiledata.get_clsFileHeader(),\
						//								  FLAG_EMPTY_FILE_MADE);
						tLastWritedTime = time(NULL);	// Last Writeing Time Resetting.
						break;
					default:
						cout << "[ ]" << endl;
				}
			}

			continue;
			//////////////////////////////////////////////
			//
			//	한도 시간 초과인 경우 처리 루틴 : 끝
			//
			//////////////////////////////////////////////
		}
		if (Udr_Bds.get_strBdsUdr() == NULL)
		{
			////////////////////////////////////////////////////////////////
			*clsLogM << clsLogM->ErrorCode(0)
					<< "Garbage Data is received, so continue" << endl;
			////////////////////////////////////////////////////////////////

			continue;
		}
		else
		{
			tTimeNow = time(NULL);	// Current Time Setting.

			Udr_Bds.conv_BDS_UDR(Udr);		//	convert UDR to BDS Format

			nRet = clsBlockData.put_strBdsBlock(Udr_Bds.get_strBdsUdr());

			if (nRet == FLAG_BLOCK_NO_MADE)	//	put UDR
				continue;

			if (nRet == FLAG_BLOCK_MADE)	//	if a UDR Block is made.
			{
				//	write UDR to BDS UDR File.
				nRet = clsFiledata.put_strBdsFile(	clsBlockData.get_unBlockSeq(),
													clsBlockData.get_strOneBlock());
				if 		(nRet == FLAG_FILE_NO_MADE)
				{
					////////////////////////////////////////////////////////////////
					*clsLogM << clsLogM->ErrorCode(0)
							<< "UDR File(2) is on writing Block Seq #"
							<< clsBlockData.get_unBlockSeq() << endl;
					////////////////////////////////////////////////////////////////
				}
				else if (nRet == FLAG_FILE_MADE)
				{
					tLastWritedTime = time(NULL);	// Last Writeing Time Resetting.

					////////////////////////////////////////////////////////////////
					*clsLogM << clsLogM->ErrorCode(0)
							<< "UDR File(2) is on writing Block Seq #"
							<< clsBlockData.get_unBlockSeq() << endl;
					////////////////////////////////////////////////////////////////

					//	Write a making file Information
					clsFileInfo.put_FileInfoData (clsFiledata.get_clsFileHeader(),\
													  FLAG_FILE_MADE);

					//	Total of Record Count in Data File
					clsBlockData.put_unTotalRecCount(0);

					////////////////////////////////////////////////////////////////
					*clsLogM << clsLogM->ErrorCode(0)
							<< "UDR File#" << clsFiledata.get_unFileSeq()
							<< " is writed !!" << endl;
					////////////////////////////////////////////////////////////////
				}
				else
				{
					*clsLogM << "Error No = [" << clsLogM->ErrorCode(6100) << "]"
							<< "Error is occured, so exit !!" << endl;
					break;
				}
			}
		}
	}	//	end of while

	delete mqPMS;
}

void checkSeqNo(CBlockData& clsBlock, CFileData& clsFile)
{
	int	nLastFileSeq = clsFile.get_LastWrittenUdrFileSeq();
	int	nCurrNextBlkSeq;

	clsFile.read_BdsUdrData(nLastFileSeq);

	nCurrNextBlkSeq = clsFile.get_unEndBlockSeq()+1;

	if (nCurrNextBlkSeq != clsBlock.get_unBlockSeqInConf())
		clsBlock.put_unBlockSeqInConf(nCurrNextBlkSeq);

	cout << "nLastFileSeq = " << nLastFileSeq << "nCurrNextBlkSeq = " << nCurrNextBlkSeq << endl;
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
			*clsLogM << "Error No = [" << clsLogM->ErrorCode(6101) << "]"
					<< "[ERROR] PMSMQ " << PMSMQ_NO << " IS FULL sleep(1)...!"	<< endl;
			sleep(1);
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
	*clsLogM << "Error No = [" << clsLogM->ErrorCode(6101) << "]"
			<< "SIGNAL :: Collector is got sigint = [" << sigNo << "]"
			<< PserialNo << "Process End " << endl;

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


