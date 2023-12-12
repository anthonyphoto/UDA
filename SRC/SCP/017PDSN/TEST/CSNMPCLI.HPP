#ifndef	_SNMP_CLIENT_PERFORM_HPP
#define _SNMP_CLIENT_PERFORM_HPP

typedef unsigned int 	uint;
#define		RECORD_SIZE		512

class CSnmpCli
{
	private:
		FILE*	fpSnmpCli;			// 통계를 쓰기위한 파일포인터
		char	strFileName[100];	// 통계파일 이름
		char	strStreamBuf[513];	// 파일의 내용을 한라인 단위로 가져오기 위한 버퍼
		int		gnMyLineNo;			// 파일에 내용을 쓸때 그 라인에 가서 쓴다.
		
		// 2620 Object Name
		uint	unIndex;			// (2620 - 5) client index
		char	strNasId[9];		// (2620 - 2) process alias
		char	strSvrIpAddr[16];	// (2620 - 6) sti uda server ip address
		uint	unSvrPort;			// (2620 - 7) sti uda pdun server port
		uint	unTimeInterval;		// (2620 - 8) skt - sti data trans time interval
		uint	unSndUdrCnt;		// (2620 - 9) udr count sent to sti uda server
		uint	unReTransCnt;		// (2620 - 10) retransmission count
		uint	unResCnt;			// (2620 - 11) respose count receive from sti uda pdun server
		uint	unMalformCnt;		// (2620 - 12) 잘못된 패킷 개수
		uint	unKeyErrCnt;		// (2620 - 13) MD5key 가 다른 response count
		uint	unPendingCnt;		// (2620 - 14) Send 시 증가하고 timeout, ack, 재전송시 감소 
		uint	unTimeOutCnt;		// (2620 - 15) time out count
		uint	unUnknownMsgCnt;	// (2620 - 16) 응답메시지 type이 잘못된 경우
		uint	unDropCnt;			// (2620 - 17) not use, default:0
		
		 
	public:
		CSnmpCli(char *strIp, uint unPort, char *strProcName);
		~CSnmpCli();
		// accessor
		void	Set_Index(uint nIndex) 			{ unIndex = nIndex; }
		void	Set_NasId(char *strProcName) 	{ memcpy(strNasId,strProcName,8); }
		void	Set_SvrIpAddr(char *strIp) 		{ strcpy(strSvrIpAddr,strIp); }
		void	Set_SvrPort(uint unPort) 		{ unSvrPort = unPort; } 
		void	Set_TimeInterval(uint unTime) 	{ unTimeInterval = unTime; }
		void	Set_SndUdrCnt(uint unCnt) 		{ unSndUdrCnt = unCnt; }
		void	Set_RetransCnt(uint unCnt) 		{ unReTransCnt = unCnt; }
		void	Set_ResCnt(uint unCnt) 			{ unResCnt = unCnt; }
		void	Set_MalformCnt(uint unCnt) 		{ unMalformCnt = unCnt; }
        void	Set_KeyErrCnt(uint unCnt) 		{ unKeyErrCnt = unCnt; }
        void	Set_PendingCnt(uint unCnt) 		{ unPendingCnt = unCnt; }
		void	Set_TimeOutCnt(uint unCnt) 		{ unTimeOutCnt = unCnt; }
		void	Set_UnknownMsgCnt(uint unCnt) 	{ unUnknownMsgCnt = unCnt; }
		void	Set_DropCnt(uint unCnt) 		{ unDropCnt = unCnt; }
		
		uint	Get_Index(uint nIndex)	{ return unIndex; }
		uint	Get_SvrPort() 			{ return unSvrPort; } 
		uint	Get_TimeInterval() 		{ return unTimeInterval; }
		uint	Get_SndUdrCnt() 		{ return unSndUdrCnt; }
		uint	Get_RetransCnt() 		{ return unReTransCnt; }
		uint	Get_ResCnt() 			{ return unResCnt; }
		uint	Get_MalformCnt() 		{ return unMalformCnt; }
        uint	Get_KeyErrCnt() 		{ return unKeyErrCnt ; }
        uint	Get_PendingCnt() 		{ return unPendingCnt; }
		uint	Get_TimeOutCnt() 		{ return unTimeOutCnt; }
		uint	Get_UnknownMsgCnt() 	{ return unUnknownMsgCnt; }
		uint	Get_DropCnt() 			{ return unDropCnt; }
		
