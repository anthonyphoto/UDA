#include <UDR.hpp>

void CBDS_UdrFormat::conv_BDS_UDR	(UdrFormat& UdrForm)
{
	conv_A_TYPE	(UdrForm.get_ItemA());
	conv_B_TYPE	(UdrForm.get_ItemB());
	conv_C_TYPE	(UdrForm.get_ItemC());
	conv_D_TYPE	(UdrForm.get_ItemD());
	conv_E_TYPE	(UdrForm.get_ItemE());
	conv_F_TYPE	(UdrForm.get_ItemF());
	conv_G_TYPE	(UdrForm.get_ItemG());
	conv_H_TYPE	(UdrForm.get_ItemH());
	conv_I_TYPE	(UdrForm.get_ItemI());
	conv_Y_TYPE	(UdrForm.get_ItemY());
	conv_Z_TYPE	(UdrForm.get_ItemZ());
}

void CBDS_UdrFormat::conv_A_TYPE	(A_INT_TYPE* stPtrItemA)
{
	conv_IntMin2StrMin	(stPtrItemA->get_unMsId());
}

void CBDS_UdrFormat::conv_B_TYPE	(B_TYPE* stPtrItemB)
{
	conv_UdrData(stPtrItemB->get_nIpAddr(), 					"B1");
	conv_UdrData(stPtrItemB->get_strNAI(), 						"B2");
}

void CBDS_UdrFormat::conv_C_TYPE	(C_TYPE* stPtrItemC)
{
	conv_UdrData(stPtrItemC->get_strAccountSessionID(), 		"C1");
	conv_UdrData(stPtrItemC->get_strCorrelationID(), 			"C2");
}

void CBDS_UdrFormat::conv_D_TYPE	(D_TYPE* stPtrItemD)
{
	conv_UdrData(stPtrItemD->get_nMIP_HomeAgent(),				"D1");
	conv_UdrData(stPtrItemD->get_nPDSN_FA_Addr(), 				"D2");
	conv_UdrData(stPtrItemD->get_nServing_PCF(), 				"D3");
	conv_UdrData(stPtrItemD->get_nServing_PCF(), 				"D4");
}

void CBDS_UdrFormat::conv_E_TYPE	(E_TYPE* stPtrItemE)
{
	conv_UdrData(stPtrItemE->get_nUserZone(),					"E1");
}

void CBDS_UdrFormat::conv_F_TYPE	(F_TYPE* stPtrItemF)
{
	conv_UdrData(stPtrItemF->get_nForwardMUXOpt(),				"F1");
	conv_UdrData(stPtrItemF->get_nReverseMUXOpt(),				"F2");
	conv_UdrData(stPtrItemF->get_nForward_Fund_Rate(),			"F3");
	conv_UdrData(stPtrItemF->get_nReverse_Fund_Rate(),			"F4");
	conv_UdrData(stPtrItemF->get_nServiceOpt(),					"F5");
	conv_UdrData(stPtrItemF->get_nForwardTrafficType(),			"F6");
	conv_UdrData(stPtrItemF->get_nReverseTrafficType(),			"F7");
	conv_UdrData(stPtrItemF->get_nFundFrameSize(),				"F8");
	conv_UdrData(stPtrItemF->get_nForwardFundamentalRC(),		"F9");
	conv_UdrData(stPtrItemF->get_nReverseFundamentalRC(),		"F10");
	conv_UdrData(stPtrItemF->get_nIP_Technology(),				"F11");
	conv_UdrData(stPtrItemF->get_nCompulsoryTunnelInd(),		"F12");
	conv_UdrData(stPtrItemF->get_nReleaseIndicator(),			"F13");
}

