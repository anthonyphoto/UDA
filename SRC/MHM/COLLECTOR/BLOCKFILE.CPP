/******************************************************************
* 파 일 명     
      : BlockFIle.cpp                
* 기   능                                                          
     : BDS File을 만들기 전에 Aggregator로 부터 들어온 Udr을 임시 파일에 저장한다. 
       그리고 BDS 형식에 맞게 변환된 UDR에 대해 File, Block Header를 붙인다.
* 작 성 자     
     : 권종욱                         
* first data  
     : 2000. 10.15                      
* last updata 
     : 2000. 12. 06                  
* 관 련 화 일  
     : BlockFIle.hpp
* program history                                
                           
* Version
  : 1.1 ( 2001. 05. 21 )                                           
******************************************************************/


#include "BlockFile.hpp"
//#include <SendLogManage.hpp>

//	Log 생성을 위한 extern 객체 생성
//	extern SendLogManage*  clsLogM;

//	BDS Send Data File Name	: use in BlockFile.cpp	(ex : BDS_UDR.11)

extern char FILE_NAME_COL_BDS_UDR[50];

/////////////////////////////////
//	CLASS	CBlockConfigData's Method
/////////////////////////////////

CBlockConfigData::CBlockConfigData(const char* strFileName)
{
//	cout << "strFileName : " << strFileName << endl;
	read_BlockConfigFile(strFileName);
//	cout << "strFileName : " << strFileName << endl;

	strcpy(strCfgFileName, strFileName);

//	cout << "strCfgFileName : " << strCfgFileName << endl;

	nMaxRecCount = (nBlockSize - nBlockHeaderSize) / nRecSize;

/*
	cout << "nMaxRecCount : " << nMaxRecCount << endl;
	cout << "nBlockSize : " << nBlockSize << endl;
	cout << "nBlockHeaderSize : " << nBlockHeaderSize << endl;
	cout << "nRecSize : " << nRecSize << endl;
*/
}

CBlockConfigData::~CBlockConfigData()
{
	;
}

void CBlockConfigData::read_BlockConfigFile()
{
	ifstream	ConfigFile(strCfgFileName, ios::binary);

	ConfigFile.read((char*)this, sizeof(CBlockConfigData));
	ConfigFile.close();
}

void CBlockConfigData::read_BlockConfigFile(const char* strFileName)
{
	ifstream	ConfigFile(strFileName, ios::binary);

	ConfigFile.read((char*)this, sizeof(CBlockConfigData));
	ConfigFile.close();
}

int CBlockConfigData::write_BlockConfigFile()
{
	errno = 0;
	
	ofstream	ConfigFile(strCfgFileName, ios::binary);
	if (!ConfigFile)
		return -1;

	ConfigFile.write(reinterpret_cast<const char *>(this), sizeof(CBlockConfigData));

	ConfigFile.flush();

	if(errno == ENOSPC)
	{
		ConfigFile.close();
		return -1;
	}

	ConfigFile.close();
	
	return 0;
}

void CBlockConfigData::put_unBlockSeq(unsigned int unValue)
{
	unBlockSeq 	= unValue;
}

int CBlockConfigData::write_unBlockSeq()
{
	if(write_BlockConfigFile() < 0)
		return -1;
	return 0;	
}


/////////////////////////////////
//	CLASS	CFileConfigData's Method
/////////////////////////////////

CFileConfigData::CFileConfigData(const char* strFileName)
{
	read_FileConfigFile(strFileName);

	strcpy(strCfgFileName, strFileName);

	nOneFileSize = (nMaxBlockCount * nOneBlockSize) + nFileHeaderSize;
}

CFileConfigData::~CFileConfigData()
{
	write_FileConfigFile();
}

void CFileConfigData::read_FileConfigFile()
{
	ifstream	ConfigFile(strCfgFileName, ios::binary);

	ConfigFile.read((char*)this, sizeof(CFileConfigData));
	ConfigFile.close();
}

void CFileConfigData::read_FileConfigFile(const char* strFileName)
{
	ifstream	ConfigFile(strFileName, ios::binary);

	ConfigFile.read((char*)this, sizeof(CFileConfigData));
	ConfigFile.close();
}

int CFileConfigData::write_FileConfigFile()
{
	errno = 0;
	
	ofstream	ConfigFile(strCfgFileName, ios::binary);
	if (!ConfigFile)
		return -1;

	ConfigFile.write(reinterpret_cast<const char*>(this), sizeof(CFileConfigData));
	ConfigFile.flush();

	if(errno == ENOSPC)
	{
		ConfigFile.close();
		return -1;
	}

	ConfigFile.close();
	
	return 0;
}

