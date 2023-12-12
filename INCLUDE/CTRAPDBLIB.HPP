/*****************************************************************************************
* file : CTrapDbLib.hpp
*
* T_TRAP_LOG ���̺� �ڵ鸵 Ŭ����.
*
*                                                                 01-05-22 5:06���� ky.
*****************************************************************************************/
#if !defined(_C_TRAP_DB_LIB_HPP_)
#define _C_TRAP_DB_LIB_HPP_




// Define
//----------------------------------------------------------
#define D_LOG_RECORD_CNT 100        // fetch�Ҷ� �Ǽ�. (100�Ǿ� �о���δ�)




// Structure
//----------------------------------------------------------
typedef struct
{
   char strRecvTime[ 17+1];   // receive time (���Խð�)
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
   bool m_nCursorFlag;              // Cursor�� ��뿩��. false:�̻��, true:���

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


