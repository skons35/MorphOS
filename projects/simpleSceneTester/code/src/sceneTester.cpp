// SceneTester.cpp : Defines the entry point for the application.
//


#include "ObjLoader.h"

#include "commonFunctions.h"

#include <iostream>



using namespace std;

constexpr const char* DEF_DATA_PATH = "..\\..\\..\\Data\\";

int main(int argc, char *argv[])
{
	cout << "SceneTester started." << endl;

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

	/*
	vector<obj::vec3> objVertices;
	// optional presence :
	vector<obj::vec3> objNormals;
	// optional presence :
	vector<obj::uv> objUvs;
	if (!objLoader.getParsedObjData(objVertices, objNormals, objUvs))
		cout << endl << "No data get from ObjLoader ....";
	 */

	// Assign to Class controlling the draw & display functions our prepared ObjLoader
	DrawControl::objLoader = &objLoader;

	//*
	int WindowName;	
	
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);  //Optional
	WindowName = glutCreateWindow("SceneTester");
	
	//Full Screen switching (option)
	//glutFullScreen();

	// assign callbacks functions :
	glutReshapeFunc(DrawControl::Reshape);
	glutMouseFunc(DrawControl::mouseCommands);
	glutDisplayFunc(DrawControl::Draw);

	//InitGL();

	glutMainLoop();
	//*/


	// removing assigned ObjLoader.
	DrawControl::objLoader = nullptr;
 

	return 0;
}
