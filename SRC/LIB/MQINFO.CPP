#include <MqInfo.hpp>

MessageQueueInfo::MessageQueueInfo()
{
	memset (strMsgQueueName, NULL, 12);    //  Format:?????_?????, Len : 12
	unRecSize		= 0;
    unRecCount		= 0;
//	unHeadOffset	= unTailOffset	= 0;
    unSendDataSeq	= 0;
    unRecvDataSeq	= 0;
    nFlagMqStatus	= FLAG_MQ_IS_EMPTY;
}

MessageQueueInfo::MessageQueueInfo(	unsigned int nJobId1,	\
				                    unsigned int nProcId1,	\
				                    unsigned int nJobId2,	\
				                    unsigned int nProcId2,	\
				                    unsigned int nRecSize,	\
				                    unsigned int nCount
								)
{
	set_MsgQueueName( nJobId1, nProcId1, nJobId2, nProcId2 );
	set_unRecSize( nRecSize );
	set_unRecCount( nCount );
//	unHeadOffset	= unTailOffset	= 0;
	unSendDataSeq	= unRecvDataSeq	= 0;
	nFlagMqStatus	= FLAG_MQ_IS_EMPTY;
}

MessageQueueInfo::MessageQueueInfo(	int			nSystemId,		\
									int			nServiceId,		\
									int			nGroupId,		\
									int			nProcessType,	\
									int			nSerialNo,		\
									int			nRemark,		\
		            				int 		nRecSize,		\
		            				int 		nCount)
{
	set_MsgQueueName( 	nSystemId,		\
						nServiceId,		\
						nGroupId,		\
						nProcessType,	\
						nSerialNo,		\
						nRemark);
	set_unRecSize( nRecSize );
	set_unRecCount( nCount );
//	unHeadOffset	= unTailOffset	= 0;
	unSendDataSeq	= unRecvDataSeq	= 0;
	nFlagMqStatus	= FLAG_MQ_IS_EMPTY;
}

void MessageQueueInfo::set_MsgQueueName (	unsigned int nJobId1,		\
			                                unsigned int nProcId1,		\
			                                unsigned int nJobId2,		\
			                                unsigned int nProcId2   )
{
    sprintf(strMsgQueueName, "%02d%03d_%02d%03d",   nJobId1, nProcId1,	\
                                                    nJobId2, nProcId2 );
}

void MessageQueueInfo::set_MsgQueueName(	int			nSystemId,		\
											int			nServiceId,		\
											int			nGroupId,		\
											int			nProcessType,	\
											int			nSerialNo,		\
											int			nRemark)
{
	sprintf(strMsgQueueName, "%01d%01d%01d%01d%02d%02d",	nSystemId,		\
															nServiceId,		\
															nGroupId,		\
															nProcessType,	\
															nSerialNo,		\
															nRemark);
}

void MessageQueueInfo::set_MsgQueueName (const char *strMqName)
{
	strcpy(strMsgQueueName, strMqName);
}

void MessageQueueInfo::set_unRecSize (unsigned int unRecordSize)
{
	unRecSize   	= unRecSize;
}

void MessageQueueInfo::set_unRecCount (unsigned int unRecCnt)
{
	unRecCount  	= unRecCnt;
}

void MessageQueueInfo::set_unSendDataSeq (unsigned int unTailLoc)
{
	unSendDataSeq	= unTailLoc;
}

void MessageQueueInfo::set_unRecvDataSeq (unsigned int unTailLoc)
{
	unRecvDataSeq	= unTailLoc;
}

void MessageQueueInfo::set_unHeadOffset (unsigned int unHeadLoc)
{
	unHeadOffset = unHeadLoc;
}

void MessageQueueInfo::set_unTailOffset (unsigned int unTailLoc)
{
	unTailOffset = unTailLoc;
}

int	MessageQueueInfo::get_unHeadOffset()
{
	int		nTempHead;

	nTempHead = ((unHeadOffset+1) % unRecCount);

//	cout << "get_unHeadOffset() = " << unHeadOffset << endl;
//	cout << "get_unTailOffset() = " << unTailOffset << endl;

	if (nTempHead == unTailOffset)
		return FLAG_MQ_IS_FULL;

	return unHeadOffset;
}

int	MessageQueueInfo::get_unTailOffset()
{
	int		nTempTail;

//	cout << "get_unHeadOffset() = " << unHeadOffset << endl;
//	cout << "get_unTailOffset() = " << unTailOffset << endl;

	if (unTailOffset == unHeadOffset)
		return FLAG_MQ_IS_EMPTY;

	return unTailOffset;
}

unsigned int MessageQueueInfo::get_AmountOfData()
{
	int		nAmountOfData;

	nAmountOfData = unHeadOffset - unTailOffset;

	if(nAmountOfData <= 0)
		nAmountOfData += unRecCount;

	return nAmountOfData;
}
