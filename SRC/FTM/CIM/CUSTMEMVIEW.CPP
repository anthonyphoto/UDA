#include"CustMemView.hpp"

static int  SystemId;
static int  ServiceId;
static int  GroupId;
static int  ProcessType;
static int  PserialNo;
static int  PreMark;

int main(int argc, char* argv[])
{
	CCustMemView*   pCustMemView;


	if( argc != 7 )    
	{        
		cout << "Usage CustMemView [SysId][SerId][GroupId][ProcessId][SerialNumber][Remark]" << endl;
		exit( 1 );                   
	}                            

	SystemId=atoi(argv[1]);
	ServiceId=atoi(argv[2]);
	GroupId=atoi(argv[3]);
	ProcessType=atoi(argv[4]);
	PserialNo=atoi(argv[5]);
	PreMark=atoi(argv[6]);

	pCustMemView = new CCustMemView(SystemId,ServiceId,GroupId,ProcessType,PserialNo,PreMark );

	pCustMemView->CustMemView();
	return 0;

}

