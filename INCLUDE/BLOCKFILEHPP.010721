#ifndef BLOCKFILE_HPP
#define BLOCKFILE_HPP

#include <fstream.h>
#include <UdrBds.hpp>
#include <unistd.h>
#include <fcntl.h>

enum	FLAG_BLOCK	{	FLAG_BLOCK_NO_MADE,
						FLAG_BLOCK_MADE,
						FLAG_EMPTY_BLOCK_MADE
					};
enum	FLAG_FILE	{	FLAG_FILE_NO_MADE,
						FLAG_FILE_MADE,
						FLAG_EMPTY_FILE_MADE,
						FLAG_INITIAL
					};

/*
//	BDS Send Data File Name		(ex : BDS_UDR.11)
const char*		FILE_NAME_BDS_UDR		=	"../TransData/BDS/BDS_UDR.";
*/
//////////////////////////////////////////////////////////////////////////////////
//	class	CBlockConfigData
//////////////////////////////////////////////////////////////////////////////////
class	CBlockConfigData
{
	private:
		int				nBlockSize;			//	in
		int				nBlockHeaderSize;	//	in
		int				nRecSize;			//	in
		int				nMaxRecCount;
		unsigned int	unBlockSeq;			//	in
		char 			strCfgFileName[128];

	public:
		CBlockConfigData()	{}
		CBlockConfigData(const char* strFileName);
		~CBlockConfigData();

		int				get_nBlockSize()				{	return nBlockSize;			}
		int				get_nBlockHeaderSize()			{	return nBlockHeaderSize;	}
		int				get_nRecSize()					{	return nRecSize;			}
		int				get_nMaxRecCount()				{	return nMaxRecCount;		}
		unsigned int	get_unBlockSeq()				{	return unBlockSeq;			}

		void			put_nBlockSize		(int nValue){	nBlockSize 	= nValue;		}
		void			put_nBlockHeaderSize(int nValue){	nBlockHeaderSize = nValue;	}
		void			put_nRecSize		(int nValue){	nRecSize 	= nValue;		}
		void			put_nMaxRecCount	(int nValue){	nMaxRecCount= nValue;		}
		void			put_unBlockSeq		(unsigned int nValue);

		void			read_BlockConfigFile();
		void			read_BlockConfigFile(const char* strFileName);
		void			write_BlockConfigFile();
		void 			write_unBlockSeq();
};

//////////////////////////////////////////////////////////////////////////////////
//	class	CFileConfigData
//////////////////////////////////////////////////////////////////////////////////
class	CFileConfigData
{
	private:
		int				nFileHeaderSize;	//	in
		int				nOneFileSize;
		int				nOneBlockSize;		//	in
		int				nMaxBlockCount;		//	in
		unsigned int	unFileSeq;			//	in
		char 			strCfgFileName[128];

	public:
		CFileConfigData(const char* strFileName);
		~CFileConfigData();

		int				get_nFileHeaderSize()				{	return nFileHeaderSize;	}
		int				get_nOneFileSize()
		{
			put_nOneFileSize(nFileHeaderSize + (nOneBlockSize*nMaxBlockCount));
			return nOneFileSize;
		}
		int				get_nOneBlockSize()					{	return nOneBlockSize;	}
		int				get_nMaxBlockCount()				{	return nMaxBlockCount;	}
		unsigned int	get_unFileSeq()						{	return unFileSeq;		}

		void			put_nFileHeaderSize	(int nValue)	{	nFileHeaderSize	= nValue;	}
		void			put_nOneFileSize 	(int nValue)	{	nOneFileSize 	= nValue;	}
		void			put_nOneBlockSize   (int nValue)	{	nOneBlockSize	= nValue;	}
		void			put_nMaxBlockCount  (int nValue)	{	nMaxBlockCount	= nValue;	}
		void			put_unFileSeq     	(unsigned int unValue);

		void			read_FileConfigFile();
		void			read_FileConfigFile(const char* strFileName);
		void			write_FileConfigFile();
		void			write_unFileSeq(unsigned int);
};

