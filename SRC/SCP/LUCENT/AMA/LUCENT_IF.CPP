#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stropts.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <time.hpp>
#include "lucent_if.hpp"

#define PRIM	0	/* Primary file */
#define	SEC		1	/* Secondary file */



//-------------------------------------------------------------------------------
Lucent_IF::Lucent_IF(char *omp_address)
{
	strcpy(OmpAddr, omp_address);
	AmaConnect();
}


//-------------------------------------------------------------------------------
Lucent_IF::~Lucent_IF()
{
	disconnect_rpc();	
}


//-------------------------------------------------------------------------------
int	Lucent_IF::AmaConnect()
{
	int			i;
	int			timeoutval=1000;
	acpstruct	acparms;				// Collector/AMAT specific parms 

	// rpc connect
	if((AMA_ERROR=connect_rpc(OmpAddr, timeoutval))<0)
	{
		cout << "Connect RPC Failed : "<< AMA_ERROR << endl;
		return 0;
	}else cout << "Connect RPC Completed : "<< AMA_ERROR << endl; 
	
	
	// Setup tracing. 
	i = set_trace( ALL_LOG, DEBUG2_LEVEL, (char *)"AMATPS.", 500000 );
	cout << "set_trace() : " << i << endl;

	
	// acparms set
    acparms.coll_passwd=320000001;	// Collector password 
	acparms.sensor_type=46; 		// Switch office type 
	acparms.sending_unit=0; 		// AMAT Sending Unit Number 
	acparms.sensor_id=1; 			// Switch office id 
   
	
	// Try to connect up to three times.
	for( i=0; i < 3; i++ )
	{
		if ((scid = connect_session((char *)"adax.0.1", acparms)) > 0) break; 
		// if ((scid = connect_session("dci.0", acparms)) > 0) break; 
	}	
	
	
	if(scid < 0) 
	{
		AMA_ERROR = scid;
		cout << "Could not connect : scid : " << scid << endl;
		disconnect_rpc();
		return 0;
	}else
	{
		AMA_ERROR = 0;
		cout << "connect_session() succeeded : scid : " << scid << endl;		
		return 1;
	}
}


//-------------------------------------------------------------------------------
int Lucent_IF::get_PrimaryData()
{
	if(AMA_ERROR) return 0;
	
	int ret=0;
	
	
	while(!ret)
	{
		ret = poll_primary(scid, fileBuf);
		
		
		if(ret==0) cout <<"poll_primary() : OK : ret : " << ret << endl;
       	else if(ret==1){
           	cout << "AMAT has no primary AMA data : ret : " << ret << endl;
           	return 1;
           	//sleep(5);
           	//continue;
       	}else if(ret==2){
       		cout << "File returned, but there is gap in block sequence numbers : ret : " << ret << endl;
       	}else if (ret < 0 ){
       		cout << "poll_primary() : ERROR : ret : " << ret << endl;
       		AMA_ERROR = 1;
       		return ret; 
       	}

		
		if(ProcessFile(PRIM))
		{
			// Received a primary file.
        	if(ack_primary(scid, ACK) != 0 )
			{
				cout << "Send ACK : ERROR " << endl;
				return -1;	
			}else cout << "Send ACK : OK " << endl;
		}else 
		{
			cout << "ProcessFILE(PRIM) : ERROR " << endl;
			return -1;
		}
	} // end of while();
}



//-------------------------------------------------------------------------------
int Lucent_IF::get_SecondaryData(int startseq, int count)
{
	if(AMA_ERROR) return 0;
	if(count>100)
	{
		cout << "poll_secondary() : count too Big size : count <= 100 " << endl;
		return 0;
	}
	
	int ret;
	
	// Poll for secondary data.
    ret = poll_secondary(scid, fileBuf, NULL, startseq, startseq+count);
	cout << "poll_secondary() : ret : " << ret << endl;


	if(ret==0)
	{
		if(!ProcessFile(SEC)) 
		{
			cout << "ProcessFILE(SEC) : ERROR " << endl;
			return -1;
		}
	}
	
	return ret;
}




