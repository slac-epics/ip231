/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* dbAccess.c */
/* base/src/db  dbAccess.c,v 1.116.2.11 2006/12/19 16:20:47 anj Exp */
/*
 *      Original Author: Bob Dalesio
 *      Current Author:  Marty Kraimer
 *      Date:            11-7-90
*/


#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "dbDefs.h"
#include "epicsThread.h"
#include "errlog.h"
#include "cantProceed.h"
#include "cvtFast.h"
#include "epicsTime.h"
#include "alarm.h"
#include "ellLib.h"
#include "dbStaticLib.h"
#include "dbBase.h"
#include "link.h"
#include "dbFldTypes.h"
#include "recSup.h"
#include "devSup.h"
#include "caeventmask.h"
#include "db_field_log.h"
#include "dbCommon.h"
#include "dbFldTypes.h"
#include "special.h"
#include "errMdef.h"
#define epicsExportSharedSymbols
#include "dbAddr.h"
#include "callback.h"
#include "dbScan.h"
#include "dbLock.h"
#include "dbEvent.h"
#include "dbConvert.h"
#include "dbConvertFast.h"
#include "epicsEvent.h"
#include "dbCa.h"
#include "dbBkpt.h"
#include "dbNotify.h"
#include "dbAccessDefs.h"
#include "recGbl.h"

epicsShareDef struct dbBase *pdbbase = 0;
epicsShareDef volatile int interruptAccept=FALSE;

static short mapDBFToDBR[DBF_NTYPES] = {
	DBR_STRING, DBR_CHAR, DBR_UCHAR, DBR_SHORT, DBR_USHORT, 
	DBR_LONG, DBR_ULONG, DBR_FLOAT, DBR_DOUBLE,
	DBR_ENUM, DBR_ENUM, DBR_ENUM,
	DBR_STRING, DBR_STRING, DBR_STRING, DBR_NOACCESS
};

/*
 *  The number of consecutive attempts that can be made to process an
 *    active record before a SCAN_ALARM is raised.  Active records
 *    (records with the pact flag set) cannot be processed until
 *    that flag becomes unset.
 */
#define MAX_LOCK 10

/* The following is to handle SPC_AS */
static SPC_ASCALLBACK spcAsCallback = 0;

void epicsShareAPI dbSpcAsRegisterCallback(SPC_ASCALLBACK func)
{
    spcAsCallback = func;
}

static long putSpecial(DBADDR *paddr,int pass)
{
    long int	(*pspecial)()=NULL;
    struct rset	*prset;
    dbCommon 	*precord=(dbCommon *)(paddr->precord);
    long	status=0;
    long	special=paddr->special;
	
    prset = dbGetRset(paddr);
    if(special<100) { /*global processing*/
	if((special==SPC_NOMOD) && (pass==0)) {
	    status = S_db_noMod;
	    recGblDbaddrError(status,paddr,"dbPut");
	    return(status);
	}else if(special==SPC_SCAN){
	    if(pass==0)
		scanDelete(precord);
	    else
		scanAdd(precord);
	}else if((special==SPC_AS) && (pass==1)) {
            if(spcAsCallback) (*spcAsCallback)(precord);
	}
    }else {
	if( prset && (pspecial = (prset->special))) {
	    status=(*pspecial)(paddr,pass);
	    if(status) return(status);
	} else if(pass==0){
	    recGblRecSupError(S_db_noSupport,paddr,"dbPut", "special");
	    return(S_db_noSupport);
	}
    }
    return(0);
}

static void get_enum_strs(DBADDR *paddr, char **ppbuffer,
	struct rset *prset,long	*options)
{
	short		field_type=paddr->field_type;
	dbFldDes	*pdbFldDes = (dbFldDes *)paddr->pfldDes;
	dbMenu		*pdbMenu;
	dbDeviceMenu	*pdbDeviceMenu;
	char		**papChoice;
	unsigned long	no_str;
	char		*ptemp;
	struct dbr_enumStrs *pdbr_enumStrs=(struct dbr_enumStrs*)(*ppbuffer);
	int			i;

	memset(pdbr_enumStrs,'\0',dbr_enumStrs_size);
	switch(field_type) {
		case DBF_ENUM:
		    if( prset && prset->get_enum_strs ) {
			(*prset->get_enum_strs)(paddr,pdbr_enumStrs);
		    } else {
			*options = (*options)^DBR_ENUM_STRS;/*Turn off option*/
		    }
		    break;
		case DBF_MENU:
		    pdbMenu = (dbMenu *)pdbFldDes->ftPvt;
		    no_str = pdbMenu->nChoice;
		    papChoice= pdbMenu->papChoiceValue;
		    goto choice_common;
		case DBF_DEVICE:
		    pdbDeviceMenu = (dbDeviceMenu *)pdbFldDes->ftPvt;
                    if(!pdbDeviceMenu) {
                        *options = (*options)^DBR_ENUM_STRS;/*Turn off option*/
                        break;
                    }
		    no_str = pdbDeviceMenu->nChoice;
		    papChoice = pdbDeviceMenu->papChoice;
		    goto choice_common;
choice_common:
		    i = sizeof(pdbr_enumStrs->strs)/
			sizeof(pdbr_enumStrs->strs[0]);
		    if(i<no_str) no_str = i;
		    pdbr_enumStrs->no_str = no_str;
		    ptemp = &(pdbr_enumStrs->strs[0][0]);
		    for (i=0; i<no_str; i++) {
			if(papChoice[i]==NULL) *ptemp=0;
			else {
			    strncpy(ptemp,papChoice[i],
				sizeof(pdbr_enumStrs->strs[0]));
			    *(ptemp+sizeof(pdbr_enumStrs->strs[0])-1) = 0;
			}
			ptemp += sizeof(pdbr_enumStrs->strs[0]);
		    }
		    break;
		default:
		    *options = (*options)^DBR_ENUM_STRS;/*Turn off option*/
		    break;
	}
	*ppbuffer = ((char *)*ppbuffer) + dbr_enumStrs_size;
	return;
}

static void get_graphics(DBADDR *paddr, char **ppbuffer,
	struct rset *prset,long	*options)
{
	struct			dbr_grDouble grd;
	int			got_data=FALSE;

        grd.upper_disp_limit = grd.lower_disp_limit = 0.0;
	if( prset && prset->get_graphic_double ) {
		(*prset->get_graphic_double)(paddr,&grd);
		got_data=TRUE;
	}
	if( (*options) & (DBR_GR_LONG) ) {
		char	*pbuffer=*ppbuffer;

		if(got_data) {
		    struct dbr_grLong *pgr=(struct dbr_grLong*)pbuffer;
		    pgr->upper_disp_limit = grd.upper_disp_limit;
		    pgr->lower_disp_limit = grd.lower_disp_limit;
		} else {
		    memset(pbuffer,'\0',dbr_grLong_size);
		    *options = (*options) ^ DBR_GR_LONG; /*Turn off option*/
		}
		*ppbuffer = ((char *)*ppbuffer) + dbr_grLong_size;
	}
	if( (*options) & (DBR_GR_DOUBLE) ) {
		char	*pbuffer=*ppbuffer;

		if(got_data) {
		    struct dbr_grDouble *pgr=(struct dbr_grDouble*)pbuffer;
		    pgr->upper_disp_limit = grd.upper_disp_limit;
		    pgr->lower_disp_limit = grd.lower_disp_limit;
		} else {
		    memset(pbuffer,'\0',dbr_grDouble_size);
		    *options = (*options) ^ DBR_GR_DOUBLE; /*Turn off option*/
		}
		*ppbuffer = ((char *)*ppbuffer) + dbr_grDouble_size;
	}
	return;
}

