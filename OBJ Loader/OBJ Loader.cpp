// OBJ Loader.cpp : Defines the entry point for the console application.
//



// ----------------------------------------------------------
// Includes 
// ----------------------------------------------------------
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include "Fractal_Creator.h"
#include "EasyBMP.h"
#include "Island_Utils.h"
#include <GLFW/glfw3.h>

#include <stdarg.h>
#include <math.h>
using namespace std;


// ----------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------C:\Windows\SysWOW64
void display();
void ReadFile();
int main(int argc, char* argv[]);
int wasMain();
void SaveImages();
void Render3D();

// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
double rotate_y=0;
double rotate_x=0;
double rotate_z=0;
double scale = 0.01;


const int width = 2000, height = 2000;
vector<vector<int>> islandFractal, islandShape, temperateFractal, heightFractal, rainFractal, islandColoured, gradientMap;



// ----------------------------------------------------------
// main() function
// ----------------------------------------------------------
int main(int argc, char* argv[]){
	Util::SeedGenerator(0);
	Fractal_Creator maker = Fractal_Creator();

	cout << "Generating Island Fractal" << endl;
	islandFractal = maker.MakeFractal(width, height, 12);

	cout << "Generating Island Shape" << endl;
	islandShape = Island_Utils::ShapeIsland(&islandFractal);

	cout << "Generating Temperate Fractal" << endl;
	temperateFractal = maker.MakeFractal(width, height, 4, 100, 0);

	cout << "Generating Rain Fractal" << endl;
	rainFractal = maker.MakeFractal(width, height, 4, 100, 0);

	cout << "Generating Gradient Map" << endl;
	gradientMap = Island_Utils::MakeCircularGradient(width, height, 255, 0);

	cout << "Generating Height Fractal" << endl;
	heightFractal = maker.MakeFractal(width, height, 12);

	cout << "Interpolating Height Fractal with Gradient Map" << endl;
	heightFractal = Island_Utils::InterpolateBitmaps(&heightFractal, &gradientMap, 0.8, 1);

	cout << "Interpolating TemperateFractal with Gradient Map" << endl;
	temperateFractal = Island_Utils::InterpolateBitmaps(&temperateFractal, &gradientMap, 1, 1, 0);
	
	cout << "Calculating Biomes" << endl;
	islandColoured = Island_Utils::CalculateBiomes(&islandFractal, &islandShape, &heightFractal, &temperateFractal, &rainFractal);

	//SaveImages();
	Render3D();
	//  Return to OS
	return 0;
}


void SaveImages()
{
	cout << "Saving IslandFractal.bmp" << endl;
	Island_Utils::SaveImage(&islandFractal, "IslandFractal.bmp");

	cout << "Saving IslandShape.bmp" << endl;
	Island_Utils::SaveImage(&islandShape, "IslandShape.bmp");

	cout << "Saving TemperateFractal.bmp" << endl;
	Island_Utils::SaveImage(&temperateFractal, "TemperateFractal.bmp");

	cout << "Saving RainFractal.bmp" << endl;
	Island_Utils::SaveImage(&rainFractal, "RainFractal.bmp");

	cout << "Saving HeightFractal.bmp" << endl;
	Island_Utils::SaveImage(&heightFractal, "HeightFractal.bmp");

	cout << "Saving GradientMap.bmp" << endl;
	Island_Utils::SaveImage(&gradientMap, "GradientMap.bmp");

	cout << "Saving IslandColoured.bmp" << endl;
	Island_Utils::SaveBiomeImage(&islandColoured, "IslandColoured.bmp");
}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void Render3D()
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	
	if (!glfwInit())
		exit(EXIT_FAILURE);
	
	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);

	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-0.6f, -0.4f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.6f, -0.4f, 0.f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.6f, 0.f);
		glEnd();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}



