#include "../INCLUDE/pdsnRule.hpp"

CPdsnRule::CPdsnRule() : CConvReFormat()
{
}

CPdsnRule::~CPdsnRule()
{
}

////////////////////////////////////////////////////////////////////////
//
//   Function Name  : pdsnOneField
//   Usage          : 하나의 field를 psdn 형태로(rfc 2138, 2139)변환하는 
//					  함수 
//	 Argument		: char* indata 		(변환할 전체 data)
//					  int 	nOffset 	(data에서 읽어들일 offset)
//					  int 	nInOneLen 	(data에서 읽어들일 size)
//					  int	nPdsnCode	(변환될 pdsn code)
//					  int 	nVendorcode	(pdsn code값이 26번일 경우 code)
//	 Return			: char* (변환된 one fleid data)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
////////////////////////////////////////////////////////////////////////
char*	CPdsnRule::pdsnOneField(char* indata, int nOffset, int nInOneLen, int nPdsnCode, int nVendorCode)
{
	int 	nVendorId 	= 0;									//  Vendor Id = 0 fix
//	int		nOutOneLen = 0;										//	output one data length
	char*	strReturnValue	;
//	char*	strVendorData;
	
	strInOneData 	= 	new	char[MAX_ONE_IN_DATA_LEN];			//	header가 없는 부분 
//	strVendorData	= 	new char[MAX_ONE_IN_DATA_LEN];			//	vendor data
	
	strReturnValue 	= 	new char[MAX_ONE_IN_DATA_LEN];			//	header가 포함한 부분 
	auto_ptr<char> apReturnValue(strReturnValue);

	memset (strReturnValue, NULL, 			MAX_ONE_IN_DATA_LEN);

	memset (strInOneData, NULL,				MAX_ONE_IN_DATA_LEN);
	memcpy (strInOneData, &indata[nOffset],	nInOneLen);			//	indata중 처리할 data 추출


    unsigned int 	unTemp;		//	31
    int 			nTemp;		//	40
    
    switch(nPdsnCode)    
    {
		case USER_NAME:			// 1 (B2)
			//	header 부분 정의 
			memcpy(strInOneData, spaceToNull(strInOneData, strlen(strInOneData)), strlen(strInOneData));
			nOneFieldLen 	= ONE_TYPE_LEN + ONE_LENGTH_LEN + strlen(strInOneData);
			ucType 			= USER_NAME;
			ucLen 			= nOneFieldLen;
			memcpy(strReturnValue, 					&ucType, 	ONE_TYPE_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN], 	&ucLen, 	ONE_LENGTH_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN+ONE_LENGTH_LEN], strInOneData, strlen(strInOneData));
			
			break;
		case NAS_IP_ADDRESS:	// 4 (D2)
			nOneFieldLen 	= ONE_TYPE_LEN + ONE_LENGTH_LEN + sizeof(int);
			ucType 			= NAS_IP_ADDRESS;
			ucLen 			= nOneFieldLen;
			memcpy(strReturnValue, 					&ucType, 	ONE_TYPE_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN], 	&ucLen, 	ONE_LENGTH_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN+ONE_LENGTH_LEN], strInOneData, sizeof(int));
			break;	

		case NAS_PORT:			// 5
			nOneFieldLen 	= ONE_TYPE_LEN + ONE_LENGTH_LEN + sizeof(int);
			ucType 			= NAS_PORT;
			ucLen 			= nOneFieldLen;
			memcpy(strReturnValue, 					&ucType, 	ONE_TYPE_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN], 	&ucLen, 	ONE_LENGTH_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN+ONE_LENGTH_LEN], strInOneData, sizeof(int));
			break;	

		case FRAMED_PROTOCOL:	// 7
			nOneFieldLen 	= ONE_TYPE_LEN + ONE_LENGTH_LEN + sizeof(int);
			ucType 		= FRAMED_PROTOCOL;
			ucLen 		= nOneFieldLen;
			memcpy(strReturnValue, 					&ucType, 	ONE_TYPE_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN], 	&ucLen, 	ONE_LENGTH_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN+ONE_LENGTH_LEN], strInOneData, sizeof(int));
			break;	

		case FRAMED_IP_ADDRESS:	// 8
			nOneFieldLen 	= ONE_TYPE_LEN + ONE_LENGTH_LEN + sizeof(int);
			ucType 			= FRAMED_IP_ADDRESS;
			ucLen 			= nOneFieldLen;
			memcpy(strReturnValue, 					&ucType, 	ONE_TYPE_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN], 	&ucLen, 	ONE_LENGTH_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN+ONE_LENGTH_LEN], strInOneData, sizeof(int));
			break;	

		case VENDOR_SPECIFIC:	// 26
			memcpy (strInOneData,	vendorSpecRule(indata, nOffset, nInOneLen, nVendorCode), nVendorLen);
			nOneFieldLen 	= ONE_TYPE_LEN + ONE_LENGTH_LEN + sizeof(int) + nVendorLen;
			ucType 			= VENDOR_SPECIFIC;
			ucLen 			= nOneFieldLen;
			memcpy(strReturnValue, 					&ucType, 	ONE_TYPE_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN], 	&ucLen, 	ONE_LENGTH_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN+ONE_LENGTH_LEN], &nVendorId, sizeof(int));
			memcpy(&strReturnValue[ONE_TYPE_LEN+ONE_LENGTH_LEN+sizeof(int)], strInOneData, nVendorLen);
			break;	

		case CALLING_STATION_ID:// 31
			memcpy(&unTemp, strInOneData, sizeof(int));
			strcpy(strInOneData, convUnIntToStr(unTemp, 10));
			memcpy(strInOneData, spaceToNull(strInOneData, strlen(strInOneData)), strlen(strInOneData));
			nOneFieldLen 	= ONE_TYPE_LEN + ONE_LENGTH_LEN + strlen(strInOneData);
			ucType 			= CALLING_STATION_ID;
			ucLen 			= nOneFieldLen;
			memcpy(strReturnValue, 					&ucType, 	ONE_TYPE_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN], 	&ucLen, 	ONE_LENGTH_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN+ONE_LENGTH_LEN], strInOneData, strlen(strInOneData)-4);
			//	drop test를 위해서 수정한 곳 ( 수정 방법 )
			//  정상 루틴을 다 돌고 이상한 값으로 대체를 한다.  	// 2001. 5. 29 ( jsj )
