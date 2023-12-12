/////////////////////////////////////////////////////////////
//
//   Class Name     : SqlErr
//   Usage          : Sql Error ó���� ���� ���� Ŭ����
//   DATE           : 2000. 12 .1
//   LAST UPDATE    :
//   AUTHOR         : Kwon Chong Wook (cwkwon@neomnt.co.kr)
//   History        :
//
/////////////////////////////////////////////////////////////
#ifndef _CSQLERR_
#define _CSQLERR_
#include <iostream.h>
#include <stdlib.h>
#include <string.h>

/////////////////////////////////////////////////////////////
//	Comment : SQL Error�� ó���ϱ� ���� Ŭ����
/////////////////////////////////////////////////////////////
class	SqlErr
{
	private:
		char	strSqlError[128];

	protected:
		static char	strSqlTemp[50];
		char        m_strSqlMsg[400];

	public:
		SqlErr();
		// Initiate SQLCA Option
		void	Init_SqlCa();
		// Error �߻��� program ������ �ʿ䰡���� ��
		virtual void	SqlError(char* strMsg=NULL);
		// Error Message print and Return
		virtual char	*SqlErrCout(char* strMsg=NULL);

};

#endif