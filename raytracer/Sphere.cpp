#include "Sphere.h"
#include "Mat4.h"
#include "Globals.h"

#include <cmath>

using Globals::enableExtraFeatures;


bool Sphere::intersect (const Ray &ray, Intersection &intersection) const
{
	Ray objectRay = ray;
	objectRay.transform(worldToObject);

	const Vector &S = objectRay.origin;
	const Vector &c = objectRay.direction;

	const double SdotC = S.dot(c);

	const double discriminant = SdotC*SdotC - c.lenSquared() * (S.lenSquared() - 1);

	if (discriminant < 0) // no hit
		return false;
	else
	{
		// does the ray originate inside the sphere?
		const bool inside = (S.lenSquared() - 1) < 0;

		// if the ray originates inside the sphere, get the far intersection
		// otherwise, get smaller/closer one
		intersection.t = (-S.dot(c) + (inside ? 1 : -1) * sqrt(discriminant)) / c.lenSquared();

		intersection.normal = worldToObject_transpose.mulVector(objectRay.at(intersection.t));
		intersection.normal.normalize();

		intersection.primitive = const_cast<Sphere*>(this);

		return true;
	}
}


bool Sphere::doesIntersect (const Ray &ray) const
{
	Ray objectRay = ray;
	objectRay.transform(worldToObject);

	const Vector &S = objectRay.origin;
	const Vector &c = objectRay.direction;

	const double SdotC = S.dot(c);

	const double discriminant = SdotC*SdotC - c.lenSquared() * (S.lenSquared() - 1);

	if (discriminant < 0)
		return false;
	else
		return true;
}


std::istream& operator >> (std::istream &in, Sphere &s)
{
	Vector scale;
	Vector position;

	std::istream &out =
		in	>> s.name
			>> position
			>> scale
			>> s.color
			>> s.ambience
			>> s.diffuse
			>> s.specular
			>> s.reflectance
			>> s.specularExponent;

	if (enableExtraFeatures)
		out	>> s.refractiveIndex;

	s.objectToWorld = Mat4::translate(position.x, position.y, position.z) * Mat4::scale(scale.x, scale.y, scale.z);
	s.worldToObject = s.objectToWorld.inverse();
	s.worldToObject_transpose = s.worldToObject.transpose();

	return out;
}