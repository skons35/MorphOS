#ifndef _MTLLOADER_H_
#define _MTLLOADER_H_

#include<vector>

#include<string>
using namespace std;

#include <RawImage.h>

namespace mtl 
{
    struct rgb  // caution : such rgb def. may exist in other namespaces
    {
        public:
            float r;
            float g;
            float b;
            //rgb() { r = g = b = 0.0f; }
            rgb() { r = g = b = 1.0f; }
    };

    // generic definition, some parameters are optional
    struct Material 
    {
        public:
            string name;
            float Ns; // spec. exponent in [0., 100.] range
            mtl::rgb Ka; // Ambient color, RGB in  [0., 1.] range
            mtl::rgb Kd; // Diffuse color
            mtl::rgb Ks; // Specular color
            mtl::rgb Ke; // Emissive color
            float Ni = 1.0; //optical density
            float  d = 1.0; //transparency , in range [0.; 1.] (no transp.) 
            int illum;
            string map_Kd; // a texture file name, if provided
            // above decoded texture data  :
            //     CAUTION : only 24/32 BMP format decoding at this point 
            struct RawImage rawImage; 
    };

    enum MaterialElement
    {
        NS, KA, KD, KS, KE, NI, D, ILLUM, MAP_KD
    };
}


class MtlLoader
{
    private:
        string m_mtlFileName = "";

        vector<mtl::Material> m_materials;

        bool m_mtlFileParsed = false;
        bool m_isUsingTextures = false;


    public:
        MtlLoader();
        bool parseMtlFile(string fileName);  

        bool extractTextureImageData(mtl::Material& currMat, string textureImageFileName);

        bool getParsedMtlData(vector<mtl::Material>& materials);

        void printDetails(bool fullDetails = false);

        bool isMtlFileParsed() { return m_mtlFileParsed; };
        bool isUsingTextures() { return m_isUsingTextures; };

    private:
        bool parseLine(string& line);
        bool parseNewMaterialDefinition(string& params);

        bool parseMaterialElementDefinition(string& params, mtl::MaterialElement me);
     
};


#endif