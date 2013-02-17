#ifndef Scene_h__
#define Scene_h__

#include <list>
#include <string>

#include "Vector.h"
#include "Primitive.h"
#include "Light.h"


/* Contains all scene primitives/lights and has methods for tracing rays through
 * it (both shadow rays and normal rays). */
class Scene
{
public:

	Scene();
	Scene(const std::string &filename);
	~Scene();

	void render();
	Vector traceRay(const Ray &ray, const int depth, double index);
	bool traceShadowRay(const Ray &ray, const double lightDistance);

private:

	void saveImage(unsigned char *pixels);

	double near;
	double left;
	double right;
	double bottom;
	double top;
	int columns;
	int rows;

	Primitive **primitives;
	unsigned int primitiveNum;

	Light **lights;
	unsigned int lightNum;

	Vector backgroundColor;
	Vector ambientIntensity;
	std::string outputName;

	Vector eye;

	// Extensions to assignment below
	int maxDepth;
	double bias;
	double refractiveIndex;
	unsigned int AASamples;
	unsigned long long tracedRays;
};

#endif // Scene_h__
