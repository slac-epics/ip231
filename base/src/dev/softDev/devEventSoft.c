/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* devEventSoft.c */
/* base/src/dev devEventSoft.c,v 1.10 2003/04/01 21:02:18 mrk Exp */
/*
 *      Author:		Janet Anderson
 *      Date:   	04-21-91
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alarm.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "recGbl.h"
#include "recSup.h"
#include "devSup.h"
#include "eventRecord.h"
#include "epicsExport.h"


/* Create the dset for devEventSoft */
static long init_record();
static long read_event();
struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	read_event;
}devEventSoft={
	5,
	NULL,
	NULL,
	init_record,
	NULL,
	read_event
};
epicsExportAddress(dset,devEventSoft);

static long init_record(eventRecord *pevent)
{

    /* event.inp must be a CONSTANT or a PV_LINK or a DB_LINK or a CA_LINK*/
    switch (pevent->inp.type) {
    case (CONSTANT) :
        if(recGblInitConstantLink(&pevent->inp,DBF_USHORT,&pevent->val))
            pevent->udf = FALSE;
        break;
    case (PV_LINK) :
    case (DB_LINK) :
    case (CA_LINK) :
        break;
    default :
	recGblRecordError(S_db_badField,(void *)pevent,
	    "devEventSoft (init_record) Illegal INP field");
	return(S_db_badField);
    }
    return(0);
}

static long read_event(eventRecord *pevent)
{
    long status;

    status = dbGetLink(&pevent->inp,DBR_USHORT,&pevent->val,0,0);
    if(pevent->inp.type!=CONSTANT && RTN_SUCCESS(status)) pevent->udf=FALSE;
    return(status);
}
