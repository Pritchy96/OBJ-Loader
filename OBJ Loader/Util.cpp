#include "stdafx.h"
#include "Util.h"
#include <cstdlib>
#include <ctime>
#include <math.h>

namespace Util
{
	int global_seed = NULL;
	void SeedGenerator(int seed)
	{
		//https://www.thenewboston.com/videos.php?cat=16&video=17503	
		if (seed == 0)	//If no number was entered as a seed.
		{
			srand(time(NULL));	//Time(Null) is a random num based on clock timer.
		}
		else	//If a number was entered as a seed..
		{
			srand(seed);	//Use it.
		}
	}

	int RandomInt(int max, int min) 
	{
		//Return random number, between min and max.
		return (min + (rand()%max));
	}

	double RandomDouble(double max, double min) 
	{
		
		double num = (double) RandomInt(max*10000, min * 10000);	//srand gives an int, so convert to a double and make it a small decimal.
		return (num/10000);	//return it between the two values.
	}

	int Round(double d)
	{
		return (int)(d + 0.5);
	}
}


/*
		double num = (double) rand()/INT_MAX;	//srand gives an int, so convert to a double and make it a small decimal.
		return min + (num * (max - min));	//return it between the two values.
		*/