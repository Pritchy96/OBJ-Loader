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

string strArrVerts[6000][5];
string strArrPolys[6000][5];

// display() Callback function
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


  //Front.
    for (int i = 0; i < 6000; i++)
	{
	  //Vertex to draw, gotten from the current poly in strArrPolys.
	  int vertToDraw = 0;

	  glBegin(GL_POLYGON);
	  glColor3f( 0, 0.5, 0.0 );  
	  //Finds the row of the vert to draw in strArrVerts by finding which number strArrPolys is looking for.
	  vertToDraw = (int) atof(strArrPolys[i][0].c_str()) - 1;
	  //Draws Vertex, getting the coords from strArrVerts.
	  glVertex3f( 
		  (float) atof(strArrVerts[vertToDraw] [0].c_str()), 
		  (float) atof(strArrVerts[vertToDraw] [1].c_str()), 
		  (float) atof(strArrVerts[vertToDraw] [2].c_str()) 
		  );      

	  glColor3f( 0, 0, 0.6 );  
	  vertToDraw = (int) atof(strArrPolys[i][1].c_str()) - 1;
	  glVertex3f(
		  (float) atof(strArrVerts[vertToDraw][0].c_str()), 
		  (float) atof(strArrVerts[vertToDraw][1].c_str()), 
		  (float) atof(strArrVerts[vertToDraw][2].c_str()) 
		  );   

	  glColor3f( 0.2, 0, 0.0 );  
	  vertToDraw = (int) atof(strArrPolys[i][2].c_str()) - 1;
	  glVertex3f(
		  (float) atof(strArrVerts[vertToDraw][0].c_str()), 
		  (float) atof(strArrVerts[vertToDraw][1].c_str()), 
		  (float) atof(strArrVerts[vertToDraw][2].c_str()) 
		  );     

	  glColor3f( 0.5, 0, 0.5 );  
	  vertToDraw = (int) atof(strArrPolys[i][3].c_str()) - 1;
	  glVertex3f(
		  (float) atof(strArrVerts[vertToDraw][0].c_str()), 
		  (float) atof(strArrVerts[vertToDraw][1].c_str()), 
		  (float) atof(strArrVerts[vertToDraw][2].c_str()) 
		  );     

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
		scale += 0.01;
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

// main() function
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
  
  //Resize Window
 // glutReshapeWindow(700, 700);

  //Enable Z-buffer depth test. Draws polys in 3d space: 
  //no overlap with polys in front/behind (last drawn polies would be on top)
  glEnable(GL_DEPTH_TEST);

  //Only render polys facing the viewport.
  glEnable(GL_CULL_FACE);
  //Change cull style.
  glCullFace(GL_FRONT);

  //Lighting
  //glEnable(GL_LIGHTING);
  //glEnable(GL_LIGHT1);


  // Callback functions
  glutDisplayFunc(display);
  glutSpecialFunc(specialKeys);

  //  Pass control to GLUT for events
  glutMainLoop();

  //  Return to OS
  return 0;
}

void ReadFile()
{
  ifstream file ("cube.obj");

  string currentLine = "";
  
  if (file.is_open())
  {
	  int vertIncrement = 0;
	  int polyIncrement = 0;

	//While we are not at the end of a file (gets the line from file if not)
    while ( getline (file, currentLine) )
    {
		stringstream stream(currentLine);		//Allows String Manipulation
		int j = 0;		//Row Iterator for both String Arrays.

		#pragma region ADDING_VERTICIES
		//If it finds an v at position 0 of the string (line begins with a v)
		if(currentLine.find("v") == 0) 
		{
			//Split String into string array, if there is a new line.
			while( getline(stream, strArrVerts[vertIncrement][j], ' ') )
			{
				//If it's empty or a v, don't increment so it can be overwritten.
				if (!strArrVerts[vertIncrement][j].empty()
					&& !strArrVerts[vertIncrement][j].find("v") == 0)
				{
					//Increment j so we put the next value in the next column.
					j++;
				}
			}
				/*Debug.
				cout << "Vert" << endl;
				cout << strArrVerts[vertIncrement][0] <<  endl;
				cout << strArrVerts[vertIncrement][1] << endl;
				cout << strArrVerts[vertIncrement][2] << "\n" << endl;
				*/

				//Increment vertIncrement so we put the next split in the next array row.
				vertIncrement++;
		}

		#pragma endregion

		#pragma region ADDING_POLYGONS

		//If it finds an f at position 0 of the string (line begins with a v)
		else if(currentLine.find("f") == 0) 
		{
			//Split String into string array, if there is a new line.
			while( getline(stream, strArrPolys[polyIncrement][j], ' ') )
			{
				//If it's empty or a v, don't increment so it can be overwritten.
				if (!strArrPolys[polyIncrement][j].empty()
					&& !strArrPolys[polyIncrement][j].find("f") == 0)
				{
					//Increment j so we put the next value in the next column.
					j++;
				}
			}
				/*Debug.
				cout << "Poly" << endl;
				cout << strArrPolys[polyIncrement][0] <<  endl;
 				cout << strArrPolys[polyIncrement][1] <<  endl;
				cout << strArrPolys[polyIncrement][2] << endl;
				cout << strArrPolys[polyIncrement][3] << "\n" << endl;
				*/

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
