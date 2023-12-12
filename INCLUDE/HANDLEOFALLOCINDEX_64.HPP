#ifndef	HANDLEOFALLOCINDEX_64_HPP
#define	HANDLEOFALLOCINDEX_64_HPP

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

inline	unsigned int getAllocSize (int nMaxData )
{
	return ((nMaxData % SIZE_BIT_LONGLONG) == 0) ? nMaxData/SIZE_BIT_LONGLONG : (nMaxData/SIZE_BIT_LONGLONG)+1;
}

//	Define	associated with shared memory
static const	char*				PATH_NAME_SHM		= "../include/HandleOfAllocIndex.hpp";


class	HandleOfAllocIndex	: public ShmMemory
{
	private:
		unsigned long*	ulBaseAddress;
		unsigned long*	ulSavedPosition;
		unsigned long*	ulMaxDataCount;
		unsigned long*  ulDataCnt;
		UINT64*			ullFlag;
		unsigned long	ulDataCount;
	public:
		// Constructor
//		HandleOfAllocIndex():ShmMemory(const char* strPathName, int nId){}
		HandleOfAllocIndex(const unsigned char, const unsigned int);
		HandleOfAllocIndex(const unsigned char,const unsigned int, unsigned long);
				
		// Accessor
		void			set_BaseAddress(unsigned long ulAddress)	{ *ulBaseAddress 	= ulAddress;	}
		void			set_ulSavedPosition(unsigned long ulSavedAddr){ *ulSavedPosition = ulSavedAddr;	}
		void			set_ulMaxDataCount(unsigned long ulMaxCnt)	{ *ulMaxDataCount 	= ulMaxCnt;		}
		void			set_ulDataCnt(unsigned long ulValue)		{ *ulDataCnt		= ulValue;		}
		
		unsigned long	get_ulSavedPosition() const					{ return *ulSavedPosition;			}
		unsigned long	get_ulBaseAddress() const					{ return *ulBaseAddress;			}
		unsigned long	get_ulMaxDataCount() const					{ return *ulMaxDataCount;			}
		unsigned long	get_FlagIndex() const 						{ return getAllocSize(*ulMaxDataCount);	}	
		unsigned long	get_ulDataCount() const 					{ return ulDataCount;				}	
		unsigned long	get_ulDataCnt() const 						{ return *ulDataCnt;				}	
		
		int				getOffsetOfEmptyRoom();
		long			checkOffsetOfEmptyRoom();
		void			setAllocIndexFlag(unsigned int, unsigned int) const;
		void			save_ulSavedPosition(unsigned long unPosition);
		int				checkFlagOfSavedPosition();
				
		// 
		unsigned long	newIndex(unsigned int);
		void			deleteIndex(unsigned int);
		void			displayLongLongValue(const char* , UINT64) const;
};

class	HandleOfAllocIndex_New	: public SharedMemory
{
	private:
		unsigned long*	ulBaseAddress;
		unsigned long*	ulSavedPosition;
		unsigned long*	ulMaxDataCount;
		unsigned long*  ulDataCnt;
		UINT64*			ullFlag;
		unsigned long	ulDataCount;
	public:
		// Constructor
		HandleOfAllocIndex_New(int, int, int, int, int, int, int);
				
		// Accessor
		void			set_BaseAddress(unsigned long ulAddress)	{ *ulBaseAddress 	= ulAddress;	}
		void			set_ulSavedPosition(unsigned long ulSavedAddr){ *ulSavedPosition = ulSavedAddr;	}
		void			set_ulMaxDataCount(unsigned long ulMaxCnt)	{ *ulMaxDataCount 	= ulMaxCnt;		}
		void			set_ulDataCnt(unsigned long ulValue)		{ *ulDataCnt		= ulValue;		}
		
		unsigned long	get_ulSavedPosition() const					{ return *ulSavedPosition;			}
		unsigned long	get_ulBaseAddress() const					{ return *ulBaseAddress;			}
		unsigned long	get_ulMaxDataCount() const					{ return *ulMaxDataCount;			}
		unsigned long	get_FlagIndex() const 						{ return getAllocSize(*ulMaxDataCount);	}	
		unsigned long	get_ulDataCount() const 					{ return ulDataCount;				}	
		unsigned long	get_ulDataCnt() const 						{ return *ulDataCnt;				}	
		
		int				getOffsetOfEmptyRoom();
		long			checkOffsetOfEmptyRoom();
		void			setAllocIndexFlag(unsigned int, unsigned int) const;
		void			save_ulSavedPosition(unsigned long unPosition);
		int				checkFlagOfSavedPosition();
				
		// 
		unsigned long	newIndex(unsigned int);
		void			deleteIndex(unsigned int);
		void			displayLongLongValue(const char* , UINT64) const;
};


#endif
