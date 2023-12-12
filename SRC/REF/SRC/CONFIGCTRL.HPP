#ifndef _CONFIG_CTRL_HPP
#define _CONFIG_CTRL_HPP

#include <stdio.h>

#include <md5.hpp>
#include <reformat.hpp>


#define ORIGIN_UDR_SIZE		500			// Udr format으로 변환되지 않은 원시 Udr		
#define	UDR_MAX_COUNT		1000		// 한 파일내 최대 원시 Udr 개수 
#define UDR_MAX_SEQ			999999999	// 원시 Udr이 저장되어 있는 최대 화일 번호 


//////////////////////////
//	ConfigCTRL Class	//
//////////////////////////

//-----------------------------------------------------------------------------------
// Reformat과 Pdsn Config 관리
class ConfigCTRL {
	private:
		uint	iSeq;		// 원시 UDR 화일번호	(1 - 999,999,999)
		uint	iNum;		// 화일내 원시 UDR 개수	(1 - 1000)
		uchar	ucId;		// Idendtifier
		int 	Reformat;	// Reformat과 Pdsn이 이 클래스를 공통으로 사용하므로 구분하기 위함이다.	
		char	strMdKey[16];	
		
		FILE	*fp;		// File Pointer
		MD5 	*conMd5;
		
		char	PATH_NAME_PDSNIF_UDR[100]; 		// 원시 Udr 파일이 저정되어 있는 경로명	
		char	FILE_NAME_PDSNIF_CFG[100];		// Pdsn Config File name
		char	FILE_NAME_REFORMAT_CFG[100];	// Reformat Config File name
				

	public:
		// sn : Service Id, pn : Process Serial Number, Reformat : Reformat여부	
		ConfigCTRL(int sn, int pn, int reformat=1, char* strMD5Key="testtesttesttest");
		~ConfigCTRL();		
		
		uint	get_iSeq()	{return iSeq;}
		uint	get_iNum()	{return iNum;}
		uchar	get_ucId()	{return ucId;}
		
		int 	read_Udr(CPdsnMsg* stPdsnMsg);	// ERROR : 0
		int 	read_Udr_COMMIT();				// UDR을 읽어서 정상적으로 처리함
		int 	write_Udr(CPdsnMsg* stPdsnMsg);	// 원시 Udr을 파일에 Write한 후  Config File에 구성정보를 Write한다.
		
		int 	write_Cfg(void);				// cfg화일에 기록한다.
		int 	read_Cfg(void);					// cfg화일을 읽어들인다.			

		int 	set_iNum(void);					// iSeq해당 화일을 읽어 UDR 갯수를 설정한다.( 프로그램처음실행시)
		void 	set_UdrSeq(int iseq, int inum)	// 원시Udr화일번호, 화일내 원시Udr 개수 Setting
		{
			iSeq=iseq;
			iNum=inum;	
			
			if(!iSeq) iSeq=1;
		}
		
		void 	get_UdrSeq(int *iseq, int *inum)// iseq : 원시UDR화일번호 inum : 화일내 원시Udr 개수 얻기	
		{
			*iseq = iSeq;
			*inum = iNum;

		}
};




//////////////////////////////////
//	Class ConfigCTRL's Method 	//
//////////////////////////////////

//-----------------------------------------------------------------------------------
// 생성자
ConfigCTRL::ConfigCTRL(int sn, int pn, int reformat, char* strMD5Key)
{
	
	iSeq=1;
	iNum=0;
	ucId=0;
	Reformat=reformat;

	memset(strMdKey, NULL	  , 16);
	memcpy(strMdKey, strMD5Key, 16);

	sprintf(PATH_NAME_PDSNIF_UDR  , "/SVC%d/RADIUS/017PDSN/017PDSN%d"   , sn, pn);
	sprintf(FILE_NAME_PDSNIF_CFG  , "/SVC%d/DATA/017pdsn%d_if.cfg"      , sn, pn);
	sprintf(FILE_NAME_REFORMAT_CFG, "/SVC%d/DATA/017pdsn%d_reformat.cfg", sn, pn);
	
	if(read_Cfg() < 0)
	{
		cout << "errno [" << errno << "]" << endl;
		exit(-1);
	}	
}


//-----------------------------------------------------------------------------------
// 소멸자
// Config File에 Write한 후 객체 소멸.
ConfigCTRL::~ConfigCTRL()
{
	write_Cfg();
}


