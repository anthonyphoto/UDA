/*-------------------------------------------------------------------------
 * NAME:        amatp_api.h
 *
 * VERSION:	@(#)amatps_api.h	1.1.1.11 01/20/97
 *
 * DESCRIPTION: This file contains the defines and structures 
 *		associated with the BILLDATS AMATPS API.
 *
 * REFERENCES:  Requirements for BILLDATS AMATPS Continuous Polling
 *		API, Version 1, Issue 1.0
 *
--------------------------------------------------------------------------*/

#ifndef _AMATPS_API_H
#define _AMATPS_API_H


/*----------------------------------------------------------------------*
 *									*
 * The following is a complete list of all possible return codes	*
 * associated with the AMATPS API.					*
 *									*
 *----------------------------------------------------------------------*/
#define EAPIPKTFAIL		-2	/* Packet Layer Failure			*/
#define EAPINORESP		-3	/* No response from AMAT		*/
#define EAPIREJSESS		-4	/* AMAT rejected session		*/
#define EAPIBADCONF		-5	/* Bad or missing configuration file	*/
#define EAPIBADDEV		-6	/* Bad or missing device		*/
#define EAPINOTDEL		-7	/* Not delivered, still holding  response from prev			*/
#define EAPINOMSG		-8	/* No message available			*/
#define EAPIREJTEST		-9	/* AMAT rejected test file		*/
#define EAPIBADPRIM		-10	/* Invalid primary file returned by AMAT */
#define EAPIBADSTATE	-11	/* Incorrect state for ack_primary	*/
#define EAPIBADSEC		-12	/* Invalid secondary file returned by AMAT */
#define EAPIBADRANGE	-13	/* sbsn and ebsn are not a valid range	*/
#define EAPIBADMAINT	-14	/* Invalid maint file returned by AMAT	*/
#define EAPINOSTATS		-15	/* Statistics not available		*/
#define EAPINOTRESET	-16	/* Could not reset link stats		*/
#define EAPIBADACCEPT	-17	/* Invalid connect accept returned by AMAT */
#define EAPIBADRESP		-18	/* Bad response received from session level */
#define EAPISYSERR		-19	/* Unix operating system call error	*/
#define EAPIBADSEQ		-20	/* Bad starting sequence number		*/
#define EAPIDTEFAIL		-21	/* No response from local DTE node 	*/
#define EAPIINVAL		-22	/* Invalid Argument			*/
#define EAPITOSOON		-23	/* poll_primary() was called to soon after it returned with no data available */
#define EAPIBADPASSWD	-24	/* AMAT password mismatch		*/
#define EAPINOTSET		-25	/* Environment variable is not set 	*/
#define EAPIBADFILE		-26	/* Trace control file creation error	*/
#define EAPITOMANY		-27	/* Max number of sessions already established */
#define EAPITRUNC		-28	/* More than 100 blocks were requested via
				 			 * poll_secondary(). poll_secondary() 
				 			 * returned only the first 100 blocks.
				 			 */
#define EAPIBADTRACER	-29	/* Bad secondary tracer file returned */
#define EAPISESSDOWN	-30	/* Session is down (not connected) with AMAT */

#define EAPIBADSZBS		-40	/* poll_primary received a block seq number that it
				 			 * did not expect.  This is probably to due an
				 			 * incorrect value for the SZBS parm in the dcf.
				 			*/

/*
 * The RPC error returns are defined here so that they don't conflict
 * with the API returns
 */

#define EARRPCFAIL		-31	/* RPC call failed */
#define EARSBADDCF		-32	/* Bad DCF name */
#define EARSBADPW		-33	/* Bad Password */
#define EARSFSERR		-34	/* File error */
#define EARSNOCC		-35	/* Not Connected */
#define EARSCONNECT		-36	/* Already Connected */


/*----------------------------------------------------------------------*
 * Miscellaneous defines						                        *
 *----------------------------------------------------------------------*/

#define MAX_FILE_BUF	153120	/* Maximum buffer size to hold a 100 block
				 				 * file (1531 bytes/block) plus 
				 				 * the 20 byte file header.
				 				 */
#define SEC_TRACE_SIZE	70		/* Size of secondary tracer file, includes:
				 				 *  - 10 byte file header
				 				 *  - 60 byte tracer record
				 				 */

#define S_AMATPASS		5	/* length of AMAT password		*/
#define S_HOCPASS		5	/* length of Collector password		*/

#define ACK				0	/* Acknowledge (send a file confirm)	*/
#define NACK			1	/* Reject (send a file reject)		*/

/*
 * Session disconnect reasons (passed as second argument to
 * disconnect_session() function call).
 */
#define DS_NORMAL	0x00	/* Normal disconnect			*/
#define DS_ABORT	0x01	/* Abort: No additional info		*/


/*----------------------------------------------------------------------*
 *									*
 * Defines for the set_trace() function call				*
 *									*
 *----------------------------------------------------------------------*/
/*
 * Defines for trace_class. Note: various combinations may be or'ed
 * together.
 */
