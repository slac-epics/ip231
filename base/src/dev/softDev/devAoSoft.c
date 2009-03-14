/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* devAoSoft.c */
/* base/src/dev devAoSoft.c,v 1.10 2003/04/01 21:02:15 mrk Exp */

/* Device Support Routines for soft Analog Output Records*/
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
#include "link.h"
#include "special.h"
#include "aoRecord.h"
#include "epicsExport.h"

/* added for Channel Access Links */
static long init_record();

/* Create the dset for devAoSoft */
static long write_ao();
struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	write_ao;
	DEVSUPFUN	special_linconv;
}devAoSoft={
	6,
	NULL,
	NULL,
	init_record,
	NULL,
	write_ao,
	NULL};
epicsExportAddress(dset,devAoSoft);


static long init_record(aoRecord *pao)
{

    long status=0;
    status = 2;
    return status;

} /* end init_record() */

static long write_ao(aoRecord *pao)
{
    long status;

    status = dbPutLink(&pao->out,DBR_DOUBLE, &pao->oval,1);

    return(status);
}
