#include "nTop.hpp"

#define		LEN_NTOP_FILENAME	44

MessageQueue<SCPFormatAgg>	*mqSCP; 
MessageQueue<MsgPmsStatus>  *PMSMQ;
SendLogManage 				*clsLogM; 

SCPFormatAgg 				SCPFormatNTOP;		
nTop_FTP					clsNTOPFTP;

int		FileAccess();
void	sigCapture(int sigNo);
void	writePMSMQ(int sigNo);

int		systemId;
int		serviceId;
int		groupId;
int		processType;
int		pserialNo;
int		pRemark;

FILE		*fd, *lsf;
char	    PATH_NAME_NTOP_CFG[80];

int main(int argc, char* argv[])
{

	if (argc < 7)
	{
		////////////////////////////////////////////////////////////////
		*clsLogM 	<< clsLogM->ErrorCode(0)
					<< "Usage: ntp [SystemId][ServiceId][GroupType][ProcessType][SerialNo][Remark]" << endl;
		////////////////////////////////////////////////////////////////
 		clsLogM->put_EventHistData(FLAG_END, -1);
 		exit(-1);
 	}

	//signal(SIGNULL,sigCapture);	//0
	signal(SIGHUP,	sigCapture);
	signal(SIGINT,	sigCapture);
	signal(SIGQUIT,	sigCapture);
	signal(SIGKILL,	sigCapture);
	signal(SIGTERM,	sigCapture);
	signal(SIGSTOP,	sigCapture);
	signal(SIGTSTP,	sigCapture);
	signal(SIGCONT,	sigCapture);	//26
	signal(SIGUSR1,	sigCapture);
	
	systemId    = 	atoi(argv[1]);
	serviceId   = 	atoi(argv[2]);
	groupId     = 	atoi(argv[3]);
	processType = 	atoi(argv[4]);
	pserialNo   = 	atoi(argv[5]);
	pRemark     = 	atoi(argv[6]); 

	PMSMQ 	 	= 	new MessageQueue<MsgPmsStatus>(systemId, 0, 1, 1, 0, 10, 1000);
	clsLogM	 	= 	new SendLogManage(systemId, serviceId, groupId, processType, pserialNo, pRemark);
	mqSCP 		= 	new MessageQueue<SCPFormatAgg>(systemId, serviceId,4,1,0,10,1000 );
	writePMSMQ(FLAG_PROC_START);

	////////////////////////////////////////////////////////////////
	*clsLogM 	<< clsLogM->ErrorCode(0)<< "NTop Start..."	<< endl;
	////////////////////////////////////////////////////////////////

	STRINGBUF		stStringBuf;
	
	char 			strBuf[MAXLEN];
	char			strMovePath[MAXLEN];	// /SVC1/BDS/NB/에 있는 file을 Move할 directory지정:/SVC1/BDS/WORK/TEMP   

	memset((char *)&stStringBuf, 0x00,  sizeof(stStringBuf));
	memset(strMovePath  	   , 0x00,	sizeof(strMovePath));	
	sprintf(strMovePath		   , "/SVC%d/BDS/WORK/TEMP/"	 , serviceId);
	sprintf(PATH_NAME_NTOP_CFG ,"/SVC%d/DATA/NTOP_Info.cfg"  , serviceId);		
	
	clsNTOPFTP.read_nTopConfigFile();
	
	// 비정상 종료한 경우 마지막으로 보낸 레코드 다음부터              
	// SCP Format으로 변환한 후 SCP MQ로 Send            
	if((fd = fopen(clsNTOPFTP.get_strNTopFileName1(), "r")) != NULL)
	{
		////////////////////////////////////////////////////////////////
		*clsLogM 	<< clsLogM->ErrorCode(0)<< "=====NTopFileName ["	
					<< clsNTOPFTP.get_strNTopFileName1()
					<< "] Sending after conversion SCP Format====="
					<< endl;
		////////////////////////////////////////////////////////////////
		
		for(int i = 0; i < clsNTOPFTP.get_nUdrCount(); i++)
		{
			memset(strBuf, 0x00, sizeof(strBuf));
			fread(strBuf, LEN_INPUT_FILE, 1, fd);
		}	
		clsNTOPFTP.SendNTopFile(clsNTOPFTP.get_nUdrCount());
		////////////////////////////////////////////////////////////////
		*clsLogM 	<< clsLogM->ErrorCode(0)<< "Record Count : " << clsNTOPFTP.get_nUdrCount() 
					<< "\n" << "\n"
					<< endl;
		////////////////////////////////////////////////////////////////

		clsNTOPFTP.MoveFile(strMovePath);
	}	
	while(1)
	{
		// /SVC1/BDS/NB/에 있는 file name access   	
 		if(FileAccess() == -1)
 		{
 			clsLogM->put_EventHistData(FLAG_END, -1);
 			exit(-1);
 		}
 			
		// ex)/SVC1/BDS/NB/SIGA.D2001201.S002.F0025052.B01형식의 file을 open 
		if((fd = fopen(clsNTOPFTP.get_strNTopFileName1(), "r")) == NULL){

			////////////////////////////////////////////////////////////////
			*clsLogM << clsLogM->ErrorCode(0)<< "Open Fail [" << clsNTOPFTP.get_strNTopFileName1() << "]" << endl;
			////////////////////////////////////////////////////////////////
 			clsLogM->put_EventHistData(FLAG_END, -1);
			exit(-1);
		}	

		clsNTOPFTP.put_nUdrCount(0);		// Record Count in file 			
		clsNTOPFTP.SendNTopFile(clsNTOPFTP.get_nUdrCount()); 
			
		////////////////////////////////////////////////////////////////
		*clsLogM 	<< clsLogM->ErrorCode(0)<< "Record Count : " << clsNTOPFTP.get_nUdrCount() 
					<< "\n" << "\n"
					<< endl;
		////////////////////////////////////////////////////////////////

		clsNTOPFTP.MoveFile(strMovePath);
	}	// end out while 
}

