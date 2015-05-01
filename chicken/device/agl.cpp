#include "agl.h"

#include "device_types.h"

Agl::Agl(Io *io)
	:	IDevice		(io, DEVICE_TYPE_AGL),
		mAds7828	(NULL)
{

}

Agl::~Agl()
{
	if (mAds7828 != NULL)
	{
		delete mAds7828;
	}
}

void Agl::init()
{
	mAds7828 = new Ads7828Driver(mIo , 0x48);
	mAds7828->useExternalRef();
}

double Agl::measureAgl()
{
	uint16_t ad = mAds7828->readSingleChannel(2);
	return ad;
}

