#include "Scene.h"
#include "Plane.h"
#include "Box.h"
#include "Sphere.h"
#include "Triangle.h"
#include "PointLight.h"
#include "AreaLight.h"
#include "Ray.h"
#include "Globals.h"

#include <iostream>
#include <fstream>

using namespace Globals;

#define SHOWPROGRESS


/* Scene::Scene
 *
 * Reads in a scene from the given filename and initializes this Scene object
 * accordingly. Throws an integer (with no meaning) if an error occurs.
 */
Scene::Scene(const std::string &filename)
{
	lightNum = 0;
	maxDepth = 3;
	bias = 0.0001;
	refractiveIndex = 1.0;
	AASamples = 1;

	std::list<Primitive*> primitivesList;
	std::list<Light*> lightsList;

	std::ifstream fin(filename);

	if (!fin.good())
	{
		std::cerr << "Error opening scene " << filename << std::endl;

		throw 0;

		return;
	}

	unsigned int keyNum = 0;

	while (!fin.eof() && fin.good())
	{
		keyNum++;

		std::string key;

		fin >> key;

		if (key == "")
			continue;
		else if (key == "ENABLEEXTRAFEATURES")
			enableExtraFeatures = true;
		else if (key == "NEAR")
			fin >> this->near;
		else if (key == "LEFT")
			fin >> this->left;
		else if (key == "RIGHT")
			fin >> this->right;
		else if (key == "BOTTOM")
			fin >> this->bottom;
		else if (key == "TOP")
			fin >> this->top;
		else if (key == "RES")
			fin >> this->columns >> this->rows;
		else if (key == "SPHERE")
		{
			Sphere *sphere = new Sphere;
			fin >> *sphere;
			primitivesList.push_back(sphere);
		}
		else if (key == "LIGHT")
		{
			PointLight *light = new PointLight;
			fin >> *light;
			lightsList.push_back(light);
		}
		else if (key == "BACK")
			fin >> this->backgroundColor;
		else if (key == "AMBIENT")
			fin >> this->ambientIntensity;
		else if (key == "OUTPUT")
			fin >> this->outputName;
		else if (key == "PLANE" && enableExtraFeatures)
		{
			Plane *plane = new Plane;
			fin >> *plane;
			primitivesList.push_back(plane);
		}
		else if (key == "TRIANGLE" && enableExtraFeatures)
		{
			Triangle *triangle = new Triangle;
			fin >> *triangle;
			primitivesList.push_back(triangle);
		}
		else if (key == "BOX" && enableExtraFeatures)
		{
			Box *box = new Box;
			fin >> *box;
			primitivesList.push_back(box);
		}
		else if (key == "AREALIGHT" && enableExtraFeatures)
		{
			AreaLight *light = new AreaLight;
			fin >> *light;
			lightsList.push_back(light);
		}
		else if (key == "MAXDEPTH" && enableExtraFeatures)
			fin >> this->maxDepth;
		else if (key == "BIAS" && enableExtraFeatures)
			fin >> this->bias;
		else if (key == "AASAMPLES" && enableExtraFeatures)
			fin >> this->AASamples;
		else if (key == "REFRACTIVEINDEX" && enableExtraFeatures)
			fin >> this->refractiveIndex;
		else
		{
			std::cerr << "Error parsing input; got key " << key << std::endl;

			throw 0;

			return;
		}
	}

	if (!fin.eof() && !fin.good())
	{
		std::cerr << "Error reading scene " << filename << " (key #" << keyNum << ")" << std::endl;

		throw 0;

		return;
	}

	fin.close();

	// transfer primitives and lights from temporary lists to fixed arrays

	primitiveNum = primitivesList.size();

	if (primitiveNum > 0)
	{
		primitives = new Primitive*[primitiveNum];

		unsigned int i = 0;

		for (std::list<Primitive*>::iterator primitive = primitivesList.begin(); primitive != primitivesList.end(); primitive++, i++)
		{
			primitives[i] = *primitive;
		}
	}

	lightNum = lightsList.size();

	if (lightNum > 0)
	{
		lights = new Light*[lightNum];

		unsigned int i = 0;

		for (std::list<Light*>::iterator light = lightsList.begin(); light != lightsList.end(); light++, i++)
		{
			lights[i] = *light;
		}
	}
}


