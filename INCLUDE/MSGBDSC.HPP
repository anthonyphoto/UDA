#include <iostream.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>//usleep()
#include <sys/time.h>
#include <strings.h>

#include <ShmInfo.hpp>
#include <SendLogManage.hpp>

#define	_DEBUG


// basic define
const unsigned short CURREMT_MSG_VER 		= 0x0100;
//const unsigned short SRC_ID_SERVICE		    = 0x5544;//UD + 0001
//const unsigned short SRC_ID_SYSTEM          = 0x0001;
const unsigned short DES_ID_SERVICE         = 0x4244;//BD + 0001
const unsigned short DES_ID_SYSTEM          = 0x0001;
const unsigned short TIME_INTER_VAL			= 10;
const unsigned short MAX_BLK_CNT			= 1;  //fixed
//const unsigned int   UNSWITCHID			    = 0x55440001;//UD0001

/*	usMsgType definition  */
const unsigned short MSG_TYPE_DATA_REQ			= 0x01;
const unsigned short MSG_TYPE_DATA_REQ_ACK		= 0x02;
const unsigned short MSG_TYPE_DATA_RES			= 0x03;
const unsigned short MSG_TYPE_DATA_RES_ACK		= 0x04;
const unsigned short MSG_TYPE_CHANGE_REQ		= 0x05;
const unsigned short MSG_TYPE_CHANGE_REQ_ACK	= 0x06;
const unsigned short MSG_TYPE_CHANGE_RES		= 0x07;
const unsigned short MSG_TYPE_CHANGE_RES_ACK	= 0x08;
const unsigned short MSG_TYPE_CHECK_REQ			= 0x09;
const unsigned short MSG_TYPE_CHECK_REQ_ACK		= 0x0A;
const unsigned short MSG_TYPE_CHECK_RES			= 0x0B;
const unsigned short MSG_TYPE_CHECK_RES_ACK		= 0x0C;
const unsigned short MSG_TYPE_DATA_REP			= 0x0D;
const unsigned short MSG_TYPE_DATA_REP_ACK		= 0x0E;
const unsigned short MSG_TYPE_CONTROL_REP		= 0x0F;
const unsigned short MSG_TYPE_CONTROL_REP_ACK	= 0x10;

/*	usMsgId definition  */
const unsigned short MSG_ID_NORMAL_BILL					=0x01;
const unsigned short MSG_ID_RETRANS_BILL				=0x02;
const unsigned short MSG_ID_TEST_NORMAL_BILL			=0x03;
const unsigned short MSG_ID_TEST_RETRANS_BILL			=0x04;
const unsigned short MSG_ID_SWITCH_DISK_STATUS			=0x05;
const unsigned short MSG_ID_MD_DISK_STATUS				=0x06;
const unsigned short MSG_ID_HOST_DISK_STATUS			=0x07;
const unsigned short MSG_ID_TEST_DISK_STATUS			=0x08;
const unsigned short MSG_ID_SWITCH_STATUS				=0x09;
const unsigned short MSG_ID_MD_STATUS					=0x0A;
const unsigned short MSG_ID_HOST_STATUS					=0x0B;
const unsigned short MSG_ID_SWITCH_LOAD_STATUS			=0x0D;
const unsigned short MSG_ID_MD_LOAD_STATUS				=0x0E;
const unsigned short MSG_ID_HOST_LOAD_STATUS			=0x0F;
const unsigned short MSG_ID_TEST_LOAD_STATUS			=0x10;
const unsigned short MSG_ID_SWITCH_BILL_STATUS			=0x11;
const unsigned short MSG_ID_MD_BILL_STATUS				=0x12;
const unsigned short MSG_ID_HOST_BILL_STATUS			=0x13;
const unsigned short MSG_ID_TEST_BILL_STATUS			=0x14;
const unsigned short MSG_ID_SWITCH_COLLECTION_CONFIG	=0x15;
const unsigned short MSG_ID_MD_COLLECTION_CONFIG		=0x16;
const unsigned short MSG_ID_HOST_COLLECTION_CONFIG		=0x17;
const unsigned short MSG_ID_TEST_COLLECTION_CONFIG		=0x18;
const unsigned short MSG_ID_SWITCH_START_BILL_INFO		=0x19;
const unsigned short MSG_ID_MD_START_BILL_INFO			=0x1A;
const unsigned short MSG_ID_HOST_START_BILL_INFO		=0x1B;
const unsigned short MSG_ID_TEST_START_BILL_INFO		=0x1C;
const unsigned short MSG_ID_SWITCH_CONFIG				=0x1D;
const unsigned short MSG_ID_MD_CONFIG					=0x1E;
const unsigned short MSG_ID_SWITCH_TEST_CONFIG			=0x1F;
const unsigned short MSG_ID_MD_TEST_CONFIG				=0x20;
const unsigned short MSG_ID_HOST_TEST_CONFIG			=0x21;
const unsigned short MSG_ID_RETRANS_COMPLETE			=0x22;
const unsigned short MSG_ID_DISCONNECT					=0x24;
const unsigned short MSG_ID_SWITCH_BILL_DATA_ERROR		=0x25;
const unsigned short MSG_ID_SWITCH_COMM_STATUS			=0x26;
const unsigned short MSG_ID_BACKUP_BILL					=0x27;