static void get_control(DBADDR *paddr, char **ppbuffer,
	struct rset *prset,long	*options)
{
	struct dbr_ctrlDouble	ctrld;
	int			got_data=FALSE;

        ctrld.upper_ctrl_limit = ctrld.lower_ctrl_limit = 0.0;
	if( prset && prset->get_control_double ) {
		(*prset->get_control_double)(paddr,&ctrld);
		got_data=TRUE;
	}
	if( (*options) & (DBR_CTRL_LONG) ) {
		char	*pbuffer=*ppbuffer;

		if(got_data) {
		    struct dbr_ctrlLong *pctrl=(struct dbr_ctrlLong*)pbuffer;
		    pctrl->upper_ctrl_limit = ctrld.upper_ctrl_limit;
		    pctrl->lower_ctrl_limit = ctrld.lower_ctrl_limit;
		} else {
		    memset(pbuffer,'\0',dbr_ctrlLong_size);
		    *options = (*options) ^ DBR_CTRL_LONG; /*Turn off option*/
		}
		*ppbuffer = ((char *)*ppbuffer) + dbr_ctrlLong_size;
	}
	if( (*options) & (DBR_CTRL_DOUBLE) ) {
		char	*pbuffer=*ppbuffer;

		if(got_data) {
		   struct dbr_ctrlDouble *pctrl=(struct dbr_ctrlDouble*)pbuffer;
		   pctrl->upper_ctrl_limit = ctrld.upper_ctrl_limit;
		   pctrl->lower_ctrl_limit = ctrld.lower_ctrl_limit;
		} else {
		    memset(pbuffer,'\0',dbr_ctrlDouble_size);
		    *options = (*options) ^ DBR_CTRL_DOUBLE; /*Turn off option*/
		}
		*ppbuffer = ((char *)*ppbuffer) + dbr_ctrlDouble_size;
	}
	return;
}

static void get_alarm(DBADDR *paddr, char **ppbuffer,
	struct rset *prset,long	*options)
{
	struct			dbr_alDouble ald;
	int			got_data=FALSE;

        ald.upper_alarm_limit = ald.upper_warning_limit = 0.0;
        ald.lower_warning_limit = ald.lower_alarm_limit = 0.0;
	if( prset && prset->get_alarm_double ) {
		(*prset->get_alarm_double)(paddr,&ald);
		got_data=TRUE;
	}
	if( (*options) & (DBR_AL_LONG) ) {
		char	*pbuffer=*ppbuffer;

		if(got_data) {
		    struct dbr_alLong *pal=(struct dbr_alLong*)pbuffer;
		    pal->upper_alarm_limit = ald.upper_alarm_limit;
		    pal->upper_warning_limit = ald.upper_warning_limit;
		    pal->lower_warning_limit = ald.lower_warning_limit;
		    pal->lower_alarm_limit = ald.lower_alarm_limit;
		} else {
		    memset(pbuffer,'\0',dbr_alLong_size);
		    *options = (*options) ^ DBR_AL_LONG; /*Turn off option*/
		}
		*ppbuffer = ((char *)*ppbuffer) + dbr_alLong_size;
	}
	if( (*options) & (DBR_AL_DOUBLE) ) {
		char	*pbuffer=*ppbuffer;

		if(got_data) {
		    struct dbr_alDouble *pal=(struct dbr_alDouble*)pbuffer;
		    pal->upper_alarm_limit = ald.upper_alarm_limit;
		    pal->upper_warning_limit = ald.upper_warning_limit;
		    pal->lower_warning_limit = ald.lower_warning_limit;
		    pal->lower_alarm_limit = ald.lower_alarm_limit;
		} else {
		    memset(pbuffer,'\0',dbr_alDouble_size);
		    *options = (*options) ^ DBR_AL_DOUBLE; /*Turn off option*/
		}
		*ppbuffer = ((char *)*ppbuffer) + dbr_alDouble_size;
	}
	return;
}

static void getOptions(DBADDR *paddr,char **poriginal,long *options,void *pflin)
{
	db_field_log	*pfl= (db_field_log *)pflin;
	struct rset	*prset;
	short		field_type=paddr->field_type;
	dbCommon	*pcommon;
	char		*pbuffer = *poriginal;

	prset=dbGetRset(paddr);
	/* Process options */
	pcommon = (dbCommon *)(paddr->precord);
	if( (*options) & DBR_STATUS ) {
	    unsigned short *pushort = (unsigned short *)pbuffer;

	    if(pfl!=NULL) {
		*pushort++ = pfl->stat;
		*pushort++ = pfl->sevr;
	    } else {
		*pushort++ = pcommon->stat;
		*pushort++ = pcommon->sevr;
	    }
	    *pushort++ = pcommon->acks;
	    *pushort++ = pcommon->ackt;
	    pbuffer = (char *)pushort;
	}
	if( (*options) & DBR_UNITS ) {
	    memset(pbuffer,'\0',dbr_units_size);
	    if( prset && prset->get_units ){ 
		(*prset->get_units)(paddr, pbuffer);
		pbuffer[DB_UNITS_SIZE-1] = '\0';
	    } else {
		*options ^= DBR_UNITS; /*Turn off DBR_UNITS*/
	    }
	    pbuffer += dbr_units_size;
	}
	if( (*options) & DBR_PRECISION ) {
	    struct dbr_precision *pdbr_precision=
		(struct dbr_precision *)pbuffer;

	    memset(pbuffer,'\0',dbr_precision_size);
	    if((field_type==DBF_FLOAT || field_type==DBF_DOUBLE)
	    &&  prset && prset->get_precision ){ 
		(*prset->get_precision)(paddr,pbuffer);
		if(pdbr_precision->field_width<=0)
			pdbr_precision->field_width =
				pdbr_precision->precision + 5;
	    } else {
		*options ^= DBR_PRECISION; /*Turn off DBR_PRECISION*/
	    }
	    pbuffer += dbr_precision_size;
	}
	if( (*options) & DBR_TIME ) {
	    unsigned long *pulong = (unsigned long *)pbuffer;
	    if(pfl!=NULL) {
		*pulong++ = pfl->time.secPastEpoch;
		*pulong++ = pfl->time.nsec;
	    } else {
		*pulong++ = pcommon->time.secPastEpoch;
		*pulong++ = pcommon->time.nsec;
	    }
	    pbuffer = (char *)pulong;
	}
	if( (*options) & DBR_ENUM_STRS )
	    get_enum_strs(paddr, &pbuffer, prset, options);
	if( (*options) & (DBR_GR_LONG|DBR_GR_DOUBLE ))
	    get_graphics(paddr, &pbuffer, prset, options);
	if((*options) & (DBR_CTRL_LONG | DBR_CTRL_DOUBLE ))
	    get_control(paddr, &pbuffer, prset, options);
	if((*options) & (DBR_AL_LONG | DBR_AL_DOUBLE ))
	    get_alarm(paddr, &pbuffer, prset, options);
	*poriginal = pbuffer;
}

