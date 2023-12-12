#include "bdspC.hpp"

bdspC *bdsC;
SendLogManage	*clsLogM;
SendLogManage	*logout;

	
int main(int argc, char **argv)
{
	
	void sigCapture(int sigNo);			

	int		systemid;
	int		serviceid;
	int		groupid;
	int		processtype;
	int		pserialno;
	int		premark;
	int		nPortNum;
	char	strIpAddr[32];
	
	if(argc != 9)
	{
		cout <<"Usage:" << argv[0] << " [systemid] [serviceid] [groupid] [processtype] [serialno] [remarks] [ipaddr] [port]\n";
//		bdsC->writePMSMQ(FLAG_PROC_ABNORMAL);//-2//���μ��� ������ stop (Network or Parrm������ ��������)
		exit(-2);//���μ��� ������ stop (Network or Parrm������ ��������)
	}
	

	signal(SIGINT,sigCapture);
	signal(SIGTERM,sigCapture);
	signal(SIGKILL,sigCapture);
	signal(SIGQUIT,sigCapture);
	signal(SIGUSR1,sigCapture);
	
	systemid    = atoi(argv[1]);
	serviceid   = atoi(argv[2]);
	groupid     = atoi(argv[3]);
	processtype = atoi(argv[4]);
	pserialno   = atoi(argv[5]);
	premark     = atoi(argv[6]);
	strcpy(strIpAddr,  argv[7]);
	nPortNum    = atoi(argv[8]);
	
	sprintf(FILE_NAME_COL_BDS_UDR, "/SVC%1d/BDS/BDS_UDR.", serviceid);

	logout = new SendLogManage(systemid, serviceid, groupid, processtype, pserialno, premark);
	
	bdsC = new bdspC(systemid, serviceid, groupid, processtype, pserialno, premark, strIpAddr,nPortNum);
	
	bdsC->run();
	
	delete bdsC;
	delete logout;
	
}
/*******************************
void sigCapture(int sigNo)
{
	//performancecout << "[NOTICE] BDSIF interrupted by user. closing..." << endl;

	if (bdsC->sendMsg2Bds(DISCONNECT_REQ) == SC);
		//performancecout << "[NOTICE] sendMsg2Bds(DISCONNECT_REQ) success!" << endl;
	//performanceelse
		//performancecout << "[ERROR] sendMsg2Bds(DISCONNECT_REQ) fail!" << endl;
	//bdsC->recvMsgFromBds();//ihy 10.07
	bdsC->fLinkStatus == LINK_STATUS_CLOSED_ABNORMAL;					
	//performancecout << "[NOTICE] BDSIF closed by user! disconnect socket by BDSIF!" << endl;

	//����4�� ����
	bdsC->_socket->CloseTCPSock();
	//����3�� �����ϰ� ���α׷� ����
	bdsC->_socket->CloseTCPSock(bdsC->gfdListenSock);

	//delete bdsC;
	exit(-1);//���μ��� ������ stop (killed)
}
*******************************/


void sigCapture(int sigNo)
{

	if (bdsC->sendMsg2Bds(DISCONNECT_REQ) == SC);
	bdsC->fLinkStatus == LINK_STATUS_CLOSED_ABNORMAL;					
	//����4�� ����
	bdsC->_socket->CloseTCPSock();
	//����3�� �����ϰ� ���α׷� ����
	bdsC->_socket->CloseTCPSock(bdsC->gfdListenSock);

	switch(sigNo)
	{
		case SIGINT :
		case SIGQUIT :
		case SIGKILL :
		case SIGTERM :
				bdsC->writePMSMQ(FLAG_PROC_KILL);//-1//���μ��� ������ stop (killed)

				//EventHist���� nFlag, �����ڵ�(��, ���۽ô� 0)
				logout->put_EventHistData(FLAG_END, -1);
				
				exit(-1);//���μ��� ������ stop (killed)
				break;
		case SIGUSR1:
				bdsC->writePMSMQ(FLAG_PROC_TERM);//0//���μ�������stop (Client��������)
				
				//EventHist���� nFlag, �����ڵ�(��, ���۽ô� 0)
				logout->put_EventHistData(FLAG_END, 0);
				
				exit(0);//���μ�������stop (Client��������)
				break;
		default:
				bdsC->writePMSMQ(FLAG_PROC_KILL);//-1//���μ��� ������ stop (killed)
				
				//EventHist���� nFlag, �����ڵ�(��, ���۽ô� 0)
				logout->put_EventHistData(FLAG_END, -1);
				
				exit(-1);//���μ��� ������ stop (killed)
				break;//���μ��� ������ stop (killed)
	}
}



