/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* epicsAssert.h,v 1.10.2.1 2004/01/30 01:24:07 jhill Exp
 *      
 *	EPICS assert  
 *
 *      Author:         Jeffrey O. Hill 
 *      Date:           022795 
 */

#ifndef assertEPICS 
#define assertEPICS 

#include "shareLib.h"

#ifdef __cplusplus
extern "C" {
#endif

#undef assert

#ifndef epicsAssertAuthor
#   define epicsAssertAuthor 0
#endif

#ifdef NDEBUG
#	define assert(ignore)  ((void) 0)
#else /* NDEBUG */

#if defined(__STDC__) || defined(__cplusplus)

epicsShareFunc extern void epicsShareAPI 
	epicsAssert (const char *pFile, const unsigned line, 
			const char *pExp, const char *pAuthorName);

#else /*__STDC__ or __cplusplus*/

epicsShareFunc extern void epicsShareAPI epicsAssert ();

#endif /*__STDC__ or __cplusplus*/

#if (defined(__STDC__) || defined(__cplusplus)) && !defined(VAXC)

#define assert(exp) \
( (exp) ? ( void ) 0 : epicsAssert( __FILE__, __LINE__, #exp, epicsAssertAuthor ) )

#else /*__STDC__ or __cplusplus*/

#define assert(exp) \
( (exp) ? ( void ) 0 : epicsAssert( __FILE__, __LINE__, "", epicsAssertAuthor ) )

#endif /* (__STDC__ or __cplusplus) and not VAXC */

#endif  /* NDEBUG */

#ifdef __cplusplus
}
#endif

#endif /* assertEPICS */

