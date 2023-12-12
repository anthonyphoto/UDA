#define SQLCA_STORAGE_CLASS extern
#include <sqlca.h>
#include "../INCLUDE/FileLogWrite.hpp"

// DECLARE SECTION�� hpp�� �����Ҷ� 
EXEC SQL INCLUDE "../INCLUDE/RefActTbl.hpp";


#ifndef _CREFFUNC_
#define _CREFFUNC_
#define OUT_MAX_BUFFER


///////////////////////////////
//		Class CRefFunc		 //
///////////////////////////////

class CRefFunc : public CRefActTbl
{
    private:
		char*	m_strTemp;		// Input Data�� ������ Input Temp Buffer
		char	m_strOutTemp[OUT_MAX_BUFFER];// Output Data Type�� �°� ��ȯ�� Data�� ������ Output Temp Buffer

		int		nActIndex;		// [t_ref_action] table���� �� Record�� �б� ���� Index
		int		nInOffset;		// �ش� Input Field�� ��ġ
		int		nOutOneLen;		// �� Output Data Format�� ��ü ����(Type, Length, Value�� ũ�⸦ ��� ���� ��)
		int 	nOutTotLen;		// ��ü Output Data size
		int		nCurrentPos;	// Output Temp Buffer���� ���� ��ġ

    public:
        CRefFunc(char *strMappingName,char *strActVer);
        ~CRefFunc();
        
        FileLogWrite*  	LogM;// Input, Output File�� log file�� ����ϱ� ����
        
        //accessor
        int 	get_OutTotLen	{ 	return 	nOutTotLen;		}
        
        //		�� ���ڵ带 Input Record�� Field���� ���� ��,
		//		Output Record�� offset�� ���̸�ŭ Output Buffer�� �����Ѵ�. 
        int		UdrReformat(char *strIn, char *strOut);

		//		Input Data�� [t_ref_action] table�� Data Type�� �°� Conversion�Ѵ�
		//		->��ȯ�� Data�� Action code�� ���� Sub action code�� �о ���ϴ� Character�� padding�Ѵ�.
		//		->Output Buffer�� Data�� Move�Ѵ�.
        int		OneFieldReformat(char *ptrSrc,char *ptrDes);

		//		Input Buffer�� ����� �� �ʵ��� ������ offset�� length�� �°� �����´�.
        void	GetOneField(char *OneCDR, char *strTemp, int nRecSeqNo);

        //		Output Buffer�� �� �ʵ��� ������ offset�� length�� �°� �����Ų��.
        void	PutOneField(char* strIn, char *strOutBuff, int nRecSeqNo);

		//		�� �ʵ��� data�� Output Buffer�� �ش� offset, length��ŭ �����Ѵ�. 
        void	DataMove(char *InBuf, char *OutBuf);

};

#endif