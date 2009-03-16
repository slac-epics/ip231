/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* base/src/rec  aoRecord.c,v 1.24.2.1 2004/10/11 22:32:05 anj Exp */
  
/* aoRecord.c - Record Support Routines for Analog Output records */
/*
 *      Original Author: Bob Dalesio
 *      Current Author:  Marty Kraimer
 *      Date:            7-14-89
 *
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "dbDefs.h"
#include "epicsPrint.h"
#include "epicsMath.h"
#include "alarm.h"
#include "cvtTable.h"
#include "dbAccess.h"
#include "dbEvent.h"
#include "dbFldTypes.h"
#include "devSup.h"
#include "errMdef.h"
#include "special.h"
#include "recSup.h"
#include "recGbl.h"
#include "menuConvert.h"
#include "menuOmsl.h"
#define GEN_SIZE_OFFSET
#include "aoRecord.h"
#undef  GEN_SIZE_OFFSET
#include "menuIvoa.h"
#include "epicsExport.h"

/* Create RSET - Record Support Entry Table*/
#define report NULL
#define initialize NULL
static long init_record(struct aoRecord *pao, int pass);
static long process();
static long special();
#define get_value NULL
#define cvt_dbaddr NULL
#define get_array_info NULL
#define put_array_info NULL
static long get_units();
static long get_precision();
#define get_enum_str NULL
#define get_enum_strs NULL
#define put_enum_str NULL
static long get_graphic_double();
static long get_control_double();
static long get_alarm_double();

rset aoRSET={
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
	get_alarm_double };

struct aodset { /* analog input dset */
	long		number;
	DEVSUPFUN	dev_report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record; /*returns: (0,2)=>(success,success no convert)*/
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	write_ao;/*(0)=>(success ) */
	DEVSUPFUN	special_linconv;
};
epicsExportAddress(rset,aoRSET);



static void checkAlarms();
static long fetch_value();
static void convert();
static void monitor();
static long writeValue();

static long init_record(struct aoRecord *pao, int pass)
{
    struct aodset *pdset;
    double 	eoff = pao->eoff, eslo = pao->eslo;
    double	value;

    if (pass==0) return(0);

    /* ao.siml must be a CONSTANT or a PV_LINK or a DB_LINK */
    if (pao->siml.type == CONSTANT) {
	recGblInitConstantLink(&pao->siml,DBF_USHORT,&pao->simm);
    }

    if(!(pdset = (struct aodset *)(pao->dset))) {
	recGblRecordError(S_dev_noDSET,(void *)pao,"ao: init_record");
	return(S_dev_noDSET);
    }
    /* get the initial value if dol is a constant*/
    if (pao->dol.type == CONSTANT) {
	if(recGblInitConstantLink(&pao->dol,DBF_DOUBLE,&pao->val))
	    pao->udf = isnan(pao->val);
    }

    /* must have write_ao function defined */
    if ((pdset->number < 6) || (pdset->write_ao ==NULL)) {
	recGblRecordError(S_dev_missingSup,(void *)pao,"ao: init_record");
	return(S_dev_missingSup);
    }
    pao->init = TRUE;
    /*The following is for old device support that doesnt know about eoff*/
    if ((pao->eslo==1.0) && (pao->eoff==0.0)) {
	pao->eoff = pao->egul;
    }

    if (pdset->init_record) {
	long status=(*pdset->init_record)(pao);
	if (pao->linr == menuConvertSLOPE) {
	    pao->eoff = eoff;
	    pao->eslo = eslo;
	}
        switch(status){
        case(0): /* convert */
	    value = (double)pao->rval + (double)pao->roff;
	    if(pao->aslo!=0.0) value *= pao->aslo;
	    value += pao->aoff;
            if (pao->linr == menuConvertNO_CONVERSION){
		; /*do nothing*/
            } else if ((pao->linr == menuConvertLINEAR) ||
		      (pao->linr == menuConvertSLOPE)) {
                value = value*pao->eslo + pao->eoff;
            }else{
                if(cvtRawToEngBpt(&value,pao->linr,pao->init,
			(void *)&pao->pbrk,&pao->lbrk)!=0) break;
            }
	    pao->val = value;
	    pao->udf = isnan(value);
        break;
        case(2): /* no convert */
        break;
        default:
	     recGblRecordError(S_dev_badInitRet,(void *)pao,"ao: init_record");
	     return(S_dev_badInitRet);
        }
    }
    pao->oval = pao->pval = pao->val;
    return(0);
}

