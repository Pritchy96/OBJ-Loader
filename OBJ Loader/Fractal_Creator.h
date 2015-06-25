#pragma once
#include "stdafx.h"
#include "Fractal_Creator.h"
#include "Util.h"
#include <vector>
#include <cstdlib>

static class Fractal_Creator
{
public:
	Fractal_Creator(void);
	~Fractal_Creator(void);
	double Fractal_Creator::Displace(double rectSize, int roughness);
	double Fractal_Creator::Round(double num);
	void Fractal_Creator::Divide(std::vector<std::vector<int>> *mapRgbValues, double x, double y, double width, double height, int roughness, double c1, double c2, double c3, double c4, int minValue, int maxValue);
	std::vector<std::vector<int>> Fractal_Creator::MakeFractal(int width, int height, int Roughness = 13, int maxValue = 255, int minValue = 0);

};

