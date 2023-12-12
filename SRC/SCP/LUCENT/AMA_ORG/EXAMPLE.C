/*-------------------------------------------------------------------
 *
 *      Copyright (c) 1995 AT&T
 *        All Rights Reserved 
 *
 *      THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T     
 *      The copyright notice above does not evidence any       
 *      actual or intended publication of such source code.   
 *
 --------------------------------------------------------------------*/

static char SccsID[]="@(#)example.c	1.1.1.7       23 Jan 1995";

/*
 * FILENAME:    example.c
 *
 * DESCRIPTION: This file contains example source code for using
 *		the BILLDATS AMATPS API shared library.
 *
 */
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stropts.h>

#include "amatps_api.h"
#include "example.h"

#define PRIM	0	/* Primary file */
#define	SEC	1	/* Secondary file */

main(argc,argv)
int argc;
char *argv[];
{
	register	i;

	int		scid,		/* Session connection identifier */
			ret,		/* function return value	 */
			timeoutval;
	acpstruct	acparms;	/* Collector/AMAT specific parms */
	u_char		fb[MAX_FILE_BUF]; /* File buffer 			 */


	
	timeoutval=1000; /* Hardcode for tests */
	if(argc<3)
	{
		fprintf(stderr,"Usage: %s OMP_name_or_address file_name start_seqnum count\n",argv[0]);
		return -1;
	}
	
	if((ret=connect_rpc(argv[1], timeoutval))<0)
	{
		fprintf(stderr, "connect rpc failed (%d)\n",ret);
		return -1;
	}else	fprintf(stderr,"connect rpc completed (%d)\n",ret);
	
	
	/*
	 * Setup tracing. 
	 */
	ret = set_trace( ALL_LOG, DEBUG2_LEVEL, "AMATPS.", 500000 );
	printf("set_trace(): ret=%d\n", ret);


	/*
	 * Setup the AMAT specific parameters.
	 */
	 
	 /*
	acparms.coll_passwd = 123456;
	acparms.sending_unit = 0;
    acparms.sensor_type = 9;
    acparms.sending_unit = 0;
    acparms.sensor_id = 123456;
    */
   
   
   	acparms.coll_passwd=320000001;/* Collector password */
	acparms.sensor_type=46; /* Switch office type */

	acparms.sending_unit=0; /* AMAT Sending Unit Number */
	acparms.sensor_id=1; /* Switch office id */
	
	/*
	 * Try to connect up to three times.
	 */
	for( i=0; i < 3; i++ )
	{
		if ((scid = connect_session("adax.0.1", acparms)) > 0) break; 
		/* if ((scid = connect_session("dci.0", acparms)) > 0) break; */
	}

	if (scid < 0) 
	{
		printf("Could not connect, scid=%d\n", scid);
		if((ret=reset_server("arpcs01"))<0)
			fprintf(stderr, "reset_server failed (%d)\n",ret);
		disconnect_rpc();
		exit(-1);
	}

    printf("connect_session() succeeded, ret=%d\n", scid);


	/*
	 * Perform a test file poll.
	 */
	ret = test_connection(scid, 10);
	printf("test_connection return = %d\n", ret);
         

	/*
	 * Poll for primary data as long as data exists.
	 */
	ret = 0;
	while ( !ret )
	{
		ret = poll_primary(scid, fb);

		printf("poll_primary(): return=%d\n", ret);

        	if ( ret == 1)
        	{
                	printf("No data available\n");
			break;
        	}

		if ( ret < 0 )
		{
			break;	/* error */
		}

		/*
		 * Received a primary file.
		 * Acknowledge the primary file.
		 */
               	if ( ack_primary(scid, ACK) != 0 )
		{
			break;	/* error */
		}

		/*
		 * Process the data...
		 */
		process_file(fb, PRIM);
	}


	/*
	 * Poll for secondary data.
	 */
        ret = poll_secondary(scid, fb, NULL, 0, 0);
	printf("poll_secondary(): return=%d\n", ret);

	if ( ret == 0 )
	{
		/*
		 * Process the data.
		 */
		process_file(fb, SEC);
	}


	/*
	 * Poll for the maintenance file.
	 */
	call_poll_maint(scid);

 
	/*
	 * Get the link statistics.
	 */
	call_get_stats(scid);


	/*
	 * Get the link status.
	 */
	ret = get_link_status(scid);
	printf("get_link_status(): return=%d\n", ret);


	/*
	 * Reset the link statistics.
	 */
	ret = reset_link_stats(scid);
	printf("reset_link_stat(): return=%d\n", ret);
    
	sleep(3);


	/*
	 * Disconnect the polling session.
	 */
	ret = disconnect_session(scid, 0);
        printf("disconnect_session() return = %d\n", ret);

	exit(0);

}


