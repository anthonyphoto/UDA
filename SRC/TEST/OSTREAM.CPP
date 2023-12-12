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
	
	//	s를 int형으로 변환해서 nB에 Assign하는 방법을
	//	찾아 주시길......
	
	// 아래 시도는 현재 에러가 남.

    
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

// ostream 을 이용하는 방법은 모르겠습니다  생각는 것은  s << dec 하면 숫자로 바뀝니다..
//   그리고 ostream& s <-- 레퍼런스인데요... 초기화된 variable을 찾습니다...
//   더 연구해 보겠습니다...

// strstream 을 이용하는 방법...
/*
    strstream strm;

	int		nA = 10000;
	int		nC = 111;
	int		nB ;
	
	strm = makeStrProcessID(nA, nB);

//    strm << nA << nC;

    nB = atoi(strm.str());

    cout << nB << endl;
    delete strm.str();    // strm.str() 메소드 사용후에는 반드시 ...
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


