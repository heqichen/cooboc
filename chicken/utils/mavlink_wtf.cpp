#include "mavlink_wtf.h"
//add this file because mavlink only can attach four communication devices at once



uint8_t mavComChan = 0;

uint8_t getMavlinkComChannel()
{
	++mavComChan;
	switch (mavComChan)
	{
		case (1):
		{
			return MAVLINK_COMM_0;
		}
		case (2):
		{
			return MAVLINK_COMM_1;
		}
		case (3):
		{
			return MAVLINK_COMM_2;
		}
		case (4):
		{
			return MAVLINK_COMM_3;
		}
		default:
		{
			//WTF
			return MAVLINK_COMM_0;
		}
	}
}