//			memset(strReturnValue , 0x00 , strlen(strInOneData));
//			sprintf(strReturnValue , "12345");
			break;	

		case ACCT_STATUS_TYPE:	// 40
			nOneFieldLen 	= ONE_TYPE_LEN + ONE_LENGTH_LEN + sizeof(int);
			ucType 			= ACCT_STATUS_TYPE;
			ucLen 			= nOneFieldLen;
			memcpy(strReturnValue, 					&ucType, 	ONE_TYPE_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN], 	&ucLen, 	ONE_LENGTH_LEN);
			
			memcpy(&nTemp, strInOneData, sizeof(int));

			if (nTemp == 0)				//  start		udr = 0,	pdsn = 1
			{
				nTemp = 1;
			}
			else if (nTemp == 1)		//	interim		udr = 1, 	pdsn = 3
			{
				nTemp = 3;
			}
			else if (nTemp == 2)		// 	stop		udr = 2, 	pdsn = 2
			{
				nTemp = 2;
			}
			else 
			{
				cout << " data Error pdsn code = 40 " << endl;
				break;
			}
			memcpy(&strReturnValue[ONE_TYPE_LEN+ONE_LENGTH_LEN], &nTemp , sizeof(int));
			break;	

		case ACCT_INPUT_OCTETS:	// 42
			nOneFieldLen 	= ONE_TYPE_LEN + ONE_LENGTH_LEN + sizeof(int);
			ucType 			= ACCT_INPUT_OCTETS;
			ucLen 			= nOneFieldLen;
			memcpy(strReturnValue, 					&ucType, 	ONE_TYPE_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN], 	&ucLen, 	ONE_LENGTH_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN+ONE_LENGTH_LEN], strInOneData, sizeof(int));
			break;	

		case ACCT_OUTPUT_OCTETS:	// 43
			nOneFieldLen 	= ONE_TYPE_LEN + ONE_LENGTH_LEN + sizeof(int);
			ucType 			= ACCT_OUTPUT_OCTETS;
			ucLen 			= nOneFieldLen;
			memcpy(strReturnValue, 					&ucType, 	ONE_TYPE_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN], 	&ucLen, 	ONE_LENGTH_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN+ONE_LENGTH_LEN], strInOneData, sizeof(int));
			break;	

		case ACCT_SESSION_ID:	// 44
			memcpy(strInOneData, spaceToNull(strInOneData, strlen(strInOneData)), strlen(strInOneData));
			nOneFieldLen 	= ONE_TYPE_LEN + ONE_LENGTH_LEN + strlen(strInOneData);
			ucType 			= ACCT_SESSION_ID;
			ucLen 			= nOneFieldLen;
			memcpy(strReturnValue, 					&ucType, 	ONE_TYPE_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN], 	&ucLen, 	ONE_LENGTH_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN+ONE_LENGTH_LEN], strInOneData, strlen(strInOneData));
			break;	

		case ACCT_SESSION_TIME:	// 46
			nOneFieldLen 	= ONE_TYPE_LEN + ONE_LENGTH_LEN + sizeof(int);
			ucType 			= ACCT_SESSION_TIME;
			ucLen 			= nOneFieldLen;
			memcpy(strReturnValue, 					&ucType, 	ONE_TYPE_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN], 	&ucLen, 	ONE_LENGTH_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN+ONE_LENGTH_LEN], strInOneData, sizeof(int));
			break;	

		case ACCT_INPUT_PACKETS:	// 47
			nOneFieldLen 	= ONE_TYPE_LEN + ONE_LENGTH_LEN + sizeof(int);
			ucType 			= ACCT_INPUT_PACKETS;
			ucLen 			= nOneFieldLen;
			memcpy(strReturnValue, 					&ucType, 	ONE_TYPE_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN], 	&ucLen, 	ONE_LENGTH_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN+ONE_LENGTH_LEN], strInOneData, sizeof(int));
			break;	

		case ACCT_OUTPUT_PACKETS:	// 48
			nOneFieldLen 	= ONE_TYPE_LEN + ONE_LENGTH_LEN + sizeof(int);
			ucType 			= ACCT_OUTPUT_PACKETS;
			ucLen 			= nOneFieldLen;
			memcpy(strReturnValue, 					&ucType, 	ONE_TYPE_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN], 	&ucLen, 	ONE_LENGTH_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN+ONE_LENGTH_LEN], strInOneData, sizeof(int));
			break;	

		case ACCT_MULTI_SESSION_ID:	// 50
			memcpy(strInOneData, spaceToNull(strInOneData, strlen(strInOneData)), strlen(strInOneData));
			nOneFieldLen 	= ONE_TYPE_LEN + ONE_LENGTH_LEN + strlen(strInOneData);
			ucType 			= ACCT_MULTI_SESSION_ID;
			ucLen 			= nOneFieldLen;
			memcpy(strReturnValue, 					&ucType, 	ONE_TYPE_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN], 	&ucLen, 	ONE_LENGTH_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN+ONE_LENGTH_LEN], strInOneData, strlen(strInOneData));
			break;	

		case ACCT_EVENT_TIME:	// 55
			nOneFieldLen 	= ONE_TYPE_LEN + ONE_LENGTH_LEN + sizeof(int);
			ucType 			= ACCT_EVENT_TIME;
			ucLen 			= nOneFieldLen;
			memcpy(strReturnValue, 					&ucType, 	ONE_TYPE_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN], 	&ucLen, 	ONE_LENGTH_LEN);
			memcpy(&strReturnValue[ONE_TYPE_LEN+ONE_LENGTH_LEN], strInOneData, sizeof(int));
			break;	
			
		case	0 :				// data 무시
			break;

    	default  :
			cout <<"!!!Error : Undefined Pdsn Code." << endl;
			delete[] strInOneData;
			return FAIL;
    }
	delete[] strInOneData;
    return strReturnValue;
}

