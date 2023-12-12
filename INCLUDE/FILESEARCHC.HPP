#include <iostream.h>
#include <stdlib.h>    // exit()
#include <dirent.h>    // opendir()
#include <sys/stat.h>  // mkdir()
#include <unistd.h>    // link(), unlink()
#include <string.h>    
#include <fcntl.h>     //open()
#include <fstream.h>   //ifstream, ofstream
#include <stdio.h>     //scptid

#define FILE_MAXLEN  60
#define CMD_MAXLEN  100

#define FILE_EXIST      1
#define FILE_NOT_EXIST -1

#define FILE_FLAG_INITIAL 0xffffffff
#define FILE_FLAG_USED    0

class fileSearchC
{
	public:	
		 fileSearchC();
		~fileSearchC();
		int getFile(char *FileName);
	private : 
		char *FileName;	
};	

fileSearchC::fileSearchC()
{	
}

fileSearchC::~fileSearchC()
{
	delete [] FileName;
}		

int fileSearchC::getFile(char *FileName)
{
	// access return success 0 / fail -1
	if (access(FileName,F_OK) == 0)
	{
		//performancecout << "[NOTICE] File [" << FileName << "] found!" << endl;
		return FILE_EXIST;
	}
	else
	{
		//performancecout << "[WARNNING] File not found!" << endl;		
		return FILE_NOT_EXIST; 
	}	
}


//전역변수로 선언
char	strCfgFileName[128];











///////////////////////////////////////////////////////////
// /SVC#/DATA/BDS_FileBlock.cfg
///////////////////////////////////////////////////////////
class	BdsFileBlock
{
	private:
		unsigned int	unFileNo;		//in
		unsigned int	unStartBlockNo;	//in
		unsigned int	unEndBlockNo;	//in
		unsigned int	unFlag;			//in
		unsigned int	unTimeInVal;	//in
		unsigned int	unMaxBillSize;	//in
	public:
//		char 			strCfgFileName[128];
			
		BdsFileBlock()	{}
		BdsFileBlock(const char* strFileName);
		~BdsFileBlock();
		
		unsigned int	getFileNo()				{	return unFileNo;		}
		unsigned int	getStartBlockNo()		{	return unStartBlockNo;	}
		unsigned int	getEndBlockNo()			{	return unEndBlockNo;	}
		unsigned int	getFlag()				{	return unFlag;			}
		unsigned int	getTimeInVal()			{	return unTimeInVal;		}
		unsigned int	getMaxBillSize()		{	return unMaxBillSize;	}
		
		void			setFileNo      (unsigned int unValue) {	unFileNo		= unValue;}
		void			setStartBlockNo(unsigned int unValue) {	unStartBlockNo	= unValue;}
		void			setEndBlockNo  (unsigned int unValue) {	unEndBlockNo	= unValue;}
		void			setFlag        (unsigned int unValue) {	unFlag			= unValue;}
		void			setTimeInVal   (unsigned int unValue) {	unTimeInVal		= unValue;}
		void			setMaxBillSize (unsigned int unValue) {	unMaxBillSize	= unValue;}
		
		void			read_BdsFileBlock();
		void			read_BdsFileBlock(const char* strFileName);
		void			write_BdsFileBlock();
		
		void			initial_BdsFileBlock();
		void			display_BdsFileBlock();
};


BdsFileBlock::BdsFileBlock(const char* strFileName)
{
	memset(strCfgFileName, 0x00, sizeof(strCfgFileName));
	strcpy(strCfgFileName, strFileName);
	
	setFileNo(4294967295);
	setStartBlockNo(4294967295);
	setEndBlockNo(4294967295);
	setFlag(FILE_FLAG_INITIAL);
	setTimeInVal(30);			//DEFAULT 15분 900 => 30초
	setMaxBillSize(1048576);	//1M
}

BdsFileBlock::~BdsFileBlock()
{
}

