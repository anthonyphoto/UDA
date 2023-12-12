typedef unsigned int 	uint;
#define		RECORD_SIZE		512

#ifndef	_SNMP_SVR_TOTAL_PERFORM_HPP
#define _SNMP_SVR_TOTAL_PERFORM_HPP

class CSnmpTotal
{
	private:
		FILE*	fpSnmpTotal;		// 통계를 쓰기위한 파일포인터
		char	strFileName[100];	// 통계파일 이름
		char	strStreamBuf[513];	// 파일의 내용을 한라인 단위로 가져오기 위한 버퍼
		int		gnMyLineNo;			// 파일에 내용을 쓸때 그 라인에 가서 쓴다.
		
		// 2621 Object Name
		uint	unTReqCnt;			// (2621 - 5) total request count
		uint	unNotReqIp;			// (2621 - 6) 잘못된 IP로부터 들어온 UDR 개수
		uint	unDupSeqCnt;		// (2621 - 7) Paket seq가 중복된 개수 
		uint	unTResCnt;			// (2621 - 8) 응답 개수 
		uint	unTMalformCnt;		// (2621 - 9) 헤더오류 개수
		uint	unKeyErrCnt;		// (2621 - 10) MD5 key error 개수 
		uint	unTParsErrCnt;		// (2621 - 11) Parsing error count
		uint	unTFWriteErrCnt;	// (2621 - 12) 파일 쓰기 오류
		uint	unTMsgErrCnt;		// (2621 - 13) 요구메시지 type이 잘못된 경우

		 
	public:
		CSnmpTotal();
		~CSnmpTotal();
		// accessor
		void	Set_TReqCnt(uint unCnt) 		{ unTReqCnt = unCnt; }
		void	Set_NotReqIp(uint unCnt) 		{ unNotReqIp = unCnt; }
		void	Set_DupSeqCnt(uint unCnt) 		{ unDupSeqCnt = unCnt; }
		void	Set_TResCnt(uint unCnt) 		{ unTResCnt = unCnt; }
		void	Set_TMalformCnt(uint unCnt)		{ unTMalformCnt = unCnt; }
		void	Set_KeyErrCnt(uint unCnt) 		{ unKeyErrCnt = unCnt; }
		void	Set_TParsErrCnt(uint unCnt) 	{ unTParsErrCnt = unCnt; }
       	void	Set_TFWriteErrCnt(uint unCnt) 	{ unTFWriteErrCnt = unCnt; }
       	void	Set_TMsgErrCnt(uint unCnt) 		{ unTMsgErrCnt = unCnt; }
       	
		uint	Get_TReqCnt() 		{ return unTReqCnt; }
		uint	Get_NotReqIp() 		{ return unNotReqIp; }
		uint	Get_DupSeqCnt() 	{ return unDupSeqCnt; }
		uint	Get_TResCnt() 		{ return unTResCnt; }
		uint	Get_TMalformCnt()	{ return unTMalformCnt; }
		uint	Get_KeyErrCnt() 	{ return unKeyErrCnt; }
		uint	Get_TParsErrCnt() 	{ return unTParsErrCnt; }
       	uint	Get_TFWriteErrCnt() { return unTFWriteErrCnt; }
       	uint	Get_TMsgErrCnt()	{ return unTMsgErrCnt; }

		void	FileChkNMake();
		int		FindMyNas();
		void	InitValue();
		void	MakeRecord();
		int		WritePerform();
};
#endif

#ifndef	_SNMP_SVR_PERFORM_HPP
#define _SNMP_SVR_PERFORM_HPP

class CSnmpSvr
{
	private:
		FILE*	fpSnmpSvr;			// 통계를 쓰기위한 파일포인터
		char	strFileName[100];	// 통계파일 이름
		char	strStreamBuf[513];	// 파일의 내용을 한라인 단위로 가져오기 위한 버퍼
		int		gnMyLineNo;			// 파일에 내용을 쓸때 그 라인에 가서 쓴다.
		
		// 2621 Object Name
		uint	unIndex;			// (2621 - 16) Server index
		char	strSvrIpAddr[16];	// (2621 - 17) sti uda server ip address
		char	strNasId[9];		// (2621 - 18) process alias
		uint	unParsErrCnt;		// (2621 - 19) parsing error
		uint	unReqCnt;			// (2621 - 20) request count receive from uda pdun client
		uint	unDupCnt;			// (2621 - 21) udr 중복개수
		uint	unResCnt;			// (2621 - 22) 응답개수
		uint	unKeyErrCnt;		// (2621 - 23) MD5key 가 다른 request count
		uint	unMalformCnt;		// (2621 - 24) 헤더오류 개수
		uint	unFWriteErrCnt;		// (2621 - 25) 파일 쓰기 오류
		uint	unMsgErrCnt;		// (2621 - 26) 요구메시지 type이 잘못된 경우
		 
