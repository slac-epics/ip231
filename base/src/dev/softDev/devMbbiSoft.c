/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* devMbbiSoft.c */
/* base/src/dev devMbbiSoft.c,v 1.10 2003/04/01 21:02:22 mrk Exp */
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

/* Create the dset for devMbbiSoft */
static long init_record();
static long read_mbbi();
struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	read_mbbi;
}devMbbiSoft={
	5,
	NULL,
	NULL,
	init_record,
	NULL,
	read_mbbi
};
epicsExportAddress(dset,devMbbiSoft);

static long init_record(mbbiRecord *pmbbi)
{

    if (pmbbi->inp.type == CONSTANT) {
        if(recGblInitConstantLink(&pmbbi->inp,DBF_ENUM,&pmbbi->val))
            pmbbi->udf = FALSE;
    }
    return(0);
}

static long read_mbbi(mbbiRecord *pmbbi)
{
    long status;

    status = dbGetLink(&pmbbi->inp,DBR_USHORT,&pmbbi->val,0,0);
    if(pmbbi->inp.type!=CONSTANT && RTN_SUCCESS(status)) pmbbi->udf=FALSE;
    return(2);
}
