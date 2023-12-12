/******************************************************************
* 파 일 명     :  CommCfgFile.hpp	  			 				  
* 기   능      :  Memory DB에서 필요한 정보를 가지고 있는 config file을 다루는 class.														  
* 작 성 자     : Han Guen Hee( jameshans@hanmail.net )                    				  
* first data  : 2001. 03. 14      	     						  
* last updata : 2001. 05. 21   
* Version     : 1.1( 2001. 05. 21 )      	              		 		 				  
* program history                      						  
******************************************************************/
#ifndef COMMCFGFILE_HPP
#define COMMCFGFILE_HPP

#include <fstream.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#define 	LEN_PATH	80
#define		LEN_NAME	30

#define     CCF_DEBUG	1

class CommCfgFile
{
	public:
		CommCfgFile( const char* strFileName );
		~CommCfgFile();
		// Accessor
		char		get_cfgId( void )			{ return cfgId;	}
		char*		get_cfgPath( void )			{ return cfgPath;	}
		char*		get_backPath( void )		{ return backPath;	}
		char*		get_logPath( void )			{ return logPath;	}
		char* 		get_procName( void )		{ return procName;	}
		char*		get_cfgFileName( void )		{ return cfgFileName;	}
		char*		get_backupFileName( void )	{ return backupFileName;	}
		char*		get_logFileName( void )		{ return logFileName;	}
		int			get_MemberPoint( void )		{ return MemberPoint;	}
		int			get_MaxKeyMember( void )	{ return MaxKeyMember;	}
		
		void		set_cfgId( char cValue )
					{ cfgId = cValue; }
		void		set_cfgPath( char* strValue )
					{
						memset( cfgPath, NULL, sizeof( cfgPath ) );
						memcpy( cfgPath, strValue, strlen( strValue ) );
					}
		void		set_backPath( char* strValue )
					{
						memset( backPath, NULL, sizeof( backPath ) );
						memcpy( backPath, strValue, strlen( strValue ) );
					}
		void		set_logPath( char* strValue )
					{
						memset( logPath, NULL, sizeof( logPath ) );
						memcpy( logPath, strValue, strlen( strValue ) );
					}
		void		set_procName( char* strValue )
					{
						memset( procName, NULL, sizeof( procName ) );
						memcpy( procName, strValue, strlen( strValue ) );
					}
		void		set_cfgFileName( char* strValue )
					{
						memset( cfgFileName, NULL, sizeof( cfgFileName ) );
						memcpy( cfgFileName, strValue, strlen( strValue ) );
					}
		void		set_backupFileName( char* strValue )
					{
						memset( backupFileName, NULL, sizeof( backupFileName ) );
						memcpy( backupFileName, strValue, strlen( strValue ) );
					}
		void		set_logFileName( char* strValue )
					{
						memset( logFileName, NULL, sizeof( logFileName ) );
						memcpy( logFileName, strValue, strlen( strValue ) );
					}
		void		set_MemberPoint( int nValue )
					{ MemberPoint = nValue; }
		void		set_MaxKeyMember( int nValue )
					{ MaxKeyMember = nValue; }
		
		void		init( void )
					{
						cfgId = NULL;
						memset( cfgPath, NULL, sizeof( cfgPath ) );
						memset( backPath, NULL, sizeof( backPath ) );
						memset( logPath, NULL, sizeof( logPath ) );
						memset( procName, NULL, sizeof( procName ) );
						memset( cfgFileName, NULL, sizeof( cfgFileName ) );
						memset( backupFileName, NULL, sizeof( backupFileName ) );
						memset( logFileName, NULL, sizeof( logFileName ) );
						MemberPoint = 0;
						MaxKeyMember = 0;
					}
		void		print_all( void );			
		void		read_CommCfgFile( void );
		void		write_CommCfgFile( void );
	private:
		char		cfgId;						// shared memory key create ID
		char		cfgPath[LEN_PATH]; 			// shared memory key create PATH
		char		backPath[LEN_PATH];			// memory backup file path
		char		logPath[LEN_PATH];			// log file path
		char		procName[LEN_NAME];			// process name
		char		cfgFileName[LEN_NAME];		// config file name
		char		backupFileName[LEN_NAME];	// backup file name
		char		logFileName[LEN_NAME];		// log file name
		int			MemberPoint;				// Pointer Count
		int			MaxKeyMember;				// Max Node Count
		
		char		strFilePath[110];			// common config file path, name buffer
};


#endif


