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
 * osdTime.h,v 1.4 2002/09/07 00:09:58 jhill Exp
 *
 * Author: Jeff Hill
 */

#ifndef osdTimeh
#define osdTimeh

/*
 * I assume that this is never defined on VMS ?
 */
struct timespec {
	time_t tv_sec; /* seconds since some epoch */
	long tv_nsec; /* nanoseconds within the second */
};

#endif /* ifndef osdTimeh */