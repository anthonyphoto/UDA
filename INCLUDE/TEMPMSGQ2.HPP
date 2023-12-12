#ifndef	TEMPMSGQ_HPP
#define TEMPMSGQ_HPP

#include <iostream.h>
#include <fstream.h>
#include <unistd.h>		// for sleep
#include "SharedMemLegacy.hpp"
#include "SharedConf.hpp"
#include "MqInfo.hpp"
#include "time.hpp"


const	unsigned int	LEN_MQ_INFO			= sizeof(MessageQueueInfo);

const	char*			PATH_MQ				= "MQ/";

template <class T>
class MessageQueue  : public ShmMemory, public MessageQueueInfo
{
	public:
		MessageQueue(int nCount,  						       \
                     int nJobId1, int nProcId1,         	   \
                     int nJobId2, int nProcId2 );
		MessageQueue(const MessageQueue& rhs,                  \
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
        
        void	makeLogFileName();
        

	private:
		T*              	pType;
		int 	        	itsSize;
		ofstream*	    	recvDataFile,
							sendDataFile;
		MessageQueueInfo*	MqInfo;
};


#endif


