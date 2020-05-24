#include "BmpLoader.h"

#include<iostream> // cout
#include<fstream> // ifstream
#include<istream>

BmpLoader::BmpLoader()
{
}

bool BmpLoader::parseBmpFile(string fileName)
{   
    m_imageParsed = false;

    m_bmpFileName = fileName;

    ifstream bmpFile(m_bmpFileName, ifstream::binary);
    if (!bmpFile.is_open())
    {
        cout << endl << "Failed to open : " << m_bmpFileName;
        return false;
    }


    // BMP HEADER READ & PROCESS 

    char bmpHeader[54];

    if(!bmpFile.read(&bmpHeader[0], 54))
    {
        cout << endl << "Failed to read file's HEADER from : " << m_bmpFileName;
        bmpFile.close();
        return false;
    }

    // ok let's check magic number, bytes BM
    if ((bmpHeader[0] != 'B') || (bmpHeader[1] != 'M'))
    {
        cout << endl << "Fail to check magic number BM, find : "<< bmpHeader[0] << "," << bmpHeader[1] <<". Aborting.";
        bmpFile.close();
        return false;
    }

    // ok (re)init internal struct BmpImage
    m_RawImage.data.clear();    
    m_RawImage.width = 0;
    m_RawImage.height = 0;
    m_RawImage.bytesPerPixel = 0;

    // parse header infos :

    // !!!! Little Indian used in BMP file ...

    //  swap bytes 2/3/4/5:
    int filesize = bmpHeader[5] << 24 | bmpHeader[4] << 16 | bmpHeader[3] << 8 | bmpHeader[2];
    //cout << endl << "Header : FileSize="<< filesize;

    //  swap bytes 10/11/12/13:
    int dataPos = bmpHeader[13] << 24 | bmpHeader[12] << 16 | bmpHeader[11] << 8 | bmpHeader[10];
    //cout << endl << "Header : dataStartPos="<< dataPos;

    //  swap bytes 18/19/20/21:
    int width = bmpHeader[21] << 24 | bmpHeader[20] << 16 | bmpHeader[19] << 8 | bmpHeader[18];
    //cout << endl << "Header : img width="<< width;

    //  swap bytes 22/23/24/25:
    int height = bmpHeader[25] << 24 | bmpHeader[24] << 16 | bmpHeader[23] << 8 | bmpHeader[22];
    //cout << endl << "Header : img height="<< height;
    

    // 2nd part : load img data 
    //  <<<< enhance me : check plane order ARGB, BGRA ????
    //
    int bytesPerPixel = 0;

    // need to check if 24bpp or 32bpp (RGB or RGBA)
    int dataLength = filesize - dataPos;
    //cout << endl << "Remaining Data Length to read : " << dataLength;

    if (0 == (dataLength - width * height * 3))
    {
        bytesPerPixel = 3;
    }
    else if (0 == (dataLength - width * height * 4))
    {
        bytesPerPixel = 4;
    }
    //cout << endl << "Bytes per pixel : "<<  bytesPerPixel << "  (0 if unexpected format)";
    
    // alloc & copy data only if format ok
    if (0 != bytesPerPixel)
    {
        m_RawImage.data.resize(dataLength);
        //cout << endl << "resized storage vector to  : " << m_BmpImage.data.size();

        // relocate just at the specified data start pos :
        bmpFile.seekg(dataPos, bmpFile.beg);
                
        // read all the img data :
        if ( !bmpFile.read((char *)&m_RawImage.data[0], dataLength) )
        {
            cout << endl << "Failed to read all image data from : " << m_bmpFileName;
            bmpFile.close();
            return false;
        }

        //cout << endl << "Copied Image data Length (bytes) : " << dataLength;

        // finalize kept infos to be returned
        m_RawImage.width = width;
        m_RawImage.height = height;
        m_RawImage.bytesPerPixel = bytesPerPixel;
    }

    // CLOSE
    bmpFile.close();   

    m_imageParsed = true; 

    return m_imageParsed;
};

void BmpLoader::printDetails()
{
    if(!m_imageParsed)
        cout << endl << "CAUTION : Image parsing NOT DONE, or FAILed ! Information may be incomplete...";
    cout << endl << "Image FileName    : " << m_bmpFileName;
    cout << endl << "Image Data Length, in byte(s)  : " << m_RawImage.data.size();
    cout << endl << "Image bytes per pixel : " << m_RawImage.bytesPerPixel;
    cout << endl << "Image Width : " << m_RawImage.width;
    cout << endl << "Image Height : " << m_RawImage.height;
}

/*
struct RawImage* BmpLoader::getRawImage() 
{
    // sanity check
    if(!m_imageParsed)
     {
        cout << endl << "No Image parsed yet. NULL info returned.";
        return nullptr;
     }

    return &m_RawImage;
}
*/

bool BmpLoader::copyRawImageTo(struct RawImage& destRawImage)
{
    // sanity check
    if (!m_imageParsed)
    {
        cout << endl << "No Image parsed yet. No data / info to copy.";
        return false;
    }

    destRawImage.data.clear();
    // copy data bytes :
    destRawImage.data.insert(destRawImage.data.end(), m_RawImage.data.begin(), m_RawImage.data.end());
    // and image infos (widht, height, bpp) :
    destRawImage.width = m_RawImage.width;
    destRawImage.height = m_RawImage.height;
    destRawImage.bytesPerPixel = m_RawImage.bytesPerPixel;

    return true;
}

