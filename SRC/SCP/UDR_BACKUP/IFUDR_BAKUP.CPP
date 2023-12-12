/******************************************************************
* 파 일 명      : ifudr_bakup.cpp
* 기   능       : UDR 화일을 백업한다. 
* 작 성 자      : 조상영                         
* first data    :                     
* last updata   : 2000. 10. 23                  
* 관 련 화 일   : program history                                
* Version       : 1.1 ( 2001. 05. 21 )                                           
******************************************************************/


#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <sys/timeb.h>
#include <time.h>


static const char *MONTH[12] = { "Jan", "Feb", "Mar", 
                                 "Apr", "May", "Jun",
                                 "Jul", "Aug", "Sep",
                                 "Oct", "Nov", "Dec" };

/*
const char* RAD1_DIR		="/SVC1/RADIUS";
const char*	RAD2_DIR		="/SVC2/RADIUS";
const char* RAD1_FNAME		="RADIUS_UDR1";
const char*	RAD2_FNAME		="RADIUS_UDR2";
const char*	RAD1_BACKUPDIR 	="/SVC1/RADIUS/BAK";
const char*	RAD2_BACKUPDIR 	="/SVC2/RADIUS/BAK";
const char*	RAD1_BNAME		="R1_";
const char*	RAD2_BNAME		="R2_";


const char*	BDS1_DIR		="/SVC1/BDS";
const char*	BDS2_DIR		="/SVC2/BDS";
const char*	BDS1_FNAME		="BDS_UDR";
const char*	BDS2_FNAME		="BDS_UDR";

const char*	BDS1_BACKUPDIR	="/SVC1/BDS/BAK";
const char*	BDS2_BACKUPDIR	="/SVC2/BDS/BAK";
const char*	BDS1_BNAME		="B1_";
const char*	BDS2_BNAME		="B2_";

#define		BACKUP_DAY		2		
#define		BACKUP_DEL_DAY	7		
struct tm {
    int tm_sec;       // second (0-61, allows for leap seconds) 
    int tm_min;       // minute (0-59) 
    int tm_hour;      // hour (0-23) 
    int tm_mday;      // day of the month (1-31) 
    int tm_mon;       // month (0-11) 
    int tm_year;      // years since 1900 
    int tm_wday;      // day of the week (0-6) 
    int tm_yday;      // day of the year (0-365) 
    int tm_isdst;     // non-0 if daylight savings time is in effect 
};

*/

//--------------------------------------------------------------------
class Udr_Backup {
	
	private:
		char 	p_dir[200]; 		// 백업대상 DIR
		char 	p_fname[50]; 		// 백업대상 화일
		int 	p_flen; 			
		
		char 	p_bakdir[200]; 		// 백업장소 DIR
		char 	p_bname[50];		// 백업화일명
		int		p_blen;
		
		int		bk_day;
		int		bk_delday;
		
		int 	CHK_PreDay(struct  tm 	*pTm); 	//백업대상인지 체크
		int 	CHK_PreDay(char *day);	      	//삭제대상인지 체크
      void  GetMonDay(char *mon, int &day, char *tmpDir);  //백업대상 월/일 리턴.
		
		void	CompressRun(void);			// 압축하기
	public:	
		/*
		pdir     : 백업대상 경로
		pfname   : 백업대상 화일 인식명 (ex> "RADIUS_UDR1.999" 이면 "RADIUS_UDR1" )
		pbakdir  : 백업할 경로
		pbname   : 백업된(tar)화일 인식명 (ex> "RA1_" 으로 주면 RA1_yyyymmdd.tar 로 명명됨)
		b_day    : 백업대상 날짜 (시스템날짜보다 몇일전이면 백업)
		b_delday : 백업된(tar)화일 삭제 지정 (시스템날짜보다 몇일전이면 삭제)	
		*/
		Udr_Backup(const char *pdir, const char *pfname, const char *pbakdir, const char *pbname, int b_day=2, int b_delday=7);
		~Udr_Backup(void){;}
		
		void Run(void);
};


//--------------------------------------------------------------------
Udr_Backup::Udr_Backup(const char *pdir, const char *pfname, const char *pbakdir, const char *pbname, int b_day, int b_delday)
{
	strcpy(p_dir, pdir);
	strcpy(p_fname, pfname);
	p_flen=strlen(pfname);
	
	strcpy(p_bakdir, pbakdir);
	strcpy(p_bname,  pbname);
	p_blen=strlen(pbname);
		
	bk_day = b_day;
	bk_delday = b_delday;
}


