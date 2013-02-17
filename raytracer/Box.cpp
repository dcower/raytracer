#include <istream>
#include "Box.h"


/* Simple slab-based unit cube intersection. i.e., a box is the intersection of
 * 6 planes, and we find which of these planes, if any, has the closest
 * intersection point.
 */
bool Box::intersect (const Ray &ray, Intersection &intersection) const
{
	Ray objectRay = ray;
	objectRay.transform(worldToObject);

	const Vector &S = objectRay.origin;
	const Vector &c = objectRay.direction;

	const double D = 0.5; // plane distance for unit cube

	double t1, t2;
	double tnear, tfar;
	char tnear_index=0, tfar_index=0;

	// intersect with the X planes
	if (c.x == 0 && (S.x < -D || S.x > D))
		return false;
	else
	{
		tnear = (-D - S.x) / c.x;
		tfar = (D - S.x) / c.x;

		if (tnear > tfar)
			std::swap(tnear, tfar);

		if (tnear > tfar)
			return false;
		if (tfar < 0)
			return false;
	}

	// intersect with the Y planes
	if (c.y == 0 && (S.y < -D || S.y > D))
		return false;
	else
	{
		t1 = (-D - S.y) / c.y;
		t2 = (D - S.y) / c.y;

		if (t1 > t2)
			std::swap(t1, t2);

		if (t1 > tnear)
		{
			tnear = t1;
			tnear_index = 1;
		}

		if (t2 < tfar)
		{
			tfar = t2;
			tfar_index = 1;
		}

		if (tnear > tfar)
			return false;
		if (tfar < 0)
			return false;
	}

	// intersect with the Z planes
	if (c.z == 0 && (S.z < -D || S.z > D))
		return false;
	else
	{
		t1 = (-D - S.z) / c.z;
		t2 = (D - S.z) / c.z;

		if (t1 > t2)
			std::swap(t1, t2);

		if (t1 > tnear)
		{
			tnear = t1;
			tnear_index = 2;
		}

		if (t2 < tfar)
		{
			tfar = t2;
			tfar_index = 2;
		}

		if (tnear > tfar)
			return false;
		if (tfar < 0)
			return false;
	}

	const Vector normals[3] = {Vector(1,0,0), Vector(0,1,0), Vector(0,0,1)};

	if (tnear <= 0)
	{
		intersection.t = tfar;
		intersection.normal = normals[tfar_index];
	}
	else
	{
		intersection.t = tnear;
		intersection.normal = normals[tnear_index];
	}

	intersection.primitive = const_cast<Box*>(this);

	intersection.normal = worldToObject_transpose.mulVector(intersection.normal);

	if (intersection.normal.dot(c) > 0)
		intersection.normal = -intersection.normal;

	intersection.normal.normalize();

	return true;
}


bool Box::doesIntersect (const Ray &ray) const
{
	Ray objectRay = ray;
	objectRay.transform(worldToObject);

	const Vector &S = objectRay.origin;
	const Vector &c = objectRay.direction;

	const double D = 0.5; // plane distance for unit cube

	double t1, t2;
	double tnear, tfar;

	if (c.x == 0 && (S.x < -D || S.x > D))
		return false;
	else
	{
		tnear = (-D - S.x) / c.x;
		tfar = (D - S.x) / c.x;

		if (tnear > tfar)
			std::swap(tnear, tfar);

		if (tnear > tfar)
			return false;
		if (tfar < 0)
			return false;
	}

	if (c.y == 0 && (S.y < -D || S.y > D))
		return false;
	else
	{
		t1 = (-D - S.y) / c.y;
		t2 = (D - S.y) / c.y;

		if (t1 > t2)
			std::swap(t1, t2);

		if (t1 > tnear)
			tnear = t1;
		if (t2 < tfar)
			tfar = t2;

		if (tnear > tfar)
			return false;
		if (tfar < 0)
			return false;
	}

	if (c.z == 0 && (S.z < -D || S.z > D))
		return false;
	else
	{
		t1 = (-D - S.z) / c.z;
		t2 = (D - S.z) / c.z;

		if (t1 > t2)
			std::swap(t1, t2);

		if (t1 > tnear)
			tnear = t1;
		if (t2 < tfar)
			tfar = t2;

		if (tnear > tfar)
			return false;
		if (tfar < 0)
			return false;
	}

	return true;
}


std::istream& operator >> (std::istream &in, Box &b)
{
	Vector scale;
	Vector position;
	Vector rotation;

	std::istream &out = in
		>> b.name
		>> position
		>> scale
		>> rotation
		>> b.color
		>> b.ambience
		>> b.diffuse
		>> b.specular
		>> b.reflectance
		>> b.specularExponent
		>> b.refractiveIndex;

	b.objectToWorld =
		Mat4::translate(position.x, position.y, position.z) *
		Mat4::rotZDeg(rotation.z) *
		Mat4::rotYDeg(rotation.y) *
		Mat4::rotXDeg(rotation.x) *
		Mat4::scale(scale.x, scale.y, scale.z);

	b.worldToObject = b.objectToWorld.inverse();
	b.worldToObject_transpose = b.worldToObject.transpose();

	return out;
}