void BdsFileBlock::read_BdsFileBlock()
{
	ifstream	fin(strCfgFileName, ios::binary);
	
	if (fin)
	{
		fin.read((char*)this, sizeof(BdsFileBlock));
	}
	else	// 파일 오픈 실패시 파일을 초기화 함.
	{
		//cout << "파일 초기화를 합니당~~" << endl;
		setFileNo      (4294967295);
		setStartBlockNo(4294967295);
		setEndBlockNo  (4294967295);
		setFlag        (FILE_FLAG_INITIAL);
		setTimeInVal   (30);	    		//DEFAULT 15분 900 => 30초
		setMaxBillSize (1048576);			//1M
		write_BdsFileBlock();
	}
	fin.close();	
}


void BdsFileBlock::read_BdsFileBlock(const char* strFileName)
{
	cout << "strFileName =>" << strFileName << endl;
	ifstream	fin(strFileName, ios::binary);
	
	if (fin)
	{
		cout << "read_BdsFileBlock(const char* strFileName) file exist" << endl;
		fin.read((char*)this, sizeof(BdsFileBlock));
	}
	else
	{
		cout << "[WARNING] read_BdsFileBlock(const char* strFileName) file not exist" << endl;
		setFileNo(4294967295);
		setStartBlockNo(4294967295);
		setEndBlockNo(4294967295);
		setFlag(FILE_FLAG_INITIAL);
		setTimeInVal(30);			//DEFAULT 15분 900 => 30초
		setMaxBillSize(1048576);	//1M
		write_BdsFileBlock();
	}
	fin.close();	
}

void BdsFileBlock::write_BdsFileBlock()
{
	ofstream	fout(strCfgFileName, ios::binary);

	//cout << "**********************" << endl;
	//cout << "strCfgFileName = " << strCfgFileName << endl;
	//cout << "**********************" << endl;

	fout.write(reinterpret_cast<const char *>(this), sizeof(BdsFileBlock));
	fout.close();

//	if (fout)
//	{
//		//cout << "write_BdsFileBlock() OK" << endl;
//		//display_BdsFileBlock();
//	}
//	else
//	{
//		//cout << "	write_BdsFileBlock() FAIL FAIL" << endl;
//	}
}

void BdsFileBlock::initial_BdsFileBlock()
{
	setFileNo(4294967295);
	setStartBlockNo(4294967295);
	setEndBlockNo(4294967295);
	setFlag(FILE_FLAG_INITIAL);
	setTimeInVal(30);			//DEFAULT 15분 900 => 30초
	setMaxBillSize(1048576);	//1M
	write_BdsFileBlock();
}

void BdsFileBlock::display_BdsFileBlock()
{
	read_BdsFileBlock();
	cout << endl;
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	cout << "strCfgFileName : " << strCfgFileName << endl;
	cout << "unFileNo       : " << unFileNo << endl;
	cout << "unStartBlockNo : " << unStartBlockNo << endl;
	cout << "unEndBlockNo   : " << unEndBlockNo << endl;
	cout << "unFlag         : " << unFlag << endl;
	cout << "unTimeInVal    : " << unTimeInVal << endl;
	cout << "unMaxBillSize  : " << unMaxBillSize << endl;
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
}









///////////////////////////////////////////////////////////
// /SVC#/DATA/RADIUS#_FileBlock[P/S].cfg
///////////////////////////////////////////////////////////
class	RadiusFileBlock
{
	private:
		unsigned int	unStartBlockNo;	//in
		unsigned int	unTimeInVal;	//in
//		char 			strCfgFileName[128];

	public:
		RadiusFileBlock()	{}
		RadiusFileBlock(const char* strFileName);
		~RadiusFileBlock();
		
		unsigned int	getStartBlockNo()		{	return unStartBlockNo;	}
		unsigned int	getTimeInVal()			{	return unTimeInVal;		}
		
		void			setStartBlockNo(unsigned int unValue){	unStartBlockNo	= unValue;}
		void			setTimeInVal(unsigned int unValue)	{	unTimeInVal		= unValue;}
				
		void			read_RadiusFileBlock();
		void			write_RadiusFileBlock();
		
		void			initial_RadiusFileBlock();
		void			display_RadiusFileBlock();
};


RadiusFileBlock::RadiusFileBlock(const char* strFileName)
{
	strcpy(strCfgFileName, strFileName);
	setStartBlockNo(4294967295);
	setTimeInVal(30);			//DEFAULT 15분 900 => 30초
}

