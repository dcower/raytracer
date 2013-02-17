#ifndef Globals_h__
#define Globals_h__

#include <random>


/* Namespace for any variables needed in a bunch of places. Not ideal. */
namespace Globals
{
	extern bool enableExtraFeatures;
	extern std::mt19937 random_generator; // Mersenne twister
	extern std::uniform_real_distribution<double> random_double; // random doubles between 0 and 1
	extern const double PI;
}

#endif // Globals_h__