Scene::~Scene()
{
	while (primitiveNum)
		delete primitives[--primitiveNum];

	delete[] primitives;

	while (lightNum)
		delete lights[--lightNum];

	delete[] lights;
}


/* Scene::render
 *
 * Renders the scene and saves it to the file designated by the input scene
 * description. Supports parallelism via OpenMP (of rows).
 */
void Scene::render()
{
	const double invAASamples = 1.0/AASamples;
	const double invAASamplesSq = invAASamples*invAASamples;
	const double invRows = 1.0/rows;
	const double invColumns = 1.0/columns;
	const double W = (right-left)/2.0;
	const double H = (top-bottom)/2.0;

	unsigned char *pixels = new unsigned char[columns*rows*3];
	tracedRays = 0;

#ifdef SHOWPROGRESS
	int processedRows = 0;
#endif

	#pragma omp parallel for
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			Vector color;

			// should maybe implement an AA scheme that reuses samples from neighboring cells
			for (unsigned int x = 0; x < AASamples; x++)
			{
				for (unsigned int y = 0; y < AASamples; y++)
				{
					// can make this calculation a bit faster, but this is clearer
					Vector direction = Vector(W * ((2.0*c + 2*invAASamples*x)*invColumns - 1), -H * ((2.0*r + 2*invAASamples*y)*invRows - 1), -near);

					Ray ray(eye + direction, direction);
					ray.direction.normalize();

					color = color + traceRay(ray, 0, refractiveIndex);
				}
			}

			color *= invAASamplesSq;
			color.clamp(0, 1);

			color = color * 255;
			pixels[3*(rows*r+c)+0] = (unsigned char)color.r;
			pixels[3*(rows*r+c)+1] = (unsigned char)color.g;
			pixels[3*(rows*r+c)+2] = (unsigned char)color.b;
		}

#ifdef SHOWPROGRESS
		int proc;
		#pragma omp atomic
		++processedRows;
		proc = processedRows;
		std::cout << proc << std::endl;
#endif
	}

	std::cout << "Traced rays: " << tracedRays << std::endl;

	saveImage(pixels);

	delete[] pixels;
}


/* Scene::traceRay
 *
 * Traces the given ray through the scene, assuming that the ray begins in
 * a medium with the given refractive index. Returns black if depth >= maxDepth,
 * or the computed color of the ray's hit point otherwise. The ray's direction
 * should be normalized.
 */
