#include <MqInfo_file.hpp>

MsgQfileInfo::MsgQfileInfo()
{
	memset (strMsgQueueName, NULL, 64);    //  Format:?????_?????, Len : 12
	unRecSize		= 0;
    unRecCount		= 0;
	unHeadOffset	= unTailOffset	= 0;
}

MsgQfileInfo::MsgQfileInfo(	int			nSystemId,		\
							int			nServiceId,		\
							int			nGroupId,		\
							int			nProcessType,	\
							int			nSerialNo,		\
							int			nRemark,			\
            				int 		n
            				)

{
	set_MsgQueueName( 	nSystemId,		\
						nServiceId,		\
						nGroupId,		\
						nProcessType,	\
						nSerialNo,		\
						nRemark);

	if (read_Mqinfo() < 0)
		cout << "MQ Info read FAIL !!! " << endl;

//	cout << "MQ Info read OK !!! " << endl;
}

void MsgQfileInfo::set_MsgQueueName(	int			nSystemId,		\
										int			nServiceId,		\
										int			nGroupId,		\
										int			nProcessType,	\
										int			nSerialNo,		\
										int			nRemark)
{
	char	strTemp[64];

	// pms는 서비스 id가 '0'
	if(	nSystemId == 1 && nServiceId == 0 )
		sprintf(strMsgQueueName, "/SVC1/DATA/MQ/", nServiceId);
	else if(nSystemId == 2 && nServiceId == 0 )
		sprintf(strMsgQueueName, "/SVC3/DATA/MQ/", nServiceId);
	else sprintf(strMsgQueueName, "/SVC%d/DATA/MQ/", nServiceId);

	sprintf(strTemp, "%01d%01d%01d%01d%02d%02d.MQ",	nSystemId,		\
													nServiceId,		\
													nGroupId,		\
													nProcessType,	\
													nSerialNo,		\
													nRemark);
	strcat(strMsgQueueName, strTemp);
}

void MsgQfileInfo::set_MsgQueueName (const char *strMqName)
{
	strcpy(strMsgQueueName, strMqName);
}

void MsgQfileInfo::set_unRecSize (unsigned int unRecordSize)
{
	unRecSize   	= unRecSize;
}

void MsgQfileInfo::set_unRecCount (unsigned int unRecCnt)
{
	unRecCount  	= unRecCnt;
}

void MsgQfileInfo::set_unHeadOffset (unsigned int unHeadLoc)
{
	unHeadOffset = unHeadLoc;
}

void MsgQfileInfo::set_unTailOffset (unsigned int unTailLoc)
{
	unTailOffset = unTailLoc;
}

char* MsgQfileInfo::get_strMsgQueueName()
{
	return strMsgQueueName;
}

unsigned int MsgQfileInfo::get_unRecSize()
{
	return unRecSize;
}

unsigned int MsgQfileInfo::get_unRecCount()
{
	return unRecCount;
}

int	MsgQfileInfo::get_unHeadOffset()
{
	int		nTempHead;

/*	if (unRecCount == 0)
	{
		cout << "unRecCount = 0, So return FLAG_MQ_IS_FULL" << endl;
		return FLAG_MQ_IS_FULL;
	}
*/
//	cout<<"get_unHeadOffset()_unRecCount:"<<get_unRecCount()<<endl;
	nTempHead = ((unHeadOffset+1) % unRecCount);

//	cout << "1111get_unHeadOffset()1" << endl;
//	cout << "unHeadOffset = " << unHeadOffset << endl;
//	cout << "nTempHead = " << nTempHead << endl;
//	cout << "unTailOffset = " << unTailOffset << endl;
//	cout << "11111111111111111111111" << endl;

	if (nTempHead == unTailOffset)
		return FLAG_MQ_IS_FULL;

	return unHeadOffset;
}

int	MsgQfileInfo::get_unTailOffset()
{
	int		nTempTail;

	if (unHeadOffset != unTailOffset)
		unTailOffset = (unTailOffset + 1) % unRecCount;
//	cout << "2222get_unTailOffset222" << endl;
//	cout << "unTailOffset = " << unHeadOffset << endl;
//	cout << "nTempTail = " << nTempTail << endl;
//	cout << "unTailOffset = " << unTailOffset << endl;
//	cout << "22222222222222222222222" << endl;

	return unTailOffset;
}

