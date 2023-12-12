#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>
#include <BlockFile.hpp>
#include <UdrBds.hpp>

char     PATH_NAME_BDS_BLOCK_CFG[50] ;                                     
char     PATH_NAME_BDS_FILE_CFG[50]  ;                                    
char     PATH_NAME_BDS_FILE_INFO[50] ;              

class CDisplayConfig
{
	private:
		int 				flag;
		CFileConfigData*	CFileConfig;
		CBlockConfigData*	CBlockConfig;
		CFileInfoData*		CFileInfo;
		
		int 				DisplayMainMenu(void);
		
		int 				DisplayFileMenu(void);
		int		 			DisplayBlockMenu(void);
		int		 			DisplayFileInfoMenu(void);
		
		void 				DisplayFileConfig( ostream &output );
		void 				DisplayBlockConfig( ostream &output );
		void 				DisplayFileInfo( ostream &output );
		
		void 				FileConfigInitialize( void );
		void 				UpdateFileHeaderSize( void );
		void		 		UpdateOneBlockSize( void );
		void 				UpdateMaxBlockCount( void );
		void 				UpdateFileSeq( void );
		
		void 				BlockConfigInitialize( void );
		void 				UpdateBlockHeaderSize( void );
		void 				UpdateBlockSize( void );
		void 				UpdateRecSize( void );
		void 				UpdateBlockSeq( void );

		void 				FileInfoInitialize( void );
		void 				UpdateunFileSeq( void );
		void		 		UpdatestrDate( void );
		void 				UpdatenStartBlockSeq( void );
		void 				UpdatenEndBlockSeq( void );
		void 				UpdatennStatus( void );
		void 				UpdatennResult( void );
	public:
		CDisplayConfig();
		~CDisplayConfig();
		void				DisplayConfig( void );

};