struct rset * epicsShareAPI dbGetRset(const struct dbAddr *paddr)
{
	struct dbFldDes *pfldDes = (struct dbFldDes *)paddr->pfldDes;

	if(!pfldDes) return(0);
	return(pfldDes->pdbRecordType->prset);
}

long epicsShareAPI dbPutAttribute(
    const char *recordTypename,const char *name,const char*value)
{
	DBENTRY		dbEntry;
	DBENTRY		*pdbEntry = &dbEntry;
	long		status=0;

        if(!pdbbase) return(S_db_notFound);
	dbInitEntry(pdbbase,pdbEntry);
	status = dbFindRecordType(pdbEntry,recordTypename);
	if(!status) status = dbPutRecordAttribute(pdbEntry,name,value);
	dbFinishEntry(pdbEntry);
	if(status) errMessage(status,"dbPutAttribute failure");
	return(status);
}

int epicsShareAPI dbIsValueField(const struct dbFldDes *pdbFldDes)
{
    if(pdbFldDes->pdbRecordType->indvalFlddes == pdbFldDes->indRecordType)
	return(TRUE);
    else
	return(FALSE);
}

int epicsShareAPI dbGetFieldIndex(const struct dbAddr *paddr)
{
    return(((struct dbFldDes *)paddr->pfldDes)->indRecordType);
}

long epicsShareAPI dbGetNelements(const struct link *plink,long *nelements)
{
    switch(plink->type) {
    case CONSTANT:
	*nelements = 0;
	return(0);
    case DB_LINK: {
	    DBADDR *paddr = (DBADDR *)plink->value.pv_link.pvt;
	    *nelements = paddr->no_elements;
	    return(0);
	}
    case CA_LINK:
	return(dbCaGetNelements(plink,nelements));
    default:
	break;
    }
    return(S_db_badField);
}

int epicsShareAPI dbIsLinkConnected(const struct link *plink)
{
    switch(plink->type) {
	case DB_LINK: return(TRUE);
	case CA_LINK: return(dbCaIsLinkConnected(plink));
	default: break;
    }
    return(FALSE);
}

int epicsShareAPI dbGetLinkDBFtype(const struct link *plink)
{
    switch(plink->type) {
	case DB_LINK: 
	{
	    DBADDR *paddr = (DBADDR *)plink->value.pv_link.pvt;

	    return((int)paddr->field_type);
	}
	case CA_LINK: return(dbCaGetLinkDBFtype(plink));
	default: break;
    }
    return(-1);
}

/*
 *  Process a record if its scan field is passive.
 *     Will notify if processing is complete by callback.
 *       (only if you are interested in completion)
 */
long epicsShareAPI dbScanPassive(dbCommon *pfrom, dbCommon *pto)
{
    long status;
	
    /* if not passive just return success */
    if(pto->scan != 0) return(0);

    if(pfrom && pfrom->ppn) dbNotifyAdd(pfrom,pto);
    status = dbProcess(pto);
    return(status);
}

/*KLUDGE: Following needed so that dbPutLink to PROC field works correctly*/
long epicsShareAPI dbScanLink(dbCommon *pfrom, dbCommon *pto)
{
    long		status;
    unsigned char	pact;

    if(pfrom && pfrom->ppn) dbNotifyAdd(pfrom,pto);
    pact = pfrom->pact;
    pfrom->pact = TRUE;
    status = dbProcess(pto);
    pfrom->pact = pact;
    return(status);
}

void epicsShareAPI dbScanFwdLink(struct link *plink)
{
    dbCommon		*precord;
    struct pv_link      *pvlink;
    short               fwdLinkValue;

    if(plink->type!=DB_LINK && plink->type!=CA_LINK) return;
    pvlink = &plink->value.pv_link;
    precord = pvlink->precord;
    if(plink->type==DB_LINK) {
        dbAddr *paddr = (dbAddr *)plink->value.pv_link.pvt;
        dbScanPassive(precord,paddr->precord);
        return;
    }
    if(!(pvlink->pvlMask & pvlOptFWD)) return;
    fwdLinkValue = 1;
    dbCaPutLink(plink,DBR_SHORT,&fwdLinkValue,1);
    return;
}

/*
 *   Process the record.
 *     1.  Check for breakpoints.
 *     2.  Check the process active flag (PACT).
 *     3.  Check the disable link.
 *     4.  Check the RSET (record support entry table) exists.
 *     5.  Run the process routine specific to the record type.
 *     6.  Check to see if record contents should be automatically printed.
 */
