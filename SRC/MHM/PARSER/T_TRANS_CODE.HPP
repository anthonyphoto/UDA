/*--------------------------------------------------------------------
	T_TRANS_CODE : Parsing된 Udr을 보낼 MQ를 결정하기 위해 사용
----------------------------------------------------------------------*/

#ifndef _T_TRANS_CODE_HPP
#define _T_TRANS_CODE_HPP

#include <iostream.h>
#include <iomanip.h>
#include <string.h>
#include <stdlib.h>
#include <strstream.h>

#define OK		1	// 성공
#define FAIL	-1	// 실패

typedef unsigned int 	uint;

//////////////////////////////
//	CLASS	Rec_Trans_Code	//
//////////////////////////////
// [t_trans_code] table의 값을 임시 저장
class Rec_Trans_Code
{
	private:
		char	svc_provider	[3+1];	// 사업자 번호 
		char	trans_flag		[2+1];	// Parsing된 Udr 전송 여부(SKT or STi MQ)

	 public:
	 	Rec_Trans_Code()
	 	{
			memset(svc_provider, 0x00, sizeof(svc_provider));
			memset(trans_flag  , 0x00, sizeof(trans_flag));
	 	}
	 	
	 	void set_svc_provider(char *str)	{	memcpy(svc_provider, str, 3);	}
	 	void set_trans_flag(char *str)		{	memcpy(trans_flag  , str, 2);	}			

        char* get_svc_provider()			{	return svc_provider;	}	
        char* get_trans_flag()				{	return trans_flag;		}	

};
        
        
        
//////////////////////////////
//	CLASS	T_Trans_Code	//
//////////////////////////////
// [t_trans_code] table의 값을 읽어 오기 위함
class T_Trans_Code
{
	private:

	public:
        T_Trans_Code()	{;}
		~T_Trans_Code()	{;}
		
		// 		T_TRANS_CODE table에서 Udr 전송 정보를 읽어옴								
		int		Get_Record(Rec_Trans_Code *recTransCode);	
};


#endif

