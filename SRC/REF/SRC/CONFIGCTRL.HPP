#ifndef _CONFIG_CTRL_HPP
#define _CONFIG_CTRL_HPP

#include <stdio.h>

#include <md5.hpp>
#include <reformat.hpp>


#define ORIGIN_UDR_SIZE		500			// Udr format���� ��ȯ���� ���� ���� Udr		
#define	UDR_MAX_COUNT		1000		// �� ���ϳ� �ִ� ���� Udr ���� 
#define UDR_MAX_SEQ			999999999	// ���� Udr�� ����Ǿ� �ִ� �ִ� ȭ�� ��ȣ 


//////////////////////////
//	ConfigCTRL Class	//
//////////////////////////

//-----------------------------------------------------------------------------------
// Reformat�� Pdsn Config ����
class ConfigCTRL {
	private:
		uint	iSeq;		// ���� UDR ȭ�Ϲ�ȣ	(1 - 999,999,999)
		uint	iNum;		// ȭ�ϳ� ���� UDR ����	(1 - 1000)
		uchar	ucId;		// Idendtifier
		int 	Reformat;	// Reformat�� Pdsn�� �� Ŭ������ �������� ����ϹǷ� �����ϱ� �����̴�.	
		char	strMdKey[16];	
		
		FILE	*fp;		// File Pointer
		MD5 	*conMd5;
		
		char	PATH_NAME_PDSNIF_UDR[100]; 		// ���� Udr ������ �����Ǿ� �ִ� ��θ�	
		char	FILE_NAME_PDSNIF_CFG[100];		// Pdsn Config File name
		char	FILE_NAME_REFORMAT_CFG[100];	// Reformat Config File name
				

	public:
		// sn : Service Id, pn : Process Serial Number, Reformat : Reformat����	
		ConfigCTRL(int sn, int pn, int reformat=1, char* strMD5Key="testtesttesttest");
		~ConfigCTRL();		
		
		uint	get_iSeq()	{return iSeq;}
		uint	get_iNum()	{return iNum;}
		uchar	get_ucId()	{return ucId;}
		
		int 	read_Udr(CPdsnMsg* stPdsnMsg);	// ERROR : 0
		int 	read_Udr_COMMIT();				// UDR�� �о ���������� ó����
		int 	write_Udr(CPdsnMsg* stPdsnMsg);	// ���� Udr�� ���Ͽ� Write�� ��  Config File�� ���������� Write�Ѵ�.
		
		int 	write_Cfg(void);				// cfgȭ�Ͽ� ����Ѵ�.
		int 	read_Cfg(void);					// cfgȭ���� �о���δ�.			

		int 	set_iNum(void);					// iSeq�ش� ȭ���� �о� UDR ������ �����Ѵ�.( ���α׷�ó�������)
		void 	set_UdrSeq(int iseq, int inum)	// ����Udrȭ�Ϲ�ȣ, ȭ�ϳ� ����Udr ���� Setting
		{
			iSeq=iseq;
			iNum=inum;	
			
			if(!iSeq) iSeq=1;
		}
		
		void 	get_UdrSeq(int *iseq, int *inum)// iseq : ����UDRȭ�Ϲ�ȣ inum : ȭ�ϳ� ����Udr ���� ���	
		{
			*iseq = iSeq;
			*inum = iNum;

		}
};




//////////////////////////////////
//	Class ConfigCTRL's Method 	//
//////////////////////////////////

//-----------------------------------------------------------------------------------
// ������
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
// �Ҹ���
// Config File�� Write�� �� ��ü �Ҹ�.
ConfigCTRL::~ConfigCTRL()
{
	write_Cfg();
}


//-----------------------------------------------------------------------------------
// ���������� ���Ͽ��� �о���δ�.
int ConfigCTRL::read_Cfg(void)
{
	if(Reformat)	// Reformat
	{
		if(!(fp = fopen(FILE_NAME_REFORMAT_CFG, "r+b")))
		{	// Update���� ���� Open
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
// ���������� ���Ͽ� ����Ѵ�.
int ConfigCTRL::write_Cfg(void)
{
	
	if(Reformat)
	{
		if((fp = fopen(FILE_NAME_REFORMAT_CFG, "w+b")))
		{	
			// Update���� ���� Open
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
// iSeq�ش� ȭ���� �о� UDR ������ �����Ѵ�.
// ���α׷� ó�� �����Ҷ� �ѹ��� �Ѵ�.->�� ���Ͽ��� ����Udr�� � ��ϵƴ��� �˻�
int ConfigCTRL::set_iNum(void)
{
	int	 flen;
	char fname[150];
	
	sprintf(fname, "%s/pdsn_udr.%d",  PATH_NAME_PDSNIF_UDR, iSeq);
	if(!(fp = fopen(fname, "r+b")))	return;// Open file for Update
	
	if(fseek(fp, 0L, SEEK_END) < 0)	// File�� ������ ���� ���� File Pointer�� ��ġ�� �����Ѵ�.
	{
		fclose(fp);
		return -1;
	}	
		
	flen = ftell(fp);				// File�� ó������ ���� ������ ��ġ�� Byte���� Return�Ѵ�.
	
	iNum = flen / ORIGIN_UDR_SIZE;	// ����Udr Count�� ��´�.
	fclose(fp);
	
	return 1;
}


//-----------------------------------------------------------------------------------
// ���� Udr�� ���Ͽ� Write�� ��  Config File�� ���������� Write�Ѵ�.
int  ConfigCTRL::write_Udr(CPdsnMsg* stPdsnMsg)
{
	char fname[150];		// ���� Udr ���ϸ�
	char strMdBuf[16];
	memset(strMdBuf, NULL, sizeof(strMdBuf));
	
	iNum++;					// ���ϳ� ���� UDR ���� 1����
	ucId++;					// Identifier 1����
	if(iNum>UDR_MAX_COUNT) 	// ���ϳ� Udr������ 1000�� �̻��̸� Sequence����, Udr ������ 1�� Setting
	{
		iSeq++;
		if(iSeq > UDR_MAX_SEQ) iSeq=1;
		iNum=1;
	}
	stPdsnMsg->set_ucIdentifier(ucId);	
//	stPdsnMsg->set_ucAuthenticator((uchar *)strMdKey);
	stPdsnMsg->set_ucAuthenticator((uchar *)strMdBuf);
	memcpy((char*)&stPdsnMsg->strAttribute[stPdsnMsg->get_usLength()-SIZE_HEADER], strMdKey, 16);

	//---------MD5 hash ����--------------
	conMd5 = new MD5;
	conMd5->update((unsigned char *)stPdsnMsg, stPdsnMsg->get_usLength()+16);
	conMd5->finalize();
	memcpy(strMdBuf, conMd5->raw_digest(), 16);
	delete conMd5;
	//---------MD5 hash ����--------------
	stPdsnMsg->set_ucAuthenticator((uchar *)strMdBuf);

	sprintf(fname, "%s/pdsn_udr.%d",  PATH_NAME_PDSNIF_UDR, iSeq);
	
	if(!(fp = fopen(fname, "a+b")))	return -1;	// ����Udr�� ���Ͽ� Write�ϱ� ���� Open
	if(fwrite((char*)stPdsnMsg, 1, ORIGIN_UDR_SIZE, fp) < 1)
	{
		cout <<  "Fail write PDSN MSG" << endl;
		fclose(fp);
		return -1;
	}	 
						
	fclose(fp);
	
	if(write_Cfg() < 0)		// ����Udr�� ���Ͽ� ���� Write�� Config File�� ���������� Write�Ѵ�.
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