#include "MtlLoader.h"

#include "BmpLoader.h"

#include<iostream> // cout
#include<fstream> // ifstream
#include<sstream> // stringstrem
//#include<istream>
#include<streambuf>
//#include<iomanip>

MtlLoader::MtlLoader()
{
}

bool MtlLoader::parseMtlFile(string fileName)
{
    // reset global flags :
    m_mtlFileParsed = false;
    m_isUsingTextures = false;

    bool parseCompleted = true;

    m_mtlFileName = fileName;

    // reset storage vectors
    m_materials.clear();    


    // try open / access file :
    ifstream mtlFile(m_mtlFileName); // text mode
    if (!mtlFile.is_open())
    {
        cout << endl << "Failed to open : " << m_mtlFileName;
        return false;
    }

    stringstream ss;
    ss << mtlFile.rdbuf(); // get file stream buffer, then pass it to stringstream

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
    mtlFile.close();

    // store global success of parsing (or not)
    m_mtlFileParsed = parseCompleted;

    return m_mtlFileParsed;
};


bool MtlLoader::parseLine(string & line)
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

    string lineParams = line.substr(bPos + 1);

    //cout << endl << "Line Type : "<< lineType << ", params : " << lineParams;

    // do the parsing
    if (lineType == "newmtl")
        return parseNewMaterialDefinition(lineParams);
    else if (lineType == "Ns")
        return parseMaterialElementDefinition(lineParams, mtl::MaterialElement::NS);
    else if (lineType == "Ka")
        return parseMaterialElementDefinition(lineParams, mtl::MaterialElement::KA);
    else if (lineType == "Kd")
        return parseMaterialElementDefinition(lineParams, mtl::MaterialElement::KD);
    else if (lineType == "Ks")
        return parseMaterialElementDefinition(lineParams, mtl::MaterialElement::KS);
    else if (lineType == "Ke")
        return parseMaterialElementDefinition(lineParams, mtl::MaterialElement::KE);
    else if (lineType == "Ni")
        return parseMaterialElementDefinition(lineParams, mtl::MaterialElement::NI);
    else if (lineType == "d")
        return parseMaterialElementDefinition(lineParams, mtl::MaterialElement::D);
    else if (lineType == "illum")
        return parseMaterialElementDefinition(lineParams, mtl::MaterialElement::ILLUM);
    else if (lineType == "map_Kd")
        return parseMaterialElementDefinition(lineParams, mtl::MaterialElement::MAP_KD);    
    else // All other type simply ignored but displayed(  ....
        cout << endl << "(IGNORED) : Line Type : " << lineType << ", params : " << lineParams;
    return true;
}

bool MtlLoader::parseNewMaterialDefinition(string& params)
{
    //cout << endl << "NEW MATERIAL to be defined: " << params;
    stringstream ss(params);
    
    mtl::Material newMat;
    ss >> newMat.name;

    m_materials.push_back(newMat);

    return true;
}

bool MtlLoader::parseMaterialElementDefinition(string& params, mtl::MaterialElement me)
{
    // Sanity check : it MUST exist a current Material def (at least)
    if (m_materials.empty())
    {
        cout << endl << "NO Current Material found ! Unexpected element definition... ";
        return false;
    }
    
    mtl::Material& currMat = m_materials.back();


    stringstream ss(params);

    switch(me)
    {
        case mtl::MaterialElement::NS:
            ss >> currMat.Ns;
            break;
        case mtl::MaterialElement::KA:
            ss >> currMat.Ka.r;
            ss >> currMat.Ka.g;
            ss >> currMat.Ka.b;
            break;
        case mtl::MaterialElement::KD:
            ss >> currMat.Kd.r;
            ss >> currMat.Kd.g;
            ss >> currMat.Kd.b;
            break;
        case mtl::MaterialElement::KS:
            ss >> currMat.Ks.r;
            ss >> currMat.Ks.g;
            ss >> currMat.Ks.b;
            break;
        case mtl::MaterialElement::KE:
            ss >> currMat.Ke.r;
            ss >> currMat.Ke.g;
            ss >> currMat.Ke.b;
            break;
        case mtl::MaterialElement::NI:
            ss >>  currMat.Ni;
            break;
        case mtl::MaterialElement::D:
            ss >> currMat.d;
            break;
        case mtl::MaterialElement::ILLUM:
            ss >> currMat.illum;
            break;
        case mtl::MaterialElement::MAP_KD:
            ss >> currMat.map_Kd;
            // keep trace of texture use
            m_isUsingTextures = true;
            // TRY immediate DECODING of the texture img (data, size, ...) : 
            // Caution : BMP only supported at this time
            return extractTextureImageData(currMat, currMat.map_Kd);
            break;
        default:
            cout << "Unexpected param to process : " << me << " (please check param provided for process)";
            return false; // error case, only params to process should trigger current function
    }

    return true;
}

