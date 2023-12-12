#ifndef	TEMPMSGQ_HPP
#define TEMPMSGQ_HPP

#include <iostream.h>
#include <fstream.h>
#include <unistd.h>		// for sleep

#ifndef	SHAREDMEMLEGACY_HPP
#include <SharedMemLegacy.hpp>
#endif

#ifndef	MQINFO_HPP
#include <MqInfo.hpp>
#endif

const	unsigned int	LEN_MQ_INFO			= sizeof(MessageQueueInfo);

template <class T>
class MessageQueue  : public SharedMemory, public MessageQueueInfo
{
	public:
		MessageQueue(  	int		nSystemId,		\
						int		nServiceId,		\
						int		nGroupId,		\
						int		nProcessType,	\
						int		nSerialNo,		\
						int		nRemark,		\
						int		nCount			\
					);
		MessageQueue(  	int		nSystemId,		\
						int		nServiceId,		\
						int		nGroupId,		\
						int		nProcessType,	\
						int		nSerialNo,		\
						int		nRemark			\
					);
		~MessageQueue();

		MessageQueue& 		operator 	=	(const MessageQueue&);
		T&                  operator 	[] 	(int offset) { return pType[offset]; }
		const T&            operator	[] 	(int offset) const { return pType[offset]; }

		int 	GetSize() const { return itsSize; }

        int 	getBitValue( int nValue, int nBaseOffset, int nLen );

        int		recvMsg (T* OneRecord);
        int		sendMsg (const T* OneRecord);
        int		get_AmountOfData()	{	return	MqInfo->get_AmountOfData();	}


	private:
		T*              	pType;
		int 	        	itsSize;
		MessageQueueInfo*	MqInfo;
};

template <class T>
MessageQueue<T>::MessageQueue		(  	int		nSystemId,		\
										int		nServiceId,		\
										int		nGroupId,		\
										int		nProcessType,	\
										int		nSerialNo,		\
										int		nRemark,		\
										int		nCount			\
									)							\
				: itsSize			(			nCount			\
									),							\
				  SharedMemory		(  			nSystemId,		\
												nServiceId,		\
												nGroupId,		\
												nProcessType,	\
												nSerialNo,		\
												nRemark,
												sizeof(MessageQueueInfo) + nCount*sizeof(T)\
									),							\
				  MessageQueueInfo	(			nSystemId,		\
												nServiceId,		\
												nGroupId,		\
												nProcessType,	\
												nSerialNo,		\
												nRemark,		\
												sizeof(T),		\
												nCount			\
				  					)
{
	unsigned long*	unTempBaseAddr;

	//	get SHM address
	unTempBaseAddr 	= (unsigned long*)get_ShmPtr();
	MqInfo 			= (MessageQueueInfo*)unTempBaseAddr;
	pType 			= (T*)((unsigned long)unTempBaseAddr+LEN_MQ_INFO);

	// put MessageQueueInfo's Data
	MqInfo->set_MsgQueueName(get_MsgQueueName());
	MqInfo->set_unRecSize(get_unRecSize());
	MqInfo->set_unRecCount(get_unRecCount());
	MqInfo->set_unSendDataSeq (0);
    MqInfo->set_unRecvDataSeq (0);
}

template <class T>
MessageQueue<T>::~MessageQueue()
{
	;
}

// operator =
template <class T>
MessageQueue<T>& MessageQueue<T>::operator= (const MessageQueue& rhs)
{
	if (this == &rhs)
		return *this;

	delete [] pType;
	itsSize = rhs.GetSize();
	pType = new T[itsSize];
	for(int i = 0; i < itsSize; i++)
		pType[i] = rhs[i];

	return *this;
}

