#include <iostream.h>
#include <fstream.h>
#include <UdrBds.hpp>
#include <BlockFile.hpp>

const char*	    PATH_NAME_BDS_BLOCK_CFG	=   "/home1/uda/UDA/cfg/BDS_BlockInfo.cfg";
const char*	    PATH_NAME_BDS_FILE_CFG	=   "/home1/uda/UDA/cfg/BDS_FileConf.cfg";
const char*	    PATH_NAME_BDS_FILE_INFO	=   "/home1/uda/UDA/cfg/BDS_FileInfo.cfg";


/////////////////////////////////
//	CLASS	CBlockConfigData's Method
/////////////////////////////////

CBlockConfigData::CBlockConfigData(const char* strFileName)
{
	read_BlockConfigFile(strFileName);
	
	strCfgFileName = new char[strlen(strFileName)+1];
	strcpy(strCfgFileName, strFileName);
	
	nMaxRecCount = (nBlockSize - nBlockHeaderSize) / nRecSize;
}

CBlockConfigData::~CBlockConfigData()
{
	delete	strCfgFileName;
	write_BlockConfigFile();
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

void CBlockConfigData::write_BlockConfigFile()
{
	ofstream	ConfigFile(strCfgFileName, ios::binary);
	
	ConfigFile.write((char*)this, sizeof(CBlockConfigData));
	ConfigFile.close();
}

void CBlockConfigData::put_unBlockSeq(unsigned int unValue)		
{	
	unBlockSeq 	= unValue;
	
	if ((unBlockSeq % 32) == 0)
		write_BlockConfigFile();

}

/////////////////////////////////
//	CLASS	CFileConfigData's Method
/////////////////////////////////

CFileConfigData::CFileConfigData(const char* strFileName)
{
	read_FileConfigFile(strFileName);
	
	strCfgFileName = new char[strlen(strFileName)+1];
	strcpy(strCfgFileName, strFileName);
	
	cout << "nMaxBlockCount = " << nMaxBlockCount << endl;
	cout << "nOneBlockSize = " << nOneBlockSize << endl;
	cout << "nFileHeaderSize = " << nFileHeaderSize << endl;
	cout << "nOneFileSize = " << nOneFileSize << endl;
	cout << "strCfgFileName = " << strCfgFileName << endl << endl;
	
	nOneFileSize = (nMaxBlockCount * nOneBlockSize) + nFileHeaderSize;
}

CFileConfigData::~CFileConfigData()
{
	delete	strCfgFileName;
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

void CFileConfigData::write_FileConfigFile()
{
	ofstream	ConfigFile(strCfgFileName, ios::binary);
	
	ConfigFile.write((char*)this, sizeof(CFileConfigData));
	ConfigFile.close();
}

void CFileConfigData::put_unFileSeq(unsigned int unValue)		
{	
	unFileSeq 	= unValue;
	
	write_FileConfigFile();
}



CFileInfoData::CFileInfoData(const char* strFileName)
{
	read_FileConfigFile(strFileName);
	
	strInfoFileName = new char[strlen(strFileName)+1];
	strcpy(strInfoFileName, strFileName);
	memset (strDate, NULL, 12);
}

CFileInfoData::~CFileInfoData()
{
	delete strInfoFileName;
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

void CFileInfoData::write_FileConfigFile()
{
	ofstream	FileInfo(strInfoFileName, ios::binary);
	
	FileInfo.write((char*)this, sizeof(CFileInfoData));
	FileInfo.close();
}
		

int main()
{
//	CBlockConfigData	CBlockConfig(PATH_NAME_BDS_BLOCK_CFG);
	CFileConfigData*	CFileConfig;
//	CFileInfoData*		CFileInfo;
	
//	CFileInfo	= new CFileInfoData(PATH_NAME_BDS_FILE_INFO);
	CFileConfig = new CFileConfigData(PATH_NAME_BDS_FILE_CFG);
	
//	CBlockConfigData.put_nBlockSize(1024);
//	CBlockConfig.put_nBlockHeaderSize(12);
//	CBlockConfig.put_nRecSize(309);
//	CBlockConfig.put_unBlockSeq(34);
	
/*	CBlockConfig.read_BlockConfigFile();
	
	cout << "nBlockSize   			= " << CBlockConfig.get_nBlockSize()		<< endl;
	cout << "get_nBlockHeaderSize	= " << CBlockConfig.get_nBlockHeaderSize()  << endl;
	cout << "nRecSize     			= " << CBlockConfig.get_nRecSize()    		<< endl;
	cout << "nMaxRecCount			= " << CBlockConfig.get_nMaxRecCount()		<< endl;
	cout << "nBlockSeq   			= " << CBlockConfig.get_unBlockSeq()   		<< endl << endl;
	
	CBlockConfig.put_unBlockSeq(32);
	CBlockConfig.read_BlockConfigFile();

	cout << "nBlockSize   			= " << CBlockConfig.get_nBlockSize()		<< endl;
	cout << "get_nBlockHeaderSize	= " << CBlockConfig.get_nBlockHeaderSize()  << endl;
	cout << "nRecSize     			= " << CBlockConfig.get_nRecSize()    		<< endl;
	cout << "nMaxRecCount			= " << CBlockConfig.get_nMaxRecCount()		<< endl;
	cout << "nBlockSeq   			= " << CBlockConfig.get_unBlockSeq()   		<< endl << endl << endl;

	CFileInfo->read_FileConfigFile();
	
	cout << "unFileSeq()   = " << CFileInfo->get_unFileSeq()      << endl;
	cout << "strDate()     = " << CFileInfo->get_strDate()        << endl;
	cout << "nStartBlockSeq= " << CFileInfo->get_nStartBlockSeq() << endl;
	cout << "nEndBlockSeq()= " << CFileInfo->get_nEndBlockSeq()   << endl;
	cout << "nStatus()     = " << CFileInfo->get_nStatus()        << endl;
	cout << "nResult()     = " << CFileInfo->get_nResult()        << endl << endl;
	
	CFileInfo->put_unFileSeq(100);
	CFileInfo->put_strDate("20000920");
	CFileInfo->put_nStartBlockSeq(100);
	CFileInfo->put_nEndBlockSeq(132);
	CFileInfo->put_nStatus(1);
	CFileInfo->put_nResult(0);

	CFileInfo->write_FileConfigFile();
	CFileInfo->read_FileConfigFile();
		
	cout << "unFileSeq()   = " << CFileInfo->get_unFileSeq()      << endl;
	cout << "strDate()     = " << CFileInfo->get_strDate()        << endl;
	cout << "nStartBlockSeq= " << CFileInfo->get_nStartBlockSeq() << endl;
	cout << "nEndBlockSeq()= " << CFileInfo->get_nEndBlockSeq()   << endl;
	cout << "nStatus()     = " << CFileInfo->get_nStatus()        << endl;
	cout << "nResult()     = " << CFileInfo->get_nResult()        << endl << endl;
*/


	CFileConfig->put_nFileHeaderSize(20);
//	CFileConfig->put_nOneFileSize(32*1024+20);
	CFileConfig->put_nOneBlockSize(1024);
	CFileConfig->put_nMaxBlockCount(32);
	CFileConfig->put_unFileSeq(0);
	
	CFileConfig->write_FileConfigFile();
	
	CFileConfig->read_FileConfigFile();

	cout << "nFileHeaderSize= " << CFileConfig->get_nFileHeaderSize() << endl;
	cout << "nOneFileSize   = " << CFileConfig->get_nOneFileSize()    << endl;
	cout << "nOneBlockSize  = " << CFileConfig->get_nOneBlockSize()   << endl;
	cout << "nMaxBlockCount = " << CFileConfig->get_nMaxBlockCount()   << endl;
	cout << "nMaxBlockCount = " << CFileConfig->get_nMaxBlockCount()   << endl;
	cout << "nMaxBlockCount = " << CFileConfig->get_nMaxBlockCount()   << endl;
	cout << "unFileSeq      = " << CFileConfig->get_unFileSeq()       << endl << endl;
	                 
	CFileConfig->put_nMaxBlockCount(32);
	CFileConfig->put_unFileSeq(300);
	CFileConfig->read_FileConfigFile();

	cout << "nFileHeaderSize= " << CFileConfig->get_nFileHeaderSize() << endl;
	cout << "nOneFileSize   = " << CFileConfig->get_nOneFileSize()    << endl;
	cout << "nOneBlockSize  = " << CFileConfig->get_nOneBlockSize()   << endl;
	cout << "nMaxBlockCount = " << CFileConfig->get_nMaxBlockCount()   << endl;
	cout << "unFileSeq      = " << CFileConfig->get_unFileSeq()       << endl << endl;

}














