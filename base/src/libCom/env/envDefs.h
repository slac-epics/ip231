/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/*	envDefs.h,v 1.21.2.3 2004/09/03 21:35:42 jhill Exp	
 *	Author:	Roger A. Cole
 *	Date:	07-20-91
 *
 * make options
 *	-DNDEBUG	don't compile assert() checking
 *      -DDEBUG         compile various debug code, including checks on
 *                      malloc'd memory
 */
/*+/mod***********************************************************************
* TITLE	envDefs.h - definitions for environment get/set routines
*
* DESCRIPTION
*	This file defines the environment parameters for EPICS.  These
*	ENV_PARAM's are initialized by $epics/share/bin/envSetupParams for
*	use by EPICS programs running under UNIX and VxWorks.
*
*	User programs can define their own environment parameters for their
*	own use--the only caveat is that such parameters aren't automatically
*	setup by EPICS.
*
* SEE ALSO
*	$epics/share/bin/envSetupParams, envSubr.c
*
*-***************************************************************************/

#ifndef envDefsH
#define envDefsH

#ifdef __cplusplus
extern "C" {
#endif

#include "shareLib.h"

typedef struct envParam {
    char	*name;		/* text name of the parameter */
    char	*pdflt;
} ENV_PARAM;

/*
 * bldEnvData looks for "epicsShareExtern READONLY ENV_PARAM"
 */
epicsShareExtern READONLY ENV_PARAM EPICS_CA_ADDR_LIST; 
epicsShareExtern READONLY ENV_PARAM EPICS_CA_CONN_TMO; 
epicsShareExtern READONLY ENV_PARAM EPICS_CA_AUTO_ADDR_LIST;
epicsShareExtern READONLY ENV_PARAM EPICS_CA_REPEATER_PORT;
epicsShareExtern READONLY ENV_PARAM EPICS_CA_SERVER_PORT;
epicsShareExtern READONLY ENV_PARAM EPICS_CA_MAX_ARRAY_BYTES;
epicsShareExtern READONLY ENV_PARAM EPICS_CA_MAX_SEARCH_PERIOD;
epicsShareExtern READONLY ENV_PARAM EPICS_CAS_INTF_ADDR_LIST;
epicsShareExtern READONLY ENV_PARAM EPICS_CAS_IGNORE_ADDR_LIST;
epicsShareExtern READONLY ENV_PARAM EPICS_CAS_AUTO_BEACON_ADDR_LIST; 
epicsShareExtern READONLY ENV_PARAM EPICS_CAS_BEACON_ADDR_LIST; 
epicsShareExtern READONLY ENV_PARAM EPICS_CAS_SERVER_PORT;
epicsShareExtern READONLY ENV_PARAM EPICS_CA_BEACON_PERIOD; /* deprecated */
epicsShareExtern READONLY ENV_PARAM EPICS_CAS_BEACON_PERIOD;
epicsShareExtern READONLY ENV_PARAM EPICS_CAS_BEACON_PORT;
epicsShareExtern READONLY ENV_PARAM EPICS_TIMEZONE;
epicsShareExtern READONLY ENV_PARAM EPICS_TS_NTP_INET;
epicsShareExtern READONLY ENV_PARAM EPICS_IOC_LOG_PORT;
epicsShareExtern READONLY ENV_PARAM EPICS_IOC_LOG_INET;
epicsShareExtern READONLY ENV_PARAM EPICS_IOC_LOG_FILE_LIMIT;
epicsShareExtern READONLY ENV_PARAM EPICS_IOC_LOG_FILE_NAME;
epicsShareExtern READONLY ENV_PARAM EPICS_IOC_LOG_FILE_COMMAND;
epicsShareExtern READONLY ENV_PARAM EPICS_CMD_PROTO_PORT;
epicsShareExtern READONLY ENV_PARAM EPICS_AR_PORT;
epicsShareExtern READONLY ENV_PARAM IOCSH_PS1;
epicsShareExtern READONLY ENV_PARAM IOCSH_HISTSIZE;

/*
 * bldEnvData looks for "epicsShareExtern ENV_PARAM" so
 * this always needs to be divided into two lines
 */
epicsShareExtern READONLY ENV_PARAM *env_param_list[];

struct in_addr;

epicsShareFunc char * epicsShareAPI 
	envGetConfigParam(const ENV_PARAM *pParam, int bufDim, char *pBuf);
epicsShareFunc const char * epicsShareAPI 
	envGetConfigParamPtr(const ENV_PARAM *pParam);
epicsShareFunc long epicsShareAPI 
	envPrtConfigParam(const ENV_PARAM *pParam);
epicsShareFunc long epicsShareAPI 
	envGetInetAddrConfigParam(const ENV_PARAM *pParam, struct in_addr *pAddr);
epicsShareFunc long epicsShareAPI 
	envGetDoubleConfigParam(const ENV_PARAM *pParam, double *pDouble);
epicsShareFunc long epicsShareAPI 
	envGetLongConfigParam(const ENV_PARAM *pParam, long *pLong);
epicsShareFunc unsigned short epicsShareAPI envGetInetPortConfigParam 
        (const ENV_PARAM *pEnv, unsigned short defaultPort);
epicsShareFunc long epicsShareAPI epicsPrtEnvParams(void);
epicsShareFunc void epicsShareAPI epicsEnvSet (const char *name, const char *value);
epicsShareFunc void epicsShareAPI epicsEnvShow (const char *name);

#ifdef __cplusplus
}
#endif

#endif /*envDefsH*/

