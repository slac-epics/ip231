/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* devEventTestIoEvent.c */
/* base/src/dev devEventTestIoEvent.c,v 1.10.2.1 2003/12/16 20:09:28 mrk Exp */
/*
 *      Author:  	Marty Kraimer
 *      Date:           01/09/92
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dbDefs.h"
#include "dbAccess.h"
#include "recGbl.h"
#include "callback.h"
#include "dbScan.h"
#include "recSup.h"
#include "devSup.h"
#include "eventRecord.h"
#include "epicsExport.h"

/* Create the dset for devEventTestIoEvent */
static long init_record();
static long get_ioint_info();
static long read_event();
struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	read_event;
}devEventTestIoEvent={
	5,
	NULL,
	NULL,
	init_record,
	get_ioint_info,
	read_event
};
epicsExportAddress(dset,devEventTestIoEvent);

typedef struct myCallback {
    CALLBACK callback;
    IOSCANPVT ioscanpvt;
}myCallback;

static long init_record(pevent,pass)
    eventRecord *pevent;
    int pass;
{
    myCallback *pcallback;

    pcallback = (myCallback *)(calloc(1,sizeof(myCallback)));
    scanIoInit(&pcallback->ioscanpvt);
    pevent->dpvt = (void *)pcallback;
    return(0);
}

static long get_ioint_info(
	int 			cmd,
	struct eventRecord 	*pevent,
	IOSCANPVT		*ppvt)
{
    myCallback *pcallback = (myCallback *)pevent->dpvt;

    *ppvt = pcallback->ioscanpvt;
    return(0);
}
    
static long read_event(pevent)
    struct eventRecord	*pevent;
{
    myCallback *pcallback= (myCallback *)pevent->dpvt;

    if(pevent->proc<=0) return(0);
    pevent->udf = FALSE;
    printf("%s Requesting Next ioEnevt\n",pevent->name);
    callbackRequestProcessCallbackDelayed(&pcallback->callback,
        pevent->prio,pevent,(double)pevent->proc);
    return(0);
}
