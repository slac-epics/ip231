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
 * rsrv.h,v 1.4.2.2 2005/09/23 15:12:25 jhill Exp
 *
 *  Author: Jeffrey O. Hill
 *      hill@luke.lanl.gov
 *      (505) 665 1831
 *  Date:   5-88
 */

#ifndef rsrvh
#define rsrvh

#include "shareLib.h"

epicsShareFunc int epicsShareAPI rsrv_init (void);
epicsShareFunc void epicsShareAPI casr (unsigned level);
epicsShareFunc void epicsShareAPI casHostNameInitiatingCurrentThread ( 
                        char * pBuf, unsigned bufSize );
epicsShareFunc void epicsShareAPI casUserNameInitiatingCurrentThread ( 
                        char * pBuf, unsigned bufSize );
void casStatsFetch ( unsigned *pChanCount, unsigned *pConnCount );

#define RSRV_OK 0
#define RSRV_ERROR (-1)

#endif /*rsrvh */
