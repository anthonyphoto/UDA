#ifndef _LUCENT_IF_H
#define _LUCENT_IF_H

#include "amatps_api.h"

#define	 AMA_SERVER_ADDR	"150.2.23.200"
#define	 AMA_DATA_PATH		"/SVC1/RADIUS/AMA"


/* Primary file header structure */
typedef struct
{
	/* file layer message header */
	u_char flhic;		/* file layer item and length	*/
	u_char fssno;		/* file send sequence number (0-255)*/
	u_char ftyp;		/* file type (not used)		*/
	u_char flen[3];		/* file length (excluding file hdr) in bytes	*/
	u_char fleicnam;	/* extended file layer item code for  file name	*/
	u_char fnamlen;		/*file name length in bytes	*/
	u_char fcode;		/*first character of file name	*/
	u_char fnumber[4];	/* remainder of file name	*/
	u_char fleicbks;	/* extended file layer item code for block number*/
	u_char fnblklen;	/* no. of bytes that follow this field	*/
	u_char nblks[4];	/* no. of blocks remaining in  primary file	*/
	u_char fleoh;		/*end of primary file header*/
	
} FHDRP; /* 20 */


/* Secondary file header structure */
typedef struct
{
	/* file layer message header */
	u_char flhic;		/* file layer item and length	*/
	u_char fssno;		/* file send sequence number (0-255)*/
	u_char ftyp;		/* file type (not used)		*/
	u_char flen[3];		/* file length (excluding file hdr) in bytes	*/
	u_char fleicnam;	/* extended file layer item code for file name	*/
	u_char fnamlen;		/*file name length in bytes	*/
	u_char fcode;		/*first character of file name	*/
	u_char fnumber[4];	/* remainder of file name	*/
	u_char fleoh;		/*end of secondary file header*/
	
} FHDRS; /* 14 */



/* AMA block Header */
typedef struct
{
	u_char seqnum[4];	/* block sequence number */
	u_char blksz[2];	/* block size including block header  but not fill characters */
	u_char recnum;		/* # AMA call records in block */
	u_char flag;		/* flag byte */
	u_char month;		/* month of year in BCD */
	u_char day;			/* day of month in BCD */
	u_char hour;		/* hour of day in BCD */
	u_char minute;		/* minute of hour in BCD */
	u_char genno;		/* generic number in BCD */
	u_char issno;		/* issue number in BCD */
} BLKHDR; /* 14 */

typedef struct 
{
	BLKHDR hdr;				/* Block header		 */
	u_char data[BLK_DATA];	/* AMA record(s)	 */
} AMABLK; /* 1531 */


class Lucent_IF
{
	
	private:
		int		scid;
		u_char	fileBuf[MAX_FILE_BUF]; 		// File buffer 			 
		char	OmpAddr[20];
		
		int 	char2int(u_char *ustr, int len);
		int		ProcessFile(int type);
		
	public:
		int		AMA_ERROR;				// ama rpc error (<0)  
		
		Lucent_IF(char *omp_address);	// rpc connect
		~Lucent_IF();					// rpc disconnect
		
		int		AmaConnect();
		void	AmaDisConnect(){ disconnect_rpc(); }	
		
		int		get_PrimaryData();
		int		get_SecondaryData(int startseq, int count);
		
		int		call_PollMaint();
		int		call_GetStats();
	
};

#endif
