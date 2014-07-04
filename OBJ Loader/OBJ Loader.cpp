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

// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
double rotate_y=0;
double rotate_x=0;
double rotate_z=0;
double scale = 0.5;

static float lightPos[] = {1.0, 0.0, 1.0, 1.0}; 

//2D vectors holding vert and poly data.
vector<vector<string>> strVcrVerts;
vector<vector<string>> strVcrPolys;

bool debug = false;

//	-------------------------------------------------------
// display() Callback function
//	-------------------------------------------------------
void display(){

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

	//Iterate until we have completeted all polygons.
	for (int i = 0; i < strVcrPolys.size(); i++)
	{
		//Vertex to draw, gotten from the current poly in strVcrPolys.
		int vertToDraw = 0;

		glBegin(GL_POLYGON);

		for (int j = 0; j < strVcrPolys[i].size(); j++)
		{
			glColor3f( 1/(j - 0.5), 1/(0.4 + j), 1/(j - 0.3) );  

			//Finds the row of the vert to draw in strArrVerts by finding which number strArrPoly is looking for.
			vertToDraw = (int) atof(strVcrPolys[i][j].c_str()) - 1;
			//Draws Vertex, getting the coords from strArrVerts.
			glVertex3f( 
				(float) atof(strVcrVerts[vertToDraw][0].c_str()), 
				(float) atof(strVcrVerts[vertToDraw][1].c_str()), 
				(float) atof(strVcrVerts[vertToDraw][2].c_str()) 
				);    
		}


		glEnd();
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
		rotate_z += 5;
		break;
	case (GLUT_KEY_LEFT):
		rotate_z -= 5;
		break;
	case (GLUT_KEY_UP):
		rotate_x += 5;
		break;
	case (GLUT_KEY_DOWN):
		rotate_x -= 5;
		break;
	case (GLUT_KEY_PAGE_UP):
		scale += 0.05;
		break;
	case (GLUT_KEY_PAGE_DOWN):
		scale -= 0.01;
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

	//Read .OBJ file.
	ReadFile();

	//  Initialize GLUT and process user parameters
	glutInit(&argc,argv);

	//  Request double buffered true color window with Z-buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize (700, 700); 
	glutInitWindowPosition (100, 100);

	// Create window
	glutCreateWindow("OBJ Loader");
	// glutFullScreen();

	//Enable Z-buffer depth test. Draws polys in 3d space: 
	//no overlap with polys in front/behind (last drawn polies would be on top)
	glEnable(GL_DEPTH_TEST);

	
		
	//Only render polys facing the viewport.
	//glEnable(GL_CULL_FACE);
	//Change cull style.
	//glCullFace(GL_FRONT);

	//Lighting
	//glEnable(GL_LIGHTING);
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
	


	// Callback functions
	glutDisplayFunc(display);
	glutSpecialFunc(specialKeys);

	//  Pass control to GLUT for events
	glutMainLoop();

	//  Return to OS
	return 0;
}

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


