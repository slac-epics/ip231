/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* cantProceed.c,v 1.10.2.6 2006/03/30 19:42:36 anj Exp */

/* Author:  Marty Kraimer Date:    04JAN99 */

#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>

#define epicsExportSharedSymbols
#include "errlog.h"
#include "cantProceed.h"
#include "epicsThread.h"

epicsShareFunc void * callocMustSucceed(size_t count, size_t size, const char *msg)
{
    void * mem = NULL;
    if (count == 0 || size == 0)
        cantProceed("%s: callocMustSucceed(%lu, %lu) - bad args\n",
                msg, (unsigned long)count, (unsigned long)size);
    
    while ((mem = calloc(count, size)) == NULL) {
        errlogPrintf("%s: callocMustSucceed(%lu, %lu) - calloc failed\n",
                msg, (unsigned long)count, (unsigned long)size);
        errlogPrintf("Thread %s (%p) suspending.\n",
                epicsThreadGetNameSelf(), epicsThreadGetIdSelf());
        errlogFlush();
        epicsThreadSuspendSelf();
    }
    return mem;
}

epicsShareFunc void * mallocMustSucceed(size_t size, const char *msg)
{
    void * mem = NULL;
    if (size == 0)
        cantProceed("%s: mallocMustSucceed(%lu) - bad arg\n",
                msg, (unsigned long)size);
    
    while ((mem = malloc(size)) == NULL) {
        errlogPrintf("%s: mallocMustSucceed(%lu) - malloc failed\n",
            msg, (unsigned long)size);
        errlogPrintf("Thread %s (%p) suspending.\n",
                epicsThreadGetNameSelf(), epicsThreadGetIdSelf());
        errlogFlush();
        epicsThreadSuspendSelf();
    }
    return mem;
}

epicsShareFunc void cantProceed(const char *msg, ...)
{
    va_list pvar;
    va_start(pvar, msg);
    if (msg)
        errlogVprintf(msg, pvar);
    va_end(pvar);
    
    errlogPrintf("Thread %s (%p) can't proceed, suspending.\n",
            epicsThreadGetNameSelf(), epicsThreadGetIdSelf());
    errlogFlush();
    
    epicsThreadSleep(1.0);
    while (1)
        epicsThreadSuspendSelf();
}
