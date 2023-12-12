#include<stdlib.h>
#include <iostream.h>
#include <fstream.h>
#include<iomanip.h>
//#include <UdrBds.hpp>
#include <BlockFile.hpp>

/*
const char*	    PATH_NAME_BDS_BLOCK_CFG	=   "/home1/uda/UDA/cfg/BDS_BlockInfo.cfg";
const char*	    PATH_NAME_BDS_FILE_CFG	=   "/home1/uda/UDA/cfg/BDS_FileConf.cfg";
const char*	    PATH_NAME_BDS_FILE_INFO	=   "/home1/uda/UDA/cfg/BDS_FileInfo.cfg";
*/


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