RadiusFileBlock::~RadiusFileBlock()
{
//	write_RadiusFileBlock();
}

void RadiusFileBlock::read_RadiusFileBlock()
{
	ifstream	fin(strCfgFileName, ios::binary);
	
	if (fin)
	{
		//cout << "read_RadiusFileBlock() file exist" << endl;
		fin.read((char*)this, sizeof(RadiusFileBlock));
		fin.close();
	}
	else
	{
		//cout << "[WARNING] read_RadiusFileBlock() file not exist" << endl;
		setStartBlockNo(4294967295);
		setTimeInVal(30);			//DEFAULT 15분 900 => 30초
		write_RadiusFileBlock();
	}
}

void RadiusFileBlock::write_RadiusFileBlock()
{
	ofstream	fout(strCfgFileName, ios::binary);
	
	fout.write(reinterpret_cast<const char *>(this), sizeof(RadiusFileBlock));
	fout.close();
}

void RadiusFileBlock::initial_RadiusFileBlock()
{
	setStartBlockNo(4294967295);
	setTimeInVal(30);			//DEFAULT 15분 900 => 30초
	write_RadiusFileBlock();
}

void RadiusFileBlock::display_RadiusFileBlock()
{
	read_RadiusFileBlock();
	cout << endl;
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	cout << "strCfgFileName : " << strCfgFileName << endl;
	cout << "unStartBlockNo : " << unStartBlockNo << endl;
	cout << "unTimeInVal    : " << unTimeInVal << endl;
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
}












///////////////////////////////////////////////////////////
// /SVC#/DATA/RADIUS#_FileNo#.cfg
///////////////////////////////////////////////////////////
class	FilterFileNo
{
	private:
		unsigned int	unStartBlockNo;	//in
//		char 			strCfgFileName[128];

	public:
		FilterFileNo()	{}
		FilterFileNo(const char* strFileName);
		~FilterFileNo();
		
		unsigned int	getStartBlockNo()		{	return unStartBlockNo;	}
		
		void			setStartBlockNo(unsigned int unValue){	unStartBlockNo	= unValue;}
				
		void			read_FilterFileNo();
		void			write_FilterFileNo();
		
		void			initial_FilterFileNo();
		void			display_FilterFileNo();
};


FilterFileNo::FilterFileNo(const char* strFileName)
{
	strcpy(strCfgFileName, strFileName);
	setStartBlockNo(4294967295);
}

FilterFileNo::~FilterFileNo()
{
//	write_FilterFileNo();
}

void FilterFileNo::read_FilterFileNo()
{
	ifstream	fin(strCfgFileName, ios::binary);
	
	if (fin)
	{
		//cout << "read_FilterFileNo() file exist" << endl;
		fin.read((char*)this, sizeof(FilterFileNo));
		fin.close();
	}
	else
	{
		//cout << "[WARNING] read_FilterFileNo() file not exist" << endl;
		setStartBlockNo(4294967295);
		write_FilterFileNo();
	}
}

void FilterFileNo::write_FilterFileNo()
{
	ofstream	fout(strCfgFileName, ios::binary);
	
	fout.write(reinterpret_cast<const char *>(this), sizeof(FilterFileNo));
	fout.close();
}

void FilterFileNo::initial_FilterFileNo()
{
	setStartBlockNo(4294967295);
	write_FilterFileNo();
}

void FilterFileNo::display_FilterFileNo()
{
	read_FilterFileNo();
	cout << endl;
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	cout << "strCfgFileName : " << strCfgFileName << endl;
	cout << "unStartBlockNo : " << unStartBlockNo << endl;
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
}










///////////////////////////////////////////////////////////
// /SVC#/DATA/SCPIF[R/S]_TID.cfg
///////////////////////////////////////////////////////////
class	ScpIF_TID
{
	private:
		
		char 	PATH_NAME_SCPIFR_TID[80];	//	=   "/SVC#/DATA/SCPIF_TID.cfg";
		char 	PATH_NAME_SCPIFS_TID[80];	//	=   "/SVC#/DATA/SCPIF_TID.cfg";
		
		char	TDay[10];			// date
 		char	STDay[10];			// date
		
	public:
		ScpIF_TID(int serviceid);
		~ScpIF_TID(){;}