static long process(pao)
	struct aoRecord     *pao;
{
	struct aodset	*pdset = (struct aodset *)(pao->dset);
	long		 status=0;
	unsigned char    pact=pao->pact;
	double		value;

	if ((pdset==NULL) || (pdset->write_ao==NULL)) {
		pao->pact=TRUE;
		recGblRecordError(S_dev_missingSup,(void *)pao,"write_ao");
		return(S_dev_missingSup);
	}

	/* fetch value and convert*/
	if (pao->pact == FALSE) {
                if ((pao->dol.type != CONSTANT)
                && (pao->omsl == menuOmslclosed_loop)) {
                    status = fetch_value(pao, &value);
                }
                else {
                    value = pao->val;
                }
		if(!status) convert(pao, value);
		pao->udf = isnan(pao->val);
	}

	/* check for alarms */
	checkAlarms(pao);

	if (pao->nsev < INVALID_ALARM )
		status=writeValue(pao); /* write the new value */
	else {
    		switch (pao->ivoa) {
		    case (menuIvoaContinue_normally) :
			status=writeValue(pao); /* write the new value */
		        break;
		    case (menuIvoaDon_t_drive_outputs) :
			break;
		    case (menuIvoaSet_output_to_IVOV) :
	                if(pao->pact == FALSE){
			 	pao->val=pao->ivov;
			 	value=pao->ivov;
				convert(pao,value);
                        }
			status=writeValue(pao); /* write the new value */
		        break;
		    default :
			status=-1;
		        recGblRecordError(S_db_badField,(void *)pao,
		                "ao:process Illegal IVOA field");
		}
	}

	/* check if device support set pact */
	if ( !pact && pao->pact ) return(0);
	pao->pact = TRUE;

	recGblGetTimeStamp(pao);

	/* check event list */
	monitor(pao);

	/* process the forward scan link record */
        recGblFwdLink(pao);

	pao->init=FALSE;
	pao->pact=FALSE;
	return(status);
}

static long special(paddr,after)
    struct dbAddr *paddr;
    int           after;
{
    struct aoRecord     *pao = (struct aoRecord *)(paddr->precord);
    struct aodset       *pdset = (struct aodset *) (pao->dset);
    int                 special_type = paddr->special;

    switch(special_type) {
    case(SPC_LINCONV):
        if(pdset->number<6 ) {
            recGblDbaddrError(S_db_noMod,paddr,"ao: special");
            return(S_db_noMod);
        }
	pao->init=TRUE;
        if ((pao->linr == menuConvertLINEAR) && pdset->special_linconv) {
	    double eoff = pao->eoff;
	    double eslo = pao->eslo;
	    long status;
	    pao->eoff = pao->egul;
	    status = (*pdset->special_linconv)(pao,after);
	    if (eoff != pao->eoff)
		db_post_events(pao, &pao->eoff, DBE_VALUE|DBE_LOG);
            if (eslo != pao->eslo)
		db_post_events(pao, &pao->eslo, DBE_VALUE|DBE_LOG);
	    return (status);
	}
	return (0);
    default:
        recGblDbaddrError(S_db_badChoice,paddr,"ao: special");
        return(S_db_badChoice);
    }
}

static long get_units(paddr,units)
    struct dbAddr *paddr;
    char	  *units;
{
    struct aoRecord	*pao=(struct aoRecord *)paddr->precord;

    strncpy(units,pao->egu,DB_UNITS_SIZE);
    return(0);
}

static long get_precision(paddr,precision)
    struct dbAddr *paddr;
    long	  *precision;
{
    struct aoRecord	*pao=(struct aoRecord *)paddr->precord;

    *precision = pao->prec;
    if(paddr->pfield == (void *)&pao->val
    || paddr->pfield == (void *)&pao->oval
    || paddr->pfield == (void *)&pao->pval) return(0);
    recGblGetPrec(paddr,precision);
    return(0);
}

