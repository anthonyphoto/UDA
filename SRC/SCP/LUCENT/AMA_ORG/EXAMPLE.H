/*-------------------------------------------------------------------
 *
 *      Copyright (c) 1997 Lucent Technologies Inc.
 *        All Rights Reserved 
 *
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF LUCENT TECHNOLOGIES INC.
 *      The copyright notice above does not evidence any       
 *      actual or intended publication of such source code.   
 *
 --------------------------------------------------------------------*/


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

/*-------------------------------------------------------------------
 *
 *      Copyright (c) 1997 Lucent Technologies Inc.
 *        All Rights Reserved 
 *
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF LUCENT TECHNOLOGIES INC.
 *      The copyright notice above does not evidence any       
 *      actual or intended publication of such source code.   
 *
 --------------------------------------------------------------------*/

