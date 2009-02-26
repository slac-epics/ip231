/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* callback.h,v 1.12.2.1 2006/12/06 17:10:26 anj Exp */

/* includes for general purpose callback tasks		*/
/*
 *      Original Author:        Marty Kraimer
 *      Date:   	        07-18-91
*/

#ifndef INCcallbackh
#define INCcallbackh 1

#include "shareLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * WINDOWS also has a "CALLBACK" type def
 */
#ifdef _WIN32
#	ifdef CALLBACK
#		undef CALLBACK
#	endif /*CALLBACK*/
#endif /*_WIN32*/

#define NUM_CALLBACK_PRIORITIES 3
#define priorityLow     0
#define priorityMedium  1
#define priorityHigh    2

typedef struct callbackPvt {
	void (*callback)(struct callbackPvt*);
	int		priority;
	void		*user; /*for use by callback user*/
        void            *timer; /*for use by callback itself*/
}CALLBACK;

typedef void    (*CALLBACKFUNC)(struct callbackPvt*);

#define callbackSetCallback(PFUN,PCALLBACK)\
( (PCALLBACK)->callback = (PFUN) )
#define callbackSetPriority(PRIORITY,PCALLBACK)\
( (PCALLBACK)->priority = (PRIORITY) )
#define callbackSetUser(USER,PCALLBACK)\
( (PCALLBACK)->user = (void *)(USER) )
#define callbackGetUser(USER,PCALLBACK)\
( (USER) = (void *)((CALLBACK *)(PCALLBACK))->user )

epicsShareFunc void epicsShareAPI callbackInit(void);
epicsShareFunc void epicsShareAPI callbackRequest(CALLBACK *pCallback);
epicsShareFunc void epicsShareAPI callbackRequestProcessCallback(
    CALLBACK *pCallback,int Priority, void *pRec);
epicsShareFunc void epicsShareAPI callbackRequestDelayed(
    CALLBACK *pCallback,double seconds);
epicsShareFunc void epicsShareAPI callbackRequestProcessCallbackDelayed(
    CALLBACK *pCallback, int Priority, void *pRec,double seconds);
epicsShareFunc int epicsShareAPI callbackSetQueueSize(int size);

#ifdef __cplusplus
}
#endif

#endif /*INCcallbackh*/