	public:
		CSnmpSvr(char *strProcName);
		~CSnmpSvr();
		// accessor
		void	Set_Index(uint nIndex) 			{ unIndex = nIndex; }
		void	Set_SvrIpAddr(char *strIp) 		{ strcpy(strSvrIpAddr,strIp); }
		void	Set_NasId(char *strProcName) 	{ memcpy(strNasId,strProcName,8); }
		void	Set_ParsErrCnt(uint unCnt) 		{ unParsErrCnt = unCnt; }
		void	Set_ReqCnt(uint unCnt) 			{ unReqCnt = unCnt; }
		void	Set_DupCnt(uint unCnt) 			{ unDupCnt = unCnt; }
		void	Set_ResCnt(uint unCnt) 			{ unResCnt = unCnt; }
		void	Set_KeyErrCnt(uint unCnt) 		{ unKeyErrCnt = unCnt; }
       	void	Set_MalformCnt(uint unCnt) 		{ unMalformCnt = unCnt; }
       	void	Set_FWriteErrCnt(uint unCnt) 	{ unFWriteErrCnt = unCnt; }
       	void	Set_MsgErrCnt(uint unCnt) 		{ unMsgErrCnt = unCnt; }
       	

		uint	Get_Index() 		{ return unIndex; }              
		uint	Get_ParsErrCnt()	{ return unParsErrCnt; }          
		uint	Get_ReqCnt() 		{ return unReqCnt; }              
		uint	Get_DupCnt() 		{ return unDupCnt; }              
		uint	Get_ResCnt() 		{ return unResCnt; }              
		uint	Get_KeyErrCnt() 	{ return unKeyErrCnt; }           
        uint	Get_MalformCnt() 	{ return unMalformCnt; }          
        uint	Get_FWriteErrCnt() 	{ return unFWriteErrCnt; }        
		uint	Get_MsgErrCnt() 	{ return unMsgErrCnt; }       

		
		char*	Get_NasId()			{ return strNasId; }
		char*	Get_SvrIpAddr() 	{ return strSvrIpAddr; }

		void	FileChkNMake();
		int		FindMyNas();
		void	InitValue();
		void	MakeRecord();
		int		WritePerform();
};

#endif
 
///////////////////////////////CSnmpTotal member Fuction//////////////////////////
CSnmpTotal::CSnmpTotal()
{
	strcpy(strFileName,"/var/SNMP/DATA/UdaSvrSnmpTotal.dat");
	FileChkNMake();
	InitValue();
}

CSnmpTotal::~CSnmpTotal()
{
	fclose(fpSnmpTotal);
}

// initialize object values 
void CSnmpTotal::InitValue(void)
{
	unTReqCnt = unNotReqIp = unDupSeqCnt = unTResCnt = unTMalformCnt = 0;
	unKeyErrCnt = unTParsErrCnt = unTFWriteErrCnt = unTMsgErrCnt =0;
}   

// 파일을 찾아서 없으면 만든다.
void CSnmpTotal::FileChkNMake(void)
{
	if ((fpSnmpTotal = fopen(strFileName, "r")) == NULL) {
		fpSnmpTotal = fopen(strFileName, "w");
		fclose(fpSnmpTotal);
	}else fclose(fpSnmpTotal);
}