long epicsShareAPI dbProcess(dbCommon *precord)
{
	struct rset	*prset = precord->rset;
	dbRecordType	*pdbRecordType = precord->rdes;
	unsigned char	tpro=precord->tpro;
	long		status = 0;
        int             *ptrace;
	int		set_trace=FALSE;
	dbFldDes	*pdbFldDes;
        int             callNotifyCompletion = FALSE;

        ptrace = dbLockSetAddrTrace(precord);
        /*
         *  Note that it is likely that if any changes are made
         *   to dbProcess() corresponding changes will have to
         *   be made in the breakpoint handler.
         */
 
        /* see if there are any stopped records or breakpoints */
        if (lset_stack_count != 0) {
           /*
            *  Check to see if the record should be processed
            *   and activate breakpoint accordingly.  If this
            *   function call returns non-zero, skip record
            *   support and fall out of dbProcess().  This is
            *   done so that a dbContTask() can be spawned to
            *   take over record processing for the lock set
            *   containing a breakpoint.
            */
            if (dbBkpt(precord))
                goto all_done;
        }
    
	/* check for trace processing*/
	if (tpro) {
                if(*ptrace==0) {
                        *ptrace = 1;
			set_trace = TRUE;
		}
	}

	/* If already active dont process */
	if (precord->pact) {
		unsigned short	monitor_mask;

		if (*ptrace) printf("%s: Active %s\n",
                        epicsThreadGetNameSelf(), precord->name);
		/* raise scan alarm after MAX_LOCK times */
		if (precord->stat==SCAN_ALARM) goto all_done;
		if (precord->lcnt++ !=MAX_LOCK) goto all_done;
		if (precord->sevr>=INVALID_ALARM) goto all_done;
		recGblSetSevr(precord, SCAN_ALARM, INVALID_ALARM);
		monitor_mask = recGblResetAlarms(precord);
		monitor_mask |= DBE_VALUE|DBE_LOG;
		pdbFldDes = pdbRecordType->papFldDes[pdbRecordType->indvalFlddes];
		db_post_events(precord,
			(void *)(((char *)precord) + pdbFldDes->offset),
			monitor_mask);
		goto all_done;
	}
        else precord->lcnt = 0;

       /*
        *  Check the record disable link.  A record will not be
        *    processed if the value retrieved through this link
        *    is equal to constant set in the record's disv field.
        */
        status = dbGetLink(&(precord->sdis),DBR_SHORT,&(precord->disa),0,0);

	/* if disabled check disable alarm severity and return success */
	if (precord->disa == precord->disv) {
		if(*ptrace) printf("%s: Disabled %s\n",
                        epicsThreadGetNameSelf(), precord->name);
		/*take care of caching and notifyCompletion*/
		precord->rpro = FALSE;
		precord->putf = FALSE;
                callNotifyCompletion = TRUE;
		/* raise disable alarm */
		if (precord->stat==DISABLE_ALARM) goto all_done;
		precord->sevr = precord->diss;
		precord->stat = DISABLE_ALARM;
		precord->nsev = 0;
		precord->nsta = 0;
		db_post_events(precord, &precord->stat, DBE_VALUE);
		db_post_events(precord, &precord->sevr, DBE_VALUE);
		pdbFldDes = pdbRecordType->papFldDes[pdbRecordType->indvalFlddes];
		db_post_events(precord,
			(void *)(((char *)precord) + pdbFldDes->offset),
			DBE_VALUE|DBE_ALARM);
		goto all_done;
	}

	/* locate record processing routine */
        /* put this in iocInit() !!! */
	if (!(prset=precord->rset) || !(prset->process)) {
                callNotifyCompletion = TRUE;
		precord->pact=1;/*set pact TRUE so error is issued only once*/
		recGblRecordError(S_db_noRSET, (void *)precord, "dbProcess");
		status = S_db_noRSET;
		if (*ptrace) printf("%s: No RSET for %s\n",
				epicsThreadGetNameSelf(), precord->name);
		goto all_done;
	}
	if(*ptrace) printf("%s: Process %s\n",
			epicsThreadGetNameSelf(), precord->name);
	/* process record */
	status = (*prset->process)(precord);
        /* Print record's fields if PRINT_MASK set in breakpoint field */
        if (lset_stack_count != 0) {
                dbPrint(precord);
        }
all_done:
	if (set_trace) *ptrace = 0;
	if(callNotifyCompletion && precord->ppn) dbNotifyCompletion(precord);
	return(status);
}

/*
 *  Fill out a database structure (*paddr) for
 *    a record given by the name "pname."
 *
 *    Returns error codes from StaticLib module, not
 *        from dbAccess.
 */
long epicsShareAPI dbNameToAddr(const char *pname,DBADDR *paddr)
{
	DBENTRY		dbEntry;
	long		status=0;
	struct rset	*prset;
	dbFldDes	*pflddes;

    if((pname == NULL) || (*pname == '\0'))
        return S_db_notFound;
        if(!pdbbase) return(S_db_notFound);
	dbInitEntry(pdbbase,&dbEntry);
	status = dbFindRecord(&dbEntry,pname);
	if(!status && !dbEntry.pfield) status=dbFindField(&dbEntry,"VAL");
	if(status) {
	    dbFinishEntry(&dbEntry);
	    return(status);
	}
	paddr->precord = dbEntry.precnode->precord;
	paddr->pfield = dbEntry.pfield;
	pflddes = dbEntry.pflddes;
	dbFinishEntry(&dbEntry);
	paddr->pfldDes = (void *)pflddes;
	paddr->field_type = pflddes->field_type;
	paddr->dbr_field_type = mapDBFToDBR[pflddes->field_type];
	paddr->field_size = pflddes->size;
	paddr->special = pflddes->special;

	/*if special is SPC_DBADDR then call cvt_dbaddr		*/
	/*it may change pfield,no_elements,field_type,dbr_field_type,*/
	/*field_size,and special*/
	paddr->no_elements=1;
	if(((paddr->special)==SPC_DBADDR)
	&& (prset=dbGetRset(paddr))
	&& (prset->cvt_dbaddr)) status = (*prset->cvt_dbaddr)(paddr);
	return(status);
}

/* JOH 10-19-04 */
static char * dbCopyInNameComponentOfPV (
    char * pBuf, unsigned bufLen, const char * pComponent )
{
    unsigned compLen = strlen ( pComponent );
    if ( compLen < bufLen ) {
        strcpy ( pBuf, pComponent );
        return pBuf + compLen;
    }
    else {
        unsigned reducedSize = bufLen - 1u;
        strncpy ( pBuf, pComponent, reducedSize );
        pBuf[reducedSize] = '\0';
        return pBuf + reducedSize;
    }
}
/*
 *  Copies PV name into pBuf of length bufLen
 *
 *    Returns the number of bytes written to pBuf
 *      not including null termination.
 *  JOH 10-19-04 
 */
unsigned dbNameOfPV (
    const dbAddr * paddr, char * pBuf, unsigned bufLen )
{
    dbFldDes * pfldDes = paddr->pfldDes;
    char * pBufTmp = pBuf;
    if ( bufLen == 0u ) {
        return 0u;
    }
    pBufTmp = dbCopyInNameComponentOfPV ( 
        pBufTmp, bufLen, paddr->precord->name );
    pBufTmp = dbCopyInNameComponentOfPV ( 
        pBufTmp, bufLen - ( pBufTmp - pBuf ), "." );
    pBufTmp = dbCopyInNameComponentOfPV ( 
        pBufTmp, bufLen - ( pBufTmp - pBuf ), pfldDes->name );
    return pBufTmp - pBuf;
}
/*
 *    Returns the number of bytes in the PV name
 *      not including null termination.
 *    JOH 10-19-04 
 */
unsigned dbNameSizeOfPV ( const dbAddr * paddr )
{
    unsigned recNameLen = strlen ( paddr->precord->name );
    dbFldDes * pfldDes = paddr->pfldDes;
    unsigned fieldNameLen = strlen ( pfldDes->name );
    return recNameLen + fieldNameLen + 1;
}

long epicsShareAPI dbValueSize(
	short     dbr_type
)
{
     /* sizes for value associated with each DBR request type */
     static long size[] = {
        MAX_STRING_SIZE,                /* STRING       */
        sizeof(char),                   /* CHAR         */
        sizeof(unsigned char),          /* UCHAR        */
        sizeof(short),                  /* SHORT        */
        sizeof(unsigned short),         /* USHORT       */
        sizeof(long),                   /* LONG         */
        sizeof(unsigned long),          /* ULONG        */
        sizeof(float),                  /* FLOAT        */
        sizeof(double),                 /* DOUBLE       */
        sizeof(unsigned short)};        /* ENUM         */

     return(size[dbr_type]);
}


