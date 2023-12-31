#include <iostream.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <strings.h>

#include <ShmInfo.hpp>
#include <SendLogManage.hpp>


#define	_DEBUG 

// basic define
const unsigned short CURREMT_MSG_VER 		= 0x0100;
const unsigned short SRC_ID_SERVICE		    = 0x5544;//UD + 0001
const unsigned short SRC_ID_SYSTEM          = 0x0001;
const unsigned short DES_ID_SERVICE         = 0x4244;//BD + 0001
const unsigned short DES_ID_SYSTEM          = 0x0001;
const unsigned short TIME_INTER_VAL			= 10; 
const unsigned short MAX_BLK_CNT			= 1;  //fixed

/*	usMsgType definition  */	
const unsigned short MSG_TYPE_LINK_REQ		= 0x01;
const unsigned short MSG_TYPE_LINK_REQ_ACK	= 0x02;
const unsigned short MSG_TYPE_STATUS_REQ	= 0x03;
const unsigned short MSG_TYPE_STATUS_REQ_ACK= 0x04;
const unsigned short MSG_TYPE_STATUS_REP	= 0x05;
const unsigned short MSG_TYPE_STATUS_REP_ACK= 0x06;
const unsigned short MSG_TYPE_DATA_REP		= 0x11;
const unsigned short MSG_TYPE_DATA_REP_ACK	= 0x12;
/*	usMsgId definition  */
const unsigned short MSG_ID_CONNECT			= 0x01;
const unsigned short MSG_ID_DISCONNECT		= 0x02;
const unsigned short MSG_ID_STATUS			= 0x03;
const unsigned short MSG_ID_NORMAL_BILL		= 0x11;
const unsigned short MSG_ID_RETRANS_BILL	= 0x12;
// error code
const unsigned short ERR_NO_DATA			= 0x01;
const unsigned short ERR_MSG_NUM            = 0x0021;
const unsigned short ERR_BILL_DATA_BLK_NUM  = 0x0023;
const unsigned short ERR_MSG_TYPE_ID        = 0x0024;
const unsigned short ERR_TIME_VAL_SIZE      = 0x0025;
const unsigned short ERR_UNDIFINE_MSG       = 0x00ff;

const int CONNECT_REQ		= 1;
const int CONNECT_REQ_ACK	= 2;
const int RELEASE_REQ		= 3;
const int RELEASE_REQ_ACK	= 4;
const int STATUS_REQ		= 5;
const int STATUS_REQ_ACK	= 6;
const int STATUS_REP		= 7;
const int STATUS_REP_ACK	= 8;
const int DATA_REP			= 9;
const int DATA_REP_ACK		= 10;


#define MAX_DATA_LENGTH 1024


////////////////////////////////////////
// Header
////////////////////////////////////////
typedef struct _tagMsgHeader
{
	unsigned short usMsgVersionId;
	unsigned short usMsgFiller;		//Fixed the Memory Boundary
	struct {
            unsigned short usServiceType;
            unsigned short usSystemId;
    } SourceId;
	struct {
            unsigned short usServiceType;
            unsigned short usSystemId;
    } DestId;
	unsigned int unMsgSeqNum;
	unsigned short usMsgType;
	unsigned short usMsgId;
	unsigned int unMsgLen;
} MSGH, *PMSGH;
////////////////////////////////////////
// Data Struct
////////////////////////////////////////
typedef struct _connectionReq
{
	unsigned int unStartBNo;
	unsigned int unTimeInVal;
	unsigned int unMaxBCnt;
	
} MSGCONREQ, *PMSGCONREQ;

typedef struct _connectionReqAck
{
	unsigned short usStatus;
	unsigned short usReason;
	unsigned int unRadiusId;
	unsigned int unStartBNo;
	unsigned int unTimeInVal;
	unsigned int unMaxBCnt;
} MSGCONREQACK, *PMSGCONREQACK;

typedef struct _dataRep
{
	unsigned short unDataType;
	unsigned short unUdrCnt;
	unsigned int unBlkNo;
	unsigned int unBlkSize;
	char strData[32768];//32768 //80
}MSGDATAREP, *PMSGDATAREP;

typedef struct _dataRepAck
{
	unsigned short usStatus;
	unsigned short usReason;
	unsigned int unBlockNo;	
	char strTimeStamp[14];
	//unsigned short usMsgFiller;		//Fixed the Memory Boundary
}MSGDATAREPACK, PMSGDATAREPACK;

typedef struct _statusRep
{
	unsigned short usStatus;
	unsigned short usErrCode;
	unsigned int unBlockNo;
	unsigned short usMsgFiller;		//Fixed the Memory Boundary	
	char strTimeStamp[14];
}MSGSTREP, PMSGSTREP;

/*
//MQueue�� STRUCTURE
typedef struct _DataNode
{
	char strData[32768];//32768 //80
}DataNode, *PDataNode;
//MQueue�� STRUCTURE
*/	
	
	
//2000.10.28 csy
struct _DATANODE
{
	unsigned int processType;	
	unsigned int processSN;	
	unsigned int processRemark;	
	//FILESEQUENCE/////////////////
	unsigned int unFileSequence;
	char strData[32768];//32768 //80
};

