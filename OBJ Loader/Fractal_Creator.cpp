// ----------------------------------------------------------
// Includes 
// ----------------------------------------------------------
#include "stdafx.h"
#include "Fractal_Creator.h"
#include "Util.h"
#include <vector>
#include <cstdlib>
#include <math.h>
using namespace std;

// ----------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------C:\Windows\SysWOW64
void display();
double Displace(double rectSize, int roughness);
double Round(double num);
void Divide(vector<vector<int>> *mapRgbValues, double x, double y, double width, double height, int roughness, double c1, double c2, double c3, double c4, int minValue, int maxValue);
vector<vector<int>> MakeFractal(int width, int height, int Roughness, int maxValue, int minValue);


// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
double screenSize = 0;   //Width + Height of screen.


vector<vector<int>> Fractal_Creator::MakeFractal(int width, int height, int Roughness, int maxValue, int minValue)
{
	screenSize = width + height;
	double maxDistance = 0, centerX = width / 2, centerY = height / 2;
	maxDistance = sqrt((pow(centerX, 2)) + (pow(centerY, 2)));

	vector<vector<int>>  map = vector<vector<int>>();
	//Resizing array and setting all values to 0.
	map.resize( width , vector<int>( height, 0 ) );

	//Calculate corner values (c1, c2, c3, c4).
	double c1 = Util::RandomDouble(1, 0);
	double c2 = Util::RandomDouble(1, 0);
	double c3 = Util::RandomDouble(1, 0);
	double c4 = Util::RandomDouble(1, 0);

	//Call Divide, begin the iteration.
	Divide(&map, 0, 0, width, height, Roughness, c1, c2, c3, c4, minValue, maxValue);

	return map;
}





/// <summary>
/// </summary>
/// <param name="colMapRgbValues"> Array of BGR Bytes </param>
/// <param name="bitmapWidth"> Width of the map image </param>
/// <param name="x"> X Pos </param>
/// <param name="y"> Y Pos </param>
/// <param name="width"> Width of current rectangle being worked on. </param>
/// <param name="height"> Height of current rectangle being worked on. </param>
/// <param name="c1"></param>
/// <param name="c2"></param>
/// <param name="c3"></param>
/// <param name="c4"></param>
void Fractal_Creator::Divide(vector<vector<int>> *mapRgbValues, double x, double y, double width, double height, int roughness, double c1, double c2, double c3, double c4, int minValue, int maxValue)
{
	//X and Y are the old c1 coordinates from the last recursive iteration.

	double middle, mid1, mid2, mid3, mid4;

	//calculate width and hight of new rectangle by halving the last.
	double newWidth = width / 2;
	double newHeight = height / 2;

	//If our rectangles are bigger than 1px x 1px.
	if (width > 1 || height > 1)
	{
		//Square Step.
		//Calculate middle Point by averaging corners and then adding a random displacement.
		middle = Round(((c1 + c2 + c3 + c4) / 4 + Displace(newWidth + newHeight, roughness)));

		//Diamond Step.
		//Calculating the edge points in order for the 4 points of each rectangle to all have values.
		//this is just the average of the two points it bisects.
		mid1 = Round((c1 + c2) / 2);
		mid2 = Round((c1 + c3) / 2);
		mid3 = Round((c2 + c4) / 2);
		mid4 = Round((c3 + c4) / 2);

		//Call divide to calculate the middle of the new rectangles.
		Divide(mapRgbValues, x, y, newWidth, newHeight, roughness, c1, mid1, mid2, middle, minValue, maxValue);
		Divide(mapRgbValues, x + newWidth, y, width - newWidth, newHeight, roughness, mid1, c2, middle, mid3, minValue, maxValue);
		Divide(mapRgbValues, x, y + newHeight, newWidth, height - newHeight, roughness, mid2, middle, c3, mid4, minValue, maxValue);
		Divide(mapRgbValues, x + newWidth, y + newHeight, width - newWidth, height - newHeight, roughness, middle, mid3, mid4, c4, minValue, maxValue);
	}
	//If our rectangles are now 1px x 1px, we are ready to calculate final values and draw.
	else 
	{
		//Average the points of the pixel sized rectangle down into a single number, which is that pixels final gradientValue.
		double heightValue = (c1 + c2 + c3 + c4) / 4;   //Height value generated from random plasma noise.
		//Setting the final value for this pixel.+
		(*mapRgbValues)[Util::Round(x)][Util::Round(y)] = (int)((heightValue * (maxValue - minValue)) - minValue);
	}
}

//Makes sure values stay within limits.
double Fractal_Creator::Round(double num)
{
	if (num > 1)
		return 1;
	else if (num < 0)
		return 0;
	else
		return num;
}

//Displaces gradientValue a small amount.
double Fractal_Creator::Displace(double rectSize, int roughness)
{
	double Max = rectSize / screenSize * roughness;
	return (Util::RandomDouble(1, 0) - 0.5) * Max;
}

Fractal_Creator::~Fractal_Creator(void)
{
}

Fractal_Creator::Fractal_Creator(void)
{


}