long epicsShareAPI dbBufferSize(
     short     dbr_type,
     long      options,
     long      no_elements
)
{
    long nbytes=0;

    nbytes += dbValueSize(dbr_type) * no_elements;
    if(options & DBR_STATUS)	nbytes += dbr_status_size;
    if(options & DBR_UNITS)	nbytes += dbr_units_size;
    if(options & DBR_PRECISION) nbytes += dbr_precision_size;
    if(options & DBR_TIME)	nbytes += dbr_time_size;
    if(options & DBR_ENUM_STRS)	nbytes += dbr_enumStrs_size;
    if(options & DBR_GR_LONG)	nbytes += dbr_grLong_size;
    if(options & DBR_GR_DOUBLE)	nbytes += dbr_grDouble_size;
    if(options & DBR_CTRL_LONG) nbytes += dbr_ctrlLong_size;
    if(options & DBR_CTRL_DOUBLE)nbytes += dbr_ctrlDouble_size;
    if(options & DBR_AL_LONG)   nbytes += dbr_alLong_size;
    if(options & DBR_AL_DOUBLE) nbytes += dbr_alDouble_size;
    return(nbytes);
}
int epicsShareAPI dbLoadDatabase(char *filename,char *path,char *substitutions)
{
    int status;
    status = dbReadDatabase(&pdbbase,filename,path,substitutions);
    return(status);
}

int epicsShareAPI dbLoadRecords(char* pfilename, char* substitutions)
{
    return(dbReadDatabase(&pdbbase,pfilename,0,substitutions));
}


long epicsShareAPI dbGetLinkValue(struct link *plink,
    short dbrType, void *pbuffer,long *poptions, long *pnRequest)
{
    long		status = 0;

    if(plink->type==CONSTANT) {
	if(poptions) *poptions = 0;
	if(pnRequest) *pnRequest = 0;
    } else if(plink->type==DB_LINK) {
	struct pv_link	*ppv_link = &(plink->value.pv_link);
	DBADDR		*paddr = ppv_link->pvt;
	dbCommon	*precord = plink->value.pv_link.precord;

	/* scan passive records with links that are process passive  */
	if(ppv_link->pvlMask&pvlOptPP) {
	    dbCommon	*pfrom = paddr->precord;
	    unsigned char   pact;

	    pact = precord->pact;
	    precord->pact = TRUE;
	    status = dbScanPassive(precord,pfrom);
	    precord->pact = pact;
	    if(status) return(status);
	}
	if(ppv_link->pvlMask&pvlOptMS && precord!= paddr->precord)
	    recGblSetSevr(precord,LINK_ALARM,paddr->precord->sevr);

	if(ppv_link->getCvt && ppv_link->lastGetdbrType==dbrType) {
	    status = (*ppv_link->getCvt)(paddr->pfield,pbuffer, paddr);
	} else {
	    unsigned short	dbfType= paddr->field_type;
	    long		no_elements = paddr->no_elements;

	    if((dbrType<0) || (dbrType>DBR_ENUM) || (dbfType > DBF_DEVICE)) {
		status = S_db_badDbrtype;
		recGblRecordError(status,(void *)precord,"GetLinkValue Failed");
		recGblSetSevr(precord,LINK_ALARM,INVALID_ALARM);
		return(status);
	    }
	    /*  attempt to make a fast link */
	    if((!poptions || (*poptions == 0))
	    && (no_elements == 1)
	    && (!pnRequest || (*pnRequest == 1))
	    && (paddr->special!=SPC_ATTRIBUTE) )
	    {
		ppv_link->getCvt = dbFastGetConvertRoutine[dbfType][dbrType];
		status = (*ppv_link->getCvt) (paddr->pfield,pbuffer, paddr);
	    }else{
		status=dbGet(paddr,dbrType,pbuffer,poptions,pnRequest,NULL);
		ppv_link->getCvt = 0;
	    }
	}
	ppv_link->lastGetdbrType = dbrType;
	if(status){
	    recGblRecordError(status,(void *)precord,"dbGetLinkValue");
	    recGblSetSevr(precord,LINK_ALARM,INVALID_ALARM);
	}
    }else if(plink->type==CA_LINK) {
	struct dbCommon	*precord = plink->value.pv_link.precord;
	const struct pv_link	*pcalink = &(plink->value.pv_link);
	unsigned short sevr;

	status=dbCaGetLink(plink,dbrType,pbuffer,&sevr,pnRequest);
	if(status) {
	    recGblSetSevr(precord,LINK_ALARM,INVALID_ALARM);
	}else if(pcalink->pvlMask&pvlOptMS){
	    recGblSetSevr(precord,LINK_ALARM,sevr);
	}
	if(poptions) *poptions = 0;
    } else {
	cantProceed("dbGetLinkValue: Illegal link type");
    }
    return(status);
}

long epicsShareAPI dbPutLinkValue(struct link *plink,
    short dbrType,const void *pbuffer,long nRequest)
{
    long	status=0;

    if(plink->type==DB_LINK) {
        struct dbCommon	*psource = plink->value.pv_link.precord;
        struct pv_link	*ppv_link= &(plink->value.pv_link);
        DBADDR		*paddr	= (DBADDR*)(ppv_link->pvt);
        dbCommon 	*pdest	= paddr->precord;

        status=dbPut(paddr,dbrType,pbuffer,nRequest);
        if(ppv_link->pvlMask&pvlOptMS)
            recGblSetSevr(pdest,LINK_ALARM,psource->nsev);
        if(status) return(status);
        if((paddr->pfield==(void *)&pdest->proc)
        || (ppv_link->pvlMask&pvlOptPP && pdest->scan==0)) {
            /*if dbPutField caused asyn record to process   */
            /* ask for reprocessing*/
            if(pdest->putf) {
                pdest->rpro = TRUE;
            } else { /* otherwise ask for the record to be processed*/
                status=dbScanLink(psource,pdest);
            }
        }
        if(status) recGblSetSevr(psource,LINK_ALARM,INVALID_ALARM);
    } else if(plink->type==CA_LINK) {
        struct dbCommon	*psource = plink->value.pv_link.precord;

        status = dbCaPutLink(plink,dbrType,pbuffer, nRequest);
        if(status < 0)
            recGblSetSevr(psource,LINK_ALARM,INVALID_ALARM);
    } else {
            cantProceed("dbPutLinkValue: Illegal link type");
    }
    return(status);
}