// error code
const unsigned short ERR_NO_DATA			= 0x01;
const unsigned short ERR_MSG_NUM            = 0x0021;
const unsigned short ERR_BILL_DATA_BLK_NUM  = 0x0023;
const unsigned short ERR_MSG_TYPE_ID        = 0x0024;
const unsigned short ERR_TIME_VAL_SIZE      = 0x0025;
const unsigned short ERR_UNDIFINE_MSG       = 0x00ff;

const int CONFIG_REQ					= 1;
const int CONFIG_REQ_ACK				= 2;
const int CONFIG_RES					= 3;
const int CONFIG_RES_ACK				= 4;
const int COLLECTION_START_INFO			= 5;
const int COLLECTION_START_INFO_ACK		= 6;
const int TRANS_CONFIG_CHANGE_REQ		= 7;//
const int TRANS_CONFIG_CHANGE_REQ_ACK	= 8;//
const int BILL_DATA_TRANS_START_REQ		= 9;
const int BILL_DATA_TRANS_START_REQ_ACK	= 10;
const int BILL_DATA_REP					= 11;
const int BILL_DATA_REP_ACK				= 12;//
const int DISK_STATUS_REQ				= 13;
const int DISK_STATUS_REQ_ACK			= 14;
const int STATUS_DATA_REP1				= 15;//
const int STATUS_DATA_REP2				= 16;//
const int STATUS_DATA_REP3				= 17;//
const int STATUS_DATA_REP4				= 18;//
const int STATUS_DATA_REP5				= 19;//
const int STATUS_DATA_REP_ACK			= 20;//
const int STATUS_REQ					= 21;//
const int STATUS_REQ_ACK				= 22;//
const int DISCONNECT_REQ				= 23;//

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
	unsigned short usMsgSeqNum;//unMsgSeqNum에서 변경
	unsigned short usContinuousNum;//추가
	unsigned short usMsgType;
	unsigned short usMsgId;
	unsigned int unMsgLen;
} MSGH, *PMSGH;

////////////////////////////////////////
// Data Struct
////////////////////////////////////////
/*
typedef struct s_msgHeader {
   short          msgVersionID;
   short          dummy;
   unsigned int   sourceID;
   unsigned int   destID;
   short          msgSeqNum;
   short          contSerialNum;
   short          msgType;
   short          msgID;
   unsigned long  msgLen;
} tmsgHeader;
*/
typedef struct _tagFileNoRange {
	unsigned int	unStartFileNo;
	unsigned int	unEndFileNo;
} FILENORANGE;
typedef struct _tagBlockNoRange {
	unsigned int	unStartBlockNo;
	unsigned int	unEndBlockNo;
} BLOCKNORANGE;
typedef struct _tagRElement {
	unsigned int	unSwitchId;
	FILENORANGE		stFileNoRange;
	BLOCKNORANGE	stBlockNoRange;
} RELEMENT;
typedef struct _tagSElement {
	unsigned int	unSwitchId;
	unsigned int	unStartFileNo;
	unsigned int	unStartBlockNo;
} SELEMENT;
typedef struct _tagDElement {
	unsigned int	unSwitchId;
	unsigned short	usStatus;
	unsigned short	usErrorCode;
	char			strDescription[40]; /* fix */
} DELEMENT;


typedef struct _tagConfigReqAck {
	unsigned short	usStatus;
	unsigned short	usReason;
	char			strDescription[60]; /* variable */
} CONFIGREQACK;

typedef struct _tagConfigRes {
	unsigned int	unMdId;
	unsigned int	unSwitchCount;
	RELEMENT		stRElement[10];
} CONFIGRES;

typedef struct _tagConfigResAck {
	unsigned short	usStatus;
	unsigned short	usReason;
	unsigned int	unMdId;
	unsigned int	unSwitchCount;
	RELEMENT		stRElement[10];
} CONFIGRESACK;

typedef struct _tagCollectionStartInfo {
	unsigned int	unMdId;
	unsigned int	unSwitchCount;
	SELEMENT		stSElement[10];
} COLLECTIONSTARTINFO;

