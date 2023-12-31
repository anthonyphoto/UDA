/******************************************************************
* 파 일 명     : HandleOfAllocIndex.hpp     					  
* 기   능                                                          
* : 생성된 shared memory에 적당한 크기만큼 메모리를 할당해주는 class.    
*   bit 연산을 이용하여 할당과 해제를 알려준다.					      
* 작 성 자     : 권 종 욱               		      				  
* first data  :  				  	     						  
* last updata : 2000. 12. 26  by 한 근 희   	 					  
* 관 련 화 일  : HandleOfAllocIndex.cpp   
* Version	  : 1.1( 2001. 05. 21 )   					  
* program history                        						  
*  	. 프로그램 주석추가 -> 2000.12.26
*   . 신세기 UDA 적용 & 정리. -> 2001. 04. 23  
******************************************************************/
#ifndef	HANDLEOFALLOCINDEX_HPP
#define	HANDLEOFALLOCINDEX_HPP

#include <iostream.h>
#include <SharedMemLegacy.hpp>

#ifndef	UINT64
	typedef	unsigned long long	UINT64;
#endif

const	int					FLAG_ON				    = 1;
const	int					FLAG_OFF			    = 0;
const	int					FLAG_ROOM_IS_EMPTY	    = 0;
const	int					FLAG_ROOM_IS_FULL	    = 1;
const	int					SIZE_BIT_LONGLONG	    = (8 * sizeof(UINT64));
const   unsigned int        CNT_MAX_LOOP_THRESHOLD  = 3;
#define			ALLOC_DEBUG			0

//===============================================================
// getAllocSize Method					   						
//  : 최대 data갯수로 메모리의 size를 계산해준다.					
// argument     : nMaxData -> Max Data Count					
// return value : Memory size									
//																
//===============================================================
inline	unsigned int getAllocSize (int nMaxData )
{
	return ((nMaxData % SIZE_BIT_LONGLONG) == 0) ? nMaxData/SIZE_BIT_LONGLONG : (nMaxData/SIZE_BIT_LONGLONG)+1;
}

class	HandleOfAllocIndex	: public ShmMemory
{
	private:
		unsigned int*	unBaseAddress;
		unsigned int*	unSavedPosition;
		unsigned int*	unMaxDataCount;
		unsigned int*   unDataCnt;
		UINT64*			ullFlag;
		unsigned int	unDataCount;
	public:
		// Constructor
	//	HandleOfAllocIndex(int, int, int, int, int, int, unsigned int);
	//	HandleOfAllocIndex(int, int, int, int, int, int, unsigned int, unsigned int);
		HandleOfAllocIndex(const char*,const unsigned char, const unsigned int);
		HandleOfAllocIndex(const char*,const unsigned char,const unsigned int, unsigned int);

		// Accessor
		void			set_BaseAddress(unsigned int unAddress)		{ *unBaseAddress 	= unAddress;	}
		void			set_unSavedPosition(unsigned int unSavedAddr){ *unSavedPosition = unSavedAddr;	}
		void			set_unMaxDataCount(unsigned int unMaxCnt)	{ *unMaxDataCount 	= unMaxCnt;		}
		void			set_unDataCnt(unsigned int unValue)			{ *unDataCnt		= unValue;		}
		
		unsigned int	get_Shm_Key()  								{ return (unsigned int ) ShmPtr(); }
		unsigned int	get_unSavedPosition() const					{ return *unSavedPosition;			}
		unsigned int	get_unBaseAddress() const					{ return *unBaseAddress;			}
		unsigned int	get_unMaxDataCount() const					{ return *unMaxDataCount;			}
		unsigned int	get_FlagIndex() const 						{ return getAllocSize(*unMaxDataCount);	}
		unsigned int	get_unDataCount() const 					{ return unDataCount;				}
		unsigned int	get_unDataCnt() const 						{ return *unDataCnt;				}

		int				getOffsetOfEmptyRoom();
		int				checkOffsetOfEmptyRoom();
		void			setAllocIndexFlag(unsigned int, unsigned int) const;
		void			save_unSavedPosition(int unPosition);
		int				checkFlagOfSavedPosition();

		//
		unsigned int	newIndex(unsigned int);
		void			deleteIndex(unsigned int);
		void			displayLongLongValue(const char* , UINT64) const;
};

#endif