		unsigned int	get_S_TID(void);	  
		unsigned int	get_R_TID(void);
		
		unsigned int	get_S_Day(void)
		{
			get_S_TID();
			return atol(TDay);
		}	  
		unsigned int	get_R_Day(void)
		{
			get_R_TID();
			return atol(STDay);
		}
		
		void			set_S_TID(unsigned int stid, unsigned int idate);
		void			set_R_TID(unsigned int rtid, unsigned int idate);
		
		void			init_TID(void)
		{
			set_S_TID(0,0);	
			set_R_TID(0,0);	
		}
		void			display_TID(void)
		{
			cout << "Schannel Transaction-ID[" << TDay << "] : " << get_S_TID() << endl;
			cout << "Rchannel Transaction-ID[" << STDay << "] : " << get_R_TID() << endl;
		}
};


ScpIF_TID::ScpIF_TID(int serviceid)
{
	
	sprintf(PATH_NAME_SCPIFS_TID, "/SVC%d/DATA/SCPIFS_TID.cfg", serviceid);
	sprintf(PATH_NAME_SCPIFR_TID, "/SVC%d/DATA/SCPIFR_TID.cfg", serviceid);
	memset(TDay, 0x00, sizeof(TDay));
	memset(STDay, 0x00, sizeof(STDay));
}


unsigned int ScpIF_TID::get_S_TID(void)
{
	unsigned int i=0;
	FILE *fp;
	
	if(!(fp = fopen(PATH_NAME_SCPIFS_TID, "r+b"))) 
	{
		cout << "ERROR : READ : " << PATH_NAME_SCPIFS_TID << endl;
		return 0;
	}else
	{
		fread(TDay, 10, 1, fp);
		fread((void*)&i, 4, 1, fp);
		fclose(fp);
	}
	
	return i;
}



unsigned int ScpIF_TID::get_R_TID(void)
{
	unsigned int i=0;
	FILE *fp;
	
	if(!(fp = fopen(PATH_NAME_SCPIFR_TID, "r+b"))) 
	{
		cout << "ERROR : READ : " << PATH_NAME_SCPIFR_TID << endl;
		return 0;
	}else
	{
		fread(STDay, 10, 1, fp);
		fread((void*)&i, 4, 1, fp);
		fclose(fp);
	}
	
	return i;
}


void ScpIF_TID::set_S_TID(unsigned int stid, unsigned int idate)
{
	FILE *fp;
	
	
	if(!(fp = fopen(PATH_NAME_SCPIFS_TID, "w+b"))) 
	{
		cout << "ERROR : WRITE : " << PATH_NAME_SCPIFS_TID << endl;
		return;
	}else
	{
		sprintf(TDay, "%8ld", idate);
		
		fwrite(TDay, 1, 10, fp);
		fwrite((void*)&stid, 1, 4, fp);
		fclose(fp);
	}
	
}


void ScpIF_TID::set_R_TID(unsigned int rtid, unsigned int idate)
{
	FILE *fp;
	
	if(!(fp = fopen(PATH_NAME_SCPIFR_TID, "w+b"))) 
	{
		cout << "ERROR : WRITE : " << PATH_NAME_SCPIFR_TID << endl;
		return;
	}else
	{
		sprintf(STDay, "%8ld", idate);
		fwrite(STDay, 1, 10, fp);
		fwrite((void*)&rtid, 1, 4, fp);
		fclose(fp);
	}
}

///////////////////////////////////////////////////////////
// /SVC#/DATA/CDS_TID.cfg
///////////////////////////////////////////////////////////
class	CdsIF_TID
{
	private:
		
		char 	PATH_NAME_SCPIFR_TID[80];	//	=   "/SVC#/DATA/SCPIF_TID.cfg";
		char 	PATH_NAME_SCPIFS_TID[80];	//	=   "/SVC#/DATA/SCPIF_TID.cfg";
		
		char	TDay[10];			// date
 		char	STDay[10];			// date
		
	public:
		CdsIF_TID(int serviceid);
		~CdsIF_TID(){;}

		unsigned int	get_S_TID(void);	  
		unsigned int	get_R_TID(void);
		
