#ifndef Sphere_h__
#define Sphere_h__

#include "Primitive.h"


/* Represents a sphere (or ellipsoid, if scaled). */
class Sphere : public Primitive
{
public:

	bool intersect (const Ray &ray, Intersection &intersection) const;
	bool doesIntersect (const Ray &ray) const;

	friend std::istream& operator >> (std::istream &in, Sphere &s);
};

#endif // Sphere_h__
