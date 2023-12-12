#include <iostream.h>
#include <ShmInfo.hpp>
#include <SendLogManage.hpp>

SendLogManage*	clsLogM;

int sum(int,int);

int main(int argc, char* argv[])
{
	if(argc < 7)
	{
		cout << "Check your arg !!!" << endl;
		exit(-1);
	}
	clsLogM = new SendLogManage(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]),
								atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));

	*clsLogM << "a + b = " << sum(1, 3) << clsLogM->ErrorCode(0) <<endl;

	for (int i = 0; i < 100; i++)
	{
		////////////////////////////////////////////////////////////////////
		//	Error Log를 생성할 때는 아래와 같이 코딩합니다.
		//	Normal Message를 출력하고자 할때는 clsLogM->ErrorCode(0)을 입력
		//
		////////////////////////////////////////////////////////////////////
		*clsLogM<< 	"LOG : Error No #" 	<< clsLogM->ErrorCode(1000) << " "
					"This is a test !!!!" 	<< endl;
		
		cout << "\nSend Error Log Data Success !!" << endl;
		
		//////////////////////////////////////////////////////////////////
		//	성능 정보에 대한 통계정보 발생은 다음과 같이 합니다.
		//	먼저 Method의 Prototype은 다음과 같습니다.
		//
		//	nFlag 		: FLAG_START / FLAG_END
		//	nDataType 	: UDR File Seq로 관리 되어지는 프로세스는 0
		//				  BDS File Seq로 관리 되어지는 프로세스는 1
		//	unFileNo	: UDR / BDS File No
		//	unUdrCnt	: UDR Count in UDR File
		//	nQueryCnt	: 한도 고객 정보 질의 회수
		//
		//	void		put_PerformanceData	(	int			 nFlag,
		//										int 		 nDataType,
		//										unsigned int unFileNo,
		//										unsigned int unUdrCnt,
		//										unsigned int nQueryCnt
		//									);
		//////////////////////////////////////////////////////////////////
		clsLogM->put_PerformanceData( FLAG_START, 1, 100, 24, 4);
		sleep(2);
		clsLogM->put_PerformanceData( FLAG_END, 1, 100, 24, 4);
		//////////////////////////////////////////////////////////////////
		cout << "Send Performance Data Success !!" << endl;
		
		//////////////////////////////////////////////////////////////////
		//	Event Hist 정보에 대한 통계정보 발생은 다음과 같이 합니다.
		//	먼저 Method의 Prototype은 다음과 같습니다.
		//	void		put_EventHistData	(	int			 nFlag,
		//										int			 nExitCode
		//									);
		//	nFlag		: FLAG_START / FLAG_END (프로세스 시작 / 종료)
		//	nExitCode	: 종료 코드 (단, 시작시는 0)
		//////////////////////////////////////////////////////////////////
		clsLogM->put_EventHistData(FLAG_START, 0);
		sleep(2);
		clsLogM->put_EventHistData(FLAG_END, 0);
		
		cout << "Send Event Hist Data Success !!" << endl;
	}
}

