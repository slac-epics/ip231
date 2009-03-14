/*
 
*****************************************************************
                          COPYRIGHT NOTIFICATION
*****************************************************************
 
THE FOLLOWING IS A NOTICE OF COPYRIGHT, AVAILABILITY OF THE CODE,
AND DISCLAIMER WHICH MUST BE INCLUDED IN THE PROLOGUE OF THE CODE
AND IN ALL SOURCE LISTINGS OF THE CODE.
 
(C)  COPYRIGHT 1993 UNIVERSITY OF CHICAGO
 
Argonne National Laboratory (ANL), with facilities in the States of
Illinois and Idaho, is owned by the United States Government, and
operated by the University of Chicago under provision of a contract
with the Department of Energy.
 
Portions of this material resulted from work developed under a U.S.
Government contract and are subject to the following license:  For
a period of five years from March 30, 1993, the Government is
granted for itself and others acting on its behalf a paid-up,
nonexclusive, irrevocable worldwide license in this computer
software to reproduce, prepare derivative works, and perform
publicly and display publicly.  With the approval of DOE, this
period may be renewed for two additional five year periods.
Following the expiration of this period or periods, the Government
is granted for itself and others acting on its behalf, a paid-up,
nonexclusive, irrevocable worldwide license in this computer
software to reproduce, prepare derivative works, distribute copies
to the public, perform publicly and display publicly, and to permit
others to do so.
 
*****************************************************************
                                DISCLAIMER
*****************************************************************
 
NEITHER THE UNITED STATES GOVERNMENT NOR ANY AGENCY THEREOF, NOR
THE UNIVERSITY OF CHICAGO, NOR ANY OF THEIR EMPLOYEES OR OFFICERS,
MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR ASSUMES ANY LEGAL
LIABILITY OR RESPONSIBILITY FOR THE ACCURACY, COMPLETENESS, OR
USEFULNESS OF ANY INFORMATION, APPARATUS, PRODUCT, OR PROCESS
DISCLOSED, OR REPRESENTS THAT ITS USE WOULD NOT INFRINGE PRIVATELY
OWNED RIGHTS.
 
*****************************************************************
LICENSING INQUIRIES MAY BE DIRECTED TO THE INDUSTRIAL TECHNOLOGY
DEVELOPMENT CENTER AT ARGONNE NATIONAL LABORATORY (708-252-2000).
*/

/*
 *
 *      Original author: 	John Winans
 *      Date from original:     9/27/93
 *      Current author:         Timo Korhonen
 *      Date                    06/28/01
 */

#include        <epicsStdio.h>  
#include        <dbDefs.h>
#include        <epicsPrint.h>
#include	<alarm.h>
#include	<dbAccess.h>
#include	<dbEvent.h>
#include	<dbFldTypes.h>
#include	<devSup.h>
#include	<errMdef.h>
#include	<recSup.h>
#include	<recGbl.h>
#include	<epicsExport.h>
#include	<erDefs.h>

#define GEN_SIZE_OFFSET
#include	<erRecord.h>
#undef  GEN_SIZE_OFFSET

#define STATIC	static

STATIC void ErMonitor(struct erRecord *pRec);

/* Create RSET - Record Support Entry Table*/
#define report NULL
#define initialize NULL
STATIC long ErInitRec(struct erRecord *, int);
STATIC long ErProc(struct erRecord *);
#define ErSpecial NULL
#define get_value NULL
#define cvt_dbaddr NULL
#define get_array_info NULL
#define put_array_info NULL
#define get_units NULL
#define get_precision NULL
#define get_enum_str NULL
#define get_enum_strs NULL
#define put_enum_str NULL
static long get_graphic_double(struct dbAddr *paddr, struct dbr_grDouble *pgd);
static long get_control_double(struct dbAddr *paddr, struct dbr_ctrlDouble *pcd);
static long get_alarm_double(struct dbAddr *paddr, struct dbr_alDouble *pad);

