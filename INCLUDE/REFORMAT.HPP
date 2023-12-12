#ifndef _REFORMAT_HPP
#define _REFORMAT_HPP

#define SIZE_HEADER			20
#define SIZE_MAX_ATTRIBUTE	480

typedef unsigned short 	sint;
typedef unsigned int 	uint;
typedef unsigned char	uchar;

//////////////////////////////
//		Class CPdsnMsg 		//
//////////////////////////////

//------------------------------------------------------------------
//UDA-PDSN 메시지
class CPdsnMsg
{
	private :
		uchar	ucCode;					// Radius 패킷의 타입을 지정('4':과금요청, '5':과금응답)
		uchar	ucIdentifier;			// 요청과 응답을 일치시킨다.
		sint	usLength;				// Code, Identifier, Length, Authenticator, Attributes 크기를 모두 합한 값
		uchar	ucAuthenticator[16];	// Client와 Radius과금서버간의 메시지 인증을 위해 사용
//		char	strAttribute[SIZE_MAX_ATTRIBUTE];// 많은 경우가 존재가 존재하며 값의 순서도 정해져 있지 않다	

	public :
		char	strAttribute[SIZE_MAX_ATTRIBUTE];// 많은 경우가 존재가 존재하며 값의 순서도 정해져 있지 않다	
	
		uchar	get_ucCode()			{ return ucCode; 		 }
		uchar	get_ucIdentifier()		{ return ucIdentifier;   }
		sint	get_usLength()			{ return usLength;		 }
		uchar*	get_ucAuthenticator()	{ return ucAuthenticator;}
//		char*	get_strAttribute()		{ return strAttribute;   }
		
		void	set_ucCode			(uchar ucValue)	{ ucCode = ucValue;		     }
		void	set_ucIdentifier	(uchar ucValue)	{ ucIdentifier = ucValue;    }
		void	set_usLength		(sint  usValue)	{ usLength = usValue; 	  	 }
		void	set_ucAuthenticator	(uchar *ucValue){ memcpy(ucAuthenticator, ucValue, 16); }
		// TEST :snmp 관련 2001.05.28.
		void	set_strAttribute	(char* strValue){ memcpy(&strAttribute[250], strValue, strlen(strValue)); }
};

#endif

