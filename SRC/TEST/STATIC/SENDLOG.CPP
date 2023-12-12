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
		//	Error Log�� ������ ���� �Ʒ��� ���� �ڵ��մϴ�.
		//	Normal Message�� ����ϰ��� �Ҷ��� clsLogM->ErrorCode(0)�� �Է�
		//
		////////////////////////////////////////////////////////////////////
		*clsLogM<< 	"LOG : Error No #" 	<< clsLogM->ErrorCode(1000) << " "
					"This is a test !!!!" 	<< endl;
		
		cout << "\nSend Error Log Data Success !!" << endl;
		
		//////////////////////////////////////////////////////////////////
		//	���� ������ ���� ������� �߻��� ������ ���� �մϴ�.
		//	���� Method�� Prototype�� ������ �����ϴ�.
		//
		//	nFlag 		: FLAG_START / FLAG_END
		//	nDataType 	: UDR File Seq�� ���� �Ǿ����� ���μ����� 0
		//				  BDS File Seq�� ���� �Ǿ����� ���μ����� 1
		//	unFileNo	: UDR / BDS File No
		//	unUdrCnt	: UDR Count in UDR File
		//	nQueryCnt	: �ѵ� ���� ���� ���� ȸ��
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
		//	Event Hist ������ ���� ������� �߻��� ������ ���� �մϴ�.
		//	���� Method�� Prototype�� ������ �����ϴ�.
		//	void		put_EventHistData	(	int			 nFlag,
		//										int			 nExitCode
		//									);
		//	nFlag		: FLAG_START / FLAG_END (���μ��� ���� / ����)
		//	nExitCode	: ���� �ڵ� (��, ���۽ô� 0)
		//////////////////////////////////////////////////////////////////
		clsLogM->put_EventHistData(FLAG_START, 0);
		sleep(2);
		clsLogM->put_EventHistData(FLAG_END, 0);
		
		cout << "Send Event Hist Data Success !!" << endl;
	}
}