long epicsShareAPI dbGetField(DBADDR *paddr,short dbrType,
    void *pbuffer, long *options,long *nRequest,void *pflin)
{
    short	dbfType = paddr->field_type;
    dbCommon	*precord = (dbCommon *)(paddr->precord);
    long	status = 0;

    dbScanLock(precord);
    if(dbfType>=DBF_INLINK && dbfType<=DBF_FWDLINK) {
	DBENTRY	dbEntry;
	dbFldDes *pfldDes = (dbFldDes *)paddr->pfldDes;
	char	*rtnString;
	char	*pbuf = (char *)pbuffer;

	if(dbrType!=DBR_STRING) {
	    status = S_db_badDbrtype;
	    goto done;
	}
	if (options && (*options))
	    getOptions(paddr, &pbuf, options, pflin);
	if(nRequest && *nRequest==0) goto done;
	dbInitEntry(pdbbase,&dbEntry);
	status = dbFindRecord(&dbEntry,precord->name);
	if(!status) status = dbFindField(&dbEntry,pfldDes->name);
	if(!status) {
	    rtnString = dbGetString(&dbEntry);
	    /*begin kludge for old db_access MAX_STRING_SIZE*/
	    if(strlen(rtnString)>=MAX_STRING_SIZE) {
	        strncpy(pbuf,rtnString,MAX_STRING_SIZE-1);
	        pbuf[MAX_STRING_SIZE-1] = 0;
	    } else {
	        strcpy(pbuf,rtnString);
	    }
	    /*end kludge for old db_access MAX_STRING_SIZE*/
	}
	dbFinishEntry(&dbEntry);
    } else {
	status = dbGet(paddr,dbrType,pbuffer,options,nRequest,pflin);
    }
done:
    dbScanUnlock(precord);
    return(status);
}

long epicsShareAPI dbGet(DBADDR *paddr,short dbrType,
    void *pbuffer,long *options,long *nRequest,void *pflin)
{
	db_field_log	*pfl= (db_field_log *)pflin;
	long		no_elements=paddr->no_elements;
	long 		offset;
	struct rset	*prset;
	short		field_type=paddr->field_type;
	long		(*pconvert_routine)();
	long		status = 0;
	char		message[80];

	if(paddr->special == SPC_ATTRIBUTE) {
	    if(dbrType!=DBR_STRING) return(S_db_badDbrtype);
	    if(!paddr->pfield) return(S_db_badField);
	    strcpy((char *)pbuffer,(char *)paddr->pfield);
	    return(0);
	}
	prset=dbGetRset(paddr);
	if(options && (*options)) {
		char *pbuf = pbuffer;

		getOptions(paddr,&pbuf,options,pflin);
		pbuffer = pbuf;
	}
        if(nRequest && *nRequest==0) return(0);
	/* Check for valid request */
	if( INVALID_DB_REQ(dbrType) || (field_type>DBF_DEVICE) ){
		sprintf(message,"dbGet - database request type is %d",dbrType);
		recGblDbaddrError(S_db_badDbrtype,paddr,message);
		return(S_db_badDbrtype);
	}
	/* check for array			*/
	if( no_elements>1 && prset && (prset->get_array_info) ) {
		status = (*prset->get_array_info)(paddr,&no_elements,&offset);
	}
	else offset=0;
	if(offset==0 && (!nRequest || no_elements==1)) {
		if(nRequest) *nRequest = 1;
		if(pfl!=NULL) {
		    DBADDR	localAddr;

		    localAddr = *paddr; /*Structure copy*/
		    localAddr.pfield = (char *)&pfl->field;
		    status = (*dbFastGetConvertRoutine[field_type][dbrType])
				(localAddr.pfield,pbuffer, &localAddr);
		} else {
		    status = (*dbFastGetConvertRoutine[field_type][dbrType])
				(paddr->pfield,pbuffer, paddr);
		}
	} else {
		long	n;

		if(nRequest) {
			if(no_elements<(*nRequest)) *nRequest = no_elements;
			n = *nRequest;
		} else {
			n = 1;
		}
		pconvert_routine=dbGetConvertRoutine[field_type][dbrType];
		if(!pconvert_routine) {
			sprintf(message,"dbGet - database request type is %d",
				dbrType);
			recGblDbaddrError(S_db_badDbrtype,paddr,message);
			return(S_db_badDbrtype);
		}
		/* convert database field  and place it in the buffer */
		if(n<=0) {
			;/*do nothing*/
		} else if(pfl!=NULL) {
		    DBADDR	localAddr;
	
		    localAddr = *paddr; /*Structure copy*/
		    localAddr.pfield = (char *)&pfl->field;
		    status=(*pconvert_routine)(&localAddr,pbuffer,n,
				no_elements,offset);
		} else {
		    status=(*pconvert_routine)(paddr,pbuffer,n,
				no_elements,offset);
		}
	}
        return(status);
}

devSup* epicsShareAPI dbDTYPtoDevSup(dbRecordType *pdbRecordType, int dtyp) {
    return (devSup *)ellNth(&pdbRecordType->devList, dtyp+1);
}

devSup* epicsShareAPI dbDSETtoDevSup(dbRecordType *prdes, struct dset *pdset) {
    devSup *pdevSup = (devSup *)ellFirst(&prdes->devList);
    while (pdevSup) {
        if (pdset == pdevSup->pdset) return pdevSup;
        pdevSup = (devSup *)ellNext(&pdevSup->node);
    }
    return NULL;
}

