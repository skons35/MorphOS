// mtlLoadTester.cpp : Defines the entry point for the application.
//


#include "MtlLoader.h"

#include <iostream>



using namespace std;

// default relative path from tests_scripts folder: 
constexpr const char* DEF_DATA_PATH = "..\\Data\\";
// default relative path from binary location :
// "..\\..\\..\\Data\\";

int main(int argc, char *argv[])
{
	cout << "mtlLoadTester started." << endl;

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

	mtlLoader.printDetails(true); // true for full details option

	return 0;
}
