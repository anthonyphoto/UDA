/******************************************************************
* 파 일 명     :  KeyDef.hpp		     			 				  *
* 기   능                                                         *
* :  Btree Key가되는 멤버들을 가지며, 거기에 대한 Accessor제공	  	  *
*                                                 			   	  *
* 작 성 자     : Han Guen Hee                    				  *
* first data  : 2001. 02. 06      	     						  *
* last updata :                  		 		 				  *
* 관 련 화 일  : 					        					  *
* program history :                       						  *
* class argument :         	        	 						  *
* bug fix :                       	     						  *
*                         	        	 						  *
******************************************************************/
#ifndef KEYDEF_HPP
#define KEYDEF_HPP
#include<iomanip.h>
#include<string.h>
#include <SendLogManage.hpp>                    // File Log

#define 	LEN_SESSIONID			8
#define		LEN_CORRELATIONID		8

extern SendLogManage*    LogM;   // 로그매니저 (Main Class에서 정의)

class KeyDef
{
	public :
		// Constructor
		KeyDef(){ unMsID = 0; }
		// Destructor
		~KeyDef(){}
		// Accessor
		void				set_unMsID( const unsigned int nValue )	{	unMsID = nValue;}
		void				set_strAccSessionID( const char* strValue )
		{	
			memset(strAccSessionID, NULL, LEN_SESSIONID+1);
			memcpy(strAccSessionID,	strValue, strlen(strValue));	
		}
		void				set_strCorrID( const char* strValue )
		{	
			memset(strCorrID,	NULL, LEN_CORRELATIONID+1);
			memcpy(strCorrID, 	strValue, strlen(strValue));	
		}
		unsigned int		get_unMsID()			{	return unMsID;	}
		char*				get_strAccSessionID()	{	return strAccSessionID;	}
		char*				get_strCorrID()			{	return strCorrID;	}
		void 				init()
							{
								unMsID = 0;	
								memset(strAccSessionID, NULL, LEN_SESSIONID+1);
								memset(strCorrID,	NULL, LEN_CORRELATIONID+1);
								memset(dummy,	NULL, 2);
							}
		void 				print_all( void ) const 
							{
								*LogM << LogM->ErrorCode(0) << "=======key define value======" << endl;
								*LogM << LogM->ErrorCode(0) << "unMsID : [" << unMsID << "]" << endl;
								*LogM << LogM->ErrorCode(0) << "strAccSessionID : [" << strAccSessionID << "]" << endl;
								*LogM << LogM->ErrorCode(0) << "strCorrID : [" << strCorrID << "]" << endl;
								*LogM << LogM->ErrorCode(0) << "=============================" << endl;
							}
		
	private : 
		// 사용자가 total size를 계산해서 메모리 구조에 맞춰줘야 한다.
		// 예를 들어 메모리 구조가 4byte단위로 끊긴다면, 4의 배수로 size를 맞춰줘야 한다.
		// 부족한 size는 더미를 넣어줘야 한다.
		unsigned int		unMsID;
		char				strAccSessionID	[LEN_SESSIONID+1];
		char				strCorrID		[LEN_CORRELATIONID+1];
		char				dummy[2];
};

#endif


