#ifndef __AVG_FILTER_H__
#define __AVG_FILTER_H__

#include "filter.h"

#ifndef NULL
#define NULL 0
#endif

class AvgFilter : public Filter
{
	public:
		AvgFilter(int length, double lastValue);
		virtual ~AvgFilter();
		virtual double filter(double value);
		void resetValue(double value);
		inline virtual double getLastValue(void) {return mLastValue;}
	private:
		double *mValueList;
		int mValueLength;
		int mCurrentHeadIndex;
};

#endif