/*
//	-------------------------------------------------------
// Readfile() Function
//	-------------------------------------------------------
void ReadFile()
{
ifstream file ("torus knot HD.obj");

string currentLine = "";

if (file.is_open())
{
int vertIncrement = 0;
int polyIncrement = 0;

//While we are not at the end of a file (gets the line from file if not)
while ( getline (file, currentLine) )
{
stringstream stream(currentLine);		//Allows String Manipulation
int j = 0;		//Column Iterator for both String Arrays.

vector<string> split ;	//Reciprocal for split value for inserting into Vector.
split.resize(1, "");

#pragma region ADDING_VERTICIES
//If it finds an v at position 0 of the string (line begins with a v)
if(currentLine.find("v") == 0) 
{
//Split String into string array, if there is a new line.
while( getline(stream, split[j], ' ') )
{
//If it's empty or a v, don't increment so it can be overwritten.
if (!split[j].empty() && 
!split[j].find("v") == 0)
{
//Add a new cell to split to be written to.
split.push_back("");
//Increment j so we put the next value in the next column.
j++;
}
}

//Removes the empty string that was added but not filled.
split.pop_back();
//Adds the finished 'split' to end of strVcrVerts.
strVcrVerts.push_back(split);

//Debug.
if (debug)
{
cout << "Vert" << endl;
cout << strVcrVerts[vertIncrement][0] <<  endl;
cout << strVcrVerts[vertIncrement][1] << endl;
cout << strVcrVerts[vertIncrement][2] << "\n" << endl;
}

//Increment vertIncrement so we put the next split in the next array row.
vertIncrement++;
}
#pragma endregion

#pragma region ADDING_POLYGONS
//If it finds an f at position 0 of the string (line begins with a v)
else if(currentLine.find("f") == 0) 
{
//Split String into string array, if there is a new line.
while( getline(stream, split[j], ' ') )
{
//If it's empty or a v, don't increment so it can be overwritten.
if (!split[j].empty() && 
!split[j].find("f") == 0)
{
//Add a new cell to split to be written to.
split.push_back("");
//Increment j so we put the next value in the next column.
j++;
}
}

//Removes the empty string that was added but not filled.
split.pop_back();
//Adds the finished 'split' to end of strVcrVerts.
strVcrPolys.push_back(split);

//Debug.
if (debug)
{
cout << "Poly" << endl;
for (int j = 0; j < strVcrPolys[polyIncrement].size(); j++)
{
cout << strVcrPolys[polyIncrement][j] <<  endl;
}
cout << "\n" << endl;
}

//Increment vertIncrement so we put the next split in the next array row.
polyIncrement++;
}
#pragma endregion
}
//We now have a completed file load.
file.close();
}

else cout << "Unable to open file"; 
}
*/

/*
//	-------------------------------------------------------
// display() Callback function
//	-------------------------------------------------------
void display(){


#pragma region LOADING_IMAGE
BMP heightmap;
heightmap.ReadFromFile("heightmap.bmp");
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
for (int i = 0; i < heightmap.TellWidth(); i++)
{
for (int j = 0; j < heightmap.TellHeight(); j++)
{
//Pixel Height/RBG Value
int pixelHeight = heightmap.GetPixel(i, j).Blue;

glBegin(GL_POLYGON);	//Must be called for each face.

//X = Width = i
//Y = Height = pixelHeight
//Z = Depth = j.


//Top Left (From Topdown)
glColor3f( 1/(0 - 0.5), 1/(0.4 + 0), 1/(0 - 0.3) );  //Colour so faces can be differentiated.
//Draws Vertex, getting the coords from strArrVerts.
glVertex3f( 
(float) (i * 10), 
(float) (pixelHeight), 
(float) (j * 10) 
);    

//Top Right (From Topdown)
glColor3f( 1/(1 - 0.5), 1/(0.4 + 1), 1/(1 - 0.3) );  //Colour so faces can be differentiated.
//Draws Vertex, getting the coords from strArrVerts.
glVertex3f( 
(float) ((i + 1) * 10), 
(float) (pixelHeight), 
(float) (j * 10) 
);    

//Bottom Left (From Topdown)
glColor3f( 1/(2 - 0.5), 1/(0.4 + 2), 1/(2 - 0.3) );  //Colour so faces can be differentiated.
//Draws Vertex, getting the coords from strArrVerts.
glVertex3f( 
(float) (i * 10), 
(float) (pixelHeight), 
(float) ((j + 1) * 10) 
);    

//Bottom Right (From Topdown)
glColor3f( 1/(3 - 0.5), 1/(0.4 + 3), 1/(3 - 0.3) );  //Colour so faces can be differentiated.
//Draws Vertex, getting the coords from strArrVerts.
glVertex3f( 
(float) ((i + 1) * 10), 
(float) (pixelHeight), 
(float) ((j + 1) * 10) 
);   

glEnd();
}
}

glFlush();
glutSwapBuffers();
}
*/