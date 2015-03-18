#include "imu.h"

#include "driver/io/io.h"
#include "device_types.h"
#include <utils/util.h>

#include <iostream>
#include <cmath>
#include <unistd.h>
using namespace std;

#define UPDATE_INTERVAL	20	//20millis per time

void *imuDataUpdatingThread(void *imuPtr)
{
	Imu *imu = (Imu*)imuPtr;

	while (imu->isImuRunning())
	{
		imu->update();
		usleep(1000UL);
		while (true)
		{
			if (millis() % UPDATE_INTERVAL == 0 )
			{
				break;
			}
			usleep(300UL);
		}
	}
}

#define GYRO_OFFSET_X	7.832
#define GYRO_SCALE_X	-1.102313212*1e-5
#define GYRO_OFFSET_Y	0
#define GYRO_SCALE_Y	1.102313212*1e-5
#define GYRO_OFFSET_Z	-4.7933333
#define GYRO_SCALE_Z	-1.064946662*1e-5


Imu::Imu(Io *io)
	:	IDevice	(io, DEVICE_TYPE_IMU),
		mIsImuRunning	(false),
		m_rawAccX	(0),
		m_rawAccY	(0),
		m_rawAccZ	(1.0),
		
		m_rawGyroX	(0),
		m_rawGyroY	(0),
		m_rawGyroZ	(0),
		
		
		m_accX	(0.0),
		m_accY	(0.0),
		m_accZ	(1.0),
		
		m_turnX	(0.0),
		m_turnY	(0.0),
		m_turnZ	(0.0),
		
		m_estMagnetX	(1.0),
		m_estMagnetY	(0.0),
		m_estMagnetZ	(0.0),
		
		m_estX	(0.0),
		m_estY	(0.0),
		m_estZ	(1.0),
		
		m_pitch	(0.0),
		m_roll	(0.0),
		m_heading	(0.0)
{
	mBmx055 = new Bmx055Driver(mIo);

	axFilter = new FolFilter(0.15);
	ayFilter = new FolFilter(0.15);
	azFilter = new FolFilter(0.15);
}

Imu::~Imu()
{
	if (mIsImuRunning)
	{
		mIsImuRunning = false;
	}
	pthread_join(mDataUpdatePThread, NULL);
	delete mBmx055;
	delete axFilter;
	delete ayFilter;
	delete azFilter;

	cout<<"imu released"<<endl;
}

void Imu::init()
{
	mIsImuRunning = true;
	pthread_create(&mDataUpdatePThread, NULL, &imuDataUpdatingThread, this);
}

void Imu::update()
{
	readData();
	fixData();
	fusionAttitude();
	fusionMagnet();
}

ImuAttitude Imu::getAttitude()
{
	ImuAttitude attitude;
	attitude.pitch = m_pitch;
	attitude.roll = m_roll;
	attitude.heading = m_heading;

	return attitude;
}


void Imu::readData(void)
{
	RawAccValue rawAccValue;
	rawAccValue.x = m_rawAccX;
	rawAccValue.y = m_rawAccY;
	rawAccValue.z = m_rawAccZ;

	RawGyroValue rawGyroValue;
	rawGyroValue.x = m_rawGyroX;
	rawGyroValue.y = m_rawGyroY;
	rawGyroValue.z = m_rawGyroZ;

	mBmx055->readAcc(rawAccValue);
	mBmx055->readGyro(rawGyroValue);
	mBmx055->readMag(mRawMagValue);

	m_rawAccX = rawAccValue.x;
	m_rawAccY = rawAccValue.y;
	m_rawAccZ = rawAccValue.z;

	m_rawGyroX = rawGyroValue.x;
	m_rawGyroY = rawGyroValue.y;
	m_rawGyroZ = rawGyroValue.z;
	
}

