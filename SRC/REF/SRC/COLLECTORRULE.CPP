#include "../INCLUDE/collectorRule.hpp"

CCollectorRule::CCollectorRule() : CConvReFormat()
{
}

CCollectorRule::~CCollectorRule()
{
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : oneField
//   Usage          : 하나의 field를 변환하는 함수 
//	 Argument		: char* indata (변환할 전체 data)
//					  int nOffset (data에서 읽어들일 offset)
//					  int nInOneLen (data에서 읽어들일 size)
//					  int nOneLen (out put data size)
//					  int nDataType (dataType)
//					  int nActionCode (action code)
//					  char strSubAction (Sub action code)
//	 Return			: char* (변환된 one fleid data)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
char* CCollectorRule::oneField(char* indata, int nOffset, int nInOneLen, int nOutLen, 
							   int nDataType, int nActionCode, char strSubAction)
{
	cout << "indata = " << indata << endl;

	char*	strInOneData;
	char*	strOutOneData;
	
	strInOneData 	= new char[nInOneLen+1];
	strOutOneData 	= new char[nOutLen+1];
	auto_ptr<char> apReturnValue(strOutOneData);
	
	memset (strInOneData, NULL, 			nInOneLen+1);
	memcpy (strInOneData, &indata[nOffset], nInOneLen);

//	cout << "strInOneData= " << strInOneData << endl;
	nOneFieldLen 	= nOutLen;
	memset (strOutOneData, 0x20, 	nOutLen);
	strOutOneData[nOutLen] =  NULL;
	
	//	임시 저장 변수들 
	int 			nTemp  = 0;
	unsigned int 	unTemp = 0;
	long			lTemp  = 0;
	unsigned long	ulTemp = 0;
	short			sTemp  = 0;
	unsigned short	usTemp = 0;
	char			cTemp;
	char			strTemp[128];

    switch  (nDataType)    
    {
		case  0 :  
	 				memcpy(&nTemp,strInOneData,sizeof(int));
					memcpy(strOutOneData, convTimeToStr(nTemp,nOutLen), nOutLen);
					break;

		case  18 :
	 				memcpy(&nTemp,strInOneData,sizeof(int));
					memcpy(strOutOneData, convIntToStr(nTemp,nOutLen), nOutLen);
					break;
		case  28 :
    				memcpy(&unTemp,strInOneData,sizeof(unsigned int));
    				memcpy(strOutOneData, convUnIntToStr(unTemp,nOutLen), nOutLen);
    				break;
		case  38 :
    				memcpy(&lTemp,strInOneData,sizeof(long));
    				memcpy(strOutOneData, convLongToStr(lTemp,nOutLen), nOutLen);
    				break;
		case  48 :
    				memcpy(&ulTemp,strInOneData,sizeof(unsigned long));
    				memcpy(strOutOneData, convUnLongToStr(ulTemp,nOutLen), nOutLen);
    				break;
		case  58 :
    				memcpy(&sTemp,strInOneData,sizeof(short));
    				memcpy(strOutOneData, convShortToStr(sTemp,nOutLen), nOutLen);
    				break;
		case  68 :
 	  				memcpy(&usTemp,strInOneData,sizeof(unsigned short));
    				memcpy(strOutOneData, convUnShortToStr(usTemp,nOutLen), nOutLen);
    				break;
		case  78 :
    				memcpy(&cTemp,strInOneData,sizeof(char));
    				memcpy(strOutOneData, convCharToStr(cTemp,nOutLen), nOutLen);
    				break;
		case  88 : 
					memcpy(strOutOneData,strInOneData,nOutLen);
					break;
		case  98 :
					memcpy(&unTemp,strInOneData,sizeof(unsigned int));
					memcpy(strOutOneData, ipToStr(unTemp), nOutLen);
					break;
    	default  :
    				cout << "Undefined Data type" << endl;
					delete[] strInOneData; 
    				return NULL;
    }

	switch(nActionCode)
	{
		case 0 :
				break;
		case 1 :
				memcpy(strOutOneData, convPrefixChar(strOutOneData, strSubAction, nOutLen), nOutLen);
				break;
		case 2 :
				memcpy(strOutOneData, convPostfixChar(strOutOneData, strSubAction, nOutLen), nOutLen);
				break;
	}
	delete[] strInOneData; 
	return strOutOneData;
}

/*
int main()
{
	char aa[1024];
	int  aaa=123124;
	unsigned long cc = 6898;
	char bb [1024];
	int ee = 123456;
	memcpy (bb, &cc, 4);
	memcpy (&bb[4], &ee, 4);
	CCollectorRule 	asdf;
//	test one field
	memcpy(aa, asdf.oneField(bb, 0, 4, 10, 48, 1, ' '), asdf.get_OneFieldLen());
	cout << "aa = " << aa << endl;
	cout << "OneFieldLen() = " << asdf.get_OneFieldLen() << endl;

	memcpy(aa, asdf.oneField(bb, 0, 4, 10, 38, 1, ' '), asdf.get_OneFieldLen());
	cout << "aa = " << aa << endl;
	cout << "OneFieldLen() = " << asdf.get_OneFieldLen() << endl;
	memcpy(aa, asdf.oneField(bb, 4, 4, 10, 18, 0, ' '), asdf.get_OneFieldLen());
	cout << "aa = " << aa << endl;
	memcpy(aa, asdf.oneField(bb, 0, 4, 16, 98, 2, '0'), asdf.get_OneFieldLen());
	cout << "aa = " << aa << endl;

	cc = 1231654;
	memcpy (bb, &cc, 4);
	memcpy(aa, asdf.oneField(bb, 0, 4, 10, 18, 0, ' '), asdf.get_OneFieldLen());

	char dd = '9';
	memset(bb, NULL, 1024);
	memset(aa, NULL, 1024);
	memcpy (bb, &dd, 1);
	memcpy(aa, asdf.oneField(bb, 0, 1, 10, 78, 1, '0'), asdf.get_OneFieldLen());
	cout << "aa = " << aa << endl;
}

*/
/* 실행결과 
indata = 
strInOneData= 
48
ulTemp : 6898
6898
aa =       6898
OneFieldLen() = 10
indata = 
strInOneData= 
38
lTemp : 6898
6898
aa =       6898
OneFieldLen() = 10
indata = 
strInOneData= 
18
nTemp : 123456
123456
aa = 123456
indata = 
strInOneData= 
98
unTemp2 : 6898
  0  0 26242
aa =   0  0 262420000
indata = 
strInOneData= 
18
nTemp : 1231654
1231654
indata = 9
strInOneData= 9
78
cTemp : 9
9
aa = 0000000009*/