//-----------------------------------------------------------------------------------
int Lucent_IF::char2int(u_char *ustr, int len)
{
	int 	ret=0;
	char 	imsi[4];
	
	memset(imsi, 0x00, 4);
	memcpy((char *)&imsi[4-len], ustr, len);
	memcpy((void *)&ret, imsi, 4);
	
	return ret;	
}



//-----------------------------------------------------------------------------------
int Lucent_IF::ProcessFile(int type)
{
	FILE 	*fp;
	int 	i, j, rec;
	short	blksz,		// block size				  
			reclen;
	int		offset;
	long	flen,		// file length, excluding the file header 
			seqnum;		// block sequence number		  

	FHDRP	*fhdrp;		// Pointer to the file header and the first AMA block in the file
	FHDRS	*fhdrs;		// Pointer to the file header and the first AMA block in the file
	AMABLK	*amablk;	// Pointer to an AMA block 
	AMAREC	*amarec;	// Pointer to an AMA record 
	
	char		fname[200];
	DateTime	dTime;
	

	if(type == PRIM )
	{	// Poll_Primary
		fhdrp = (FHDRP *)fileBuf;

		flen = char2int(fhdrp->flen, 3);
		offset = sizeof(FHDRP);
		
		// LOG --------------------------------------------------
		cout <<"fhdrp->fssno   : " << (int)fhdrp->fssno << endl;
		cout <<"fhdrp->fnamlen : " << (int)fhdrp->fnamlen << endl;
		cout <<"fhdrp->fcode   : " << (int)fhdrp->fcode << endl;
		cout <<"fhdrp->flen    : " << flen << endl;
		if(!flen) return 0;
		
		/* FILE WRITE */
		sprintf(fname, "%s/amap.%s.%d", AMA_DATA_PATH, dTime.get_DateTime(), fhdrp->fssno);
		if(!(fp = fopen(fname, "w+b")))
		{
			printf("FILE OPEN ERROR : [%s]\n", fname);
			return 0;
		}else
		{
			//fwrite((void *)fhdrp, 1, flen, fp);
			//fclose(fp);
		}
		// LOG --------------------------------------------------
		
	}else
	{	// Poll_Secondary
		fhdrs = (FHDRS *)fileBuf;

		flen = char2int(fhdrs->flen, 3);
		offset = sizeof( FHDRS );
		
		/* LOG */
		cout << "fhdrs->fssno   : " <<(int)fhdrs->fssno << endl;
		cout << "fhdrs->fnamlen : " <<(int)fhdrs->fnamlen << endl;
		cout << "fhdrs->fcode   : " <<(int)fhdrs->fcode << endl;
		cout << "fhdrs->flen    : " <<flen << endl;
		if(!flen) return 0;
		
		/* FILE WRITE */
		sprintf(fname, "%s/amas.%s.%d", AMA_DATA_PATH, dTime.get_DateTime(), fhdrs->fssno);
		if(!(fp = fopen(fname, "w+b")))
		{
			printf("FILE OPEN ERROR [%s]\n", fname);
			return 0;
		}else
		{
			//fwrite((void *)fhdrs, 1, flen, fp);
			//fclose(fp);
		}
		/* LOG */
	}


	// Set the pointer to the first block after the file header.
	i = offset;

	// Process each block in the file.
	while( i < (flen + offset) )
	{
		amablk = (AMABLK *) &fileBuf[i];

		memcpy(&seqnum, amablk->hdr.seqnum, 4);
		memcpy(&blksz, amablk->hdr.blksz, 2);

		printf("Block: seqnum=%d, length=%d, records=%d\n",
			seqnum, blksz,
			amablk->hdr.recnum );
		
		// Process each record in the block.
		for(rec=0, j=0; rec < (int)amablk->hdr.recnum; rec++ )
		{
			amarec = (AMAREC *) &amablk->data[j];

			memcpy(&reclen, amarec->hdr.reclen, 2);

			printf("\tRecord %d: length = %d, structure=%.2x%.2x%.2x\n",
				rec, reclen,
				amarec->hdr.strcode[0],
				amarec->hdr.strcode[1],
				amarec->hdr.strcode[2] );

			// Skip to the next record.
			j += reclen;
			
			// FILE WRITE
			fwrite((void *)amarec, 1, sizeof(AMAREC), fp);

		}
		
		// Skip to the next block
		i += sizeof(AMABLK);
	}
	
	fclose(fp);
	return 1;
}


