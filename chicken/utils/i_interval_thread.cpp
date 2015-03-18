#include "i_interval_thread.h"
#include "util.h"

#include <unistd.h>

void *intervalThreadStart(void *iiThreadPtr)
{
	IIntervalThread *iThread = (IIntervalThread*)iiThreadPtr;

	while (iThread->isRunning())
	{
		iThread->work();
		usleep(1000UL);
		unsigned long interval = iThread->getInterval();
		while (true)
		{
			if ((millis() % interval) == 0)
			{
				break;
			}
			usleep(300UL);
		}
	}

}


IIntervalThread::IIntervalThread()
	:	mInterval	(1000UL),
		mIsRunning	(false)

{

}

IIntervalThread::~IIntervalThread()
{
	mIsRunning = false;
	pthread_join(mPThread, NULL);
}

void IIntervalThread::start()
{
	if (!mIsRunning)
	{
		mIsRunning = true;
		pthread_create(&mPThread, NULL, &intervalThreadStart, this);
	}
}