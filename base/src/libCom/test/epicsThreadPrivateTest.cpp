/*************************************************************************\
* Copyright (c) 2006 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/
/* epicsThreadPrivateTest.cpp,v 1.10.2.4 2006/11/10 21:43:16 anj Exp */

/* Author: Jeff Hill Date:    March 28 2001 */

#include <stdio.h>

#include "epicsTime.h"
#include "epicsThread.h"
#include "epicsUnitTest.h"
#include "testMain.h"

static epicsThreadPrivate < bool > priv;

extern "C" void epicsThreadPrivateTestThread ( void * )
{
    testOk1 ( 0 == priv.get () );
    static bool var;
    priv.set ( &var );
    testOk1 ( &var == priv.get () );
}

MAIN(epicsThreadPrivateTest)
{
    testPlan(5);

    static bool var;
    priv.set ( &var );
    testOk1 ( &var == priv.get() );

    epicsThreadCreate ( "epicsThreadPrivateTest", epicsThreadPriorityMax, 
        epicsThreadGetStackSize ( epicsThreadStackSmall ), 
        epicsThreadPrivateTestThread, 0 );
    epicsThreadSleep ( 1.0 );
    testOk1 ( &var == priv.get() );

    priv.set ( 0 );
    testOk1 ( 0 == priv.get() );

    return testDone();
}

