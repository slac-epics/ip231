
/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* systemCallIntMech.cpp,v 1.2.2.1 2004/05/14 13:36:01 norume Exp */
/*
 *      Author:		Jeff Hill 
 */

#include "osiSock.h"

enum epicsSocketSystemCallInterruptMechanismQueryInfo 
        epicsSocketSystemCallInterruptMechanismQuery ()
{
    return esscimqi_socketBothShutdownRequired;
}
