#ifndef Plane_h__
#define Plane_h__

#include "Primitive.h"


/* A 3D plane, defined by a (not necessarily normalized) normal and distance D.
 * That is, the plane x*N_x + y*N_y + z*N_z = D. */
class Plane : public Primitive
{
public:

	bool intersect (const Ray &ray, Intersection &intersection) const;
	bool doesIntersect (const Ray &ray) const;

	Vector N;
	double D;

	friend std::istream& operator >> (std::istream &in, Plane &p);
};

#endif // Plane_h__
