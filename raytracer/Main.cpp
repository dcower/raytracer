#include <iostream>
#include <ctime>

#include "Scene.h"
#include "Globals.h"


int main (int argc, char *argv[])
{
	// top version with only with C++11 (or at least what VS2012 supports)
	//Globals::random_generator.seed((unsigned long)std::chrono::system_clock::now().time_since_epoch().count());
	Globals::random_generator.seed((unsigned long)(time(NULL)+rand()));

	if (argc != 2)
	{
		std::cerr << "usage: raytracer <scenefilename>" << std::endl;

		return -1;
	}

	try
	{
		Scene scene(argv[1]);

		scene.render();
	}
	catch (int)
	{
		std::cerr << "Error loading scene!" << std::endl;

		return -1;
	}

	return 0;
}
