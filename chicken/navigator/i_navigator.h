#ifndef __I_NAVIGATOR_H__
#define __I_NAVIGATOR_H__

#include <utils/i_interval_thread.h>

class INavigator	:	public IIntervalThread
{
	public:
		INavigator();
		virtual ~INavigator();
		virtual void work();
		virtual void setEnabled(bool isEnabled);
		virtual void navigate()=0;
		virtual void onTakeover()=0;
		virtual void onRelease()=0;
	private:
		bool mIsEnabled;

};


#endif

