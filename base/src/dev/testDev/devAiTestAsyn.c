/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* devAiTestAsyn.c */
/* base/src/dev devAiTestAsyn.c,v 1.10.2.5 2003/12/16 20:09:28 mrk Exp */

/* devAiTestAsyn.c - Device Support Routines for testing asynchronous processing*/
/*
 *      Original Author: Bob Dalesio
 *      Current Author:  Marty Kraimer
 *      Date:            6-1-90
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alarm.h"
#include "callback.h"
#include "cvtTable.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "recGbl.h"
#include "recSup.h"
#include "devSup.h"
#include "link.h"
#include "dbCommon.h"
#include "aiRecord.h"
#include "epicsExport.h"

static long init_record(struct aiRecord *pai)
{
    CALLBACK *pcallback;
    switch (pai->inp.type) {
    case (CONSTANT) :
        pcallback = (CALLBACK *)(calloc(1,sizeof(CALLBACK)));
        pai->dpvt = (void *)pcallback;
        break;
    default :
        recGblRecordError(S_db_badField,(void *)pai,
            "devAiTestAsyn (init_record) Illegal INP field");
        pai->pact=TRUE;
        return(S_db_badField);
    }
    return(0);
}

static long read_ai(struct aiRecord *pai)
{
    CALLBACK *pcallback = (CALLBACK *)pai->dpvt;
    if(pai->pact) {
        pai->val += 0.1; /* Change VAL just to show we've done something. */
        pai->udf = FALSE; /* We modify VAL so we are responsible for UDF too. */
        printf("Completed asynchronous processing: %s\n",pai->name);
        return(2); /* don`t convert*/
    } 
    if(pai->disv<=0) return(2);
    printf("Starting asynchronous processing: %s\n",pai->name);
    pai->pact=TRUE;
    callbackRequestProcessCallbackDelayed(pcallback,
        pai->prio,pai,(double)pai->disv);
    return(0);
}

/* Create the dset for devAiTestAsyn */
struct {
    long        number;
    DEVSUPFUN   report;
    DEVSUPFUN   init;
    DEVSUPFUN   init_record;
    DEVSUPFUN   get_ioint_info;
    DEVSUPFUN   read_ai;
    DEVSUPFUN   special_linconv;
}devAiTestAsyn={
    6,
    NULL,
    NULL,
    init_record,
    NULL,
    read_ai,
    NULL
};
epicsExportAddress(dset,devAiTestAsyn);
