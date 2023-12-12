#ifndef	TEMPMSGQ_HPP
#define EMPMSGQ_HPP

#include <iostream.h>
#include <fstream.h>
#include <unistd.h>		// for sleep
#include "/home1/uda/cwkwon/SharedMem/SharedMemLegacy1.hpp"
#include "/home1/uda/cwkwon/CONFIG/SharedConf.hpp"
#include "/home1/uda/cwkwon/IPC/MqInfo1.hpp"


const	unsigned int	LEN_MQ_INFO			= sizeof(MessageQueueInfo);

template <class T>
class MessageQueue  : public ShmMemory, public MessageQueueInfo
{
	public:
		MessageQueue(const char *strFile, int nCount,          \
                     int nJobId1, int nProcId1,                \
                     int nJobId2, int nProcId2 );
		MessageQueue(const char *strFile,                      \
		             const MessageQueue& rhs,                  \
                     int nJobId1, int nProcId1,                \
                     int nJobId2, int nProcId2 );
		
		~MessageQueue();
		
		MessageQueue& 		operator 	=	(const MessageQueue&);
		T&                  operator 	[] 	(int offset) { return pType[offset]; }
		const T&            operator	[] 	(int offset) const { return pType[offset]; }
		
		int 	GetSize() const { return itsSize; }
        
        int 	getBitValue( int nValue, int nBaseOffset, int nLen );
        int 	getFtokKeyId( int nJobId1, int nProcId1, int nJobId2, int nProcId2 );
        
        int		recvMsg (T* OneRecord);
        int		sendMsg (const T* OneRecord);
        

	private:
		T*              	pType;
		int 	        	itsSize;
		char*           	strFileName;
		ofstream*	    	outfile;
/*		ofstream*	    	recvDataFile,
							sendDataFile;
*/		MessageQueueInfo*	MqInfo;
};

template <class T>
MessageQueue<T>::MessageQueue		(  	const char *strFile, int nCount,	\
                                		int nJobId1, int nProcId1,      	\
                                		int nJobId2, int nProcId2 )     	\
				: itsSize			(	nCount								\
									),										\
				  ShmMemory			(	PATH_NAME_SHM_IPC,					\
                            			getFtokKeyId(	nJobId1, nProcId1,	\
                                    					nJobId2, nProcId2 ),\
				            			sizeof(MessageQueueInfo) + 			\
				            			nCount*sizeof(T),					\
				            			0660|IPC_CREAT						\
				            		),										\
				  MessageQueueInfo	(	nJobId1, nProcId1, nJobId2,			\
				  						nProcId2, sizeof(T), nCount			\
				  					)
{
	unsigned int*	unTempBaseAddr;
	
	//	get SHM address
	unTempBaseAddr 	= (unsigned int*)ShmPtr();
	MqInfo 			= (MessageQueueInfo*)unTempBaseAddr;
	pType 			= (T*)((unsigned int)unTempBaseAddr+LEN_MQ_INFO);

	// put MessageQueueInfo's Data
	MqInfo->set_MsgQueueName(get_MsgQueueName());
	MqInfo->set_unRecSize(get_unRecSize());
	MqInfo->set_unRecCount(get_unRecCount());
	MqInfo->set_unSendDataSeq (0);
    MqInfo->set_unRecvDataSeq (0);
//    MqInfo->set_unHeadOffset (0);
//    MqInfo->set_unTailOffset (0);
    
    //initialize the pType
    for(int i = 0; i < nCount; i++)
		memset ((char*)&pType[i], NULL, sizeof(T));
	
	strFileName = new char[strlen(strFile)+1];
	memcpy(strFileName, strFile, strlen(strFile));
	strFileName[strlen(strFile)] = NULL;
	
	outfile = new ofstream(strFileName, ios::app);
}

