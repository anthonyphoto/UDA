/*
 * $Log: CSnmpAcctTotal.hpp $
 * Revision 0.9.0  2001/05/28 21:48:14  dongcheol
 * renew jjk's CSnmpAcctTotal
 * Revision 0.9.1  ...
 * ...
 */

typedef unsigned int 	uint;
#define		RECORD_SIZE		512

#ifndef	_SNMP_ACCT_TOTAL_HPP_
#define _SNMP_ACCT_TOTAL_HPP_

class CSnmpAcctTotal
{
	private:
		FILE*	fpSnmpTotal;		// 통계를 쓰기위한 파일포인터
		char	strFileName[100];	// 통계파일 이름
		char	strStreamBuf[513];	// 파일의 내용을 한라인 단위로 가져오기 위한 버퍼
		int		gnMyLineNo;			// 파일에 내용을 쓸때 그 라인에 가서 쓴다.
		
		// refer to "SNMP_Agent.doc" $1.4.3.6
		uint	unStart;	// PDSN으로부터 전송된 Start UDR 개수 
		uint	unIterim;	// PDSN으로부터 전송된 Interim UDR 개수 
		uint	unStop;		// PDSN으로부터 전송된 Stop UDR 개수 
		uint	unDrop;		// PDSN으로부터 전송된 Drop UDR 개수 
		uint	unTotal;	// PDSN으로부터 전송된 Total UDR 개수 

	public:
		CSnmpAcctTotal();
		~CSnmpAcctTotal();
		// accessor
		void	Set_Start	(uint i)	{ unStart  	= i; }
		void	Set_Iterim	(uint i)	{ unIterim	= i; }
		void	Set_Stop	(uint i)	{ unStop	= i; }
		void	Set_Drop	(uint i)	{ unDrop	= i; }
		void	Set_Total 	(uint i)	{ unTotal 	= i; }

		uint	Get_Start	()	{ return unStart ; }
		uint	Get_Iterim	()	{ return unIterim; }
		uint	Get_Stop	()	{ return unStop	 ; }
		uint	Get_Drop	()	{ return unDrop	 ; }
		uint	Get_Total 	()	{ return unTotal ; }

		void	FileChkNMake();
		int		FindMyNas();
		void	InitValue();
		void	MakeRecord();
		int		WritePerform();
};

#endif 


CSnmpAcctTotal::CSnmpAcctTotal()
{
	strcpy(strFileName,"/var/SNMP/DATA/UdaAccountTotal.stat");
	FileChkNMake();
	InitValue();
}

CSnmpAcctTotal::~CSnmpAcctTotal()
{
	fclose(fpSnmpTotal);
}

// initialize object values 
void CSnmpAcctTotal::InitValue(void)
{
	unStart = unIterim = unStop = unDrop = unTotal = 0;	
}   

// 파일을 찾아서 없으면 만든다.
void CSnmpAcctTotal::FileChkNMake(void)
{
	if ((fpSnmpTotal = fopen(strFileName, "r")) == NULL) {
		fpSnmpTotal = fopen(strFileName, "w");
		fclose(fpSnmpTotal);
	}else fclose(fpSnmpTotal);
}

// 파일의 첫라인부터 자신의 프로세스 alias를 찾는다.
// 찾으면 '라인번호', 아니면 '-1'RETURN
int	CSnmpAcctTotal::FindMyNas(void)
{
	int		nLineNo = 0;
	uint	nTmpPort;
	char	strTmpNas[9];
	char	strTmpIp[16];
	
	memset(strStreamBuf,0x00,513);
	memset(strTmpNas,0x00,9);
	memset(strTmpIp,0x00,16);
	if((fpSnmpTotal = fopen(strFileName, "r")) == 0x00){
//		printf("File[%s] open error!!\n", strFileName);
		system("touch /var/SNMP/DATA/UdaAccountTotal.stat");
		fpSnmpTotal = fopen(strFileName, "r");
		return -1;
	}
	if(fgets(strStreamBuf, 513, fpSnmpTotal) != 0x00) {
//		printf("Read strStreamBuf:[%s]\n",strStreamBuf);
		sscanf(strStreamBuf,"%u\t%u\t%u\t%u\t%u\t",
				&unStart,&unIterim,&unStop,&unDrop,&unTotal);
		fclose(fpSnmpTotal);
		gnMyLineNo = nLineNo;
	}else {
		InitValue();
		gnMyLineNo = nLineNo;
		fclose(fpSnmpTotal);
	}
	return nLineNo;
}