//-----------------------------------------------------------------------------------
// 구성정보를 파일에서 읽어들인다.
int ConfigCTRL::read_Cfg(void)
{
	if(Reformat)	// Reformat
	{
		if(!(fp = fopen(FILE_NAME_REFORMAT_CFG, "r+b")))
		{	// Update위해 파일 Open
			iSeq=1;
			iNum=0;
			ucId=0;
		}
		else
		{
			if(fread((void*)&iSeq, 4, 1, fp) < 1)
			{
				fclose(fp);
				return -1;
			}	 
			
			if(fread((void*)&iNum, 4, 1, fp) < 1)
			{
				fclose(fp);
				return -1;
			}	 
							
			if(fread((void*)&ucId, 1, 1, fp) < 1)
			{
				fclose(fp);
				return -1;
			}	 
										
			fclose(fp);
			
			if(!iSeq) iSeq=1;
			cout << FILE_NAME_REFORMAT_CFG << "  " << iSeq << " : " <<  iNum << " : " << (int)ucId << endl;	
		}
	}
	else		// Pdsn
	{
		if(!(fp = fopen(FILE_NAME_PDSNIF_CFG, "r+b")))
		{
			iSeq=1;
			iNum=0;
			ucId=0;
		}
		else{
			if(fread((void*)&iSeq, 4, 1, fp) < 1)
			{
				fclose(fp);
				return -1;
			}	 
						
			if(fread((void*)&iNum, 4, 1, fp) < 1)
			{
				fclose(fp);
				return -1;
			}	 
						
			if(fread((void*)&ucId, 1, 1, fp) < 1)
			{
				fclose(fp);
				return -1;
			}	 
						
			fclose(fp);
			
			if(!iSeq) iSeq=1;
			cout << FILE_NAME_PDSNIF_CFG << "  " << iSeq << " : " <<  iNum << " : " << (int)ucId<< endl;	
		}
	}

	fclose(fp);
	
	return 1;
}


//-----------------------------------------------------------------------------------
// 구성정보를 파일에 기록한다.
int ConfigCTRL::write_Cfg(void)
{
	
	if(Reformat)
	{
		if((fp = fopen(FILE_NAME_REFORMAT_CFG, "w+b")))
		{	
			// Update위해 파일 Open
			if(fwrite((void*)&iSeq, 1, 4, fp) < 1)
			{
				fclose(fp);
				return -1;
			}	 
									
			if(fwrite((void*)&iNum, 1, 4, fp) < 1)
			{
				fclose(fp);
				return -1;
			}	 
						
			if(fwrite((void*)&ucId, 1, 1, fp) < 1)
			{
				fclose(fp);
				return -1;
			}	 
						
			fclose(fp);
			
			cout << FILE_NAME_REFORMAT_CFG << "  " << iSeq << " : " <<  iNum << " : " << (int)ucId << endl;	
		}
	}
	else
	{
		if((fp = fopen(FILE_NAME_PDSNIF_CFG, "w+b")))
		{
			if(fwrite((void*)&iSeq, 1, 4, fp) < 1)
			{
				fclose(fp);
				return -1;
			}	 
			if(fwrite((void*)&iNum, 1, 4, fp) < 1)
			{
				fclose(fp);
				return -1;
			}	 
			if(fwrite((void*)&ucId, 1, 1, fp) < 1)
			{
				fclose(fp);
				return -1;
			}	 
						
			fclose(fp);
			
			cout << FILE_NAME_PDSNIF_CFG << "  " << iSeq << " : " <<  iNum << " : " << (int)ucId << endl;	
		}
	}
	
	fclose(fp);
	
	return 1;
}


//-----------------------------------------------------------------------------------
// iSeq해당 화일을 읽어 UDR 갯수를 설정한다.
// 프로그램 처음 실행할때 한번만 한다.->한 파일에서 원시Udr이 몇개 기록됐는지 검사
int ConfigCTRL::set_iNum(void)
{
	int	 flen;
	char fname[150];
	
	sprintf(fname, "%s/pdsn_udr.%d",  PATH_NAME_PDSNIF_UDR, iSeq);
	if(!(fp = fopen(fname, "r+b")))	return;// Open file for Update
	
	if(fseek(fp, 0L, SEEK_END) < 0)	// File의 끝에서 부터 계산된 File Pointer의 위치를 지정한다.
	{
		fclose(fp);
		return -1;
	}	
		
	flen = ftell(fp);				// File의 처음부터 계산된 현재의 위치를 Byte수로 Return한다.
	
	iNum = flen / ORIGIN_UDR_SIZE;	// 원시Udr Count를 얻는다.
	fclose(fp);
	
	return 1;
}


