/*************************************************************************\
* Copyright (c) 2002 The University of Saskatchewan
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/*
 * RTEMS configuration for EPICS
 *  rtems_config.c,v 1.2.2.12 2006/09/19 15:46:13 lange Exp
 *      Author: W. Eric Norum
 *              norume@aps.anl.gov
 *              (630) 252-4793
 */

#include <rtems.h>

/*
 ***********************************************************************
 *                         RTEMS CONFIGURATION                         *
 ***********************************************************************
 */
/* #define STACK_CHECKER_ON                1 */

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXECUTIVE_RAM_SIZE        (2000*1024)
#define CONFIGURE_MAXIMUM_TASKS             rtems_resource_unlimited(30)
#define CONFIGURE_MAXIMUM_SEMAPHORES        rtems_resource_unlimited(500)
#define CONFIGURE_MAXIMUM_TIMERS            rtems_resource_unlimited(20)
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES    rtems_resource_unlimited(5)
#define CONFIGURE_MAXIMUM_USER_EXTENSIONS   1

#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 100
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_MAXIMUM_DRIVERS       8

#define CONFIGURE_MICROSECONDS_PER_TICK 20000

#define CONFIGURE_INIT_TASK_PRIORITY    80

#define CONFIGURE_INIT
#define CONFIGURE_INIT_TASK_INITIAL_MODES (RTEMS_PREEMPT | \
                    RTEMS_NO_TIMESLICE | \
                    RTEMS_NO_ASR | \
                    RTEMS_INTERRUPT_LEVEL(0))
#define CONFIGURE_INIT_TASK_ATTRIBUTES (RTEMS_FLOATING_POINT | RTEMS_LOCAL)
#define CONFIGURE_INIT_TASK_STACK_SIZE  (16*1024)
rtems_task Init (rtems_task_argument argument);

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#warning "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
#warning "needs review"
#warning "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
/* This should be made BSP dependent, not CPU dependent !! */
#if !defined(__mc68040__) && !defined(__mcf5200__) && !defined(__arm__)  /* don't have RTC code */
#define CONFIGURE_APPLICATION_NEEDS_RTC_DRIVER
#endif

#include <rtems/confdefs.h>
