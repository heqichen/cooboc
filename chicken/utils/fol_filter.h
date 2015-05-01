#ifndef __FOL_FILTER_H__
#define __FOL_FILTER_H__


// FOL Filter  = First Order Lag Filter

#include "filter.h"


/**
template function must defined in the same file...
new compiler can define functino in other file using keyword "export"
**/


class FolFilter : public Filter
{
	public:
		FolFilter(double q);
		FolFilter(double q, double lastValue);
		virtual ~FolFilter();
		virtual double filter(double);
		inline virtual double getLastValue(void) {return mLastValue;}
	private:
		double mQ;
};




#endif
