#include <CommCfgFile.hpp>

CommCfgFile::CommCfgFile( const char* strFileName )
{
	if( CCF_DEBUG )
	{
		cout << "[CommCfgFile] Constructor" << endl;
	}
	sprintf( strFilePath, "%s", strFileName );
	
	init();
	if( CCF_DEBUG )
	{
		cout << "[CommCfgFile] strFilePath= " << strFilePath << endl;
	}
}
CommCfgFile::~CommCfgFile()
{
}
void CommCfgFile::read_CommCfgFile( void )
{
	ifstream	ConfigFile( strFilePath, ios::binary);

	if( !ConfigFile )
	{
		cout << "[CommCfgFile][read_CommCfgFile] Config File Open fail" << endl;
		exit( 1 );
	}
	ConfigFile.read( (char*)this, sizeof( CommCfgFile ) );
	ConfigFile.close();
}
void CommCfgFile::write_CommCfgFile( void )
{
	ofstream	ConfigFile( strFilePath, ios::binary );

	if( !ConfigFile )
	{
		cout << "[CommCfgFile][write_CommCfgFile] Config File Open fail" << endl;
		exit( 1 );
	}
	ConfigFile.write( reinterpret_cast<const char *>( this ), sizeof( CommCfgFile ) );
	ConfigFile.close();
}
void CommCfgFile::print_all( void )
{
	cout << "=========== CommCfgFile Member =============" << endl;		
	cout << "cfgId=[" << cfgId << "]" << endl;
	cout << "cfgPath=[" << cfgPath << "]" << endl;
	cout << "backPath=[" << backPath << "]" << endl;
	cout << "logPath=[" << logPath << "]" << endl;
	cout << "procName=[" << procName << "]" << endl;
	cout << "cfgFileName=[" << cfgFileName << "]" << endl;
	cout << "backupFileName=[" << backupFileName << "]" << endl;
	cout << "logFileName=[" << logFileName << "]" << endl;
	cout << "MemberPoint=[" << MemberPoint << "]" << endl;
	cout << "MaxKeyMember=[" << MaxKeyMember << "]" << endl;
	cout << "===========================================" << endl;	
}

