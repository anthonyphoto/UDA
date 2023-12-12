/*
 * NAME:	client.c
 *
 * AUTHOR/DATE:	Scott Elliott		12/01/94
 *		David O. Chambers	01/16/96
 *		- added ifdef within connect_session for acparms
 *
 * DESCRIPTION:	This file contains function calls to provide a consistent
 *		interface to the AMATPS+ RPC functions.  These functions are
 *		based on the AMATPS Continuous Polling API, and provide a
 *		translation from the API function calls to the RPC function
 *		calls.
 *
 * REFERENCES:
 *
 */

#include <rpc/rpc.h>
#include "amatps.h"

/*
 * Additional include files needed for the Starserver FT
 */
#ifdef us2_1
#include <bsd/netdb.h>
#include <bsd/sys/types.h>
#include <bsd/sys/socket.h>
#include <bsd/netinet/in.h>
#endif


/*
 * The SVR3 version of RPC on the StarServer FT does not have a "clnt_create"
 * call.  This one only connects using TCP (it ignores nettype). 
 */
#ifdef us2_1
CLIENT *
clnt_create(
char	*host,
u_long	prognum,
u_long	versnum,
char	*nettype
)
{
        struct sockaddr_in      addr ;

        CLIENT *clnt;

        int     fd ;

        struct hostent          *hp ;
 



        if( ( hp = gethostbyname( host ) ) == NULL )
        {
                return NULL ;
        }

        memcpy( &addr.sin_addr, hp->h_addr, hp->h_length ) ;
        addr.sin_family = hp->h_addrtype ;
        addr.sin_port = 0 ;

        fd = RPC_ANYSOCK ;

        clnt = clnttcp_create(&addr, prognum, versnum, &fd, 0, 0 );

	return clnt ;
}
/*
 * CLSET_TIMEOUT may not be defined on the Starserver FT
 */
#ifndef CLSET_TIMEOUT
#define CLSET_TIMEOUT 1
#endif
#endif


/*
 * connect_rpc() and disconnect_rpc() use a single client handle, which means
 * that the process can have only one client connection at a time.
 */
CLIENT *clnt_handle ;


int
connect_rpc( char *host, int timeout )
{
	struct timeval tval ;

	if ( clnt_handle != (CLIENT *) NULL)
	{
		/*
		 * Connection is already established.
		 * This keeps the user from overwriting the current
		 * client handle.
		 */
		return EARSCONNECT ;
	}

	clnt_handle = clnt_create( host, AMATPSPLUS, AMATPSPLUSVER, "circuit_v" ) ;

	if (clnt_handle == (CLIENT *) NULL) {
		return EARRPCFAIL ;
	}

	tval.tv_sec = timeout ;

	clnt_control( clnt_handle, CLSET_TIMEOUT, (char *) &tval ) ;

	return 0 ;
}

void
disconnect_rpc()
{
	/*
	 * clnt_destroy will core dump if called with
	 * clnt_handle set to NULL.
	 */
	if ( clnt_handle == NULL )
		return;

	clnt_destroy( clnt_handle ) ;

	clnt_handle = NULL ;
}


/*
 * The remaining routines are AMATPS Continuous Polling API to RPC translations
 */

int
ack_primary( int scid, int ack )
{
	int		*result;
	int		rcode ;

	ack_input	arg;




	arg.scid = scid ;
	arg.ack = ack ;

	//result = rpc_ack_primary_1(&arg, clnt_handle ) ;
	result = rpc_ack_primary_1(arg, clnt_handle ) ;
	
	if( result == (int *) NULL )
	{
		return EARRPCFAIL ;
	}

	rcode = *result ;
	//xdr_free(xdrproc_t proc, char *objp);
	xdr_free((xdrproc_t)xdr_int, (char *) result ) ;

	return rcode ;
}

