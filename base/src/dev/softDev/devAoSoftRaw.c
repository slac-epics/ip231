/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* devAoSoftRaw.c */
/* base/src/dev devAoSoftRaw.c,v 1.12 2003/04/01 21:02:15 mrk Exp */

/* Device Support Routines for soft raw Analog Output Records*/
/*
 *      Author:         Janet Anderson
 *      Date:           09-25-91
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alarm.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "dbEvent.h"
#include "recGbl.h"
#include "recSup.h"
#include "devSup.h"
#include "link.h"
#include "special.h"
#include "aoRecord.h"
#include "epicsExport.h"

/* Create the dset for devAoSoftRaw */
static long write_ao(aoRecord *pao);
struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	write_ao;
	DEVSUPFUN	special_linconv;
}devAoSoftRaw={
	6,
	NULL,
	NULL,
	NULL,
	NULL,
	write_ao,
	NULL
};
epicsExportAddress(dset,devAoSoftRaw);

static long write_ao(aoRecord *pao)
{
    long status;

    status = dbPutLink(&pao->out,DBR_LONG,&pao->rval,1);

    return(status);
}
