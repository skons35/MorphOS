// SceneTester.cpp : Defines the entry point for the application.
//



#include "commonFunctions.h"

#include <iostream>



using namespace std;



int main(int argc, char *argv[])
{
	cout << "SceneTester started." << endl;

	int WindowName;	
	
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);  //Optional
	WindowName = glutCreateWindow("SceneTester");
	
	//Full Screen switching (option)
	//glutFullScreen();

	glutReshapeFunc(Reshape);
	glutDisplayFunc(Draw);

	//InitGL();

	glutMainLoop();

 
return 0;
}