int
connect_session(
char		*dcf,
acpstruct	acparms
)
{
	int		*result ;
	int		rcode ;

	connect_input	arg ;

#ifdef i386
	/*
	 * The acparms structure is declared as an 'opaque' field
	 * within the connect_input structure.  'opaque' turns off
	 * XDR byte swapping, even though these fields really need
	 * to be swapped.  Therefore, we must 'manually' swap the 
	 * bytes on machines whose byte order is opposite that of the
	 * RPC Server (Sun), such as ix86 based machines (GIS 3xxx).
	 *
	 * The change was made here rather than changing the opaque declaration,
	 * which would have resulted in the need for a reissue of the RPC
	 * server package.
	 */
        acparms.sensor_id = htonl(acparms.sensor_id);
        acparms.sensor_type = htonl(acparms.sensor_type);
        acparms.coll_passwd = htonl(acparms.coll_passwd);
        acparms.sending_unit = htonl(acparms.sending_unit);
#endif

	arg.dcf = dcf ;
	arg.acparms.acparms_val = (char*) &acparms ;
	arg.acparms.acparms_len = sizeof( acparms ) ;

	
	//result = rpc_connect_session_1( &arg, clnt_handle ) ;
	result = rpc_connect_session_1( arg, clnt_handle ) ;

	if( result == (int *) NULL )
	{
		return EARRPCFAIL ;
	}

	rcode = *result ;

	xdr_free( xdr_int, (char *) result ) ;

	return rcode ;
}


int
disconnect_session( int scid, int type )
{
	int			*result ;
	int			rcode ;

	disconnect_input	arg ;



	arg.scid = scid ;
	arg.type = type ;

	//result = rpc_disconnect_session_1( &arg, clnt_handle ) ;
	result = rpc_disconnect_session_1( arg, clnt_handle ) ;

	if( result == (int *) NULL )
	{
		return EARRPCFAIL ;
	}

	rcode = *result ;

	xdr_free( xdr_int, (char *) result ) ;

	return rcode ;
}

int
get_link_stats(
int		scid,
x25_stat_buf	*sb
)
{
	link_return	*result ;

	int		scode ;


	result = rpc_get_link_stats_1( &scid, clnt_handle ) ;

	if( result == NULL )
	{
		return EARRPCFAIL ;
	}

	memcpy( sb, result->x25_stat_buf.x25_stat_buf_val,
		 result->x25_stat_buf.x25_stat_buf_len ) ;

	scode = result->scode ;

	xdr_free( xdr_link_return, (char *) result ) ;

	return scode ;
}

int
get_link_status(
int scid
)
{
	int	*result ;
	int	rcode ;



	result = rpc_get_link_status_1( &scid, clnt_handle ) ;

	if( result == NULL )
	{
		return EARRPCFAIL ;
	}

	rcode = *result ;

	xdr_free( xdr_int, (char *) result ) ;

	return rcode ;
}

int
poll_primary(
int		scid,
unsigned char	*fb
)
{
	primary_return	*result ;

	int		scode ;



	result = rpc_poll_primary_1( &scid, clnt_handle ) ;

	if( result == NULL )
	{
		return EARRPCFAIL ;
	}

	memcpy( fb, result->fb.fb_val, result->fb.fb_len ) ;

	scode = result->scode ;

	xdr_free( xdr_primary_return, (char *) result ) ;

	return scode ;
}

int
poll_secondary(
int		scid,
unsigned char	*fb,
unsigned char	*tb,
long		sbsn,
long		ebsn
)
{
	secondary_input		arg ;
	secondary_return	*result ;

	int			scode ;


	arg.scid = scid ;
	arg.sbsn = sbsn ;
	arg.ebsn = ebsn ;

	if( fb != NULL )
	{
		arg.fb = 1 ;
	}
	else
	{
		arg.fb = 0 ;
	}

	if( tb != NULL )
	{
		arg.tb = 1 ;
	}
	else
	{
		arg.tb = 0 ;
	}

	//result = rpc_poll_secondary_1( &arg, clnt_handle ) ;
	result = rpc_poll_secondary_1( arg, clnt_handle ) ;

	if( result == NULL )
	{
		return EARRPCFAIL ;
	}

	if( tb != NULL )
	{
		memcpy( tb, result->tb.tb_val, result->tb.tb_len ) ;
	}

	memcpy( fb, result->fb.fb_val, result->fb.fb_len ) ;

	scode = result->scode ;

	xdr_free( xdr_secondary_return, (char *) result ) ;

	return scode ;
}

int
poll_maint(
int	scid,
m_file	*fb
)
{
	maint_return	*result ;

	int		scode ;



	result = rpc_poll_maint_1( &scid, clnt_handle ) ;

	if( result == NULL )
	{
		return EARRPCFAIL ;
	}

	memcpy( fb, result->fb.fb_val, result->fb.fb_len ) ;

	scode = result->scode ;

	xdr_free( xdr_maint_return, (char *) result ) ;

	return scode ;
}