void CBDS_UdrFormat::conv_G_TYPE	(G_TYPE* stPtrItemG)
{
	conv_UdrData(stPtrItemG->get_nDataOctetCntTerminat(),		"G1");
	conv_UdrData(stPtrItemG->get_nDataOctetCntOrigin(),			"G2");
	conv_UdrData(stPtrItemG->get_nBadPPPframeCnt(),				"G3");
	conv_UdrData(stPtrItemG->get_nStartEventTime(),				"G4");
	conv_UdrData(stPtrItemG->get_nEndEventTime(),				"G5");
	conv_UdrData(stPtrItemG->get_nActiveTime(),					"G8");
	conv_UdrData(stPtrItemG->get_nNumOfActiveTransitions(),		"G9");
	conv_UdrData(stPtrItemG->get_nSDB_OctetCountTerminat(),		"G10");
	conv_UdrData(stPtrItemG->get_nSDB_OctetCountOrigin(),		"G11");
	conv_UdrData(stPtrItemG->get_nNumOfSDBsTerminat(),			"G12");
	conv_UdrData(stPtrItemG->get_nNumOfSDBsOrigin(),			"G13");
	conv_UdrData(stPtrItemG->get_nNumOfPPPbytesReceived(),		"G14");
}

void CBDS_UdrFormat::conv_H_TYPE	(H_TYPE* stPtrItemH)
{
	conv_UdrData(stPtrItemH->get_nAlternateBillingId(),			"H1");
}

void CBDS_UdrFormat::conv_I_TYPE	(I_TYPE* stPtrItemI)
{
	conv_UdrData(stPtrItemI->get_nIP_QoS(), 							"I1");
	conv_UdrData(stPtrItemI->get_nInterconnectIP_NetworkProviderID(),	"I2");
	conv_UdrData(stPtrItemI->get_nInterconnectIP_NetworkServiceQoS(),	"I3");
	conv_UdrData(stPtrItemI->get_nAirlinkQoS(),							"I4");
}

void CBDS_UdrFormat::conv_Y_TYPE	(Y_TYPE* stPtrItemY)
{
	conv_UdrData(stPtrItemY->get_nAirlinkRecType(), 			"Y1");
	conv_UdrData(stPtrItemY->get_nRP_SessionID(), 				"Y2");
	conv_UdrData(stPtrItemY->get_nAirlinkSeqNum(), 				"Y3");
	conv_UdrData(stPtrItemY->get_nMobileOrigin_TerminatID(),	"Y4");
}

void CBDS_UdrFormat::conv_Z_TYPE	(Z_TYPE* stPtrItemZ)
{
	conv_UdrData(stPtrItemZ->get_strContainer(),				"Z1");
}

//	INT형의 MIN을 String로 변환

void CBDS_UdrFormat::conv_IntMin2StrMin	(unsigned int	unMIN)
{
	char	strMin[11];
	int		nIndex = 0;
	
	memset( strTempBdsUdr,	0x20, 		sizeof(strTempBdsUdr)  );
	
	sprintf(strMin, "%010d", unMIN);
	memcpy(strTempBdsUdr, strMin, strlen(strMin));
	
	nIndex = getOffsetNlength("A1");
	
	memcpy(&strBdsUdr[stPtrUdrItem[nIndex].nStringOffset], strTempBdsUdr,\
		stPtrUdrItem[nIndex].nStringLen);
}

//	UDR Item정보를 입력으로 변환 포맷 정보를 획득
int	CBDS_UdrFormat::getOffsetNlength(char* strItemType)
{
	for(int i = 0; i<get_nUdrCount(); i++)
	{
		if (!memcmp((char*)&stPtrUdrItem[i].strItem, strItemType, strlen(strItemType)))
			return i;
	}
	cout << "nUdrCount in for end = " << get_nUdrCount() << endl;
	cout << "Not Found !!" << endl;
	return -1;
}