// 파일의 첫라인부터 자신의 프로세스 alias를 찾는다.
// 찾으면 '라인번호', 아니면 '-1'RETURN
int	CSnmpTotal::FindMyNas(void)
{
	int		nLineNo = 0;
	uint	nTmpPort;
	char	strTmpNas[9];
	char	strTmpIp[16];
	
	memset(strStreamBuf,0x00,513);
	memset(strTmpNas,0x00,9);
	memset(strTmpIp,0x00,16);
	if((fpSnmpTotal = fopen(strFileName, "r")) == 0x00){
		printf("File[%s] open error!!\n", strFileName);
		system("touch /var/SNMP/DATA/UdaSvrSnmpTotal.dat");
		fpSnmpTotal = fopen(strFileName, "r");
		return -1;
	}
	if(fgets(strStreamBuf, 513, fpSnmpTotal) != 0x00) {
//		printf("Read strStreamBuf:[%s]\n",strStreamBuf);
		sscanf(strStreamBuf,"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u",
			&unTReqCnt,&unNotReqIp,&unDupSeqCnt,&unTResCnt,
			&unTMalformCnt,&unKeyErrCnt,&unTParsErrCnt,&unTFWriteErrCnt,&unTMsgErrCnt);

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
void CSnmpTotal::MakeRecord(void)
{
	memset(strStreamBuf,0x20,513);
	sprintf(strStreamBuf,"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u",
			unTReqCnt,unNotReqIp,unDupSeqCnt,unTResCnt,
			unTMalformCnt,unKeyErrCnt,unTParsErrCnt,unTFWriteErrCnt,unTMsgErrCnt);
	memcpy(&strStreamBuf[strlen(strStreamBuf)]," ",1);
//	printf("MakeRecord : [%s]\n", strStreamBuf);
	
}

// 변경하고자하는 Record가 만들어지고난 후 파일에 기록하는 함수
int	CSnmpTotal::WritePerform(void)
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


///////////////////////////////CSnmpSvr member Fuction//////////////////////////
CSnmpSvr::CSnmpSvr(char *strProcName)
{
	strcpy(strFileName,"/var/SNMP/DATA/UdaSvrSnmp.dat");
	FileChkNMake();
	InitValue();
	Set_NasId(strProcName);
}

CSnmpSvr::~CSnmpSvr()
{
	fclose(fpSnmpSvr);
}

// initialize object values 
void CSnmpSvr::InitValue(void)
{
	unParsErrCnt = unIndex =  unReqCnt = unMsgErrCnt = 0;
	unResCnt = unKeyErrCnt = unMalformCnt = unFWriteErrCnt = unDupCnt =0;

}   

// 파일을 찾아서 없으면 만든다.
void CSnmpSvr::FileChkNMake(void)
{
	if ((fpSnmpSvr = fopen(strFileName, "r")) == NULL) {
		fpSnmpSvr = fopen(strFileName, "w");
		fclose(fpSnmpSvr);
	}else fclose(fpSnmpSvr);
}

// 파일의 첫라인부터 자신의 데이타가 들어온 IP를 찾는다.
// 찾으면 '라인번호', 아니면 '-1', 파일에 내용이 없으면 '0' RETURN
int	CSnmpSvr::FindMyNas(void)
{
	int		nLineNo = 0;
	uint	nTmpPort;
	char	strTmpNas[9];
	char	strTmpIp[16];
	
	memset(strStreamBuf,0x00,513);
	memset(strTmpNas,0x00,9);
	memset(strTmpIp,0x00,16);
	if((fpSnmpSvr = fopen(strFileName, "r")) == 0x00){
//		printf("File[%s] open error!!\n", strFileName);
		system("touch /var/SNMP/DATA/UdaSvrSnmp.dat");
		fpSnmpSvr = fopen(strFileName, "r");
//		exit(1);
	}
//	printf("Start Find NAS : [%s]\n",Get_NasId());
	while (1) {
		if(fgets(strStreamBuf, 513, fpSnmpSvr) != 0x00) {
//			printf("Read strStreamBuf:[%s]\n",strStreamBuf);
//			if(!strncmp(&strStreamBuf[0],NULL,1)) continue;
			sscanf(strStreamBuf,"%u\t%s\t%s\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u",
				&unIndex,strTmpIp,strTmpNas,&unParsErrCnt,&unReqCnt,&unDupCnt,
				&unResCnt,&unKeyErrCnt,&unMalformCnt,&unFWriteErrCnt,&unMsgErrCnt);
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
//			printf("Not My ProcAlias...\n");
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
void CSnmpSvr::MakeRecord(void)
{
	memset(strStreamBuf,0x20,513);
	sprintf(strStreamBuf,"%u\t%s\t%s\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u",
			unIndex,strSvrIpAddr,strNasId,unParsErrCnt,unReqCnt,unDupCnt,
			unResCnt,unKeyErrCnt,unMalformCnt,unFWriteErrCnt,unMsgErrCnt);

	memcpy(&strStreamBuf[strlen(strStreamBuf)]," ",1);
//	printf("MakeRecord : [%s]\n", strStreamBuf);
	
}

// 변경하고자하는 Record가 만들어지고난 후 파일에 기록하는 함수
int	CSnmpSvr::WritePerform(void)
{
	char	strTRecord[512];
	long	offset;
	int		fp;
	
	MakeRecord();
	memset(strTRecord,0x20,512);
	memcpy(strTRecord,strStreamBuf,500);
	memcpy(&strTRecord[510],"\r\n",2);

	if ((fp = open(strFileName, O_RDWR)) == NULL) {
//		printf("file open error!\n");
		return -1;
	}
	
	offset = (RECORD_SIZE) * gnMyLineNo;
	lseek(fp, offset, SEEK_SET); // 파일의 첫부분부터 offset위치까지 옮긴다.
	write(fp, strTRecord,512);
	
//	printf("RecordSize : [%d], offset :[%ld]\n", strlen(strTRecord), offset);
//	printf("File Write End!\n");
	
	close(fp);
	return 1;
}