CDisplayConfig::CDisplayConfig()
{
	CFileConfig		= new CFileConfigData(PATH_NAME_BDS_FILE_CFG);
	CBlockConfig	= new CBlockConfigData(PATH_NAME_BDS_BLOCK_CFG);
	CFileInfo		= new CFileInfoData(PATH_NAME_BDS_FILE_INFO);
	
	flag = 1;
}
CDisplayConfig::~CDisplayConfig()
{
		delete CFileConfig;
		delete CBlockConfig;
}
void CDisplayConfig::DisplayConfig(void)
{
	while(flag)
	{
		switch( DisplayMainMenu() )
		{
			case 1: 
				switch( DisplayFileMenu() )
				{
					case 1:
						cout << "\33[2J\33[r";
    					DisplayFileConfig( cout);
						cout.flush();
                        FileConfigInitialize();
						break;
					case 2:
						cout << "\33[2J\33[r";
    					DisplayFileConfig( cout);
						cout.flush();
						UpdateFileHeaderSize();
						break;
					case 3:
						cout << "\33[2J\33[r";
    					DisplayFileConfig( cout);
						cout.flush();
						UpdateOneBlockSize();
						break;
					case 4:
						cout << "\33[2J\33[r";
    					DisplayFileConfig( cout);
						cout.flush();
						UpdateMaxBlockCount();
						break;
					case 5:
						cout << "\33[2J\33[r";
    					DisplayFileConfig( cout);
						cout.flush();
						UpdateFileSeq();
						break;
					case 6:
    					DisplayFileConfig( cout);
						cout.flush();
						getchar();
						break;
					case 7:
					default:
						break;
				};
				break;
			case 2:
				switch( DisplayBlockMenu() )
				{
					case 1:
						cout << "\33[2J\33[r";
                        DisplayBlockConfig( cout );
						cout.flush();
						BlockConfigInitialize();
						break;
					case 2:
						cout << "\33[2J\33[r";
                        DisplayBlockConfig( cout );
						cout.flush();
						UpdateBlockHeaderSize();
						break;
					case 3:
						cout << "\33[2J\33[r";
                        DisplayBlockConfig( cout );
						cout.flush();
						UpdateBlockSize();
						break;
					case 4:
						cout << "\33[2J\33[r";
                        DisplayBlockConfig( cout );
						cout.flush();
						UpdateRecSize();
						break;
					case 5:
						cout << "\33[2J\33[r";
                        DisplayBlockConfig( cout );
						cout.flush();
						UpdateBlockSeq();
						break;
					case 6:
                        DisplayBlockConfig( cout );
						cout.flush();
						getchar();
						break;
					case 7:
					default:
						break;
				};
				break;
			case 3:
				switch( DisplayFileInfoMenu() )
				{
					case 1:
						cout << "\33[2J\33[r";
                        DisplayFileInfo( cout );
						cout.flush();
						FileInfoInitialize();
						break;
					case 2:
						cout << "\33[2J\33[r";
                        DisplayFileInfo( cout );
						cout.flush();
						UpdateunFileSeq();
						break;
					case 3:
						cout << "\33[2J\33[r";
                        DisplayFileInfo( cout );
						cout.flush();
						UpdatenStartBlockSeq();
						break;
					case 4:
						cout << "\33[2J\33[r";
                        DisplayFileInfo( cout );
						cout.flush();
						UpdatenEndBlockSeq();
						break;
					case 5:
						cout << "\33[2J\33[r";
                        DisplayFileInfo( cout );
						cout.flush();
						UpdatennStatus();
						break;
					case 6:
						cout << "\33[2J\33[r";
                        DisplayFileInfo( cout );
						cout.flush();
						UpdatennResult();
						break;						
					case 7:
                        DisplayFileInfo( cout );
						cout.flush();
						getchar();
						break;
					case 8:
					default:
						break;
				};
				break;
			default:
				flag=0;
				break;
		}
	}
}
void CDisplayConfig::FileConfigInitialize( )
{
	int				nFileHeaderSize;
	int 			nOneBlockSize;
	int 			nMaxBlockCount;
	unsigned int	unFileSeq;

	cout << "Insert File Header Size: ";
	cin >> nFileHeaderSize;
	cout.flush();
	cout << "Insert One Block Size: ";
	cin >> nOneBlockSize;
	cout.flush();
	cout << "Insert Max Block Count: ";
	cin >> nMaxBlockCount;
	cout.flush();
	cout << "Insert File Sequence: ";
	cin >> unFileSeq;
	cout.flush();

	CFileConfig->put_nFileHeaderSize( nFileHeaderSize );
	CFileConfig->put_nOneBlockSize( nOneBlockSize );
	CFileConfig->put_nMaxBlockCount( nMaxBlockCount );
	CFileConfig->put_unFileSeq( unFileSeq );
	
	CFileConfig->write_FileConfigFile();
	CFileConfig->read_FileConfigFile();
	DisplayFileConfig( cout );
	cout << endl;
	cout.flush();
	getchar();
}
void CDisplayConfig::UpdateFileHeaderSize( )
{
	int				nFileHeaderSize;

	cout << "Insert File Header Size: ";
	cin >> nFileHeaderSize;
	cout.flush();

	CFileConfig->put_nFileHeaderSize( nFileHeaderSize );
	
	CFileConfig->write_FileConfigFile();
	CFileConfig->read_FileConfigFile();
	DisplayFileConfig( cout );
	cout << endl;
	cout.flush();
	getchar();
}
void CDisplayConfig::UpdateOneBlockSize( )
{
	int 			nOneBlockSize;

	cout << "Insert One Block Size: ";
	cin >> nOneBlockSize;
	cout.flush();

	CFileConfig->put_nOneBlockSize( nOneBlockSize );
	
	CFileConfig->write_FileConfigFile();
	CFileConfig->read_FileConfigFile();
	DisplayFileConfig( cout );
	cout << endl;
	cout.flush();
	getchar();
}
void CDisplayConfig::UpdateMaxBlockCount( )
{
	int 			nMaxBlockCount;

	cout << "Insert Max Block Count: ";
	cin >> nMaxBlockCount;
	cout.flush();

	CFileConfig->put_nMaxBlockCount( nMaxBlockCount );
	
	CFileConfig->write_FileConfigFile();
	CFileConfig->read_FileConfigFile();
	DisplayFileConfig( cout );
	cout << endl;
	cout.flush();
	getchar();
}
void CDisplayConfig::UpdateFileSeq( )
{
	unsigned int	unFileSeq;

	cout << "Insert File Sequence: ";
	cin >> unFileSeq;
	cout.flush();

	CFileConfig->put_unFileSeq( unFileSeq );
	
	CFileConfig->write_FileConfigFile();
	CFileConfig->read_FileConfigFile();
	DisplayFileConfig( cout );
	cout << endl;
	cout.flush();
	getchar();
}
void CDisplayConfig::BlockConfigInitialize( )
{
	int				nBlockHeaderSize;
	int 			nBlockSize;
	int 			nRecSize;
	unsigned int	unBlockSeq;

	cout << "Insert Block Header Size: ";
	cin >> nBlockHeaderSize;
	cout.flush();
	cout << "Insert Block Size: ";
	cin >> nBlockSize;
	cout.flush();
	cout << "Insert Record Size: ";
	cin >> nRecSize;
	cout.flush();
	cout << "Insert Block Sequence: ";
	cin >> unBlockSeq;
	cout.flush();

	CBlockConfig->put_nBlockHeaderSize( nBlockHeaderSize );
	CBlockConfig->put_nBlockSize( nBlockSize );
	CBlockConfig->put_nRecSize( nRecSize );
	CBlockConfig->put_unBlockSeq( unBlockSeq );
	
	CBlockConfig->write_BlockConfigFile();
	CBlockConfig->read_BlockConfigFile();
	DisplayBlockConfig( cout );
	cout << endl;
	cout.flush();
	getchar();
}
void CDisplayConfig::UpdateBlockHeaderSize( )
{
	int				nBlockHeaderSize;

	cout << "Insert Block Header Size: ";
	cin >> nBlockHeaderSize;
	cout.flush();

	CBlockConfig->put_nBlockHeaderSize( nBlockHeaderSize );
	
	CBlockConfig->write_BlockConfigFile();
	CBlockConfig->read_BlockConfigFile();
	DisplayBlockConfig( cout );
	cout << endl;
	cout.flush();
	getchar();
}
void CDisplayConfig::UpdateBlockSize( )
{
	int 			nBlockSize;

	cout << "Insert Block Size: ";
	cin >> nBlockSize;
	cout.flush();

	CBlockConfig->put_nBlockSize( nBlockSize );
	
	CBlockConfig->write_BlockConfigFile();
	CBlockConfig->read_BlockConfigFile();
	DisplayBlockConfig( cout );
	cout << endl;
	cout.flush();
	getchar();
}
void CDisplayConfig::UpdateRecSize( )
{
	int 			nRecSize;

	cout << "Insert Record Size: ";
	cin >> nRecSize;
	cout.flush();

	CBlockConfig->put_nRecSize( nRecSize );
	
	CBlockConfig->write_BlockConfigFile();
	CBlockConfig->read_BlockConfigFile();
	DisplayBlockConfig( cout );
	cout << endl;
	cout.flush();
	getchar();
}
void CDisplayConfig::UpdateBlockSeq( )
{
	unsigned int	unBlockSeq;

	cout << "Insert Block Sequence: ";
	cin >> unBlockSeq;
	cout.flush();

	CBlockConfig->put_unBlockSeq( unBlockSeq );
	
	CBlockConfig->write_BlockConfigFile();
	CBlockConfig->read_BlockConfigFile();
	DisplayBlockConfig( cout );
	cout << endl;
	cout.flush();
	getchar();
}

