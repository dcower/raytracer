#ifndef PointLight_h__
#define PointLight_h__

#include "Light.h"


/* Represents an infinite point light. */
class PointLight : public Light
{
public:

	PointLight();
	~PointLight();

	double getContribution (Scene &scene, const Vector point);

	friend std::istream& operator >> (std::istream &in, PointLight &light);
};

#endif // PointLight_h__
