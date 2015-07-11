//#pragma once
#include <vector>
#include "EasyBMP.h"
using namespace std;



namespace Island_Utils
{
	vector<vector<int>> ShapeIsland(vector<vector<int>> *fractal);
	vector<vector<int>> CalculateBiomes(vector<vector<int>> *islandFractal, vector<vector<int>> *islandShape, 
		vector<vector<int>> *heightFractal, vector<vector<int>> *tempFractal, vector<vector<int>> *rainFractal);
	vector<int> GetBiomeColour(int biome);
	vector<vector<int>> MakeCircularGradient(int width, int height, int maxValue = 255, int minValue = 0);
	vector<vector<int>> InterpolateBitmaps(vector<vector<int>> *bmp1, vector<vector<int>> *bmp2, double bmp1Coeff = 0.5,
		double bmp2Coeff = 0.5, int offset = 0, int maxValue = 255, int minValue = 0);
	void SaveImage(vector<vector<int>>*  array, string imageName);
	void SaveBiomeImage(vector<vector<int>>*  array, string imageName);
	
};