typedef struct _tagCollectionStartInfoAck {
	unsigned short	usStatus;
	unsigned short	usReason;
	unsigned int	unMdId;
	unsigned int	unSwitchCount;
	SELEMENT		stSElement[10];
} COLLECTIONSTARTINFOACK;

typedef struct _tagTransConfigChangeReq {
	unsigned int	unTimeInVal;
	unsigned int	unMaxBillSize;
} TRANSCONFIGCHANGEREQ;

typedef struct _tagTransConfigChangeReqAck {
	unsigned short	usStatus;
	unsigned short	usReason;
	unsigned int	unTimeInVal;
	unsigned int	unMaxBillSize;
	char			strDescription[60]; /* fix */
} TRANSCONFIGCHANGEREQACK;

typedef struct _tagBillDataTransStartReq {
	unsigned int	unMdId;
	unsigned int	unSwitchCount;
	unsigned int	unSwitchId[10];
} BILLDATATRANSSTARTREQ;

typedef struct _tagBillDataTransStartReqAck {
	unsigned short	usStatus;
	unsigned short	usReason;
	char			strDescription[60]; /* variable */
} BILLDATATRANSSTARTREQACK;
/*****************************************************
typedef struct _tagBillDataRep {
	unsigned int	unSwitchId;
	unsigned short	usDataType;
	unsigned short	usCdrSize;
	unsigned int	unBlockSize;
	unsigned int	unFileNo;
	BLOCKNORANGE	stBlockNoRange;
	char			strBillData[32768];
} BILLDATAREP;
*****************************************************/
typedef struct _tagBillDataRepBLOCK {
	unsigned short	usBlockId;
	unsigned short  usMsgFiller;		//Fixed the Memory Boundary
	unsigned int	unBlockCount;
	unsigned int	unRecordCount;	
	char			strBillData1[336];
	char			strBillData2[336];
	char			strBillData3[336];
	char			strUnUsed[4];	
} BILLDATAREPBLOCK;
typedef struct _tagBillDataRep {
	unsigned int	unSwitchId;
	unsigned short	usDataType;
	unsigned short	usCdrSize;
	unsigned int	unBlockSize;
	unsigned int	unFileNo;
	BLOCKNORANGE	stBlockNoRange;
//	BILLDATAREPBLOCK strBillData[32];
	char			strBillData[32*1024];
} BILLDATAREP;

typedef struct _tagBillDataRepAck {
	unsigned int	unSwitchId;
	unsigned int	unFileNo;
	BLOCKNORANGE	stBlockNoRange;
	char			strTimeStamp[14];
	unsigned short	usStatus;
	unsigned short	usReason;
	char			strDescription[60]; /* variable */
} BILLDATAREPACK;

// HOST => MD
typedef struct _tagDiskStatusReq {
	unsigned int	unMdId;
	unsigned int	unSwitchCount;
	unsigned int	unSwitchId[10];
} DISKSTATUSREQ;

typedef struct _tagDiskStatusReqAck {
	unsigned int	unMdId;
	unsigned short	usStatus;
	unsigned short	usErrorCode;
	char			strDescription[40]; //fix
	unsigned int	unSwitchCount;
	DELEMENT		stDElement[10]; //variable
} DISKSTATUSREQACK;

/************************************
// HOST => SWITCH
typedef struct _tagDiskStatusReq {
	unsigned int	unSwitchId;
} DISKSTATUSREQ;

typedef struct _tagDiskStatusReqAck {
	unsigned int	unSwitchId;	
} DISKSTATUSREQACK;
************************************/





typedef struct _tagStatusDataRep1{
	unsigned short	usStatus;
	unsigned short	usSysType;
	unsigned int	unSysId;
	unsigned short	usErrorCode;
	char			strTimeStamp[12];
	char			strDescription[60]; /* fix */
} STATUSDATAREP1;

typedef struct _tagStatusDataRep2 {
	unsigned short	usStatus;
	unsigned short	usSysType;
	unsigned int	unSysId;
	char			strDescription[60]; /* fix */
} STATUSDATAREP2;

typedef struct _tagStatusDataRep3 {
	unsigned short	usStatus;
	unsigned short	usSysType;
	unsigned int	unSysId;
	unsigned short	usErrorCode;
	char			strTimeStamp[12];
	char			strDescription[60]; /* fix */
} STATUSDATAREP3;

typedef struct _tagStatusDataRep4 {
	unsigned short	usStatus;
	unsigned short	usSysType;
	unsigned int	unSysId;
	unsigned short	usErrorCode;
	char			strTimeStamp[12];
	char			strDescription[60]; /* fix */
} STATUSDATAREP4;

