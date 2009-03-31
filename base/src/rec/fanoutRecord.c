/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* fanoutRecord.c */
/* base/src/rec  fanoutRecord.c,v 1.15 2003/04/01 21:01:50 mrk Exp */
/*
 *      Original Author: Bob Dalesio
 *      Current Author:  Marty Kraimer
 *      Date:            12-20-88
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "dbDefs.h"
#include "epicsPrint.h"
#include "alarm.h"
#include "dbAccess.h"
#include "dbEvent.h"
#include "dbFldTypes.h"
#include "errMdef.h"
#include "recSup.h"
#include "recGbl.h"
#include "dbCommon.h"
#define GEN_SIZE_OFFSET
#include "fanoutRecord.h"
#undef  GEN_SIZE_OFFSET
#include "epicsExport.h"

/* Create RSET - Record Support Entry Table*/
#define report NULL
#define initialize NULL
static long init_record();
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

rset fanoutRSET={
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
epicsExportAddress(rset,fanoutRSET);

static long init_record(pfanout,pass)
    struct fanoutRecord	*pfanout;
    int pass;
{

    if (pass==0) return(0);
    recGblInitConstantLink(&pfanout->sell,DBF_USHORT,&pfanout->seln);
    return(0);
}
	
static long process(pfanout)
    struct fanoutRecord     *pfanout;
{
    struct link    *plink;
    unsigned short state;
    short          i;
    unsigned short monitor_mask;

    pfanout->pact = TRUE;

    /* fetch link selection  */
    dbGetLink(&(pfanout->sell),DBR_USHORT,&(pfanout->seln),0,0);
    switch (pfanout->selm){
    case (fanoutSELM_All):
        plink=&(pfanout->lnk1);
        state=pfanout->seln;
        for ( i=0; i<6; i++, state>>=1, plink++) {
            if(plink->type!=CONSTANT) dbScanFwdLink(plink);
        }
        break;
    case (fanoutSELM_Specified):
        if(pfanout->seln>6) {
		recGblSetSevr(pfanout,SOFT_ALARM,INVALID_ALARM);
            break;
        }
        if(pfanout->seln==0) {
            break;
        }
        plink=&(pfanout->lnk1);
        plink += (pfanout->seln-1); dbScanFwdLink(plink);
        break;
    case (fanoutSELM_Mask):
        if(pfanout->seln==0) {
            break;
        }
        if(pfanout->seln>63 ) {
            recGblSetSevr(pfanout,SOFT_ALARM,INVALID_ALARM);
            break;
        }
        plink=&(pfanout->lnk1);
        state=pfanout->seln;
        for ( i=0; i<6; i++, state>>=1, plink++) {
            if(state & 1 && plink->type!=CONSTANT) dbScanFwdLink(plink);
        }
        break;
    default:
        recGblSetSevr(pfanout,SOFT_ALARM,INVALID_ALARM);
    }
    pfanout->udf=FALSE;
    recGblGetTimeStamp(pfanout);
    /* check monitors*/
    /* get previous stat and sevr  and new stat and sevr*/
    monitor_mask = recGblResetAlarms(pfanout);
    /* process the forward scan link record */
    recGblFwdLink(pfanout);
    pfanout->pact=FALSE;
    return(0);
}