static long get_graphic_double(paddr,pgd)
    struct dbAddr *paddr;
    struct dbr_grDouble	*pgd;
{
    struct aoRecord	*pao=(struct aoRecord *)paddr->precord;

    if(paddr->pfield==(void *)&pao->val
    || paddr->pfield==(void *)&pao->hihi
    || paddr->pfield==(void *)&pao->high
    || paddr->pfield==(void *)&pao->low
    || paddr->pfield==(void *)&pao->lolo
    || paddr->pfield==(void *)&pao->oval
    || paddr->pfield==(void *)&pao->pval){
        pgd->upper_disp_limit = pao->hopr;
        pgd->lower_disp_limit = pao->lopr;
    } else recGblGetGraphicDouble(paddr,pgd);
    return(0);
}

static long get_control_double(paddr,pcd)
    struct dbAddr *paddr;
    struct dbr_ctrlDouble *pcd;
{
    struct aoRecord	*pao=(struct aoRecord *)paddr->precord;

    if(paddr->pfield==(void *)&pao->val
    || paddr->pfield==(void *)&pao->hihi
    || paddr->pfield==(void *)&pao->high
    || paddr->pfield==(void *)&pao->low
    || paddr->pfield==(void *)&pao->lolo
    || paddr->pfield==(void *)&pao->oval
    || paddr->pfield==(void *)&pao->pval){
        pcd->upper_ctrl_limit = pao->drvh;
        pcd->lower_ctrl_limit = pao->drvl;
    } else recGblGetControlDouble(paddr,pcd);
    return(0);
}
static long get_alarm_double(paddr,pad)
    struct dbAddr *paddr;
    struct dbr_alDouble	*pad;
{
    struct aoRecord	*pao=(struct aoRecord *)paddr->precord;

    if(paddr->pfield==(void *)&pao->val){
         pad->upper_alarm_limit = pao->hihi;
         pad->upper_warning_limit = pao->high;
         pad->lower_warning_limit = pao->low;
         pad->lower_alarm_limit = pao->lolo;
    } else recGblGetAlarmDouble(paddr,pad);
    return(0);
}

static void checkAlarms(pao)
    struct aoRecord	*pao;
{
	double		val;
	double		hyst, lalm, hihi, high, low, lolo;
	unsigned short	hhsv, llsv, hsv, lsv;

	if (pao->udf) {
 		recGblSetSevr(pao,UDF_ALARM,INVALID_ALARM);
		return;
	}
	hihi = pao->hihi; lolo = pao->lolo; high = pao->high; low = pao->low;
	hhsv = pao->hhsv; llsv = pao->llsv; hsv = pao->hsv; lsv = pao->lsv;
	val = pao->val; hyst = pao->hyst; lalm = pao->lalm;

	/* alarm condition hihi */
	if (hhsv && (val >= hihi || ((lalm==hihi) && (val >= hihi-hyst)))){
	        if (recGblSetSevr(pao,HIHI_ALARM,pao->hhsv)) pao->lalm = hihi;
		return;
	}

	/* alarm condition lolo */
	if (llsv && (val <= lolo || ((lalm==lolo) && (val <= lolo+hyst)))){
	        if (recGblSetSevr(pao,LOLO_ALARM,pao->llsv)) pao->lalm = lolo;
		return;
	}

	/* alarm condition high */
	if (hsv && (val >= high || ((lalm==high) && (val >= high-hyst)))){
	        if (recGblSetSevr(pao,HIGH_ALARM,pao->hsv)) pao->lalm = high;
		return;
	}

	/* alarm condition low */
	if (lsv && (val <= low || ((lalm==low) && (val <= low+hyst)))){
	        if (recGblSetSevr(pao,LOW_ALARM,pao->lsv)) pao->lalm = low;
		return;
	}

	/* we get here only if val is out of alarm by at least hyst */
	pao->lalm = val;
	return;
}

static long fetch_value(pao,pvalue)
    struct aoRecord  *pao;
    double *pvalue;
{
	short		save_pact;
	long		status;

	save_pact = pao->pact;
	pao->pact = TRUE;

	/* don't allow dbputs to val field */
	pao->val=pao->pval;

        status = dbGetLink(&pao->dol,DBR_DOUBLE,pvalue,0,0);
        pao->pact = save_pact;

	if (status) {
           recGblSetSevr(pao,LINK_ALARM,INVALID_ALARM);
           return(status);
	}

        if (pao->oif == aoOIF_Incremental)
           *pvalue += pao->val;

	return(0);
}