int
call_poll_maint( scid )
int scid;
{
	register	i;
	int		ret;
	u_char		*ptr;
	m_file		mf;
	m_record	*m;
	union mtc_sr	todisk, tocoll;

	if ( (ret = poll_maint(scid, &mf)) != 0)
	{
		printf("poll_maint(): ret=%d\n", ret);
		return(ret);
	}

	printf("Maintenance Records:\n");

	for (i=0; i< MTC_DAYS; i++)
	{
		m = &mf.mr[i];

		todisk.send_rec.low = m->todisk_low;
		todisk.send_rec.high = m->todisk_high;
		tocoll.send_rec.low = m->tocoll_low;
		tocoll.send_rec.high = m->tocoll_high;

		printf("\n*** Record %d:\n", i);

        	printf("\tDate of Record: %.2x/%.2x\n",
                 	m->mtc_mon, m->mtc_day);
        	printf("\tFlag                                 0x%.8x\n", m->flag);
        	printf("\tCalls Where Dialing Complete         %ld\n", m->orig);
        	printf("\tCompleted Calls                      %ld\n", m->complete);
        	printf("\tIncomplete Calls                     %ld\n", m->incomplete);
        	printf("\tMutilated Records                    %d\n", m->mutilrec);
        	printf("\tRecords Written To Disk              %ld\n", 
				todisk.low_high);
        	printf("\tRecords Sent To Collector            %ld\n", 
				tocoll.low_high);
        	printf("\tCalls Over 2 Midnights               %d\n", m->longdur);
        	printf("\tRecords Lost For Any Reason          %d\n", m->totlost);
        	printf("\tNumber Minor Disk Alarms (70%%)       %d\n", m->minalrm);
        	printf("\tNumber Major Disk Alarms (90%%)       %d\n", m->majalrm);
        	printf("\tNumber Critical Disk Alarms (100%%)   %d\n", m->crtalrm);
        	printf("\tNumber Collector CRC Errors          %d\n",  m->crcerrs);
        	printf("\tHDLC Port Errors	             %d\n",  m->hdlerrs);
        	printf("\tTimes T1 Idle Link Timer Expired     %d\n",  m->timexp);
        	printf("\tFrames Sent To Collector             %ld\n", m->frmsent);
        	printf("\tFrames Received From Collector       %ld\n", m->frmrec);
        	printf("\tPackets Sent To Collector            %ld\n", m->pacsent);
        	printf("\tPackets Received From Collector      %ld\n", m->pacrec);
        	printf("\tFrame Errors & Rejects               %d\n",  m->framerr);
        	printf("\tPacket Errors & Rejects              %d\n",  m->pacerr);

	}

	
	return(0);

}



int
call_get_stats( scid )
int scid;
{
	int		ret;
	x25_stat_buf	sb;

	if ( (ret = get_link_stats( scid, &sb )) != 0)
	{
		printf("get_link_stats(): ret=%d\n", ret);
		return(ret);
	}

	printf("\nLAPB STATISTICS\n");
	printf("Number of bad frames received %d\n", sb.LAPB.in_bad_frame);
	printf("Number of FRMR frames received %d\n", sb.LAPB.in_FRMR_frame);
	printf("Number of T1 timeouts %d\n", sb.LAPB.num_T1_timeout);
	printf("Number of REJ frames received %d\n", sb.LAPB.in_REJ_frame);
	printf("Number of REJ frames transmitted %d\n", sb.LAPB.out_REJ_frame);
	printf("Number of short frames received %d\n", sb.LAPB.in_short_frame);
	printf("Number of frames received %ld\n", sb.LAPB.in_frame);
	printf("Number of frames transmitted %ld\n", sb.LAPB.out_frame);


	printf("\nPACKET STATISTICS\n");
	printf("Number of outgoing data packets %d\n", 
		sb.pkt.out_data_packet);
	printf("Number of incoming data packets %d\n", 
		sb.pkt.in_data_packet);
	printf("Number of outgoing data bytes %d\n", 
		sb.pkt.out_data_packet_byte);
	printf("Number of incoming data bytes %d\n", 
		sb.pkt.in_data_packet_byte);
	printf("Number of outgoing expedited packets %d\n", 
		sb.pkt.out_exdata_packet);
	printf("Number of incoming expedited packets %d\n", 
		sb.pkt.in_exdata_packet);
	printf("Number of outgoing expedited bytes %d\n", 
		sb.pkt.out_exdata_packet_byte);
	printf("Number of incoming expedited bytes %d\n", 
		sb.pkt.in_exdata_packet_byte);
	printf("Number of outgoing call packets %d\n", 
		sb.pkt.out_call_packet);
	printf("Number of incoming call packets %d\n", 
		sb.pkt.in_call_packet);
	printf("Number of outgoing clear packets %d\n", 
		sb.pkt.out_clear_packet);
	printf("Number of incoming clear packets %d\n", 
		sb.pkt.in_clear_packet);
	printf("Number of outgoing reset packets %d\n", 
		sb.pkt.out_reset_packet);
	printf("Number of incoming reset packets %d\n", 
		sb.pkt.in_reset_packet);
	printf("Number of outgoing restart packets %d\n", 
		sb.pkt.out_restart_packet);
	printf("Number of incoming restart packets %d\n", 
		sb.pkt.in_restart_packet);
	printf("Number of linkup packets %d\n", 
		sb.pkt.linkup_packet);
	printf("Number of linkdown packets %d\n", 
		sb.pkt.linkdown_packet);
	printf("R_State  %d\n", 
		sb.pkt.R_State);

	return(0);

} /* call_get_stats() */