/*

// ----------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------C:\Windows\SysWOW64
void display();
void specialKeys();
void ReadFile();
int wasMain();

// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
double rotate_y=0;
double rotate_x=0;
double rotate_z=0;
double scale = 0.01;

static float lightPos[] = {1.0, 0.0, 1.0, 1.0}; 

bool debug = false;

//	-------------------------------------------------------
// display() Callback function
//	-------------------------------------------------------
void display(){


#pragma region LOADING_IMAGE
BMP heightmap, colourmap;
heightmap.ReadFromFile("HeightMap.bmp");
colourmap.ReadFromFile("ColourMap.bmp");
#pragma endregion

#pragma region SET_UP_OPENGL.
//  Clear screen and Z-buffer
glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

// Reset transformations
glLoadIdentity();

// Other Transformations
// glTranslatef( 0.1, 0.0, 0.0 );

// Rotation.
glRotatef( rotate_x, 1.0, 0.0, 0.0 );
glRotatef( rotate_y, 0.0, 1.0, 0.0 );
glRotatef(rotate_z, 0.0, 0.0, 1.0);

// Other Transformations
glScalef( scale, scale, scale ); 
#pragma endregion

//Iterate through bitmap
for (int i = -(heightmap.TellWidth() / 2); i < (heightmap.TellWidth()/2); i++)	//Goes from -100 to + 100 rather than 0 - 200 so camera is centered.
{
for (int j = -(heightmap.TellHeight() / 2); (j < heightmap.TellHeight() / 2); j++) //As above. This means some statements below use (+ heightmap.TellHeight()/2) to make it 0 - 200 again.
{
int iFromZero = i + (heightmap.TellWidth()/2);
int jFromZero = j + (heightmap.TellHeight()/2);

//Pixel Height/RBG Value
int pixelHeight = heightmap.GetPixel(iFromZero, jFromZero).Blue;

glBegin(GL_POLYGON);	//Must be called for each face.

//X = Width = i
//Y = Height = pixelHeight
//Z = Depth = j.

glColor3f( 
(float) colourmap.GetPixel(iFromZero, jFromZero).Red / 255, 
(float) colourmap.GetPixel(iFromZero, jFromZero).Green / 255, 
(float) colourmap.GetPixel(iFromZero, jFromZero).Blue / 255);  //Colour so faces can be differentiated.

float height = ((float) 255 - pixelHeight) / 10;

#pragma region TOP_FACE
glVertex3f(		//Top Left (looking at face)
(float) (i), 
height, 
(float) (j) 
);    
glVertex3f(		//Top Right (looking at face)
(float) ((i + 1)), 
height, 
(float) (j) 
);    
glVertex3f(		//Bottom Right (looking at face)
(float) ((i + 1)), 
height, 
(float) ((j + 1)) 
);  
glVertex3f(		//Bottom Left (looking at face)
(float) (i), 
height, 
(float) ((j + 1)) 
);    
#pragma endregion

#pragma region LEFT_FACE
glVertex3f(		//Top Left (looking at face)
(float) (i), 
height, 
(float) (j) 
);    
glVertex3f(		//Top Right (looking at face)
(float) (i), 
height, 
(float) ((j + 1)) 
);  

glVertex3f(		//Bottom Right (looking at face)
(float) (i), 
0, 
(float) ((j + 1)) 
);  
glVertex3f(		//Bottom Left (looking at face)
(float) (i), 
0, 
(float) (j) 
);    
#pragma endregion

#pragma region TOP_FACE
glVertex3f(		//Top Right (looking at face)
(float) ((i + 1)), 
height, 
(float) (j) 
);    
glVertex3f(		//Top Left (looking at face)
(float) ((i + 1)), 
height, 
(float) ((j + 1)) 
);  
glVertex3f(		//Bottom Left (looking at face)
(float) ((i + 1)), 
0, 
(float) ((j + 1)) 
);  
glVertex3f(		//Bottom Right (looking at face)
(float) ((i + 1)), 
0, 
(float) (j) 
);    
#pragma endregion

#pragma region FRONT_FACE 
glVertex3f(		//Top Right (looking at face)
(float) ((i + 1)), 
height, 
(float) ((j + 1)) 
);  
glVertex3f(		//Top Left (looking at face)
(float) (i), 
height, 
(float) ((j + 1)) 
);   
glVertex3f(		//Bottom Left (looking at face)
(float) (i), 
0, 
(float) ((j + 1)) 
);  
glVertex3f(		//Bottom Right (looking at face)
(float) ((i + 1)), 
0, 
(float) ((j + 1)) 
);  
#pragma endregion

#pragma region BACK_FACE
glVertex3f(		//Top Right (looking at face)
(float) (i), 
height, 
(float) (j) 
);    
glVertex3f(		//Top Left (looking at face)
(float) ((i + 1)), 
height, 
(float) (j) 
);    
glVertex3f(		//Bottom Left (looking at face)
(float) ((i + 1)), 
0, 
(float) (j) 
);    
glVertex3f(		//Bottom Right (looking at face)
(float) (i), 
0, 
(float) (j) 
);   
#pragma endregion
glEnd();
}
}

glFlush();
glutSwapBuffers();
}
*/