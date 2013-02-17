#ifndef Primitive_h__
#define Primitive_h__

#include <istream>
#include <string>

#include "Vector.h"
#include "Mat4.h"
#include "Ray.h"
#include "Intersection.h"


/* Base class for renderable primitives.
 * Contains material information too, but that should probably be moved into
 * a different Material class. Has two methods, intersect and doesIntersect.
 * 
 * intersect is used when you want the intersection information, whereas
 * doesIntersect only returns a boolean and is used for shadow rays.
 * 
 * This distinction yields a small performance improvement, but should be combined
 * into one method, with the Intersection object capable of calculating the necessary
 * information if needed (to avoid recomputation).
 */
class Primitive
{
public:

	Primitive();
	virtual ~Primitive() {};

	virtual bool intersect (const Ray &ray, Intersection &intersection) const = 0;
	virtual bool doesIntersect (const Ray &ray) const = 0;

	std::string name;
	Vector color;

	double ambience;
	double diffuse;
	double specular;
	double reflectance;
	int specularExponent;

	Mat4 worldToObject;
	Mat4 worldToObject_transpose;
	Mat4 objectToWorld;

	// extra features below
	double refractiveIndex;
};

#endif // Primitive_h__