//	int ==> String
//	int 값을 문자열로 변환
void CBDS_UdrFormat::conv_UdrData(int strUdrData, char* strUdrType)
{
	int		nIndex = 0;
				
	nIndex = getOffsetNlength(strUdrType);
	
	if(	conv_Int2strValue(strUdrData, stPtrUdrItem[nIndex].nStringLen) < 0 )
	{
		cout << strUdrData << " " << stPtrUdrItem[nIndex].nStringLen << \
		" " << strUdrType << endl;
		cout << "conv_Int2strValue FAIL !!" << endl;
	}
	
	memcpy(&strBdsUdr[stPtrUdrItem[nIndex].nStringOffset], strTempBdsUdr,\
		stPtrUdrItem[nIndex].nStringLen);
}

//	String ==> String
//	문자열을 정해진 길이로 변환
void CBDS_UdrFormat::conv_UdrData(char*	strUdrData, char* strUdrType)
{
	int		nIndex = 0;
				
	memset( strTempBdsUdr,	0x20, 		sizeof(strTempBdsUdr)  );
	
	nIndex = getOffsetNlength(strUdrType);
	
	cout << "\n++++++++++conv_UdrData+++++++++++++++++" << endl;
	cout << "strUdrData = [" << strUdrData << "]" << endl;
	cout << "strlen(strUdrData) = [" << strlen(strUdrData) << "]" << endl;
	
	memcpy( strTempBdsUdr, 	strUdrData,	strlen(strUdrData) );
	
	memcpy(&strBdsUdr[stPtrUdrItem[nIndex].nStringOffset], strTempBdsUdr,\
		stPtrUdrItem[nIndex].nStringLen);
}

//	IP Addr ==> String
//	IP Address (int) 값을 문자열로 변환
void CBDS_UdrFormat::conv_UdrData(unsigned int	nIpAddr, char* strUdrType)
{
	char			strClass[4];
	unsigned int	nFilter = 0xFF000000;
	unsigned int	nClass	= 0;
	int				nIndex	= 0;
	
	memset( strTempBdsUdr,	NULL, 		sizeof(strTempBdsUdr)  );

	for(int i = 0, j = 3; i < 4; i++, --j)
	{
		nClass = nIpAddr & nFilter;
		nClass 	>>= (j * 8);
		nFilter >>= 8;	
		sprintf(strClass, "%03d", nClass);
		strcat(strTempBdsUdr, strClass);
	}
	
	nIndex = getOffsetNlength(strUdrType);
	
	memcpy(&strBdsUdr[stPtrUdrItem[nIndex].nStringOffset], strTempBdsUdr,\
		stPtrUdrItem[nIndex].nStringLen);
}


//	Radius에서 입력된 값이 BDS의 요청 데이타 길이와 상이할 경우
//	자리수 만큼 'F'채운다.

int	CBDS_UdrFormat::conv_Int2strValue(int nValue, int nLength)
{
	memset( strTempBdsUdr,	0x20, 		sizeof(strTempBdsUdr)  );
	strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
	
	switch(nLength)
	{
		case	1:
			if( (nValue / 10) >= 1 )
			{
				strcpy(strTempBdsUdr, "F");
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return -1;
			}
			else
			{
				sprintf(strTempBdsUdr, "%1d", nValue);
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return 0;
			}
		case	2:
			if( (nValue / 100) >= 1 )
			{
				strcpy(strTempBdsUdr, "FF");
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				cout << nValue << " " << nLength << endl;
				return -1;
			}
			else
			{
				sprintf(strTempBdsUdr, "%2d", nValue);
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return 0;
			}
		case	3:
			if( (nValue / 1000) >= 1 )
			{
				strcpy(strTempBdsUdr, "FFF");
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return -1;
			}
			else
			{
				sprintf(strTempBdsUdr, "%3d", nValue);
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return 0;
			}
		case	10:
			if( (nValue / 10000000000) >= 1 )
			{
				strcpy(strTempBdsUdr, "FFFFFFFFFF");
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return -1;
			}
			else
			{
				sprintf(strTempBdsUdr, "%10d", nValue);
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return 0;
			}
	}
}