#define END_LOG		0x0000	/* Disable logging			*/
#define APPLIC_LOG	0x0002	/* Log application layer messages	*/
#define SESSION_LOG	0x0004	/* Log session layer messages		*/
#define DUAL_LOG	APPLIC_LOG|SESSION_LOG	
#define MONITOR_LOG	0x0010	/* Log monitor function messages	*/
#define PERFORM_LOG	0x0020	/* Log performance messages		*/
#define ALL_LOG		0xffff	/* Log all possible messages		*/


/*
 * Defines for trace_level.
 */
#define PROCESS_LEVEL	1	/* Output process level messages	*/
#define FUNCTION_LEVEL	2	/* Output function level messages	*/
#define DEBUG1_LEVEL	4	/* Output level 1 debug messages	*/
#define DEBUG2_LEVEL	5	/* Output level 2 debug messages	*/


/*----------------------------------------------------------------------*
 * Structure to pass AMAT specific parameters to the connect_session()	*
 * API function.							*
 *----------------------------------------------------------------------*/

typedef struct
{
	int	coll_passwd;	/* Collector password			*/
	int	sensor_type;	/* Switch office type			*/
	int	sending_unit;	/* AMAT Sending Unit Number		*/
	int	sensor_id;		/* Switch office id			*/
} acpstruct;



/*----------------------------------------------------------------------*
 * Following definitions apply to the maintenance file			*
 *----------------------------------------------------------------------*/
/*
 * Maintenance record.
 * Note: splitting the todisk and tocoll fields into low and high bytes
 *       is necessary for proper byte alignment.
 */
typedef struct
{
	u_char	mtc_mon;	/* month record written to disk		*/
	u_char	mtc_day;	/* day record written to disk		*/
	u_short	flag;		/* flag[16,.....1,0]
				 		 * bit 0: 0=previously sent, else 1
				 		 * bit 1: 1=record partially filled,
				 		 *        else 0
		 		 		 * bits 16-2: unused
						 */
	long	orig;		/* # Calls where dialing complete	*/
	long	complete;	/* # of completed calls			*/
	long	incomplete;	/* # of incomplete calls		*/
	u_short	mutilrec;	/* # of mutilated records		*/
	u_short	todisk_low;	/* # Records written to disk		*/
	u_short	todisk_high;
	u_short	tocoll_low;	/* # Records send to collector		*/
	u_short	tocoll_high;
	u_short	longdur;	/* # calls over 2 midnights             */
	u_short	totlost;	/* # records lost for any reason	*/
	u_short	minalrm;	/* # minor disk alarms 70%		*/
	u_short	majalrm;	/* # major disk alarms 90%		*/
	u_short	crtalrm;	/* # critical disk alarms 100%		*/
	long	spare1;		/* spare fields				*/
	long	spare2;
	u_short	spare3;
	u_short	crcerrs;	/* # times CRC check differs		*/
	u_short	hdlerrs;	/* # HDLC port hardware errors		*/
	u_short	timexp;		/* # times T1 idle link timer expired	*/
	long	frmsent;	/* # frames sent to collector		*/
	long	frmrec;		/* # frames received by AMAT		*/
	long	pacsent;	/* # packets sent to collector		*/
	long	pacrec;		/* # packets received by AMAT		*/
	u_short	framerr;	/* # frame errors & rejects		*/
	u_short	pacerr;		/* # packet errors & rejects		*/
} m_record;

/*
 * Maintenance file.
 */
#define	MTC_DAYS	7	/* Number of days in the maintenance file */

typedef struct 
{
	u_char		bh[8];	/* 8-byte block header. Contains no useful info
				 * Should have all bits set to 
				 * 1 (all bytes hex FF)
				 */
	m_record	mr[MTC_DAYS];  /* The seven maintenance records	*/
} m_file;


/*
 * The following structure is used to combine the low and high
 * bytes (todisk and tocoll) defined above into a long.
 */
union  mtc_sr
{
	long	low_high;
	struct
	{
		u_short	low;
		u_short	high;
	} send_rec;
};


/*
 * Statistics structure returned via get_link_stats()
 * Note: The structure of the following functions were originally
 *	 based upon the LAPB_accounting and pkt_accounting structures
 *	 defined in Adax/npi.h.
 */
typedef struct
{
        u_short in_bad_frame;	/* The bad frames received */
        u_short in_FRMR_frame;	/* The number of FRMR frames received */
        u_short num_T1_timeout;	/* The number of T1 timeouts */
        u_short in_REJ_frame;	/* The number of REJ frames received */
        u_short out_REJ_frame;	/* The number of REJ frames transmitted */
        u_short in_short_frame;	/* The number of frames shorter than 
				 				 * minimun length received */
        u_long in_frame;	/* The number of frames received */
        u_long out_frame;	/* The number of frames transmitted */

} LAPB_stats;

