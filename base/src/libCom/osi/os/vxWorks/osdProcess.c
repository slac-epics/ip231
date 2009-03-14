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
 * osdProcess.c,v 1.4 2002/07/12 21:35:06 jba Exp
 * 
 * Operating System Dependent Implementation of osiProcess.h
 *
 * Author: Jeff Hill
 *
 */

#include <limits.h>
#include <string.h>

#include <remLib.h>

#define epicsExportSharedSymbols
#include "osiProcess.h"
#include "errlog.h"

epicsShareFunc osiGetUserNameReturn epicsShareAPI osiGetUserName (char *pBuf, unsigned bufSizeIn)
{
	char pName[MAX_IDENTITY_LEN];
    unsigned uiLength;
    size_t len;

	remCurIdGet ( pName, NULL );
    len = strlen ( pName );

    if (len>UINT_MAX || len<=0) {
        return osiGetUserNameFail;
    }
    uiLength = (unsigned) len;

    if ( uiLength + 1 >= bufSizeIn ) {
        return osiGetUserNameFail;
    }

    strncpy ( pBuf, pName, (size_t) bufSizeIn );

    return osiGetUserNameSuccess;
}

epicsShareFunc osiSpawnDetachedProcessReturn epicsShareAPI osiSpawnDetachedProcess
    (const char *pProcessName, const char *pBaseExecutableName)
{
    return osiSpawnDetachedProcessNoSupport;
}
