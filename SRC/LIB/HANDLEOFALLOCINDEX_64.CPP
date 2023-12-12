#include <HandleOfAllocIndex_64.hpp>

HandleOfAllocIndex::HandleOfAllocIndex(const unsigned char ucShmID,		\
										 const unsigned int unMaxData)	\
	: ShmMemory(PATH_NAME_SHM, ucShmID,									\
				((sizeof(unsigned long)*4)+(getAllocSize(unMaxData)*8)), 0660|IPC_CREAT)
{
	//cout << ucShmID << " " << unMaxData << endl;
	//cout << "i'm HandleOfAllocIndex Constructor!!!" << endl;
	
	//	Address 할당
	ulBaseAddress 	= (unsigned long*)ShmPtr();
	ulSavedPosition	= sizeof(unsigned long) + ulBaseAddress;
	ulMaxDataCount	= sizeof(unsigned long) + ulSavedPosition;
	ulDataCnt		= sizeof(unsigned long) + ulMaxDataCount;
	ullFlag			= (UINT64*)(sizeof(unsigned long) + ulDataCnt);
	
	//cout << "ulBaseAddress " << ulBaseAddress << endl;
	
	//shm_lock();

	set_ulSavedPosition(0);
	set_BaseAddress(0);
	set_ulMaxDataCount(unMaxData);
    
    ulDataCount = get_ulDataCnt();
    
	//shm_release();

}

HandleOfAllocIndex::HandleOfAllocIndex(const unsigned char ucShmID,		\
										const unsigned int unMaxData,	\
										unsigned long unStartAddress)	\
	: ShmMemory(PATH_NAME_SHM, ucShmID,									\
				((sizeof(unsigned long)*4)+(getAllocSize(unMaxData)*8)), 0660|IPC_CREAT)
{
	//cout << "i'm HandleOfAllocIndex Constructor!!!" << endl;
	//	Address 할당 시작
	ulBaseAddress 	= (unsigned long*)ShmPtr();
	ulSavedPosition	= sizeof(unsigned long) + ulBaseAddress;
	ulMaxDataCount	= sizeof(unsigned long) + ulSavedPosition;
	ulDataCnt		= sizeof(unsigned long) + ulMaxDataCount;
	ullFlag			= (UINT64*)(sizeof(unsigned long) + ulDataCnt);
	//	Address 할당 끝

	//cout << "ulBaseAddress " << ulBaseAddress << endl;
	
	//shm_lock();

	set_ulSavedPosition(0);
	set_BaseAddress(unStartAddress);
	set_ulMaxDataCount(unMaxData);
	
	ulDataCount = get_ulDataCnt();
		
//	shm_release();
}

void HandleOfAllocIndex::save_ulSavedPosition(unsigned long ulPosition)
{
	if (ulPosition >= get_ulMaxDataCount())
    {
//		set_ulSavedPosition(1);
		set_ulSavedPosition(0);
	//	cout << "118 get_ulSavedPosition() = " << get_ulSavedPosition() << endl;
		
	}
	else
	{
		set_ulSavedPosition(ulPosition);
	 //   cout << "124get_ulSavedPosition() = " << get_ulSavedPosition() << endl;
	}
}

int HandleOfAllocIndex::getOffsetOfEmptyRoom()
{
	unsigned long	ulPosition;
	
	switch (checkFlagOfSavedPosition())     //  현재 Index의 상태를 반환
	{
		case	FLAG_ROOM_IS_EMPTY:         //  비어 있으면
			ulPosition = get_ulSavedPosition();
			save_ulSavedPosition(ulPosition + 1);
//////
	//		cout << "FLAG_ROOM_IS_EMPTY : ulPosition" << ulPosition << endl;
			return (ulPosition);
		
		case	FLAG_ROOM_IS_FULL:          //  이미 점유되어 있으면 빈 곳 검색
            int     nLoopCount = 0;
			
			while(1)
			{
			    ulPosition = checkOffsetOfEmptyRoom();  // Empty Room을 찾는다.
			    
			    if (ulPosition >= get_ulMaxDataCount())
			    {
			        save_ulSavedPosition(0);
                    nLoopCount++;
                }
			    else
			        break;
			    
			    if (nLoopCount > CNT_MAX_LOOP_THRESHOLD)
			    {
			        cout << "Overflow allocated Index!!!" << endl;
			        return -1;
			    }
			}
			
			save_ulSavedPosition(ulPosition + 1);
//////
	//		cout << "FLAG_ROOM_IS_FULL : ulPosition" << ulPosition << endl;
			return (ulPosition);
	}
	cout << "Unknown Flag : checkFlagOfSavedPosition() = " << checkFlagOfSavedPosition() << endl;
	return -1;
}
			
