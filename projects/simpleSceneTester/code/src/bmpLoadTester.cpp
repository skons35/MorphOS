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

	if (parseRes)
	{
		struct RawImage rawImageCopy; // empty RawImage; receiving data copy below
				
		if(bmpLoader.copyRawImageTo(rawImageCopy))
		 {
			cout << endl << "----Read back _copied_ image infos : ----";			
			cout << endl << "Image Width x height :  " << rawImageCopy.width << " x " << rawImageCopy.height;
			cout << endl << "Image Data byte(s): " << rawImageCopy.data.size();
			cout << endl << "Image data bytes per pixel :  " << rawImageCopy.bytesPerPixel;
		 }
	}

	return 0;
}
