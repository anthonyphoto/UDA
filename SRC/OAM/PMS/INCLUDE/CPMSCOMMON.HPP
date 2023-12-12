/*******************************************************************************
* Version 1.1 ( 2001. 05. 21 )
*******************************************************************************/
#ifndef _PMSCOMMON_
#define _PMSCOMMON_

#include <ctype.h>
#include <stdio.h>
#include <iostream.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
//#include <SendLogManage.hpp>

#define NAME		'N'
#define ID			'I'

// pms command
#define START		'S'
#define STOP		'T'
#define OPERATION   'O'

#define PAUSE		'P'
#define WAKE		'W'
#define STATUS		'C'

// pms limit
#define ALL			'A'
#define GROUP		'G'
#define ONE			'O'
#define SELECT		'S'
#define UPDATE		'U'
#define DISK		'D'
#define CPU			'C'
#define LOGMANAGER  'L'
#define USERMANAGEMENT 'M'

// process status
#define INIT        'I'
#define RUNNING		'S'
#define STOPPED		'T'
#define KILLED		'K'
#define ABNORMAL	'A'
#define ERRNETWORK  'E'
#define REGNETWORK  'R'
#define PAUSED		'P'

// HA의 동작 여부를 알 수 있는 플래그 파일.
#define HASTARTFLAGFILE "/etc/serviceMaster/bin/start.haflag"

// PMS 시작 시간 저장 파일
#define D_PMS_START_FILENAME "/var/SNMP/CONF/StartTime.cfg"

// data파일 초기화 오브젝트
#define D_INIT_FILE_COMMAND  "/var/SNMP/SCRIPT/initfile"

// TRAP
#define D_TRAP_ON                        1  // trap value on
#define D_TRAP_OFF                       0  // trap value off
#define D_UDA_TRAP_SERVICE_START	      100  // uda trap service start.      trap number.
#define D_UDA_TRAP_SERVICE_STOP	      101  // uda trap service stop.       trap number.
#define D_UDA_TRAP_SERVICE_TAKE_OVER   104  // uda trap service take-over.  trap number.
#define D_UDA_TRAP_PROCESS_UP          113  // uda trap process up(run).    trap number.
#define D_UDA_TRAP_PROCESS_DOWN      10007  // uda trap process down(stop). trap number.
#define D_UDA_TRAP_PROXY_FAIL         5002  // uda trap proxy fail.         trap number.




// PMS command structure
typedef struct _command
{
	char	command;			// (S)tart, (T)erminate, (O)peration
	char	type;				// (A)ll, (G)roup, pr(O)cess, (S)elect, (U)pdate, (D)isk, (C)pu
	char	subContent[10+1];
	char	date	  [14+1];	// client date
	char	filePart;			// 파일 구분	
	char	item1     [10+1];
	char	item2     [10+1];
	char	item3     [10+1];
	char	item4     [10+1];
	char	item5     [10+1];
	char	item6     [10+1];
} command_t, *pcommand_t;

// process infomation list structure
typedef struct _ProcInfo
{
	char	prog_name	[12+1];	// program name
	char	prog_path	[50+1];	// program path
	char	proc_name	[12+1];	// process name
	int		sys_id;				// system type   uda1 or uda2
	int		svc_id;				// service type  1 or 2 or ...
	int		group_type;			// group id
	int		proc_type;			// process type
	int		proc_sn;			// process serial
	int		proc_remark;		// process remark
	int		ip_cnt;				// ip count
	int		channel_cnt;		// channel count
	char	p_ipaddr1	[16+1];	// IP ADDRESS 1
	int		p_send1;			// SEND 1
	int		p_recv1;			// RECIEVE 1
	char	p_ipaddr2	[16+1];	// IP ADDRESS 2
	int		p_send2;			// SEND 2
	int		p_recv2;			// RECV 2
	char	proc_status;		// process status S:Running, T:Stopped, K:killed, A:Abnormal
	char	net_status;			// network status E:Error network, R:regular network
	int		sys_pid;			// process pid
	char	proc_run_time[14+1]; // process run time
	int		runCnt;				// start cnt  5이상이면 모든 프로세스 종료후 PMS종료.
	int  	exitFlag;			// 0일경우 외부종료, 1일경우 PMS종료.
} PROCINFO, *PPROCINFO;

// backup file infomation structure
typedef struct _BackInfo
{
//	int  sys_id;				// system id (uda1, uda2)
//	char backup_id	[  5+1];	// backup id
	char srcDir     [100+1];
	char srcFile	[ 20+1];	// source       file path & name
	char dstDir     [100+1];
	char dstFile	[ 20+1];	// destination  file path & name
	int  bak_day;				// backup day
	int  bak_delday;			// backup delete day
	char bak_time	[6+1];		// backup time
	bool procFlag;				// 처리 완료 Flag true면 처리, false면 미처리
} BackInfo, *pBackInfo;

/**************************************
//PMSMQueue용 STRUCTURE
struct _DATANODE
{
	int		sys_id;				// MQHeader | system type   uda1 or uda2
	int		svc_id;				// MQHeader | service type  1 or 2 or ...
	int		group_type;			// MQHeader | group id
	int		proc_type;			// MQHeader | process type
	int		proc_sn;			// MQHeader | process serial
	int		proc_remark;		// MQHeader | process remark
	int		msg_type;			// MQHeader | msg type
	int		pid;				// MQData   | pid
	int		status_flag;		// MQData   | status flag
};

class	DataNodePMS	: public _DATANODE
{
	public:
		operator struct _DATANODE* () const { return (_DATANODE*)this; }
};
**************************************/

bool DebugLog(char* file , char* format, ...);
bool ErrorLog(int errCode, char* format, ...);

#endif