void Imu::fixData(void)
{
	//TODO
	// need to calibate 128.0
	m_accX = axFilter->filter(m_rawAccX / 128.0);
	m_accY =  - ayFilter->filter(m_rawAccY / 128.0);
	m_accZ = azFilter->filter(m_rawAccZ / 128.0);
	
	m_turnX = (m_rawGyroX + GYRO_OFFSET_X) * GYRO_SCALE_X;
	m_turnY = (m_rawGyroY + GYRO_OFFSET_Y) * GYRO_SCALE_Y;
	m_turnZ = (m_rawGyroZ + GYRO_OFFSET_Z) * GYRO_SCALE_Z;
}

void Imu::fusionMagnet(void)
{
	double r;
	double gyroX, gyroY, gyroZ;
	iterateGyroMagnet(&gyroX, &gyroY, &gyroZ);
	
	r = sqrt(gyroX*gyroX + gyroY*gyroY + gyroZ*gyroZ);
	
	m_estMagnetX = gyroX / r;
	m_estMagnetY = gyroY / r;
	m_estMagnetZ = gyroZ / r;
	
	//setp1. project head on earth
	Point3D unitX(1.0, 0.0, 0.0);
	Point3D earthPlane(m_estX, m_estY, m_estZ);
	Point3D xOnEarth = unitX.projectOnPlane(earthPlane);
	
	//step2. project magnet on earth
	Point3D magnet(m_estMagnetX, m_estMagnetY, m_estMagnetZ);
	Point3D magnetOnEarth = magnet.projectOnPlane(earthPlane);
	
	//setp3. calculate rotation diration
	Point3D cross = magnetOnEarth.crossProduct(xOnEarth);
	double inner = cross.dotProduct(earthPlane);
	double isDirPostive = true;
	if (inner < 0)
	{
		isDirPostive = false;
	}
	
	//step4. calculate cos(heading)
	double cosHeading = xOnEarth.dotProduct(magnetOnEarth) / (xOnEarth.absolute() * magnetOnEarth.absolute());
	double heading = acos(cosHeading);
	
	//step5. calculate the result
	if (!isDirPostive)
	{
		heading = 2*PI - heading;
	}
	m_heading = heading;
}

inline void Imu::iterateGyroMagnet(double *gyroEstX, double *gyroEstY, double *gyroEstZ)
{
	double diffX, diffY, diffZ;
	diffX = 0.0;
	diffY = 0.0;
	diffZ = 0.0;
	
	double diffRight, diffTop;
	//1. calculate gyro x
	calculateUniaxialGyroEffect(m_estMagnetX, m_estMagnetY, m_estMagnetZ, m_turnX, &diffRight, &diffTop);
	diffY += diffRight;
	diffZ += diffTop;
	
	//2. calculate gyro y
	calculateUniaxialGyroEffect(m_estMagnetY, m_estMagnetZ, m_estMagnetX, m_turnY, &diffRight, &diffTop);
	diffZ += diffRight;
	diffX += diffTop;
	
	//3. calculate gyro z
	calculateUniaxialGyroEffect(m_estMagnetZ, m_estMagnetX, m_estMagnetY, m_turnZ, &diffRight, &diffTop);
	diffX += diffRight;
	diffY += diffTop;
	
	*gyroEstX = m_estMagnetX + diffX;
	*gyroEstY = m_estMagnetY + diffY;
	*gyroEstZ = m_estMagnetZ + diffZ;
}

