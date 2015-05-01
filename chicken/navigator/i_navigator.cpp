#include "i_navigator.h"


INavigator::INavigator()
	:	mIsEnabled	(false)
{

}

INavigator::~INavigator()
{

}

void INavigator::work()
{
	if (!mIsEnabled)
	{
		return ;
	}
	navigate();
}

void INavigator::setEnabled(bool isEnabled)
{
	if (isEnabled != mIsEnabled)
	{
		mIsEnabled = isEnabled;
		if (isEnabled)
		{
			onTakeover();
		}
		else
		{
			onRelease();
		}
	}
}