//------------------------------------------------------------------------------
// /SVC1/BDS/NB/에 있는 file name을 Class:nTopFileConfig의 멤버인                 
// strNTopFileName1(ex)/SVC1/BDS/NB/SIGA.D2001201.S002.F0025052.B01)                   ,
// strNTopFileName2(ex)/SVC1/BDS/NB/SIGA.D2001201.S002.F0025052.B01.INFO)에 저장       

int FileAccess()
{
	char strCmd[MAXLEN];		// ll -rt /SVC%d/BDS/NB/ |grep ntop|awk '{print $9}'	      
	char strBuf[MAXLEN];		// 처음으로 fgets()할때 사용                                 
	char strBuf1[MAXLEN];		// 처음으로 fgets()해서 얻은 file name을 임시 저장              	
	char strNTopName1[MAXLEN];	// ex)SIGA.D2001201.S002.F0025052.B01형식의 file name 저장                         
	char strNTopName2[MAXLEN];	// ex)SIGA.D2001201.S002.F0025052.B01.INFO형식의 file name 저장                                 
	char strPathName[MAXLEN];	// /SVC1/BDS/NB/                                                                 

	memset(strCmd      , 	0x00,	strlen(strCmd));
	memset(strBuf	   , 	0x00, 	sizeof(strBuf));
	memset(strBuf1	   , 	0x00, 	sizeof(strBuf1));	
	memset(strNTopName1, 	0x00, 	sizeof(strNTopName1));
	memset(strNTopName2, 	0x00,	sizeof(strNTopName2));
	memset(strPathName , 	0x00, 	sizeof(strPathName));
	
	sprintf(strPathName,	"/SVC%d/BDS/NB/"		 , serviceId);
	sprintf(strCmd	   , 	"ll -rt /SVC%d/BDS/NB/ |grep ntop|awk '{print $9}'", serviceId);
	
	while(1)
	{
		if((lsf = popen(strCmd, "r")) == NULL)
		{

			////////////////////////////////////////////////////////////////
			*clsLogM 	<< clsLogM->ErrorCode(0)<< "Popen1 Fail ["<< strCmd << "]" << endl;
			////////////////////////////////////////////////////////////////
			clsLogM->put_EventHistData(FLAG_END, -1);
			return -1;

		}

		if(fgets(strBuf, LEN_INPUT_FILE, lsf) == NULL)
		{
			pclose(lsf);
			sleep(2);
			continue;
		}
		else	break;
	}	

	sscanf(strBuf		,	"%s"	, strBuf1);
	sprintf(strNTopName1,	"%s"	, strPathName);
	strncat(strNTopName1,	strBuf1 , strlen(strBuf1));
		
	// file name형식이 SIGA....B01.INFO일때 다음에 찾을 file name을 SIGA....B01으로 make    
	// 그렇지 않고 file name형식이 SIGA....B01일때 다음에 찾을 file name을 SIGA....B01.INFO로 make       
	if(strncmp(strNTopName1+LEN_NTOP_FILENAME, ".INFO", 5) ==0)
				strncpy(strNTopName2, strNTopName1, LEN_NTOP_FILENAME);
	else		sprintf(strNTopName2, "%s.INFO"	  , strNTopName1);

	if(access(strNTopName2, F_OK) == -1)                                     
	{                                                                           
		////////////////////////////////////////////////////////////////
		*clsLogM 	<< clsLogM->ErrorCode(0) << "===== Searching [" << strNTopName2 << "] =====" << endl;
		////////////////////////////////////////////////////////////////
		for(int i = 0; ; i++)     
		{
			pclose(lsf);
			
			if((lsf = popen(strCmd, "r")) == NULL)
			{                              
                                                                          
				////////////////////////////////////////////////////////////////
				*clsLogM 	<< clsLogM->ErrorCode(0)<< "Popen2 Fail ["<< strCmd << "]" << endl;
				////////////////////////////////////////////////////////////////
 		        clsLogM->put_EventHistData(FLAG_END, -1);
 		        return -1;                               
			}                                                                   
                                                                          
			if(access(strNTopName2, F_OK) == -1)
			{
				i = 0;
				sleep(2);
				continue;
			}	
			else		break;
		}		                               
	}	

	if(strncmp(strNTopName1+LEN_NTOP_FILENAME, ".INFO", 5) ==0)
	{
		char strTemp[MAXLEN];
			
		memcpy(strTemp	   , strNTopName1, sizeof(strTemp));
		memcpy(strNTopName1, strNTopName2, sizeof(strNTopName1));
		memcpy(strNTopName2, strTemp	 , sizeof(strNTopName2));
	}
	////////////////////////////////////////////////////////////////
	*clsLogM 	<< clsLogM->ErrorCode(0) <<  "nTopFileName : " << strNTopName1 << endl;
	////////////////////////////////////////////////////////////////

	clsNTOPFTP.put_strNTopFileName1(strNTopName1);
	clsNTOPFTP.put_strNTopFileName2(strNTopName2);
	
	pclose(lsf);
	return 0;
}

			
//------------------------------------------------------------------------------
void writePMSMQ(int sigNo)
{
	int i = 0;
	char PMSMQ_NO[2];
	MsgPmsStatus	OneRec(MSG_STATUS, systemId, serviceId, groupId, processType, pserialNo, 10);

	OneRec.set_nStatus(sigNo);
	
	sprintf(PMSMQ_NO, "%d", systemId);
	
	while(1)
	{
		i++;
		if (PMSMQ->sendMsg(&OneRec) == FLAG_MQ_IS_FULL)
		{
			*clsLogM 	<< "LOG : PMSMQIS FULL sleep(1)...!" << endl;
			sleep(1);
			if (i >= 10)
			{
				kill(0,SIGINT);
			}
		}
		else
		{
			break;
		}
	}
}

//------------------------------------------------------------------------------
void sigCapture(int sigNo)
{
	////////////////////////////////////////////////////////////////
	*clsLogM 	<< clsLogM->ErrorCode(0) <<  "Got Signal, so Process Stop!!!" << endl;
	////////////////////////////////////////////////////////////////
	switch(sigNo)
	{
		case SIGINT :
		case SIGQUIT :
		case SIGKILL :
		case SIGTERM :
				writePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				clsLogM->put_EventHistData(FLAG_END, -1);
				delete PMSMQ;				
				delete clsLogM;
				delete mqSCP;
				exit(-1);
				break;
		case SIGUSR1:
				writePMSMQ(FLAG_PROC_TERM);//0//프로세스정상stop (Client업무명령)
				clsLogM->put_EventHistData(FLAG_END, 0);
				delete PMSMQ;
				delete clsLogM;	
				delete mqSCP;
				exit(0);
				break;
		default:
				writePMSMQ(FLAG_PROC_KILL);//-1//프로세스 비정상 stop (killed)
				clsLogM->put_EventHistData(FLAG_END, -1);
				delete PMSMQ;				
				delete clsLogM;
				delete mqSCP;
				exit(-1);
				break;
	}
}



