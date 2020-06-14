
#include "ObjLoader.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include<istream>
#include<streambuf>
#include<iomanip>

#include<algorithm> // string replace()


ObjLoader::ObjLoader()
{
}

bool ObjLoader::parseObjFile(string fileName)
{  
    bool parseCompleted = true;

    m_objFileName = fileName;

    // reset storage vectors
    m_vertices.clear();
    m_normals.clear();
    m_uvs.clear();
    m_objFaces.clear();
    m_mtllibs.clear();
    // clear extra infos (if any) if using additional loaders (MTL, BMP)
    m_materials.clear();


    // try open / access file :
    ifstream objFile(m_objFileName); // text mode
    if (!objFile.is_open())
    {
        cout << endl << "Failed to open : " << m_objFileName;
        return false;
    }

    stringstream ss;
    ss << objFile.rdbuf(); // get file stream buffer, then pass it to stringstream
    
    string line;
    char delim = 0x0A; // LF
    //int lidx = 0;
    while (!ss.eof())
    {   
        //lidx++;
        std::getline(ss, line, delim); 

        //cout << endl << "parsing line [" << line << "] ";

        bool lineParsed = parseLine(line);

        parseCompleted = parseCompleted && lineParsed; // one failure means global failure reported
    }      

    // CLOSE
    objFile.close();

    return parseCompleted;
};

bool ObjLoader::parseLine(string & line)
{
    if (line.empty())
        return true; // accepting an empty line as NOT an error (last line case, often)

    // specific comment line process (as maybe a simple # one carac. line)
    if (line[0] == 0x23)
    {
        cout << endl << "IGNORED COMMENT Line : " << line;
        return true;
    }

    size_t bPos = line.find_first_of(' ');
    if (std::string::npos == bPos) // no space detected in line
    {
        // not "usual case", check that line format
        cout << endl << "Unexpected line , format : " << line << " , considered as an error";
        return false; 
    }

    string lineType = line.substr(0, bPos);
    
    string lineParams = line.substr(bPos+1);

    //cout << endl << "Line Type : "<< lineType << ", params : " << typeParams;

    // do the parsing
    if (lineType == "v")
        return parseVertexDefinition(lineParams);
    else if (lineType == "vn")
        return parseVertexNormalDefinition(lineParams);
    else if (lineType == "vt")
        return parseVertexTextureDefinition(lineParams);
    else if (lineType == "f")
        return parseFaceTypeDefinition(lineParams);
    else if (lineType == "mtllib")
        return parseMaterialTemplateLibraryDefinition(lineParams);
    else if (lineType == "usemtl")
        return defineCurrentMaterial(lineParams);
    else // All other type simply ignored but displayed(  ....
        cout << endl << "(IGNORED) : Line Type : " << lineType << ", params : " << lineParams;
    return true;
}

bool ObjLoader::parseVertexDefinition(string& params)
{    
    stringstream ss(params);        
    obj::vec3 v;    
    ss >> v.x; 
    ss >> v.y; 
    ss >> v.z;
    m_vertices.push_back(v);

    return true;
}

bool ObjLoader::parseVertexNormalDefinition(string& params)
{
    stringstream ss(params);
    obj::vec3 vn;
    ss >> vn.x;
    ss >> vn.y;
    ss >> vn.z;
    m_normals.push_back(vn);

    return true;
}

bool ObjLoader::parseVertexTextureDefinition(string& params)
{
    stringstream ss(params);
    obj::uv uv;
    ss >> uv.u;
    ss >> uv.v;    
    m_uvs.push_back(uv);

    return true;
}

bool ObjLoader::parseFaceTypeDefinition(string& params)
{    
    bool faceProcessed = true;

    obj::Face face;

    // DO THE face details extraction , BUT the format may vary :
    // (ALSO  : possibly more than 3 vertices, if a polygon face instead of simple triangle)
    //
    //           -  v1 v2 v3 .... 
    //           -  v1/vt1 v2/vt2 v3/vt3 .... 
    //           -  v1//vn1 v2//vn2 v3//vn3 .... 
    //           -  v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 .... 

    // split the provided string, part per part (separated per space)     
    istringstream iss(params);
    do 
      { 
        string subStr;
        iss >> subStr;
        
        // Assuming getting an empty line part means : end of param line
        if (subStr.empty())
            break;

        //cout << endl << " extrc. face params : " << subStr;
        faceProcessed = faceProcessed && processFaceVertexInfos(subStr, face);

      } 
    while (iss);
    
    m_objFaces.push_back(face);
    
    // tempo debug :
    //cout << endl << "stored face(s) now :" << m_objFaces.size();

    return faceProcessed;
}

