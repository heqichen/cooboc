#ifndef __UTIL_H__
#define __UTIL_H__

#ifndef NULL
#define NULL 0
#endif

#define EPSILON 1e-10
#define PI 3.14159265358979323846264332795
#define RAD_TO_DEG 180/PI


class Point3D
{
	public:
		Point3D(double x, double y, double z);
		Point3D projectOnPlane(Point3D plane);
		double dotProduct(Point3D point);	//inner product
		Point3D crossProduct(Point3D point);	//vector product
		double absolute();
		double x, y, z;

		
};

unsigned long millis();
int constraint(int x, int minx, int maxx);
double constraint(double x, double minx, double maxxI);
double cycleCompare(double target, double current, double cycleSize);


#endif