long HandleOfAllocIndex::checkOffsetOfEmptyRoom()
{
	const UINT64			ullRoomIsFull 		= 0xFFFFFFFFFFFFFFFF,
							ullRoomIsHalfFull	= 0xFFFF000000000000;
	const UINT64			ullFilterMask		= 0x8000000000000000;
	const unsigned int		unRoomIsHalfFull	= 0xFFFFFFFF;
	const unsigned short	ulRoomShortHalfFull	= 0xFFFF;
	UINT64					ullTemp = 0;
	unsigned long 			n1stForIndex = 0,
							n2ndForIndex = 0;

	n1stForIndex = get_ulSavedPosition() / SIZE_BIT_LONGLONG;
	
	for(n1stForIndex; n1stForIndex < get_FlagIndex(); n1stForIndex++)
	{
		if (ullFlag[n1stForIndex] != ullRoomIsFull)
		{
			if ((ullFlag[n1stForIndex] >> 32) != unRoomIsHalfFull)
			{
				if ((ullFlag[n1stForIndex] >> 48) != ulRoomShortHalfFull)
					n2ndForIndex = 0;
				else
					n2ndForIndex = 16;
			}
			else
			{
				if ((ullFlag[n1stForIndex] << 48) != ullRoomIsHalfFull)
					n2ndForIndex = 32;
				else
					n2ndForIndex = 48;
			}
			
			for(n2ndForIndex; n2ndForIndex < SIZE_BIT_LONGLONG+1; n2ndForIndex++)
			{
				ullTemp = 0;
				ullTemp = ullFlag[n1stForIndex] << n2ndForIndex;
				
				if((ullTemp & ullFilterMask) == FLAG_OFF)
					return ((n1stForIndex * SIZE_BIT_LONGLONG) + n2ndForIndex);     // Return Index
			}
		}
	}
	return -1;	//	Overflow
}

int HandleOfAllocIndex::checkFlagOfSavedPosition()
{
	const UINT64	ullFilterMask	= 0x0000000000000001;
	UINT64			ullTempMask		= 0;
	unsigned int	unRoomSegment 	= 0,
					unRoom			= 0;
	unsigned long	ulPosition 		= get_ulSavedPosition();
	
	unRoomSegment = ulPosition		/  SIZE_BIT_LONGLONG;
	unRoom        = ulPosition		%  SIZE_BIT_LONGLONG;
	ullTempMask	  = ullFilterMask 	<< (SIZE_BIT_LONGLONG-1) - unRoom;

	if ((ullFlag[unRoomSegment] & ullFilterMask) != ullFilterMask)
		return (FLAG_ROOM_IS_EMPTY);
	else
		return (FLAG_ROOM_IS_FULL);
}

void HandleOfAllocIndex::setAllocIndexFlag(unsigned int unFlagIndex, unsigned int unSwitch) const
{
	const UINT64	ullFilterMask	= 0x0000000000000001;
	UINT64			ullTempMask		= 0;
	unsigned int	unRoomSegment 	= 0,
					unRoom			= 0;
					
	unRoomSegment = unFlagIndex 	/  SIZE_BIT_LONGLONG;
	unRoom        = unFlagIndex 	%  SIZE_BIT_LONGLONG;
	ullTempMask	  = ullFilterMask	<< (SIZE_BIT_LONGLONG-1) - unRoom;

///////////////
#ifdef	__DEBUG
    cout << "===========================================================" << endl;
	cout << endl << "246unFlagIndex = " << unFlagIndex << endl;
	displayLongLongValue("247ullFlag[unRoomSegment]", ullFlag[unRoomSegment]);
	displayLongLongValue("248ullTempMask", ullTempMask);
	cout << endl << "unSwitch = " << unSwitch << endl << endl;
#endif
///////////////

	switch(unSwitch)
	{
		case	FLAG_ON	:
			ullFlag[unRoomSegment] |= ullTempMask;
			break;

		case	FLAG_OFF	:
			ullFlag[unRoomSegment] ^= ullTempMask;
			break;
	}
///////////////
#ifdef	__DEBUG
	displayLongLongValue("243ullFlag[unRoomSegment]", ullFlag[unRoomSegment]);
	cout << "===========================================================" << endl;
#endif
///////////////
}	