//////////////////////////////////////////////////////////////////////////////////
//	class	CBlockData
//////////////////////////////////////////////////////////////////////////////////
class	CBlockData
{
	private:
		CBlockConfigData*	clsPtrConfig;		//	UDR Block Info
		CbdsUdrBlockHeader	clsBlockHeader;		//	UDR Block Header
		char*				strBillingData;		//	UDR Data in Block
		char*				strOneBlock;		//	A Block
		int					nTempRecCount;		//	Current Record Count
		int					nEmptyRecCount;		//	Current Empty Record Count
		unsigned int		unTotalRecCount;	//	Total Record Count.
		int					nBillDataLen;		//	UDR Data Length in Block

	public:
		CBlockData(const char* strCfgFileName);
		~CBlockData();

		char*			get_strOneBlock()		{	return strOneBlock;						}
		unsigned int 	get_unBlockSeq()		{	return clsBlockHeader.get_unBlockSeq();	}
		unsigned int 	get_unBlockSeqInConf()	{	return clsPtrConfig->get_unBlockSeq();	}
		int			 	get_nMaxRecCount()		{	return clsPtrConfig->get_nMaxRecCount();}

		unsigned int 	get_unTotalRecCount()	{	return unTotalRecCount;					}

		void		 	put_unBlockSeqInConf(unsigned int unValue)
												{	clsPtrConfig->put_unBlockSeq(unValue);	}
		void		 	write_unBlockSeqInConf(){	clsPtrConfig->write_unBlockSeq();	}
		int				put_strBdsBlock();
		int				put_strBdsBlock(char* strValue);
		void			put_unTotalRecCount(unsigned int unValue)	{	unTotalRecCount = unValue;	}
};

//////////////////////////////////////////////////////////////////////////////////
//	class	CUdrBlock
//////////////////////////////////////////////////////////////////////////////////
class CUdrBlock
{
	private:
		CbdsUdrBlockHeader	clsBlockHeader;
		char				strUdrData[1012];
	public:
		CUdrBlock(int nBlockSize = 1024)
		{
//			strUdrData = new char[(nBlockSize-sizeof(CbdsUdrFileHeader))+1];
			memset(strUdrData, NULL, 1012);
		}
//		~CUdrBlock()							{	delete strUdrData;	}

		void 			put_UdrBlock(char* strUdrBlock, int nBlockSize = 1024)
		{
			memcpy((char*)&clsBlockHeader, strUdrBlock, sizeof(CbdsUdrBlockHeader));
			memcpy(strUdrData,				&strUdrBlock[sizeof(CbdsUdrBlockHeader)],	\
											nBlockSize - sizeof(CbdsUdrBlockHeader));
		}

		unsigned int	get_unBlockSeq()		{	return clsBlockHeader.get_unBlockSeq();	}
		unsigned int	get_nRecCount()			{	return clsBlockHeader.get_nRecCount();	}

		char*			get_UdrBlock()			{	return strUdrData;	}
};

//////////////////////////////////////////////////////////////////////////////////
//	class	CFileData
//////////////////////////////////////////////////////////////////////////////////

class	CFileData
{
	private:
		CFileConfigData*	clsPtrConfig;		//	UDR File Conf Info
		CbdsUdrFileHeader	clsFileHeader;		//	UDR File Header
		char*				strBillingData;		//	UDR Data
		char*				strOneFile;			//	A File
		char*				strOneBlock;		//	A Block
		int					nTempBlockCount;	//	Current Block Count
		int					nBillDataBlockLen;	//	Length of UDR Data Block in File
		char				strFileName[128];	//	To Send Data File Name

	public:
		CFileData(const char* strCfgFileName);
		~CFileData();

		int 				put_strBdsFileBuffer(int nBlockSeqNum, char* strValue);
		int 				put_strBdsFile();
		CbdsUdrFileHeader&	get_clsFileHeader()		{	return	clsFileHeader;	}
		int					get_nBillDataBlockLen()	{	return	nBillDataBlockLen;	}
		char*				get_strBillingData()	{	return	strBillingData;		}
		void 				get_strBillingData(	int		 	nIndex,	\
											  	CUdrBlock*	clsUdrOneBlock);

