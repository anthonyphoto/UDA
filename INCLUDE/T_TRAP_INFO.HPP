#ifndef _CLOADACTTABLE_
#define _CLOADACTTABLE_

//#define SQLCA_STORAGE_CLASS extern
//#include <sqlca.h>

#include <iostream.h>
#include <stdlib.h>
#include "/home/uda1/include/SqlErr.hpp"

#define   OKACTION        1
#define   FAILACTION      -1


//----------------------------------------
// Declare Host Variable
typedef struct    __TrapInfo
{
	int         Trap_No;
	char        Trap_name		[64+1];
	char        Trap_Type		[20+1];
	char        Trap_desc		[128+1];
}TRAPINFO;
//----------------------------------------

class    t_trap_info : public SqlErr
{
    private:

    public:
    	TRAPINFO*	m_TrapInfo;
        t_trap_info();
		~t_trap_info();
		int	nFlag;

		void init_TrapTbl();
        void get_RecordCount()    throw(char *);// table에서 레코드 개수를 가져온다.
        void show_TrapTbl();					// Table의 내용을 Display하기 위한 함수
		void show_TrapInfoTable();

        void declare_Cursor()    throw(char *);
        void open_Cursor()       throw(char *);
        void close_Cursor();
        void fetch_Data()        throw(char *);
        void fetch_all()		 throw(char *);	//	pms 요청에 의한 수정 ( jsj 5. 25 )
		int SearchIndex(int nCode);

};

#endif