int
reset_link_stats(
int scid
)
{
	int	*result ;
	int	rcode ;



	result = rpc_reset_link_stats_1( &scid, clnt_handle ) ;

	if( result == NULL )
	{
		return EARRPCFAIL ;
	}

	rcode = *result ;

	xdr_free( xdr_int, (char *) result ) ;

	return rcode ;
}

int
set_trace(
int	trace_class,
int	trace_level,
char	*file_prefix,
long	max_file_size
)
{
	int		*result ;
	int		rcode ;

	trace_input	arg ;



	arg.trace_class = trace_class ;
	arg.trace_level = trace_level ;
	arg.file_prefix = file_prefix ;
	arg.max_file_size = max_file_size ;

	//result = rpc_set_trace_1( &arg, clnt_handle ) ;
	result = rpc_set_trace_1( arg, clnt_handle ) ;

	if( result == NULL )
	{
		return EARRPCFAIL ;
	}

	rcode = *result ;

	xdr_free( xdr_int, (char *) result ) ;

	return rcode ;
}

int
ssend(
int		scid,
int		mtype,
unsigned char	*smb,
int		mbsize
)
{
	int		*result ;
	int		rcode ;

	ssend_input	arg ;



	arg.scid = scid ;
	arg.mtype = mtype ;
	arg.smb.smb_len = mbsize ;
	arg.smb.smb_val = (char *) smb ;

	//result = rpc_ssend_1( &arg, clnt_handle ) ;
	result = rpc_ssend_1( arg, clnt_handle ) ;

	if( result == NULL )
	{
		return EARRPCFAIL ;
	}

	rcode = *result ;

	xdr_free( xdr_int, (char *) result ) ;

	return rcode ;
}

int
srecv(
int		scid,
int		*mtype,
unsigned char	*smb,
int		mbsize
)
{
	srecv_return	*result ;

	srecv_input	arg ;

	int		scode ;



	arg.scid = scid ;
	arg.mbsize = mbsize ;

	//result = rpc_srecv_1( &arg, clnt_handle ) ;
	result = rpc_srecv_1( arg, clnt_handle ) ;

	if( result == NULL )
	{
		return EARRPCFAIL ;
	}

	memcpy( smb, result->smb.smb_val, result->smb.smb_len ) ;

	*mtype = result->mtype ;

	scode = result->scode ;

	xdr_free( xdr_maint_return, (char *) result ) ;

	return scode ;
}

int
test_connection(
int	scid,
int	count
)
{
	int		*result ;
	int		rcode ;

	test_input	arg ;


	arg.scid = scid ;
	arg.count = count ;


	//result = rpc_test_connection_1( &arg, clnt_handle ) ;
	result = rpc_test_connection_1( arg, clnt_handle ) ;

	if( result == NULL )
	{
		return EARRPCFAIL ;
	}

	rcode = *result ;

	xdr_free( xdr_int, (char *) result ) ;

	return rcode ;
}

int
reset_server( char *password )
{
	int	*result ;
	int	rcode ;



	result = rpc_reset_server_1( &password, clnt_handle ) ;

	if( result == NULL )
	{
		return EARRPCFAIL ;
	}

	rcode = *result ;

	xdr_free( xdr_int, (char *) result ) ;

	return rcode ;
} 


int
get_dcf(
char *dcf,
unsigned char *fb
)
{
	dcf_return	*result ;

	int		scode ;




	result = rpc_get_dcf_1( &dcf, clnt_handle ) ;

	if( result == NULL )
	{
		return EARRPCFAIL ;
	}

	memcpy( fb, result->fb, strlen( result->fb ) ) ;

	scode = result->scode ;

	xdr_free( xdr_dcf_return, (char *) result ) ;

	return scode ;
}

int
put_dcf(
char *dcf,
char *fb,
unsigned char cflag,
char *arpcs_passwd
)
{
	int		*result ;
	int		scode ;

	put_input	arg ;


	arg.dcf = dcf ;
	arg.fb = fb ;
	arg.cflag = cflag ;
	arg.password = arpcs_passwd ;

	//result = rpc_put_dcf_1( &arg, clnt_handle ) ;
	result = rpc_put_dcf_1( arg, clnt_handle ) ;

	if( result == NULL )
	{
		return EARRPCFAIL ;
	}

	scode = *result ;

	xdr_free( xdr_int, (char *) result ) ;

	return scode ;
}
