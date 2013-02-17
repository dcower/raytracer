#include "Vector.h"


std::istream& operator >> (std::istream &in, Vector &v)
{
	return in >> v.x >> v.y >> v.z;
}
