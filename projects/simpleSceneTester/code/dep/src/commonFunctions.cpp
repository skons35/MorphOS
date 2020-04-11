
#include "commonFunctions.h"



float vertPosTri_1[9] = { 0.0f,  0.0f, -5.0f, 		2.0f, 0.0f, -5.0f, 		0.0f, 2.0f, -5.0f };
// fixed to be counter wise order two :
float vertPosTri_2[9] = { -3.0f, -3.0f, -8.0f,	   0.0f, -3.0f, -8.0f,      -3.0f, 1.0f, -8.0f };

// Blue :
float vertColTri_1[9] = { 0.0f,  0.0f, 1.0f,	 0.0f, 0.0f, 1.0f,	    0.0f,  0.0f, 1.0f };
// Yellow :
float vertColTri_2[9] = { 1.0f,  1.0f, 0.0f,	 1.0f, 1.0f, 0.0f,	    1.0f,  1.0f, 0.0f };


/*
bool pseudoFunction()
{
std::cout << "	pseudoFunction called" << std::endl;
 return true; 
}
*/

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		45,
		float(width) / float(height),
		0.1,
		100
	); 	//Pour les explications, lire le tutorial sur OGL et win
	glMatrixMode(GL_MODELVIEW); 	//Optionnel
}

void Draw()
{
	//VA : add DEPTH testing (for Z Buffer use and correct 3D Objects layout)
	glEnable( GL_DEPTH_TEST); // masked object are not drawn

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); 	//select MODELVIEW matrix
	glLoadIdentity(); 	//set as Identity
	//gluLookAt(0, 0, -10, 0, 0, 0, 0, 1, 0);


	glEnableClientState( GL_VERTEX_ARRAY);
	glEnableClientState( GL_COLOR_ARRAY);
	// 1st triangle
	glVertexPointer( 3, GL_FLOAT, 0, vertPosTri_1);
	glColorPointer( 3, GL_FLOAT, 0, vertColTri_1);
	glDrawArrays( GL_TRIANGLES, 0, 3); // draw 1st triangle
	// 2nd triangle
	glVertexPointer( 3, GL_FLOAT, 0, vertPosTri_2);
	glColorPointer( 3, GL_FLOAT, 0, vertColTri_2);
	glDrawArrays( GL_TRIANGLES, 0, 3); // draw 2nd triangle	
	glDisableClientState( GL_COLOR_ARRAY);
	glDisableClientState( GL_VERTEX_ARRAY);


	glutSwapBuffers(); // swap the buffers using GLUT
}