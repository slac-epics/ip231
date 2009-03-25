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
 *      casEventMask.cc,v 1.23.2.1 2003/09/23 16:52:59 jhill Exp
 *
 *      Author  Jeffrey O. Hill
 *              johill@lanl.gov
 *              505 665 1831
 */

#include <stdio.h>
#include <limits.h>

#define epicsExportSharedSymbols
#include "casdef.h"
#include "casEventRegistry.h"

#ifdef TEST
main ()
{
	casEventRegistry 	reg;
	casEventMask		bill1 (reg, "bill");
	casEventMask		bill2 (reg, "bill");
	casEventMask		bill3 (reg, "bill");
	casEventMask		art1 (reg, "art");
	casEventMask		art2 (reg, "art");
	casEventMask		jane (reg, "jane");
	casEventMask		artBill;
	casEventMask		tmp;

	bill1.show(10u);
	reg.show(10u);
	bill2.show(10u);
	reg.show(10u);
	bill3.show(10u);
	reg.show(10u);
	jane.show(10u);
	reg.show(10u);
	art1.show(10u);
	reg.show(10u);
	art2.show(10u);
	reg.show(10u);

	assert (bill1 == bill2);
	assert (bill1 == bill3);
	assert (jane != bill1);
	assert (jane != art1);
	assert (bill1 != art1);
	assert (art1 == art2);

	artBill = art1 | bill1;
	tmp = artBill & art1;
	assert (tmp.eventsSelected());
	tmp = artBill & bill1;
	assert (tmp.eventsSelected());
	tmp = artBill&jane;
	assert (tmp.noEventsSelected());
}
#endif

casEventMask casEventRegistry::maskAllocator ()
{
	casEventMask    evMask;
 
	if ( this->maskBitAllocator < CHAR_BIT * sizeof ( evMask.mask ) ) {
		evMask.mask = 1u << ( this->maskBitAllocator++ );
	}
	return evMask;
}

casEventMask casEventRegistry::registerEvent ( const char *pName )
{
	//
	// NOTE: pName outlives id here
	// (so the refString option is ok)
	//
	stringId id ( pName, stringId::refString );
	casEventMaskEntry * pEntry;
	casEventMask mask;

	pEntry = this->lookup ( id );
	if (pEntry) {
		mask = *pEntry;
	}
	else {
		mask = this->maskAllocator ();
		if ( mask.mask == 0u ) {
			errMessage ( S_cas_tooManyEvents, "casEventRegistry::registerEvent" );
		}
		else {
			pEntry = new casEventMaskEntry ( *this, mask, pName );
			mask = *pEntry;
		}
	}
	return mask;
}

void casEventMask::show ( unsigned level ) const
{
	if ( level > 0u ) {
		printf ( "casEventMask = %x\n", this->mask );
	}
}

casEventMask::casEventMask ( casEventRegistry & reg, const char * pName )
{
        *this = reg.registerEvent ( pName );
}

void casEventRegistry::show ( unsigned level ) const
{
	if ( level > 1u ) {
		printf ("casEventRegistry: bit allocator = %d\n", 
				this->maskBitAllocator);
	}
	this->resTable < casEventMaskEntry, stringId >::show ( level );
}

casEventMaskEntry::casEventMaskEntry (
	casEventRegistry & regIn, casEventMask maskIn, const char * pName ) :
	casEventMask ( maskIn ), stringId ( pName ), reg ( regIn )
{
	int 	stat;

	assert ( this->resourceName() != NULL );
	stat = this->reg.add ( *this );
	assert ( stat == 0 );
}

casEventMaskEntry::~casEventMaskEntry()
{
        this->reg.remove ( *this );
}

void casEventMaskEntry::destroy ()
{
	delete this;
}

void casEventMaskEntry::show ( unsigned level ) const 
{
	this->casEventMask::show ( level );
	this->stringId::show ( level );
}

