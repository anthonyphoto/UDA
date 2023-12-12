#include <stdio.h>
#include <unistd.h>
#include <fstream.h>
#include <fcntl.h>
#include <UDR.hpp>

/*
#ifdef	_SVC1
	const char*		FILE_NAME_COL_BDS_UDR		=	"/SVC1/BDS/BDS_UDR.";
#elif	_SVC2
	const char*		FILE_NAME_COL_BDS_UDR		=	"/SVC2/BDS/BDS_UDR.";
#endif
*/
class	TempUdrFile
{
	private:
		char	strFileName[64];
		long	nUdrCount;

	public:
		TempUdrFile(int nRecSize);
		int		get_nUdrCount()		{	return nUdrCount;	}
		char*	get_strFileName()	{	return strFileName;	}
		long	get_TempFileSize();

		int		write_TempUdrFile(UdrFormat* strUdr);
		int		read_TempUdrFile(UdrFormat* strUdr, int nOffset);


		//smy 2000.12.5
		void	clear_TempUdrFile()	{	memset((void *)this, 0x00, sizeof(TempUdrFile));	}
		
};