////////////////////////////////////////////////////////////////////////
//
//   Function Name  : vendorSpecRule
//   Usage          : pdsn code 26에 해당하는 특수 형태를 처리하는 함수 
//	 Argument		: char* inOneData 		(변환할 전체 data)
//					  int 	nVendorcode	(pdsn code값이 26번일 경우 code)
//	 Return			: char* (변환된 one fleid data)
//   DATE           : 2001. 3 .
//   LAST UPDATE    :
//   AUTHOR         : Jung Seung jin	(sjjung@neomnt.com)
//   History        :
//
////////////////////////////////////////////////////////////////////////
char*	CPdsnRule::vendorSpecRule(char* indata, int nOffset, int nInOneLen, int nVendorCode)
{
	char*			strVendorReturn;		//	ucVendorType + ucVendorLen + InOneData
	char*			strVenOnedata;			//	header가 없는 부분
	unsigned char	ucVendorType;
	unsigned char 	ucVendorLen;			//	ucVendorType size + ucVendorLen size + nVendorId size
	int				nVendorId;
	
	strVenOnedata 		= 	new	char[MAX_ONE_IN_DATA_LEN];
	strVendorReturn		= 	new	char[MAX_ONE_IN_DATA_LEN];
	auto_ptr<char> apReturnValue(strVendorReturn);
	
	memset (strVendorReturn,	NULL, 	MAX_ONE_IN_DATA_LEN);
	memset (strVenOnedata, 		NULL,	MAX_ONE_IN_DATA_LEN);

	memcpy (strVenOnedata, &indata[nOffset],	nInOneLen);			//	indata중 처리할 data 추출

	switch (nVendorCode)
	{
		case 	7 :				// unsigned int, int 
		case	9 :
		case	11:
		case	12:
		case	13:
		case	14:
		case	15:
		case	16:
		case	17:
		case	18:
		case	19:
		case	20:
		case	21:
		case	22:
		case	23:
		case	24:
		case	25:
		case	30:
		case	31:
		case	32:
		case	33:
		case	34:
		case	35:
		case	36:
		case	37:
		case	38:
		case	39:
		case	43:
			nVendorLen	 	= ONE_TYPE_LEN + ONE_LENGTH_LEN + sizeof(int);
			ucVendorType 	= nVendorCode;
			ucVendorLen 	= nVendorLen;
			memcpy(strVendorReturn, 				&ucVendorType, 	ONE_TYPE_LEN);
			memcpy(&strVendorReturn[ONE_TYPE_LEN], 	&ucVendorLen, 	ONE_LENGTH_LEN);
			memcpy(&strVendorReturn[ONE_TYPE_LEN+ONE_LENGTH_LEN], strVenOnedata, sizeof(int));
			break;
			
		case 	10:				//	string
		case   	44:
			memcpy(strVenOnedata, spaceToNull(strVenOnedata, strlen(strVenOnedata)), strlen(strVenOnedata));
			nVendorLen	 	= ONE_TYPE_LEN + ONE_LENGTH_LEN + strlen(strVenOnedata);
			ucVendorType 	= nVendorCode;
			ucVendorLen 	= nVendorLen;
			memcpy(strVendorReturn, 				&ucVendorType, 	ONE_TYPE_LEN);
			memcpy(&strVendorReturn[ONE_TYPE_LEN], 	&ucVendorLen, 	ONE_LENGTH_LEN);
			memcpy(&strVendorReturn[ONE_TYPE_LEN+ONE_LENGTH_LEN], strVenOnedata, strlen(strVenOnedata));
			break;
    	default  :
			cout <<"!!!Error : Undefined Vendor Code." << endl;
			delete[] strVenOnedata;
			return FAIL;
	}
	delete[] strVenOnedata;
	return	strVendorReturn;
}		
	
