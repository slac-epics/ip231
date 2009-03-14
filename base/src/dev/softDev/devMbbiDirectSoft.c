/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* devMbbiDirectSoft.c */
/* base/src/dev devMbbiDirectSoft.c,v 1.10 2003/04/01 21:02:21 mrk Exp */
/*
 *      Original Author: Bob Dalesio
 *      Current Author:  Matthew Needes
 *      Date:            10-08-93
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
#include "mbbiDirectRecord.h"
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
}devMbbiDirectSoft={
	5,
	NULL,
	NULL,
	init_record,
	NULL,
	read_mbbi
};
epicsExportAddress(dset,devMbbiDirectSoft);

static long init_record(mbbiDirectRecord *pmbbi)
{

    /* mbbi.inp must be a CONSTANT or a PV_LINK or a DB_LINK or a CA_LINK*/
    switch (pmbbi->inp.type) {
    case (CONSTANT) :
        if(recGblInitConstantLink(&pmbbi->inp,DBF_ENUM,&pmbbi->val))
            pmbbi->udf = FALSE;
        break;
    case (DB_LINK) :
    case (PV_LINK) :
    case (CA_LINK) :
        break;
    default :
	recGblRecordError(S_db_badField,(void *)pmbbi,
		"devMbbiDirectSoft (init_record) Illegal INP field");
	return(S_db_badField);
    }
    return(0);
}

static long read_mbbi(mbbiDirectRecord	*pmbbi)
{
    long status;

    status = dbGetLink(&pmbbi->inp,DBR_USHORT,&pmbbi->val,0,0);
    if (pmbbi->inp.type!=CONSTANT && RTN_SUCCESS(status)) pmbbi->udf=FALSE;
    return(2);
}