// 먼저 accessor를 통하여 변경하고자하는 값을 set하고 MakeRecord함수를 호출한다
void CSnmpAcctTotal::MakeRecord(void)
{
	memset(strStreamBuf,0x20,513);
	sprintf(strStreamBuf,"%u\t%u\t%u\t%u\t%u",
			unStart, unIterim, unStop, unDrop, unTotal);	
	memcpy(&strStreamBuf[strlen(strStreamBuf)]," ",1);
	//printf("MakeRecord : [%s]\n", strStreamBuf);
}

// 변경하고자하는 Record가 만들어지고난 후 파일에 기록하는 함수
int	CSnmpAcctTotal::WritePerform(void)
{
	char	strTRecord[512];
	long	offset;
	int		fp;
	
	MakeRecord();
	memset(strTRecord,0x20,512);
	memcpy(strTRecord,strStreamBuf,500);
	memcpy(&strTRecord[510],"\r\n",2);

	if ((fp = open(strFileName, O_RDWR)) == NULL) {
		printf("file open error!\n");
		return -1;
	}
	
	lseek(fp, 0, SEEK_SET); // total파일은 한 라인만 존재한다
	write(fp, strTRecord,512);
	
//	printf("RecordSize : [%d], offset :[%ld]\n", strlen(strTRecord), offset);
//	printf("File Write End!\n");
	
	close(fp);
	return 1;
}


#ifndef	_SNMP_ACCT_HPP_
#define _SNMP_ACCT_HPP_

class CSnmpAcct
{
	private:
		FILE*	fpSnmpSvr;		// 통계를 쓰기위한 파일포인터
		char	strFileName[100];	// 통계파일 이름
		char	strStreamBuf[513];	// 파일의 내용을 한라인 단위로 가져오기 위한 버퍼
		int		gnMyLineNo;			// 파일에 내용을 쓸때 그 라인에 가서 쓴다.
		
		// refer to "SNMP_Agent.doc" $1.4.3.5
		uint	unIndex;			// index
		char	strSvrIpAddr[16];	// pdsn ip address
		uint	unStart;			// 해당 PDSN으로부터 전송된 Start UDR 개수 
		uint	unIterim;			// 해당 PDSN으로부터 전송된 Interim UDR 개수 
		uint	unStop;				// 해당 PDSN으로부터 전송된 Stop UDR 개수 
		uint	unDrop;				// 해당 PDSN으로부터 전송된 Drop UDR 개수 
		uint	unTotal;			// 해당 PDSN으로부터 전송된 Total UDR 개수 

	public:
		CSnmpAcct();
		~CSnmpAcct();
		// accessor
		void	Set_Index	(uint i)		{ unIndex	= i; }
		void	Set_Start	(uint i)		{ unStart  	= i; }
		void	Set_Iterim	(uint i)		{ unIterim	= i; }
		void	Set_Stop	(uint i)		{ unStop	= i; }
		void	Set_Drop	(uint i)		{ unDrop	= i; }
		void	Set_Total 	(uint i)		{ unTotal 	= i; }
		void	Set_IpAddr	(char* s) { strcpy(strSvrIpAddr, s); }

		uint	Get_Index	()	{ return unIndex ; }
		uint	Get_Start	()	{ return unStart ; }
		uint	Get_Iterim	()	{ return unIterim; }
		uint	Get_Stop	()	{ return unStop	 ; }
		uint	Get_Drop	()	{ return unDrop	 ; }
		uint	Get_Total 	()	{ return unTotal ; }
		char* 	Get_IpAddr	()	{ return strSvrIpAddr; }

		void	FileChkNMake();
		int		FindMyNas();
		void	InitValue();
		void	MakeRecord();
		int		WritePerform();
};

#endif 


CSnmpAcct::CSnmpAcct()
{
	strcpy(strFileName,"/var/SNMP/DATA/UdaAccount.stat");
	FileChkNMake();
	InitValue();
}

