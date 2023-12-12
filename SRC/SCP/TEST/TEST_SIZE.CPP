//#include <scpformat.hpp>
#include <stdio.h>

const int       LEN_DATE    = 20;
const int       LEN_TIME    = 20;
#define BLK_DATA        1517    /* Max number of bytes of call record data within one block */

/*
 * Primary file header structure
 */
typedef struct
{
	/* file layer message header */
	u_char flhic;		/* file layer item and length	*/
	u_char fssno;		/* file send sequence number (0-255)*/
	u_char ftyp;		/* file type (not used)		*/
	u_char flen[3];		/* file length (excluding file hdr)
				 * in bytes	*/
	u_char fleicnam;	/* extended file layer item code for 
				 * file name	*/
	u_char fnamlen;		/*file name length in bytes	*/
	u_char fcode;		/*first character of file name	*/
	u_char fnumber[4];	/* remainder of file name	*/
	u_char fleicbks;	/* extended file layer item code
				 * for block number*/
	u_char fnblklen;	/* no. of bytes that follow this
				 * field	*/
	u_char nblks[4];	/* no. of blocks remaining in 
				 * primary file	*/
	u_char fleoh;		/*end of primary file header*/
	
} FHDRP;


/*
 * Secondary file header structure
 */
typedef struct
{
	/* file layer message header */
	u_char flhic;		/* file layer item and length	*/
	u_char fssno;		/* file send sequence number (0-255)*/
	u_char ftyp;		/* file type (not used)		*/
	u_char flen[3];		/* file length (excluding file hdr)
				 * in bytes	*/
	u_char fleicnam;	/* extended file layer item code for 
				 * file name	*/
	u_char fnamlen;		/*file name length in bytes	*/
	u_char fcode;		/*first character of file name	*/
	u_char fnumber[4];	/* remainder of file name	*/
	u_char fleoh;		/*end of secondary file header*/
	
} FHDRS;



/*
 * AMA block Header
 */
typedef struct
{
	u_char seqnum[4];	/* block sequence number */
	u_char blksz[2];	/* block size including block header
				   but not fill characters */
	u_char recnum;		/* # AMA call records in block */
	u_char flag;		/* flag byte */
	u_char month;		/* month of year in BCD */
	u_char day;		/* day of month in BCD */
	u_char hour;		/* hour of day in BCD */
	u_char minute;		/* minute of hour in BCD */
	u_char genno;		/* generic number in BCD */
	u_char issno;		/* issue number in BCD */
} BLKHDR;

typedef struct 
{
	BLKHDR hdr;		/* Block header		 */
	u_char data[BLK_DATA];	/* AMA record(s)	 */
} AMABLK;


/*
 * The common portion of an AMA billing record header.
 */
typedef struct
{
	u_char	reclen[2];	/* Record length		*/
	u_char	spanned[2];	/* Spanning record		*/
        u_char	validrec;       /* valid record indicator	*/
        u_char	strcode[3];     /* structure code		*/
        u_char	ctype[2];       /* call type			*/
} RECHDR;

/*
 * The structure of an unexpanded AMA billing record.
 */
typedef struct
{
	RECHDR hdr;				 /* Record header */
        u_char data[(BLK_DATA-sizeof(RECHDR) )]; /* rest of record (variable length) */
} AMAREC;


enum MSG_TYPE 		{	MSG_STATUS, 		//	Msg Type : To PMS
						MSG_LOG,			//	Msg Log
						MSG_ERR,			//	Msg Error
						MSG_STATS_PERFORM,	//	Msg Perform
						MSG_STATS_EVENT,		//	Msg Event
						MSG_STATS_ERROR		//	Msg Stats of Error
					};

enum FLAG_STATUS 	{	FLAG_NW_NORMAL = -4,
						FLAG_NW_ERROR,
						FLAG_PROC_ABNORMAL,
						FLAG_PROC_KILL,
						FLAG_PROC_TERM,
						FLAG_PROC_START
					};

enum FLAG_PERFORM	{	FLAG_START,
						FLAG_END,
						FLAG_ACC
					};


//-------------------------------------------------------------------------------------
class	HeaderOfMessage
{
	private:
		int		nMsgType;		//  Message Type
		int		nSystemId;		//	SYSTEM ID
		int		nSvcId;			//	SERVICE ID
		int		nGrpType;		//	GROUP ID
		int		nProcType;		//	PROCESS TYPE
		int		nProcSerial;	//	PROCESS SERIAL NUMBER
		int		nProcRemark;	//	PROCESS REMARK
		char	strDate[LEN_DATE+1];
		char	strTime[LEN_TIME+1];

	public:
		HeaderOfMessage()
		{
			nMsgType	= 0;
    		nSystemId	= 0;
    		nSvcId		= 0;
    		nGrpType	= 0;
    		nProcType	= 0;
    		nProcSerial	= 0;
    		nProcRemark	= 0;
		}

};


//-------------------------------------------------------------------------------------
class	MsgPmsStatus	: public HeaderOfMessage
{
	private:
		int		nPID;
		int		nStatus;

	public:
		
		MsgPmsStatus()	{	;	}	//	Message Queue Template을 위해 추가

};

class	MsgLog	: public HeaderOfMessage
{
	private:
		int		nErrorCode;
		char	strMsg[256];

	public:

		MsgLog()	{	;	}	//	Message Queue Template을 위해 추가


};


//-------------------------------------------------------------------------------------
class	ProcPerformData	: public HeaderOfMessage
{
	private:
		int				nBillDataType;
		unsigned int	unFileSeq;
		char			strProcId[9];
		unsigned int	nUdrCount;
		unsigned int	nFailUdrCount;						//smy 2000.11.25
		unsigned int	nQuery_Count;
		char			strStartDateTime[15];
		char			strEndDateTime[15];
		int				nACCStartTime;

	public:
		ProcPerformData(){;}

};


//-------------------------------------------------------------------------------------
class	ProcEventData	: public HeaderOfMessage
{
	private:
		char	strEventDateTime[15];
		int		nExitCode;

	public:
		
		ProcEventData()	{	;	}	//	Message Queue Template을 위해 추가
};


//-------------------------------------------------------------------------------------
class	ProcErrInfo	: public HeaderOfMessage
{
	private:
		char	strDayTime[30];
		char	strEventDate[9];
		char	strEventTime[7];
		char	strErrCode[5];
		char	strFileName[40];
		char	strErrDetail[512];

	public:
		ProcErrInfo()	{	;	}	//	Message Queue Template을 위해 추가

};





int main(void)
{
	
	//printf("SCPFormatAgg=%d\n", sizeof(SCPFormatAgg));
	//printf("CustFormat=%d\n", sizeof(CustFormat));
	
	/*
	printf("FHDRP = %d\n", sizeof(FHDRP));
	printf("FHDRS = %d\n", sizeof(FHDRS));
	printf("BLKHDR = %d\n", sizeof(BLKHDR));
	printf("AMABLK = %d\n", sizeof(AMABLK));
	printf("RECHDR = %d\n", sizeof(RECHDR));
	printf("AMAREC = %d\n", sizeof(AMAREC));
	*/
	
	printf("ProcErrInfo = %d\n", sizeof(ProcErrInfo));
	printf("ProcPerformData=%d\n",sizeof(ProcPerformData));
}
