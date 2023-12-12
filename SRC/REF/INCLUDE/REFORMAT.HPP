#ifndef _REFORMAT_HPP
#define _REFORMAT_HPP
#include <string.h>
#define SIZE_MAX_ATTRIBUTE	480

typedef unsigned short 	sint;
typedef unsigned int 	uint;
typedef unsigned char	uchar;

//////////////////////////////
//		Class CPdsnMsg 		//
//////////////////////////////

//------------------------------------------------------------------
//UDA-PDSN �޽���
class CPdsnMsg
{
	private :
		uchar	ucCode;					// Radius ��Ŷ�� Ÿ���� ����('4':���ݿ�û, '5':��������)
		uchar	ucIdentifier;			// ��û�� ������ ��ġ��Ų��.
		sint	usLength;				// Code, Identifier, Length, Authenticator, Attributes ũ�⸦ ��� ���� ��
		uchar	ucAuthenticator[16];	// Client�� Radius���ݼ������� �޽��� ������ ���� ���
		char	strAttribute[SIZE_MAX_ATTRIBUTE];// ���� ��찡 ���簡 �����ϸ� ���� ������ ������ ���� �ʴ�	

	public :
		uchar	get_ucCode()			{ return ucCode; 		 }
		uchar	get_ucIdentifier()		{ return ucIdentifier;   }
		sint	get_usLength()			{ return usLength;		 }
		uchar*	get_ucAuthenticator()	{ return ucAuthenticator;}
		char*	get_strAttribute()		{ return strAttribute;   }
		
		void	set_ucCode			(uchar ucValue)	{ ucCode = ucValue;		     }
		void	set_ucIdentifier	(uchar ucValue)	{ ucIdentifier = ucValue;    }
		void	set_usLength		(sint  usValue)	{ usLength = usValue; 	  	 }
		void	set_ucAuthenticator	(uchar *ucValue){ memcpy(ucAuthenticator, ucValue, 16); }
		void	set_strAttribute	(char* strValue){ memcpy(strAttribute, strValue, sizeof(strAttribute)); }
};

#endif
