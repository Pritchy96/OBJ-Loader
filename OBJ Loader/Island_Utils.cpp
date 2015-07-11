// ----------------------------------------------------------
// Includes 
// ----------------------------------------------------------
#include "stdafx.h"
#include "Island_Utils.h"
#include <vector>
#include <cstdlib>
#include <math.h>
#include "Util.h"
#include "EasyBMP.h"
using namespace std;
// ----------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------C:\Windows\SysWOW64


namespace Island_Utils
{
	vector<vector<int>> ShapeIsland(vector<vector<int>> *fractal)
	{
		double maxDistance = 0, width = (*fractal).size(), height = (*fractal)[0].size(), centerX = width / 2, centerY = height / 2;
		maxDistance = sqrt((pow(centerX, 2)) + (pow(centerY, 2)));
		maxDistance = sqrt((pow(centerX, 2)) + (pow(centerY, 2)));

		vector<vector<int>>  map = vector<vector<int>>();
		//Resizing array and setting all values to 0.
		map.resize( width , vector<int>( height, 0 ) );


		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{

#pragma region Generating Island Shape
				double distX = abs(x - centerX), distY = abs(y - centerY);    //Distance fron center in x and y.
				double distance = sqrt(pow(distX, 2) + pow(distY, 2));   //Distance from center.
				double fractalValue = ((*fractal)[x][y]);   //Retrieve the fractal value at position (x, y).
				double heightValue = fractalValue / 255;   //Height value generated from random plasma noise. Dictates how chaotic the island is.
				double gradientValue = ((distance / maxDistance));  //Gradient used to get an island shape
				int gradientStrength = 255; //how prevalent the Circular gradient is in the final value. Reduce to make reduce the centering effect. Not reccomended to change below 255.
				int heightStrength = 100;   //How prevalant the heightmap is in the final value. Reduce for smaller, less chaotic islands.
				int offset = -90;    //Offset used to make boost the value to make bigger islands. Reduce for smaller islands.

				double finalValue = (double)abs((heightStrength * heightValue) + (gradientValue * gradientStrength) + offset); //Construct the final value for the island.
#pragma endregion

#pragma region Removing Fractal (blue for land, black for sea).
				//Keep value between 0 and 255
				if (finalValue > 109 || distance > (maxDistance*3/4))    //If we're high enough to be considered ocean or close to the edge.
				{
					//If we're close to the center and it's going to be water, instead make it land.  
					//if (distance < 170)
					//{
					//	finalValue = 0;
						//finalValue = (byte)Math.Min(255, (finalValue + (int)(((float)finalValue / 255) * rand.Next(-5, 5))));
					//}
					//else //Otherwise make it sea.
					//{
						finalValue = 255;
					//}
				}
				
				else
				{
					finalValue = 0;
					//finalValue = (byte)Math.Min(255, (finalValue + (int)(((float)finalValue / 255) * rand.Next(170-5, 5))));
				}
#pragma endregion

				map[x][y] = finalValue;
			}
		}