class	DataNode	: public _DATANODE
{
	public:
		operator struct _DATANODE* () const { return (_DATANODE*)this; }
};



//PMSMQueue�� STRUCTURE
struct _DATANODE_PMS
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

	
class radiuspPacketC
{
public :
	MSGH 			stMsgh;
	MSGCONREQ 		stMsgConReq;
	MSGCONREQACK	stMsgConReqAck;
	MSGDATAREP		stMsgDataRep;
	MSGDATAREPACK	stMsgDataRepAck;
	MSGSTREP		stMsgStRep;
	
	
public :
	void setAll(
	unsigned short usMsgVersionId,
	unsigned short usSourceIdSType,
	unsigned short usSourceIdSId,
	unsigned short usDestIdSType,
	unsigned short usSestIdSId,		
	unsigned int unMsgSeqNum,
	unsigned short usMsgType,
	unsigned short usMsgId )
	{
		setMsgVersionId( usMsgVersionId );
		setSourceId( usSourceIdSType, usSourceIdSId );
		setDestId( usDestIdSType, usSestIdSId );
		setMsgSeqNum( unMsgSeqNum );
		setMsgType( usMsgType );
		setMsgId( usMsgId );
	}

	void setMsgVersionId( unsigned short usMsgVersionId ) { stMsgh.usMsgVersionId = CURREMT_MSG_VER; }
	void setSourceId( unsigned short usSourceIdSType, unsigned short usSourceIdSId )
	{
		stMsgh.SourceId.usServiceType = usSourceIdSType;
		stMsgh.SourceId.usSystemId = usSourceIdSId;
	}
	void setDestId( unsigned short usDestIdSType, unsigned short usSestIdSId )
	{
		stMsgh.DestId.usServiceType = usDestIdSType;
		stMsgh.DestId.usSystemId = usSestIdSId;
	}
	void setMsgSeqNum( unsigned int unMsgSeqNum )   { stMsgh.unMsgSeqNum = unMsgSeqNum; }
	void setMsgType( unsigned short usMsgType )     { stMsgh.usMsgType = usMsgType; }
	void setMsgId( unsigned short usMsgId )         { stMsgh.usMsgId = usMsgId; }
	void setMsgLen( unsigned int unMsgLen )         { stMsgh.unMsgLen = unMsgLen; }
	void init( void ) {
		 memset( this,0x00,sizeof(this) );
		 
	}
	int getHeaderLength( void )  { return sizeof(stMsgh); }

	unsigned short getMsgVersionId( void )   { return stMsgh.usMsgVersionId; }
	unsigned short getSourceIdSType( void )  { return stMsgh.SourceId.usServiceType; }
	unsigned short getSourceIdSId( void )    { return stMsgh.SourceId.usSystemId; }
	unsigned short getDestIdSType( void )    { return stMsgh.DestId.usServiceType; }
	unsigned short getDestIdSId( void )      { return stMsgh.DestId.usSystemId; }
	unsigned int getMsgSeqNum( void )      { return stMsgh.unMsgSeqNum; }
	unsigned short getMsgType( void )        { return stMsgh.usMsgType; }
	unsigned short getMsgId( void )          { return stMsgh.usMsgId; }
	unsigned int getMsgLen( void )         { return stMsgh.unMsgLen; }	
	
