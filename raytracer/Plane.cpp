#include "Plane.h"


/* Straightforward textbook plane intersection routine. */
bool Plane::intersect (const Ray &ray, Intersection &intersection) const
{
	const double NdotRayDir = N.dot(ray.direction);

	if (NdotRayDir < 0.000001 && NdotRayDir > -0.000001) // no hit
		return false;

	intersection.t = (D - N.dot(ray.origin)) / (NdotRayDir);

	if (NdotRayDir > 0)
		intersection.normal = -N;
	else
		intersection.normal = N;

	intersection.primitive = const_cast<Plane*>(this);

	return true;
}


bool Plane::doesIntersect (const Ray &ray) const
{
	const double NdotRayDir = N.dot(ray.direction);

	if (NdotRayDir < 0.000001 && NdotRayDir > -0.000001) // no hit
		return false;

	return true;
}


std::istream& operator >> (std::istream &in, Plane &p)
{
	return in
		>> p.name
		>> p.N
		>> p.D
		>> p.color
		>> p.ambience
		>> p.diffuse
		>> p.specular
		>> p.reflectance
		>> p.specularExponent
		>> p.refractiveIndex;
}
