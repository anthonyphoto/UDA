#ifndef	_SNMP_CLIENT_PERFORM_HPP
#define _SNMP_CLIENT_PERFORM_HPP

typedef unsigned int 	uint;
#define		RECORD_SIZE		512

class CSnmpCli
{
	private:
		FILE*	fpSnmpCli;			// ��踦 �������� ����������
		char	strFileName[100];	// ������� �̸�
		char	strStreamBuf[513];	// ������ ������ �Ѷ��� ������ �������� ���� ����
		int		gnMyLineNo;			// ���Ͽ� ������ ���� �� ���ο� ���� ����.
		
		// 2620 Object Name
		uint	unIndex;			// (2620 - 5) client index
		char	strNasId[9];		// (2620 - 2) process alias
		char	strSvrIpAddr[16];	// (2620 - 6) sti uda server ip address
		uint	unSvrPort;			// (2620 - 7) sti uda pdun server port
		uint	unTimeInterval;		// (2620 - 8) skt - sti data trans time interval
		uint	unSndUdrCnt;		// (2620 - 9) udr count sent to sti uda server
		uint	unReTransCnt;		// (2620 - 10) retransmission count
		uint	unResCnt;			// (2620 - 11) respose count receive from sti uda pdun server
		uint	unMalformCnt;		// (2620 - 12) �߸��� ��Ŷ ����
		uint	unKeyErrCnt;		// (2620 - 13) MD5key �� �ٸ� response count
		uint	unPendingCnt;		// (2620 - 14) Send �� �����ϰ� timeout, ack, �����۽� ���� 
		uint	unTimeOutCnt;		// (2620 - 15) time out count
		uint	unUnknownMsgCnt;	// (2620 - 16) ����޽��� type�� �߸��� ���
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
	strcpy(strFileName,"/var/SNMP/DATA/UdaCliSnmp.dat");
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

// ������ ã�Ƽ� ������ �����.
void CSnmpCli::FileChkNMake(void)
{
	if ((fpSnmpCli = fopen(strFileName, "r")) == NULL) {
		fpSnmpCli = fopen(strFileName, "w");
		fclose(fpSnmpCli);
	}else fclose(fpSnmpCli);
}

// ������ ù���κ��� �ڽ��� ���μ��� alias�� ã�´�.
// ã���� '���ι�ȣ', �ƴϸ� '-1', ���Ͽ� ������ ������ '0' RETURN
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
		system("touch /var/SNMP/DATA/UdaCliSnmp.dat");
		fpSnmpCli = fopen(strFileName, "r");
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

// ���� accessor�� ���Ͽ� �����ϰ����ϴ� ���� set�ϰ� MakeRecord�Լ��� ȣ���Ѵ�
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

// �����ϰ����ϴ� Record�� ����������� �� ���Ͽ� ����ϴ� �Լ�
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
	lseek(fp, offset, SEEK_SET); // ������ ù�κк��� offset��ġ���� �ű��.
	write(fp, strTRecord,512);
	
//	printf("RecordSize : [%d], offset :[%ld]\n", strlen(strTRecord), offset);
//	printf("File Write End!\n");
	
	fclose(fpSnmpCli);
	return 1;
}