//-----------------------------------------------------------------------------------
int Lucent_IF::call_PollMaint()
{
	int				i;
	int				ret;
	u_char			*ptr;
	m_file			mf;
	m_record		*m;
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

        printf("\tDate of Record: %.2x/%.2x\n",	m->mtc_mon, m->mtc_day);
        printf("\tFlag                                 0x%.8x\n", m->flag);
        printf("\tCalls Where Dialing Complete         %ld\n", m->orig);
        printf("\tCompleted Calls                      %ld\n", m->complete);
        printf("\tIncomplete Calls                     %ld\n", m->incomplete);
        printf("\tMutilated Records                    %d\n", m->mutilrec);
        printf("\tRecords Written To Disk              %ld\n", todisk.low_high);
        printf("\tRecords Sent To Collector            %ld\n", 	tocoll.low_high);
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


//-----------------------------------------------------------------------------------
int Lucent_IF::call_GetStats()
{
	
	int				ret;
	x25_stat_buf	sb;

	if((ret = get_link_stats( scid, &sb )) != 0)
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
	printf("Number of outgoing data packets %d\n", 		sb.pkt.out_data_packet);
	printf("Number of incoming data packets %d\n", 		sb.pkt.in_data_packet);
	printf("Number of outgoing data bytes %d\n",   		sb.pkt.out_data_packet_byte);
	printf("Number of incoming data bytes %d\n",   		sb.pkt.in_data_packet_byte);
	printf("Number of outgoing expedited packets %d\n", sb.pkt.out_exdata_packet);
	printf("Number of incoming expedited packets %d\n", sb.pkt.in_exdata_packet);
	printf("Number of outgoing expedited bytes %d\n", 	sb.pkt.out_exdata_packet_byte);
	printf("Number of incoming expedited bytes %d\n", 	sb.pkt.in_exdata_packet_byte);
	printf("Number of outgoing call packets %d\n", 		sb.pkt.out_call_packet);
	printf("Number of incoming call packets %d\n", 		sb.pkt.in_call_packet);
	printf("Number of outgoing clear packets %d\n", 	sb.pkt.out_clear_packet);
	printf("Number of incoming clear packets %d\n", 	sb.pkt.in_clear_packet);
	printf("Number of outgoing reset packets %d\n", 	sb.pkt.out_reset_packet);
	printf("Number of incoming reset packets %d\n", 	sb.pkt.in_reset_packet);
	printf("Number of outgoing restart packets %d\n", 	sb.pkt.out_restart_packet);
	printf("Number of incoming restart packets %d\n", 	sb.pkt.in_restart_packet);
	printf("Number of linkup packets %d\n",     		sb.pkt.linkup_packet);
	printf("Number of linkdown packets %d\n", 		    sb.pkt.linkdown_packet);
	printf("R_State  %d\n", 							sb.pkt.R_State);

	return(0);	
}


//================================================================================
int main(int argc, char *argv[])
{
	Lucent_IF	*LucentM;


	
	if(argc<2)
	{
		cout << "Usage: "<< argv[0] <<"  OMP_address " << endl;
		return -1;
	}
	
	LucentM = new Lucent_IF(argv[1]);
	while(1)
	{
		if(!LucentM->AMA_ERROR)
		{
			if(LucentM->get_PrimaryData()<0) continue;
			cout << "[AMA_LOG] No Data : waiting 60s..!!" << endl;
			sleep(60);	// data 가 없으면 sleep후 다시 poll한다.
		}else
		{
			LucentM->AmaDisConnect();
			
			cout << "[AMA_ERROR] Reconnect : waiting 10s..!!" << endl;
			sleep(10);
			LucentM->AmaConnect();	
		}
	}
	
	delete LucentM;
}	
