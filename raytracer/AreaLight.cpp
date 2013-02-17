#include "AreaLight.h"
#include "Globals.h"
#include "Scene.h"

using Globals::random_generator;
using Globals::random_double;


AreaLight::AreaLight()
{
	axis1Samples = axis2Samples = 1;
}


/* AreaLight::getPoint
 * 
 * Calculates a random point on the area light.
 */
Vector AreaLight::getPoint()
{
	return position + axis1 * (-0.5 + random_double(random_generator))*axis1L
		+ axis2 * (-0.5 + random_double(random_generator))*axis2L;
}


/* AreaLight::getContribution
 * 
 * Calculates the contribution (from 0, meaning fully shadowed, to 1, meaning
 * 100% of the light rays hit the point given). A stochastic, stratified sampling
 * scheme is used; a grid is formed over the area light, and a random
 * point within each cell is used.
 */
double AreaLight::getContribution (Scene &scene, const Vector point)
{
	const double invAxis1Samples = 1.0/axis1Samples;
	const double invAxis2Samples = 1.0/axis2Samples;
	double contribution = 0;

	for (unsigned int x = 0; x < axis1Samples; x++)
	{
		for (unsigned int y = 0; y < axis2Samples; y++)
		{
			/*Vector p = position + axis1 * (x*invAxis1Samples + random_double(random_generator)*invAxis1Samples) * axis1L
								+ axis2 * (y*invAxis2Samples + random_double(random_generator)*invAxis2Samples) * axis2L;*/

			Vector p = position + axis1 * ((x+0.5)*invAxis1Samples + (random_double(random_generator)-0.5)*invAxis1Samples - 0.5) * axis1L
								+ axis2 * ((y+0.5)*invAxis2Samples + (random_double(random_generator)-0.5)*invAxis2Samples - 0.5) * axis2L;

			Vector hitToLight = p - point;
			double dist = hitToLight.length();
			hitToLight.normalize();
			const double r = 3;
			const double d = dist / r;
			//double invSquare = 1.0 / (d*d);
			const double invSquare = 1.0;
			//const double invSquare = 1.0;

			if (!scene.traceShadowRay(Ray(point, hitToLight), dist))
				contribution+=invSquare;
		}
	}

	return contribution / (axis1Samples*axis2Samples);
}


std::istream& operator >> (std::istream &in, AreaLight &light)
{
	return in
		>> light.name
		>> light.position
		>> light.axis1
		>> light.axis2
		>> light.axis1L
		>> light.axis2L
		>> light.axis1Samples
		>> light.axis2Samples
		>> light.intensity;
}