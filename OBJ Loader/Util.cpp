#include "stdafx.h"
#include "Util.h"
#include <cstdlib>
#include <ctime>

namespace Util
{

	void SeedGenerator(int seed)
	{
		//https://www.thenewboston.com/videos.php?cat=16&video=17503	
		if (seed = NULL)	//If no number was entered as a seed.
		{
			srand(time(NULL));	//Time(Null) is a random num based on clock timer.
		}
		else	//If a number was entered as a seed..
		{
			srand(seed);	//Use it.
		}
	}

	int RandomInt(int max, int min, unsigned int newSeed) 
	{
		//Seeding the Num Generator.
		srand(newSeed);

		//Return random number, between min and max.
		return (min + (rand()%max));
	}

	double RandomDouble(double max, double min, unsigned int newSeed) //Time(Null) is a random num based on clock timer.
	{
		//Seeding the Num Generator.
		srand(newSeed);

		double num = (double) rand()/INT_MAX;	//srand gives an int, so convert to a double and make it a small decimal.
		return min + (num * (max - min));	//return it between the two values.
	}
}