/*
int main()
{
	char aa[1024];
	char bb[1024];
	unsigned int  nint = 123456;
	unsigned char uc;
	memset (aa, NULL, 1024);
	memset (bb, NULL, 1024);
	sprintf (aa, "%s", "9346898");
	memcpy (&aa[7],&nint,4);
	CPdsnRule 	asdf;
//	test one field
	memcpy(bb, asdf.pdsnOneField(aa, 0, 7, 26, 10), asdf.get_OneFieldLen());
	int i;
	for ( i = 0 ; i < asdf.get_OneFieldLen() ; i++)
	{
		printf("bb[%d]=%x\t", i, bb[i]);
	}
	printf("\n");
	cout << "OneFieldLen() = " << asdf.get_OneFieldLen() << endl;
	printf("\n");

	memcpy(bb, asdf.pdsnOneField(aa, 7, 4, 40, 0), asdf.get_OneFieldLen());
	unsigned int nresult;
	memcpy(&nresult,&bb[2], 4);
	for ( i = 0 ; i < asdf.get_OneFieldLen() ; i++)
	{
		printf("bb[%d]=%x\t", i, bb[i]);
	}
	printf("\n");
	cout << "nresult = " << nresult << endl;
	cout << "OneFieldLen() = " << asdf.get_OneFieldLen() << endl;

	memcpy(bb, asdf.pdsnOneField(aa, 7, 4, 26, 7), asdf.get_OneFieldLen());
	int vendorid;
	memcpy(&vendorid,&bb[2], 4);
	cout << "vendorid = " << vendorid << endl;	
	memcpy(&nresult,&bb[8], 4);
	for ( i = 0 ; i < asdf.get_OneFieldLen() ; i++)
	{
		printf("bb[%d]=%x\t", i, bb[i]);
	}
	printf("\n");
	cout << "nresult = " << nresult << endl;
	cout << "OneFieldLen() = " << asdf.get_OneFieldLen() << endl;

}
*/
/*
실행결과 
VendorLen = 9
bb[0]=1a        bb[1]=f bb[2]=0 bb[3]=0 bb[4]=0 bb[5]=a bb[6]=a bb[7]=9 bb[8]=39        bb[9]=33        bb[10]=34       bb[11]=36bb[12]=38       bb[13]=39       bb[14]=38
OneFieldLen() = 15

 data Error pdsn code = 40 
bb[0]=28        bb[1]=6 bb[2]=0 bb[3]=0 bb[4]=0 bb[5]=0
nresult = 0
OneFieldLen() = 6
VendorLen = 6
vendorid = 7
bb[0]=1a        bb[1]=c bb[2]=0 bb[3]=0 bb[4]=0 bb[5]=7 bb[6]=7 bb[7]=6 bb[8]=0 bb[9]=1 bb[10]=ffffffe2 bb[11]=40
nresult = 123456
OneFieldLen() = 12
*/
