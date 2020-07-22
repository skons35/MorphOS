
#include "glDrawControl.h"

//#include <gl/gl.h>

/*

float vertPosTri_1[9] = { 0.0f,  0.0f, -5.0f, 		2.0f, 0.0f, -5.0f, 		0.0f, 2.0f, -5.0f };
// fixed to be counter wise order two :
float vertPosTri_2[9] = { -3.0f, -3.0f, -8.0f,	   0.0f, -3.0f, -8.0f,      -3.0f, 1.0f, -8.0f };

// Blue :
float vertColTri_1[9] = { 0.0f,  0.0f, 1.0f,	 0.0f, 0.0f, 1.0f,	    0.0f,  0.0f, 1.0f };
// Yellow :
float vertColTri_2[9] = { 1.0f,  1.0f, 0.0f,	 1.0f, 1.0f, 0.0f,	    1.0f,  1.0f, 0.0f };
*/


std::vector<LoadedObject> DrawControl::loadedObjects = vector<LoadedObject>();
std::vector<GLuint> DrawControl::glTexturesIDs = vector<GLuint>();

static float rotAngle = 0.0f;
static float rotDir = 1.0f; // +1 or -1

static double fov = 60.0f; // 45.0f;

// camer initial location :
static float xPos = 0.0f, yPos = 0.0f, zPos = -6.0f; 

bool DrawControl::storeLoadedObject(ObjLoader objLoader)
{
	//cout << endl << "storeLoadedObject : BEFORE adding object, storage element(s) : " << loadedObjects.size();

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

void DrawControl::setDefaultsForDrawing()
{
	// tempo : define a clear color not fully black (useful for debug until object texturing is fully tested)
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	// set DEPTH testing (for Z Buffer use and correct 3D Objects layout)
	glEnable(GL_DEPTH_TEST); // masked object are not drawn
}

// Note : below method must be called after GL context is created,
// and will serve to bind a unique Texture ID to any declared texture 
// in material(s) of stored LoadedObject
int DrawControl::prepareTexturesForOpenGL() 
{
	int textureBindCount = 0; 

	glTexturesIDs.clear();

	// STEP 1 : parse all stored objects, check their material(s),
	// to estimate the global/total number of texture(s) to be binded.

	cout << endl << "prepareTexturesForOpenGL() : checking for texture binding needs...";

	for (int idx = 0; idx < loadedObjects.size(); idx++)
	{
		LoadedObject& lo = loadedObjects[idx];

		// iterate on every stored object, and render it
		for (int i = 0; i < lo.objMats.size(); i++)
		{
			mtl::Material& om = lo.objMats[i];

			if (!om.map_Kd.empty())
			{
				// VA : ENHANCE ME !! below
				//      >>>>   we could use multiple glGenTextures() call; each for one element

				//cout << endl << "Loaded Obj. #" << idx << " has texture to be binded : " << om.map_Kd.c_str();

				// texture binding to be done ! count it
				textureBindCount++;
			}
		}
	}

	cout << endl << "prepareTexturesForOpenGL() : Textures(s) to bind : " << textureBindCount;

	// preparing GL texturing, ONLY WHEN NEEDED :
	if (textureBindCount == 0)
	{
		cout << endl << "NO texture to be used. Skipping GL texturing preparation.";
		return 0;
	}


	// STEP 2 : RE-parse all stored objects, check their material(s),
	// and assigned a unique ID to each Texture to be used (taking IDs from OpenGL provided ones)

	cout << endl << "prepareTexturesForOpenGL() : Assigning Texture(s) id(s):";

	// NOTE : if invalid texture images were provided, the code will crash just after that above message
	//        images maybe invalid for GL if :
	//           - its size in WxH, not being multiple of power of 2,
	//                   >>> quick fix : reexport the texture image to fit valid size, such as : 512x256, 1024x1024, ....
	//           - zero length data (and 0 width, and 0 height), possibly a bug in my BMP reader, sorry.
	//                  >>> quick fix :  reexport the texture image in BMP non RLE encoded format, and size valid as above
		
	glTexturesIDs.resize(textureBindCount);

	glEnable(GL_TEXTURE_2D); // enable 2D Texturing (server? side) 

	glGenTextures(textureBindCount, &glTexturesIDs[0]);  // def. in GL.h

	// tempo display : check all generated Texture IDs (should be unique integers)
	//for (int i = 0; i < glTexturesIDs.size(); i++)
	//	cout << endl << "tex.id generated for binding : " << glTexturesIDs[i];

	unsigned int uniqueIdPtr = 0;
	for (int idx = 0; idx < loadedObjects.size(); idx++)
	{
		LoadedObject& lo = loadedObjects[idx];

		// iterate on every stored object, and render it
		for (int i = 0; i < lo.objMats.size(); i++)
		{
			mtl::Material& om = lo.objMats[i];

			if (!om.map_Kd.empty())
			{
				RawImage& ri = om.rawImage;
				ri.uniqueId = glTexturesIDs[uniqueIdPtr];
				uniqueIdPtr++;

				// DO THE BINDING :
				glBindTexture(GL_TEXTURE_2D, (GLuint)ri.uniqueId);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // NEAREST);
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				// load the texture data in GL
				glTexImage2D(
					GL_TEXTURE_2D, // will applied on above activated target
					0, // level of detail, here 0, so Base
					GL_RGB, //GL_BGR,            // GL_BGRA, // 24bpp ok, 32bpp -> conversion needed ? to be checked
					ri.width,
					ri.height,
					0, //border's width
					GL_RGB, //GL_BGR,            //GL_BGRA, // for Morphos, working target format choice is GL_BGRA or GL_BGR
					GL_UNSIGNED_BYTE, // type
					&ri.data[0]);

				cout << endl << "For Loaded Obj. #" << idx << ", texture : " << om.map_Kd.c_str() << " binded with GL using Id : " << ri.uniqueId;
			}
		}
	}

	return textureBindCount;
}

void DrawControl::Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(
		fov, //45,
		float(width) / float(height),
		0.01, // 0.1
		300 // 100
	); 	
	
	// set back the matrix to ModelView
	glMatrixMode(GL_MODELVIEW); 	//Option
	glLoadIdentity();              //
}

