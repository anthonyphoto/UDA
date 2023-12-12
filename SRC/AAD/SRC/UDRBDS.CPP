#include <fstream.h>
#include "../INCLUDE/UdrBds.hpp"

//extern char	strFileNm[200];		// Text형식의 BDS File Name
extern int  nFlag;	// 파일의 처음에만 BDS Udr의 항목에 대한 설명을 적음

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
//	conv_Y_TYPE	(UdrForm.get_ItemY());
//	conv_Z_TYPE	(UdrForm.get_ItemZ());
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
	conv_UdrData(stPtrItemD->get_strBS_MSC_ID(), 				"D4");
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
	conv_UdrData(stPtrItemF->get_nServiceCorrelation(),			"F51");
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
	conv_UdrData((time_t)stPtrItemG->get_nStartEventTime(),		"G4");
	conv_UdrData((time_t)stPtrItemG->get_nEndEventTime(),		"G5");
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

	memcpy(&strBdsUdr[stPtrUdrItem[nIndex].nStringOffset], strTempBdsUdr,	\
		stPtrUdrItem[nIndex].nStringLen);
}

//	UDR Item정보를 입력으로 변환 포맷 정보를 획득
int	CBDS_UdrFormat::getOffsetNlength(const char* strItemType)
{
	for(int i = 0; i<get_nUdrCount(); i++)
	{
		if (!memcmp((char*)&stPtrUdrItem[i].strItem, strItemType, strlen(strItemType)))
			return i;
	}
/*
	cout << "nUdrCount in for end = " << get_nUdrCount() << endl;
	cout << "Not Found !! Type [" << strItemType << "]" <<endl;
	cout << "Udr Item is [" << stPtrUdrItem[i].strItem << "]" << endl;
	cout << "strItemType is [" << strItemType << "]" << endl;
	cout << "strlen(strItemType) is [" << strlen(strItemType) << "]" << endl;
*/
	return -1;
}

