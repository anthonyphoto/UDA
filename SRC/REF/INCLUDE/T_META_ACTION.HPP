#define SQLCA_STORAGE_CLASS extern
#include <sqlca.h>

#include <iostream.h>
#include <stdlib.h>
#include "../INCLUDE/SqlErr.hpp"

#ifndef	  OK
#define   OK        1
#define   FAIL      -1
#endif

#ifndef _CLOADACTTABLE_
#define _CLOADACTTABLE_

//----------------------------------------
// Declare Host Variable
EXEC SQL BEGIN     DECLARE SECTION;

    typedef struct    __ActionTable
    {
        char        mapping_name    [8+1];
        char        Version         [5+1];
        char        Action_Code     [2+1];
        char        Sub_Action_Code [2+1];
        char        Data_Type       [2+1];
        VARCHAR     In_Field_Name   [30+1];
        VARCHAR     Out_Field_Name  [30+1];
        int         Seq_No;
        int         Out_Field_Length;
        int         Out_Field_Offset;
        int         In_Field_Length; 
        int         In_Field_Offset;
    }ActionTable;

EXEC SQL END     DECLARE SECTION;
//----------------------------------------



///////////////////////////////////
//		Class LoadActTable		 //
///////////////////////////////////

class    CLoadActTable : public SqlErr
{
    private:

    public:
        EXEC SQL BEGIN     DECLARE SECTION;
            char          m_strMappingName[8+1];   // Mapping Name
            char          m_strMapVersion[5+1];    // Mapping Version
            ActionTable*  m_ActionTable;           // Action Table
            int           m_nActRecCount;          // Action Record Count
        EXEC SQL END     DECLARE SECTION;

        int m_nActStartRecIndex;// Action Start Record Index

        CLoadActTable(char *strMappingName,char *strVer);
		~CLoadActTable();

        void init_ActionTable();// [t_clt_action] table�� ���ڵ尳����ŭ ������� �迭�� �ʱ�ȭ�Ѵ�.

        void get_RecordCount()    throw(char *);// [t_clt_action] table���� ���ڵ� ������ �����´�.
        void show_ActionTable();// [t_clt_action] Table�� ������ Display�ϱ� ���� �Լ�

        void declare_Cursor()    throw(char *);
        void open_Cursor()       throw(char *);
        void close_Cursor();
        void fetch_Data()        throw(char *);
        
       	// [t_clt_action]table�� record count��ŭ ������ ���鼭 Mapping Name, Mapping Version��
       	// ������ Ȯ���� �� Sequence Number�� ���� 0�� ���ڵ��� ���� ��ġ�� ã�´�.
        int SearchActIndex();

};

#endif