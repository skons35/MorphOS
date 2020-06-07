#ifndef OBJLOADER_H
#define OBJLOADER_H


#include "MtlLoader.h"
//#include "BmpLoader.h"

#include <iostream> // cout

#include<vector>

#include<string>

using namespace std;

namespace obj 
{
    struct uv
    {
        public: 
            float u;
            float v;
            uv() { u = v = 0.0f; }
    };

    struct vec3 
    {
        public:
            float x;
            float y;
            float z;
            vec3() { x = y = z = 0.0f; }
    };

    struct Face 
    {
         public:
            // Caution : OBJ convention use indexes starting at 1 
            // but here we store them starting back from 0 !!
            vector<int> vertexIndices;
            vector<int> normalIndices;
            vector<int> uvIndices;
            Face() { } // no init, vectors empty at creation
    };
}

// See : https://en.wikipedia.org/wiki/Wavefront_.obj_file
// to get more detailes on the .OBJ file format

class ObjLoader
{
    private:
        string m_objFileName = "";
        vector<obj::vec3> m_vertices;
        // optional presence :
        vector<obj::vec3> m_normals;
        // optional presence :
        vector<obj::uv> m_uvs;
        // below faces are manipulating indexes (starting at 1),
        //  of :  vertices, normals (if relevant), UV textures (if relevant)
        vector<obj::Face> m_objFaces;

        // optional MTL file(s) name(s) specified  (useful when object is using materials and/or is textured)
        // !!!! possibly multiple material file(s) material provided
        vector<string> m_mtllibs; 

        // optional material(s) data, requiring additional parsers/loaders :
        // MTL -> MtlLoader, plus texture data loader if any (currently only BMP type managed)
        vector<mtl::Material> m_materials;  

        unsigned int m_currentMaterialIdx = 0;

    public:
        ObjLoader();
        
        bool parseObjFile(string fileName);       
        bool getParsedObjData( vector<obj::vec3>& vertices, vector<uint8_t>& vertIndices,  // vert. indices are usefull for using glDrawElements()
                               vector<obj::vec3>& vertNormals,     // possibly empty
                               vector<obj::uv> vertUvs,           // possibly empty
                               vector<mtl::Material>& materials  // possibly empty
                            );
        void printDetails(bool fullDetails = false);

    private:
        bool parseLine(string& line);
        bool parseVertexDefinition(string& params);
        bool parseVertexNormalDefinition(string& params);
        bool parseVertexTextureDefinition(string& params);
        bool parseFaceTypeDefinition(string& params);
        bool processFaceVertexInfos(string& infoStr, obj::Face& face);

        bool parseMaterialTemplateLibraryDefinition(string& params);
        bool extractMTLFileInfos(string mtlFileName);

        bool defineCurrentMaterial(string& lineParams);
};



#endif