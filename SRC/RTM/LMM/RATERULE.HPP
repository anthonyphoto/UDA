#ifndef _RATERULE_HPP
#define _RATERULE_HPP

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <SendLogManage.hpp>

//#define  DB_USERNAME  "uda"
//#define  DB_PASSWORD  "uda"

/* ��Ŷ���� ��� */
/*
1. ��Ŷ���� = (��Ŷ�� / ��Ŷsize)
2. ��� = (�ش�ð���Ŷ����ȭ��) * (��Ŷ����)
*/


struct RATE_TABLE {
	int 	Rtype;	// ���� (�Ϲ�:1 ����:2 �ɾ�:3) => ����Ÿ�� TTL, N-TOP ���� ���İ������
	int		Today;	// ���� (����(��-��):1 ����:2)
	int		Stime;	// ���۽ð� = (HH*100)+mm
	int		Ltime;	// ���ð�  = (HH*100)+mm
	int		Rate;	// 10�ʴ� ��ȭ��	(����)
	int		PRate;	// ��Ŷ�� ��ȭ��	(����)
	int		PSize;	// ��ŶSize
};

/* �����ڵ�(�Ϲ�/����/�ɾ�, ���ϱ���(����,�����,������), ���۽ð�(HHmm), ���ð�, 10�ʴ���ȭ��, ��Ŷ����ȭ��, ��ŶSize 
RateTable 	tblRate[6]=	{{1,1,  800, 2100, 17, 17, 512},
						 {2,1,  600,  800, 12, 12, 512},
						 {2,1, 2100, 2400, 12, 12, 512},
						 {2,2,  600, 2400, 12, 12, 512},
						 {3,2,    0,  600,  8,  8, 512},
						 {3,1,    0,  600,  8,  8, 512} };

*/

struct HOLIDAY_TABLE {
	char 	Date[10];	// YYYYMMDD
	char	Name[10];	// ���ϸ�
};

typedef RATE_TABLE		RateTable;
typedef HOLIDAY_TABLE	HolidayTable;

#define		FNAME_RATE_TABLE		"/SVC1/DATA/rate_tbl.cfg"	// �������� => DB�����ʿ�
#define		FNAME_HOLIDAY_TABLE		"/SVC1/DATA/holiday.cfg"
#define		RATE_MAX	30			// ������̺� �ִ밳��
#define		HOLIDAY_MAX	50			// ���������� �ִ밳��

//--------------------------------------------------------------------
class RateRule {
	
	private:
		int				Rn;		// ������̺� ����
		int				Hn;		// ������ ����
		int				chk_H;	// 1.���� 2.����� 3.����
		int				OnPacket;	// ��Ŷ���ݿ���
		int				PSize;
		int				LimitSec;
		int				fgHI;
		char			strDay[14];
		int				DBerror;
		
		time_t			STime;
		time_t			ETime;
		struct  tm 		*Tm;	
		RateTable		tblRate[RATE_MAX];		// ������̺�
		HolidayTable	tblHoliday[HOLIDAY_MAX];// ����������
		
		int				CHK_Holyday(void);		// ������üũ
		int				GetRate(void);			// ������̺��� �˻��� 10�ʴ��� ����
						// ��ȭ����� ����Ѵ�(�ð�����) amount : �ð��� select : ������ 1(����) 2(��)
		//int				GetTelRate(int amount, int select);	
		int				GetTelRate(int amount, int select, int last);	
		int				GetRate(int select);	// select : ������ 1(����) 2(��)
		
//		int				ConnectToDB(void);
//		int				Db_close(void);
		
		int 			FetchRateDB(void);
		int				FetchHolidayDB(void);
	
	public:
		RateRule(void);
		~RateRule(void){;}
		
		//int GetRate(int rtype, time_t stime, time_t etime, int packet=0); 
		// rtype : 1(����) 2(����) / stime: ���۽ð�  / etime: ���ð�  /last: ��������꿩�� / parket : ��Ŷ��
		int GetRate(int rtype, time_t stime, time_t etime, int last, int packet=1); 
		int	GetLimitSec(void)	{ return LimitSec; } // ����ϰ� ���� �ð� & ��Ŷ
};

#endif

