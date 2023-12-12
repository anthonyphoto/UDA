#define SQLCA_STORAGE_CLASS extern
#include <sqlca.h>

#include <iostream.h>
#include <stdlib.h>
#include "../INCLUDE/SqlErr.hpp"

#ifndef	  OKPDSN
#define   OKPDSN        1
#define   FAILPDSN      -1
#endif


#ifndef _C017Pdsn_
#define _C017Pdsn_

//----------------------------------------
// Declare Host Variable
EXEC SQL BEGIN     DECLARE SECTION;

    typedef struct    __PdsnTable
    {
        int         Seq_No;
        int    	    Pdsn_Code;
        int   	    Vendor_Code;
    }PdsnTable;

EXEC SQL END     DECLARE SECTION;
//----------------------------------------



///////////////////////////////
//		Class C017Pdsn		 //
///////////////////////////////

class C017Pdsn : public SqlErr
{
    private:

    public:
        EXEC SQL BEGIN		DECLARE SECTION;
			int				m_nSeq_no;
            PdsnTable*  	m_PdsnTable;           // Action Table
            int           	m_nOutRecCount;        // Action Record Count
        EXEC SQL END		DECLARE SECTION;

        int m_nOutStartRecIndex;// Action Start Record Index

        C017Pdsn();
		~C017Pdsn();

        void init_PdsnTable();// [t_017pdsn_out] table의 레코드개수만큼 만들어진 배열을 초기화한다.

        void get_RecordCount()    throw(char *);// [t_017pdsn_out] table에서 레코드 개수를 가져온다.
        void show_PdsnTable();// [t_017pdsn_out] Table의 내용을 Display하기 위한 함수

        void declare_Cursor()    throw(char *);
        void open_Cursor()       throw(char *);
        void close_Cursor();
        void fetch_Data()        throw(char *);
        
       	// [t_017pdsn_out]table의 record count만큼 루프를 돌면서 Mapping Name, Mapping Version이
       	// 같은지 확인한 후 Sequence Number의 값이 0인 레코드의 시작 위치를 찾는다.
        int SearchPdsnIndex();

};

#endif
