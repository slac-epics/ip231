/*************************************************************************\
* Copyright (c) 2006 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/

/*
 * Dummy version -- use if application does not provide its own version
 *  epicsRtemsInitHookPost.c,v 1.1.2.1 2006/06/29 15:22:49 norume Exp
 */
#include "epicsRtemsInitHooks.h"

int
epicsRtemsInitPostSetBootConfigFromNVRAM(struct rtems_bsdnet_config *config)
{
    return 0;
}