template <class T>
int MessageQueue<T>::getBitValue( int nValue, int nBaseOffset, int nLen )
{
    int     nFilterMask = 0,
            nTempFilterMask = 0,
            nTempMask   = 0x80000000;

    for( int i = nBaseOffset; i < (nBaseOffset+nLen); i++)  //  Make a Filtering Mask
        nFilterMask |= nTempMask >> i;

    nFilterMask &= nValue;     // Filtering the Input data

    nFilterMask >>= ((sizeof(int)*8)-(nBaseOffset+nLen));

    return ( nFilterMask );
}

template <class T>
int MessageQueue<T>::recvMsg(T* OneRecord)
{
	int		nCurrTailOffset = 0,
			nReturnValue = 0;

	nReturnValue = mem_lock();
	if (nReturnValue < 0)
		cout << "FAIL : mem_lock Fail" << endl;

	//	get Current Tail Offset
	nCurrTailOffset = MqInfo->get_unTailOffset();

	if (nCurrTailOffset == FLAG_MQ_IS_EMPTY)
	{
		if (mem_unlock() < 0)
			cout << "FAIL : mem_unlock Fail" << endl;
//	cout << "At LIB MQ is Empty" << endl;
//	cout << "At LIB get_unHeadOffset= " << MqInfo->get_unHeadOffset() << endl;
//	cout << "At LIB get_unTailOffset= " << nCurrTailOffset			<< endl;
//	cout << "At LIB get_AmountOfData= " << MqInfo->get_AmountOfData() << endl;
		return FLAG_MQ_IS_EMPTY;
	}
	else
	{
		//	Move Data to MQ
		memset ((char*)OneRecord, NULL, sizeof(T));
		memcpy ((char*)OneRecord, (char*)&pType[nCurrTailOffset], sizeof(T));

//	cout << "Before Save At LIB get_unTailOffset= " << nCurrTailOffset		<< endl;

		//	Save Next Tail Offset
		MqInfo->set_unTailOffset ((nCurrTailOffset+1) % MqInfo->get_unRecCount());

//	cout << "At LIB get_unHeadOffset= " << MqInfo->get_unHeadOffset() << endl;

//	cout << "At LIB get_AmountOfData= " << MqInfo->get_AmountOfData() << endl;

		if (mem_unlock() < 0)
			cout << "FAIL : mem_unlock Fail" << endl;

		return 0;
	}
}

template <class T>
int MessageQueue<T>::sendMsg(const T* OneRecord)
{
	int		nCurrHeadOffset = 0,
			nReturnValue = 0;

	nReturnValue = mem_lock();

	if (nReturnValue < 0)
		cout << "FAIL : mem_lock Fail" << endl;

	//	get Current Head Offset
	nCurrHeadOffset = MqInfo->get_unHeadOffset();

	//	chekc MQ Full
	if (nCurrHeadOffset == FLAG_MQ_IS_FULL)
	{
		if (mem_unlock() < 0)
			cout << "FAIL : mem_unlock Fail" << endl;

//	cout << "At LIB MQ is Full" << endl;
//	cout << "At LIB get_unHeadOffset= " << nCurrHeadOffset			<< endl;
//	cout << "At LIB get_unTailOffset= " << MqInfo->get_unTailOffset() << endl;
//	cout << "At LIB get_AmountOfData= " << MqInfo->get_AmountOfData() << endl;

		return FLAG_MQ_IS_FULL;
	}
	else
	{
		//	Move Data to MQ
		memcpy ((char*)&pType[nCurrHeadOffset], (char*)OneRecord, sizeof(T));

		//	Save Next Head Offset
		MqInfo->set_unHeadOffset ((nCurrHeadOffset+1) % MqInfo->get_unRecCount());

//	cout << "At LIB get_unHeadOffset= " << nCurrHeadOffset << endl;
//	cout << "At LIB get_unTailOffset= " << MqInfo->get_unTailOffset() << endl;
//	cout << "At LIB get_AmountOfData= " << MqInfo->get_AmountOfData() << endl;

		if (mem_unlock() < 0)
			cout << "FAIL : mem_unlock Fail" << endl;
		return 0;
	}
}


#endif


