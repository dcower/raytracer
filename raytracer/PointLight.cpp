#include "PointLight.h"
#include "Scene.h"


PointLight::PointLight()
{
}


PointLight::~PointLight()
{
}


/* PointLight::getContribution
 * 
 * Returns 1 if the ray from the hit point to the point light is unobstructed;
 * 0 otherwise.
 * */
double PointLight::getContribution(Scene &scene, const Vector point)
{
	Vector hitToLight = this->position - point;
	double dist = hitToLight.length();
	hitToLight.normalize();

	return scene.traceShadowRay(Ray(point, hitToLight), dist) ? 0.0 : 1.0;
}


std::istream& operator >> (std::istream &in, PointLight &light)
{
	return in	>> light.name
				>> light.position
				>> light.intensity;
}