//	int ==> String
//	int 값을 문자열로 변환
void CBDS_UdrFormat::conv_UdrData(int strUdrData, const char* strUdrType)
{
	int		nIndex = 0;
	int		nStringLength = 0;

	nIndex = getOffsetNlength(strUdrType);

	nStringLength = stPtrUdrItem[nIndex].nStringLen;

	if (!memcmp(strUdrType, "G8", 2))	//	if "G8" 000000000 + 0(1/10Secs)
		nStringLength -= 1;

	if(	conv_Int2strValue(strUdrData, nStringLength) < 0 )
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
void CBDS_UdrFormat::conv_UdrData(char*	strUdrData, const char* strUdrType)
{
	int		nIndex 	= 0;
	int 	nStrLen;

	memset( strTempBdsUdr,	0x20, 		sizeof(strTempBdsUdr)  );
	strTempBdsUdr[((sizeof(strTempBdsUdr))-1)] = NULL;

	nIndex = getOffsetNlength(strUdrType);			// Get Config Data Index

	switch(stPtrUdrItem[nIndex].nAdjustment)
	{
		case	RIGHT:
			nStrLen	= strlen(strUdrData);
			if (nStrLen > stPtrUdrItem[nIndex].nLength)		// String Length Check
			{
				cout << "Data[" << strUdrData << "]" << "Length is Overflow Type is ["	\
					 << strUdrType << "]" << endl;
				memset(strUdrData, 'F', stPtrUdrItem[nIndex].nLength);
				memcpy(strTempBdsUdr, strUdrData, stPtrUdrItem[nIndex].nLength);
			}
			else
				memcpy(&strTempBdsUdr[stPtrUdrItem[nIndex].nLength - nStrLen],		\
						strUdrData, nStrLen);
			break;
		case	LEFT:
		case	NONE:
			memcpy( strTempBdsUdr, 	strUdrData,	stPtrUdrItem[nIndex].nLength );
			break;
		default:
			cout << "Adjustment Type[" << stPtrUdrItem[nIndex].nAdjustment << "] is Fail" << endl;
	}
//	strTempBdsUdr[stPtrUdrItem[nIndex].nLength] = NULL;
	memcpy(&strBdsUdr[stPtrUdrItem[nIndex].nStringOffset], strTempBdsUdr,		\
			stPtrUdrItem[nIndex].nStringLen);

/*
	cout << "\n++++++++++conv_UdrData+++++++++++++++++" << endl;
	cout << "strUdrData = [" << strUdrData << "]" << endl;
	cout << "strlen(strUdrData) = [" << strlen(strUdrData) << "]" << endl;
*/

}

//	IP Addr ==> String
//	IP Address (int) 값을 문자열로 변환
void CBDS_UdrFormat::conv_UdrData(unsigned int	nIpAddr, const char* strUdrType)
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

//	Timestamp ==> String DateTime
//	Timestamp (long(time_t)) 값을 문자열로 변환
void CBDS_UdrFormat::conv_UdrData(time_t tTimeStamp, const char* strUdrType)
{
	int				nIndex	= 0;
	struct tm   	*s;

	memset( strTempBdsUdr,	NULL, 		sizeof(strTempBdsUdr)  );

	s = (struct tm *) localtime( &tTimeStamp );

	sprintf(strTempBdsUdr,"%04d%02d%02d%02d%02d%02d0", 	s->tm_year+1900,
														s->tm_mon+1,
														s->tm_mday,
        	        									s->tm_hour,
        	        									s->tm_min,
        	        									s->tm_sec	);

	nIndex = getOffsetNlength(strUdrType);

	memcpy(&strBdsUdr[stPtrUdrItem[nIndex].nStringOffset], strTempBdsUdr,\
		stPtrUdrItem[nIndex].nStringLen);
}


//	Radius에서 입력된 값이 BDS의 요청 데이타 길이와 상이할 경우
//	자리수 만큼 'F'채운다.

int	CBDS_UdrFormat::conv_Int2strValue(int nValue, int nLength)
{
	memset( strTempBdsUdr,	0x20, 		sizeof(strTempBdsUdr)  );

	switch(nLength)
	{
		case	1:
			if( ((nValue / 10) >= 1) || (nValue < 0) )
			{
				strcpy(strTempBdsUdr, "F");
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return -1;
			}
			else
			{
				sprintf(strTempBdsUdr, "%01d", nValue);
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return 0;
			}
		case	2:
			if( ((nValue / 100) >= 1) || (nValue < 0) )
			{
				strcpy(strTempBdsUdr, "FF");
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
//				cout << nValue << " " << nLength << endl;
				return -1;
			}
			else
			{
				sprintf(strTempBdsUdr, "%02d", nValue);
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return 0;
			}
		case	3:
			if( ((nValue / 1000) >= 1) || (nValue < 0) )
			{
				strcpy(strTempBdsUdr, "FFF");
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return -1;
			}
			else
			{
				sprintf(strTempBdsUdr, "%03d", nValue);
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return 0;
			}
		case	5:
			if( ((nValue / 100000) >= 1) || (nValue < 0) )
			{
				strcpy(strTempBdsUdr, "FFFFF");
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return -1;
			}
			else
			{
				sprintf(strTempBdsUdr, "%05d", nValue);
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return 0;
			}
		case	9:
			if( ((nValue / 1000000000) >= 1) || (nValue < 0) )
			{
				strcpy(strTempBdsUdr, "FFFFFFFFF");
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return -1;
			}
			else
			{
				sprintf(strTempBdsUdr, "%09d0", nValue);
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return 0;
			}
		case	10:
			if( ((nValue / 10000000000) >= 1) || (nValue < 0) )
			{
				strcpy(strTempBdsUdr, "FFFFFFFFFF");
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return -1;
			}
			else
			{
				sprintf(strTempBdsUdr, "%010d", nValue);
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return 0;
			}
		case	15:
			if( ((nValue / 1000000000000000) >= 1) || (nValue < 0) )
			{
				strcpy(strTempBdsUdr, "FFFFFFFFFFFFFFF");
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return -1;
			}
			else
			{
				sprintf(strTempBdsUdr, "%015d", nValue);
				strTempBdsUdr[strlen(strTempBdsUdr)] = NULL;
				return 0;
			}
		default:
			memset(strTempBdsUdr, 'F', nLength);
			strTempBdsUdr[nLength] = NULL;
			return -1;
	}
}

void	CBDS_UdrFormat::display_UdrItem(char*	strBlockOfBillData, int nRecSize, unsigned int unUdrCnt, const char* fname)
{
	char	strBuf[128];
	const char	UDR_ITEM[41][4] = {	"A1",\
									"B1", "B2",\
									"C1", "C2",\
									"D1", "D2", "D3", "D4",\
									"E1",\
									"F1", "F2", "F3", "F4", "F5", "F51", "F6", "F7", "F8" ,\
									"F9" , "F10", "F11", "F12", "F13",\
									"G1", "G2", "G3", "G4", "G5", "G8", "G9", "G10",\
									"G11", "G12", "G13", "G14",\
									"H1",\
									"I1", "I2", "I3", "I4" };
	
	fstream	TempBdsFile(fname, ios::app);
	if(nFlag == 1){
		TempBdsFile <<" MSID,IPAddress,Network Access Identifier,Account Session ID,Correlation ID,MIP Home Agent,PDSN/FA Address,Serving PCF,BS/MSC ID,User Zone,Forward Mux Option,Reverse Mux Option,Forward Fundamental Rate,Reverse Fundamental Rate,Service Option,Service Option,Fundamental Traffic Type,Reverse Traffic Type,Fundamental Frame Size, Forward Fundamental RC, Reverse Fundamental RC, IP Technology,Compulsory Tunnel Indicator,Release Indicator,Data Octet Count(Terminating),Data Octet Count(Originating),Bad PPP Frame Count,StartEventTime,EndEventTime,Active Time,Number of Active Tramsitions,SDB Octet Count(Terminating),SDB Octet Count(Originating),Number of SDBs(Terminating),Number of SDBs(Originating),Number of PPP bytes received,Alternate Billing Identifier,IP Quality of Service,Interconnection IP Network Provider ID,Interconning IP Network Service Quality of Service,Airlink Quality of Service\n";
		nFlag = 0;
	}
	for(int k = 0; k < unUdrCnt ; k++){
		for(int j = 0; j < 41; j++){
			int nIndex = getOffsetNlength(UDR_ITEM[j]);
			memset(strBuf, NULL, 128);

			memcpy(strBuf, &strBlockOfBillData[(k*nRecSize)+stPtrUdrItem[nIndex].nStringOffset],
							stPtrUdrItem[nIndex].nStringLen);
			TempBdsFile << strBuf << ", ";
		}
		TempBdsFile << "\n";
	}
	TempBdsFile.close();
}

#include "UDRManager.hpp"
#include "AbnormalAcctDetector.hpp"
#include "FileLogWrite.hpp"

extern FileLogWrite* flog;

using namespace aad;

void CBDS_UdrFormat::collectUDRItem(char* strBlockOfBillData, int nRecSize, unsigned int unUdrCnt, fstream& fout, float text, float origin, float multi)
{
    char    strBuf[128];
    string  item;
    const char  UDR_ITEM[41][4] = { "A1",\
                                    "B1", "B2",\
                                    "C1", "C2",\
                                    "D1", "D2", "D3", "D4",\
                                    "E1",\
                                    "F1", "F2", "F3", "F4", "F5", "F51", "F6", "F7", "F8" ,\
                                    "F9" , "F10", "F11", "F12", "F13",\
                                    "G1", "G2", "G3", "G4", "G5", "G8", "G9", "G10",\
                                    "G11", "G12", "G13", "G14",\
                                    "H1",\
                                    "I1", "I2", "I3", "I4" };
    
    UDRManager um;
    AbnormalAcctDetector aad;

    for(int k = 0; k < unUdrCnt ; k++) {
       
        um.clear();

        for(int j = 0; j < 41; j++) {

            int nIndex = getOffsetNlength(UDR_ITEM[j]);
            memset(strBuf, NULL, 128);

            memcpy(strBuf, &strBlockOfBillData[(k*nRecSize)+stPtrUdrItem[nIndex].nStringOffset],
                            stPtrUdrItem[nIndex].nStringLen);
            item.assign(strBuf);
            um.push_back(item);
        }

        if (aad.checkAbnormal(&um, text, origin, multi) == true) { // abnormal 
            flog->Write("found abnormal");
            um.fprint(fout);
        }
    }
}