		return map;
	}  

	vector<vector<int>> CalculateBiomes(vector<vector<int>> *islandFractal, vector<vector<int>> *islandShape, 
		vector<vector<int>> *heightFractal, vector<vector<int>> *tempFractal, vector<vector<int>> *rainFractal)
	{
		int width = (*islandFractal).size(), height = (*islandFractal)[0].size();

		vector<vector<int>>  colouredIsland = vector<vector<int>>();
		//Resizing array and setting all values to 0.
		colouredIsland.resize( width , vector<int>( height, 0 ) );

		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{

				if ((*islandShape)[x][y] == 0)  //Land
				{
					if ((*tempFractal)[x][y] < 20)
					{
						//Tundra
						colouredIsland[x][y] = 8;
					}
					else if ((*tempFractal)[x][y] < 40)
					{
						if ((*rainFractal)[x][y] < 10)
						{
							//Tropical Sand
							colouredIsland[x][y] = 0;
						}
						else if ((*rainFractal)[x][y] < 20)
						{
							//Desert
							colouredIsland[x][y] = 1;
						}
						else if ((*rainFractal)[x][y] < 60)
						{
							//Taiga
							colouredIsland[x][y] = 7;
						}
						else
						{
							//Swamp
							colouredIsland[x][y] = 6;
						}
					}
					else if ((*tempFractal)[x][y] < 50)
					{
						if ((*rainFractal)[x][y] < 10)
						{
							//Tropical Sand
							colouredIsland[x][y] = 0;
						}
						else if ((*rainFractal)[x][y] < 20)
						{
							//Desert
							colouredIsland[x][y] = 1;
						}
						else if ((*rainFractal)[x][y] < 50)
						{
							//Dedious Forest
							colouredIsland[x][y] = 4;
						}
						else if ((*rainFractal)[x][y] < 80)
						{
							//Rain Forest
							colouredIsland[x][y] = 5;
						}
						else
						{
							//Swamp
							colouredIsland[x][y] = 6;
						}
					}
					else if ((*tempFractal)[x][y] < 60)
					{
						if ((*rainFractal)[x][y] < 10)
						{
							//Tropical Sand
							colouredIsland[x][y] = 0;
						}
						else if ((*rainFractal)[x][y] < 30)
						{
							//Desert
							colouredIsland[x][y] = 1;
						}
						if ((*rainFractal)[x][y] < 50)
						{
							//Dedious Forest
							colouredIsland[x][y] = 4;
						}
						else if ((*rainFractal)[x][y] < 80)
						{
							//Rain Forest
							colouredIsland[x][y] = 5;
						}
						else
						{
							//Swamp
							colouredIsland[x][y] = 6;
						}
					}
					else if ((*tempFractal)[x][y] < 70)
					{
						if ((*rainFractal)[x][y] < 10)
						{
							//Tropical Sand
							colouredIsland[x][y] = 0;
						}
						else if ((*rainFractal)[x][y] < 30)
						{
							//Desert
							colouredIsland[x][y] = 1;
						}
						else if ((*rainFractal)[x][y] < 60)
						{
							//Dedious Forest
							colouredIsland[x][y] = 4;
						}
						else if ((*rainFractal)[x][y] < 80)
						{
							//Rain Forest
							colouredIsland[x][y] = 5;
						}
						else
						{
							//Swamp
							colouredIsland[x][y] = 6;
						}
					}
					else if ((*tempFractal)[x][y] < 90)
					{
						if ((*rainFractal)[x][y] < 20)
						{
							//Tropical Sand
							colouredIsland[x][y] = 0;
						}
						else if ((*rainFractal)[x][y] < 60)
						{
							//Tropical Seasonal Forest/Savanna
							colouredIsland[x][y] = 2;
						}
						else
						{
							//Tropical Forest
							colouredIsland[x][y] = 3;
						}
					}
					else
					{
						if ((*rainFractal)[x][y] < 30)
						{
							//Tropical Sand
							colouredIsland[x][y] = 0;
						}
						else if ((*rainFractal)[x][y] < 70)
						{
							//Tropical Seasonal Forest/Savanna
							colouredIsland[x][y] = 2;
						}
						else
						{
							//Tropical Forest
							colouredIsland[x][y] = 3;
						}
					}
				}
				else //Sea
				{
					if ((*heightFractal)[x][y] < 58) //Deep areas
					{
						colouredIsland[x][y] = -2;
					}
					else
					{
						//Inner sea
						colouredIsland[x][y] = -1;
					}
				}
			}
		}

		return colouredIsland;
	}

	vector<int> GetBiomeColour(int biome)
	{
		vector<int> colour = vector<int>();
		colour.resize(3);

		switch (biome)
		{
		case -2:
			{
				//Deep Sea
				colour[0] = 28;
				colour[1] = 50;
				colour[2] = 63;
				break;
			}
		case -1:
			{
				//Shallow Sea.
				colour[0] = 38;
				colour[1] = 60;
				colour[2] = 73;
				break;
			}
		case 0:
			{
				//Tropical Sand
				colour[0] = 250;
				colour[1] = 148;
				colour[2] = 24;
				break;
			}
		case 1:
			{
				//Sand
				colour[0] = 250;
				colour[1] = 219;
				colour[2] = 7;
				break;
			}
		case 2:
			{
				//Tropical Seasonal Forest/Savanna
				colour[0] = 250;
				colour[1] = 219;
				colour[2] = 7;
				break;
			}
		case 3:
			{
				//Tropical Rain Forest
				colour[0] = 155;
				colour[1] = 224;
				colour[2] = 35;

				break;
			}
		case 4:
			{
				//Dedious Forest
				colour[0] = 46;
				colour[1] = 177;
				colour[2] =83 ;
				break;
			}
		case 5:
			{
				//Temperate Rain Forest
				colour[0] = 7;
				colour[1] = 249;
				colour[2] = 162;
				break;
			}
		case 6:
			{
				//Swamp
				colour[0] = 76;
				colour[1] = 102;
				colour[2] = 0;
				break;
			}
		case 7:
			{
				//Taiga
				colour[0] = 5;
				colour[1] = 102;
				colour[2] = 33;
				break;
			}
		case 8:
			{
				//Tundra
				colour[0] = 85;
				colour[1] = 235;
				colour[2] = 249;
				break;
			}
		}

		return colour;
	}

	vector<vector<int>> MakeCircularGradient(int width, int height, int maxValue, int minValue)
	{
		vector<vector<int>> map = vector<vector<int>>();
		//Resizing array and setting all values to 0.
		map.resize(width , vector<int>( height, 0 ) );

		double maxDistance = 0, centerX = width / 2, centerY = height / 2;
		maxDistance = sqrt((pow(centerX, 2)) + (pow(centerY, 2)));

		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{

				double distX = abs(x - centerX), distY = abs(y - centerY);    //Distance fron center in x and y.
				double distance = sqrt(pow(distX, 2) + pow(distY, 2));   //Distance from center.

				map[x][y] = (int)((distance / maxDistance) * 255);
			}
		}
		return map;
	}

	vector<vector<int>> InterpolateBitmaps(vector<vector<int>> *bmp1, vector<vector<int>> *bmp2, double bmp1Coeff,
		double bmp2Coeff, int offset, int maxValue, int minValue)
	{
		int width = (*bmp1).size(), height = (*bmp1)[0].size();

		vector<vector<int>>  newBmp = vector<vector<int>>();
		//Resizing array and setting all values to 0.
		newBmp.resize( width , vector<int>( height, 0 ));

		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				double val = (((*bmp1)[x][y] * bmp1Coeff) + ((*bmp2)[x][y] * bmp2Coeff));

				if (val > maxValue)
				{
					val = maxValue;
				}
				else if (val < minValue)
				{
					val = minValue;
				}

				newBmp[x][y] = (int)(val);
			}
		}
		return newBmp;
	}

	void SaveImage(vector<vector<int>>*  array, string imageName)
	{
		BMP image;
		int width = (*array).size(), height = (*array)[0].size();
		image.SetSize(width, height);
		image.SetBitDepth(32);

		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				//vector<int> colour = Island_Utils::GetBiomeColour((*array)[x][y]);
				image(x, y)->Red = (*array)[x][y];
				image(x, y)->Green = (*array)[x][y];
				image(x, y)->Blue = (*array)[x][y];
			}
		}

		const char *cstr = imageName.c_str();
		image.WriteToFile(cstr);
	}
	
	void SaveBiomeImage(vector<vector<int>>*  array, string imageName)
	{
		BMP image;
		int width = (*array).size(), height = (*array)[0].size();
		image.SetSize(width, height);
		image.SetBitDepth(32);

		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				vector<int> colour = Island_Utils::GetBiomeColour((*array)[x][y]);
				image(x, y)->Red = colour[0];
				image(x, y)->Green = colour[1];
				image(x, y)->Blue = colour[2];
			}
		}

		const char *cstr = imageName.c_str();
		image.WriteToFile(cstr);
	}
	
}