unsigned int MsgQfileInfo::get_AmountOfData()
{
	int		nAmountOfData;

	nAmountOfData = unHeadOffset - unTailOffset;

	if(nAmountOfData <= 0)
		nAmountOfData += unRecCount;

	return nAmountOfData;
}

int	MsgQfileInfo::open_MqFile(char*	strMqFileName)
{
	return open(strMqFileName,O_RDWR);
}

int	MsgQfileInfo::close_MqFile(int fp)
{
	close(fp);
	return 0;
}

void MsgQfileInfo::read_Mqinfo(fstream* fQFile)
{
	fQFile->seekg(0);
	fQFile->read(reinterpret_cast<char *>(this), sizeof(MsgQfileInfo));
}

int MsgQfileInfo::read_Mqinfo()
{
	ifstream	MqFile(strMsgQueueName, ios::binary);
	if (!MqFile)
	{
//		cout 	<< "1@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
//		cout 	<< "Couldn't open the MQ File[" << strMsgQueueName
//				<< "]" << endl;
//		cout 	<< "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
//
		return -1;
	}

	MqFile.seekg(0);
	MqFile.read(reinterpret_cast<char *>(this), sizeof(MsgQfileInfo));
	MqFile.close();

	return 0;
}

int MsgQfileInfo::read_Mqinfo(int fp)
{
	if( lseek(fp,0,SEEK_SET)==-1 )
		return -4;
	if( read(fp,(char *)this,sizeof(MsgQfileInfo))==-1 )
		return -5;
//	cout << "000000read_Mqinfo00000" << endl;
//	cout << "unHeadOffset = " << unHeadOffset << endl;
//	cout << "unTailOffset = " << unTailOffset << endl;
//	cout << "0000000000000000000000" << endl;

}

void MsgQfileInfo::write_Mqinfo(fstream* fQFile)
{
	fQFile->seekp(0);
	fQFile->write(reinterpret_cast<const char *>(this), sizeof(MsgQfileInfo));
	fQFile->flush();
}

