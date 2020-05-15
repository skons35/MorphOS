// SceneTester.cpp : Defines the entry point for the application.
//


#include "ObjLoader.h"

#include "MtlLoader.h"

#include "BmpLoader.h"

#include "commonFunctions.h"

#include <iostream>



using namespace std;

// default relative path from tests_scripts folder: 
constexpr const char* DEF_DATA_PATH = "..\\Data\\";
// default relative path from binary location :
// "..\\..\\..\\Data\\";

int main(int argc, char *argv[])
{
	cout << "SceneTester started." << endl;

	// 1) test load MTL file
	cout << "testing load MTL file..." << endl;
	string fullPathFileName;

	if (argc > 1)
	{
		cout << endl << "Using cmd line given param...";
		fullPathFileName = argv[1];
	}
	else
	{
		cout << endl << "NO File provide on cmd line. Will use a default one...";
		std::string dataPath = DEF_DATA_PATH;
		std::string mtlFileName = "VA_textured_faces_with_ananas.mtl";
		fullPathFileName = dataPath + mtlFileName;
	}

	cout << endl << "Will parse MTL File :" << fullPathFileName;

	MtlLoader mtlLoader;
	bool parseRes = mtlLoader.parseMtlFile(fullPathFileName);
	cout << endl << "- MTL File Parsing : " << (parseRes ? "SUCCESS" : "FAILED");

	mtlLoader.printDetails(true);

/*
	// 1) test load BMP file
	cout << "testing load BMP file..." << endl;
	string fullPathFileName;

	if (argc > 1)
	{
		cout << endl << "Using cmd line given param...";
		fullPathFileName = argv[1];
	}
	else
	{
		cout << endl << "NO File provide on cmd line. Will use a default one...";
		std::string dataPath = DEF_DATA_PATH;
		std::string bmpFileName = "skons_rd_256x512_RGB_24bpp.bmp";
		fullPathFileName = dataPath + bmpFileName;
	}

	cout << endl << "Will parse Image File :" << fullPathFileName;

	BmpLoader bmpLoader;
	bool parseRes = bmpLoader.parseBmpFile(fullPathFileName);
	cout << endl << "- BMP File Parsing : " << (parseRes ? "SUCCESS" : "FAILED");
	
	bmpLoader.printDetails();
*/

/*
	// 2) OBJ load & display
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


	// Assign to Class controlling the draw & display functions our prepared ObjLoader
	DrawControl::objLoader = &objLoader;

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


	// removing assigned ObjLoader.
	DrawControl::objLoader = nullptr;
 
 */

	return 0;
}
