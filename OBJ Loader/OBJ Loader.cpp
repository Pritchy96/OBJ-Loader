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
// ----------------------------------------------------------C:\Windows\SysWOW64\
void display();
void specialKeys();
void ReadFile();

// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
double rotate_y=0;
double rotate_x=0;
string splitString[500][5];

// ----------------------------------------------------------
// display() Callback function
// ----------------------------------------------------------
void display(){

  //  Clear screen and Z-buffer
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  // Reset transformations
  glLoadIdentity();

  // Other Transformations
  // glTranslatef( 0.1, 0.0, 0.0 );      // Not included
  // glRotatef( 180, 0.0, 1.0, 0.0 );    // Not included

  // Rotate when user changes rotate_x and rotate_y
  glRotatef( rotate_x, 1.0, 0.0, 0.0 );
  glRotatef( rotate_y, 0.0, 1.0, 0.0 );

  // Other Transformations
   glScalef( 0.5, 0.5, 0.5 );          // Not included

  //Front.
  glBegin(GL_POLYGON);

  glColor3f( 0, 0.5, 0.0 );   

  
  glVertex3f((float) atof(splitString[1][2].c_str()), (float) atof(splitString[1][3].c_str()), (float) atof(splitString[1][4].c_str()) );      
  glVertex3f((float) atof(splitString[2][2].c_str()), (float) atof(splitString[2][3].c_str()), (float) atof(splitString[2][4].c_str()) );   
  glVertex3f((float) atof(splitString[3][2].c_str()), (float) atof(splitString[3][3].c_str()), (float) atof(splitString[3][4].c_str()) );     
  glVertex3f((float) atof(splitString[4][2].c_str()), (float) atof(splitString[4][3].c_str()), (float) atof(splitString[4][4].c_str()) );     

cout << splitString[0][2] << endl;
cout << splitString[0][3] << endl;
cout << splitString[0][4] << "\n" << endl;

  glEnd();

  glFlush();
  glutSwapBuffers();

}

// ----------------------------------------------------------
// specialKeys() Callback Function
// ----------------------------------------------------------
void specialKeys( int key, int x, int y ) {

  //  Right arrow - increase rotation by 5 degree
  if (key == GLUT_KEY_RIGHT)
    rotate_y += 5;

  //  Left arrow - decrease rotation by 5 degree
  else if (key == GLUT_KEY_LEFT)
    rotate_y -= 5;

  else if (key == GLUT_KEY_UP)
    rotate_x += 5;

  else if (key == GLUT_KEY_DOWN)
    rotate_x -= 5;

  //  Request display update
  glutPostRedisplay();

}

// ----------------------------------------------------------
// main() function
// ----------------------------------------------------------
int main(int argc, char* argv[]){

	ReadFile();

  //  Initialize GLUT and process user parameters
  glutInit(&argc,argv);

  //  Request double buffered true color window with Z-buffer
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  // Create window
  glutCreateWindow("Awesome Cube");

  //  Enable Z-buffer depth test
  glEnable(GL_DEPTH_TEST);

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
  ifstream file ("plane.obj");

  string currentLine = "";
  
  if (file.is_open())
  {
	  int i = 0;

	//While we are not at the end of a file (gets the line from file if not)
    while ( getline (file, currentLine) )
    {
		//If this line begins with a V..
		if(0 == currentLine.find("v")) 
		{
			i++;
			stringstream stream(currentLine);
			int j = 0;

			//Split String into string array, if there is a new line.
			while( getline(stream, splitString[i][j], ' ') )
			{
				//Increment i so we put the next split in the next array index.
				j++;
			}

			//We now have a completed string array.
				

				cout << splitString[i][2] << endl;
				cout << splitString[i][3] << endl;
				cout << splitString[i][4] << "\n" << endl;
		}
    }
    file.close();
  }

  else cout << "Unable to open file"; 
}