static void convert(pao,value)
    struct aoRecord  *pao;
    double value;
{
        /* check drive limits */
	if(pao->drvh > pao->drvl) {
        	if (value > pao->drvh) value = pao->drvh;
        	else if (value < pao->drvl) value = pao->drvl;
	}
	pao->val = value;
	pao->pval = value;

	/* now set value equal to desired output value */
        /* apply the output rate of change */
        if ( (pao->oroc) != 0.0 ){/*must be defined and >0*/
		double		diff;

                diff = value - pao->oval;
                if (diff < 0){
                        if (pao->oroc < -diff) value = pao->oval - pao->oroc;
                }else if (pao->oroc < diff) value = pao->oval + pao->oroc;
        }
	pao->omod = (pao->oval!=value);
	pao->oval = value;

        /* convert */
        switch (pao->linr) {
        case menuConvertNO_CONVERSION:
            break; /* do nothing*/
        case menuConvertLINEAR:
        case menuConvertSLOPE:
            if (pao->eslo == 0.0) value = 0;
            else value = (value - pao->eoff) / pao->eslo;
            break;
        default:
            if(cvtEngToRawBpt(&value,pao->linr,pao->init,(void *)&pao->pbrk,&pao->lbrk)!=0){
                recGblSetSevr(pao,SOFT_ALARM,MAJOR_ALARM);
                return;
            }
        }
	value -= pao->aoff;
	if(pao->aslo!=0.0) value /= pao->aslo;
	if (value >= 0.0) pao->rval = value + 0.5 - pao->roff;
	else pao->rval = value - 0.5 - pao->roff;
}


static void monitor(pao)
    struct aoRecord	*pao;
{
	unsigned short	monitor_mask;
	double		delta;

        monitor_mask = recGblResetAlarms(pao);
        /* check for value change */
        delta = pao->mlst - pao->val;
        if(delta<0.0) delta = -delta;
        if (delta > pao->mdel) {
                /* post events for value change */
                monitor_mask |= DBE_VALUE;
                /* update last value monitored */
                pao->mlst = pao->val;
        }
        /* check for archive change */
        delta = pao->alst - pao->val;
        if(delta<0.0) delta = -delta;
        if (delta > pao->adel) {
                /* post events on value field for archive change */
                monitor_mask |= DBE_LOG;
                /* update last archive value monitored */
                pao->alst = pao->val;
        }


        /* send out monitors connected to the value field */
        if (monitor_mask){
                db_post_events(pao,&pao->val,monitor_mask);
	}
	if(pao->omod) monitor_mask |= (DBE_VALUE|DBE_LOG);
	if(monitor_mask) {
		pao->omod = FALSE;
		db_post_events(pao,&pao->oval,monitor_mask);
		if(pao->oraw != pao->rval) {
                	db_post_events(pao,&pao->rval,
			    monitor_mask|DBE_VALUE|DBE_LOG);
			pao->oraw = pao->rval;
		}
		if(pao->orbv != pao->rbv) {
                	db_post_events(pao,&pao->rbv,
			    monitor_mask|DBE_VALUE|DBE_LOG);
			pao->orbv = pao->rbv;
		}
	}
	return;
}

static long writeValue(pao)
	struct aoRecord	*pao;
{
	long		status;
        struct aodset 	*pdset = (struct aodset *) (pao->dset);

	if (pao->pact == TRUE){
		status=(*pdset->write_ao)(pao);
		return(status);
	}

        status = dbGetLink(&pao->siml,DBR_USHORT,&(pao->simm),0,0);
	if (status)
		return(status);

	if (pao->simm == NO){
		status=(*pdset->write_ao)(pao);
		return(status);
	}
	if (pao->simm == YES){
		status = dbPutLink(&(pao->siol),DBR_DOUBLE,&(pao->oval),1);
	} else {
		status=-1;
		recGblSetSevr(pao,SOFT_ALARM,INVALID_ALARM);
		return(status);
	}
        recGblSetSevr(pao,SIMM_ALARM,pao->sims);

	return(status);
}