		unsigned int	get_S_Day(void)
		{
			get_S_TID();
			return atol(TDay);
		}	  
		unsigned int	get_R_Day(void)
		{
			get_R_TID();
			return atol(STDay);
		}
		
		void			set_S_TID(unsigned int stid, unsigned int idate);
		void			set_R_TID(unsigned int rtid, unsigned int idate);
		
		void			init_TID(void)
		{
			set_S_TID(0,0);	
			set_R_TID(0,0);	
		}
		void			display_TID(void)
		{
			cout << "Schannel Transaction-ID[" << TDay << "] : " << get_S_TID() << endl;
			cout << "Rchannel Transaction-ID[" << STDay << "] : " << get_R_TID() << endl;
		}
};


CdsIF_TID::CdsIF_TID(int serviceid)
{
	
	sprintf(PATH_NAME_SCPIFS_TID, "/SVC%d/DATA/CDS_TID.cfg", serviceid);
	sprintf(PATH_NAME_SCPIFR_TID, "/SVC%d/DATA/CDS_TID.cfg", serviceid);
	memset(TDay, 0x00, sizeof(TDay));
	memset(STDay, 0x00, sizeof(STDay));
}


unsigned int CdsIF_TID::get_S_TID(void)
{
	unsigned int i=0;
	FILE *fp;
	
	if(!(fp = fopen(PATH_NAME_SCPIFS_TID, "r+b"))) 
	{
		cout << "ERROR : READ : " << PATH_NAME_SCPIFS_TID << endl;
		return 0;
	}else
	{
		fread(TDay, 10, 1, fp);
		fread((void*)&i, 4, 1, fp);
		fclose(fp);
	}
	
	return i;
}



unsigned int CdsIF_TID::get_R_TID(void)
{
	unsigned int i=0;
	FILE *fp;
	
	if(!(fp = fopen(PATH_NAME_SCPIFR_TID, "r+b"))) 
	{
		cout << "ERROR : READ : " << PATH_NAME_SCPIFR_TID << endl;
		return 0;
	}else
	{
		fread(STDay, 10, 1, fp);
		fread((void*)&i, 4, 1, fp);
		fclose(fp);
	}
	
	fclose(fp);
	return i;
}


void CdsIF_TID::set_S_TID(unsigned int stid, unsigned int idate)
{
	FILE *fp;
	
	
	if(!(fp = fopen(PATH_NAME_SCPIFS_TID, "w+b"))) 
	{
		cout << "ERROR : WRITE : " << PATH_NAME_SCPIFS_TID << endl;
		return;
	}else
	{
		sprintf(TDay, "%8ld", idate);
		
		fwrite(TDay, 1, 10, fp);
		fwrite((void*)&stid, 1, 4, fp);
		fclose(fp);
	}
	
}


void CdsIF_TID::set_R_TID(unsigned int rtid, unsigned int idate)
{
	FILE *fp;
	
	if(!(fp = fopen(PATH_NAME_SCPIFR_TID, "w+b"))) 
	{
		cout << "ERROR : WRITE : " << PATH_NAME_SCPIFR_TID << endl;
		return;
	}else
	{
		sprintf(STDay, "%8ld", idate);
		fwrite(STDay, 1, 10, fp);
		fwrite((void*)&rtid, 1, 4, fp);
		fclose(fp);
	}
}