// extract texture image and assign it to current Material, along relevant map_Kd name
bool MtlLoader::extractTextureImageData(mtl::Material& currMat, string textureImageFileName)
{
    // Important : the BMP/PNG/... texture file may be located in same folder than refering MTL
    //             or relatively to it;
    // we need to regenerate relevant file path before opening it
    string texFilePathAndName;

    size_t fnPos = this->m_mtlFileName.find_last_of("/\\");

    if (std::string::npos == fnPos) // no folder path (full or partial) in obj filename
        texFilePathAndName = textureImageFileName; // assume MTL in same folder than OBJ
    else
        texFilePathAndName = this->m_mtlFileName.substr(0, fnPos + 1) + textureImageFileName; // assume relative path from OBJ file

    cout << endl << "Processing TEXTURE file : " + texFilePathAndName;

    // TO BE CONTINUED

    // Open & process Texture file using adequate reader :

    // check BMP extension ? 
    size_t bmpPos = textureImageFileName.find_last_of(".bmp");
    if (std::string::npos != bmpPos)
    {
        BmpLoader bmpLoader;
        bool parseRes = bmpLoader.parseBmpFile(texFilePathAndName);
        cout << endl << "(BMP Texture file Parsing : " << (parseRes ? "SUCCESS" : "FAILED") << ")";

        //bmpLoader.printDetails();

        bool copyDone = false;
        if (parseRes)
        {
            copyDone = bmpLoader.copyRawImageTo(currMat.rawImage);

            // tempo debug : 
            /*
            if (copyDone)
            {
                cout << endl << "Read back _copied_ Material Texture infos : ";
                cout << endl << "Image Width x height :  " << currMat.rawImage.width << " x " << currMat.rawImage.height;
                cout << endl << "Image Data byte(s): " << currMat.rawImage.data.size();
                cout << endl << "Image data bytes per pixel :  " << currMat.rawImage.bytesPerPixel;
            }
            */
        }

        return parseRes;
    }
    
    // TO DO : add other cases plus related loader there : (png, ...) 

    cout << endl << "Unmanaged texture file format....";
    return false;    
}

bool MtlLoader::getParsedMtlData(vector<mtl::Material>& materials)
{
    // sanity check : some material(s) infos were extracted ???
    if (m_materials.empty())
        return false;

    materials.clear();
    
    // copy data into provided vector
    materials.insert(materials.end(), m_materials.begin(), m_materials.end());

    return true;
}

void MtlLoader::printDetails(bool fullDetails)
{
    cout << endl << "MTL file    : " << m_mtlFileName;
    cout << endl << "Material(s) : " << m_materials.size();
    cout << endl << "MTL file contains texture(s) definition(s) ? : " << (m_isUsingTextures ? "Yes" : "No" );

    if (fullDetails)
    {
        cout << endl << "---------------------------";

        for (int i = 0; i < m_materials.size(); i++)
        {
            mtl::Material& mat = m_materials[i];
            cout << endl << "material #" << i << " : ";
            cout << endl << "  - Name : " << mat.name;
            cout << endl << "  - Ns   : " << mat.Ns;
            cout << endl << "  - Ka   : " << mat.Ka.r << " ," << mat.Ka.g << " ," << mat.Ka.b ;
            cout << endl << "  - Kd   : " << mat.Kd.r << " ," << mat.Kd.g << " ," << mat.Kd.b;
            cout << endl << "  - Ks   : " << mat.Ks.r << " ," << mat.Ks.g << " ," << mat.Ks.b;
            cout << endl << "  - Ke   : " << mat.Ke.r << " ," << mat.Ke.g << " ," << mat.Ke.b;
            cout << endl << "  - Ni   : " << mat.Ni;
            cout << endl << "  - d    : " << mat.d;
            cout << endl << "  - illum  : " << mat.illum;
            if (m_isUsingTextures)
            {
                cout << endl << "  - map_Kd : " << mat.map_Kd;
                // decoded texture infos :
                cout << endl << "    > Decoded Material Texture infos : ";
                cout << endl << "      .Image Width x height :  " << mat.rawImage.width << " x " << mat.rawImage.height;
                cout << endl << "      .Image Data byte(s): " << mat.rawImage.data.size();
                cout << endl << "      .Image data bytes per pixel :  " << mat.rawImage.bytesPerPixel;                
            }
        }
    }

    /*
    cout << endl << "OBJ file    : " << m_objFileName;
    cout << endl << "Vertices    : " << m_vertices.size();
    cout << endl << "Normals     : " << m_normals.size();
    cout << endl << "Vert. UVs   : " << m_uvs.size();
    cout << endl << "Obj. Face(s): " << m_objFaces.size();

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
            cout << endl << "UV.#" << i << " : " << m_uvs[i].u << ", " << m_uvs[i].v;
        }

        for (int i = 0; i < m_objFaces.size(); i++)
        {
            cout << endl << "Face.#" << i << " : " << endl << "  -vertex count = " << m_objFaces[i].vertexIndices.size()
                << endl << "  -UVs     count = " << m_objFaces[i].uvIndices.size()
                << endl << "  -Normals count = " << m_objFaces[i].normalIndices.size();
        }

        cout << endl << "---------------------------";
    }
*/
}