static long dbPutFieldLink(
    DBADDR *paddr,short dbrType,const void *pbuffer,long nRequest)
{
    long        status = 0;
    long        special=paddr->special;
    dbCommon    *precord = (dbCommon *)(paddr->precord);
    DBLINK  *plink = (DBLINK *)paddr->pfield;
    const char  *pstring = (const char *)pbuffer;
    DBENTRY	dbEntry;
    dbFldDes    *pfldDes = (dbFldDes *)paddr->pfldDes;
    DBADDR      dbaddr;
    devSup *pdevSup = NULL;
    struct dset *new_dset = NULL;
    struct dsxt *new_dsxt = NULL;
    int inpOut;
    short scan;

    if ((dbrType!=DBR_STRING) && (dbrType!=DBR_CHAR) && (dbrType!=DBR_UCHAR))
        return S_db_badDbrtype;

    if (((dbrType==DBR_CHAR) || (dbrType==DBR_UCHAR)) &&
        (pstring[nRequest] != '\0'))
        return S_db_badField;

    dbInitEntry(pdbbase,&dbEntry);
    status=dbFindRecord(&dbEntry,precord->name);
    if (!status) status=dbFindField(&dbEntry,pfldDes->name);
    if (status) return status;

    inpOut = !(strcmp(pfldDes->name,"INP") && strcmp(pfldDes->name,"OUT"));

    dbLockSetGblLock();
    dbLockSetRecordLock(precord);

    if (inpOut) {
        pdevSup = dbDTYPtoDevSup(precord->rdes, precord->dtyp);
        if (pdevSup == NULL ||
            (new_dset = pdevSup->pdset) == NULL ||
            (new_dsxt = pdevSup->pdsxt) == NULL ||
            new_dsxt->add_record == NULL ||
            (precord->dset &&
             ((pdevSup = dbDSETtoDevSup(precord->rdes, precord->dset)) == NULL ||
              pdevSup->pdsxt == NULL ||
              pdevSup->pdsxt->del_record == NULL))) {
            status = S_db_noSupport;
            goto unlock;
        }
    }

    scan = precord->scan;
    if (inpOut && (scan==SCAN_IO_EVENT)) {
        scanDelete(precord);
        precord->scan = SCAN_PASSIVE;
    }

    switch (plink->type) { /* Old link type */
    case DB_LINK:
        free(plink->value.pv_link.pvt);
        plink->value.pv_link.pvt = 0;
        plink->type = PV_LINK;
        plink->value.pv_link.getCvt = 0;
        plink->value.pv_link.pvlMask = 0;
        plink->value.pv_link.lastGetdbrType = 0;
        dbLockSetSplit(precord);
        break;

    case CA_LINK:
        dbCaRemoveLink(plink);
        plink->type = PV_LINK;
        plink->value.pv_link.getCvt = 0;
        plink->value.pv_link.pvlMask = 0;
        plink->value.pv_link.lastGetdbrType = 0;
        break;

    case CONSTANT:
        break;  /* do nothing */

    case PV_LINK:
    case MACRO_LINK:
        break;  /* should never get here */

    default: /* Hardware address */
        if (!inpOut)
            status = S_db_badHWaddr;
        else
            if (precord->dset)
                status = pdevSup->pdsxt->del_record(precord);
        if (status) goto restoreScan;
        break;
    }

    if (special) status = putSpecial(paddr,0);
    if (!status) status = dbPutString(&dbEntry,pstring);

    if (inpOut) {
        precord->dset = new_dset;
        precord->dpvt = NULL;
        precord->pact = FALSE;
    }

    if (!status && special) status = putSpecial(paddr,1);
    if (status) {
        if (inpOut) {
            precord->dset = NULL;
            precord->pact = TRUE;
        }
        goto postScanEvent;
    }

    switch (plink->type) { /* New link type */
    case PV_LINK:
        if (plink==&precord->tsel) recGblTSELwasModified(plink);
        if (!(plink->value.pv_link.pvlMask & (pvlOptCA|pvlOptCP|pvlOptCPP)) &&
            (dbNameToAddr(plink->value.pv_link.pvname,&dbaddr)==0)) {
            /* It's a DB link */
            DBADDR      *pdbAddr;

            plink->type = DB_LINK;
            pdbAddr = dbCalloc(1,sizeof(struct dbAddr));
            *pdbAddr = dbaddr; /* NB: structure copy */;
            plink->value.pv_link.precord = precord;
            plink->value.pv_link.pvt = pdbAddr;
            dbLockSetRecordLock(pdbAddr->precord);
            dbLockSetMerge(precord,pdbAddr->precord);
        } else { /* Make it a CA link */
            char        *pperiod;

            plink->type = CA_LINK;
            plink->value.pv_link.precord = precord;
            if (pfldDes->field_type==DBF_INLINK) {
                plink->value.pv_link.pvlMask |= pvlOptInpNative;
            }
            dbCaAddLink(plink);
            if (pfldDes->field_type==DBF_FWDLINK) {
                pperiod = strrchr(plink->value.pv_link.pvname,'.');
                if (pperiod && strstr(pperiod,"PROC"))
                    plink->value.pv_link.pvlMask |= pvlOptFWD;
            }
        }
        break;

    case CONSTANT:
        break;

    case DB_LINK:
    case CA_LINK:
    case MACRO_LINK:
        break;  /* should never get here */

    default: /* Hardware address */
        if (!inpOut)
            status = S_db_badHWaddr;
        else
            status = new_dsxt->add_record(precord);
        if (status) {
            precord->dset = NULL;
            precord->pact = TRUE;
            goto postScanEvent;
        }
        break;
    }
    db_post_events(precord,plink,DBE_VALUE|DBE_LOG);

restoreScan:
    if (inpOut && (scan==SCAN_IO_EVENT)) { /* undo scanDelete() */
        precord->scan = scan;
        scanAdd(precord);
    }
postScanEvent:
    if (scan != precord->scan)
        db_post_events(precord, &precord->scan, DBE_VALUE|DBE_LOG);
unlock:
    dbLockSetGblUnlock();
    dbFinishEntry(&dbEntry);
    return status;
}

long epicsShareAPI dbPutField(
    DBADDR *paddr,short dbrType,const void *pbuffer,long  nRequest)
{
    long	status = 0;
    long	special=paddr->special;
    dbFldDes	*pfldDes=(dbFldDes *)(paddr->pfldDes);
    dbCommon 	*precord = (dbCommon *)(paddr->precord);
    short	dbfType = paddr->field_type;

    if(special==SPC_ATTRIBUTE)
        return S_db_noMod;

    /*check for putField disabled*/
    if (precord->disp &&
        (void *)(&precord->disp) != paddr->pfield)
        return(S_db_putDisabled);

    if(dbfType>=DBF_INLINK && dbfType<=DBF_FWDLINK)
        return dbPutFieldLink(paddr,dbrType,pbuffer,nRequest);

    dbScanLock(precord);
    status=dbPut(paddr,dbrType,pbuffer,nRequest);
    if(status==0){
        if((paddr->pfield==(void *)&precord->proc)
	||(pfldDes->process_passive && precord->scan==0 
	&& dbrType<DBR_PUT_ACKT)) {
	    if(precord->pact) {
		if(precord->tpro)
		    printf("%s: Active %s\n",
                        epicsThreadGetNameSelf(), precord->name);
		precord->rpro = TRUE;
	    } else {
		/*indicate that dbPutField called dbProcess*/
		precord->putf = TRUE;
		status=dbProcess(precord);
	    }
	}
    }
    dbScanUnlock(precord);
    return(status);
}

static long putAckt(DBADDR *paddr, const unsigned short *pbuffer, long nRequest,
    long no_elements, long offset)
{
    dbCommon *precord=(dbCommon *)(paddr->precord);

    if(*pbuffer == precord->ackt) return(0);
    precord->ackt = *pbuffer;
    db_post_events(precord,&precord->ackt,DBE_VALUE|DBE_ALARM);
    if(!precord->ackt && precord->acks>precord->sevr) {
	precord->acks = precord->sevr;
	db_post_events(precord,&precord->acks,DBE_VALUE|DBE_ALARM);
    }
    db_post_events(precord,NULL,DBE_ALARM);
    return(0);
}

static long putAcks(DBADDR *paddr, const unsigned short *pbuffer, long nRequest,
    long no_elements, long offset)
{
    dbCommon *precord=(dbCommon *)(paddr->precord);

    if(*pbuffer >= precord->acks) {
	precord->acks = 0;
	db_post_events(precord,NULL,DBE_ALARM);
	db_post_events(precord,&precord->acks,DBE_VALUE|DBE_ALARM);
    }
    return(0);
}

