/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* recWaveform.c */
/* base/src/rec  waveformRecord.c,v 1.16 2003/04/01 21:02:04 mrk Exp */

/* recWaveform.c - Record Support Routines for Waveform records */
/*
 *      Original Author: Bob Dalesio
 *      Current Author:  Marty Kraimer
 *      Date:            7-14-89
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
#include "dbScan.h"
#include "devSup.h"
#include "errMdef.h"
#include "recSup.h"
#include "recGbl.h"
#define GEN_SIZE_OFFSET
#include "waveformRecord.h"
#undef  GEN_SIZE_OFFSET
#include "epicsExport.h"
/* Create RSET - Record Support Entry Table*/
#define report NULL
#define initialize NULL
static long init_record();
static long process();
#define special NULL
#define get_value NULL
static long cvt_dbaddr();
static long get_array_info();
static long put_array_info();
static long get_units();
static long get_precision();
#define get_enum_str NULL
#define get_enum_strs NULL
#define put_enum_str NULL
static long get_graphic_double();
static long get_control_double();
#define get_alarm_double NULL
rset waveformRSET={
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
epicsExportAddress(rset,waveformRSET);
struct wfdset { /* waveform dset */
        long            number;
        DEVSUPFUN       dev_report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record; /*returns: (-1,0)=>(failure,success)*/
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read_wf; /*returns: (-1,0)=>(failure,success)*/
};
/*sizes of field types*/
static int sizeofTypes[] = {0,1,1,2,2,4,4,4,8,2};
static void monitor();
static long readValue();

static long init_record(pwf,pass)
    struct waveformRecord	*pwf;
    int pass;
{
    struct wfdset *pdset;
    long status;

    if (pass==0){
	if(pwf->nelm<=0) pwf->nelm=1;
	if(pwf->ftvl == 0) {
		pwf->bptr = (char *)calloc(pwf->nelm,MAX_STRING_SIZE);
	} else {
		if(pwf->ftvl>DBF_ENUM) pwf->ftvl=2;
		pwf->bptr = (char *)calloc(pwf->nelm,sizeofTypes[pwf->ftvl]);
	}
	if(pwf->nelm==1) {
	    pwf->nord = 1;
	} else {
	    pwf->nord = 0;
	}
	return(0);
    }

    /* wf.siml must be a CONSTANT or a PV_LINK or a DB_LINK */
    if (pwf->siml.type == CONSTANT) {
	recGblInitConstantLink(&pwf->siml,DBF_USHORT,&pwf->simm);
    }

    /* must have dset defined */
    if(!(pdset = (struct wfdset *)(pwf->dset))) {
        recGblRecordError(S_dev_noDSET,(void *)pwf,"wf: init_record");
        return(S_dev_noDSET);
    }
    /* must have read_wf function defined */
    if( (pdset->number < 5) || (pdset->read_wf == NULL) ) {
        recGblRecordError(S_dev_missingSup,(void *)pwf,"wf: init_record");
        return(S_dev_missingSup);
    }
    if( pdset->init_record ) {
        if((status=(*pdset->init_record)(pwf))) return(status);
    }
    return(0);
}

static long process(pwf)
	struct waveformRecord	*pwf;
{
        struct wfdset   *pdset = (struct wfdset *)(pwf->dset);
	long		 status;
	unsigned char    pact=pwf->pact;

        if( (pdset==NULL) || (pdset->read_wf==NULL) ) {
                pwf->pact=TRUE;
                recGblRecordError(S_dev_missingSup,(void *)pwf,"read_wf");
                return(S_dev_missingSup);
        }

	if ( pact && pwf->busy ) return(0);

	status=readValue(pwf); /* read the new value */
	/* check if device support set pact */
	if ( !pact && pwf->pact ) return(0);
        pwf->pact = TRUE;

	pwf->udf=FALSE;
	recGblGetTimeStamp(pwf);

	monitor(pwf);
        /* process the forward scan link record */
        recGblFwdLink(pwf);

        pwf->pact=FALSE;
        return(0);
}

static long cvt_dbaddr(paddr)
    struct dbAddr *paddr;
{
    struct waveformRecord *pwf=(struct waveformRecord *)paddr->precord;

    paddr->pfield = (void *)(pwf->bptr);
    paddr->no_elements = pwf->nelm;
    paddr->field_type = pwf->ftvl;
    if(pwf->ftvl==0)  paddr->field_size = MAX_STRING_SIZE;
    else paddr->field_size = sizeofTypes[pwf->ftvl];
    paddr->dbr_field_type = pwf->ftvl;
    return(0);
}

static long get_array_info(paddr,no_elements,offset)
    struct dbAddr *paddr;
    long	  *no_elements;
    long	  *offset;
{
    struct waveformRecord	*pwf=(struct waveformRecord *)paddr->precord;

    *no_elements =  pwf->nord;
    *offset = 0;
    return(0);
}

static long put_array_info(paddr,nNew)
    struct dbAddr *paddr;
    long	  nNew;
{
    struct waveformRecord	*pwf=(struct waveformRecord *)paddr->precord;

    pwf->nord = nNew;
    if(pwf->nord > pwf->nelm) pwf->nord = pwf->nelm;
    return(0);
}

static long get_units(paddr,units)
    struct dbAddr *paddr;
    char	  *units;
{
    struct waveformRecord	*pwf=(struct waveformRecord *)paddr->precord;

    strncpy(units,pwf->egu,DB_UNITS_SIZE);
    return(0);
}

static long get_precision(paddr,precision)
    struct dbAddr *paddr;
    long	  *precision;
{
    struct waveformRecord	*pwf=(struct waveformRecord *)paddr->precord;

    *precision = pwf->prec;
    if(paddr->pfield==(void *)pwf->bptr) return(0);
    recGblGetPrec(paddr,precision);
    return(0);
}

static long get_graphic_double(paddr,pgd)
    struct dbAddr *paddr;
    struct dbr_grDouble *pgd;
{
    struct waveformRecord     *pwf=(struct waveformRecord *)paddr->precord;

    if(paddr->pfield==(void *)pwf->bptr){
        pgd->upper_disp_limit = pwf->hopr;
        pgd->lower_disp_limit = pwf->lopr;
    } else recGblGetGraphicDouble(paddr,pgd);
    return(0);
}
static long get_control_double(paddr,pcd)
    struct dbAddr *paddr;
    struct dbr_ctrlDouble *pcd;
{
    struct waveformRecord     *pwf=(struct waveformRecord *)paddr->precord;

    if(paddr->pfield==(void *)pwf->bptr){
        pcd->upper_ctrl_limit = pwf->hopr;
        pcd->lower_ctrl_limit = pwf->lopr;
    } else recGblGetControlDouble(paddr,pcd);
    return(0);
}

static void monitor(pwf)
    struct waveformRecord	*pwf;
{
	unsigned short	monitor_mask;

        monitor_mask = recGblResetAlarms(pwf);
	monitor_mask |= (DBE_LOG|DBE_VALUE);
        if(monitor_mask) db_post_events(pwf,pwf->bptr,monitor_mask);
	return;

}

static long readValue(pwf)
        struct waveformRecord *pwf;
{
        long            status;
	long		nRequest;
        struct wfdset   *pdset = (struct wfdset *) (pwf->dset);


        if (pwf->pact == TRUE){
                status=(*pdset->read_wf)(pwf);
                return(status);
        }

        status=dbGetLink(&(pwf->siml), DBR_ENUM,&(pwf->simm),0,0);
        if (status)
                return(status);

        if (pwf->simm == NO){
                status=(*pdset->read_wf)(pwf);
                return(status);
        }
        if (pwf->simm == YES){
        	nRequest=pwf->nelm;
                status=dbGetLink(&(pwf->siol),
                                pwf->ftvl,pwf->bptr,0,&nRequest);
                /* nord set only for db links: needed for old db_access */
        	if (pwf->siol.type != CONSTANT ){
                pwf->nord = nRequest;
                if (status==0) pwf->udf=FALSE;
            }
        } else {
                status=-1;
                recGblSetSevr(pwf,SOFT_ALARM,INVALID_ALARM);
                return(status);
        }
        recGblSetSevr(pwf,SIMM_ALARM,pwf->sims);

        return(status);
}