void DrawControl::Draw()
{
	//VA : MOVED above in setDefaultsForDrawing()
	/*
	// set DEPTH testing (for Z Buffer use and correct 3D Objects layout)
	glEnable( GL_DEPTH_TEST); // masked object are not drawn
	*/

	// clear screen and depth buffer :
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	glMatrixMode(GL_MODELVIEW); 	//select MODELVIEW matrix
	glLoadIdentity(); 	//set as Identity
	
	//gluLookAt(0, 0, -10, 0, 0, 0, 0, 1, 0);

	//Test : adjust the location of object rendering  ( to control placement distance from camera) :
	//float xPos = 0.0f, yPos = 0.0f, zPos = -6.0f;
	
	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);

	// also introduce slow object(s) rotation (ON/OFF controlled per mouse button clicks)
	//static float xRot = 0.0f, yRot = 1.0f, zRot = 0.0f;
	static float xRot = 0.3f, yRot = 1.0f, zRot = 0.3f; // set small rot. for all axis
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
							
			glEnableClientState(GL_VERTEX_ARRAY);

			glVertexPointer(3, GL_FLOAT, 0, &lo.objVertices[0]); // the COMPLETE list of vertices is used for each draxing iteration below

			for (int m = 0; m < lo.objMats.size(); m++)
			{
				mtl::Material& objMat = lo.objMats[m];
				
				// first assign color using material one (simple approach : we use Kd)
				mtl::rgb& matKd = objMat.Kd;
				glColor3f(matKd.r, matKd.g, matKd.b);

				// Does current material uses a texture ?				
				if (!objMat.map_Kd.empty())
				{				

					RawImage& ri = objMat.rawImage;
					
					// re bind GL tewture with assigned unique id :
					glBindTexture(GL_TEXTURE_2D, (GLuint)ri.uniqueId);					

					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glTexCoordPointer(2, GL_FLOAT, 0, &lo.objUvs[0]); // again, the COMPLETE list of UVs is used for each draxing iteration below

				}
			

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

				if (!objMat.map_Kd.empty())
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
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
		case 3: // wheel mouse forward
			//cout << endl << "Wheel mouse FWD";
			//zPos = 2.0f * zPos;
			zPos += 2.0f;
			glutPostRedisplay();
			break;
		case 4: // wheel mouse backward
			//cout << endl << "Wheel mouse BWD";
			//zPos = 0.5f * zPos;
			zPos -= 2.0f;
			glutPostRedisplay();
			break;
		default: // middle button, ...
			glutIdleFunc(nullptr); // stop auto rotation & update
	}
}