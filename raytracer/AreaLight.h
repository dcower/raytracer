#ifndef AreaLight_h__
#define AreaLight_h__

#include "Light.h"


/* Represents a parallelogram (suggested: rectangular) light.
 * 
 * It is defined by two (normalized) axes and their lengths. i.e., if axis1
 * is (1,0,0) and axis2 is (0,0,1), and axis1L is 4 and axis2L is 8, then
 * the light will extend between -2 and 2 from its position along the X axis,
 * and -4 and 4 from its position along the Z axis.
 * */
class AreaLight : public Light
{
public:

	AreaLight();

	Vector getPoint();
	double AreaLight::getContribution (Scene &scene, const Vector point);

	double axis1L, axis2L;
	Vector axis1, axis2;
	unsigned int axis1Samples, axis2Samples;

	friend std::istream& operator >> (std::istream &in, AreaLight &light);
};

#endif // AreaLight_h__
