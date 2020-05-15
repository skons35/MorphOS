#ifndef _MTLLOADER_H_
#define _MTLLOADER_H_

#include<vector>

#include<string>
using namespace std;


namespace mtl 
{
    struct rgb  // caution : such rgb def. may exist in other namespaces
    {
        public:
            float r;
            float g;
            float b;
            rgb() { r = g = b = 0.0f; }
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
            string map_Kd; // companion texture file name, if relevant
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