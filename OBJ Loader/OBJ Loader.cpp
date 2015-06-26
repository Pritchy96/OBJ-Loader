// OBJ Loader.cpp : Defines the entry point for the console application.
//



// ----------------------------------------------------------
// Includes 
// ----------------------------------------------------------
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Fractal_Creator.h"
#include "EasyBMP.h"
#include "Island_Utils.h"


#include <stdarg.h>
#include <math.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;


// ----------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------C:\Windows\SysWOW64
void display();
void specialKeys();
void ReadFile();
int main(int argc, char* argv[]);
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


const int width = 200, height = 200;
vector<vector<int>> islandFractal, islandShape, temperateFractal, heightFractal, rainFractal, islandColoured, gradientMap;

//	-------------------------------------------------------
// display() Callback function
//	-------------------------------------------------------
void display(){

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
	for (int i = -(width / 2); i < (width/2); i++)	//Goes from -100 to + 100 rather than 0 - 200 so camera is centered.
	{
		for (int j = -(height / 2); (j < height / 2); j++) //As above. This means some statements below use (+ heightmap.TellHeight()/2) to make it 0 - 200 again.
		{
			int iFromZero = i + (width/2);
			int jFromZero = j + (height/2);

			//Pixel Height/RBG Value
			int pixelHeight = heightFractal[iFromZero][jFromZero];

			glBegin(GL_POLYGON);	//Must be called for each face.

			//X = Width = i
			//Y = Height = pixelHeight
			//Z = Depth = j

		vector<int> colour = vector<int>();
		colour.resize(3);
		colour = Island_Utils::GetBiomeColour(islandColoured[iFromZero][jFromZero]);

			glColor3f( 
				(float) (((float)colour[0] + (heightFractal[iFromZero][jFromZero]/5))/255), 
				(float) (((float)colour[1] + (heightFractal[iFromZero][jFromZero]/5))/255), 
				(float) (((float)colour[2] + (heightFractal[iFromZero][jFromZero]/5))/255));

			float height = ((float)300- pixelHeight)/10;

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

// ----------------------------------------------------------
// specialKeys() Callback Function
// ----------------------------------------------------------
void specialKeys( int key, int x, int y ) {

	//Increase/Decrease Rotation. (degrees)
	switch (key)
	{
	case (GLUT_KEY_RIGHT):
		rotate_z += 10;
		break;
	case (GLUT_KEY_LEFT):
		rotate_z -= 10;
		break;
	case (GLUT_KEY_UP):
		rotate_x += 10;
		break;
	case (GLUT_KEY_DOWN):
		rotate_x -= 10;
		break;
	case (GLUT_KEY_HOME):
		rotate_y += 10;
		break;
	case (GLUT_KEY_END):
		rotate_y -= 10;
		break;
	case (GLUT_KEY_PAGE_UP):
		scale += 0.01;
		break;
	case (GLUT_KEY_PAGE_DOWN):
		scale -= 0.005;
		break;
	case (GLUT_KEY_F4):
		glutDestroyWindow(0);
		exit(0);
		break;
	}
	//  update display.
	glutPostRedisplay();

} 

// ----------------------------------------------------------
// main() function
// ----------------------------------------------------------
int main(int argc, char* argv[]){
	Util::SeedGenerator(0);
	Fractal_Creator maker = Fractal_Creator();

	islandFractal = maker.MakeFractal(width, height, 12);
	islandShape = Island_Utils::ShapeIsland(&islandFractal);

	temperateFractal = maker.MakeFractal(width, height, 4, 100, 0);
	rainFractal = maker.MakeFractal(width, height, 4, 100, 0);
	gradientMap = Island_Utils::MakeCircularGradient(width, height, 255, 0);

	heightFractal = maker.MakeFractal(width, height, 12); 
	heightFractal = Island_Utils::InterpolateBitmaps(&heightFractal, &gradientMap, 0.5, 1);
	temperateFractal = Island_Utils::InterpolateBitmaps(&temperateFractal, &gradientMap, 1, 1, 0);
	
	islandColoured = Island_Utils::CalculateBiomes(&islandFractal, &islandShape, &heightFractal, &temperateFractal, &rainFractal);


	//  Initialize GLUT and process user parameters
	glutInit(&argc,argv);

	//  Request double buffered true color window with Z-buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize (900, 900); 
	glutInitWindowPosition (0, 0);

	// Create window
	glutCreateWindow("OBJ Loader");
	// glutFullScreen();

	//Enable Z-buffer depth test. Draws polys in 3d space: 
	//no overlap with polys in front/behind (last drawn polies would be on top)
	glEnable(GL_DEPTH_TEST);

	//Only render polys facing the viewport. Creates graphical glitches.
	//glEnable(GL_CULL_FACE);
	//Change cull style.
	//glCullFace(GL_FRONT);
	//glEnable(GL_LIGHTING);
	/*Lighting
	//
	glEnable(GL_LIGHT0);

	// Create light components.
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat position[] = { 0.0f, 2.0f, 1.0f, 1.0f };

	// Assign created components to GL_LIGHT0.
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	*/

	// Callback functions
	glutDisplayFunc(display);
	glutSpecialFunc(specialKeys);

	//  Pass control to GLUT for events
	glutMainLoop();

	//  Return to OS
	return 0;
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