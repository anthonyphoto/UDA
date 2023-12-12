#ifndef SHAREDCONF_HPP
#define SHAREDCONF_HPP

//	Shared Memory Related

#ifdef	_SVC1
	const char*				SHM_KEY_FILE	    	=	"/SVC1/DATA/SharedMemory.cfg";
	const char*	            PATH_NAME_SHM_IPC	 	=   "/SVC1/DATA/ShmIPC.cfg";
#elif	_SVC2
	const char*				SHM_KEY_FILE	    	=	"/SVC2/DATA/SharedMemory.cfg";
	const char*	            PATH_NAME_SHM_IPC	 	=   "/SVC2/DATA/ShmIPC.cfg";
#endif

const unsigned char		SHM_ID_LOG				=	'A';
const unsigned char		SHM_ID_IPC				=	'B';


//	File Information	Related
const char*				PATH_PROJ_ROOT			=	"../";
const char*				PATH_CONF_FILE			=	"../cfg/";
const char*				PATH_LOG_FILE			=	"../log/";

#endif

/*
#ifdef	BLOCKFILE_HPP
extern const char*						SHM_KEY_FILE;
extern const char*	            		PATH_NAME_SHM_IPC;
extern const char*	            		PATH_NAME_BDS_BLOCK_CFG;
extern const char*	            		PATH_NAME_BDS_FILE_CFG;
extern const char*						FILE_NAME_BDS_UDR;
//extern static const unsigned char		SHM_ID_LOG;
//extern static const unsigned char		SHM_ID_IPC;
extern const char*						PATH_PROJ_ROOT;
extern const char*						PATH_CONF_FILE;
extern const char*						PATH_LOG_FILE;

#endif
*/
