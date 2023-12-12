#define SQLCA_STORAGE_CLASS extern
#include <sqlca.h>
#include "../INCLUDE/FileLogWrite.hpp"

// DECLARE SECTION을 hpp에 선언할때 
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
		char*	m_strTemp;		// Input Data를 저장한 Input Temp Buffer
		char	m_strOutTemp[OUT_MAX_BUFFER];// Output Data Type에 맞게 변환된 Data를 저장한 Output Temp Buffer

		int		nActIndex;		// [t_ref_action] table에서 한 Record를 읽기 위한 Index
		int		nInOffset;		// 해당 Input Field의 위치
		int		nOutOneLen;		// 한 Output Data Format의 전체 길이(Type, Length, Value의 크기를 모두 합한 값)
		int 	nOutTotLen;		// 전체 Output Data size
		int		nCurrentPos;	// Output Temp Buffer에서 현재 위치

    public:
        CRefFunc(char *strMappingName,char *strActVer);
        ~CRefFunc();
        
        FileLogWrite*  	LogM;// Input, Output File을 log file에 기록하기 위함
        
        //accessor
        int 	get_OutTotLen	{ 	return 	nOutTotLen;		}
        
        //		한 레코드를 Input Record의 Field별로 읽은 후,
		//		Output Record의 offset과 길이만큼 Output Buffer에 저장한다. 
        int		UdrReformat(char *strIn, char *strOut);

		//		Input Data를 [t_ref_action] table의 Data Type에 맞게 Conversion한다
		//		->변환된 Data를 Action code에 따라 Sub action code를 읽어서 원하는 Character로 padding한다.
		//		->Output Buffer로 Data를 Move한다.
        int		OneFieldReformat(char *ptrSrc,char *ptrDes);

		//		Input Buffer에 저장된 한 필드의 내용을 offset과 length에 맞게 가져온다.
        void	GetOneField(char *OneCDR, char *strTemp, int nRecSeqNo);

        //		Output Buffer에 한 필드의 내용을 offset과 length에 맞게 저장시킨다.
        void	PutOneField(char* strIn, char *strOutBuff, int nRecSeqNo);

		//		한 필드의 data를 Output Buffer에 해당 offset, length만큼 저장한다. 
        void	DataMove(char *InBuf, char *OutBuf);

};

#endif
