/*--------------------------------------------------------------------
	T_TRANS_CODE : Parsing�� Udr�� ���� MQ�� �����ϱ� ���� ���
----------------------------------------------------------------------*/

#ifndef _T_TRANS_CODE_HPP
#define _T_TRANS_CODE_HPP

#include <iostream.h>
#include <iomanip.h>
#include <string.h>
#include <stdlib.h>
#include <strstream.h>

#define OK		1	// ����
#define FAIL	-1	// ����

typedef unsigned int 	uint;

//////////////////////////////
//	CLASS	Rec_Trans_Code	//
//////////////////////////////
// [t_trans_code] table�� ���� �ӽ� ����
class Rec_Trans_Code
{
	private:
		char	svc_provider	[3+1];	// ����� ��ȣ 
		char	trans_flag		[2+1];	// Parsing�� Udr ���� ����(SKT or STi MQ)

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
// [t_trans_code] table�� ���� �о� ���� ����
class T_Trans_Code
{
	private:

	public:
        T_Trans_Code()	{;}
		~T_Trans_Code()	{;}
		
		// 		T_TRANS_CODE table���� Udr ���� ������ �о��								
		int		Get_Record(Rec_Trans_Code *recTransCode);	
};


#endif