// BELOW is done a special embedded process, as we are going
// to pass from OBJ format indexing (starting from 1) to indexing starting from 0 (usual in C)
// >>> so any v/vn/vt index read as y, will be stored as (y-1) index
bool ObjLoader::processFaceVertexInfos(string& infoStr, obj::Face& face)
{
    // Now, deal with each face's vertex info, in one of the format : 
    //           v 
    //           v/vt 
    //           v//vn 
    //           v/vt/vn 

    char sep = '/';
        
    size_t firstSepPos = infoStr.find_first_of(sep);
    if (string::npos == firstSepPos)
    {
        // format case : v 
        int vIdx;

        std::istringstream iss(infoStr);
        iss >> vIdx;

        // tempo display :
        //cout << endl << "Parsed vIdx " << vIdx;

        face.vertexIndices.push_back(vIdx-1); // stored following 0 indexing        

        return true;
    }

    size_t lastSepPos = infoStr.find_last_of(sep);
    if (firstSepPos == lastSepPos)
    {
        // format case :  v/vt 
        int vIdx, vtIdx;

        std::replace(infoStr.begin(), infoStr.end(), sep, ' ');
        std::istringstream iss(infoStr);
        iss >> vIdx; // >> "/"
        iss >> vtIdx;

        // tempo display :
        //cout << endl << "Parsed vIdx, vtIdx " << vIdx << ", " << vtIdx;

        face.vertexIndices.push_back(vIdx-1);  // stored following 0 indexing        
        face.uvIndices.push_back(vtIdx-1);    //
        
        return true;
    }

    if (lastSepPos - firstSepPos == 1)  // contiguous characters
    {
        // format case :  v//vn 
        int vIdx, vnIdx;
        
        std::replace(infoStr.begin(), infoStr.end(), sep, ' ');
        std::istringstream iss(infoStr);
        iss >> vIdx; // >> "//" 
        iss >> vnIdx;

        // tempo display :
        //cout << endl << "Parsed vIdx, vnIdx " << vIdx << ", " << vnIdx;

        face.vertexIndices.push_back(vIdx-1);    // stored following 0 indexing      
        face.normalIndices.push_back(vnIdx-1);  //
        
        return true;
    }
    
    // last case, format :       v/vt/vn 
    int vIdx, vtIdx, vnIdx;
    std::replace(infoStr.begin(), infoStr.end(), sep, ' ');
    std::istringstream iss(infoStr);
    iss >> vIdx;  // >> "/"
    iss >> vtIdx; // >> "/"
    iss >> vnIdx;

    // tempo display :
    //cout << endl << "Parsed vIdx, vtIdx, vnIdx " << vIdx << ", " << vtIdx << ", " << vnIdx;

    face.vertexIndices.push_back(vIdx-1);    // stored following 0 indexing  
    face.uvIndices.push_back(vtIdx-1);      //
    face.normalIndices.push_back(vnIdx-1); //
   
    return true;
}


bool ObjLoader::parseMaterialTemplateLibraryDefinition(string& params)
{
    stringstream ss(params);
    string mtlFileName;
    ss >> mtlFileName;

    m_mtllibs.push_back(mtlFileName);

    // we need materials infos right now, so immediately parse that file and return result
    return extractMTLFileInfos(mtlFileName);
}

bool ObjLoader::extractMTLFileInfos(string mtlFileName)
{
    // Important : the MTL file may be located in same folder than OBJ
    //             or relatively to it;
    // we need to regenerate relevant file path before opening it
    string mtlFilePathAndName;

    size_t fnPos = this->m_objFileName.find_last_of("/\\");

    if (std::string::npos == fnPos) // no folder path (full or partial) in obj filename
        mtlFilePathAndName = mtlFileName; // assume MTL in same folder than OBJ
    else
        mtlFilePathAndName = this->m_objFileName.substr(0, fnPos + 1) + mtlFileName; // assume relative path from OBJ file

    cout << endl << "Processing MTL file : " + mtlFilePathAndName;

    // open & process MTL :
    MtlLoader mtlLoader;
    bool parseRes = mtlLoader.parseMtlFile(mtlFilePathAndName);

    cout << endl << "- Parsing MTL File " << mtlFilePathAndName << ", result : " << (parseRes ? "SUCCESS" : "FAILED");

    // accumulate extracted Material(s) data here
    if (parseRes)
    {
        vector<mtl::Material> extMat;
        mtlLoader.getParsedMtlData(extMat);
        m_materials.insert(m_materials.end(), extMat.begin(), extMat.end());
    }

    return parseRes;
}