CSnmpAcct::~CSnmpAcct()
{
	fclose(fpSnmpSvr);
}

// initialize object values 
void CSnmpAcct::InitValue(void)
{
	unIndex = unStart = unIterim = unStop = unDrop = unTotal = 0;	
}   

// 파일을 찾아서 없으면 만든다.
void CSnmpAcct::FileChkNMake(void)
{
	if ((fpSnmpSvr = fopen(strFileName, "r")) == NULL) {
		fpSnmpSvr = fopen(strFileName, "w");
		fclose(fpSnmpSvr);
	}else fclose(fpSnmpSvr);
}

// 파일의 첫라인부터 자신의 프로세스 alias를 찾는다.
// 찾으면 '라인번호', 아니면 '-1'RETURN
int	CSnmpAcct::FindMyNas(void)
{
	int		nLineNo = 0;
	uint	nTmpPort;
	char	strTmpIp[16];
	
	memset(strStreamBuf,0x00,513);
	memset(strTmpIp,0x00,16);
	if((fpSnmpSvr = fopen(strFileName, "r")) == 0x00){
//		printf("File[%s] open error!!\n", strFileName);
		system("touch /var/SNMP/DATA/UdaAccount.stat");
		fpSnmpSvr = fopen(strFileName, "r");
		return -1;
	}
	printf("Start Find NAS : [%s]\n",Get_IpAddr());
	while (1) {
		if(fgets(strStreamBuf, 513, fpSnmpSvr) != 0x00) {
//			printf("Read strStreamBuf:[%s]\n",strStreamBuf);
			if(!strncmp(&strStreamBuf[0],NULL,1)) continue;
			sscanf(strStreamBuf,"%u\t%s\t%u\t%u\t%u\t%u\t%u",
					&unIndex,strTmpIp,&unStart,&unIterim,&unStop,&unDrop,&unTotal);	
		// found
			if(!strncmp(strSvrIpAddr, strTmpIp,strlen(strSvrIpAddr))) {
				printf("Found My ProcAlias....\n");
//				printf("My Perform Data:[%s]\n",strStreamBuf);
				Set_Index(nLineNo);
				fclose(fpSnmpSvr);
				gnMyLineNo = nLineNo;
				return nLineNo;
			}//if
		}else {
			printf("Not My ProcAlias...\n");
			InitValue();
			break;
		}
		memset(strStreamBuf,0x00,513);
		nLineNo++;
	}//while
	// not found
	fclose(fpSnmpSvr);
	InitValue();
	Set_Index(nLineNo);
	gnMyLineNo = nLineNo;
	return -1;
}


// 먼저 accessor를 통하여 변경하고자하는 값을 set하고 MakeRecord함수를 호출한다
void CSnmpAcct::MakeRecord(void)
{
	memset(strStreamBuf,0x20,513);
	sprintf(strStreamBuf,"%u\t%s\t%u\t%u\t%u\t%u\t%u",
			unIndex, strSvrIpAddr, unStart, unIterim, unStop, unDrop, unTotal);	
	memcpy(&strStreamBuf[strlen(strStreamBuf)]," ",1);
	//printf("MakeRecord : [%s]\n", strStreamBuf);
}

// 변경하고자하는 Record가 만들어지고난 후 파일에 기록하는 함수
int	CSnmpAcct::WritePerform(void)
{
	char	strTRecord[512];
	long	offset;
	int		fp;
	
	MakeRecord();
	memset(strTRecord,0x20,512);
	memcpy(strTRecord,strStreamBuf,500);
	memcpy(&strTRecord[510],"\r\n",2);

	if ((fp = open(strFileName, O_RDWR)) == NULL) {
		//printf("file open error!\n");
		return -1;
	}
	
	offset = (RECORD_SIZE) * gnMyLineNo;
	lseek(fp, offset, SEEK_SET); // 파일의 첫부분부터 offset위치까지 옮긴다.
	write(fp, strTRecord,512);
	
//	printf("RecordSize : [%d], offset :[%ld]\n", strlen(strTRecord), offset);
	//printf("File Write End!\n");
	
	close(fp);
	return 1;
}

