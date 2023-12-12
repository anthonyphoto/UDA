#include "TempUdrFile.hpp"


TempUdrFile::TempUdrFile(int nRecSize)
{
	sprintf(strFileName, "%s.TEMP", FILE_NAME_COL_BDS_UDR);
	nUdrCount = get_TempFileSize() / nRecSize;
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
	ofstream	UdrTempFile(strFileName, ios::app | ios::binary);
	if (!UdrTempFile)
		return -1;

	UdrTempFile.write(reinterpret_cast<const char*>(strUdr), sizeof(UdrFormat));
	UdrTempFile.flush();
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