typedef struct
{
        u_long out_data_packet;		/* The total outgoing data packets */
        u_long in_data_packet;		/* The total incoming data packets */
        u_long out_data_packet_byte;	/* The total outgoing data packets in bytes */
        u_long in_data_packet_byte;	/* The total incoming data packets in bytes */
        u_long out_exdata_packet;	/* total outgoing expedited data packets */
        u_long in_exdata_packet;	/* total incoming expedited data  packets */
        u_long out_exdata_packet_byte;	/* total outgoing expedited data packets in bytes */
        u_long in_exdata_packet_byte;	/* total incoming expedited data packets in bytes */
        u_long out_call_packet;		/* The total outgoing call packets */
        u_long in_call_packet;		/* The total incoming call packets */
        u_long out_clear_packet;	/* The total outgoing clear packets */
        u_long in_clear_packet;		/* The total incoming clear packets */
        u_long out_reset_packet;	/* The total outgoing reset packets */
        u_long in_reset_packet;		/* The total incoming reset packets */
        u_long out_restart_packet;	/* The total outgoing restart packets */
        u_long in_restart_packet;	/* The total incoming restart packets */
        u_long linkup_packet;		/* The total linkup packets */
        u_long linkdown_packet;		/* The total linkdown packets */
        u_char R_State;
} PLP_stats;


typedef struct 
{
	LAPB_stats	LAPB;		/* Link Layer stats 		*/
	PLP_stats	pkt;		/* Packet Layer Protocol stats	*/
} x25_stat_buf;

/*----------------------------------------------------------------------*
 *									*
 * Following definitions are used by the expand_ama_rec() function	*
 * which performs BAF call record data expansion.			*
 *									*
 *----------------------------------------------------------------------*/
#define BLK_DATA        1517    /* Max number of bytes of call record data within one block */
#define PRIMARY_CALL	0x00	/* Passed to fn and inserted as part of the sensor_id */
#define SECONDARY_CALL	0x10	/* Passed to fn and inserted as part of the sensor_id */
#define MULTISWITCHED	1		/* AMAT is interfacing with more than one switch */
#define UNISWITCHED		0		/* AMAT is interfacing with one switch */

/*
 * The common portion of an AMA billing record header.
 */
typedef struct
{
	u_char	reclen[2];		/* Record length		*/
	u_char	spanned[2];		/* Spanning record		*/
    u_char	validrec;       /* valid record indicator	*/
    u_char	strcode[3];     /* structure code		*/
    u_char	ctype[2];       /* call type			*/
} RECHDR; /* 10 */

/*
 * The structure of an unexpanded AMA billing record.
 */
typedef struct
{
	RECHDR hdr;				 				 /* Record header */
    u_char data[(BLK_DATA-sizeof(RECHDR) )]; /* rest of record (variable length) */
} AMAREC; /* 1517 */


/*
 * The structure of an expanded AMA billing record 
 * (result of expand_ama_rec() function call).
 */
typedef struct
{
	RECHDR hdr;						/* common header */
	u_char sensor_type[2];			/* Switch sensor type */
	u_char sensor_id[4];			/* Switch sensor identification */
	u_char office_type[2];			/* Recording Office type */
	u_char office_id[4];			/* Recording Office identification */
	u_char data[(BLK_DATA-sizeof(RECHDR))];	/* common data */
} EXPAND_AMAREC;



/*----------------------------------------------------------------------*
 * Function Declarations						*
 *----------------------------------------------------------------------*/
#ifdef  __cplusplus
extern "C" {
#endif


#if ( defined(__STDC__) || defined( __cplusplus )) && !defined(_NO_PROTOTYPE)

/* client.c */
int connect_rpc( char *host, int timeout );
void disconnect_rpc();
/* client.c */

/*
 * API Function Declarations: File (Application) Level
 */
int	poll_primary( int, u_char *);
int	ack_primary( int, int);
int	poll_maint( int, m_file *);
int	test_connection( int, int);
int	poll_secondary( int, u_char *, u_char *, long, long);


/*
 * API Function Declarations: Session Level
 */
int	connect_session( char *, acpstruct );
int	disconnect_session( int, int );
int	ssend( int, int, u_char *, int );
int	srecv( int, int *, u_char *, int);


/*
 * API Function Declarations: Monitor/Trace 
 */
int	get_link_stats( int, x25_stat_buf *);
int	get_link_status( int );
int	reset_link_stats( int );
int	set_trace( int, int, char *, long );

/*
 * API Function Declarations: Miscellaneous
 */
int	char2int( u_char *, int);
FILE    *open_dcf( char * );
char    *get_dcf_parm( FILE *,  char *);
int     int2bcd(int, int, u_char *);
short	expand_ama_rec( AMAREC *, char *, int, int, long, int, long, int);

#else
/*
 * API Function Declarations: File (Application) Level
 */
int	poll_primary();
int	ack_primary();
int	poll_maint();
int	test_connection();
int	poll_secondary();


/*
 * API Function Declarations: Session Level
 */
int	connect_session();
int	disconnect_session();
int	ssend();
int	srecv();


/*
 * API Function Declarations: Monitor/Trace 
 */
int	get_link_stats();
int	get_link_status();
int	reset_link_stats();
int	set_trace();

/*
 * API Function Declarations: Miscellaneous
 */
int	char2int();
FILE    *open_dcf(); 
char    *get_dcf_parm(); 
int     int2bcd(); 
short	expand_ama_rec();

#endif

#ifdef  __cplusplus
}
#endif


#endif