template <class T>
MessageQueue<T>::MessageQueue		(	const char *strFile,				\
										const MessageQueue& rhs,			\
	                            	    int nJobId1, int nProcId1,			\
	                            	    int nJobId2, int nProcId2 )			\
                : ShmMemory			(	PATH_NAME_SHM_IPC,					\
                            			getFtokKeyId( nJobId1, nProcId1,	\
                                    			      nJobId2, nProcId2 ),	\
                            			rhs.GetSize(),						\
				            			0660|IPC_CREAT						\
				            		),										\
				  MessageQueueInfo	(	nJobId1, nProcId1, nJobId2,			\
				  						nProcId2, sizeof(T), rhs.GetSize()	\
				  					)
{
	itsSize = rhs.GetSize();
	
	//	get SHM address
	MqInfo = (MessageQueue*)ShmPtr();
	pType = (T*)MqInfo + LEN_MQ_INFO;
	
	// put MessageQueueInfo's Data
	MqInfo->set_MsgQueueName(get_MsgQueueName());
	MqInfo->set_unRecSize(get_unRecSize());
	MqInfo->set_unRecCount(get_unRecCount());
	MqInfo->set_unSendDataSeq (0);
    MqInfo->set_unRecvDataSeq (0);
    MqInfo->set_unHeadOffset (0);
    MqInfo->set_unTailOffset (0);
    
	for(int i = 0; i < itsSize; i++)
		pType[i] = rhs[i];
	
	strFileName = new char[strlen(strFile)+1];
	memcpy(strFileName, strFile, strlen(strFile));
	strFileName[strlen(strFile)] = NULL;
	
    outfile = new ofstream(strFileName, ios::binary);
}

template <class T>
MessageQueue<T>::~MessageQueue()
{
	delete []   strFileName;
	delete      outfile;
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
int MessageQueue<T>::getFtokKeyId( int nJobId1, int nProcId1, int nJobId2, int nProcId2 )
{
    int     nResult = 0;
            
    if ( (nJobId1 < 64) && (nProcId1 < 256) && (nJobId2 < 64) && (nProcId2 < 256) )
    {
        nResult |= nJobId1  << ( (sizeof(int)*8 - OFFSET_JOB_ID_1)  - LEN_JOB_ID);
        nResult |= nProcId1 << ( (sizeof(int)*8 - OFFSET_PROC_ID_1) - LEN_PROC_ID);
        nResult |= nJobId2  << ( (sizeof(int)*8 - OFFSET_JOB_ID_2)  - LEN_JOB_ID);
        nResult |= nProcId2 << ( (sizeof(int)*8 - OFFSET_PROC_ID_2) - LEN_PROC_ID);
        
        nResult %= 0xFF;
        
        return (nResult);
    }
    else
    {
     	cout << "getFtokKeyId Fail" << endl;
        return -1;
	}
}

template <class T>
int MessageQueue<T>::recvMsg(T* OneRecord)
{
	int		nCurrTailOffset = 0,
			nNextTailOffset = 0,
			nReturnValue = 0;
	
	nReturnValue = shm_lock();
	
	nCurrTailOffset = MqInfo->get_unTailOffset();
	
	memset ((char*)OneRecord, NULL, sizeof(T));
	memcpy ((char*)OneRecord, (char*)&pType[nCurrTailOffset], sizeof(T));
	
	nNextTailOffset = MqInfo->count_unTailOffset();
	
	if (nNextTailOffset == FLAG_MQ_IS_EMPTY)
	{
		MqInfo->set_unTailOffset (nCurrTailOffset);
		shm_release();
		return FLAG_MQ_IS_EMPTY;
	}
	else
	{
		MqInfo->set_unTailOffset (nNextTailOffset);
		shm_release();	
		return 0;
	}
}

template <class T>
int MessageQueue<T>::sendMsg(const T* OneRecord)
{
	int		nCurrHeadOffset = 0,
			nNextHeadOffset = 0,
			nReturnValue = 0;
			
	nReturnValue = shm_lock();
	
	if (nReturnValue < 0)
		error("shm_lock Fail");
	
	nCurrHeadOffset = MqInfo->get_unHeadOffset();
	
	memcpy ((char*)&pType[nCurrHeadOffset], (char*)OneRecord, sizeof(T));
	
	nNextHeadOffset = MqInfo->count_unHeadOffset();
	
	if (nNextHeadOffset == FLAG_MQ_IS_FULL)
	{
		MqInfo->set_unHeadOffset (nCurrHeadOffset);
		shm_release();
		return FLAG_MQ_IS_FULL;
	}
	else
	{
		MqInfo->set_unHeadOffset (nNextHeadOffset);
		shm_release();	
		return 0;
	}
}
#endif


