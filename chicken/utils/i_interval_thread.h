#ifndef __I_THREAD_H__
#define __I_THREAD_H__

#include <pthread.h>

class IIntervalThread
{
	public:
		virtual ~IIntervalThread();
		virtual void work(void) = 0;

		void start();
		IIntervalThread();
		void setInterval(unsigned long interval) {mInterval = interval;}
		unsigned long getInterval() const {return mInterval;}
		bool isRunning() const {return mIsRunning;}
		
	private:
		unsigned long mInterval;
		bool mIsRunning;
		pthread_t mPThread;
};

#endif

