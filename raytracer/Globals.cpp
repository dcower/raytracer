#include "Globals.h"
#include <cmath>

namespace Globals
{
	bool enableExtraFeatures = false;
	std::mt19937 random_generator;
	std::uniform_real_distribution<double> random_double;
	const double PI = std::atan(1.0)*4;
}