#ifndef OBJLOADER_H
#define OBJLOADER_H

//#include <iostream>
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

        // useful when object is textured :
        vector<string> m_mtllibs; // possibly multiple material file(s) material provided



    public:
        ObjLoader();
        bool parseObjFile(string fileName);
        //bool getParsedObjData(vector<obj::vec3>& vertices, vector<obj::vec3>& vertNormals, vector<obj::uv> vertUvs);
        bool getParsedObjData( vector<obj::vec3>& vertices, vector<uint8_t>& vertIndices,  // vert. indices are usefull for using glDrawElements()
                               vector<obj::vec3>& vertNormals, vector<obj::uv> vertUvs);
        void printDetails(bool fullDetails = false);

    private:
        bool parseLine(string& line);
        bool parseVertexDefinition(string& params);
        bool parseVertexNormalDefinition(string& params);
        bool parseVertexTextureDefinition(string& params);
        bool parseFaceTypeDefinition(string& params);
        bool processFaceVertexInfos(string& infoStr, obj::Face& face);

};



#endif