/*
///////////////////////////////////////////////////////////
// /SVC#/DATA/SCPIF[R/S]_TID.cfg
///////////////////////////////////////////////////////////
class	ScpIF_TID
{
	private:
		
		char 	PATH_NAME_SCPIFR_TID[80];	//	=   "/SVC#/DATA/SCPIF_TID.cfg";
		char 	PATH_NAME_SCPIFS_TID[80];	//	=   "/SVC#/DATA/SCPIF_TID.cfg";
		
	public:
		ScpIF_TID(int serviceid);
		~ScpIF_TID(){;}

		unsigned int	get_S_TID(void);	  
		unsigned int	get_R_TID(void);
		
		void			set_S_TID(unsigned int stid);
		void			set_R_TID(unsigned int rtid);
		
		void			init_TID(void)
		{
			set_S_TID(0);	
			set_R_TID(0);	
		}
		void			display_TID(void)
		{
			cout << "Schannel Transaction-ID : " << get_S_TID() << endl;
			cout << "Schannel Transaction-ID : " << get_R_TID() << endl;
		}
};

ScpIF_TID::ScpIF_TID(int serviceid)
{
	
	sprintf(PATH_NAME_SCPIFS_TID, "/SVC%d/DATA/SCPIFS_TID.cfg", serviceid);
	sprintf(PATH_NAME_SCPIFR_TID, "/SVC%d/DATA/SCPIFR_TID.cfg", serviceid);
}


unsigned int ScpIF_TID::get_S_TID(void)
{
	unsigned int i=0;
	FILE *fp;
	
	if(!(fp = fopen(PATH_NAME_SCPIFS_TID, "r+b"))) 
	{
		cout << "ERROR : READ : " << PATH_NAME_SCPIFS_TID << endl;
		return 0;
	}else
	{
		fread((void*)&i, 4, 1, fp);
		fclose(fp);
	}
	
	return i;
}

unsigned int ScpIF_TID::get_R_TID(void)
{
	unsigned int i=0;
	FILE *fp;
	
	if(!(fp = fopen(PATH_NAME_SCPIFR_TID, "r+b"))) 
	{
		cout << "ERROR : READ : " << PATH_NAME_SCPIFR_TID << endl;
		return 0;
	}else
	{
		fread((void*)&i, 4, 1, fp);
		fclose(fp);
	}
	
	return i;
}

void ScpIF_TID::set_S_TID(unsigned int stid)
{
	FILE *fp;
	
	if(!(fp = fopen(PATH_NAME_SCPIFS_TID, "w+b"))) 
	{
		cout << "ERROR : WRITE : " << PATH_NAME_SCPIFS_TID << endl;
		return;
	}else
	{
		fwrite((void*)&stid, 1, 4, fp);
		fclose(fp);
	}
	
}


void ScpIF_TID::set_R_TID(unsigned int rtid)
{
	FILE *fp;
	
	if(!(fp = fopen(PATH_NAME_SCPIFR_TID, "w+b"))) 
	{
		cout << "ERROR : WRITE : " << PATH_NAME_SCPIFR_TID << endl;
		return;
	}else
	{
		fwrite((void*)&rtid, 1, 4, fp);
		fclose(fp);
	}
}

*/










///////////////////////////////////////////////////////////
// /SVC#/DATA/AGG_AccountCnt.cfg
///////////////////////////////////////////////////////////
class	AccountCnt
{
	private:
		unsigned int	unAccountCnt;	//in
//		char 			strCfgFileName[128];

	public:
		AccountCnt()	{}
		AccountCnt(const char* strFileName);
		~AccountCnt();
		
		unsigned int	getAccountCnt()		{	return unAccountCnt;	}
		
		void			setAccountCnt(unsigned int unValue){	unAccountCnt	= unValue;}
				
		void			read_AccountCnt();
		void			write_AccountCnt();
		
		void			initial_AccountCnt();
		void			display_AccountCnt();
};

AccountCnt::AccountCnt(const char* strFileName)
{
	strcpy(strCfgFileName, strFileName);
	setAccountCnt(10);
}

AccountCnt::~AccountCnt()
{
//	write_AccountCnt();
}

void AccountCnt::read_AccountCnt()
{
	ifstream	fin(strCfgFileName, ios::binary);
	
	if (fin)
	{
		//cout << "read_AccountCnt() file exist" << endl;
		fin.read((char*)this, sizeof(AccountCnt));
		fin.close();
	}
	else
	{
		//cout << "[WARNING] read_AccountCnt() file not exist" << endl;
		setAccountCnt(10);
		write_AccountCnt();
	}
}

void AccountCnt::write_AccountCnt()
{
	ofstream	fout(strCfgFileName, ios::binary);
	
	fout.write(reinterpret_cast<const char *>(this), sizeof(AccountCnt));
	fout.close();
}

void AccountCnt::initial_AccountCnt()
{
	setAccountCnt(10);
	write_AccountCnt();
}

void AccountCnt::display_AccountCnt()
{
	read_AccountCnt();
	cout << endl;
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	cout << "strCfgFileName : " << strCfgFileName << endl;
	cout << "unAccountCnt : " << unAccountCnt << endl;
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
}

