void CDisplayConfig::FileInfoInitialize( void )
{
	unsigned int	unFileSeq;
	int 			nStartBlockSeq;
	int 			nEndBlockSeq;
	int				nStatus;
	int				nResult;

	cout << "Insert File Seq: ";
	cin >> unFileSeq;
	cout.flush();
	cout << "Insert Start Block Seq: ";
	cin >> nStartBlockSeq;
	cout.flush();
	cout << "Insert End Block Seq: ";
	cin >> nEndBlockSeq;
	cout.flush();
	cout << "Insert nStatus: ";
	cin >> nStatus;
	cout.flush();	
	cout << "Insert nResult: ";
	cin >> nResult;
	cout.flush();

	CFileInfo->put_unFileSeq(unFileSeq);
	CFileInfo->put_nStartBlockSeq(nStartBlockSeq);
	CFileInfo->put_nEndBlockSeq(nEndBlockSeq);
	CFileInfo->put_nStatus(nStatus);
	CFileInfo->put_nResult(nResult);
	              		
	CFileInfo->write_FileConfigFile();
	CFileInfo->read_FileConfigFile();
	DisplayFileInfo( cout );
	cout << endl;
	cout.flush();
	getchar();
}

void CDisplayConfig::UpdateunFileSeq( void )
{
	unsigned int	unFileSeq;
		
	cout << "Insert File Seq: ";
	cin >> unFileSeq;
	cout.flush();
	
	CFileInfo->put_unFileSeq(unFileSeq);
	
	CFileInfo->write_FileConfigFile();
	CFileInfo->read_FileConfigFile();
	DisplayFileInfo( cout );
	cout << endl;
	cout.flush();
	getchar();
}