void CFileConfigData::put_unFileSeq(unsigned int unValue)
{
	unFileSeq 	= unValue;

//	write_FileConfigFile();		//	2000. 12. 5
}

int CFileConfigData::write_unFileSeq(unsigned int unValue)
{
	unFileSeq 	= unValue;
	if(write_FileConfigFile() < 0)
		return -1;
	return 0;	
}

/////////////////////////////////
//	CLASS	CBlockData's Method
/////////////////////////////////

CBlockData::CBlockData(const char* strCfgFileName)
{
	clsPtrConfig = new CBlockConfigData(strCfgFileName);	//	get Block Info

	nBillDataLen =	clsPtrConfig->get_nBlockSize() - \
					clsPtrConfig->get_nBlockHeaderSize();

	strBillingData  = new char[nBillDataLen+1];
	strOneBlock		= new char[clsPtrConfig->get_nBlockSize()+1];

	memset (strBillingData,	0x20, nBillDataLen);
	strBillingData[nBillDataLen] = NULL;
	memset (strOneBlock,	NULL, clsPtrConfig->get_nBlockSize()+1);

	nTempRecCount	= 0;
	nEmptyRecCount	= 0;
	unTotalRecCount	= 0;

	clsPtrConfig->put_nMaxRecCount(nBillDataLen/clsPtrConfig->get_nRecSize());

	clsBlockHeader.put_nRecCount(0);
	clsBlockHeader.put_unBlockSeq(clsPtrConfig->get_unBlockSeq());
}

CBlockData::~CBlockData()
{
	delete	clsPtrConfig;
	delete	strBillingData;
	delete	strOneBlock;
}

int CBlockData::put_strBdsBlock()		//	Fill the Empty Record.
{
	char*	strEmptyBuf;
	unsigned int 	nTempSeq;

	strEmptyBuf = new char[clsPtrConfig->get_nRecSize()+1];
	memset(strEmptyBuf, 0x20, clsPtrConfig->get_nRecSize());
	strEmptyBuf[clsPtrConfig->get_nRecSize()] = NULL;

	nEmptyRecCount = nTempRecCount;

	if (nTempRecCount == 0)		//	Current Block has no Data.
	{
		//	Move Empty Block Header Data(Record Count) to a Block
		clsBlockHeader.put_nRecCount(0);
		clsBlockHeader.put_unBlockSeq(0);
	}
	else						//	Current Block has some Data.
	{
		//	Move Block Header Data(Record Count) to a Block
		clsBlockHeader.put_nRecCount(nTempRecCount);

		nTempSeq = clsPtrConfig->get_unBlockSeq();

		if ( nTempSeq == 0xFFFFFFFF)
			nTempSeq = 0;

		clsBlockHeader.put_unBlockSeq(nTempSeq);

		//	Move & Write Block Seq to Config Data

		nTempSeq = (clsBlockHeader.get_unBlockSeq()+1);

		if ( nTempSeq == 0xFFFFFFFF)
			nTempSeq = 0;

		// Update Block Seq to Config File
		clsPtrConfig->put_unBlockSeq(nTempSeq);
	}

	//	Move Empty Record to a Block
	while(1)
	{
		memcpy(	strBillingData+(nEmptyRecCount*clsPtrConfig->get_nRecSize()),
				strEmptyBuf,
				clsPtrConfig->get_nRecSize()	);

		nEmptyRecCount++;

		if (nEmptyRecCount == clsPtrConfig->get_nMaxRecCount())
			break;
	}

	//	Move Block Header to a UDR Block.
	memcpy(strOneBlock, (char*)&clsBlockHeader, sizeof(CbdsUdrBlockHeader));
	//	Move Bill Data to a Block
	memcpy(strOneBlock+sizeof(CbdsUdrBlockHeader), strBillingData, nBillDataLen	);

	memset (strBillingData,	0x20, nBillDataLen);
	strBillingData[nBillDataLen] = NULL;
	nTempRecCount = nEmptyRecCount = 0;

	delete strEmptyBuf;

	if ((clsBlockHeader.get_nRecCount() == 0) &&
		(unTotalRecCount			   	== 0))
		return FLAG_EMPTY_BLOCK_MADE;

	nTempRecCount = 0;

	return FLAG_BLOCK_MADE;
}

