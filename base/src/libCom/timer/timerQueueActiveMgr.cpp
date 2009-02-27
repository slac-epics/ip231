/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/*
 *      timerQueueActiveMgr.cpp,v 1.10 2002/10/23 22:29:32 jhill Exp
 *
 *      Author  Jeffrey O. Hill
 *              johill@lanl.gov
 *              505 665 1831
 */

#include <limits.h>

#define epicsExportSharedSymbols
#include "epicsGuard.h"
#include "timerPrivate.h"

timerQueueActiveMgr::timerQueueActiveMgr ()
{
}

timerQueueActiveMgr::~timerQueueActiveMgr ()
{
    epicsGuard < epicsMutex > locker ( this->mutex );
}
    
epicsTimerQueueActiveForC & timerQueueActiveMgr::allocate (
        bool okToShare, unsigned threadPriority )
{
    epicsGuard < epicsMutex > locker ( this->mutex );
    if ( okToShare ) {
        tsDLIter < epicsTimerQueueActiveForC > iter = this->sharedQueueList.firstIter ();
        while ( iter.valid () ) {
            if ( iter->threadPriority () == threadPriority ) {
                assert ( iter->timerQueueActiveMgrPrivate::referenceCount < UINT_MAX );
                iter->timerQueueActiveMgrPrivate::referenceCount++;
                return *iter;
            }
            iter++;
        }
    }

    epicsTimerQueueActiveForC & queue = 
        * new epicsTimerQueueActiveForC ( okToShare, threadPriority );
    queue.timerQueueActiveMgrPrivate::referenceCount = 1u;
    if ( okToShare ) {
        this->sharedQueueList.add ( queue );
    }
    return queue;
}

void timerQueueActiveMgr::release ( epicsTimerQueueActiveForC &queue )
{
    epicsGuard < epicsMutex > locker ( this->mutex );
    assert ( queue.timerQueueActiveMgrPrivate::referenceCount > 0u );
    queue.timerQueueActiveMgrPrivate::referenceCount--;
    if ( queue.timerQueueActiveMgrPrivate::referenceCount == 0u ) {
        if ( queue.sharingOK () ) {
            this->sharedQueueList.remove ( queue );
        }
        timerQueueActiveMgrPrivate *pPriv = &queue;
        delete pPriv;
    }
}

timerQueueActiveMgrPrivate::timerQueueActiveMgrPrivate () :
    referenceCount ( 0u )
{
}

timerQueueActiveMgrPrivate::~timerQueueActiveMgrPrivate () 
{
}