void CDisplayConfig::UpdatenStartBlockSeq( void )
{
	int 			nStartBlockSeq;
	int 			nEndBlockSeq;
	int				nStatus;
	int				nResult;

	cout << "Insert Start Block Seq: ";
	cin >> nStartBlockSeq;
	cout.flush();

	CFileInfo->put_nStartBlockSeq(nStartBlockSeq);

	CFileInfo->write_FileConfigFile();
	CFileInfo->read_FileConfigFile();
	DisplayFileInfo( cout );
	cout << endl;
	cout.flush();
	getchar();
}
void CDisplayConfig::UpdatenEndBlockSeq( void )
{
	int 			nEndBlockSeq;

	cout << "Insert End Block Seq: ";
	cin >> nEndBlockSeq;
	cout.flush();

	CFileInfo->put_nEndBlockSeq(nEndBlockSeq);
	              		
	CFileInfo->write_FileConfigFile();
	CFileInfo->read_FileConfigFile();
	DisplayFileInfo( cout );
	cout << endl;
	cout.flush();
	getchar();
}
void CDisplayConfig::UpdatennStatus( void )
{
	int				nStatus;

	cout << "Insert nStatus: ";
	cin >> nStatus;
	cout.flush();

	CFileInfo->put_nStatus(nStatus);
	              		
	CFileInfo->write_FileConfigFile();
	CFileInfo->read_FileConfigFile();
	DisplayFileInfo( cout );
	cout << endl;
	cout.flush();
	getchar();
}
void CDisplayConfig::UpdatennResult( void )
{
	int				nResult;

	cout << "Insert nResult: ";
	cin >> nResult;
	cout.flush();

	CFileInfo->put_nResult(nResult);
	              		
	CFileInfo->write_FileConfigFile();
	CFileInfo->read_FileConfigFile();
	DisplayFileInfo( cout );
	cout << endl;
	cout.flush();
	getchar();
}