long epicsShareAPI dbPut(DBADDR *paddr,short dbrType,
    const void *pbuffer,long nRequest)
{
	long		no_elements=paddr->no_elements;
	long		dummy;
	long		offset;
	struct rset	*prset;
	dbCommon 	*precord=(dbCommon *)(paddr->precord);
	long		status=0;
	dbFldDes	*pfldDes;
	long		special=paddr->special;
	short		field_type=paddr->field_type;
	int		isValueField;
	char		message[80];

        if(special==SPC_ATTRIBUTE) return(S_db_noMod);
	if(dbrType==DBR_PUT_ACKT && field_type<=DBF_DEVICE) {
	    return putAckt(paddr,(const unsigned short*)pbuffer,1l,1l,0l);
	} else if(dbrType==DBR_PUT_ACKS && field_type<=DBF_DEVICE) {
	    return putAcks(paddr,(const unsigned short*)pbuffer,1l,1l,0l);
	} else if( INVALID_DB_REQ(dbrType) || (field_type>DBF_DEVICE)) {
		sprintf(message,"dbPut - database request type is %d",dbrType);
		recGblDbaddrError(S_db_badDbrtype,paddr,message);
		return(S_db_badDbrtype);
	}
	
	prset=dbGetRset(paddr);
	
	/* check for special processing	is required */
	if(special) {
	    status = putSpecial(paddr,0);
	    if(status) return(status);
	}
	if(no_elements<=1) {
		status = (*dbFastPutConvertRoutine[dbrType][field_type])
			(pbuffer,paddr->pfield, paddr);
	} else {
		if(prset && (prset->get_array_info) ) {
			status= (*prset->get_array_info)
				(paddr,&dummy,&offset);
		}
		else offset=0;
		if(no_elements<(nRequest)) nRequest = no_elements;
		status=(*dbPutConvertRoutine[dbrType][field_type])
			(paddr,pbuffer,nRequest,no_elements,offset);
		/* update array info	*/
		if(prset && (prset->put_array_info) && !status) {
			status= (*prset->put_array_info)
				(paddr,nRequest);
		}
	}
	if(status) return(status);

	/* check for special processing	is required */
	if(special) {
	    status = putSpecial(paddr,1);
	    if(status) return(status);
	}
	/* propagate events for this field */
	/* if the field is VAL and pvlMask&pvlOptPP is true dont propagate*/
	pfldDes = (dbFldDes *)(paddr->pfldDes);
	isValueField = dbIsValueField(pfldDes);
	if (isValueField) precord->udf=FALSE;
	if(precord->mlis.count &&
	(!isValueField || (!pfldDes->process_passive)))
		db_post_events(precord,paddr->pfield,DBE_VALUE|DBE_LOG);

	return(status);
}

/* various utility routines */
long epicsShareAPI dbGetControlLimits(
    const struct link *plink,double *low, double *high)
{
    struct buffer {
        DBRctrlDouble
        double value;
    } buffer;
    DBADDR *paddr;
    long options = DBR_CTRL_DOUBLE;
    long number_elements = 0;
    long status;

    if(plink->type == CA_LINK) return(dbCaGetControlLimits(plink,low,high));
    if(plink->type !=DB_LINK) return(S_db_notFound);
    paddr = (DBADDR *)plink->value.pv_link.pvt;
    status = dbGet(paddr,DBR_DOUBLE,&buffer,&options,&number_elements,0);
    if(status) return(status);
    *low = buffer.lower_ctrl_limit;
    *high = buffer.upper_ctrl_limit;
    return(0);
}

long epicsShareAPI dbGetGraphicLimits(
    const struct link *plink,double *low, double *high)
{
    struct buffer {
        DBRgrDouble
        double value;
    } buffer;
    DBADDR *paddr;
    long options = DBR_GR_DOUBLE;
    long number_elements = 0;
    long status;

    if(plink->type == CA_LINK) return(dbCaGetGraphicLimits(plink,low,high));
    if(plink->type !=DB_LINK) return(S_db_notFound);
    paddr = (DBADDR *)plink->value.pv_link.pvt;
    status = dbGet(paddr,DBR_DOUBLE,&buffer,&options,&number_elements,0);
    if(status) return(status);
    *low = buffer.lower_disp_limit;
    *high = buffer.upper_disp_limit;
    return(0);
}

long epicsShareAPI dbGetAlarmLimits(const struct link *plink,
	double *lolo, double *low, double *high, double *hihi)
{
    struct buffer {
        DBRalDouble
        double value;
    } buffer;
    DBADDR *paddr;
    long options = DBR_AL_DOUBLE;
    long number_elements = 0;
    long status;

    if(plink->type == CA_LINK)
        return(dbCaGetAlarmLimits(plink,lolo,low,high,hihi));
    if(plink->type !=DB_LINK) return(S_db_notFound);
    paddr = (DBADDR *)plink->value.pv_link.pvt;
    status = dbGet(paddr,DBR_DOUBLE,&buffer,&options,&number_elements,0);
    if(status) return(status);
    *lolo = buffer.lower_alarm_limit;
    *low = buffer.lower_warning_limit;
    *high = buffer.upper_warning_limit;
    *hihi = buffer.upper_alarm_limit;
    return(0);
}

long epicsShareAPI dbGetPrecision(const struct link *plink,short *precision)
{
    struct buffer {
        DBRprecision
        double value;
    } buffer;
    DBADDR *paddr;
    long options = DBR_PRECISION;
    long number_elements = 0;
    long status;

    if(plink->type == CA_LINK) return(dbCaGetPrecision(plink,precision));
    if(plink->type !=DB_LINK) return(S_db_notFound);
    paddr = (DBADDR *)plink->value.pv_link.pvt;
    status = dbGet(paddr,DBR_DOUBLE,&buffer,&options,&number_elements,0);
    if(status) return(status);
    *precision = buffer.precision;
    return(0);
}

long epicsShareAPI dbGetUnits(
    const struct link *plink,char *units,int unitsSize)
{
    struct buffer {
        DBRunits
        double value;
    } buffer;
    DBADDR *paddr;
    long options = DBR_UNITS;
    long number_elements = 0;
    long status;

    if(plink->type == CA_LINK) return(dbCaGetUnits(plink,units,unitsSize));
    if(plink->type !=DB_LINK) return(S_db_notFound);
    paddr = (DBADDR *)plink->value.pv_link.pvt;
    status = dbGet(paddr,DBR_DOUBLE,&buffer,&options,&number_elements,0);
    if(status) return(status);
    strncpy(units,buffer.units,unitsSize);
    return(0);
}

long epicsShareAPI dbGetSevr(const struct link *plink,short *severity)
{
    DBADDR *paddr;

    if(plink->type == CA_LINK) return(dbCaGetSevr(plink,severity));
    if(plink->type !=DB_LINK) return(S_db_notFound);
    paddr = (DBADDR *)plink->value.pv_link.pvt;
    *severity = paddr->precord->sevr;
    return(0);
}

long epicsShareAPI dbGetTimeStamp(const struct link *plink,epicsTimeStamp *pstamp)
{
    DBADDR *paddr;

    if(plink->type == CA_LINK) return(dbCaGetTimeStamp(plink,pstamp));
    if(plink->type !=DB_LINK) return(S_db_notFound);
    paddr = (DBADDR *)plink->value.pv_link.pvt;
    *pstamp = paddr->precord->time;
    return(0);
}
