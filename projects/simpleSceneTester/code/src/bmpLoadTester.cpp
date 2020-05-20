// bmpLoadTester.cpp : Defines the entry point for the application.
//


#include "BmpLoader.h"

#include <iostream>



using namespace std;

// default relative path from tests_scripts folder: 
constexpr const char* DEF_DATA_PATH = "..\\Data\\";
// default relative path from binary location :
// "..\\..\\..\\Data\\";

int main(int argc, char *argv[])
{
	cout << "bmpLoadTester started." << endl;


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



	return 0;
}