int CDisplayConfig::DisplayMainMenu(void)
{
	int select;

	cout << "\33[2J\33[r";
	cout << "\n\n\n\n\n\n" << endl;
	cout << "\t\t******** Display Config Data ********\n" << endl;
	cout << "\t\t*                                   *\n" << endl;
	cout << "\t\t* 1. Display File Config Data       *\n" << endl;
	cout << "\t\t*                                   *\n" << endl;
	cout << "\t\t* 2. Display Block Config Data.     *\n" << endl;
	cout << "\t\t*                                   *\n" << endl;
	cout << "\t\t* 3. Display File Info Data.        *\n" << endl;
	cout << "\t\t*                                   *\n" << endl;
	cout << "\t\t* 4. Exit                           *\n" << endl;
	cout << "\t\t*                                   *\n" << endl;
	cout << "\t\t*************************************\n" << endl;
	cout << "\n" << endl;
	cout << "\t\tselect menu: ";
	cin >> select;
	return select;
}
int CDisplayConfig::DisplayFileMenu(void)
{
	int select;

	cout << "\33[2J\33[r";
	cout << "\n" << endl;
	cout << "\t\t***** Display File Config Data ******\n" << endl;
	cout << "\t\t* 1. Initialize                     *\n" << endl;
	cout << "\t\t* 2. Update FileHeaderSize          *\n" << endl;
	cout << "\t\t* 3. Update OneBlockSize            *\n" << endl;
	cout << "\t\t* 4. Update MaxBlockCount           *\n" << endl;
	cout << "\t\t* 5. Update FileSeq                 *\n" << endl;
	cout << "\t\t* 6. Display Current Information    *\n" << endl;
	cout << "\t\t* 7. Back                           *\n" << endl;
	cout << "\t\t*************************************\n" << endl;
	cout << "\n" << endl;
	cout << "\t\tselect menu: ";
	cin >> select;
	return select;
}
int CDisplayConfig::DisplayBlockMenu(void)
{
	int select;

	cout << "\33[2J\33[r";
	cout << "\n" << endl;
	cout << "\t\t***** Display Block Config Data *****\n" << endl;
	cout << "\t\t* 1. Initialize                     *\n" << endl;
	cout << "\t\t* 2. Update BlockHeaderSize         *\n" << endl;
	cout << "\t\t* 3. Update BlockSize               *\n" << endl;
	cout << "\t\t* 4. Update RecordSize              *\n" << endl;
	cout << "\t\t* 5. Update BlockSeq                *\n" << endl;
	cout << "\t\t* 6. Display Current Information    *\n" << endl;
	cout << "\t\t* 7. Back                           *\n" << endl;
	cout << "\t\t*************************************\n" << endl;
	cout << "\n" << endl;
	cout << "\t\tselect menu: ";
	cin >> select;
	return select;
}

int CDisplayConfig::DisplayFileInfoMenu(void)
{
	int select;

	cout << "\33[2J\33[r";
	cout << "\n" << endl;
	cout << "\t\t***** Display Block Config Data *****\n" << endl;
	cout << "\t\t* 1. Initialize                     *\n" << endl;
	cout << "\t\t* 2. Update unFileSeq               *\n" << endl; 
	cout << "\t\t* 3. Update nStartBlockSeq          *\n" << endl;
	cout << "\t\t* 4. Update nEndBlockSeq            *\n" << endl; 
	cout << "\t\t* 5. Update nStatus                 *\n" << endl; 
	cout << "\t\t* 6. Update nResult                 *\n" << endl; 
	cout << "\t\t* 7. Display Current Information    *\n" << endl;       
	cout << "\t\t* 8. Back                           *\n" << endl;
	cout << "\t\t*************************************\n" << endl;
	cout << "\n" << endl;
	cout << "\t\tselect menu: ";
	cin >> select;
	return select;
}


