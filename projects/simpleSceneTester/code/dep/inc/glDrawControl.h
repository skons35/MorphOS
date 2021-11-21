#ifndef GLDRAWCONTROL_H
#define GLDRAWCONTROL_H

#include <iostream>

#include "ObjLoader.h"

// TODO: Reference additional headers your program requires here.

//#include <gl/freeglut.h>
//#include <gl/freeglut_ext.h>

//#include <gl/gl.h>
//#include <gl/glut.h>
#include <GL/glut.h> // !! case sensitive (GL, not gl)


//bool pseudoFunction();


struct LoadedObject
{
	vector<obj::vec3> objVertices;
	vector<obj::vec3> objNormals; // possibly empty
	vector<obj::uv> objUvs; // possibly empty

	vector<mtl::Material> objMats; // at least will contain one default material
	vector<vector<obj::Face>> objMatFaces; // at least will contain one set of loaded faces
	
	LoadedObject() {} // no init, vectors empty at creation
};


class DrawControl 
{
	public:

		static vector<LoadedObject> loadedObjects; // initialized in cpp
		static vector<GLuint> glTexturesIDs;     //

		static bool storeLoadedObject(ObjLoader objLoader);

		static void setDefaultsForDrawing();
		static int prepareTexturesForOpenGL(); // estimate & do any texture binding needed fo GL drawing

		static void Reshape(int width, int height);
		static void Draw();

		static void triggerObjectRotationThenDisplay();
		static void mouseCommands(int button, int state, int x, int y);

};


#endif