///////////////////////////////////////////////////////////
// /SVC#/DATA/OBS_FlagDate.cfg
///////////////////////////////////////////////////////////
class	ObsFlagDate
{
	private:
		unsigned int	unFlag;			//in 0:MDB에서 삭제 1:END생성 2:All
		unsigned int	unInterval;		//in
		unsigned int	unBeforeTime;	//in
	public:
//		char 			strCfgFileName[128];
			
		ObsFlagDate()	{}
		ObsFlagDate(const char* strFileName);
		~ObsFlagDate();

		unsigned int	getFlag()					{	return unFlag;	}		
		unsigned int	getInterval()				{	return unInterval;		}
		unsigned int	getBeforeTime()				{	return unBeforeTime;	}

		void			setFlag  (unsigned int unValue) 			{	unFlag	= unValue;}		
		void			setInterval      (unsigned int unValue) 	{	unInterval		= unValue;}
		void			setBeforeTime(unsigned int unValue) 		{	unBeforeTime	= unValue;}
		
		void			read_ObsFlagDate();
		void			read_ObsFlagDate(const char* strFileName);
		void			write_ObsFlagDate();
		
		void			initial_ObsFlagDate();
		void			display_ObsFlagDate();
};


ObsFlagDate::ObsFlagDate(const char* strFileName)
{
	memset(strCfgFileName, 0x00, sizeof(strCfgFileName));
	strcpy(strCfgFileName, strFileName);

	setFlag(2);//0:MDB에서 삭제 1:END생성 2:All
	setInterval(60);
	setBeforeTime(20);
}

ObsFlagDate::~ObsFlagDate()
{
}

void ObsFlagDate::read_ObsFlagDate()
{
	ifstream	fin(strCfgFileName, ios::binary);
	
	if (fin)
	{
		fin.read((char*)this, sizeof(ObsFlagDate));
	}
	else	// 파일 오픈 실패시 파일을 초기화 함.
	{
		//cout << "파일 초기화를 합니당~~" << endl;
		setFlag(2);//0:MDB에서 삭제 1:END생성 2:All
		setInterval(60);
		setBeforeTime(20);
		write_ObsFlagDate();
	}
	fin.close();	
}


void ObsFlagDate::read_ObsFlagDate(const char* strFileName)
{
	cout << "strFileName =>" << strFileName << endl;
	ifstream	fin(strFileName, ios::binary);
	
	if (fin)
	{
		cout << "read_ObsFlagDate(const char* strFileName) file exist" << endl;
		fin.read((char*)this, sizeof(ObsFlagDate));
	}
	else
	{
		cout << "[WARNING] read_ObsFlagDate(const char* strFileName) file not exist" << endl;
		setFlag(2);//0:MDB에서 삭제 1:END생성 2:All
		setInterval(60);
		setBeforeTime(20);
		write_ObsFlagDate();
	}
	fin.close();	
}

void ObsFlagDate::write_ObsFlagDate()
{
	ofstream	fout(strCfgFileName, ios::binary);

	//cout << "**********************" << endl;
	//cout << "strCfgFileName = " << strCfgFileName << endl;
	//cout << "**********************" << endl;

	fout.write(reinterpret_cast<const char *>(this), sizeof(ObsFlagDate));
	fout.close();

//	if (fout)
//	{
//		//cout << "write_ObsFlagDate() OK" << endl;
//		//display_ObsFlagDate();
//	}
//	else
//	{
//		//cout << "	write_ObsFlagDate() FAIL FAIL" << endl;
//	}
}

void ObsFlagDate::initial_ObsFlagDate()
{
	setFlag(2);//0:MDB에서 삭제 1:END생성 2:All
	setInterval(60);
	setBeforeTime(20);
	write_ObsFlagDate();
}

void ObsFlagDate::display_ObsFlagDate()
{
	read_ObsFlagDate();
	cout << endl;
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	cout << "strCfgFileName : " << strCfgFileName << endl;
	cout << "unFlag         : " << unFlag << endl;
	cout << "unInterval       : " << unInterval << endl;
	cout << "unBeforeTime       : " << unBeforeTime << endl;
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
}


