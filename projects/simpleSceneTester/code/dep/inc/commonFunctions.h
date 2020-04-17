#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H

#include <iostream>

#include "ObjLoader.h"

// TODO: Reference additional headers your program requires here.

//#include <gl/freeglut.h>
//#include <gl/freeglut_ext.h>

//#include <gl/gl.h>
#include <gl/glut.h>


//bool pseudoFunction();

class DrawControl 
{
	public :

		static ObjLoader *objLoader;

		static void Reshape(int width, int height);
		static void Draw();

		static void triggerObjectRotationThenDisplay();
		static void mouseCommands(int button, int state, int x, int y);

};


#endif