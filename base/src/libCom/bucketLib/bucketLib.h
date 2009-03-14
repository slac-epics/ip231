/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/*
 *	bucketLib.h,v 1.10 2002/07/12 21:34:18 jba Exp
 *
 *      Author: Jeffrey O. Hill
 *              hill@luke.lanl.gov
 *              (505) 665 1831
 *      Date:  9-93 
 *
 *	NOTES:
 *	.01 Storage for identifier must persist until an item is deleted
 */

#ifndef INCbucketLibh
#define INCbucketLibh

#ifdef __cplusplus
extern "C" {
#endif

#include "errMdef.h"
#include "epicsTypes.h"
#include "shareLib.h"

typedef	unsigned 	BUCKETID;

typedef enum {bidtUnsigned, bidtPointer, bidtString} buckTypeOfId;

typedef struct item{
	struct item	*pItem;
	READONLY void	*pId;
	READONLY void   *pApp;
	buckTypeOfId	type;
}ITEM;

typedef struct bucket{
	ITEM		**pTable;
	void		*freeListPVT;
	unsigned	hashIdMask;
	unsigned	hashIdNBits;
        unsigned        nInUse;
}BUCKET;

epicsShareFunc BUCKET * epicsShareAPI bucketCreate (unsigned nHashTableEntries);
epicsShareFunc int epicsShareAPI bucketFree (BUCKET *prb);
epicsShareFunc int epicsShareAPI bucketShow (BUCKET *pb);

/*
 * !! Identifier must exist (and remain constant) at the specified address until
 * the item is deleted from the bucket !!
 */
epicsShareFunc int epicsShareAPI bucketAddItemUnsignedId (BUCKET *prb, 
		READONLY unsigned *pId, READONLY void *pApp);
epicsShareFunc int epicsShareAPI bucketAddItemPointerId (BUCKET *prb, 
		void * READONLY *pId, READONLY void *pApp);
epicsShareFunc int epicsShareAPI bucketAddItemStringId (BUCKET *prb, 
		READONLY char *pId, READONLY void *pApp);

epicsShareFunc int epicsShareAPI bucketRemoveItemUnsignedId (BUCKET *prb, READONLY unsigned *pId);
epicsShareFunc int epicsShareAPI bucketRemoveItemPointerId (BUCKET *prb, void * READONLY *pId);
epicsShareFunc int epicsShareAPI bucketRemoveItemStringId (BUCKET *prb, READONLY char *pId);

epicsShareFunc void * epicsShareAPI bucketLookupItemUnsignedId (BUCKET *prb, READONLY unsigned *pId);
epicsShareFunc void * epicsShareAPI bucketLookupItemPointerId (BUCKET *prb, void * READONLY *pId);
epicsShareFunc void * epicsShareAPI bucketLookupItemStringId (BUCKET *prb, READONLY char *pId);

epicsShareFunc void * epicsShareAPI bucketLookupAndRemoveItemUnsignedId (BUCKET *prb, READONLY unsigned *pId);
epicsShareFunc void * epicsShareAPI bucketLookupAndRemoveItemPointerId (BUCKET *prb, void * READONLY *pId);
epicsShareFunc void * epicsShareAPI bucketLookupAndRemoveItemStringId (BUCKET *prb, READONLY char *pId);


/*
 * Status returned by bucketLib functions
 */
#define BUCKET_SUCCESS		S_bucket_success
#define S_bucket_success	0
#define S_bucket_noMemory	(M_bucket | 1) 	/*Memory allocation failed*/
#define S_bucket_idInUse	(M_bucket | 2) 	/*Identifier already in use*/
#define S_bucket_uknId		(M_bucket | 3) 	/*Unknown identifier*/

#ifdef __cplusplus
}
#endif

#endif /*INCbucketLibh*/

