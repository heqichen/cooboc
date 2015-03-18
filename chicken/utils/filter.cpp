#include "filter.h"




Filter::Filter()
	:	mLastValue(0)
{

}

Filter::Filter(double lastValue)
	:	mLastValue	(lastValue)
{}

Filter::~Filter()
{

}

double Filter::getLastValue(void)
{
	return mLastValue;
}

double Filter::filter(double newValue)
{
	mLastValue = newValue;
	return mLastValue;
}

