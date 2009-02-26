/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* devMbbiSoftRaw.c */
/* base/src/dev devMbbiSoftRaw.c,v 1.11 2003/04/01 21:02:23 mrk Exp */
/*
 *      Original Author: Bob Dalesio
 *      Current Author:  Marty Kraimer
 *      Date:            6-1-90
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
#include "mbbiRecord.h"
#include "epicsExport.h"

/* Create the dset for devMbbiSoftRaw */
static long init_record();
static long read_mbbi();
struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	read_mbbi;
}devMbbiSoftRaw={
	5,
	NULL,
	NULL,
	init_record,
	NULL,
	read_mbbi
};
epicsExportAddress(dset,devMbbiSoftRaw);

static long init_record(mbbiRecord *pmbbi)
{

    if (pmbbi->inp.type == CONSTANT) {
	recGblInitConstantLink(&pmbbi->inp,DBF_ULONG,&pmbbi->rval);
    }
    /*to preserve old functionality*/
    if(pmbbi->nobt == 0) pmbbi->mask = 0xffffffff;
    pmbbi->mask <<= pmbbi->shft;
    return(0);
}

static long read_mbbi(mbbiRecord *pmbbi)
{
    long status;

    status = dbGetLink(&pmbbi->inp,DBR_LONG,&pmbbi->rval,0,0);
    if(status==0) pmbbi->rval &= pmbbi->mask;
    return(0);
}
