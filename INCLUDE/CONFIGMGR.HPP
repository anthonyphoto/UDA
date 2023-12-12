#include <stdio.h>
#include <UDR.hpp>

#define UDR_SIZE		372
#define	UDR_MAX_COUNT	1000
#define UDR_MAX_SEQ		999999999

//-----------------------------------------------------------------------------------
class ConfigMGR {

	private:
		unsigned int	iSeq;		// UDR 화일번호 (1 - 999,999,999)
		unsigned int	iNum;		// 화일내 UDR갯수 (1-1000)
		int 			Filter;
		
		FILE			*fp;
		
		char	PATH_NAME_PDSNIF_UDR[100]; //, "/SVC%d/DATA/RADIUS%d_FileBlock%d.cfg", SystemId, ProSerialNo, ProReMark);
		char	PATH_NAME_ERROR_UDR[100];
		char	FILE_NAME_PDSNIF_CFG[100];
		char	FILE_NAME_FILTER_CFG[100];
				
	public:
		// sn : serviceid  /pn : processsn  /filter : 필터여부
		ConfigMGR(int sn, int pn, int filter=1);	
		~ConfigMGR(){write_Cfg();}
		
		void set_UdrSeq(int iseq, int inum)
		{
			iSeq=iseq;
			iNum=inum;	
			
			if(!iSeq) iSeq=1;
		}
		
		// iseq : UDR화일번호 inum : 화일내 번호얻기
		void get_UdrSeq(int *iseq, int *inum)	
		{
			*iseq = iSeq;
			*inum = iNum;

		}
	
		
		unsigned int get_iSeq()	{return iSeq;}
		unsigned int get_iNum()	{return iNum;}
	
		
		// iSeq해당 화일을 읽어 UDR 갯수를 설정한다. ( PDSNIF 처음실행시)
		void set_iNum(void);
	
		int read_Udr(UdrFormat *fmUDR);		// ERROR : 0
		int write_Udr(UdrFormat *fmUDR); 	// ERROR : 0
        int readUdrOfAcct(UdrFormat *fmUDR);  // dongcheol 05/25/2001
		
		// cfg화일에 기록한다.
		void write_Cfg(void);	
		// cfg화일을 읽어들인다.
		void read_Cfg(void);			
		
		//--------------------------- 
		// ERROR CDR Write
		void write_Error(char *sBuf);
};


//-----------------------------------------------------------------------------------
ConfigMGR::ConfigMGR(int sn, int pn, int filter)
{
	
	iSeq=1;
	iNum=0;
	Filter=filter;
	sprintf(PATH_NAME_PDSNIF_UDR, "/SVC%d/RADIUS/PDSN%d", sn, pn);
	sprintf(PATH_NAME_ERROR_UDR, "/SVC%d/RADIUS/ERROR%d", sn, pn);
	sprintf(FILE_NAME_PDSNIF_CFG, "/SVC%d/DATA/pdsn%d_if.cfg", sn, pn);
	sprintf(FILE_NAME_FILTER_CFG, "/SVC%d/DATA/pdsn%d_filter.cfg", sn, pn);
	
	read_Cfg();
	/*
	if(filter)
	{
		if(!(fp = fopen(FILE_NAME_FILTER_CFG, "r+b"))) 
		{
			iSeq=1;
			iNum=0;
		}else
		{
			fread((void*)&iSeq, 4, 1, fp);
			fread((void*)&iNum, 4, 1, fp);
			fclose(fp);
			
			if(!iSeq) iSeq=1;
			cout << FILE_NAME_FILTER_CFG << "  " << iSeq << " : " <<  iNum << " : " << endl;	
		}
	}else
	{
		if(!(fp = fopen(FILE_NAME_PDSNIF_CFG, "r+b"))) 
		{
			iSeq=1;
			iNum=0;
		}else
		{
			fread((void*)&iSeq, 4, 1, fp);
			fread((void*)&iNum, 4, 1, fp);
			fclose(fp);
			
			if(!iSeq) iSeq=1;
			cout << FILE_NAME_PDSNIF_CFG << "  " << iSeq << " : " <<  iNum << " : " << endl;	
		}
	}
	
	fclose(fp);
	*/
}



//-----------------------------------------------------------------------------------
void ConfigMGR::read_Cfg(void)
{
	if(Filter)
	{
		if(!(fp = fopen(FILE_NAME_FILTER_CFG, "r+b"))) 
		{
			iSeq=1;
			iNum=0;
		}else
		{
			fread((void*)&iSeq, 4, 1, fp);
			fread((void*)&iNum, 4, 1, fp);
			fclose(fp);
			
			if(!iSeq) iSeq=1;
			cout << FILE_NAME_FILTER_CFG << "  " << iSeq << " : " <<  iNum << " : " << endl;	
		}
	}else
	{
		if(!(fp = fopen(FILE_NAME_PDSNIF_CFG, "r+b"))) 
		{
			iSeq=1;
			iNum=0;
		}else
		{
			fread((void*)&iSeq, 4, 1, fp);
			fread((void*)&iNum, 4, 1, fp);
			fclose(fp);
			
			if(!iSeq) iSeq=1;
			cout << FILE_NAME_PDSNIF_CFG << "  " << iSeq << " : " <<  iNum << " : " << endl;	
		}
	}
	fclose(fp);
}


