/*
#include <iostream.h>
#include <string>

int main()
{
	ostream&		s;
	int		nA = 10000;
	int		nC = 111;
	int		nB ;

	s << nA << nC;
	
	//	s�� int������ ��ȯ�ؼ� nB�� Assign�ϴ� �����
	//	ã�� �ֽñ�......
	
	// �Ʒ� �õ��� ���� ������ ��.

    
	//nB = atoi(reinterpret_cast<const char *>(s));
	//cout << nB << endl;

//    cout << dec;
//    cout << s.rdbuf() << endl;

    return 0;
}
*/


#include <iostream.h>
#include <strstream.h>
#include <string>

/*
strstream& makeStrProcessID(int nA, int nB)
{
	strstream& s;
	
	s << nA		\
	  << nB;
	
	return s;
}        
*/

class	Test
{
	private:
		int		nTest;
		char	nString[100];
	public:
		void	put_nTest(int nValue)	{	nTest = nValue;		}
		
		friend void operator << (Test& clsTest, char* strValue)
		{
			memcpy(clsTest.nString, strValue, strlen(strValue));
			cout << "nTest   = " << clsTest.nTest << endl;
			cout << "nString = " << clsTest.nString << endl;
			
			
		}
			
};

int main(void) {

// ostream �� �̿��ϴ� ����� �𸣰ڽ��ϴ�  ������ ����  s << dec �ϸ� ���ڷ� �ٲ�ϴ�..
//   �׸��� ostream& s <-- ���۷����ε���... �ʱ�ȭ�� variable�� ã���ϴ�...
//   �� ������ ���ڽ��ϴ�...

// strstream �� �̿��ϴ� ���...
/*
    strstream strm;

	int		nA = 10000;
	int		nC = 111;
	int		nB ;
	
	strm = makeStrProcessID(nA, nB);

//    strm << nA << nC;

    nB = atoi(strm.str());

    cout << nB << endl;
    delete strm.str();    // strm.str() �޼ҵ� ����Ŀ��� �ݵ�� ...
*/
	Test	clsTest;
	int		nttt = 1;
	
	clsTest.put_nTest(10000);
	clsTest << "Test Test Test";
	
    return 0;
}
/*
strstream& makeStrProcessID(int nA, int nB)
{
	strstream& s;
	
	s << nA		\
	  << nB;
	
	return s;
}        
*/

