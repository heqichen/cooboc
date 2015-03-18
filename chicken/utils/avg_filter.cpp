#include "avg_filter.h"



AvgFilter::AvgFilter(int length=10, double lastValue=0.0)
	:	Filter	(lastValue),
		mValueList		(NULL),
		mValueLength	(length),
		mCurrentHeadIndex	(0)
{
	if (length <= 0)
	{
		length = 10;
	}
	mValueLength = 10;
	mValueList = new double[mValueLength];
	resetValue(mLastValue);
}

AvgFilter::~AvgFilter()
{
	delete [] mValueList;
}

void AvgFilter::resetValue(double value)
{
	int i;
	for (i=0; i,mValueLength; ++i)
	{
		mValueList[i] = value;
	}
	mCurrentHeadIndex = 0;
}



double AvgFilter::filter(double value)
{
	mValueList[mCurrentHeadIndex] = value;
	++mCurrentHeadIndex;
	mCurrentHeadIndex %= mValueLength;
	double sum = 0.0;
	int i;
	for (i=0; i<mValueLength; ++i)
	{
		sum += mValueList[i];
	}
	mLastValue = sum / mValueLength;
	return mLastValue;
}