void Imu::fusionAttitude(void)
{
	double r;
	
	//================================================
	//SETP 1. normalize acc value for "ACC. Attitude"
	//================================================
	double nax, nay, naz;
	r = sqrt(m_accX*m_accX + m_accY*m_accY + m_accZ*m_accZ);
	nax = m_accX / r;
	nay = m_accY / r;
	naz = m_accZ / r;
	
	//================================================
	//SETP 2. calaulate "Gyro Attitude"
	//================================================
	double gex, gey, gez;
	iterateGyroAttitude(&gex, &gey, &gez);
	r = sqrt(gex*gex + gey*gey + gez*gez);
	gex /= r;
	gey /= r;
	gez /= r;
	
	//================================================
	//SETP 3. culcate confidence factor for acc meter
	//================================================
	double confidence = m_accX * m_accX + m_accY * m_accY + m_accZ * m_accZ;
	
	if (confidence > 1)
	{
		confidence = 1 / confidence;
	}
	confidence = confidence*0.08 + 0.002;
	
	
	//================================================
	//SETP 4. Fusion "ACC. Attitude" and "Gyro Attitude"
	//================================================
	
	/*
	m_estX = nax;
	m_estY = nay;
	m_estZ = naz;
	*/
	
	m_estX = nax*confidence + gex*(1-confidence);
	m_estY = nay*confidence + gey*(1-confidence);
	m_estZ = naz*confidence + gez*(1-confidence);
	
	Point3D earthPlane(m_estX, m_estY, m_estZ);

	//calculate pitch
	Point3D unitX(1, 0, 0);
	Point3D xOnEarth = unitX.projectOnPlane(earthPlane);
	
	m_pitch = acos(unitX.dotProduct(xOnEarth) / (unitX.absolute() * xOnEarth.absolute()));
	
	if (m_estX > 0)
	{
		m_pitch = -m_pitch;
	}
	
	//calculate roll
	Point3D unitY(0, 1, 0);
	Point3D yOnEarth = unitY.projectOnPlane(earthPlane);
	m_roll = acos(unitY.dotProduct(yOnEarth) / (unitY.absolute() * yOnEarth.absolute()));
	
	if (m_estZ < 0)
	{
		m_roll = PI - m_roll;
	}
	
	if (m_estY < 0)
	{
		m_roll = -m_roll;
	}
	
}



//calculate gyro attitude
inline void Imu::iterateGyroAttitude(double *gyroEstX, double *gyroEstY, double *gyroEstZ)
{
	double diffX, diffY, diffZ;
	diffX = 0.0;
	diffY = 0.0;
	diffZ = 0.0;
	
	double diffRight, diffTop;
	//1. calculate gyro x
	calculateUniaxialGyroEffect(m_estX, m_estY, m_estZ, m_turnX, &diffRight, &diffTop);
	diffY += diffRight;
	diffZ += diffTop;
	
	//2. calculate gyro y
	calculateUniaxialGyroEffect(m_estY, m_estZ, m_estX, m_turnY, &diffRight, &diffTop);
	diffZ += diffRight;
	diffX += diffTop;
	
	//3. calculate gyro z
	calculateUniaxialGyroEffect(m_estZ, m_estX, m_estY, m_turnZ, &diffRight, &diffTop);
	diffX += diffRight;
	diffY += diffTop;
	
	*gyroEstX = m_estX + diffX;
	*gyroEstY = m_estY + diffY;
	*gyroEstZ = m_estZ + diffZ;
}

inline void Imu::calculateUniaxialGyroEffect(double lastEstFront, double lastEstRight, double lastEstTop, double gyroRate, double *diffRight, double *diffTop)
{
	//==========================================================================
	// X is front
	// Y is right
	// Z is top
	//==========================================================================
	double a_zy;
	double r_zy_z;

	double estX = lastEstFront;
	double estY = lastEstRight;
	double estZ = lastEstTop;
	
	a_zy = sqrt(estY*estY + estZ*estZ);
	
	if (fabs(a_zy) < EPSILON)
	{
		a_zy = EPSILON;
	}
	
	if (fabs(estY) < fabs(estZ))
	{
		if (estZ > 0)
		{
			r_zy_z = asin(estY / a_zy);
		}
		else
		{
			r_zy_z = PI - asin(estY / a_zy);
		}
		if (r_zy_z < 0)
		{
			r_zy_z += 2*PI;
		}
	}
	else
	{
			if (estY < 0)
		{
			r_zy_z = 2*PI - acos(estZ / a_zy);
		}
		else
		{
			r_zy_z = acos(estZ / a_zy);
		}
	}
	
	double eg_y_x;	//gyro x affect on acc y
	double eg_z_x;	//gyro x affect on acc z
	
	eg_y_x = sin(r_zy_z + gyroRate) * a_zy - estY;
	eg_z_x = cos(r_zy_z + gyroRate) * a_zy - estZ;
	
	
	*diffRight = eg_y_x;
	*diffTop = eg_z_x;
}