		//	CFileConfigData's Accessor
		int					get_OneBlockSize()		{	return clsPtrConfig->get_nOneBlockSize();	}
		unsigned int 		get_unFileSeqInConf()	{	return clsPtrConfig->get_unFileSeq();		}
		int					get_nMaxBlockCount()	{	return clsPtrConfig->get_nMaxBlockCount();	}

		//	CbdsUdrFileHeader's Accessor
		unsigned int 		get_unFileSeq()			{	return clsFileHeader.get_unFileSeq();		}
		unsigned int 		get_unBlockCount()		{	return clsFileHeader.get_unBlockCount();	}
		unsigned int		get_unStartBlockSeq()	{	return clsFileHeader.get_unStartBlockSeq();	}
		unsigned int		get_unEndBlockSeq()		{	return clsFileHeader.get_unEndBlockSeq();	}
		unsigned int		get_LastWrittenUdrFileSeq();
		//	BDS UDR 파일 저장 디렉토리 :
		//	FILE_NAME_BDS_UDR = "../TransData/BDS/BDS_UDR.[unSeq]"
		void				make_BdsUdrFileName(unsigned int unSeq);
//		void				make_BdsUdrFileName(char* strBdsFileName, unsigned int unSeq);
		char*				get_BdsUdrFileName()	{	return strFileName;							}

		int					write_BdsUdrData(unsigned int unSeq);
		int					read_BdsUdrData(unsigned int unSeq);

		//smy 2000.12.5
		void		 	write_unFileSeqInConf(unsigned int unValue)		{	clsPtrConfig->write_unFileSeq(unValue);	}
};

//////////////////////////////////////////////////////////////////////////////////
//	class	CFileInfoData
//////////////////////////////////////////////////////////////////////////////////
class	CFileInfoData
{
	private:
		unsigned int	unFileSeq;
//		char			strDate[12];
		int				nStartBlockSeq;
		int				nEndBlockSeq;
		int				nStatus;
//		int				nResult;
		char			strInfoFileName[128];

	public:
		CFileInfoData(const char* strFileName);
		~CFileInfoData();

		unsigned int	get_unFileSeq()						{	return unFileSeq;			}
//		char*			get_strDate()						{	return strDate;				}
		int				get_nStartBlockSeq()				{	return nStartBlockSeq;		}
		int				get_nEndBlockSeq()					{	return nEndBlockSeq;		}
		int				get_nStatus()						{	return nStatus;				}
//		int				get_nResult()						{	return nResult;				}

		void			put_unFileSeq(unsigned int unValue)	{	unFileSeq		= unValue;	}
//		void			put_strDate(char* strValue)			{   strcpy(strDate, strValue);	}
		void			put_nStartBlockSeq(int nValue)		{   nStartBlockSeq	= nValue;	}
		void			put_nEndBlockSeq(int nValue)		{   nEndBlockSeq	= nValue;	}
		void			put_nStatus(int nValue)				{   nStatus			= nValue;	}
//		void			put_nResult(int nValue)				{   nResult			= nValue;	}

		void			put_FileInfoData(CbdsUdrFileHeader& clsUdrFileHeader, int nStatus);

		void			read_FileConfigFile();
		void			read_FileConfigFile(const char* strFileName);
		void			write_FileConfigFile();
};

class	TempUdrFile
{
	private:
		char	strFileName[64];
		long	nUdrCount;
		int		nRecordSize;

	public:
		TempUdrFile(int nRecSize);
		int		get_nUdrCount()		{	return nUdrCount;	}
		char*	get_strFileName()	{	return strFileName;	}
		long	get_TempFileSize();

		int		write_TempUdrFile(UdrFormat* strUdr);
		int		read_TempUdrFile(UdrFormat* strUdr, int nOffset);

		void	put_strFileName(const char *strValue);
		void 	get_nUdrCountFromFile();

		int unlink_TempUdrFile()	{	return unlink(strFileName);	}
};
#endif
