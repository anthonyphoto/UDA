/******************************************************************
* 파 일 명     : HandleOfAllocIndex_new.cpp     					  
* 기   능                                                          
* : 생성된 shared memory에 적당한 크기만큼 메모리를 할당해주는 class.   
*   bit 연산을 이용하여 할당과 해제를 알려준다.					      
* 작 성 자     : 권 종 욱               		      				  
* first data  :  				  	     						  
* last updata : 2000. 12. 26  by 한 근 희   	 					  
* 관 련 화 일  : HandleOfAllocIndex_new.hpp   
* Version	  : 1.1( 2001. 05. 21 )   					  
* program history                        						  
*  2000.12.26 -> 프로그램 주석추가        	 				 		 
******************************************************************/
#include <HandleOfAllocIndex_new.hpp>

//===============================================================
//Constructor								   					=
//  : allocation을 위한 shared memory를 생성한다.			  		=
// argument     : 프로세스 형상구분 6가지.	 						=
//				  unMaxData -> Max Data Count					=
// return value : None											=
//																=
//===============================================================
HandleOfAllocIndex_New::HandleOfAllocIndex_New(	int				nLSystemId,			\
												int				nLServiceId,		\
												int				nLGroupId,			\
												int				nLProcessType,		\
												int				nLSerialNo,			\
												int				nLRemark,			\
												unsigned int	unMaxData	)		\
	: SharedMemory	(	nLSystemId,					\
						nLServiceId,				\
						nLGroupId,					\
						nLProcessType,				\
						nLSerialNo,					\
						nLRemark,					\
						((sizeof(unsigned long)*4)+(getAllocSize(unMaxData)*8))						\
					)



{
	// savedposition을 찾아서 setting해주기 위한 임시 변수.
	int		nTempPosition;  // add Han Guen Hee 2001. 1. 2
	//cout << "i'm HandleOfAllocIndex Constructor!!!" << endl;
	//	Address 할당 시작
	unBaseAddress 	= (unsigned int*)get_ShmPtr();
	unSavedPosition	= sizeof(unsigned int) + unBaseAddress;
	unMaxDataCount	= sizeof(unsigned int) + unSavedPosition;
	unDataCnt		= sizeof(unsigned int) + unMaxDataCount;
	ullFlag			= (UINT64*)(sizeof(unsigned int) + unDataCnt);
	//	Address 할당 끝

	//cout << "unBaseAddress " << unBaseAddress << endl;

	if (mem_lock() < 0)
		cout << "LOCK FAIL in HandleOfAllocIndex's Constructor" << endl;

	// savedposition을 찾아서 setting
	// add Han Guen Hee 2001. 1. 2
	nTempPosition=checkOffsetOfEmptyRoom();
	if( nTempPosition == -1 )
		cout << "Overflow!!" << endl;
	else
		set_unSavedPosition(nTempPosition);
	
	set_BaseAddress(0);
	set_unMaxDataCount(unMaxData);

	unDataCount = get_unDataCnt();

	if (mem_unlock() < 0)
		cout << "UNLOCK FAIL in HandleOfAllocIndex's Constructor" << endl;
}
//===============================================================
//Constructor								   					=
//  : allocation을 위한 shared memory를 생성한다.			  		=
// argument     : 프로세스 형상구분 6가지.	 						=
//				  unMaxData -> Max Data Count					=
//				  unStartAddress -> 메모리 시작주소 				=
// return value : None											=
//																=
//===============================================================
HandleOfAllocIndex_New::HandleOfAllocIndex_New(	int				nLSystemId,			\
												int				nLServiceId,		\
												int				nLGroupId,			\
												int				nLProcessType,		\
												int				nLSerialNo,			\
												int				nLRemark,			\
												unsigned int	unMaxData,			\
												unsigned int	unStartAddress	)	\
	: SharedMemory	(	nLSystemId,													\
						nLServiceId,												\
						nLGroupId,													\
						nLProcessType,												\
						nLSerialNo,													\
						nLRemark,													\
						((sizeof(unsigned long)*4)+(getAllocSize(unMaxData)*8))		\
					)



