/*****************************************************************************************
* file : CTrapDbLib.hpp
*
* T_TRAP_LOG 테이블 핸들링 클래스.
*
*                                                                 01-05-22 5:06오후 ky.
*****************************************************************************************/
#if !defined(_C_TRAP_DB_LIB_HPP_)
#define _C_TRAP_DB_LIB_HPP_




// Define
//----------------------------------------------------------
#define D_LOG_RECORD_CNT 100        // fetch할때 건수. (100건씩 읽어들인다)




// Structure
//----------------------------------------------------------
typedef struct
{
   char strRecvTime[ 17+1];   // receive time (삽입시간)
   int  nTrapNo;              // tran number
   char strTrapName[ 64+1];   // trap name
   int  nTrapValue;           // trap value
   char strTrapDesc[128+1];   // trap description
   int  strTrapDescLen;       // trap description length
} trapT;




// Class
//----------------------------------------------------------
class CTrapDbLib
{
private:
   bool m_nCursorFlag;              // Cursor의 사용여부. false:미사용, true:사용

public:
    CTrapDbLib();
   ~CTrapDbLib();

   bool OpenCursor();                                          // cursor open
   int  Select(trapT *data);                                   // fetch
   bool CloseCursor();                                         // cursor close
   bool UpdateSendTime(const char *ptrKey);                    // trap send time update
   bool Insert(const int nTrapNo,    const char *ptrTrapName,  // trap data insert
               const int nTrapValue, const char *ptrTrapDesc);
};//end CTrapDbLib




#endif//end _C_TRAP_DB_LIB_HPP_