typedef struct _tagStatusDataRep5 {
	unsigned short	usStatus;
	unsigned short	usSysType;
	unsigned int	unSysId;
	unsigned short	usErrorCode;
	char			strTimeStamp[12];
	char			strDescription[60]; /* fix */
} STATUSDATAREP5;






//PMSMQueue용 STRUCTURE
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


class bdspPacketC
{
public :
/*
	MSGH 			stMsgh;
	MSGCONREQ 		stMsgConReq;
	MSGCONREQACK	stMsgConReqAck;
	MSGDATAREP		stMsgDataRep;
	MSGDATAREPACK	stMsgDataRepAck;
	MSGSTREP		stMsgStRep;
*/


	MSGH                        stMsgh;
	CONFIGREQACK                stConfigReqAck;
	CONFIGRES                   stConfigRes;
	CONFIGRESACK                stConfigResAck;
	COLLECTIONSTARTINFO         stCollectionStartInfo;
	COLLECTIONSTARTINFOACK      stCollectionStartInfoAck;
	TRANSCONFIGCHANGEREQ        stTransConfigChangeReq;
	TRANSCONFIGCHANGEREQACK     stTransConfigChangeReqAck;
	BILLDATATRANSSTARTREQ       stBillDataTransStartReq;
	BILLDATATRANSSTARTREQACK    stBillDataTransStartReqAck;
	BILLDATAREP                 stBillDataRep;
	BILLDATAREPACK              stBillDataRepAck;
	DISKSTATUSREQ               stDiskStatusReq;
	DISKSTATUSREQACK            stDiskStatusReqAck;
	STATUSDATAREP1              stStatusDataRep1;
	STATUSDATAREP2              stStatusDataRep2;
	STATUSDATAREP3              stStatusDataRep3;
	STATUSDATAREP4              stStatusDataRep4;
	STATUSDATAREP5              stStatusDataRep5;

