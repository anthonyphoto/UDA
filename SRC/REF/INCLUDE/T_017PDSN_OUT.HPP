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

        void init_PdsnTable();// [t_017pdsn_out] table�� ���ڵ尳����ŭ ������� �迭�� �ʱ�ȭ�Ѵ�.

        void get_RecordCount()    throw(char *);// [t_017pdsn_out] table���� ���ڵ� ������ �����´�.
        void show_PdsnTable();// [t_017pdsn_out] Table�� ������ Display�ϱ� ���� �Լ�

        void declare_Cursor()    throw(char *);
        void open_Cursor()       throw(char *);
        void close_Cursor();
        void fetch_Data()        throw(char *);
        
       	// [t_017pdsn_out]table�� record count��ŭ ������ ���鼭 Mapping Name, Mapping Version��
       	// ������ Ȯ���� �� Sequence Number�� ���� 0�� ���ڵ��� ���� ��ġ�� ã�´�.
        int SearchPdsnIndex();

};

#endif