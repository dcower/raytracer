#ifndef Box_h__
#define Box_h__

#include "Primitive.h"


/* Unit-cube primitive. Change size/rotation with scale/rotation matrices.
 * 
 * The box is first rotated about X, then Y, then Z. */
class Box : public Primitive
{
public:

	bool intersect (const Ray &ray, Intersection &intersection) const;
	bool doesIntersect (const Ray &ray) const;

	friend std::istream& operator >> (std::istream &in, Box &b);
};

#endif // Box_h__