//-----------------SYCHO----------------------
int	char2int( u_char *ustr, int len)
{
	u_char imsi[10];
	memset(imsi, 0x00, sizeof(imsi));
	memcpy(imsi, ustr, len);
	return atoi(imsi);	
}
//--------------------------------------------


/*
 * DESCRIPTION:	This function processes either a primary or
 *		a secondary billing file.
 */
int
process_file( fb, type )
u_char *fb;
int	type;	/* PRIM or SEC */
{
	register i,
		 j,
		 rec;

	short	blksz,		/* block size				  */
		reclen;

	int	offset;

	long	flen,		/* file length, excluding the file header */
		seqnum;		/* block sequence number		  */

	FHDRP	*fhdrp;		/* Pointer to the file header and the first
				 * AMA block in the file.
				 */
	FHDRS	*fhdrs;		/* Pointer to the file header and the first
				 * AMA block in the file.
				 */

	AMABLK	*amablk;	/* Pointer to an AMA block */
	AMAREC	*amarec;	/* Pointer to an AMA record */


	if ( type == PRIM )
	{
		fhdrp = (FHDRP *) fb;

		/*
		 * Get the file length.
		 * NOTE: char2int() is provided with the AMATPS API library.
		 */
		flen = char2int(fhdrp->flen, 3);
		offset = sizeof( FHDRP );
	}
	else
	{
		fhdrs = (FHDRS *) fb;

		/*
		 * Get the file length.
		 * NOTE: char2int() is provided with the AMATPS API library.
		 */
		flen = char2int(fhdrs->flen, 3);
		offset = sizeof( FHDRS );
	}

	/*
	 * Set the pointer to the first block after the file header.
	 */
	i = offset;

	printf("File: length=%d\n", flen);


	/*
	 * Process each block in the file.
	 */
	while( i < (flen + offset) )
	{
		amablk = (AMABLK *) &fb[i];

		memcpy(&seqnum, amablk->hdr.seqnum, 4);
		memcpy(&blksz, amablk->hdr.blksz, 2);

		printf("Block: seqnum=%d, length=%d, records=%d\n",
			seqnum, blksz,
			amablk->hdr.recnum );
		
		/*
		 * Process each record in the block.
		 */
		for(rec=0, j=0; rec < (int)amablk->hdr.recnum; rec++ )
		{
			amarec = (AMAREC *) &amablk->data[j];

			memcpy(&reclen, amarec->hdr.reclen, 2);

			printf("\tRecord %d: length = %d, structure=%.2x%.2x%.2x\n",
				rec, reclen,
				amarec->hdr.strcode[0],
				amarec->hdr.strcode[1],
				amarec->hdr.strcode[2] );

			/*
			 * Skip to the next record.
			 */
			j += reclen;

		}
		
		/*
		 * Skip to the next block
		 */
		i += sizeof(AMABLK);
	}
	
	return(0);
	
}


/*-------------------------------------------------------------------
 *
 *      Copyright (c) 1995 AT&T
 *        All Rights Reserved 
 *
 *      THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T     
 *      The copyright notice above does not evidence any       
 *      actual or intended publication of such source code.   
 *
 --------------------------------------------------------------------*/
