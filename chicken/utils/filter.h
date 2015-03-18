#ifndef __FILTER_H__
#define __FILTER_H__



class Filter
{
	public:
		Filter();
		Filter(double lastValue);
		virtual ~Filter();
		virtual double filter(double);
		inline virtual double getLastValue(void);
	protected:
		double mLastValue;
};


#endif
