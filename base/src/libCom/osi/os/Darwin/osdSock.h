/*************************************************************************\
* Copyright (c) 2002 The University of Saskatchewan
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/*
 * osdSock.h,v 1.6.2.2 2005/06/28 20:33:41 norume Exp
 *
 * Author: Eric Norum
 */

#ifndef osdSockH
#define osdSockH

#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>

#include <sys/types.h>
#include <sys/param.h> /* for MAXHOSTNAMELEN */
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
/*#include <sys/filio.h>
#include <sys/sockio.h>*/
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <unistd.h> /* close() and others */


#ifdef __cplusplus
}
#endif
 
typedef int                     SOCKET;
#define INVALID_SOCKET          (-1)
#define SOCKERRNO               errno
#define socket_ioctl(A,B,C)     ioctl(A,B,C)
typedef int osiSockIoctl_t;
typedef socklen_t osiSocklen_t;

#define FD_IN_FDSET(FD) ((FD)<FD_SETSIZE)

#define SOCK_EWOULDBLOCK EWOULDBLOCK
#define SOCK_ENOBUFS ENOBUFS
#define SOCK_ECONNRESET ECONNRESET
#define SOCK_ETIMEDOUT ETIMEDOUT
#define SOCK_EADDRINUSE EADDRINUSE
#define SOCK_ECONNREFUSED ECONNREFUSED
#define SOCK_ECONNABORTED ECONNABORTED
#define SOCK_EINPROGRESS EINPROGRESS
#define SOCK_EISCONN EISCONN
#define SOCK_EALREADY EALREADY
#define SOCK_EINVAL EINVAL
#define SOCK_EINTR EINTR
#define SOCK_EPIPE EPIPE
#define SOCK_EMFILE EMFILE
#define SOCK_SHUTDOWN ESHUTDOWN
#define SOCK_ENOTSOCK ENOTSOCK
#define SOCK_EBADF EBADF

#ifndef SHUT_RD
#define SHUT_RD 0
#endif

#ifndef SHUT_WR
#define SHUT_WR 1
#endif

#ifndef SHUT_RDWR
#define SHUT_RDWR 2
#endif

#define ifreq_size(pifreq) ((pifreq)->ifr_addr.sa_len + sizeof((pifreq)->ifr_name))

#endif /*osdSockH*/
