/*************************************************************************\
* Copyright (c) 2006 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/

/* epicsUnitTest.h,v 1.1.2.2 2006/11/07 15:35:14 anj Exp
 * Author: Andrew Johnson
 */

#include <stdarg.h>

#include "compilerDependencies.h"
#include "shareLib.h"

#ifdef __cplusplus
extern "C" {
#endif

epicsShareFunc void testPlan(int tests);
epicsShareFunc int  testOkV(int pass, const char *fmt, va_list pvar);
epicsShareFunc int  testOk(int pass, const char *fmt, ...)
						EPICS_PRINTF_STYLE(2, 3);
epicsShareFunc void testPass(const char *fmt, ...)
						EPICS_PRINTF_STYLE(1, 2);
epicsShareFunc void testFail(const char *fmt, ...)
						EPICS_PRINTF_STYLE(1, 2);
epicsShareFunc void testSkip(int skip, const char *why);
epicsShareFunc void testTodoBegin(const char *why);
epicsShareFunc void testTodoEnd();
epicsShareFunc int  testDiag(const char *fmt, ...)
						EPICS_PRINTF_STYLE(1, 2);
epicsShareFunc void testAbort(const char *fmt, ...)
						EPICS_PRINTF_STYLE(1, 2);
epicsShareFunc int  testDone(void);

#define testOk1(cond) testOk(cond, "%s", #cond)

#ifdef __cplusplus
}
#endif