	int i;//for i Element

public :
	void setAll(
	unsigned short usMsgVersionId,
	unsigned short usSourceIdSType,
	unsigned short usSourceIdSId,
	unsigned short usDestIdSType,
	unsigned short usSestIdSId,
	unsigned short usMsgSeqNum,
	unsigned short usContinuousNum,
	unsigned short usMsgType,
	unsigned short usMsgId )
	{
		setMsgVersionId( usMsgVersionId );
		setSourceId( usSourceIdSType, usSourceIdSId );
		setDestId( usDestIdSType, usSestIdSId );
		setMsgSeqNum( usMsgSeqNum );
		setMsgConNum( usContinuousNum );
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
	void setMsgSeqNum( unsigned short usMsgSeqNum )   { stMsgh.usMsgSeqNum = usMsgSeqNum; }
	void setMsgConNum( unsigned short usContinuousNum )  { stMsgh.usContinuousNum = usContinuousNum; }
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
	unsigned int getMsgSeqNum( void )      { return stMsgh.usMsgSeqNum; }
	unsigned int getMsgConNum( void )      { return stMsgh.usContinuousNum; }
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
		cout << " getMsgSeqNum()       [" << getMsgSeqNum()           << "]" << endl;
		//performancecout << " getMsgConNum()       [" << getMsgConNum()           << "]" << endl;
		cout << " getMsgType()         [" << getMsgType()             << "]" << endl;
		cout << " getMsgId()           [" << getMsgId()               << "]" << endl;
		cout << " getMsgLen()          [" << getMsgLen()              << "]" << endl;
		switch(nMsgType)
		{
			case CONFIG_REQ://no data
				cout << " ===== bdspPacketC::CONFIG_REQ=====\n";
				break;
			case CONFIG_REQ_ACK:
				cout << " ===== bdspPacketC::CONFIG_REQ_ACK=====\n";
				cout << " usStatus           [" << stConfigReqAck.usStatus       << "]" << endl;
				cout << " usReason           [" << stConfigReqAck.usReason       << "]" << endl;
				cout << " strDescription     [" << stConfigReqAck.strDescription << "]" << endl;
				break;
			case CONFIG_RES:
				cout << " ===== bdspPacketC::CONFIG_RES=====\n";
				cout << " unMdId                                        [" << stConfigRes.unMdId   << "]" << endl;
				cout << " unSwitchCount                                 [" << stConfigRes.unSwitchCount  << "]" << endl;
				//preformancefor(i=0; i<10; i++)
				for(i=0; i<1; i++)
				{
				cout << " stRElement[" << i << "].unSwitchId                    [" << stConfigRes.stRElement[i].unSwitchId                    << "]" << endl;
				cout << " stRElement[" << i << "].stFileNoRange.unStartFileNo   [" << stConfigRes.stRElement[i].stFileNoRange.unStartFileNo   << "]" << endl;
				cout << " stRElement[" << i << "].stFileNoRange.unEndFileNo     [" << stConfigRes.stRElement[i].stFileNoRange.unEndFileNo     << "]" << endl;
				cout << " stRElement[" << i << "].stBlockNoRange.unStartBlockNo [" << stConfigRes.stRElement[i].stBlockNoRange.unStartBlockNo << "]" << endl;
				cout << " stRElement[" << i << "].stBlockNoRange.unEndBlockNo   [" << stConfigRes.stRElement[i].stBlockNoRange.unEndBlockNo   << "]" << endl;
				}
				break;
			case CONFIG_RES_ACK:
				cout << " ===== bdspPacketC::CONFIG_RES_ACK=====\n";
				cout << " usStatus                                      [" << stConfigResAck.usStatus       << "]" << endl;
				cout << " usReason                                      [" << stConfigResAck.usReason       << "]" << endl;
				cout << " unMdId                                        [" << stConfigResAck.unMdId         << "]" << endl;
				cout << " unSwitchCount                                 [" << stConfigResAck.unSwitchCount  << "]" << endl;
				//preformancefor(i=0; i<10; i++)
				for(i=0; i<1; i++)
				{
				cout << " stRElement[" << i << "].unSwitchId                    [" << stConfigResAck.stRElement[i].unSwitchId                    << "]" << endl;
				cout << " stRElement[" << i << "].stFileNoRange.unStartFileNo   [" << stConfigResAck.stRElement[i].stFileNoRange.unStartFileNo   << "]" << endl;
				cout << " stRElement[" << i << "].stFileNoRange.unEndFileNo     [" << stConfigResAck.stRElement[i].stFileNoRange.unEndFileNo     << "]" << endl;
				cout << " stRElement[" << i << "].stBlockNoRange.unStartBlockNo [" << stConfigResAck.stRElement[i].stBlockNoRange.unStartBlockNo << "]" << endl;
				cout << " stRElement[" << i << "].stBlockNoRange.unEndBlockNo   [" << stConfigResAck.stRElement[i].stBlockNoRange.unEndBlockNo   << "]" << endl;
				}
				break;
			case COLLECTION_START_INFO:
				cout << " ===== bdspPacketC::COLLECTION_START_INFO=====\n";
				cout << " unMdId                          [" << stCollectionStartInfo.unMdId        << "]" << endl;
				cout << " unSwitchCount                   [" << stCollectionStartInfo.unSwitchCount << "]" << endl;
				//preformancefor(i=0; i<10; i++)
				for(i=0; i<1; i++)
				{
				cout << " stSElement[" << i << "].unSwitchId      [" << stCollectionStartInfo.stSElement[i].unSwitchId     << "]" << endl;
				cout << " stSElement[" << i << "].unStartFileNo   [" << stCollectionStartInfo.stSElement[i].unStartFileNo  << "]" << endl;
				cout << " stSElement[" << i << "].unStartBlockNo  [" << stCollectionStartInfo.stSElement[i].unStartBlockNo << "]" << endl;
				}
				break;
			case COLLECTION_START_INFO_ACK:
				cout << " ===== bdspPacketC::COLLECTION_START_INFO_ACK=====\n";
				cout << " usStatus                        [" << stCollectionStartInfoAck.usStatus       << "]" << endl;
				cout << " usReason                        [" << stCollectionStartInfoAck.usReason       << "]" << endl;
				cout << " unMdId                          [" << stCollectionStartInfoAck.unMdId         << "]" << endl;
				cout << " unSwitchCount                   [" << stCollectionStartInfoAck.unSwitchCount  << "]" << endl;
				//preformancefor(i=0; i<10; i++)
				for(i=0; i<1; i++)
				{
				cout << " stSElement[" << i << "].unSwitchId      [" << stCollectionStartInfoAck.stSElement[i].unSwitchId     << "]" << endl;
				cout << " stSElement[" << i << "].unStartFileNo   [" << stCollectionStartInfoAck.stSElement[i].unStartFileNo  << "]" << endl;
				cout << " stSElement[" << i << "].unStartBlockNo  [" << stCollectionStartInfoAck.stSElement[i].unStartBlockNo << "]" << endl;
				}
				break;
			case TRANS_CONFIG_CHANGE_REQ:
				cout << " ===== bdspPacketC::TRANS_CONFIG_CHANGE_REQ=====\n";
				cout << " unTimeInVal    [" << stTransConfigChangeReq.unTimeInVal << "]" << endl;
				cout << " unMaxBillSize  [" << stTransConfigChangeReq.unMaxBillSize << "]" << endl;
				break;
			case TRANS_CONFIG_CHANGE_REQ_ACK:
				cout << " ===== bdspPacketC::TRANS_CONFIG_CHANGE_REQ_ACK=====\n";
				cout << " usStatus        [" << stTransConfigChangeReqAck.usStatus        << "]" << endl;
				cout << " usReason        [" << stTransConfigChangeReqAck.usReason        << "]" << endl;
				cout << " unTimeInVal     [" << stTransConfigChangeReqAck.unTimeInVal     << "]" << endl;
				cout << " unMaxBillSize   [" << stTransConfigChangeReqAck.unMaxBillSize   << "]" << endl;
				cout << " strDescription  [" << stTransConfigChangeReqAck.strDescription  << "]" << endl;
				break;
			case BILL_DATA_TRANS_START_REQ:
				cout << " ===== bdspPacketC::BILL_DATA_TRANS_START_REQ=====\n";
				cout << " unMdId            [" << stBillDataTransStartReq.unMdId        << "]" << endl;
				cout << " unSwitchCount     [" << stBillDataTransStartReq.unSwitchCount << "]" << endl;
				//preformancefor(i=0; i<10; i++)
				for(i=0; i<1; i++)
				{
				cout << " unSwitchId[" << i << "]   [" << stBillDataTransStartReq.unSwitchId  << "]" << endl;
				}
				break;
			case BILL_DATA_TRANS_START_REQ_ACK:
				cout << " ===== bdspPacketC::BILL_DATA_TRANS_START_REQ_ACK=====\n";
				cout << " usStatus        [" << stBillDataTransStartReqAck.usStatus        << "]" << endl;
				cout << " usReason        [" << stBillDataTransStartReqAck.usReason        << "]" << endl;
				cout << " strDescription  [" << stBillDataTransStartReqAck.strDescription  << "]" << endl;
				break;





			/**********************************************************************************************
			case BILL_DATA_REP:
				cout << " ===== bdspPacketC::BILL_DATA_REP=====\n";
				cout << " unSwitchId                    [" << stBillDataRep.unSwitchId                    << "]" << endl;
				cout << " usDataType                    [" << stBillDataRep.usDataType                    << "]" << endl;
				cout << " usCdrSize                     [" << stBillDataRep.usCdrSize                     << "]" << endl;
				cout << " unBlockSize                   [" << stBillDataRep.unBlockSize                   << "]" << endl;
				cout << " unFileNo                      [" << stBillDataRep.unFileNo                      << "]" << endl;
				cout << " stBlockNoRange.unStartBlockNo [" << stBillDataRep.stBlockNoRange.unStartBlockNo << "]" << endl;
				cout << " stBlockNoRange.unEndBlockNo   [" << stBillDataRep.stBlockNoRange.unEndBlockNo   << "]" << endl;
				cout << " strBillData                   [" << stBillDataRep.strBillData                   << "]" << endl;
				break;
			**********************************************************************************************/				
			case BILL_DATA_REP:
				cout << " ===== bdspPacketC::BILL_DATA_REP=====\n";
				cout << " unSwitchId                    [" << stBillDataRep.unSwitchId                    << "]" << endl;
				cout << " usDataType                    [" << stBillDataRep.usDataType                    << "]" << endl;
				cout << " usCdrSize                     [" << stBillDataRep.usCdrSize                     << "]" << endl;
				cout << " unBlockSize                   [" << stBillDataRep.unBlockSize                   << "]" << endl;
				cout << " unFileNo                      [" << stBillDataRep.unFileNo                      << "]" << endl;
				cout << " stBlockNoRange.unStartBlockNo [" << stBillDataRep.stBlockNoRange.unStartBlockNo << "]" << endl;
				cout << " stBlockNoRange.unEndBlockNo   [" << stBillDataRep.stBlockNoRange.unEndBlockNo   << "]" << endl;
/*				
				for(i=0; i<32; i++)
				{
				cout << " strBillData[" << i << "].usBlockId      [" << stBillDataRep.strBillData[i].usBlockId     << "]" << endl;
				cout << " strBillData[" << i << "].unBlockCount   [" << stBillDataRep.strBillData[i].unBlockCount  << "]" << endl;
				cout << " strBillData[" << i << "].unRecordCount  [" << stBillDataRep.strBillData[i].unRecordCount << "]" << endl;*/
//스트레스테스트
/*******************************************************************************************************				
				cout << " strBillData[" << i << "].strBillData1   [" << stBillDataRep.strBillData[i].strBillData1     << "]" << endl;
				cout << " strBillData[" << i << "].strBillData2   [" << stBillDataRep.strBillData[i].strBillData2     << "]" << endl;
				cout << " strBillData[" << i << "].strBillData3   [" << stBillDataRep.strBillData[i].strBillData3     << "]" << endl;								
				cout << " strBillData[" << i << "].strUnUsed   [" << stBillDataRep.strBillData[i].strUnUsed  << "]" << endl;
********************************************************************************************************/							
/*				}*/
				break;





			case BILL_DATA_REP_ACK:
				cout << " ===== bdspPacketC::BILL_DATA_REP_ACK=====\n";
				cout << " unSwitchId                    [" << stBillDataRepAck.unSwitchId                    << "]" << endl;
				cout << " unFileNo                      [" << stBillDataRepAck.unFileNo                       << "]" << endl;
				cout << " stBlockNoRange.unStartBlockNo [" << stBillDataRepAck.stBlockNoRange.unStartBlockNo << "]" << endl;
				cout << " stBlockNoRange.unEndBlockNo   [" << stBillDataRepAck.stBlockNoRange.unEndBlockNo   << "]" << endl;
				cout << " strTimeStamp                  [" << stBillDataRepAck.strTimeStamp                  << "]" << endl;
				cout << " usStatus                      [" << stBillDataRepAck.usStatus                      << "]" << endl;
				cout << " usReason                      [" << stBillDataRepAck.usReason                      << "]" << endl;
				cout << " strDescription                [" << stBillDataRepAck.strDescription                << "]" << endl;
				break;

			case DISK_STATUS_REQ:
				cout << " ===== bdspPacketC::DISK_STATUS_REQ=====\n";
				cout << " unMdId         [" << stDiskStatusReq.unMdId         << "]" << endl;
				cout << " unSwitchCount  [" << stDiskStatusReq.unSwitchCount  << "]" << endl;
				//preformancefor(i=0; i<10; i++)
				for(i=0; i<1; i++)
				{
				cout << " unSwitchId[" << i << "]" << "[" << stDiskStatusReq.unSwitchId[i] << "]" << endl;
				}
				break;
			case DISK_STATUS_REQ_ACK:
				cout << " ===== bdspPacketC::DISK_STATUS_REQ_ACK=====\n";
				cout << " unMdId             [" << stDiskStatusReqAck.unMdId         << "]" << endl;				
				cout << " usStatus           [" << stDiskStatusReqAck.usStatus       << "]" << endl;
				cout << " usErrorCode        [" << stDiskStatusReqAck.usErrorCode    << "]" << endl;
				cout << " strDescription     [" << stDiskStatusReqAck.strDescription << "]" << endl;
				cout << " unSwitchCount      [" << stDiskStatusReqAck.unSwitchCount  << "]" << endl;				
				//preformancefor(i=0; i<10; i++)
				for(i=0; i<1; i++)
				{
				cout << " stDElement[" << i << "].unSwitchId     [" << stDiskStatusReqAck.stDElement[i].unSwitchId     << "]" << endl;
				cout << " stDElement[" << i << "].usStatus       [" << stDiskStatusReqAck.stDElement[i].usStatus       << "]" << endl;
				cout << " stDElement[" << i << "].usErrorCode    [" << stDiskStatusReqAck.stDElement[i].usErrorCode    << "]" << endl;
				cout << " stDElement[" << i << "].strDescription [" << stDiskStatusReqAck.stDElement[i].strDescription << "]" << endl;
				}				
				break;
			case STATUS_DATA_REP1:
				cout << " ===== bdspPacketC::STATUS_DATA_REP1=====\n";
				cout << " usStatus        [" << stStatusDataRep1.usStatus        << "]" << endl;
				cout << " usSysType       [" << stStatusDataRep1.usSysType       << "]" << endl;
				cout << " unSysId         [" << stStatusDataRep1.unSysId         << "]" << endl;
				cout << " usErrorCode     [" << stStatusDataRep1.usErrorCode     << "]" << endl;
				cout << " strTimeStamp    [" << stStatusDataRep1.strTimeStamp    << "]" << endl;
				cout << " strDescription  [" << stStatusDataRep1.strDescription  << "]" << endl;
				break;
			case STATUS_DATA_REP2:
				cout << " ===== bdspPacketC::STATUS_DATA_REP2=====\n";
				cout << " usStatus        [" << stStatusDataRep2.usStatus        << "]" << endl;
				cout << " usSysType       [" << stStatusDataRep2.usSysType       << "]" << endl;
				cout << " unSysId         [" << stStatusDataRep2.unSysId         << "]" << endl;
				cout << " strDescription  [" << stStatusDataRep2.strDescription  << "]" << endl;
				break;
			case STATUS_DATA_REP3:
				cout << " ===== bdspPacketC::STATUS_DATA_REP3=====\n";
				cout << " usStatus        [" << stStatusDataRep3.usStatus        << "]" << endl;
				cout << " usSysType       [" << stStatusDataRep3.usSysType       << "]" << endl;
				cout << " unSysId         [" << stStatusDataRep3.unSysId         << "]" << endl;
				cout << " usErrorCode     [" << stStatusDataRep3.usErrorCode     << "]" << endl;
				cout << " strTimeStamp    [" << stStatusDataRep3.strTimeStamp    << "]" << endl;
				cout << " strDescription  [" << stStatusDataRep3.strDescription  << "]" << endl;
				break;
			case STATUS_DATA_REP4:
				cout << " ===== bdspPacketC::STATUS_DATA_REP4=====\n";
				cout << " usStatus        [" << stStatusDataRep4.usStatus        << "]" << endl;
				cout << " usSysType       [" << stStatusDataRep4.usSysType       << "]" << endl;
				cout << " unSysId         [" << stStatusDataRep4.unSysId         << "]" << endl;
				cout << " usErrorCode     [" << stStatusDataRep4.usErrorCode     << "]" << endl;
				cout << " strTimeStamp    [" << stStatusDataRep4.strTimeStamp    << "]" << endl;
				cout << " strDescription  [" << stStatusDataRep4.strDescription  << "]" << endl;
				break;
			case STATUS_DATA_REP5:
				cout << " ===== bdspPacketC::STATUS_DATA_REP5=====\n";
				cout << " usStatus        [" << stStatusDataRep5.usStatus        << "]" << endl;
				cout << " usSysType       [" << stStatusDataRep5.usSysType       << "]" << endl;
				cout << " unSysId         [" << stStatusDataRep5.unSysId         << "]" << endl;
				cout << " usErrorCode     [" << stStatusDataRep5.usErrorCode     << "]" << endl;
				cout << " strTimeStamp    [" << stStatusDataRep5.strTimeStamp    << "]" << endl;
				cout << " strDescription  [" << stStatusDataRep5.strDescription  << "]" << endl;
				break;
			case STATUS_DATA_REP_ACK://no data
				cout << " ===== bdspPacketC::STATUS_DATA_REP_ACK=====\n";
				break;
			case STATUS_REQ://no data
				cout << " ===== bdspPacketC::STATUS_REQ=====\n";
				break;
			case STATUS_REQ_ACK://no data//ihy 10.07
				cout << " ===== bdspPacketC::STATUS_REQ_ACK=====\n";
				break;
			case DISCONNECT_REQ://no data
				cout << " ===== bdspPacketC::DISCONNECT_REQ=====\n";
				break;
			default:
				break;
		}
		//performancecout << " ===== END=======================================================\n";
		cout<<"\n\n";
	}

