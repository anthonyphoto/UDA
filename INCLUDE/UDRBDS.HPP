#ifndef	UDRBDS_HPP
#define	UDRBDS_HPP

#include <string.h>
#include <UDR.hpp>
//#include "TempConfigData.hpp"

//	Associated with BDS UDR Format
//	const int	LEN_BLOCK_HEADER						= 12;
//	const int	LEN_ONE_BDS_BLOCK						= 1024;
//	const int	LEN_BDS_UDR								= 309;

class CBDS_UdrFormat
{
	private:
		STUDRITEMPTR	stPtrUdrItem;
		int				nUdrCount;
		char			strBdsUdr[512];
		char			strTempBdsUdr[128];
	
	public:
		CBDS_UdrFormat()		
		{
			nUdrCount = CNT_UDR_ITEM;		//	CNT_UDR_ITEM = 57
			stPtrUdrItem = new STUDRITEM[nUdrCount];
			
			memset ((char*)stPtrUdrItem, NULL, sizeof(STUDRITEM)*nUdrCount);
			for (int i = 0; i < nUdrCount; i++)
				memcpy((char*)&stPtrUdrItem[i], (char*)&stConfigUdr[i], sizeof(STUDRITEM));

			memset(strBdsUdr, NULL, sizeof(strBdsUdr));
		}
		~CBDS_UdrFormat()	{	delete [] stPtrUdrItem;				}

		void 	conv_A_TYPE	(A_INT_TYPE*	stPtrItemA);
		void 	conv_B_TYPE	(B_TYPE* 		stPtrItemB);
		void 	conv_C_TYPE	(C_TYPE* 		stPtrItemC);
		void 	conv_D_TYPE	(D_TYPE* 		stPtrItemD);
		void 	conv_E_TYPE	(E_TYPE* 		stPtrItemE);
		void 	conv_F_TYPE	(F_TYPE* 		stPtrItemF);
		void 	conv_G_TYPE	(G_TYPE* 		stPtrItemG);
		void 	conv_H_TYPE	(H_TYPE* 		stPtrItemH);
		void 	conv_I_TYPE	(I_TYPE* 		stPtrItemI);
		void 	conv_Y_TYPE	(Y_TYPE* 		stPtrItemY);
		void 	conv_Z_TYPE	(Z_TYPE* 		stPtrItemZ);

		void 	conv_BDS_UDR(UdrFormat& UdrForm);
		void	conv_UdrData		(int 			strUdrData,\
									 const char*	strUdrType);
		void	conv_UdrData		(char*			strUdrData,\
							 		 const char* 	strUdrType	);
		void	conv_UdrData		(unsigned int	nIpAddr,
									 const char* 	strUdrType);
		void 	conv_UdrData		(time_t 		tTimeStamp,
									 const char* 	strUdrType);
		void	conv_IntMin2StrMin	(unsigned int	nIpAddr);
		int		getOffsetNlength	(const char* strItemType);
		int		conv_Int2strValue	(int nValue, int nLenth);
		
		char*	get_strBdsUdr()	{	return strBdsUdr;	}
		int		get_nUdrCount()	{	return nUdrCount;	}
		
		void	display_UdrItem(char*	strBlockOfBillData, int nRecSize);
};

class	CbdsUdrBlockHeader
{
	private:
		short			sBlockId;
		short			sDummy;
		unsigned int	unBlockSeq;
		unsigned int	nRecCount;
	public:
		CbdsUdrBlockHeader()
		{
			sBlockId 	= 0x4244;		//	BD
			sDummy 		= 0;
			unBlockSeq 	= nRecCount = 0;
		}	
		unsigned int	get_unBlockSeq()					{	return unBlockSeq;		}
		unsigned int	get_nRecCount()						{	return nRecCount;		}
		void			put_unBlockSeq(unsigned int unValue){	unBlockSeq = unValue;	}
		void			put_nRecCount(unsigned int unValue)	{	nRecCount = unValue;	}
};

class CbdsUdrFileHeader
{
	private:
		unsigned int	unFileID;
		unsigned int	unFileSeq;
		unsigned int	unBlockCount;
		unsigned int	unStartBlockSeq;
		unsigned int	unEndBlockSeq;
	public:
		CbdsUdrFileHeader()	{	unFileID = 0x46440000;	}	//	FD + NULL *2
		
		unsigned int 	get_unFileID()								{	return unFileID;		}
		unsigned int 	get_unFileSeq()								{	return unFileSeq;		}
		unsigned int 	get_unBlockCount()							{	return unBlockCount;	}
		unsigned int	get_unStartBlockSeq()						{	return unStartBlockSeq;	}
		unsigned int	get_unEndBlockSeq()							{	return unEndBlockSeq;	}

		void			put_unFileID(unsigned int unValue)			{	unFileID 		= unValue;	}
		void			put_unFileSeq(unsigned int unValue)			{	unFileSeq 		= unValue;	}
		void			put_unBlockCount(unsigned int unValue)		{	unBlockCount 	= unValue;	}
		void			put_unStartBlockSeq(unsigned int unValue)	{	unStartBlockSeq = unValue;	}
		void			put_unEndBlockSeq(unsigned int unValue)		{	unEndBlockSeq 	= unValue;	}
};

#endif
