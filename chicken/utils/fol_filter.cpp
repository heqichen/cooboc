// FOL Filter  = First Order Lag Filter

#include "filter.h"
#include "fol_filter.h"





FolFilter::FolFilter(double q)
	:	Filter::Filter(),
		mQ(q)
{}


FolFilter::FolFilter(double q, double lastValue)
	:	Filter::Filter(),
		mQ(q)
{
	mLastValue = lastValue;
}


double FolFilter::filter(double newValue)
{
	mLastValue = (1-mQ)*mLastValue + mQ*newValue;
	return mLastValue;
}


FolFilter::~FolFilter()
{
	
}
