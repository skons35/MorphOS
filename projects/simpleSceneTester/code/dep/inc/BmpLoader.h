#ifndef _BMPLOADER_H_
#define _BMPLOADER_H_


#include<RawImage.h>

/*
#include<vector>

#include<string>
using namespace std;


struct RawImage
{
    vector<unsigned char> data;
    int width;
    int height;
    int bytesPerPixel;
};
*/

class BmpLoader
{
private:
    string m_bmpFileName = "";
    
    struct RawImage m_RawImage;

    bool m_imageParsed = false;


public:
    BmpLoader();
    bool parseBmpFile(string fileName);    
    //bool getParsedObjData(vector<obj::vec3>& vertices, vector<uint8_t>& vertIndices,  // vert. indices are usefull for using glDrawElements()
    //    vector<obj::vec3>& vertNormals, vector<obj::uv> vertUvs);
    void printDetails();

    bool isImageParsed() { return m_imageParsed; };

    //struct RawImage* getRawImage();  

    bool copyRawImageTo(struct RawImage& destRawImage);

};


/*
#include<stdbool.h> // defines bool ,true, false

struct BmpImage 
{
  unsigned char* data;
  int width;
  int height;
  int bytesPerPixel;
};

struct BmpImage* readBmpContent(const char * bmpFileName);

bool freeBmpImage(struct BmpImage* bmpImage);
*/


#endif // _BMPLOADER_H_

/*
struct BmpImage
{
  unsigned char* data;
  int width;
  int height;
  int bytesPerPixel;
};

struct BmpImage* readBmpContent(const char * bmpFileName);

bool freeBmpImage(struct BmpImage* bmpImage);
*/