		char*	Get_NasId()				{ return strNasId; }
		void	Get_SvrIpAddr(char *strIp) 		{ strcpy(strIp,strSvrIpAddr); }

		void	FileChkNMake();
		int		FindMyNas();
		void	InitValue();
		void	MakeRecord();
		int		WritePerform();
};

#endif 


CSnmpCli::CSnmpCli(char *strIp, uint unPort, char *strProcName)
{
	strcpy(strFileName,"../SNMP/DATA/UdaCliSnmp.dat");
	FileChkNMake();
	InitValue();
	Set_NasId(strProcName);
	Set_SvrIpAddr(strIp);
	Set_SvrPort(unPort);
}

CSnmpCli::~CSnmpCli()
{
	fclose(fpSnmpCli);
}

// initialize object values 
void CSnmpCli::InitValue(void)
{
	unIndex =  unTimeInterval = unSndUdrCnt = 0;
	unReTransCnt = unResCnt = unMalformCnt = unKeyErrCnt = gnMyLineNo =0;
	unPendingCnt = unTimeOutCnt = unUnknownMsgCnt = unDropCnt = 0;
}

// 파일을 찾아서 없으면 만든다.
void CSnmpCli::FileChkNMake(void)
{
	if ((fpSnmpCli = fopen(strFileName, "r")) == NULL) {
		fpSnmpCli = fopen(strFileName, "w");
		fclose(fpSnmpCli);
	}else fclose(fpSnmpCli);
}

// 파일의 첫라인부터 자신의 프로세스 alias를 찾는다.
// 찾으면 '라인번호', 아니면 '-1', 파일에 내용이 없으면 '0' RETURN
int	CSnmpCli::FindMyNas(void)
{
	int		nLineNo = 0;
	uint	nTmpPort;
	char	strTmpNas[9];
	char	strTmpIp[16];
	
	memset(strStreamBuf,0x00,513);
	memset(strTmpNas,0x00,9);
	memset(strTmpIp,0x00,16);
	if((fpSnmpCli = fopen(strFileName, "r")) == 0x00){
//		printf("File[%s] open error!!\n", strFileName);
		exit(-1);
	}
//	printf("Start Find NAS : [%s]\n",Get_NasId());
	while (1) {
		if(fgets(strStreamBuf, 513, fpSnmpCli) != 0x00) {
//			printf("Read strStreamBuf:[%s]\n",strStreamBuf);
			if(!strncmp(&strStreamBuf[0],NULL,1)) continue;
			
			sscanf(strStreamBuf,"%u\t%s\t%s\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u",
				&unIndex,strTmpNas,strTmpIp,&nTmpPort,&unTimeInterval,&unSndUdrCnt,
				&unReTransCnt,&unResCnt,&unMalformCnt,&unKeyErrCnt,&unPendingCnt,&unTimeOutCnt,
				&unUnknownMsgCnt,&unDropCnt);
			// found
			if(!strncmp(strNasId, strTmpNas,8)) {
//				printf("Found My ProcAlias....\n");
//				printf("My Perform Data:[%s]\n",strStreamBuf);
				Set_Index(nLineNo);
				fclose(fpSnmpCli);
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
	fclose(fpSnmpCli);
	InitValue();
	Set_Index(nLineNo);
	gnMyLineNo = nLineNo;
	return -1;
}

// 먼저 accessor를 통하여 변경하고자하는 값을 set하고 MakeRecord함수를 호출한다
void CSnmpCli::MakeRecord(void)
{
	memset(strStreamBuf,0x20,513);
	
	sprintf(strStreamBuf,"%u\t%s\t%s\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u",
			unIndex,strNasId,strSvrIpAddr,unSvrPort,unTimeInterval,unSndUdrCnt,
			unReTransCnt,unResCnt,unMalformCnt,unKeyErrCnt,unPendingCnt,unTimeOutCnt,
			unUnknownMsgCnt,unDropCnt);
	memcpy(&strStreamBuf[strlen(strStreamBuf)]," ",1);
//	printf("MakeRecord : [%s]\n", strStreamBuf);
	
}

// 변경하고자하는 Record가 만들어지고난 후 파일에 기록하는 함수
int	CSnmpCli::WritePerform(void)
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
	
	fclose(fpSnmpCli);
	return 1;
}



