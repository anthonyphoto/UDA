#include "../INCLUDE/convReFormat.hpp"

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : convTimeToStr
//   Usage          : int���� time��  string���� ��ȯ �ϴ� �Լ� 
//	 Argument		: int nValue (��ȯ�� ����)
//					  int size (string size)
//	 Return			: char* (��ȯ�� string)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
char* CConvReFormat::convTimeToStr (int nValue, int size)
{
	char* 	strReturnValue;
	long	lTemp = nValue;
	strReturnValue = new char[size+1];
	auto_ptr<char> apReturnValue(strReturnValue);
	memset (strReturnValue, NULL, size+1);
	
	struct tm *s;
	
	s = localtime(&lTemp);
	sprintf(strReturnValue,"%04d%02d%02d%02d%02d%02d",  s->tm_year+1900,
														s->tm_mon+1,
														s->tm_mday,
        	        									s->tm_hour,
        	        									s->tm_min,
        	        									s->tm_sec	);
	return strReturnValue;
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : convShortToStr
//   Usage          : short�� string���� ��ȯ �ϴ� �Լ� 
//	 Argument		: int sValue (��ȯ�� ����)
//					  int size (string size)
//	 Return			: char* (��ȯ�� string)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
char* CConvReFormat::convShortToStr (short sValue, int size)
{
	char* 	strReturnValue;
	strReturnValue = new char[size+1];
	auto_ptr<char> apReturnValue(strReturnValue);
	memset (strReturnValue, NULL, size+1);
	
	sprintf (strReturnValue, "%d", sValue);
	return strReturnValue;
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : convUnShortToStr
//   Usage          : unsigned short�� string���� ��ȯ �ϴ� �Լ� 
//	 Argument		: int usValue (��ȯ�� ����)
//					  int size (string size)
//	 Return			: char* (��ȯ�� string)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
char* CConvReFormat::convUnShortToStr (unsigned short usValue, int size)
{
	char* 	strReturnValue;
	strReturnValue = new char[size+1];
	auto_ptr<char> apReturnValue(strReturnValue);
	memset (strReturnValue, NULL, size+1);
	
	sprintf (strReturnValue, "%d", usValue);
	return strReturnValue;
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : convIntToStr
//   Usage          : int�� string���� ��ȯ �ϴ� �Լ� 
//	 Argument		: int nValue (��ȯ�� ����)
//					  int size (string size)
//	 Return			: char* (��ȯ�� string)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
char* CConvReFormat::convIntToStr (int nValue, int size)
{
	char* 	strReturnValue;
	strReturnValue = new char[size+1];
	auto_ptr<char> apReturnValue(strReturnValue);
	memset (strReturnValue, NULL, size+1);
	
	sprintf (strReturnValue, "%d", nValue);
	return strReturnValue;
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : convCharToStr
//   Usage          : char�� string���� ��ȯ �ϴ� �Լ� 
//	 Argument		: char cValue (��ȯ�� ����)
//					  int size (string size)
//	 Return			: char* (��ȯ�� string)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
char* CConvReFormat::convCharToStr (char cValue, int size)
{
	char* 	strReturnValue;
	strReturnValue = new char[size+1];
	auto_ptr<char> apReturnValue(strReturnValue);
	memset (strReturnValue, NULL, size+1);

	sprintf (strReturnValue, "%c", cValue);
	return strReturnValue;
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : convUnIntToStr
//   Usage          : unsigned int�� string���� ��ȯ �ϴ� �Լ� 
//	 Argument		: unsigned int unValue (��ȯ�� ����)
//					  int size (string size)
//	 Return			: char* (��ȯ�� string)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
char* CConvReFormat::convUnIntToStr (unsigned int unValue, int size)
{
	char* 	strReturnValue;
	strReturnValue = new char[size+1];
	auto_ptr<char> apReturnValue(strReturnValue);
	memset (strReturnValue, NULL, size+1);

	sprintf (strReturnValue, "%u", unValue);
	return strReturnValue;
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : convLongToStr
//   Usage          : long�� string���� ��ȯ �ϴ� �Լ� 
//	 Argument		: long lValue (��ȯ�� ����)
//					  int size (string size)
//	 Return			: char* (��ȯ�� string)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
char* CConvReFormat::convLongToStr (long lValue, int size)
{
	char* 	strReturnValue;
	strReturnValue = new char[size+1];
	auto_ptr<char> apReturnValue(strReturnValue);
	memset (strReturnValue, NULL, size+1);

	sprintf (strReturnValue, "%l", lValue);
	return strReturnValue;
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : convUnLongToStr
//   Usage          : unsigned long�� string���� ��ȯ �ϴ� �Լ� 
//	 Argument		: long ulValue (��ȯ�� ����)
//					  int size (string size)
//	 Return			: char* (��ȯ�� string)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
char* CConvReFormat::convUnLongToStr (unsigned long ulValue, int size)
{
	char* 	strReturnValue;
	strReturnValue = new char[size+1];
	auto_ptr<char> apReturnValue(strReturnValue);
	memset (strReturnValue, NULL, size+1);

	sprintf (strReturnValue, "%ul", ulValue);
	cout << "unLong = 	" << strReturnValue << endl;
	return strReturnValue;
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : convStrToInt
//   Usage          : string�� int���� ��ȯ �ϴ� �Լ� 
//	 Argument		: char* strValue (��ȯ�� ����)
//	 Return			: int (��ȯ�� int)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
int  CConvReFormat::convStrToInt (char* strValue)
{
	int nReturnValue;
	nReturnValue = atoi(strValue);
	return nReturnValue;
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : convStrToUnInt
//   Usage          : string�� unsigned int���� ��ȯ �ϴ� �Լ� 
//	 Argument		: char* strValue (��ȯ�� ����)
//	 Return			: unsigned int (��ȯ�� unsigned int)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
unsigned int  CConvReFormat::convStrToUnInt (char* strValue)
{
	unsigned int unReturnValue;
	unReturnValue = (unsigned int)atoi(strValue);
	return unReturnValue;
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : convStrToLong
//   Usage          : string�� long���� ��ȯ �ϴ� �Լ� 
//	 Argument		: char* strValue (��ȯ�� ����)
//	 Return			: long (��ȯ�� long)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
long  CConvReFormat::convStrToLong (char* strValue)
{
	long lReturnValue;
	lReturnValue = atol(strValue);
	return lReturnValue;
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : convStrToUnLong
//   Usage          : string�� unsigned long���� ��ȯ �ϴ� �Լ� 
//	 Argument		: char* strValue (��ȯ�� ����)
//	 Return			: unsigned long (��ȯ�� unsigned long)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
unsigned long  CConvReFormat::convStrToUnLong (char* strValue)
{
	unsigned long ulReturnValue;
	ulReturnValue = (unsigned long)atol(strValue);
	return ulReturnValue;
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : convStrToShort
//   Usage          : string�� short���� ��ȯ �ϴ� �Լ� 
//	 Argument		: char* strValue (��ȯ�� ����)
//	 Return			: short (��ȯ�� short)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
short	CConvReFormat::convStrToShort (char* strValue)
{
	short sReturnValue;
	sReturnValue = (short)atoi(strValue);
	return sReturnValue;
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : convStrToUnShort
//   Usage          : string�� unsigned short���� ��ȯ �ϴ� �Լ� 
//	 Argument		: char* strValue (��ȯ�� ����)
//	 Return			: unsigned short (��ȯ�� unsigned short)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
unsigned short	CConvReFormat::convStrToUnShort(char* strValue)
{
	short usReturnValue;
	usReturnValue = (unsigned short)atoi(strValue);
	return usReturnValue;
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : spaceToNull
//   Usage          : space�� ä���� string�� null �ֱ�  
//	 Argument		: char* strValue (��ȯ�� ����)
//					  int size (��ȯ�� char*�� ���� / ��ȯ�� ���� ����)
//	 Return			: char* (null�� �� char*)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
char* CConvReFormat::spaceToNull (char* strValue, int size)
{
	char* 	strReturnValue;
	strReturnValue = new char[size+1];
	auto_ptr<char> apReturnValue(strReturnValue);
	memset (strReturnValue, NULL, size+1);

	memcpy (strReturnValue, strValue, size);
	strtok (strReturnValue, " ");
	return strReturnValue;
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : nullToSpace
//   Usage          : null�� ä���� string�� space �ֱ�  
//	 Argument		: char* strValue (��ȯ�� ����)
//					  int size (��ȯ�� char*�� ���� / ��ȯ�� ���� ����)
//	 Return			: char* (space�� �� char*)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Shin Mi Youn	(smy@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
char* CConvReFormat::nullToSpace (char* strValue, int size)
{
   	char* 	strReturnValue;
	strReturnValue = new char[size+1];
	auto_ptr<char> apReturnValue(strReturnValue);
 	memcpy (strReturnValue, strValue, size);
    for(int i=0;i<size;i++)
    {
        if(*(strReturnValue+i)==0x00)		*(strReturnValue+i)=0x20;
    }
	return strReturnValue;
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : ipToStr
//   Usage          : ip�� string���� 
//	 Argument		: unsigned int unIP
//					  int size (��ȯ�� char*�� ���� / ��ȯ�� ���� ����)
//	 Return			: char* (space�� �� char*)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
char* CConvReFormat::ipToStr (unsigned int unIP)
{
	unsigned char strTempValue[4];								// ip dot�� �������� �迭 
	char*	strReturnValue;
	strReturnValue = new char[IPLEN+1];
	auto_ptr<char> apReturnValue(strReturnValue);

	memset( strReturnValue, NULL, IPLEN+1 );
	memcpy(strTempValue, (char *)&unIP, 4);
	sprintf(strReturnValue, "%03d%03d%03d%03d", strTempValue[0], strTempValue[1],strTempValue[2],strTempValue[3]);

	return strReturnValue;
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : convPrefixChar
//   Usage          : string�� size��ŭ ���ʿ� Ư�� cPrefix�� ä��� �Լ�
//	 Argument		: char* strValue (��ȯ�� char*)
//					  char cPrefix (���ʿ� ä���� Ư������)
//					  int  size (��ȯ�� char*�� ����)
//	 Return			: char* (��ȯ�� char*)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
char*	CConvReFormat::convPrefixChar	(char* strValue, char cPrefix, int size)
{
	if (size <= 0)
	{
		cout << "convPrefixChar size error " << endl;
		exit(0);
//		return NULL;
	}

	char* 	strReturnValue;
	strReturnValue = new char[size+1];
	auto_ptr<char> apReturnValue(strReturnValue);
	memset (strReturnValue, NULL, size+1);

    string number(strValue);
    string padding(size - strlen(strValue), cPrefix);
    string result;
    result = padding + number ;
    memcpy(strReturnValue, result.c_str(), result.length());
	return strReturnValue;
}

//////////////////////////////////////////////////////////////////////
//
//   Function Name  : convPostfixChar
//   Usage          : string�� size��ŭ ���ʿ� Ư�� cPrefix�� ä��� �Լ�
//	 Argument		: char* strValue (��ȯ�� char*)
//					  char cPostfix (���ʿ� ä���� Ư������)
//					  int  size (��ȯ�� char*�� ����)
//	 Return			: char* (��ȯ�� char*)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
//////////////////////////////////////////////////////////////////////
char*	CConvReFormat::convPostfixChar	(char* strValue, char cPostfix, int size)
{
	if (size <= 0)
	{
		cout << "convPostfixChar size error " << endl;
		exit(0);
//		return NULL;
	}
	char* 	strReturnValue;
	strReturnValue = new char[size+1];
	auto_ptr<char> apReturnValue(strReturnValue);
	memset (strReturnValue, NULL, size+1);

    string number(strValue);
    string padding(size - strlen(strValue), cPostfix);
    string result;
    result = number + padding;
    memcpy(strReturnValue, result.c_str(), result.length());
	return strReturnValue;
}
/*
int main()
{
	char aa[21];
	int  aaa=123124;
	CConvReFormat 	asdf;
//	test inttostr
	strcpy(aa, asdf.convIntToStr (aaa,sizeof(aa)));
	cout << "size = " << sizeof(aa) << endl;
	cout << "result = " << aa << endl;

//	test convPrefixChar
	strcpy(aa, asdf.convPrefixChar	(aa, ' ', sizeof(aa)));
	cout << "size = " << sizeof(aa) << endl;
	cout << "result = " << aa << endl;
	
//  test chartostr
    memset(aa, NULL, sizeof(aa));
    memcpy (aa, asdf.convCharToStr('a',15),15);
	cout << "size = " << sizeof(aa) << endl;
	cout << "result = " << aa << endl;

//  test convUnIntToStr
    memset(aa, NULL, sizeof(aa));
    memcpy (aa, asdf.convUnIntToStr(1231451411,10),10);
	cout << "size = " << sizeof(aa) << endl;
	cout << "result = " << aa << endl;

//	test convPostfixChar
	strcpy(aa, asdf.convPostfixChar	(aa, ' ', sizeof(aa)));
	cout << "size = " << sizeof(aa) << endl;
	cout << "result = " << aa << endl;

//	test convPostfixChar
	strcpy(aa, asdf.spaceToNull (aa, sizeof(aa)));
	cout << "size = " << sizeof(aa) << endl;
	cout << "result = " << aa << endl;
	
//  test convStrToUnInt
	unsigned int bb;
    bb  = asdf.convStrToUnInt("4112314514");
	cout << "size = " << sizeof(bb) << endl;
	cout << "result = " << bb << endl;

//  test convStrToUnInt
	unsigned short cc;
    cc  = asdf.convStrToUnShort("32761");
	cout << "size = " << sizeof(cc) << endl;
	cout << "result = " << cc << endl;

// 	test ipToStr
    memset(aa, NULL, sizeof(aa));
    memcpy (aa, asdf.ipToStr(4112314514), sizeof(aa));
	cout << "size = " << sizeof(aa) << endl;
	cout << "result = " << aa << endl;
}		

/* test ������ 
size = 21
result = 123124
size = 21
result =                123124
size = 21
result = a
size = 21
result = 12314514
size = 21
result = 123145140000000000000
size = 4
result = 4112314514
size = 2
result = 32761
size = 21
result = 245.28.240.146*/