#ifndef Triangle_h__
#define Triangle_h__

#include "Primitive.h"
#include "Intersection.h"


/* Represents a single triangle. */
class Triangle : public Primitive
{
public:

	bool intersect (const Ray &ray, Intersection &intersection) const;
	bool doesIntersect (const Ray &ray) const;

	Vector p1, p2, p3;

	// the triangle's plane
	Vector N;
	double D;

	friend std::istream& operator >> (std::istream &in, Triangle &t);
};

#endif // Triangle_h__
