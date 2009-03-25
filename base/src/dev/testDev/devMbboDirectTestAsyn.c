/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* devMbboDirectTestAsyn.c */
/* base/src/dev devMbboDirectTestAsyn.c,v 1.1.2.1 2003/11/06 17:03:07 mrk Exp */

/* devMbboDirectTestAsyn.c - Device Support Routines for testing asynchronous processing*/
/*
 *      Author:  Marty Kraimer
 *      Date:    06NOV2003
 */


#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
#include "mbboRecord.h"
#include "epicsExport.h"

/* Create the dset for devMbboDirectTestAsyn */
static long init_record();
static long write_mbbo();
struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	write_mbbo;
	DEVSUPFUN	special_linconv;
}devMbboDirectTestAsyn={
	6,
	NULL,
	NULL,
	init_record,
	NULL,
	write_mbbo,
	NULL
};
epicsExportAddress(dset,devMbboDirectTestAsyn);

static long init_record(pmbbo)
    struct mbboRecord	*pmbbo;
{
    CALLBACK *pcallback;

    /* mbbo.out must be a CONSTANT*/
    switch (pmbbo->out.type) {
    case (CONSTANT) :
	pcallback = (CALLBACK *)(calloc(1,sizeof(CALLBACK)));
	pmbbo->dpvt = (void *)pcallback;
	break;
    default :
	recGblRecordError(S_db_badField,(void *)pmbbo,
		"devMbboDirectTestAsyn (init_record) Illegal OUT field");
	pmbbo->pact=TRUE;
	return(S_db_badField);
    }
    return(2);
}

static long write_mbbo(pmbbo)
    struct mbboRecord	*pmbbo;
{
    CALLBACK *pcallback=(CALLBACK *)(pmbbo->dpvt);

    /* mbbo.out must be a CONSTANT*/
    switch (pmbbo->out.type) {
    case (CONSTANT) :
	if(pmbbo->pact) {
		printf("Completed asynchronous processing: %s\n",pmbbo->name);
		return(0);
	} else {
                if(pmbbo->disv<=0) return(0);
		printf("Starting asynchronous processing: %s\n",pmbbo->name);
		pmbbo->pact=TRUE;
                callbackRequestProcessCallbackDelayed(
                    pcallback,pmbbo->prio,pmbbo,(double)pmbbo->disv);
		return(0);
	}
    default :
        if(recGblSetSevr(pmbbo,SOFT_ALARM,INVALID_ALARM)){
		if(pmbbo->stat!=SOFT_ALARM) {
			recGblRecordError(S_db_badField,(void *)pmbbo,
			   "devMbboDirectTestAsyn (write_mbbo) Illegal OUT field");
		}
	}
    }
    return(0);
}