void CDisplayConfig::DisplayFileConfig( ostream &output )
{
	output 	<< endl;
	output 	<< "\t\t ***************************" << endl;
	output 	<< "\t\t * Information File Config *" << endl;
	output 	<< "\t\t ***************************" << endl;
	output 	<< endl;
	output 	<< "========================================================================" << '\n';
	output 	<< setiosflags( ios::left ) 
			<< setw( 18 ) << "FileHeaderSize" 
			<< setw( 15 ) << "OneFileSize" 
			<< setw( 15 ) << "OneBlockSize" 
			<< setw( 15 ) << "MaxBlockSize" 
			<< setw( 15 ) << "FileSeq" 
			<< '\n';
	output 	<< setiosflags( ios::left ) 
			<< setw( 18 ) << CFileConfig->get_nFileHeaderSize() 
   			<< setw( 15 )  << CFileConfig->get_nOneFileSize()  
	    	<< setw( 15 )  << CFileConfig->get_nOneBlockSize()
	    	<< setw( 15 )  << CFileConfig->get_nMaxBlockCount()
        	<< setw( 15 )  << CFileConfig->get_unFileSeq() 
        	<< '\n';
	output 	<< "========================================================================" << '\n';
	output 	<< "Press anykey !!!!" << endl;
}

unsigned int	unFileSeq;
char			strDate[12];
int				nStartBlockSeq;
int				nEndBlockSeq;
int				nStatus;
int				nResult;


void CDisplayConfig::DisplayFileInfo( ostream &output )
{
	output 	<< endl;
	output 	<< "\t\t ***************************" << endl;
	output 	<< "\t\t * Information File Info   *" << endl;
	output 	<< "\t\t ***************************" << endl;
	output 	<< endl;
	output 	<< "========================================================================" << '\n';
	output 	<< setiosflags( ios::left ) 
			<< setw( 18 ) << "unFileSeq" 
			<< setw( 10 ) << "strDate" 
			<< setw( 15 ) << "nStartBlockSeq" 
			<< setw( 15 ) << "nEndBlockSeq" 
			<< setw( 15 ) << "nStatus" 
			<< setw( 15 ) << "nResult" 
			<< '\n';
	output 	<< setiosflags( ios::left ) 
			<< setw( 18 )  << CFileInfo->get_unFileSeq()
   			<< setw( 10 )  << CFileInfo->get_strDate()  
	    	<< setw( 15 )  << CFileInfo->get_nStartBlockSeq()
	    	<< setw( 15 )  << CFileInfo->get_nEndBlockSeq()
        	<< setw( 15 )  << CFileInfo->get_nStatus()
        	<< setw( 15 )  << CFileInfo->get_nResult()
        	<< '\n';
	output 	<< "========================================================================" << '\n';
	output 	<< "Press anykey !!!!" << endl;
}

void CDisplayConfig::DisplayBlockConfig( ostream &output )
{
	output 	<< endl;
	output 	<< "\t\t ****************************" << endl;
	output 	<< "\t\t * Information Block Config *" << endl;
	output 	<< "\t\t ****************************" << endl;
	output 	<< endl;
	output 	<< "========================================================================" << '\n';
	output 	<< setiosflags( ios::left )
			<< setw( 18 ) << "BlockHeaderSize" 
			<< setw( 15 ) << "BlockSize"
			<< setw( 15 ) << "RecSize" 
			<< setw( 15 ) << "MaxRecSize"
			<< setw( 15 ) << "BlockSeq"
			<< '\n';
	output 	<< setiosflags( ios::left )
			<< setw( 18 ) << CBlockConfig->get_nBlockHeaderSize() 
   			<< setw( 15 )  << CBlockConfig->get_nBlockSize()  
	    	<< setw( 15 )  << CBlockConfig->get_nRecSize()
	    	<< setw( 15 )  << CBlockConfig->get_nMaxRecCount()
        	<< setw( 15 )  << CBlockConfig->get_unBlockSeq()
        	<< '\n';
	output 	<< "========================================================================" << '\n';
	output << "Press anykey !!!!" << endl;
}