bool ObjLoader::defineCurrentMaterial(string& params) 
{
    // sanity check :
    if (m_materials.empty())
    {
        cout << endl << "NO material currently defined, cannot assign one as current...";
        return false;
    }

    stringstream ss(params);
    string matName;
    ss >> matName;

    bool foundEntry = false;
    for(int i=0; i<m_materials.size(); i++)
    {
        if (m_materials[i].name == matName)
        {
            m_currentMaterialIdx = i;
            cout << endl << "Current Material set to : '" << matName << "' (idx:"<< m_currentMaterialIdx<<")";
            foundEntry = true;
            break;
        }
    }
    
    return foundEntry;
}

bool ObjLoader::getParsedObjData(vector<obj::vec3>& vertices, vector<uint8_t>& vertIndices,  // vert. indices are usefull for using glDrawElements()
    vector<obj::vec3>& vertNormals, vector<obj::uv>& vertUvs, vector<mtl::Material>& materials)
{
    // sanity check : some vertices were extracted ???
    if (m_vertices.empty())
        return false;

    vertices.clear();
    vertIndices.clear();
    vertNormals.clear();
    vertUvs.clear();
    // optional Material(s) info clear
    materials.clear();

    // copy data into provided vectors
    vertices.insert(vertices.end(), m_vertices.begin(), m_vertices.end());
    vertNormals.insert(vertNormals.end(), m_normals.begin(), m_normals.end());
    vertUvs.insert(vertUvs.end(), m_uvs.begin(), m_uvs.end());

    // feed the vertex indices vector
    for (int i = 0; i < m_objFaces.size(); i++)
    {
        obj::Face& currFace = m_objFaces[i];

        vertIndices.insert(vertIndices.end(), currFace.vertexIndices.begin(), currFace.vertexIndices.end());
    }

    // provide optional Material(s) info defined
    materials.insert(materials.end(), m_materials.begin(), m_materials.end());

    return true;
}

void ObjLoader::printDetails(bool fullDetails)
{
    cout << endl << "OBJ file    : " << m_objFileName;
    cout << endl << "Vertices    : " << m_vertices.size();
    cout << endl << "Normals     : " << m_normals.size();
    cout << endl << "Vert. UVs   : " << m_uvs.size();
    cout << endl << "Obj. Face(s): " << m_objFaces.size();
    // optional / extra infos using specifcs loader(s) :
    if (!m_mtllibs.empty())
    {
        cout << endl << "(OPT) MTL file(s) defined : " << m_mtllibs.size();
        cout << endl << "(OPT) Total Material(s) defined : " << m_materials.size();
    }
    else
        cout << endl << "(OPT) no MTL file definition found.";
    
    if (fullDetails)
    {
        cout << endl << "---------------------------";

        for (int i = 0; i < m_vertices.size(); i++)
         {
            cout << endl << "vert.#" << i << " : " << m_vertices[i].x << ", " << m_vertices[i].y << ", " << m_vertices[i].z;
         }

        for (int i = 0; i < m_normals.size(); i++)
        {
            cout << endl << "norm.#" << i << " : " << m_normals[i].x << ", " << m_normals[i].y << ", " << m_normals[i].z;
        }

        for (int i = 0; i < m_uvs.size(); i++)
        {
            cout << endl << "UV.#" << i << " : " << m_uvs[i].u << ", " << m_uvs[i].v ;
        }

        for (int i = 0; i < m_objFaces.size(); i++)
        {
            cout << endl << "Face.#" << i << " : " << endl << "  -vertex count = " << m_objFaces[i].vertexIndices.size()
                << endl << "  -UVs     count = " << m_objFaces[i].uvIndices.size()
                << endl << "  -Normals count = " << m_objFaces[i].normalIndices.size();
        }

        cout << endl << "---------------------------";
    }
}

