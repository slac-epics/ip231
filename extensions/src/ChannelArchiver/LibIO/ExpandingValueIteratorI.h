#ifndef __EXPANDINGVALUEITERATOR_H__
#define __EXPANDINGVALUEITERATOR_H__

#include "ValueIteratorI.h"

//////////////////////////////////////////////////////////////////////
//CLASS ExpandingValueIteratorI
// A CLASS ValueIteratorI that expands "repeated" values
// into multiple values.
// (Repeated values have a severity of ARCH_REPEAT,
// the status is then interpreted as a repetition count).
//
// Clients using this filter will never see repeat counts
// but always the expanded values. 
//
// <H3>Example Usage</H3>
// The expanding iterator requires a "base" iterator for
// the archive type that you work on:
//	<BLOCKQUOTE><PRE>
//	ValueIterator   base(archive);
//	channel->getValueAfterTime(start, base );
//	</PRE></BLOCKQUOTE>
//
// When creating an ExpandingValueIteratorI for that base iterator,
// it's usually convenient to access it via a CLASS ValueIterator smart wrapper
// so that it behaves like an ordinary iterator:
//	<BLOCKQUOTE><PRE>
//	ValueIterator value (new ExpandingValueIteratorI(base));
//	while (value)
//	{
//		cout << *value << "\n";
//		++value;
//	}
//	</PRE></BLOCKQUOTE>
//
// <U>TO DO:</U><BR>
// Huge repeat counts are expanded. This takes forever
// and the repeats may not be useful.

// TODO: see above
class ExpandingValueIteratorI : public ValueIteratorI
{
public:
	//* Create an ExpandingValueIteratorI based
	// on another CLASS ValueIterator.
	//
	// The base iterator has to exist for the lifetime
	// of the ExpandingValueIterator.
	ExpandingValueIteratorI(ValueIterator &base);

	ExpandingValueIteratorI(ValueIteratorI *base);
	void attach(ValueIteratorI *base);
	~ExpandingValueIteratorI();

	//* Implemented of ValueIteratorI:
	bool isValid() const;
	const ValueI * getValue() const;
	bool next();
	bool prev();
	size_t determineChunk(const epicsTime &until);
	double getPeriod() const;     

	//* Is current value generated by expanding repeated values?
	bool isExpanded() const;

private:
	ValueIteratorI *_base;

    CtrlInfo  *_info;
    ValueI    *_repeat_value;
    epicsTime  _until;
};

inline bool ExpandingValueIteratorI::isExpanded() const
{	return _repeat_value != 0; }

#endif //__EXPANDINGVALUEITERATOR_H__

