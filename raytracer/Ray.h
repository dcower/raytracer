#ifndef Ray_h__
#define Ray_h__

#include "Mat4.h"
#include "Vector.h"


/* Represents a ray in 3D space. */
class Ray
{
public:

	Ray(Vector o, Vector d);
	Ray(const Ray &ray);

	Ray &operator = (const Ray &b)
	{
		origin = b.origin;
		direction = b.direction;

		return *this;
	}

	Ray &transform(const Mat4 &m);

	Vector at(const double &t) const;

	Vector origin;
	Vector direction;
};

#endif // Ray_h__