{
	// savedposition을 찾아서 setting해주기 위한 임시 변수.
	int		nTempPosition;  // add Han Guen Hee 2001. 1. 2
	//cout << "i'm HandleOfAllocIndex Constructor!!!" << endl;
	//	Address 할당 시작
	unBaseAddress 	= (unsigned int*)get_ShmPtr();
	unSavedPosition	= sizeof(unsigned int) + unBaseAddress;
	unMaxDataCount	= sizeof(unsigned int) + unSavedPosition;
	unDataCnt		= sizeof(unsigned int) + unMaxDataCount;
	ullFlag			= (UINT64*)(sizeof(unsigned int) + unDataCnt);
	//	Address 할당 끝

	//cout << "unBaseAddress " << unBaseAddress << endl;

	if (mem_lock() < 0)
		cout << "LOCK FAIL in HandleOfAllocIndex's Constructor" << endl;

	// savedposition을 찾아서 setting
	// add Han Guen Hee 2001. 1. 2
	nTempPosition=checkOffsetOfEmptyRoom();
	if( nTempPosition == -1 )
		cout << "Overflow!!" << endl;
	else
		set_unSavedPosition(nTempPosition);
		
	set_BaseAddress(unStartAddress);
	set_unMaxDataCount(unMaxData);

	unDataCount = get_unDataCnt();

	if (mem_unlock() < 0)
		cout << "UNLOCK FAIL in HandleOfAllocIndex's Constructor" << endl;
}
//===============================================================
// save_unSavedPosition Method				   					=
//  : 마지막으로 할당된 위치를 저장한다.						  		=
// argument     : unPosition -> 마지막으로 할당된 위치				=
// return value : None											=
//																=
//===============================================================
void HandleOfAllocIndex_New::save_unSavedPosition(int unPosition)
{
	if (unPosition >= get_unMaxDataCount())
    {
//		set_unSavedPosition(1);
		set_unSavedPosition(0);
	//	cout << "118 get_unSavedPosition() = " << get_unSavedPosition() << endl;

	}
	else
	{
		set_unSavedPosition(unPosition);
	 //   cout << "124get_unSavedPosition() = " << get_unSavedPosition() << endl;
	}
}
//===============================================================
// getOffsetOfEmptyRoom Method				   					=
//  : 비어있는 방의 offset을 반환해준다.					  		=
// argument     : None											=
// return value : 비어있는 방의 offset -> 정상						=
//				  -1 -> error ( overflow )						=
//===============================================================
int HandleOfAllocIndex_New::getOffsetOfEmptyRoom()
{
	unsigned int	unPosition;

	switch (checkFlagOfSavedPosition())     //  현재 Index의 상태를 반환
	{
		case	FLAG_ROOM_IS_EMPTY:         //  비어 있으면
			unPosition = get_unSavedPosition();
			if( 0 )
			{
				cout << "getOffsetOfEmptyRoom : unPosition=> " << unPosition << endl;
			}
			save_unSavedPosition(unPosition + 1);
//////
	//		cout << "FLAG_ROOM_IS_EMPTY : unPosition" << unPosition << endl;
			return (unPosition);

		case	FLAG_ROOM_IS_FULL:          //  이미 점유되어 있으면 빈 곳 검색
            int     nLoopCount = 0;

			while(1)
			{
			    unPosition = checkOffsetOfEmptyRoom();  // Empty Room을 찾는다.

			    if (unPosition >= get_unMaxDataCount())
			    {
			        save_unSavedPosition(0);
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

			save_unSavedPosition(unPosition + 1);
//////
	//		cout << "FLAG_ROOM_IS_FULL : unPosition" << unPosition << endl;
			return (unPosition);
	}
	//cout << "Unknown Flag : checkFlagOfSavedPosition() = " << checkFlagOfSavedPosition() << endl;
	return -1;
}
//===============================================================
// checkOffsetOfEmptyRoom Method			   					=
//  : 현재 Index를 검색하는 Method							  		=
// argument     : None											=
// return value : 비어있는 방의 index -> 정상						=
//				  -1 -> error ( overflow )						=
//===============================================================
int HandleOfAllocIndex_New::checkOffsetOfEmptyRoom()
{
	const UINT64			ullRoomIsFull 		= 0xFFFFFFFFFFFFFFFF,
							ullRoomIsHalfFull	= 0xFFFF000000000000;
	const UINT64			ullFilterMask		= 0x8000000000000000;
	const unsigned long		ulRoomIsHalfFull	= 0xFFFFFFFF;
	const unsigned short	ulRoomShortHalfFull	= 0xFFFF;
	UINT64					ullTemp = 0;
	unsigned int 			n1stForIndex = 0,
							n2ndForIndex = 0;

	n1stForIndex = get_unSavedPosition() / SIZE_BIT_LONGLONG;

	if( 0 )
	{
		cout << "checkOffsetOfEmptyRoom : n1stForIndex=>" << n1stForIndex << endl;
		cout << "checkOffsetOfEmptyRoom : get_FlagIndex=>" << get_FlagIndex() << endl;
	}
	for(n1stForIndex; n1stForIndex < get_FlagIndex(); n1stForIndex++)
	{
		if (ullFlag[n1stForIndex] != ullRoomIsFull)
		{
			if ((ullFlag[n1stForIndex] >> 32) != ulRoomIsHalfFull)
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
				{
					//cout << "checkOffsetOfEmptyRoom : return value=>" << (n1stForIndex * SIZE_BIT_LONGLONG) + n2ndForIndex << endl;
					return ((n1stForIndex * SIZE_BIT_LONGLONG) + n2ndForIndex);     // Return Index
				}
			}
		}
	}
	return -1;	//	Overflow
}
//===============================================================
// checkFlagOfSavedPosition Method			   					=
//  : savedposition의 상태를 check						  		=
// argument     : None											=
// return value : FLAG_ROOM_IS_EMPTY -> 비어있음.					=
//				  FLAG_ROOM_IS_FULL -> 이미 사용중임				=
//===============================================================
int HandleOfAllocIndex_New::checkFlagOfSavedPosition()
{
	const UINT64	ullFilterMask	= 0x0000000000000001;
	UINT64			ullTempMask		= 0;
	unsigned int	unRoomSegment 	= 0,
					unRoom			= 0;
	unsigned int	unPosition 		= get_unSavedPosition();

	unRoomSegment = unPosition		/  SIZE_BIT_LONGLONG;
	unRoom        = unPosition		%  SIZE_BIT_LONGLONG;
	ullTempMask	  = ullFilterMask 	<< (SIZE_BIT_LONGLONG-1) - unRoom;

	if( 0 )
	{
		cout << "checkFlagOfSavedPosition  unPosition=> " << unPosition << endl;
		cout << "checkFlagOfSavedPosition  ullFilterMask=> " << (unsigned int)ullFilterMask << endl;
		cout << "checkFlagOfSavedPosition  ullTempMask=> " << (unsigned int)ullTempMask << endl;
		cout << "checkFlagOfSavedPosition  unRoomSegment=> " << (unsigned int)unRoomSegment << endl;
		cout << "checkFlagOfSavedPosition  unRoom=> " << (unsigned int)unRoom << endl;
		displayLongLongValue("ullFlag[unRoomSegment]", ullFlag[unRoomSegment]);
		displayLongLongValue("ullFilterMask", ullFilterMask);
		displayLongLongValue("(ullFlag[unRoomSegment] & ullFilterMask)", (ullFlag[unRoomSegment] & ullFilterMask));
	}
	// 2001. 05. 17 --> ullFilterMask ==> ullTempMask 로 고침.
	if ((ullFlag[unRoomSegment] & ullTempMask) != ullTempMask)
	{
		cout << "checkFlagOfSavedPosition : return empty!!!" << endl;
		return (FLAG_ROOM_IS_EMPTY);
	}
	else
	{
		cout << "checkFlagOfSavedPosition : return full!!!" << endl;
		return (FLAG_ROOM_IS_FULL);
	}
}
//===============================================================
// setAllocIndexFlag Method					   					=
//  : index를 alloction상태로 setting해준다.				  		=
// argument     : unFlagIndex -> index							=
//				  unSwitch	-> 할당인지 해제인지를 나타내는 flag		=
// return value : None.											=
//===============================================================
void HandleOfAllocIndex_New::setAllocIndexFlag(unsigned int unFlagIndex, unsigned int unSwitch) const
{
	const UINT64	ullFilterMask	= 0x0000000000000001;
	UINT64			ullTempMask		= 0;
	unsigned int	unRoomSegment 	= 0,
					unRoom			= 0;

	unRoomSegment = unFlagIndex 	/  SIZE_BIT_LONGLONG;
	unRoom        = unFlagIndex 	%  SIZE_BIT_LONGLONG;
	ullTempMask	  = ullFilterMask	<< (SIZE_BIT_LONGLONG-1) - unRoom;

	if( 0 )
	{
		cout << " setAllocIndexFlag : unRoomSegment=>" << unRoomSegment << endl;
		cout << " setAllocIndexFlag : unRoom=>" <<unRoom << endl;
	}
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
//===============================================================
// newIndex Method							   					=
//  : 새로운 메모리를 할당.								  		=
// argument     : unRecordSize -> 레코드 size					=
// return value : 할당된 메모리 주소.								=
// 				  0-> 더이상 할당할수 없을경우.                     =
//===============================================================
unsigned int HandleOfAllocIndex_New::newIndex(unsigned int unRecordSize)
{
	unsigned int	unOffsetOfEmptyRoom = 0,
					unAllocatedAddr     = 0,
					unTempDataCount		= 0;

	mem_lock();

	if( 0 )
	{
		cout << "newIndex : unRecordSize =>" << (unsigned int)unRecordSize << endl;
	}
	if( 0 )
	{
		cout << "newIndex : unOffsetOfEmptyRoom =>" << (unsigned int)unOffsetOfEmptyRoom << endl;
		cout << "newIndex : get_unDataCount() =>" << (unsigned int)get_unDataCount() << endl;
		cout << "newIndex : get_unMaxDataCount() =>" << (unsigned int)get_unMaxDataCount() << endl;
	}
	// 2001. 05. 17
	unTempDataCount = get_unDataCount();
	if (get_unMaxDataCount() > unTempDataCount)	// Check Max Data Count
	{
		unOffsetOfEmptyRoom = getOffsetOfEmptyRoom();	
		
		if ( (int)unOffsetOfEmptyRoom >= 0)			// 	Check Empty Room is Full
	    {
	    	setAllocIndexFlag(unOffsetOfEmptyRoom, FLAG_ON);
	    	unAllocatedAddr = (unOffsetOfEmptyRoom * unRecordSize) + get_unBaseAddress();
	    	mem_unlock();
	
	    	set_unDataCnt(unTempDataCount++);
	    	if( 1 )
	    	{
	    //		cout << "newIndex : unDataCount =>" << (unsigned int)unDataCount << endl;
				cout << "newIndex : unAllocatedAddr =>[" << (unsigned int)unAllocatedAddr << "]" << endl;
			}
	    	return(unAllocatedAddr);
	    }
	}
    mem_unlock();

	return 0;	//	더 이상 할당할 수 없는 경우
}
//===============================================================
// deleteIndex Method					   						=
//  : 할당된 memory를 해제시켜 준다.						  		=
// argument     : unAllocatedIndex -> index						=
// return value : None.											=
//===============================================================
void HandleOfAllocIndex_New::deleteIndex(unsigned int unAllocatedIndex)
{
	mem_lock();

	unsigned int unTempDataCount = get_unDataCount();

	setAllocIndexFlag(unAllocatedIndex, FLAG_OFF);

	mem_unlock();

    if (unTempDataCount != 0)
    	set_unDataCnt(--unTempDataCount);
 //   cout << "316get_unDataCount() = " << get_unDataCount() << endl;
 //   cout << "===========================================================" << endl;
}
//===============================================================
// displayLongLongValue Method					   				=
//  : 메모리 할당에 사용된 변수의 현재값을 출력해준다.			  		=
// argument     : strTag -> 문자열								=
//				  ullValue	-> 할당하는데 사용된 8byte변수값     	=
// return value : None.											=
//===============================================================
void HandleOfAllocIndex_New::displayLongLongValue(const char* strTag, UINT64 ullValue) const
{
	const UINT64	ullFilterMask	= 0x8000000000000000;

 	cout << strTag << " = ";

	for (unsigned short usIndex = 1; usIndex <= 64; usIndex++)
	{
		cout << (ullValue & ullFilterMask ? '1' : '0');
		ullValue <<= 1;

		if ( usIndex % 8 == 0 )
			cout << ' ';
	}
	cout << endl;
}
