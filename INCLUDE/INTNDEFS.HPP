#ifndef __INTERNAL_DEFS_H__
#define __INTERNAL_DEFS_H__

/* original */
/*#ifndef PERMS
#define PERMS					0660
#endif*/
#ifndef DUMMY
#define DUMMY					0
#endif
#ifndef TRUE
#define TRUE					1
#endif
#ifndef FALSE
#define FALSE					0
#endif
#ifndef _BOOLEAN_
#define _BOOLEAN_
typedef int BOOL;
#endif

#define ASSERT(ex)              assert(ex)
#define VARIFY(ex)              assert(ex)
#define CLOCKS_PER_MILLISEC		(CLOCKS_PER_SEC / 100)
#define SETTIMEOUT(x)           (clock() + (clock_t) x * CLOCKS_PER_MILLISEC);
/* original */


#endif /* __INTERNAL_DEFS_H__ */




