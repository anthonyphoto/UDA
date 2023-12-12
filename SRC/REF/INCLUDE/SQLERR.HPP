/////////////////////////////////////////////////////////////
//
//   Class Name     : SqlErr
//   Usage          : Sql Error 처리를 위한 공용 클래스
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
//	Comment : SQL Error를 처리하기 위한 클래스
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
		// Error 발생후 program 종료할 필요가있을 때
		virtual void	SqlError(char* strMsg=NULL);
		// Error Message print and Return
		virtual char	*SqlErrCout(char* strMsg=NULL);

};

#endif