	void printAll( void )
	{
		//performancecout << " ===== bdspPacketC::printAll()=====\n";
		//performancecout << " stMsgh.usMsgVersionId         [" << stMsgh.usMsgVersionId         << "]" << endl;
		//performancecout << " stMsgh.SourceId.usServiceType [" << stMsgh.SourceId.usServiceType << "]" << endl;
		//performancecout << " stMsgh.SourceId.usSystemId    [" << stMsgh.SourceId.usSystemId    << "]" << endl;
		//performancecout << " stMsgh.DestId.usServiceType   [" << stMsgh.DestId.usServiceType   << "]" << endl;
		//performancecout << " stMsgh.DestId.usSystemId      [" << stMsgh.DestId.usSystemId      << "]" << endl;
		//performancecout << " stMsgh.usMsgSeqNum            [" << stMsgh.usMsgSeqNum            << "]" << endl;
		//performancecout << " stMsgh.usContinuousNum        [" << stMsgh.usContinuousNum        << "]" << endl;		
		//performancecout << " stMsgh.usMsgType              [" << stMsgh.usMsgType              << "]" << endl;
		//performancecout << " stMsgh.usMsgId                [" << stMsgh.usMsgId                << "]" << endl;
		//performancecout << " stMsgh.unMsgLen               [" << stMsgh.unMsgLen               << "]" << endl;
		//performancecout << " ===== bdspPacketC::printAll()=====\n";
	}


	bdspPacketC()
	{
		////cout << " bdspPacketC::bdspPacketC() Start Constructor!!!!!!!!!!!\n";
		init();
		////setAll(0xffff, 0xffff, 0xffff,  0xffff, 0xffff, 0xffffffff, 0xffff, 0xffff);
		////cout << " bdspPacketC::bdspPacketC() End Constructor!!!!!!!!!!!\n";
	}
	~bdspPacketC()
	{
		//performancecout << " bdspPacketC::bdspPacketC() Destructor!!!!!!!!!!!\n";
	}
};





