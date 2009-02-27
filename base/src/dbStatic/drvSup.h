/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* drvSup.h	Driver Support	*/
/* share/epicsH drvSup.h,v 1.3 2003/04/01 21:06:09 mrk Exp */

/*
 *      Author:          Marty Kraimer
 *      Date:            6-1-90
 */

#ifndef INCdrvSuph
#define INCdrvSuph 1

typedef long (*DRVSUPFUN) ();	/* ptr to driver support function*/

typedef struct drvet {	/* driver entry table */
	long		number;		/*number of support routines*/
	DRVSUPFUN	report;		/*print report*/
	DRVSUPFUN	init;		/*init support*/
	/*other functions are device dependent*/
}drvet;
#define DRVETNUMBER ( (sizeof(struct drvet) -sizeof(long))/sizeof(DRVSUPFUN) )

#define S_drv_noDrvSup   (M_drvSup| 1) /*SDR_DRVSUP: Driver support missing*/
#define S_drv_noDrvet    (M_drvSup| 3) /*Missing driver support entry table*/

#endif
