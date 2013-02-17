#ifndef Intersection_h__
#define Intersection_h__

#include "Vector.h"


class Primitive;

/* Storage container for information about an intersection between a ray
 * and a primitive. */
class Intersection
{
public:

	double t;
	Vector normal;
	Primitive *primitive;
};

#endif // Intersection_h__
