#ifndef _RAWIMAGE_H_
#define _RAWIMAGE_H_

#include<vector>

#include<string>
using namespace std;


struct RawImage
{
    vector<unsigned char> data;
    unsigned int width;
    unsigned int height;
    unsigned int bytesPerPixel;
    unsigned int uniqueId; // assigned & used for GL drawing (texture binding)
    RawImage() { data.clear();  width = height = bytesPerPixel = uniqueId = 0; }
};


#endif // _RAWIMAGE_H_
