
#include "glDrawControl.h"


/*

float vertPosTri_1[9] = { 0.0f,  0.0f, -5.0f, 		2.0f, 0.0f, -5.0f, 		0.0f, 2.0f, -5.0f };
// fixed to be counter wise order two :
float vertPosTri_2[9] = { -3.0f, -3.0f, -8.0f,	   0.0f, -3.0f, -8.0f,      -3.0f, 1.0f, -8.0f };

// Blue :
float vertColTri_1[9] = { 0.0f,  0.0f, 1.0f,	 0.0f, 0.0f, 1.0f,	    0.0f,  0.0f, 1.0f };
// Yellow :
float vertColTri_2[9] = { 1.0f,  1.0f, 0.0f,	 1.0f, 1.0f, 0.0f,	    1.0f,  1.0f, 0.0f };
*/



//ObjLoader* DrawControl::objLoader = nullptr;

std::vector<LoadedObject> DrawControl::loadedObjects = vector<LoadedObject>();

static float rotAngle = 0.0f;
static float rotDir = 1.0f; // +1 or -1


bool DrawControl::storeLoadedObject(ObjLoader objLoader)
{
	cout << endl << "storeLoadedObject : BEFORE adding object, storage element(s) : " << loadedObjects.size();

	LoadedObject lo;

	// try acquiring all loaded data
	if (!objLoader.getParsedObjData(lo.objVertices, lo.objNormals, lo.objUvs,
		lo.objMats, lo.objMatFaces)
		)
	{
		cout << endl << "FAILed to get valid data get from ObjLoader .... Skipped.";
		return false;
	}
	
	// add object to local storage if success
	loadedObjects.push_back(lo);

	cout << endl << "storeLoadedObject : AFTER adding object, storage element(s) : " << loadedObjects.size();

	return true;
}

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
	//static float xRot = 0.0f, yRot = 1.0f, zRot = 0.0f;
	static float xRot = 0.2f, yRot = 1.0f, zRot = 0.2f; // set small rot. for all axis
	glRotatef(rotAngle, xRot, yRot, zRot);	

	// iterate on every stored object, and render it
	for(int idx = 0; idx < loadedObjects.size();idx++)	
	{		
		LoadedObject & lo = loadedObjects[idx];
				
		// 3 cases : 
		//   (A)  : just obj file => no provided (material) color(s), just vertices (+ normals possibly) and faces
		//                           THEN a default material MUST be created & used BELOW
		//   (B)  : obj + mtl file => provided material(s) color, 
		//                            with vertices (+ normals possibly)
		//                            but without UVs information
		//   (C)  : obj + MTL(s) + text. file(s) ==> provided material as texture(s) file, with UVs,
		//                                           with vertices (+ normals possibly)

		// tempo debug display :
		cout << endl << "DRAW() : Stored OBJ #"<< idx <<" > vertices : " << lo.objVertices.size()
						<< ", normals : " << lo.objNormals.size() << ", UVs : " << lo.objUvs.size()
						<< ", material(s) : " << lo.objMats.size() << ", set(s) of face(s) : " << lo.objMatFaces.size();

		// process cases amongst 3
		if (lo.objMats.empty())
			{	
			// CASE A

			cout << endl << "CASE A : no material(s), use a common default RED COLOR for all vertices...";
				
			glEnableClientState(GL_VERTEX_ARRAY);					
			glVertexPointer(3, GL_FLOAT, 0, &lo.objVertices[0]);
			// tempo : simply assign ONE unique color for obj to be drawn :
			glColor3f(1.0f, 0.0f, 0.0f); // RED

			// we could assume to find only ONE set of face(s), all using that DEFAULT "material"
			// but just in case parse as if in gneral case (multiple mat, so multiple faces' set
			//cout << endl << "--Set of faces to draw :" << lo.objMatFaces.size();
			for (int i = 0; i < lo.objMatFaces.size(); i++)
			{
				vector<obj::Face>& matFaces = lo.objMatFaces[i];
				//cout << endl << "-set #" << i << " ,contains " << matFaces.size() << " face(s) to draw";
				for (int f = 0; f < matFaces.size(); f++)
				{
					obj::Face & face = matFaces[f];		

					//cout << endl << "vertex indice(s) to load :" << face.vertexIndices.size() << " >>> ";
					//for (int vi = 0; vi < face.vertexIndices.size(); vi++)
					//	cout << face.vertexIndices[vi] << " ";

					if (3 == face.vertexIndices.size())
					{
						// Using array of vertex indices defining that face(s) set (Note : provided as vector of INT !)
						glDrawElements(GL_TRIANGLES, (GLsizei)face.vertexIndices.size(), GL_UNSIGNED_INT, (GLvoid*)&face.vertexIndices[0]);
					}
					else if (4 == face.vertexIndices.size())
					{
						// Using array of vertex indices defining that face(s) set (Note : provided as vector of INT !)
						glDrawElements(GL_QUADS, (GLsizei)face.vertexIndices.size(), GL_UNSIGNED_INT, (GLvoid*)&face.vertexIndices[0]);
					}
					else
					{
						//cout << endl << "Drawing face as a polygon, as described per : " << face.vertexIndices.size() << " vertex indexes";
						// Using array of vertex indices defining that face(s) set (Note : provided as vector of INT !)
						glDrawElements(GL_POLYGON, (GLsizei)face.vertexIndices.size(), GL_UNSIGNED_INT, (GLvoid*)&face.vertexIndices[0]);
					}
				}
			}

			glDisableClientState(GL_VERTEX_ARRAY);
			}
		else if(lo.objUvs.empty())
		{
			// CASE B 

			cout << endl << "CASE B : material(s) color(s) defined, no texture(s) ...";

			glEnableClientState(GL_VERTEX_ARRAY);
				
			glVertexPointer(3, GL_FLOAT, 0, &lo.objVertices[0]); // the COMPLETE list of vertices is used for each draxing iteration below

			for(int m=0; m< lo.objMats.size(); m++)
			{
				mtl::rgb& matKd = lo.objMats[m].Kd;
				// first assign color using material one (simple approach : we use Kd)
				glColor3f(matKd.r, matKd.g, matKd.b);

				// per construction, for each defined material; a matching vector of faces exist; 
				// ... so render it with related material
				vector<obj::Face>& matFaces = lo.objMatFaces[m]; // remember : set of faces with index m is related to material with index m
				//cout << endl << "-set #" << m << " ,contains " << matFaces.size() << " face(s) to draw";
				for (int f = 0; f < matFaces.size(); f++)
				{
					obj::Face& face = matFaces[f];

					//cout << endl << "vertex indice(s) to load :" << face.vertexIndices.size() << " >>> ";
					//for (int vi = 0; vi < face.vertexIndices.size(); vi++)
					//	cout << face.vertexIndices[vi] << " ";

					if (3 == face.vertexIndices.size())
					{
						// Using array of vertex indices defining that face(s) set (Note : provided as vector of INT !)
						glDrawElements(GL_TRIANGLES, (GLsizei)face.vertexIndices.size(), GL_UNSIGNED_INT, (GLvoid*)&face.vertexIndices[0]);
					}
					else if (4 == face.vertexIndices.size())
					{
						// Using array of vertex indices defining that face(s) set (Note : provided as vector of INT !)
						glDrawElements(GL_QUADS, (GLsizei)face.vertexIndices.size(), GL_UNSIGNED_INT, (GLvoid*)&face.vertexIndices[0]);
					}
					else
					{
						//cout << endl << "Drawing face as a polygon, as described per : " << face.vertexIndices.size() << " vertex indexes";
						// Using array of vertex indices defining that face(s) set (Note : provided as vector of INT !)
						glDrawElements(GL_POLYGON, (GLsizei)face.vertexIndices.size(), GL_UNSIGNED_INT, (GLvoid*)&face.vertexIndices[0]);
					}
				}

			}
				
			glDisableClientState(GL_VERTEX_ARRAY);
		}
		else // some UVs are provided
		{
			// CASE C 

			cout << endl << "CASE C : material(s) color(s) defined, using texture(s) ...";
				
			cout << endl << "FINISH ME !!!!  (apply textures if any, not only material colors)";
				
			glEnableClientState(GL_VERTEX_ARRAY);

			glVertexPointer(3, GL_FLOAT, 0, &lo.objVertices[0]); // the COMPLETE list of vertices is used for each draxing iteration below

			for (int m = 0; m < lo.objMats.size(); m++)
			{
				mtl::rgb& matKd = lo.objMats[m].Kd;
				// first assign color using material one (simple approach : we use Kd)
				glColor3f(matKd.r, matKd.g, matKd.b);

				//
				// TO BE FINISHED (below) : 	manage texture, use UVs array and UVs indices (per faces)
				//

				// per construction, for each defined material; a matching vector of faces exist; 
				// ... so render it with related material
				vector<obj::Face>& matFaces = lo.objMatFaces[m]; // remember : set of faces with index m is related to material with index m
				//cout << endl << "-set #" << m << " ,contains " << matFaces.size() << " face(s) to draw";
				for (int f = 0; f < matFaces.size(); f++)
				{
					obj::Face& face = matFaces[f];

					//cout << endl << "vertex indice(s) to load :" << face.vertexIndices.size() << " >>> ";
					//for (int vi = 0; vi < face.vertexIndices.size(); vi++)
					//	cout << face.vertexIndices[vi] << " ";

					if (3 == face.vertexIndices.size())
					{
						// Using array of vertex indices defining that face(s) set (Note : provided as vector of INT !)
						glDrawElements(GL_TRIANGLES, (GLsizei)face.vertexIndices.size(), GL_UNSIGNED_INT, (GLvoid*)&face.vertexIndices[0]);
					}
					else if (4 == face.vertexIndices.size())
					{
						// Using array of vertex indices defining that face(s) set (Note : provided as vector of INT !)
						glDrawElements(GL_QUADS, (GLsizei)face.vertexIndices.size(), GL_UNSIGNED_INT, (GLvoid*)&face.vertexIndices[0]);
					}
					else
					{
						//cout << endl << "Drawing face as a polygon, as described per : " << face.vertexIndices.size() << " vertex indexes";
						// Using array of vertex indices defining that face(s) set (Note : provided as vector of INT !)
						glDrawElements(GL_POLYGON, (GLsizei)face.vertexIndices.size(), GL_UNSIGNED_INT, (GLvoid*)&face.vertexIndices[0]);
					}
				}

			}

			glDisableClientState(GL_VERTEX_ARRAY);	
		}

		/* // prev. simple test code :
		{
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
		}
		*/		
	}
	

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