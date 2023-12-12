#include <iostream.h>
#include <configCTRL.hpp>
#include "../INCLUDE/dblib.hpp"
#include "../INCLUDE/RfFunc.hpp"

#define SIZE_MAX_ATTRIBUTE	480
#define REQUEST_CODE		4

typedef unsigned short 	sint;
typedef unsigned int 	uint;
typedef unsigned char	uchar;

//------------------------------------------------------------------
//UDA-PDSN 메시지
typedef struct PDSNMSG{
	uchar	ucCode;					// Radius 패킷의 타입을 지정('4':과금요청, '5':과금응답)
	uchar	ucIdentifier;			// 요청과 응답을 일치시킨다.
	sint	usLength;				// Code, Identifier, Length, Authenticator, Attributes 크기를 모두 합한 값
	uchar	ucAuthenticator[16];	// Client와 Radius과금서버간의 메시지 인증을 위해 사용
	char	strAttribute[SIZE_MAX_ATTRIBUTE];// 많은 경우가 존재가 존재하며 값의 순서도 정해져 있지 않다
}PdsnMsg;	


//------------------------------------------------------------------
// [t_ref_action] table정보를 가지고 Input Record를 Output Type에 따라 변환한 후,
// Output format(Type, Length, Value)에 맞게 저장한다.
CRefFunc        *clsRefObj;
ConfigCTRL		*clsCfgCTRL;// Reformat 구성정보를 Config File에 Write or Read하기 위함 
PdsnMsg			stPdsnMsg;

char	gstrAtribut[SIZE_MAX_ATTRIBUTE];//Output format에 따라 저장된 값들 
int		ProcesingParser();
void	Destructor(void);



//==============================================================================
int main()
{
    CDblib	clsDblib;					// DB
	clsDblib.connectDB("uda1", "uda1");	// DB Connect

	clsRefObj  = new CRefFunc("REF01", "1");
    clsCfgCTRL = new ConfigCTRL(1,3);

    clsCfgCTRL->set_iNum();
    stPdsnMsg.ucCode = REQUEST_CODE;

    int		nInRecSize 	 = 0;	// 한 Input Record의 크기
    char*	strInBuf;			// Input으로 한 레코드를 저장함

    nInRecSize  = clsRefObj->m_ActionTable[0].In_Field_Length; //[t_clt_action]table에서 한 Input Record의 크기를 읽어옴
	strInBuf    = new char[nInRecSize+1];
	memset(strInBuf, 0x00, nInRecSize+1);	

	// Open Input File for read
	ifstream InFile("./UDR017.txt", ios::in | ios::binary);
	if (!InFile){
		cout << "!!!Error : InFile Unable to open." << endl;
		return -1;
	}
	int	nRecCount    = 0;	// Record Count
    int i = 1;
	while(i--)
    {
		
		InFile.read(strInBuf, nInRecSize);			// Input File에서 한 레코드를 읽음

	    memset(gstrAtribut, 0x00, sizeof(gstrAtribut));
		
		clsRefObj->LogM->Write("\n\n");
		clsRefObj->LogM->Write("================================");
		clsRefObj->LogM->Write("#Udr Format[%d] : ", nRecCount+1);
		clsRefObj->LogM->WriteHexLog(strInBuf, nInRecSize);	// 읽어들인 한 레코드를 hexa로 그 크기만큼 log를 남김 
		clsRefObj->LogM->Write("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		clsRefObj->LogM->Write("-InputRecSize  : %d" , nInRecSize);

		if(ProcesingParser(strInBuf) < 0)	{ Destructor();	return -1; }
		nRecCount++;	// 한 레코드를 읽은 다음 count 증가

		clsRefObj->LogM->Write("\n");
		clsRefObj->LogM->Write("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		clsRefObj->LogM->Write("#Pdsn Format[%d] : ", nRecCount);
		clsRefObj->LogM->WriteHexLog((char*)&stPdsnMsg, sizeof(PDSNMSG));// 변환된 후의 한 레코드를 hexa로 그 크기만큼 log를 남김 
		clsRefObj->LogM->Write("================================\n");
		memset(strInBuf, 0x00, nInRecSize+1);	
	}	
	InFile.close();
	clsDblib.disconnectDB();
	Destructor();
	return 0;
}		


//------------------------------------------------------------------------------
int ProcesingParser(char* strInBuf)
{
	
	if(clsRefObj->UdrReformat(strInBuf,gstrAtribut)<0)	return -1;
	memcpy(stPdsnMsg.strAttribute, gstrAtribut, sizeof(gstrAtribut));

	clsRefObj->LogM->Write("#Attribute);
	clsRefObj->LogM->WriteHexLog(gstrAtribut, SIZE_MAX_ATTRIBUTE);

	if(clsCfgCTRL->write_Udr(&stPdsnMsg) < 0){
		cout << "Cannot Open  File for Write Reformat Udr. " << endl;
		return -1;
	}
		
	cout << "Write Reformat Udr Config Information." << endl;
	return 1;
}	



//------------------------------------------------------------------------------
void Destructor(void)
{
	delete [] strInBuf;
	delete clsCfgCTRL;
	delete clsRefObj;
}	