int CBlockData::put_strBdsBlock(char* strValue)
{
	memcpy(	strBillingData+(nTempRecCount*clsPtrConfig->get_nRecSize()),
			strValue,
			clsPtrConfig->get_nRecSize()	);

	nTempRecCount++;

	if( nTempRecCount >= clsPtrConfig->get_nMaxRecCount() )
	{
		if ( nTempRecCount == clsPtrConfig->get_nMaxRecCount() )
		{
			//	Move Block Header Data(Record Count) to a Block
			clsBlockHeader.put_nRecCount(nTempRecCount);
			clsBlockHeader.put_unBlockSeq(clsPtrConfig->get_unBlockSeq());

			memcpy(strOneBlock, (char*)&clsBlockHeader, sizeof(CbdsUdrBlockHeader));
			//	Move Bill Data to a Block
			memcpy(strOneBlock+sizeof(CbdsUdrBlockHeader),\
					strBillingData,	nBillDataLen	);

			unsigned int 	nTempSeq = (clsBlockHeader.get_unBlockSeq()+1);

			if ( nTempSeq == 0xFFFFFFFF)
				nTempSeq = 0;

			//	Move & Write Block Seq to Config Data
			clsPtrConfig->put_unBlockSeq( nTempSeq );

			memset (strBillingData,	0x20, nBillDataLen);
			strBillingData[nBillDataLen] = NULL;
			nTempRecCount = 0;

			unTotalRecCount = (unTotalRecCount+3) % (0xFFFFFFFF);

			return(FLAG_BLOCK_MADE);
		}
		else
		{
			cout << "FAIL : int CBlockData::put_strBdsBlock(char* strValue)";
			cout << "\t" << "Record Count = " << nTempRecCount << endl;
			return -1;
		}
	}
	return(FLAG_BLOCK_NO_MADE);
}

/////////////////////////////////
//	CLASS	CFileData's Method
/////////////////////////////////

CFileData::CFileData(const char* strCfgFileName)
{
	clsPtrConfig = new CFileConfigData(strCfgFileName);	//	get Block Info

	nBillDataBlockLen = clsPtrConfig->get_nOneFileSize() - clsPtrConfig->get_nFileHeaderSize();

	strOneFile 		= new char[clsPtrConfig->get_nOneFileSize()+1];
	strBillingData	= new char[nBillDataBlockLen+1];
	strOneBlock		= new char[clsPtrConfig->get_nOneBlockSize()+1];

	memset(strOneFile, 		NULL, clsPtrConfig->get_nOneFileSize()+1);
	memset(strOneBlock,		NULL, clsPtrConfig->get_nOneBlockSize()+1);
	memset(strBillingData,	NULL, nBillDataBlockLen+1);

	memset(strFileName,		NULL, 128);

	nTempBlockCount = 0;

	if(clsPtrConfig->get_nOneBlockSize() == 0)
		cout << "ERROR : clsPtrConfig->get_nOneBlockSize() = " << clsPtrConfig->get_nOneBlockSize() << endl;

	clsPtrConfig->put_nMaxBlockCount(	nBillDataBlockLen / \
										clsPtrConfig->get_nOneBlockSize()	);

	cout << "nMaxBlockCount = " << clsPtrConfig->get_nMaxBlockCount() << endl;
	cout << "nBillDataBlockLen = " << nBillDataBlockLen << endl;
	cout << "clsPtrConfig->get_nOneBlockSize() = " << clsPtrConfig->get_nOneBlockSize() << endl;

	clsFileHeader.put_unBlockCount(0);

	cout << "nMaxBlockCount = " << clsPtrConfig->get_nMaxBlockCount() << endl;

	clsFileHeader.put_unFileSeq(clsPtrConfig->get_unFileSeq());

	cout << "CFileData's Constructor is OK !!" << endl;
}

CFileData::~CFileData()
{
	delete	clsPtrConfig;
	delete	strOneFile;
}

void CFileData::get_strBillingData(int nIndex, CUdrBlock* clsUdrOneBlock)
{
	memcpy((char*)clsUdrOneBlock, &strBillingData[nIndex*clsPtrConfig->get_nOneBlockSize()],
									clsPtrConfig->get_nOneBlockSize());

}

