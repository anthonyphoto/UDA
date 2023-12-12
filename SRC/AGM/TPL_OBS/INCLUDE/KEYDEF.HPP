/******************************************************************
* �� �� ��     :  KeyDef.hpp		     			 			
* ��   ��                                                       
* :  Btree Key���Ǵ� ������� ������, �ű⿡ ���� Accessor����	
*                                                 			   	   
* �� �� ��     : Han Guen Hee( jameshans@hanmail.net )                    				     
* first data  : 2001. 02. 06      	     						     
* last updata : 2001. 05. 21                 		 		 				      
* �� �� ȭ ��  : 					        					     
* Version 	  : 1.1( 2001. 05. 21 )                                  
* program history :                       						
******************************************************************/
#ifndef KEYDEF_HPP                                                 
#define KEYDEF_HPP
#include<iomanip.h>
#include<string.h>
#include <SendLogManage.hpp>                    // File Log

#define 	LEN_SESSIONID			8
#define		LEN_CORRELATIONID		8

extern SendLogManage*    LogM;   // �α׸Ŵ��� (Main Class���� ����)

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
		// ����ڰ� total size�� ����ؼ� �޸� ������ ������� �Ѵ�.
		// ���� ��� �޸� ������ 4byte������ ����ٸ�, 4�� ����� size�� ������� �Ѵ�.
		// ������ size�� ���̸� �־���� �Ѵ�.
		unsigned int		unMsID;
		char				strAccSessionID	[LEN_SESSIONID+1];
		char				strCorrID		[LEN_CORRELATIONID+1];
		char				dummy[2];
};

#endif