//--------------------------------------------------------------------
// 해당일 전일이면 1, 아니면 0
int Udr_Backup::CHK_PreDay(struct  tm *pTm)
{
	struct  tm 	*imsiTm;
	time_t	today;
	char	imsi1[10];
	char	imsi2[10];
	
	time(&today);	// 오늘날자 얻기
	today -= (86400 * bk_day);
	imsiTm = localtime(&today);
	
	
	sprintf(imsi1, "%04d%02d%02d", imsiTm->tm_year,imsiTm->tm_mon,imsiTm->tm_mday);
	sprintf(imsi2, "%04d%02d%02d", pTm->tm_year,pTm->tm_mon,pTm->tm_mday);
	
	
	if(strncmp(imsi1, imsi2, 8) >= 0) return 1;
	else return 0;
}

//--------------------------------------------------------------------
// 해당일 전일이면 1, 아니면 0
int Udr_Backup::CHK_PreDay(char *day)
{
	char		pday[10];
	struct  tm 	*Tm;
	time_t		today;
	
	time(&today);	// 오늘날자 얻기
	today -= (86400 * bk_delday);
	Tm = localtime(&today);
	sprintf(pday,"%04d%02d%02d", Tm->tm_year+1900, Tm->tm_mon+1, Tm->tm_mday);
	
	if(strncmp(day, pday, 8)<=0) return 1;
	else return 0;
}

// 백업대상 달과 날짜를 리턴한다. 2001. 6. 7. ky. 추가.
//----------------------------------------------------------
void Udr_Backup::GetMonDay(char *mon, int &day, char *tmpDir)
{
	struct timeb s;
	struct tm *curTm;
	
	ftime(&s);
	time_t t = s.time;
	
	t -= (86400 * bk_day);
	curTm = localtime(&t);
	
	sprintf(mon, "%s", MONTH[curTm->tm_mon]);
	day = curTm->tm_mday;
	
	// temp dir name을 리턴한다.
	// 같은 시각에 동시에 프로세스가 기동할 경우 Temp dir이 같으면
	// 백업파일들이 유실될 수도 있으므로
	// 유일한 dir name을 만든다.
	sprintf(tmpDir, "%02d%02d%02d%03d", curTm->tm_hour, curTm->tm_min, curTm->tm_sec, s.millitm);
}

//---------------------------------------------------------------------------
void Udr_Backup::Run(void)
{
	char    ftime[100];
	char    fname[200];
	char  	cmd[200];
	char	bakname[100];
	DIR*  	pDir;
	struct  dirent 	*dp;
	struct  stat 	finfo;
	time_t  t;
	struct  tm 		*Tm;
	
   (void) time(&t);
   t -= (86400 * bk_day);
   Tm = localtime(&t);
   sprintf(bakname, "%s/%s%04d%02d%02d.tar", p_bakdir, p_bname, Tm->tm_year+1900, Tm->tm_mon+1, Tm->tm_mday);
	
//	if((pDir=opendir(p_dir))==NULL)
//	{
//		printf("Opendir Error [%s]\n", p_dir);	
//		return;
//	}
//	
//	while ( (dp = readdir(pDir)) != NULL )		// 디렉토리 읽음.
//	{
//		if(strncmp(dp->d_name, p_fname, p_flen)) continue;  // 다른화일이면
//		
//		sprintf(fname, "%s/%s", p_dir, dp->d_name);
//		//printf("fname = %s\n", fname);
//		if(stat(fname, &finfo)<0) continue;	// 화일정보를 얻는다.
//		
//		//Tm = localtime(&finfo.st_mtime); // 화일의 일시를 구조체형식으로 변환	
//		memcpy((char *)&Tm, (char *)localtime(&finfo.st_mtime), sizeof(struct tm));
//		sprintf(bakname, "%s/%s%04d%02d%02d.tar", p_bakdir, p_bname, Tm.tm_year+1900, Tm.tm_mon+1, Tm.tm_mday);
//		printf("[%18s] %04d%02d%02d %02d:%02d:%02d \n",dp->d_name, Tm.tm_year+1900, Tm.tm_mon+1, Tm.tm_mday,
//	                                             Tm.tm_hour,      Tm.tm_min,   Tm.tm_sec );
//		
//		if(CHK_PreDay(&Tm)) 
//		{	// 백업대상이면 Tar 한다.
//			printf("[%s] %s backup..\n", p_dir, dp->d_name);
//			if(!access(bakname, F_OK))
//			{	// 백업화일이 존재하면..
//				sprintf(cmd, "tar rvf %s %s", bakname, fname);
//				system(cmd);
//			}else
//			{
//				sprintf(cmd, "tar cvf %s %s", bakname, fname);
//				system(cmd);
//			}
//			sprintf(cmd, "rm -f %s", fname);
//			system(cmd);
//		}
//		
//	} // end of while();
//	closedir(pDir);

   char strMonth[10];	 // 백업 대상 월
   int  nDay;				 // 백업 대상 일
   char strTmpDir[9+1];  // 백업 임시 디렉토리.
   char TmpDir[100];     // 임시 디렉토리명.
	char strCmd[512];     // system command
	
   GetMonDay(strMonth, nDay, strTmpDir);		// 백업 대상 월,일, 임시디렉토리명을 리턴받음.
   
   sprintf(TmpDir, "%s%s/", p_dir, strTmpDir);
   
	mkdir(TmpDir, 0755);
	chdir(TmpDir);

	// move ...
	sprintf(strCmd, "ls -l %s | col | grep %s | awk '{if(\"%s\"==$6) { if(%d==$7) { print \"mv %s\" $9 \" %s\" $9}}}' | sh -",
	p_dir, p_fname, strMonth, nDay, p_dir, TmpDir);
	system(strCmd);
	
	// tar ...
	sprintf(strCmd, "tar cf %s %s*", bakname, p_fname);
	system(strCmd);
	
	// delete ...
	sprintf(strCmd, "rm -f %s*", p_fname);
	system(strCmd);
	
	chdir("..");
	rmdir(TmpDir);
	
	//---------------
	// compress
	//sprintf(cmd, "compress %s", bakname);
	//system(cmd);
	CompressRun();	//sycho 2001.05.03
	//---------------
	
	
	//-------------
	if((pDir=opendir(p_bakdir))==NULL)
	{
		printf("Opendir Error [%s]\n", p_bakdir);	
		return;
	}
	
	while ( (dp = readdir(pDir)) != NULL )		// 디렉토리 읽음.
	{
		if(strncmp(dp->d_name, p_bname, p_blen)) continue;  // 다른화일이면
		sprintf(fname, "%s/%s", p_bakdir, dp->d_name);
		
		if(CHK_PreDay((char *)&dp->d_name[p_blen])) 
		{	// 삭제대상이면 del..
			printf("[DEL] %s \n", dp->d_name);
			sprintf(cmd, "rm -f %s", fname);
			system(cmd);
		}
		
	} // end of while();
	
	closedir(pDir);
}



