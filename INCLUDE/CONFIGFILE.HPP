#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

//////////////////////////
//	BDS Environment Info
//////////////////////////

#ifdef	_SVC1
//	UDR Block Information
const char*	    PATH_NAME_BDS_BLOCK_CFG			=   "/SVC1/DATA/BDS_BlockInfo.cfg";

//	UDR File Information
const char*	    PATH_NAME_BDS_FILE_CFG			=   "/SVC1/DATA/BDS_FileConf.cfg";

//	UDR File writing Information
const char*	    PATH_NAME_BDS_FILE_INFO			=   "/SVC1/DATA/BDS_FileInfo.cfg";

//	BDS Interface writing Information
const char*	    PATH_NAME_BDS_FILE_BLOCK_INFO	=   "/SVC1/DATA/BDS_FileBlock.cfg";

#elif	_SVC2
//	UDR Block Information
const char*	    PATH_NAME_BDS_BLOCK_CFG			=   "/SVC2/DATA/BDS_BlockInfo.cfg";

//	UDR File Information
const char*	    PATH_NAME_BDS_FILE_CFG			=   "/SVC2/DATA/BDS_FileConf.cfg";

//	UDR File writing Information
const char*	    PATH_NAME_BDS_FILE_INFO			=   "/SVC2/DATA/BDS_FileInfo.cfg";

//	BDS Interface writing Information
const char*	    PATH_NAME_BDS_FILE_BLOCK_INFO	=   "/SVC2/DATA/BDS_FileBlock.cfg";
#endif

#endif