void MsgQfileInfo::write_Mqinfo()
{
	fstream	MqFile(strMsgQueueName, ios::out | ios::binary);
	if (!MqFile)
	{
//		cout 	<< "2@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
//		cout 	<< "Couldn't open the MQ File[" << strMsgQueueName
//				<< "]" << endl;
//		cout 	<< "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	}

	MqFile.seekp(0);
	MqFile.write(reinterpret_cast<const char *>(this), sizeof(MsgQfileInfo));
	MqFile.flush();
	MqFile.close();
}

int MsgQfileInfo::write_Mqinfo(int fp)
{
	if( lseek(fp,0,SEEK_SET)==-1 )
		return -4;
	if( write(fp,(char *)this,sizeof(MsgQfileInfo))==-1 )
		return -5;
}

int MsgQfileInfo::get_OffsetOfTail()
{
	return	(	sizeof(strMsgQueueName)	+
				sizeof(unRecSize)		+
				sizeof(unRecCount)		+
				sizeof(unHeadOffset)
			);
}

int MsgQfileInfo::get_OffsetOfHead()
{
	return	(	sizeof(strMsgQueueName)	+
				sizeof(unRecSize)		+
				sizeof(unRecCount)
			);
}

int MsgQfileInfo::put_Message(const void* strMsg, int fp)
{
	struct	flock	m_flag;
	int 			nRet = 0;
	int				nCurrentOffset;

	m_flag.l_pid = getpid();
	m_flag.l_whence = 0;
	m_flag.l_start = 0;
	m_flag.l_len = 0;
	m_flag.l_type = F_WRLCK;


	if (fp < 0)	{
		if ( (fp = open_MqFile(get_strMsgQueueName())) < 0 ){
			cout <<"file open error!!: "<<get_strMsgQueueName()<<endl;
			return -1;
		}
	}

	if( fcntl(fp,F_SETLKW,&m_flag) == -1 ){		//	Here is LOCK Point.
		return -1;
	}

	m_flag.l_type = F_UNLCK;	//	For UNLOCK

	read_Mqinfo(fp);

	//	check MQ Full
	if ((nCurrentOffset = get_unHeadOffset()) == FLAG_MQ_IS_FULL){
		fcntl(fp,F_SETLKW,&m_flag);
		return FLAG_MQ_IS_FULL;
	}

	if( lseek(fp, unRecSize*nCurrentOffset+sizeof(MsgQfileInfo), SEEK_SET)==-1 )
	{
		fcntl(fp,F_SETLKW,&m_flag);
		return -1;
	}
	if( write(fp, strMsg, unRecSize)==-1 )
	{
		fcntl(fp,F_SETLKW,&m_flag);
		return -1;
	}

	set_unHeadOffset ((nCurrentOffset+1) % get_unRecCount());

	if ( write_Mqinfo(fp) < 0 )
	{
		fcntl(fp,F_SETLKW,&m_flag);
		return -1;
	}

	if( fcntl(fp,F_SETLKW,&m_flag) == -1 ){
		return -1;
	}
	return 0;
}

int MsgQfileInfo::get_Message(void* strMsg, int fp)
{
	int 			nRet = 0;
	struct	flock	m_flag;

	m_flag.l_pid = getpid();
	m_flag.l_whence = 0;
	m_flag.l_start = 0;
	m_flag.l_len = 0;
	m_flag.l_type = F_WRLCK;

	if (fp < 0)	{
		if ( (fp = open_MqFile(get_strMsgQueueName())) < 0 ){
			cout <<"file open error!!: "<<get_strMsgQueueName()<<endl;
			return -1;
		}
	}

	if( fcntl(fp,F_SETLKW,&m_flag) == -1 ){		//	Here is LOCK Point.
		return -1;
	}

	m_flag.l_type = F_UNLCK;	//	For UNLOCK

	read_Mqinfo(fp);			//	Read MqInfo(Header)

	if (unHeadOffset == unTailOffset)
	{
		fcntl(fp,F_SETLKW,&m_flag);
		return FLAG_MQ_IS_EMPTY;	//empty
	}

	if( lseek(fp, unRecSize*unTailOffset+sizeof(MsgQfileInfo), SEEK_SET)==-1 )
	{
		fcntl(fp,F_SETLKW,&m_flag);
		return -1;
	}

	if( read(fp, strMsg, unRecSize)==-1 )
	{
		fcntl(fp,F_SETLKW,&m_flag);
		return -1;
	}

	nRet = get_unTailOffset();

	if( write_Mqinfo(fp) < 0)
	{
		fcntl(fp,F_SETLKW,&m_flag);
		return -1;
	}

	fcntl(fp,F_SETLKW,&m_flag);

	return 0;
}

int MsgQfileInfo::create_MqFile	(	int			nSystemId,		\
									int			nServiceId,		\
									int			nGroupId,		\
									int			nProcessType,	\
									int			nSerialNo,		\
									int			nRemark,		\
									int			nRecSize,		\
									int			nRecCnt
								)
{
	char*	strBuffer = new char[nRecSize+1];
	int		fp;
	FILE	*tp;

	memset(strBuffer, 0x20, nRecSize);
	strBuffer[nRecSize] = NULL;

	set_MsgQueueName(	nSystemId,		nServiceId,	nGroupId,
						nProcessType, 	nSerialNo,	nRemark		);
	unRecSize		= (unsigned int)nRecSize;
    unRecCount		= (unsigned int)nRecCnt;
    unHeadOffset	= 0;
    unTailOffset	= 0;

	tp = fopen(strMsgQueueName,"w+");
	fclose(tp);

	if ((fp = open_MqFile(strMsgQueueName)) < 0)
	{
		cout <<"file open error!!: "<<strMsgQueueName<<endl;
		return -1;
	}
	if( write_Mqinfo(fp) < 0)
	{
		close(fp);
		return -2;
	}

	if( lseek(fp, sizeof(MsgQfileInfo), SEEK_SET)==-1 )
	{
		close(fp);
		return -3;
	}

	for(int i = 0; i < nRecCnt; i++)
	{
		if( write(fp,strBuffer,nRecSize) != nRecSize )
		{
			close(fp);
			return -4;
		}
	}
	close(fp);
	return 0;
}


