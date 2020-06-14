
#include "commonFunctions.h"


///*

float vertPosTri_1[9] = { 0.0f,  0.0f, -5.0f, 		2.0f, 0.0f, -5.0f, 		0.0f, 2.0f, -5.0f };
// fixed to be counter wise order two :
float vertPosTri_2[9] = { -3.0f, -3.0f, -8.0f,	   0.0f, -3.0f, -8.0f,      -3.0f, 1.0f, -8.0f };

// Blue :
float vertColTri_1[9] = { 0.0f,  0.0f, 1.0f,	 0.0f, 0.0f, 1.0f,	    0.0f,  0.0f, 1.0f };
// Yellow :
float vertColTri_2[9] = { 1.0f,  1.0f, 0.0f,	 1.0f, 1.0f, 0.0f,	    1.0f,  1.0f, 0.0f };
//*/



ObjLoader* DrawControl::objLoader = nullptr;

static float rotAngle = 0.0f;
static float rotDir = 1.0f; // +1 or -1

void DrawControl::Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(
		45,
		float(width) / float(height),
		0.1,
		100
	); 	
	
	// set back the matrix to ModelView
	glMatrixMode(GL_MODELVIEW); 	//Option
	glLoadIdentity();              //
}

void DrawControl::Draw()
{
	//VA : add DEPTH testing (for Z Buffer use and correct 3D Objects layout)
	glEnable( GL_DEPTH_TEST); // masked object are not drawn

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); 	//select MODELVIEW matrix
	glLoadIdentity(); 	//set as Identity
	
	//gluLookAt(0, 0, -10, 0, 0, 0, 0, 1, 0);

	//Test : adjust the location of object rendering  ( to control placement distance from camera) :
	float xPos = 0.0f, yPos = 0.0f, zPos = -6.0f;
	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);

	// also introduce slow object(s) rotation (ON/OFF controlled per mouse button clicks)
	static float xRot = 0.0f, yRot = 1.0f, zRot = 0.0f;
	glRotatef(rotAngle, xRot, yRot, zRot);	

	///*
	if (DrawControl::objLoader != nullptr)
	{
		vector<obj::vec3> objVertices;
		//
		vector<uint8_t> objVertIndices;
		// optional presence :
		vector<obj::vec3> objNormals;  // not used yet
		// optional presence :
		vector<obj::uv> objUvs; // not used yet

		// optional presence :
		vector<mtl::Material> objMats; // not used yet

		if (! DrawControl::objLoader->getParsedObjData( objVertices, objVertIndices,
														objNormals, objUvs, objMats)
			)
			cout << endl << "No data get from ObjLoader ....";
		else 
		{
			// 3 cases : 
			//   (A)  : just obj file => no provided (matrial) color(s), just vertices (+ normals possibly)
			//   (B)  : obj + mtl file => provided material(s) color, 
			//                            with vertices (+ normals possibly)
			//                            but without UVs information
			//   (C)  : obj + MTL(s) + text. file(s) ==> provided material as texture(s) file, with UVs,
			//                                           with vertices (+ normals possibly)

			// tempo debug :
			//cout << endl << "Provided OBJ vertices : " << objVertices.size() << ", vert. indices : " << objVertIndices.size()
			//	<< ", UVs : " << objUvs.size() << ", material(s) : " << objMats.size();

			// process cases amongst 3
			if (objMats.empty())  
			 {	
				// CASE A

				cout << endl << "CASE A : no material(s), use a common default RED COLOR for all vertices...";
				cout << endl << "Provided OBJ vertices : " << objVertices.size() << ", and vert. indices : " << objVertIndices.size();
				glEnableClientState(GL_VERTEX_ARRAY);					
				glVertexPointer(3, GL_FLOAT, 0, &objVertices[0]);					
				// tempo : assign ONE unique color for obj to be drawn :
				glColor3f(1.0f, 0.0f, 0.0f); // RED
				// Using array of vertex indices
				glDrawElements(GL_TRIANGLES, (GLsizei)objVertIndices.size(), GL_UNSIGNED_BYTE, &objVertIndices[0]);
				glDisableClientState(GL_VERTEX_ARRAY);
			 }
			else if(objUvs.empty()) 
			{
				// CASE B 

				cout << endl << "CASE B : material(s) color(s) defined, no texture(s) ...";
				cout << endl << "Provided OBJ vertices : " << objVertices.size() << ", vert. indices : " << objVertIndices.size()
					<< ", material(s) : " << objMats.size();
				
				//
				// TO BE FINISHED : 				
				//

				// VA : lot of rewrite needed, at both the OBJ & MTL reading part :
				//      as we need to maintain a set of vertices, per related material
				cout << endl << "WRITE ME !!!!"; 
			}
			else
			{
				// CASE C 

				cout << endl << "CASE C : material(s) color(s) defined, using texture(s) ...";
				cout << endl << "Provided OBJ vertices : " << objVertices.size() << ", vert. indices : " << objVertIndices.size()
					<< ", UVs : " << objUvs.size() << ", material(s) : " << objMats.size();
				
				//
				// TO BE FINISHED : 				
				//

				// VA : lot of rewrite needed, at both the OBJ & MTL reading part :
				//      as we need to maintain a set of vertices, per related material
				cout << endl << "WRITE ME !!!!";
			}
			

			/*
			//cout << endl << "Provided OBJ vertices : " << objVertices.size() << ", and vert. indices : " << objVertIndices.size();;

			glEnableClientState(GL_VERTEX_ARRAY);
			//glEnableClientState(GL_COLOR_ARRAY);
			// 
			glVertexPointer(3, GL_FLOAT, 0, &objVertices[0]);
			//glColorPointer(3, GL_FLOAT, 0, vertCols);

			// tempo : assign ONE unique color for obj to be drawn :
			glColor3f(1.0f, 0.0f, 0.0f); // RED
			
			//glDrawArrays(GL_TRIANGLES, 0, (GLsizei) 2 * 3); // objVertices.size()); // draw 1st triangle

			// Using array of vertex indices
			glDrawElements(GL_TRIANGLES, (GLsizei)objVertIndices.size(), GL_UNSIGNED_BYTE, &objVertIndices[0]);

			//glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
			*/
		}

	}
	//*/

	/*
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
	*/

	glPopMatrix();

	glutSwapBuffers(); // swap the buffers using GLUT
}

void DrawControl::triggerObjectRotationThenDisplay()
{

	rotAngle += rotDir * 1.0f;

	if (rotAngle >= 360.0f)
		rotAngle -= 360.0f;
	else if (rotAngle <= 0.0f)
		rotAngle += 360.0f;

	// trigger scene re-draw  (i.e. update);
	glutPostRedisplay();
}

void DrawControl::mouseCommands(int mButton, int bStatus, int x, int y)
{
	//cout << endl << "Mouse button pressed  : " << mButton << ", status : " << bStatus << ", x/y : " << x << "/" << y;

	// Let's say we do actions only when buttons are released (NOT pressed down):
	if (bStatus != GLUT_UP)
		return;

	switch(mButton)
	{
		case GLUT_LEFT_BUTTON:
			// triggers automatic rotation & scene update
			glutIdleFunc(triggerObjectRotationThenDisplay); 
			break;
		case GLUT_RIGHT_BUTTON:
			rotDir = - rotDir; // inverse rotation
			break;
		default: // middle button, ...
			glutIdleFunc(nullptr); // stop auto rotation & update
	}
}