//---------------------------------------------------------------------------
void Udr_Backup::CompressRun(void)
{
	char	cmd[200];
	char	strCmd[200];
	char	fname[100];
	char	fcheck[100];
	FILE	*fp;
	int     ret;
	
	
	sprintf(cmd, "ls %s%s*.tar", p_bakdir, p_bname);

	while(1)
	{
		memset(strCmd, 0x00, sizeof(strCmd));
		if((fp = popen(cmd, "r")) == NULL)
		{
			perror("popen err:");
			return ;
		}else
		{
			if(!fgets(strCmd, 200, fp)) 
			{
				pclose(fp);
				return ;
			} 
			strCmd[strlen(strCmd)-1]=0x00;
			strcpy(fname, strtok(strCmd, " "));
		}
	
		sprintf(fcheck, "%s.Z", fname);
		if(access(fcheck, F_OK)!=0)
		{	// 화일이 없으면 압축한다.
			sprintf(strCmd, "compress %s", fname);
			ret = system(strCmd);
			if (ret != 0) return ;	// compress 실패시 루프를 빠진다.
		}else
		{
			sprintf(strCmd, "mv %s %s.2", fcheck, fcheck);
			ret = system(strCmd);
			if (ret != 0) return ;	// compress 실패시 루프를 빠진다.
		}
	
		printf("\n %s \n", strCmd);
		pclose( fp );
		sleep(1);
	}
	
	return ;
	
}



//---------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	if(argc < 7)
	{
		
		cout << "udr_bak [DIR] [FNAME] [BACKDIR] [BACKNAME] [B_DAY] [B_DELDAY]" << endl;
		exit(1);	
	}
	
	int			b_day, b_delday;
	Udr_Backup 	*Ubackup;
	
	
	b_day = atoi(argv[5]);
	b_delday = atoi(argv[6]);
	
	Ubackup = new Udr_Backup(argv[1], argv[2], argv[3], argv[4], b_day, b_delday);
	
	Ubackup->Run();
	
	delete Ubackup;
	exit(1);
}


// 결과
// 기존 알고리즘의 경우 3-4000개의 파일을 백업하는데 대략 12시간 이상이 걸림.
// 수정 알고리즘의 경우 15000개의 파일을 백업하는데 11분 걸림.
// real    11:33.6
// user     1:42.5
// sys      3:05.0
//
//
// 주의 사항.
// 1. 기본적으로 백업대상 디렉토리와 백업디렉토리는 절대경로 이어야 함.
// 2. 쉘명령을 많이 사용하므로 운영환경이 변경될 경우 소스 수정 및 테스트가 필요함.
//    (백업 처리에만 해당됨. 예로 tar나 compress명령이 없을 경우.. 또는 ll이 alias로 잡혀있지 않은 경우,
//     그외 명령 패스가 설정되지 않은경우, access 권한이 없는 경우 등등 ...)
//
// 2001. 6. 7일 목요일 ky.
