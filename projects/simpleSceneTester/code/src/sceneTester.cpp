// SceneTester.cpp : Defines the entry point for the application.
//


#include "ObjLoader.h"

//#include "MtlLoader.h"

//#include "BmpLoader.h"

#include "glDrawControl.h"

#include <iostream>



using namespace std;

// default relative path from tests_scripts folder: 
constexpr const char* DEF_DATA_PATH = "..\\Data\\";
// default relative path from binary location :
// "..\\..\\..\\Data\\";

int main(int argc, char *argv[])
{
	cout << "SceneTester started." << endl;

///*
	// 1) OBJ load & infos display
    cout << "testing load OBJ file..." << endl;
	string fullPathFileName;

	// optional cmd line  for loading an obj file
	if(argc>1)
	{
		cout << endl << "Using cmd line given param...";
		fullPathFileName = argv[1];
	}
	else 
	{
		cout << endl << "NO File provide on cmd line. Will use a default one...";
		std::string dataPath = DEF_DATA_PATH;
		std::string objFileName = "VA_textured_faces_with_ananas.obj";
		//std::string objFileName = "VA_colored_faces_with_mat.obj";
		//std::string objFileName = "VA_colored_faces.obj";

		fullPathFileName = dataPath + objFileName;
	}	

	cout << endl << "Will read file : " << fullPathFileName;

	ObjLoader objLoader;
	bool parseRes = false;
	try 
	{
		parseRes = objLoader.parseObjFile(fullPathFileName);		
		cout << endl << "- Parsing result : " << (parseRes ? "SUCCESS" : "FAILED");
	}
	catch(exception exc) 
	{
		cout << endl << "ERROR processing file, with exception : "<< exc.what();
		cout << endl << "Terminating App";
		return -1;
	}
	
	objLoader.printDetails(false); // true for full details option

	// 2) use OpenGL to display the OBJ loaded
///*

	// Assign to Class controlling the draw & display functions 
	// our prepared ObjLoader data
	
	bool addToStorage = DrawControl::storeLoadedObject(objLoader);
	cout << endl << "- GL Draw Control storage adding result : " << (addToStorage ? "SUCCESS" : "FAILED");

	// add more loaded ojects into storage if needed....


	int WindowName;	
	
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);  //Optional
	WindowName = glutCreateWindow("SceneTester");
	
	//Full Screen switching (option)
	//glutFullScreen();

	DrawControl::setDefaultsForDrawing(); // defines GL background color, ....

	// HERE, we do a ONE SHOT review of loaded object(s)
	// notably regarding texture(s) binding (so GL context MUST be created at this point, ....)
	int preparedTexturesCount = DrawControl::prepareTexturesForOpenGL();
	cout << endl << "- GL Draw Control has prepared : " << preparedTexturesCount << " texture(s) for OpenGL use";


	// assign callbacks functions :
	glutReshapeFunc(DrawControl::Reshape);
	glutMouseFunc(DrawControl::mouseCommands);
	glutDisplayFunc(DrawControl::Draw);

	//InitGL();

	glutMainLoop();

 //*/

	return 0;
}