Vector Scene::traceRay(const Ray &ray, const int depth, double index)
{
	if (depth >= maxDepth)
		return Vector();

#pragma omp atomic
	tracedRays++;

	Vector color;

	bool hit = false;
	Intersection closest;

	for (unsigned int i = 0; i < primitiveNum; i++)
	{
		Intersection intersection;

		bool hits = primitives[i]->intersect(ray, intersection);

		if (hits && intersection.t >= 0)
		{
			if (intersection.t < closest.t || !hit)
			{
				closest = intersection;

				hit = true;
			}
		}
	}

	if (hit)
	{
		const double &t = closest.t;
		const Vector &normal = closest.normal;
		const Primitive *primitive = closest.primitive;
		const Vector hitPosition = ray.at(t);

		// compute ambient color
		color = primitive->color * this->ambientIntensity * primitive->ambience;

		// make sure the ray direction and normal point toward each other
		if (normal.dot(ray.direction) >= 0)
			closest.normal = -normal;

		for (unsigned int i = 0; i < lightNum; i++)
		{
			Light *light = lights[i];

			Vector L = light->position - (hitPosition + normal*bias);
			L.normalize();
			const double NdotL = normal.dot(L);

			if (NdotL <= 0) // angle between normal and light vector is >= 90 deg
				continue;
			
			const double lightContribution = light->getContribution(*this, hitPosition + normal*bias);

			if (primitive->diffuse > 0)
			{
				// compute and add diffuse color
				Vector diffuse = (light->intensity * primitive->diffuse * NdotL * primitive->color) * lightContribution;
				color = color + diffuse;
			}

			const Vector R = normal * (2*normal.dot(L)) - L;
			const Vector V = -ray.direction; // ray direction is already normalized..
			const double RdotV = R.dot(V);

			if (RdotV > 0 && primitive->specular > 0)
			{
				// compute and add specular highlight color
				Vector specular = (light->intensity * primitive->specular * pow(RdotV, primitive->specularExponent)) * lightContribution;
				color = color + specular;
			}
		}

		Vector reflection;

		if (primitive->reflectance > 0 || (enableExtraFeatures && primitive->refractiveIndex != 0))
		{
			Vector reflected = normal * -2*(normal.dot(ray.direction)) + ray.direction;
			reflection = traceRay (Ray(hitPosition + reflected * bias, reflected), depth+1, index);

			if (!enableExtraFeatures)
				color = color + reflection * primitive->reflectance;
		}

		// NOTE: intersecting refractive objects are not supported, and
		// placing the camera within a refractive object isn't supported.
		if (enableExtraFeatures && primitive->refractiveIndex != 0)
		{
			const double n1 = index;
			double n2;

			// are we leaving an object or entering one?
			const bool leaving = index != this->refractiveIndex;

			if (leaving)
				n2 = this->refractiveIndex;
			else
				n2 = primitive->refractiveIndex;

			const double n = n1 / n2;

			const double cosTheta_1 = normal.dot(-ray.direction);
			const double cosTheta_2Sq = 1 - n*n*(1 - cosTheta_1*cosTheta_1);

			if (cosTheta_2Sq >= 0)
			{
				const double cosTheta_2 = sqrt(cosTheta_2Sq);

				// might want to switch to schlick's approximation at some point
				const double rOrthogonal = (n1 * cosTheta_1 - n2 * cosTheta_2) / (n1 * cosTheta_1 + n2 * cosTheta_2);
				const double rParallel = (n2 * cosTheta_1 - n1 * cosTheta_2) / (n2 * cosTheta_1 + n1 * cosTheta_2);
				const double reflectance = (rOrthogonal * rOrthogonal + rParallel * rParallel) * 0.5;

				// should add beer's law to handle absorption. requires returning distance from traceRay though..
				const Vector T = ray.direction * n + normal * (n * cosTheta_1 - cosTheta_2);
				Vector refraction = traceRay( Ray( hitPosition + T * bias, T ), depth+1, n2);
				color = refraction * (1-reflectance) + reflection * reflectance;
			}
		}
		else if (enableExtraFeatures && primitive->reflectance > 0)
			color = color + reflection * primitive->reflectance;
	}
	else
	{
		if (depth == 0)
			color = backgroundColor;
		//else
		//	color = Vector();
	}

	return color;
}


/* Scene::traceShadowRay
 *
 * Traces the given ray through the scene, returning true at any time if an
 * object is hit along its path with t > 0 and t < lightDistance. The ray's
 * direction should be normalized.
 */
bool Scene::traceShadowRay(const Ray &ray, const double lightDistance)
{
#pragma omp atomic
	tracedRays++;

	for (unsigned int i = 0; i < primitiveNum; i++)
	{
		Primitive *primitive = primitives[i];

		const bool hit = primitive->doesIntersect(ray);

		if (hit)
		{
			Intersection intersection;
			primitive->intersect(ray, intersection);

			if (intersection.t > 0 && intersection.t < lightDistance)
				return true;
		}
	}

	return false;
}


/* Scene::saveImage
 *
 * Saves the given pixels to a PPM file designated by the scene description.
 * (Borrowed from Petros Faloutsos.)
 */
void Scene::saveImage(unsigned char* pixels) {
	FILE *fp;
	const int maxVal=255; 

	printf("Saving image %s: %d x %d\n", outputName.c_str(), columns, rows);
	fp = fopen(outputName.c_str(),"wb");
	if (!fp) {
		printf("Unable to open file '%s'\n", outputName.c_str());
		return;
	}
	fprintf(fp, "P6\n");
	fprintf(fp, "%d %d\n", columns, rows);
	fprintf(fp, "%d\n", maxVal);

	for(int j = 0; j < rows; j++) {
		fwrite(&pixels[j*columns*3], 3,columns,fp);
	}

	fclose(fp);
}