	void printMsg( int nMsgType )
	{
		cout << " ===== HEADER====================================================\n";
		//performancecout << " sizeof(stMsgh)  	   [" << sizeof(stMsgh)           << "]" << endl;
		//performancecout << " getusMsgVersionId()  [" << getMsgVersionId()        << "]" << endl;
		//performancecout << " getSourceIdSType()   [" << getSourceIdSType()       << "]" << endl;
		//performancecout << " getSourceIdSId()     [" << getSourceIdSId()         << "]" << endl;
		//performancecout << " getDestIdSType()     [" << getDestIdSType()         << "]" << endl;		
		//performancecout << " getDestIdSId()       [" << getDestIdSId()           << "]" << endl;
		cout << " getunMsgSeqNum()     [" << getMsgSeqNum()           << "]" << endl;
		cout << " getusMsgType()       [" << getMsgType()             << "]" << endl;
		cout << " getusMsgId()         [" << getMsgId()               << "]" << endl;
		cout << " getunMsgLen()        [" << getMsgLen()              << "]" << endl;
		switch(nMsgType)
		{
			case CONNECT_REQ:
		cout << " ===== radiuspPacketC::CONNECTION_REQ=====\n";
		cout << " unStartBNo           [" << stMsgConReq.unStartBNo   << "]" << endl;
		//performancecout << " unTimeInVal          [" << stMsgConReq.unTimeInVal  << "]" << endl;
		//performancecout << " unMaxBCnt            [" << stMsgConReq.unMaxBCnt    << "]" << endl;
		   break;
			
			case CONNECT_REQ_ACK:
		cout << " ===== radiuspPacketC::CONNECTION_REQ_ACK=====\n";
		cout << " usStatus             [" << stMsgConReqAck.usStatus  << "]" << endl;
		cout << " usReason             [" << stMsgConReqAck.usReason  << "]" << endl;
		//performancecout << " unRadiusId           [" << stMsgConReqAck.unRadiusId<< "]" << endl;
		cout << " unStartBNo           [" << stMsgConReqAck.unStartBNo<< "]" << endl;
		//performancecout << " unTimeInVal          [" << stMsgConReqAck.unTimeInVal<< "]" << endl;
		//performancecout << " unMaxBCnt            [" << stMsgConReqAck.unMaxBCnt << "]" << endl;
			break;
			
			case DATA_REP:
		cout << " ===== radiuspPacketC::DATA_REP=====\n";
		//performancecout << " unDataType           [" << stMsgDataRep.unDataType  << "]" << endl;
		//performancecout << " unUdrCnt             [" << stMsgDataRep.unUdrCnt    << "]" << endl;
		cout << " unBlkNo              [" << stMsgDataRep.unBlkNo     << "]" << endl;
		//performancecout << " unBlkSize            [" << stMsgDataRep.unBlkSize   << "]" << endl;
		#ifdef _DEBUGForLogData
			//performancecout << " strData              [" << stMsgDataRep.strData     << "]" << endl;
		#endif
			break;
		
			case DATA_REP_ACK:
		cout << " ===== radiuspPacketC::DATA_REP_ACK=====\n";
		cout << " usStatus        [" << stMsgDataRepAck.usStatus      << "]" << endl;
		cout << " usReason        [" << stMsgDataRepAck.usReason      << "]" << endl;
		cout << " unBlockNo       [" << stMsgDataRepAck.unBlockNo     << "]" << endl;		
		//performancecout << " strTimeStamp    [" << stMsgDataRepAck.strTimeStamp  << "]" << endl;
			break;
		   
		    case STATUS_REP:
		cout << " ===== radiuspPacketC::STATUS_REP=====\n";
		cout << " usStatus        [" << stMsgStRep.usStatus           << "]" << endl;
		cout << " usErrCode       [" << stMsgStRep.usErrCode          << "]" << endl;	
		cout << " unBlockNo       [" << stMsgStRep.unBlockNo          << "]" << endl;				
		//performancecout << " strTimeStamp    [" << stMsgStRep.strTimeStamp       << "]" << endl;	
			break;
			
			case STATUS_REP_ACK:
		cout << " ===== radiuspPacketC::STATUS_REP_ACK=====\n";
			break;
			
			case STATUS_REQ:
		cout << " ===== radiuspPacketC::STATUS_REQ=====\n";
			break;
			
			case STATUS_REQ_ACK:
		cout << " ===== radiuspPacketC::STATUS_REQ_ACK=====\n";
			break;

			case RELEASE_REQ:
		cout << " ===== radiuspPacketC::RELEASE_REQ=====\n";
			break;

			case RELEASE_REQ_ACK:
		cout << " ===== radiuspPacketC::RELEASE_REQ_ACK=====\n";
			break;
			
			default: break;
	   }
		//performancecout << " ===== END=======================================================\n";
		cout<<"\n\n";
	}

	void printAll( void )
	{
		//performancecout << " ===== radiuspPacketC::printAll()=====\n";
		//performancecout << " stMsgp.stMsgh.usMsgVersionId         [" << stMsgh.usMsgVersionId         << "]" << endl;
		//performancecout << " stMsgp.stMsgh.SourceId.usServiceType [" << stMsgh.SourceId.usServiceType << "]" << endl;
		//performancecout << " stMsgp.stMsgh.SourceId.usSystemId    [" << stMsgh.SourceId.usSystemId    << "]" << endl;		
		//performancecout << " stMsgp.stMsgh.DestId.usServiceType   [" << stMsgh.DestId.usServiceType   << "]" << endl;
		//performancecout << " stMsgp.stMsgh.DestId.usSystemId      [" << stMsgh.DestId.usSystemId      << "]" << endl;		
		//performancecout << " stMsgp.stMsgh.unMsgSeqNum            [" << stMsgh.unMsgSeqNum            << "]" << endl;
		//performancecout << " stMsgp.stMsgh.usMsgType              [" << stMsgh.usMsgType              << "]" << endl;
		//performancecout << " stMsgp.stMsgh.usMsgId                [" << stMsgh.usMsgId                << "]" << endl;
		//performancecout << " stMsgp.stMsgh.unMsgLen               [" << stMsgh.unMsgLen               << "]" << endl;
//		//performancecout << " stMsgp.strData                       [" << stMsgp.strData                       << "]" << endl;
		//performancecout << " ===== radiuspPacketC::printAll()=====\n";
	}


	radiuspPacketC()
	{
		//////performancecout << " CPacket::CPacket() Start Constructor!!!!!!!!!!!\n";
		init();
		setAll(0xffff, 0xffff, 0xffff,  0xffff, 0xffff, 0xffffffff, 0xffff, 0xffff);
		//////performancecout << " CPacket::CPacket() End Constructor!!!!!!!!!!!\n";
	}
	~radiuspPacketC()
	{
		//performancecout << " CPacket::CPacket() Destructor!!!!!!!!!!!\n";
	}
};



