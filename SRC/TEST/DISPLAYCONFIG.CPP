#include "DisplayConfig.hpp"

char FILE_NAME_COL_BDS_UDR[50];

int main(int argc, char* argv[])
{
	int		nSvcId = atoi(argv[1]);

	sprintf(PATH_NAME_BDS_BLOCK_CFG ,"/SVC%d/DATA/BDS_BlockInfo.cfg", nSvcId);
	sprintf(PATH_NAME_BDS_FILE_CFG  ,"/SVC%d/DATA/BDS_FileConf.cfg"	, nSvcId);
	sprintf(FILE_NAME_COL_BDS_UDR   ,"/SVC%d/BDS/BDS_UDR."          , nSvcId);

	CDisplayConfig	cDisplayConfig;

	cDisplayConfig.DisplayConfig();
}

