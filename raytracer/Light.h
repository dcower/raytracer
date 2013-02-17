#ifndef Light_h__
#define Light_h__

#include <string>
#include "Vector.h"


class Scene;

/* Base class for lights */
class Light
{
public:

	virtual ~Light() { }

	virtual Vector getPoint()
	{
		return position;
	};

	virtual double getContribution (Scene &scene, const Vector point) = 0;

	std::string name;
	Vector position;
	Vector intensity;
};

#endif // Light_h__