int CFileData::put_strBdsFileBuffer(unsigned int nBlockSeqNum, char* strValue)
{

	memcpy(	strBillingData+(nTempBlockCount*clsPtrConfig->get_nOneBlockSize()),
			strValue,
			clsPtrConfig->get_nOneBlockSize()	);

	//	Set Block Seq
	if (nTempBlockCount == 0)
	{
		clsFileHeader.put_unStartBlockSeq(nBlockSeqNum);
		clsFileHeader.put_unEndBlockSeq(nBlockSeqNum);
	}
	else if (nBlockSeqNum != 0)
		clsFileHeader.put_unEndBlockSeq(nBlockSeqNum);

	nTempBlockCount++;

	if(	(nTempBlockCount >= clsPtrConfig->get_nMaxBlockCount()))
		return FLAG_FILE_MADE;
	else
		return FLAG_FILE_NO_MADE;
}

int CFileData::put_strBdsFile()
{
	unsigned int	unBlockCount = (clsFileHeader.get_unEndBlockSeq() - \
									clsFileHeader.get_unStartBlockSeq()) + 1;

	//	Move File Header to a File
	clsFileHeader.put_unBlockCount(unBlockCount);
	clsFileHeader.put_unFileSeq(clsPtrConfig->get_unFileSeq());

	memcpy(strOneFile, (char*)&clsFileHeader, sizeof(CbdsUdrFileHeader));

	//	Move Bill Data Block to a File Buffer
	memcpy(&strOneFile[sizeof(CbdsUdrFileHeader)], strBillingData,	nBillDataBlockLen	);

	//	Write Bill Data
	if(write_BdsUdrData(clsPtrConfig->get_unFileSeq()) < 0)
		return -1;

	unsigned int 	nTempSeq = (clsFileHeader.get_unFileSeq()+1);

	if ( nTempSeq == 0xFFFFFFFF)
		nTempSeq = 0;

	//	Move & Write File Seq to Config Data
	clsPtrConfig->put_unFileSeq(nTempSeq);

	//	Used Buffer is initialized
	memset (strBillingData,	NULL, nBillDataBlockLen+1);
	nTempBlockCount = 0;

	return 0;
}

void CFileData::make_BdsUdrFileName(unsigned int unSeq)
{
	char	strSeqBuf[24];
	
	memset(strFileName,		NULL, 128);
	memset(strSeqBuf,		NULL, 24);
	strcat (strFileName, FILE_NAME_COL_BDS_UDR);	//	"SVC1/BDS/BDS_UDR."
	cout << "FILE_NAME_COL_BDS_UDR : " << FILE_NAME_COL_BDS_UDR << endl;
	sprintf(strSeqBuf, "%u", unSeq);
	strcat(strFileName, strSeqBuf);
	cout << "strFileName : " << strFileName << endl;
}

int CFileData::write_BdsUdrData(unsigned int unSeq)
{
	make_BdsUdrFileName(unSeq);

	errno = 0;
	
	ofstream	BdsUdrFile(strFileName, ios::app | ios::noreplace | ios::binary);

	if (!BdsUdrFile)
	{
		cout << "Unable to open : [" <<strFileName << "]" << endl;
		return -1;
	}

	BdsUdrFile.write(reinterpret_cast<const char*>(strOneFile), clsPtrConfig->get_nOneFileSize());

	BdsUdrFile.flush();

	if(errno == ENOSPC)
	{
		BdsUdrFile.close();
		return -1;
	}

	BdsUdrFile.close();

	return 0;
}

int CFileData::read_BdsUdrData(unsigned int unSeq)
{
	make_BdsUdrFileName(unSeq);

	ifstream	BdsUdrFile(strFileName, ios::binary);

	if (!BdsUdrFile)
	{
		cout << "Unable to open : [" << strFileName << "]" << endl;
		return -1;
	}

	memset(strOneFile, NULL, clsPtrConfig->get_nOneFileSize()+1);
	BdsUdrFile.read(reinterpret_cast<char*>(strOneFile), clsPtrConfig->get_nOneFileSize());
	BdsUdrFile.close();

	memcpy((char*)&clsFileHeader, strOneFile, sizeof(CbdsUdrFileHeader));
	memcpy(strBillingData		, strOneFile+sizeof(CbdsUdrFileHeader), nBillDataBlockLen );

	return 0;
}

unsigned int CFileData::get_LastWrittenUdrFileSeq()
{
	int 	nRet 		= 0,
			TempFileSeq = 0;

	for(int i = 0; ; i++)
	{
		nRet = read_BdsUdrData(get_unFileSeqInConf() - i);
		if (nRet == 0 && i == 0)
		{
			TempFileSeq = get_unFileSeq();

			for(int j = 1; j < 5; j++)
			{
				nRet = read_BdsUdrData(get_unFileSeqInConf() + j);
				if (nRet == 0)
				{
					TempFileSeq = get_unFileSeq();
					continue;
				}
				else
				{
					if (TempFileSeq != 0)
						return TempFileSeq;
				}
			}
		}
		else if (nRet == 0)
			return (get_unFileSeq());
	}
	return -1;
}

