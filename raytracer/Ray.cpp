#include "Ray.h"


Ray::Ray (Vector o, Vector d) : origin(o), direction(d)
{
}


Ray::Ray(const Ray &ray)
{
	this->origin = ray.origin;
	this->direction = ray.direction;
}


Vector Ray::at(const double &t) const
{
	return origin + direction*t;
}


Ray &Ray::transform(const Mat4 &m)
{
	origin = m.mulPoint(origin);
	direction = m.mulVector(direction);

	return *this;
}