/*
///////////////////////////////////////////////////////////
// /SVC#/DATA/OBS_FlagDate.cfg
///////////////////////////////////////////////////////////
class	ObsFlagDate
{
	private:
		unsigned int	unFlag;			//in 0:MDB에서 삭제 1:END생성
		unsigned int	unYYMMDD;		//in
		unsigned int	unHHMMSS;		//in
	public:
//		char 			strCfgFileName[128];
			
		ObsFlagDate()	{}
		ObsFlagDate(const char* strFileName);
		~ObsFlagDate();

		unsigned int	getFlag()				{	return unFlag;	}		
		unsigned int	getYYMMDD()				{	return unYYMMDD;		}
		unsigned int	getHHMMSS()				{	return unHHMMSS;	}

		void			setFlag  (unsigned int unValue) 		{	unFlag	= unValue;}		
		void			setYYMMDD      (unsigned int unValue) 	{	unYYMMDD		= unValue;}
		void			setHHMMSS(unsigned int unValue) 		{	unHHMMSS	= unValue;}
		
		void			read_ObsFlagDate();
		void			read_ObsFlagDate(const char* strFileName);
		void			write_ObsFlagDate();
		
		void			initial_ObsFlagDate();
		void			display_ObsFlagDate();
};


ObsFlagDate::ObsFlagDate(const char* strFileName)
{
	memset(strCfgFileName, 0x00, sizeof(strCfgFileName));
	strcpy(strCfgFileName, strFileName);

	setFlag(1);//0:MDB에서 삭제 1:END생성
	setYYMMDD(20001231);
	setHHMMSS(100000);
}

ObsFlagDate::~ObsFlagDate()
{
}

void ObsFlagDate::read_ObsFlagDate()
{
	ifstream	fin(strCfgFileName, ios::binary);
	
	if (fin)
	{
		fin.read((char*)this, sizeof(ObsFlagDate));
	}
	else	// 파일 오픈 실패시 파일을 초기화 함.
	{
		//cout << "파일 초기화를 합니당~~" << endl;
		setFlag(1);//0:MDB에서 삭제 1:END생성
		setYYMMDD(20001231);
		setHHMMSS(100000);
		write_ObsFlagDate();
	}
	fin.close();	
}


void ObsFlagDate::read_ObsFlagDate(const char* strFileName)
{
	cout << "strFileName =>" << strFileName << endl;
	ifstream	fin(strFileName, ios::binary);
	
	if (fin)
	{
		cout << "read_ObsFlagDate(const char* strFileName) file exist" << endl;
		fin.read((char*)this, sizeof(ObsFlagDate));
	}
	else
	{
		cout << "[WARNING] read_ObsFlagDate(const char* strFileName) file not exist" << endl;
		setFlag(1);//0:MDB에서 삭제 1:END생성
		setYYMMDD(20001231);
		setHHMMSS(100000);
		write_ObsFlagDate();
	}
	fin.close();	
}

void ObsFlagDate::write_ObsFlagDate()
{
	ofstream	fout(strCfgFileName, ios::binary);

	//cout << "**********************" << endl;
	//cout << "strCfgFileName = " << strCfgFileName << endl;
	//cout << "**********************" << endl;

	fout.write(reinterpret_cast<const char *>(this), sizeof(ObsFlagDate));
	fout.close();

//	if (fout)
//	{
//		//cout << "write_ObsFlagDate() OK" << endl;
//		//display_ObsFlagDate();
//	}
//	else
//	{
//		//cout << "	write_ObsFlagDate() FAIL FAIL" << endl;
//	}
}

void ObsFlagDate::initial_ObsFlagDate()
{
	setFlag(1);//0:MDB에서 삭제 1:END생성
	setYYMMDD(20001231);
	setHHMMSS(100000);
	write_ObsFlagDate();
}

void ObsFlagDate::display_ObsFlagDate()
{
	read_ObsFlagDate();
	cout << endl;
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	cout << "strCfgFileName : " << strCfgFileName << endl;
	cout << "unFlag         : " << unFlag << endl;
	cout << "unYYMMDD       : " << unYYMMDD << endl;
	cout << "unHHMMSS       : " << unHHMMSS << endl;
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
}
*/


































