#include <iostream.h>
#include <time.h>
#include <BlockFile.hpp>
#include <ConfigFile.hpp>
#include <time.hpp>
#include <StatusFile.hpp>
#include <MsgFormat.hpp>
#include <SendLogManage.hpp>

//	Log Manager
SendLogManage*				clsLogM;

int writeBdsFile(CBlockData& clsBlockData, CFileData& clsFiledata, int nSeq);

int main(int argc, char* argv[])
{
	//	Declare Variable
	CBlockData		clsBlockData(PATH_NAME_BDS_BLOCK_CFG);
	CFileData		clsFiledata(PATH_NAME_BDS_FILE_CFG);

	if ( writeBdsFile(	clsBlockData,		\
						clsFiledata,		\
						atoi(argv[1]) ) < 0)
	{
		cout << "Error Occured in writeBdsFile()" << endl;
	}
}

int writeBdsFile(CBlockData& clsBlockData, CFileData& clsFiledata, int nSeq)
{
	TempUdrFile		clsTempFile(sizeof(UdrFormat));	//	Declare Temp File
	CBDS_UdrFormat	Udr_Bds;
	UdrFormat		Udr;
	int				nRet = 0;
	char			buf[64];
	int				nCompleteCode = 0;

	memset(buf, NULL, 64);

	sprintf( buf, "/SVC1/BDS/UDR%d.TEMP",	nSeq);

	cout << "buf = [" << buf << "]" << endl;

	buf[strlen(buf)] = NULL;

	cout << "buf = [" << buf << "]" << endl;

	clsTempFile.put_strFileName(buf);

	//	Open Temp File
	ifstream		UdrTempFile(clsTempFile.get_strFileName(), ios::in | ios::binary);
	if (!UdrTempFile)
	{
		////////////////////////////////////////////////////////////////
		cout << "Err Code#" << "\tUDR Temp File [" << clsTempFile.get_strFileName()
				<< "] couldn't open in writeBdsFile" << endl;
		////////////////////////////////////////////////////////////////
		return -1;
	}

	//	Get Udr Count from Temp File
	clsTempFile.get_nUdrCountFromFile();

	//	Make the BdsUdrFileName
	clsFiledata.make_BdsUdrFileName(clsFiledata.get_unFileSeqInConf());

	////////////////////////////////////////////////////////////////
	cout 	<< "BDS UDR File Name is " <<  clsFiledata.get_BdsUdrFileName()
			<< "Udr Count = "			<< clsTempFile.get_nUdrCount() << endl;
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
				cout << " i = " << i << "\tnRet = " << nRet << endl;
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
				cout << "UDR File(1) is on writing Block Seq #"
						<< clsBlockData.get_unBlockSeq() << endl;
				////////////////////////////////////////////////////////////////
				continue;
			}
			else if ( 	(nCompleteCode == 1) || (nRet == FLAG_FILE_MADE)	)
			{
				//	Initialize Total of Record Count in Data File
				clsBlockData.put_unTotalRecCount(0);

				////////////////////////////////////////////////////////////////
				cout << "UDR File#" << clsFiledata.get_unFileSeq()
						<< " is writed !!" << endl;
				////////////////////////////////////////////////////////////////

				//	Update Performance Data (End Transaction)
//				clsLogM->put_PerformanceData(	FLAG_END,   1,
//												clsFiledata.get_unFileSeq(),
//												clsTempFile.get_nUdrCountFromFile(),
//												0, 0);
				break;
			}
			else
			{
				cout << "Error No = [" << "]"
						<< "Error is occured, so exit !!" << endl;
				break;
			}
		}
	}	//end of while

	//	Write BDS UDR Data
	clsFiledata.put_strBdsFile();

	//	Update : Last Block Seq & File Seq
	clsBlockData.write_unBlockSeqInConf();
	clsFiledata.write_unFileSeqInConf(clsFiledata.get_unFileSeqInConf());
/*
	//	unlink	Tempfile
	if (clsTempFile.unlink_TempUdrFile() < 0)	{
		////////////////////////////////////////////////////////////////
		cout	<< "UDR Temp File UNLINK FAIL !!!" << endl;
		////////////////////////////////////////////////////////////////;
	}
*/
	UdrTempFile.close();

	return 0;
}


