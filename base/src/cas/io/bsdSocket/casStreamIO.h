
/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
//
// casStreamIO.h,v 1.2.2.1 2004/09/22 17:55:00 jhill Exp
//

#ifndef casStreamIOh
#define casStreamIOh

#include "casStrmClient.h"

struct ioArgsToNewStreamIO {
	caNetAddr addr;
	SOCKET sock;
};

class casStreamIO : public casStrmClient {
public:
	casStreamIO ( caServerI &, clientBufMemoryManager &, 
        const ioArgsToNewStreamIO & );
	~casStreamIO ();
	int getFD () const;
	void xSetNonBlocking ();
    const caNetAddr getAddr() const;
	void hostName ( char *pBuf, unsigned bufSize ) const;

private:
	SOCKET sock;
	struct sockaddr_in addr;
	xBlockingStatus blockingFlag;

    bool sockHasBeenShutdown;
	xBlockingStatus blockingState() const;
	bufSizeT incomingBytesPresent() const;
	static bufSizeT optimumBufferSize ();
	void osdShow ( unsigned level ) const;
	outBufClient::flushCondition osdSend ( const char *pBuf, bufSizeT nBytesReq, 
		bufSizeT & nBytesActual );
	inBufClient::fillCondition osdRecv ( char *pBuf, bufSizeT nBytesReq, 
		bufSizeT & nBytesActual );
    void forceDisconnect ();
	casStreamIO ( const casStreamIO & );
	casStreamIO & operator = ( const casStreamIO & );
};

inline const caNetAddr casStreamIO::getAddr() const
{
	return caNetAddr ( this->addr );
}

#endif // casStreamIOh
