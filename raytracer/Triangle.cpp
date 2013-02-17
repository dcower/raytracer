#include "Triangle.h"


bool Triangle::intersect(const Ray &ray, Intersection &intersection) const
{
	const double NdotRayDir = N.dot(ray.direction);

	if (NdotRayDir < 0.000001 && NdotRayDir > -0.000001) // no hit
		return false;

	intersection.t = (D - N.dot(ray.origin)) / (NdotRayDir);
	const Vector i = ray.at(intersection.t); // intersection point with plane

	// for an edge p1<->p2, we check to see if the cross product of that edge
	// vector (p2-p1) and the vector from p1 to the intersection point points
	// in the same direction as the normal vector
	if ((p2-p1).cross(i - p1).dot(N) >= 0 &&
		(p3-p2).cross(i - p2).dot(N) >= 0 &&
		(p1-p3).cross(i - p3).dot(N) >= 0)
	{
		// i is within the triangle
		intersection.primitive = const_cast<Triangle*>(this);

		if (NdotRayDir > 0)
			intersection.normal = -N;
		else
			intersection.normal = N;

		return true;
	}

	return false;
}


bool Triangle::doesIntersect (const Ray &ray) const
{
	const double NdotRayDir = N.dot(ray.direction);

	if (NdotRayDir < 0.000001 && NdotRayDir > -0.000001) // no hit
		return false;

	const double t = (D - N.dot(ray.origin)) / (NdotRayDir);
	const Vector i = ray.at(t); // intersection point with plane

	if (t < 0)
		return false;

	if ((p2-p1).cross(i - p1).dot(N) >= 0 &&
		(p3-p2).cross(i - p2).dot(N) >= 0 &&
		(p1-p3).cross(i - p3).dot(N) >= 0)
	{
		// i is within the triangle

		return true;
	}

	return false;
}


std::istream& operator >> (std::istream &in, Triangle &t)
{
	std::istream&out = in
		>> t.name
		>> t.p1
		>> t.p2
		>> t.p3
		>> t.color
		>> t.ambience
		>> t.diffuse
		>> t.specular
		>> t.reflectance
		>> t.specularExponent
		>> t.refractiveIndex;

	// calculate the plane that the triangle lies on
	t.N = (t.p2-t.p1).cross(t.p3-t.p1);
	t.N.normalize();
	t.D = t.N.dot(t.p1);

	return out;
}
