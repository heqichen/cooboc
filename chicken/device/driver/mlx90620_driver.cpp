#include "mlx90620_driver.h"

#include <iostream>
using namespace std;

Mlx90620Driver::Mlx90620Driver(Io *io, char *serialPath)
	:	mSerialHandler	(NULL)
{
	mSerialHandler = io->getSerialHandler(serialPath);
}