unsigned long HandleOfAllocIndex::newIndex(unsigned int unRecordSize)
{
	unsigned long	ulAllocatedAddr     = 0;
	unsigned int	unOffsetOfEmptyRoom = 0;
	
	shm_lock();
	
	unOffsetOfEmptyRoom = getOffsetOfEmptyRoom();
	
	if (    ((unsigned long)unOffsetOfEmptyRoom >= 0)  &&  \
            (get_ulDataCount() <= get_ulMaxDataCount()) )
//            ((int)unOffsetOfEmptyRoom <= get_ulMaxDataCount()) )
    {	
    	setAllocIndexFlag(unOffsetOfEmptyRoom, FLAG_ON);
    	ulAllocatedAddr = (unOffsetOfEmptyRoom * unRecordSize) + get_ulBaseAddress();
    	
    	cout << "297unOffsetOfEmptyRoom = " << unOffsetOfEmptyRoom <<\
    	"get_ulBaseAddress()" << get_ulBaseAddress() << "ulBaseAddress = " << ulBaseAddress << endl;
    	cout << "unOffsetOfEmptyRoom = " << unOffsetOfEmptyRoom << "ulAllocatedAddr = " << ulAllocatedAddr << endl;
    	
    	shm_release();
    	set_ulDataCnt(ulDataCount++);
    	cout << "unOffsetOfEmptyRoom = " << unOffsetOfEmptyRoom << "ulAllocatedAddr = " << ulAllocatedAddr << endl;
    	cout << "300unRecordSize = " << unRecordSize << "ulBaseAddress = " << get_ulBaseAddress() << endl;
    	cout << "301unOffsetOfEmptyRoom = " << unOffsetOfEmptyRoom << "ulBaseAddress = " << ulBaseAddress << endl;
    	cout << "302get_ulDataCount() = " << get_ulDataCount() << endl;
    	cout << "303ulAllocatedAddr = " << ulAllocatedAddr << endl;
    	return(ulAllocatedAddr);
    }

    shm_release();	
    
    cout << "get_ulDataCount() = " << get_ulDataCount() << endl;
    cout << "unOffsetOfEmptyRoom = " << unOffsetOfEmptyRoom << endl;
    cout << "get_ulMaxDataCount() = " << get_ulMaxDataCount() << endl;
    
	return 0;	//	더 이상 할당할 수 없는 경우
}

void HandleOfAllocIndex::deleteIndex(unsigned int unAllocatedIndex)
{
	shm_lock();
	
  //  cout << "===========================================================" << endl;
   // cout << "313HandleOfAllocIndex::deleteIndex(unsigned int unAllocatedIndex) = "\
            << unAllocatedIndex << endl;


	setAllocIndexFlag(unAllocatedIndex, FLAG_OFF);
	
	shm_release();
	
    set_ulDataCnt(--ulDataCount);
 //   cout << "316get_ulDataCount() = " << get_ulDataCount() << endl;
 //   cout << "===========================================================" << endl;
}

void HandleOfAllocIndex::displayLongLongValue(const char* strTag, UINT64 ullValue) const
{
	const UINT64	ullFilterMask	= 0x8000000000000000;
	
//	cout << strTag << " = ";
	
	for (unsigned short usIndex = 1; usIndex <= 64; usIndex++)
	{
		cout << (ullValue & ullFilterMask ? '1' : '0');
		ullValue <<= 1;
		
		if ( usIndex % 8 == 0 )
			cout << ' ';
	}
	cout << endl;
}
