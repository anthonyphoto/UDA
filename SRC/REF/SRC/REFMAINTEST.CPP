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
//UDA-PDSN �޽���
typedef struct PDSNMSG{
	uchar	ucCode;					// Radius ��Ŷ�� Ÿ���� ����('4':���ݿ�û, '5':��������)
	uchar	ucIdentifier;			// ��û�� ������ ��ġ��Ų��.
	sint	usLength;				// Code, Identifier, Length, Authenticator, Attributes ũ�⸦ ��� ���� ��
	uchar	ucAuthenticator[16];	// Client�� Radius���ݼ������� �޽��� ������ ���� ���
	char	strAttribute[SIZE_MAX_ATTRIBUTE];// ���� ��찡 ���簡 �����ϸ� ���� ������ ������ ���� �ʴ�
}PdsnMsg;	


//------------------------------------------------------------------
// [t_ref_action] table������ ������ Input Record�� Output Type�� ���� ��ȯ�� ��,
// Output format(Type, Length, Value)�� �°� �����Ѵ�.
CRefFunc        *clsRefObj;
ConfigCTRL		*clsCfgCTRL;// Reformat ���������� Config File�� Write or Read�ϱ� ���� 
PdsnMsg			stPdsnMsg;

char	gstrAtribut[SIZE_MAX_ATTRIBUTE];//Output format�� ���� ����� ���� 
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

    int		nInRecSize 	 = 0;	// �� Input Record�� ũ��
    char*	strInBuf;			// Input���� �� ���ڵ带 ������

    nInRecSize  = clsRefObj->m_ActionTable[0].In_Field_Length; //[t_clt_action]table���� �� Input Record�� ũ�⸦ �о��
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
		
		InFile.read(strInBuf, nInRecSize);			// Input File���� �� ���ڵ带 ����

	    memset(gstrAtribut, 0x00, sizeof(gstrAtribut));
		
		clsRefObj->LogM->Write("\n\n");
		clsRefObj->LogM->Write("================================");
		clsRefObj->LogM->Write("#Udr Format[%d] : ", nRecCount+1);
		clsRefObj->LogM->WriteHexLog(strInBuf, nInRecSize);	// �о���� �� ���ڵ带 hexa�� �� ũ�⸸ŭ log�� ���� 
		clsRefObj->LogM->Write("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		clsRefObj->LogM->Write("-InputRecSize  : %d" , nInRecSize);

		if(ProcesingParser(strInBuf) < 0)	{ Destructor();	return -1; }
		nRecCount++;	// �� ���ڵ带 ���� ���� count ����

		clsRefObj->LogM->Write("\n");
		clsRefObj->LogM->Write("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		clsRefObj->LogM->Write("#Pdsn Format[%d] : ", nRecCount);
		clsRefObj->LogM->WriteHexLog((char*)&stPdsnMsg, sizeof(PDSNMSG));// ��ȯ�� ���� �� ���ڵ带 hexa�� �� ũ�⸸ŭ log�� ���� 
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