///////////////////////////////////
//	CLASS	CFileInfoData's Method
///////////////////////////////////

CFileInfoData::CFileInfoData(const char* strFileName)
{
	read_FileConfigFile(strFileName);

	strcpy(strInfoFileName, strFileName);
//	memset (strDate, NULL, 12);
}

CFileInfoData::~CFileInfoData()
{
	;
}

void CFileInfoData::read_FileConfigFile()
{
	ifstream	FileInfo(strInfoFileName, ios::binary);

	FileInfo.read((char*)this, sizeof(CFileInfoData));
	FileInfo.close();
}

void CFileInfoData::read_FileConfigFile(const char* strFileName)
{
	ifstream	FileInfo(strFileName, ios::binary);

	FileInfo.read((char*)this, sizeof(CFileInfoData));
	FileInfo.close();
}

int CFileInfoData::write_FileConfigFile()
{
	errno = 0;
	
	ofstream	FileInfo(strInfoFileName, ios::binary);

	if (!FileInfo)
		return -1;

	FileInfo.write(reinterpret_cast<const char*>(this), sizeof(CFileInfoData));
	FileInfo.flush();

	if(errno == ENOSPC)
	{
		FileInfo.close();
		return -1;
	}

	FileInfo.close();

	return 0;
}

int CFileInfoData::put_FileInfoData(CbdsUdrFileHeader& clsUdrFileHeader, int nStatus)
{
	put_unFileSeq(clsUdrFileHeader.get_unFileSeq());
	put_nStartBlockSeq(clsUdrFileHeader.get_unStartBlockSeq());
	put_nEndBlockSeq(clsUdrFileHeader.get_unEndBlockSeq());
	put_nStatus(nStatus);


/*
	cout <<	"\n=========================================" << endl;
	cout << "clsUdrFileHeader.get_unFileSeq()       = " << clsUdrFileHeader.get_unFileSeq() << endl;
	cout << "clsUdrFileHeader.get_unStartBlockSeq() = " << clsUdrFileHeader.get_unStartBlockSeq() << endl;
	cout << "clsUdrFileHeader.get_unEndBlockSeq()   = " << clsUdrFileHeader.get_unEndBlockSeq() << endl;
	cout << "nStatus                                = " << nStatus << endl;
	cout <<	"=========================================" << endl;
*/
	if(write_FileConfigFile() < 0)
		return -1;
	return 0;	
}



TempUdrFile::TempUdrFile(int nRecSize)
{
	nRecordSize = nRecSize;
//	sprintf(strFileName, "%sTEMP", FILE_NAME_COL_BDS_UDR);
//	nUdrCount = get_TempFileSize() / nRecSize;
	;
}

void TempUdrFile::get_nUdrCountFromFile()
{
	nUdrCount = get_TempFileSize() / nRecordSize;
}

long TempUdrFile::get_TempFileSize()
{
	int		nOffset = 0,
			nFd;

	off_t	FileSize;

	if((nFd = open(strFileName, O_RDONLY)) < 0)
		FileSize = 0;

	if((FileSize=lseek(nFd, nOffset, SEEK_END)) < 0)
		FileSize = 0;

	close (nFd);
	return FileSize;
}

int	TempUdrFile::write_TempUdrFile(UdrFormat* strUdr)
{
	errno = 0;

	ofstream	UdrTempFile(strFileName, ios::app | ios::binary);
	if (!UdrTempFile)
		return -1;

	UdrTempFile.write(reinterpret_cast<const char*>(strUdr), sizeof(UdrFormat));

	UdrTempFile.flush();

	if(errno == ENOSPC)
	{
		UdrTempFile.close();
		return -1;
	}

	UdrTempFile.close();

	return 0;
}

int	TempUdrFile::read_TempUdrFile(UdrFormat* strUdr, int nOffset)
{
	ifstream	UdrTempFile(strFileName, ios::binary);
	if (!UdrTempFile)
		return -1;

	UdrTempFile.seekg(nOffset * sizeof(UdrFormat));
//	UdrTempFile.write(reinterpret_cast<const char*>(strUdr), sizeof(UdrFormat));

	UdrTempFile.close();

	return 0;
}

void TempUdrFile::put_strFileName(const char *strValue)
{
	memcpy(strFileName,	strValue, strlen(strValue));
	strFileName[strlen(strValue)] = NULL;
}
