#ifndef MQINFO_HPP
#define MQINFO_HPP

#include <iostream.h>
#include <stdlib.h>

const   unsigned int    LEN_JOB_ID          = 6;
const   unsigned int    LEN_PROC_ID         = 8;

const   unsigned int    OFFSET_JOB_ID_1     = 2;
const   unsigned int    OFFSET_PROC_ID_1    = OFFSET_JOB_ID_1 + LEN_JOB_ID;
const   unsigned int    OFFSET_JOB_ID_2     = OFFSET_PROC_ID_1 + LEN_PROC_ID+2;
const   unsigned int    OFFSET_PROC_ID_2    = OFFSET_JOB_ID_2 + LEN_JOB_ID;

const	int				FLAG_MQ_IS_FULL		= -1;
const	int				FLAG_MQ_IS_EMPTY	= -2;

class MessageQueueInfo
{
	private:
		//  Message Queue Info
		char			strMsgQueueName[12];    //  Format:?????_?????, Len : 12
		unsigned int	unRecSize;
	    unsigned int	unRecCount;
	    unsigned int	unHeadOffset;
	    unsigned int	unTailOffset;
	    unsigned int	unSendDataSeq;
		unsigned int	unRecvDataSeq;
		int				nFlagMqStatus;

	public:
		//	Constructor
	    MessageQueueInfo();
	    MessageQueueInfo				(	unsigned int nJobId1,		\
				            				unsigned int nProcId1,		\
				              				unsigned int nJobId2,		\
				            				unsigned int nProcId2,		\
				            				unsigned int nRecSize,		\
				            				unsigned int nCount);

		MessageQueueInfo				(	int			nSystemId,		\
											int			nServiceId,		\
											int			nGroupId,		\
											int			nProcessType,	\
											int			nSerialNo,		\
											int			nRemark,		\
				            				int 		nRecSize,		\
				            				int 		nCount);
		//	SET Value Operations
		void			set_MsgQueueName(	unsigned int nJobId1,		\
			                                unsigned int nProcId1,		\
			                                unsigned int nJobId2,		\
			                                unsigned int nProcId2);
		void			set_MsgQueueName(	int			nSystemId,		\
											int			nServiceId,		\
											int			nGroupId,		\
											int			nProcessType,	\
											int			nSerialNo,		\
											int			nRemark);
		void			set_MsgQueueName 	(const char *strMqName);
        void			set_unRecSize 		(unsigned int unRecordSize);
        void 			set_unRecCount 		(unsigned int unRecCnt);
        void 			set_unSendDataSeq	(unsigned int unTailLoc);
        void 			set_unRecvDataSeq	(unsigned int unTailLoc);
		void 			set_unHeadOffset	(unsigned int unHeadLoc);
        void 			set_unTailOffset	(unsigned int unTailLoc);

		//	GET Value Operations
		char*			get_MsgQueueName()		{	return strMsgQueueName;	}
        unsigned int	get_unRecSize()const 	{	return unRecSize;		}
        unsigned int	get_unRecCount()const	{	return unRecCount;		}
        unsigned int	get_unSendDataSeq()const{	return unSendDataSeq;	}
        unsigned int	get_unRecvDataSeq()const{	return unRecvDataSeq;	}
		unsigned int	get_AmountOfData();
		int				get_unHeadOffset();
        int				get_unTailOffset();

        int				count_unTailOffset();
        int				count_unHeadOffset();
};


#endif

