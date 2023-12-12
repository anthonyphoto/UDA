#ifndef MQINFO_HPP
#define MQINFO_HPP

#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <unistd.h>

const	int				FLAG_MQ_IS_FULL		= -1;
const	int				FLAG_MQ_IS_EMPTY	= -2;
const	int				FLAG_MQ_IS_INIT		= -3;


class MsgQfileInfo
{
	private:
		//  Message Queue Info
		char			strMsgQueueName[64];    //  Format:?????_?????, Len : 12
		unsigned int	unRecSize;
	    unsigned int	unRecCount;
	    unsigned int	unHeadOffset;
	    unsigned int	unTailOffset;

	public:
		//	Constructor
				MsgQfileInfo();
				MsgQfileInfo	(	int			nSystemId,		\
									int			nServiceId,		\
									int			nGroupId,		\
									int			nProcessType,	\
									int			nSerialNo,		\
									int			nRemark,			\
									int			n
								);

				void 			set_MsgQueueName(	int		nSystemId,		\
													int		nServiceId,		\
													int		nGroupId,		\
													int		nProcessType,	\
													int		nSerialNo,		\
													int		nRemark
												);
				void 			set_MsgQueueName(const char *strMqName);
				void 			set_unRecSize	(unsigned int unRecordSize);
				void 			set_unRecCount 	(unsigned int unRecCnt);
				void 			set_unHeadOffset(unsigned int unHeadLoc);
				void 			set_unTailOffset(unsigned int unTailLoc);

				char*			get_strMsgQueueName();
				unsigned int	get_unRecSize();
	    		unsigned int	get_unRecCount();
				int				get_unHeadOffset();
				int				get_unTailOffset();
				unsigned int 	get_AmountOfData();

				int				open_MqFile		(char*	strMqFileName);
				int				close_MqFile	(int fp);

				void 			read_Mqinfo		(fstream* fQFile);
				int 			read_Mqinfo		(int fp);
				int 			read_Mqinfo		();

				void 			write_Mqinfo	(fstream* fQFile);
				int 			write_Mqinfo	(int fp);
				void 			write_Mqinfo	();


				int 			get_OffsetOfTail();
				int 			get_OffsetOfHead();

				int 			put_Message		(const void* strMsg, int fp);
				int				get_Message		(void* strMsg, int fp);

				int 			create_MqFile	(	int		nSystemId,		\
													int		nServiceId,		\
													int		nGroupId,		\
													int		nProcessType,	\
													int		nSerialNo,		\
													int		nRemark,		\
													int		nRecSize,		\
													int		nRecCnt
												);

};


#endif

