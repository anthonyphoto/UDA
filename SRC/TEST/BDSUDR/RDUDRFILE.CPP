#include <stdlib.h>
#include <ctype.h>

unsigned int	unFileNum;
unsigned int    unRecCnt;

#include <BlockFile.hpp>
#include <UdrBds.hpp>
#include <SendLogManage.hpp>
SendLogManage*  clsLogM;
void displayFileHeader(CFileData& clsUdrFile);

//	UDR File Information

/*
#ifdef	_SVC1
const char*	    PATH_NAME_BDS_FILE_CFG	=   "/SVC1/DATA/BDS_FileConf.cfg";
#elif	_SVC2
const char*	    PATH_NAME_BDS_FILE_CFG	=   "/SVC2/DATA/BDS_FileConf.cfg";
#endif
*/
char FILE_NAME_COL_BDS_UDR[50];
char	    PATH_NAME_BDS_FILE_CFG[100];	

int main(int argc, char* argv[])
{
	unsigned char	ucSwitch1;
	unsigned char	ucSwitch2;
//	unsigned int	unFileNum;
	unsigned int	unSumOfBlockCount = 0;
	unsigned int	unSumOfRecCount = 0;
	
	if (argc < 2 )
	{
		cout << "UDAGE : readF [File Seq #] [ServiceId]" << endl;
		cout << "Ex    : readF 10" << endl;
		exit(1);
	}
	unFileNum =(unsigned int)atoi(argv[1]);
//	cout << "\nFile Number ? ";
//	cin >> unFileNum;
//	cout << "\nDo you want to see a full data?(Y/N) ";
//	cin >> ucSwitch1;
	
	
	char	strBuf[128];
	char	strUdrBuf[1024];
/**/char	strUdrBuf1[(1024*32)+1];
	
	memset(strUdrBuf, NULL, 1024);
/**/memset(strUdrBuf1, NULL, (1024*32)+1);
	
	CBDS_UdrFormat	clsBDSform;

	memset(PATH_NAME_BDS_FILE_CFG, 0x00, sizeof(PATH_NAME_BDS_FILE_CFG));
	sprintf(PATH_NAME_BDS_FILE_CFG, "/SVC%d/DATA/BDS_FileConf.cfg", atoi(argv[2]));
	
	CFileData	clsUdrFile(PATH_NAME_BDS_FILE_CFG);

	if (clsUdrFile.read_BdsUdrData(unFileNum) < 0 )
	{
//		exit -1;	
	}

	CUdrBlock	clsUdrOneBlock(clsUdrFile.get_OneBlockSize());
	
	displayFileHeader(clsUdrFile);
	
/**/memcpy(strUdrBuf1, clsUdrFile.get_strBillingData(), clsUdrFile.get_nBillDataBlockLen());
	
//	for(int i = 0; i < clsUdrOneBlock.get_nRecCount()-1; i++)
	for(int i = 0; i < 32; i++)
	{
//		clsUdrFile.get_strBillingData(i,&clsUdrOneBlock);

		memset(strUdrBuf, NULL, 1024);
		memset(strBuf, NULL, 128);

/**/	memcpy((char*)&clsUdrOneBlock, &strUdrBuf1[(i*1024)], 1024);
		
		cout <<"Block Seq    = " << clsUdrOneBlock.get_unBlockSeq() << "\t";
		cout <<"Record Count = " << clsUdrOneBlock.get_nRecCount() << endl << endl;
		
		if (clsUdrOneBlock.get_unBlockSeq() != 0)
			unSumOfBlockCount++;

		unRecCnt = clsUdrOneBlock.get_nRecCount();
		if (clsUdrOneBlock.get_nRecCount()==0)
			break;
		unSumOfRecCount += clsUdrOneBlock.get_nRecCount();
		
//		memcpy(strUdrBuf, clsUdrOneBlock.get_UdrBlock(), 1012);
/**///	memcpy(strUdrBuf, &strUdrBuf1[(i*1024)+12], 1012);
		memcpy(strUdrBuf, &strUdrBuf1[(i*1024)+12], (336*clsUdrOneBlock.get_nRecCount())-4);
		
//		cout << "\nContinue ? (Y/N) ";
//		cin >> ucSwitch2;
//		if (toupper(ucSwitch2) == 'Y')
			clsBDSform.display_UdrItem(strUdrBuf, 336);
			
		
	}
	
	cout << "\n== Result ==" << endl;
	cout << "Block Count = " << unSumOfBlockCount << "\t\t";
	cout << "Record Count = " << unSumOfRecCount << endl << endl << endl;
	
	
}


void displayFileHeader(CFileData& clsUdrFile)	
{
	cout << "\n== FILE HEADER ==" << endl;
	cout << "unFileID = " << clsUdrFile.get_unFileSeq() << endl;
	cout << "unBlockCount = " << clsUdrFile.get_unBlockCount() << endl;
	cout << "unStartBlockSeq = " << clsUdrFile.get_unStartBlockSeq() << endl;
	cout << "unEndBlockSeq = " << clsUdrFile.get_unEndBlockSeq() << endl << endl;
}