//-----------------------------------------------------------------------------------
// 원시 Udr을 파일에 Write한 후  Config File에 구성정보를 Write한다.
int  ConfigCTRL::write_Udr(CPdsnMsg* stPdsnMsg)
{
	char fname[150];		// 원시 Udr 파일명
	char strMdBuf[16];
	memset(strMdBuf, NULL, sizeof(strMdBuf));
	
	iNum++;					// 파일내 원시 UDR 개수 1증가
	ucId++;					// Identifier 1증가
	if(iNum>UDR_MAX_COUNT) 	// 파일내 Udr개수가 1000개 이상이면 Sequence증가, Udr 개수는 1로 Setting
	{
		iSeq++;
		if(iSeq > UDR_MAX_SEQ) iSeq=1;
		iNum=1;
	}
	stPdsnMsg->set_ucIdentifier(ucId);	
//	stPdsnMsg->set_ucAuthenticator((uchar *)strMdKey);
	stPdsnMsg->set_ucAuthenticator((uchar *)strMdBuf);
	memcpy((char*)&stPdsnMsg->strAttribute[stPdsnMsg->get_usLength()-SIZE_HEADER], strMdKey, 16);

	//---------MD5 hash 생성--------------
	conMd5 = new MD5;
	conMd5->update((unsigned char *)stPdsnMsg, stPdsnMsg->get_usLength()+16);
	conMd5->finalize();
	memcpy(strMdBuf, conMd5->raw_digest(), 16);
	delete conMd5;
	//---------MD5 hash 생성--------------
	stPdsnMsg->set_ucAuthenticator((uchar *)strMdBuf);

	sprintf(fname, "%s/pdsn_udr.%d",  PATH_NAME_PDSNIF_UDR, iSeq);
	
	if(!(fp = fopen(fname, "a+b")))	return -1;	// 원시Udr을 파일에 Write하기 위해 Open
	if(fwrite((char*)stPdsnMsg, 1, ORIGIN_UDR_SIZE, fp) < 1)
	{
		cout <<  "Fail write PDSN MSG" << endl;
		fclose(fp);
		return -1;
	}	 
						
	fclose(fp);
	
	if(write_Cfg() < 0)		// 원시Udr을 파일에 파일 Write후 Config File에 구성정보를 Write한다.
		return -1;
 	
	return 1;
}



//-----------------------------------------------------------------------------------
int ConfigCTRL::read_Udr_COMMIT()
{
	iNum++;
	if(iNum>UDR_MAX_COUNT-1) 
	{
		iSeq++;
		if(iSeq > UDR_MAX_SEQ) iSeq=1;
		iNum=0;
	}
	
	if(write_Cfg() < 0)
		return -1;
 	
	return 1;
}


//-----------------------------------------------------------------------------------
int  ConfigCTRL::read_Udr(CPdsnMsg* stPdsnMsg)
{
	int		rlen;
	long	fpPosi;
	char 	fname[150];
	
	sprintf(fname, "%s/pdsn_udr.%d",  PATH_NAME_PDSNIF_UDR, iSeq);
	
	if(!(fp = fopen(fname, "r+b")))// Open file for Update
	{
		cout << "[ConfigCTRL] File Not Found : " << fname << endl;
		return -1;
	}
	
	fpPosi = ORIGIN_UDR_SIZE*iNum;
	
	if(fseek(fp, fpPosi, SEEK_SET)!=0)
	{ 
		fclose(fp); 
		return -1; 
	}
	
	rlen = fread((void*)stPdsnMsg, 1, ORIGIN_UDR_SIZE, fp);
	if(rlen < ORIGIN_UDR_SIZE) 
	{ 
		fclose(fp); 
		return -1; 
	}

	fclose(fp);
	
	/*
	iNum++;
	if(iNum>UDR_MAX_COUNT-1) 
	{
		iSeq++;
		if(iSeq > UDR_MAX_SEQ) iSeq=1;
		iNum=0;
	}
	
	write_Cfg();
	*/
	return 1;
}


#endif