//-----------------------------------------------------------------------------------
void ConfigMGR::write_Cfg(void)
{
	
	if(Filter)
	{
		if((fp = fopen(FILE_NAME_FILTER_CFG, "w+b"))) 
		{
			fwrite((void*)&iSeq, 1, 4, fp);
			fwrite((void*)&iNum, 1, 4, fp);
			fclose(fp);
			
			cout << FILE_NAME_FILTER_CFG << "  " << iSeq << " : " <<  iNum << " : " << endl;	
		}
	}else
	{
		if((fp = fopen(FILE_NAME_PDSNIF_CFG, "w+b"))) 
		{
			fwrite((void*)&iSeq, 1, 4, fp);
			fwrite((void*)&iNum, 1, 4, fp);
			fclose(fp);
			
			cout << FILE_NAME_PDSNIF_CFG << "  " << iSeq << " : " <<  iNum << " : " << endl;	
		}
	}
	
	fclose(fp);
}


//-----------------------------------------------------------------------------------
void ConfigMGR::set_iNum(void)
{
	int	 flen;
	char fname[150];
	
	sprintf(fname, "%s/pdsn_udr.%d",  PATH_NAME_PDSNIF_UDR, iSeq);
	if(!(fp = fopen(fname, "r+b"))) 
	{
		iNum=0;
		return;
	}
	
	fseek(fp, 0L, SEEK_END);
	
	flen = ftell(fp);
	
	iNum = flen / UDR_SIZE;
	fclose(fp);
}




//-----------------------------------------------------------------------------------
int  ConfigMGR::write_Udr(UdrFormat *fmUDR)
{
	char fname[150];
	
	read_Cfg();
	
	iNum++;
	if(iNum>UDR_MAX_COUNT) 
	{
		iSeq++;
		if(iSeq > UDR_MAX_SEQ) iSeq=1;
		iNum=1;
	}
	
	fmUDR->get_ItemHead()->put_nUdrFileSeq(iSeq);
	fmUDR->get_ItemHead()->put_nUdrSeqNo(iNum);
	
	sprintf(fname, "%s/pdsn_udr.%d",  PATH_NAME_PDSNIF_UDR, iSeq);
	
	if(!(fp = fopen(fname, "a+b")))
	{
		return 0;
	}
	fwrite((void*)fmUDR, 1, UDR_SIZE, fp);
	
	fclose(fp);
	
	write_Cfg();
	return 1;
}



//-----------------------------------------------------------------------------------
int  ConfigMGR::read_Udr(UdrFormat *fmUDR)
{
	int		rlen;
	long	fpPosi;
	char 	fname[150];

	
	read_Cfg();
	
	sprintf(fname, "%s/pdsn_udr.%d",  PATH_NAME_PDSNIF_UDR, iSeq);
	
	if(!(fp = fopen(fname, "r+b")))
	{
		cout << "[CONMGR] File Not Found : " << fname << endl;
		return 0;
	}
	
	fpPosi = UDR_SIZE*iNum;
	//cout << "[CONMGR] fpPosi : " << fpPosi << endl;
	
	if(fseek(fp, fpPosi, SEEK_SET)!=0) { 
		cout << "[CONMGR] fseek ERROR" << endl;
		fclose(fp); 
		return 0; 
	}
	
	rlen = fread((void*)fmUDR, 1, UDR_SIZE, fp);
	if(rlen < UDR_SIZE) 
	{ 
		cout << "[CONMGR] fread ERROR : " << rlen << endl;
		fclose(fp); 
		return 0; 
	}
	fclose(fp);
	
	//cout << "[CONMGR] Read Position : " << iNum << endl;
	
	iNum++;
	if(iNum>UDR_MAX_COUNT-1) 
	{
		iSeq++;
		if(iSeq > UDR_MAX_SEQ) iSeq=1;
		iNum=0;
	}
	
	write_Cfg();
	return 1;
}

int  ConfigMGR::readUdrOfAcct(UdrFormat *fmUDR)
{
	int		rlen;
	long	fpPosi;
	char 	fname[150];
	
	sprintf(fname, "%s/pdsn_udr.%d",  PATH_NAME_PDSNIF_UDR, iSeq);
	
	if(!(fp = fopen(fname, "r+b")))
	{
		cout << "[CONMGR] File Not Found : " << fname << endl;
		return 0;
	}
	
	fpPosi = UDR_SIZE*iNum;
	
	if(fseek(fp, fpPosi, SEEK_SET)!=0) { 
		cout << "[CONMGR] fseek ERROR" << endl;
		fclose(fp); 
		return 0; 
	}
	
	rlen = fread((void*)fmUDR, 1, UDR_SIZE, fp);
	if(rlen < UDR_SIZE) 
	{ 
		// cout << "[CONMGR] fread ERROR : " << rlen << endl;
		fclose(fp); 
		return 0; 
	}
	fclose(fp);
	
	iNum++;
	if(iNum>UDR_MAX_COUNT-1) 
	{
		iSeq++;
		if(iSeq > UDR_MAX_SEQ) iSeq=1;
		iNum=0;
	}
	
	return 1;
}


//-----------------------------------------------------------------------------------
void ConfigMGR::write_Error(char *sBuf)
{
	char 		fname[150];
	time_t		t;
	struct tm 	*s;
	
	time(&t);
	s = (struct tm *) localtime( &t );

	sprintf(fname, "%s/err_udr.%04d%02d%02d",  PATH_NAME_ERROR_UDR, s->tm_year+1900, s->tm_mon+1, s->tm_mday);

	
	if(!(fp = fopen(fname, "a+b"))) return;
	
	fwrite(sBuf, 1, 500, fp);
	
	fclose(fp);	
}

