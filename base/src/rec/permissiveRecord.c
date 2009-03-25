/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* recPermissive.c */
/* base/src/rec  permissiveRecord.c,v 1.11 2003/04/01 21:01:57 mrk Exp */

/* recPermissive.c - Record Support Routines for Permissive records */
/*
 *      Original Author: Bob Dalesio
 *      Current Author:  Marty Kraimer
 *      Date:            10-10-90
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "dbDefs.h"
#include "epicsPrint.h"
#include "dbAccess.h"
#include "dbEvent.h"
#include "dbFldTypes.h"
#include "errMdef.h"
#include "recSup.h"
#include "recGbl.h"
#define GEN_SIZE_OFFSET
#include "permissiveRecord.h"
#undef  GEN_SIZE_OFFSET
#include "epicsExport.h"

/* Create RSET - Record Support Entry Table*/
#define report NULL
#define initialize NULL
#define init_record NULL
static long process();
#define special NULL
#define get_value NULL
#define cvt_dbaddr NULL
#define get_array_info NULL
#define put_array_info NULL
#define get_units NULL
#define get_precision NULL
#define get_enum_str NULL
#define get_enum_strs NULL
#define put_enum_str NULL
#define get_graphic_double NULL
#define get_control_double NULL
#define get_alarm_double NULL

rset permissiveRSET={
	RSETNUMBER,
	report,
	initialize,
	init_record,
	process,
	special,
	get_value,
	cvt_dbaddr,
	get_array_info,
	put_array_info,
	get_units,
	get_precision,
	get_enum_str,
	get_enum_strs,
	put_enum_str,
	get_graphic_double,
	get_control_double,
	get_alarm_double
};
epicsExportAddress(rset,permissiveRSET);

static void monitor();

static long process(ppermissive)
    struct permissiveRecord     *ppermissive;
{

    ppermissive->pact=TRUE;
    ppermissive->udf=FALSE;
    recGblGetTimeStamp(ppermissive);
    monitor(ppermissive);
    recGblFwdLink(ppermissive);
    ppermissive->pact=FALSE;
    return(0);
}

static void monitor(ppermissive)
    struct permissiveRecord             *ppermissive;
{
    unsigned short  monitor_mask;
    unsigned short  val,oval,wflg,oflg;

    monitor_mask = recGblResetAlarms(ppermissive);
    /* get val,oval,wflg,oflg*/
    val=ppermissive->val;
    oval=ppermissive->oval;
    wflg=ppermissive->wflg;
    oflg=ppermissive->oflg;
    /*set  oval and oflg*/
    ppermissive->oval = val;
    ppermissive->oflg = wflg;
    if(oval != val) {
	db_post_events(ppermissive,&ppermissive->val,
	    monitor_mask|DBE_VALUE|DBE_LOG);
    }
    if(oflg != wflg) {
        db_post_events(ppermissive,&ppermissive->wflg,
	    monitor_mask|DBE_VALUE|DBE_LOG);
    }
    return;
}