rset erRSET={
	RSETNUMBER,
	report,
	initialize,
	ErInitRec,
	ErProc,
	ErSpecial,
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

epicsExportAddress (rset, erRSET);

/******************************************************************************
 *
 * Init an ER record.
 *
 ******************************************************************************/
STATIC long ErInitRec(struct erRecord *pRec, int pass)
{
  ErDsetStruct	*pDset = (ErDsetStruct *) pRec->dset;

  if (pass == 1)
  {
    /* Init the card via driver calls */
    /* Make sure we have a usable device support module */
    if (pDset == NULL)
    {
      recGblRecordError(S_dev_noDSET,(void *)pRec, "er: ErInitRec");
      return(S_dev_noDSET);
    }
    if (pDset->number < 5)
    {
      recGblRecordError(S_dev_missingSup,(void *)pRec, "er: ErInitRec");
      return(S_dev_missingSup);
    }
    if( pDset->initRec != NULL) 
      return(*pDset->initRec)(pRec);
  }
  pRec->taxi = 0;
  pRec->ltax = 0;

  return(0);
}
/******************************************************************************
 *
 * Process an ER record.
 *
 ******************************************************************************/
STATIC long ErProc(struct erRecord *pRec)
{
  ErDsetStruct  *pDset = (ErDsetStruct *) pRec->dset;

  pRec->pact=TRUE;

  if (pRec->tpro > 10)
    printf("recEr::ErProc(%s) entered\n",  pRec->name);

  if (pDset->proc != NULL) {
    long status = (*pDset->proc)(pRec);
    if (status) return (status);
  }

  /* Take care of time stamps and such */
  pRec->udf=FALSE;
  /* tsLocalTime(&pRec->time);*/
  recGblGetTimeStamp(pRec);

  /* Deal with monitor stuff */
  ErMonitor(pRec);
  /* process the forward scan link record */
  recGblFwdLink(pRec);

  pRec->pact=FALSE;
  return(0);
}
#if 0  /* this is not needed */
/******************************************************************************
 *
 * Used to check for changed field values
 *
 ******************************************************************************/
STATIC long ErSpecial(struct dbAddr *paddr, int after)
{
  struct erRecord	*pRec = (struct erRecord *)(paddr->precord);
  void			*p

  if(!after) 
    return(0);

  /* Make sure we have the proper special flag type spec'd */
  if (pdbAddr->special != SPC_MOD)
  {
    recGblDbaddrError(S_db_badChoice,pdbAddr,"bp: special");
    return(S_db_badChoice);
  }
  p = (void *)(pdbAddr->pfield);

  /* Figure out which field has been changed */
  if (p == &pRec->dg0d)
    pRec->dgcm |= 1;
  else ...

  return(0);
}
#endif
/******************************************************************************
 *
 * Post any events for fields that might have changed while processing.
 *
 ******************************************************************************/
STATIC void ErMonitor(struct erRecord *pRec)
{
  unsigned short  monitor_mask;

  monitor_mask = recGblResetAlarms(pRec);
  monitor_mask |= (DBE_VALUE | DBE_LOG);
  db_post_events(pRec, &pRec->val, monitor_mask);

  if (pRec->taxi != pRec->ltax) {
    pRec->ltax = pRec->taxi;
    db_post_events(pRec, &pRec->plok, monitor_mask);    
    db_post_events(pRec, &pRec->taxi, monitor_mask);
  }
  return;
}
static long get_graphic_double(struct dbAddr *paddr, struct dbr_grDouble *pgd)
{
  struct erRecord     *pRec=(struct erRecord *)paddr->precord;
 
  if(paddr->pfield==(void *)&pRec->val)
  {
    pgd->upper_disp_limit = 0;
    pgd->lower_disp_limit = 0;
  }
  else if(paddr->pfield==(void *)&pRec->dg0d
       || paddr->pfield==(void *)&pRec->dg0w
       || paddr->pfield==(void *)&pRec->dg1d
       || paddr->pfield==(void *)&pRec->dg1w
       || paddr->pfield==(void *)&pRec->dg2d
       || paddr->pfield==(void *)&pRec->dg2w
       || paddr->pfield==(void *)&pRec->dg3d
       || paddr->pfield==(void *)&pRec->dg3w)
  {
    pgd->upper_disp_limit = 64*1024;
    pgd->lower_disp_limit = 0;
  }
  else 
    recGblGetGraphicDouble(paddr,pgd);
  return(0);
}
 
static long get_control_double(struct dbAddr *paddr, struct dbr_ctrlDouble *pcd)
{
  struct erRecord     *pRec=(struct erRecord *)paddr->precord;
 
  if(paddr->pfield==(void *)&pRec->val)
  {
    pcd->upper_ctrl_limit = 0;
    pcd->lower_ctrl_limit = 1;
  }
  else if(paddr->pfield==(void *)&pRec->dg0d
       || paddr->pfield==(void *)&pRec->dg0w
       || paddr->pfield==(void *)&pRec->dg1d
       || paddr->pfield==(void *)&pRec->dg1w
       || paddr->pfield==(void *)&pRec->dg2d
       || paddr->pfield==(void *)&pRec->dg2w
       || paddr->pfield==(void *)&pRec->dg3d
       || paddr->pfield==(void *)&pRec->dg3w)
  {
    pcd->upper_ctrl_limit = 64*1024;
    pcd->lower_ctrl_limit = 0;
  }
  else
    recGblGetControlDouble(paddr,pcd);
    return(0);
}
 
static long get_alarm_double(struct dbAddr *paddr, struct dbr_alDouble *pad)
{
#if 0
  {
     pad->upper_alarm_limit = 2;
     pad->upper_warning_limit = 2;
     pad->lower_warning_limit = -1;
     pad->lower_alarm_limit = -1;
  } else 
#endif
    recGblGetAlarmDouble(paddr,pad);
  return(0);
}
