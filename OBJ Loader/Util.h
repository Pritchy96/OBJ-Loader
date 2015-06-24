#pragma once


	namespace Util
	{
	extern int seed;
	int RandomInt(int max, int min, int newSeed = seed);
	double RandomDouble(double max, double min, int newSeed = seed);
	void SeedGenerator(int seed = NULL);
	}