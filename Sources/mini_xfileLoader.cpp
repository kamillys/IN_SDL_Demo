#include "mini_xfileLoader.h"
#include "exception.h"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <map>

using namespace std;
using namespace VTF;
using namespace glm;

namespace mini {

void XFileLoader::Load(string filename)
{	
	m_nodes.clear();
	m_meshes.clear();
	m_materials.clear();
	m_materialLookup.clear();
	ifstream s;
	s.open(filename);
	if (!s)
		throw "Could not open file!";
	CheckHeader(s);
	int prevSybling = -1;
	while (true)
	{
		XFileToken token = XFileToken::NextToken(s);
		if (token.m_type == XFileToken::Identifier)
		{
			if (token.m_content == "Frame")
				prevSybling = ReadSceneNode(s, prevSybling);
			else if (token.m_content == "Mesh")
				ReadMesh(s);
			else
				SkipDataObject(s);
		}
		else if (token.m_type == XFileToken::LeftBrace)
			SkipDataReference(s);
		else if (token.m_type == XFileToken::None)
			break;
        else
            THROW_EXCEPTION_T("Parsing error", ParsingException);
	}
	s.close();
}

int XFileLoader::ReadSceneNode(istream& s, int prevSybling)
{
	int nodeIdx = m_nodes.size();
	m_nodes.push_back(SceneNode());
	if (prevSybling != -1)
		m_nodes[prevSybling].m_nextSybling = nodeIdx;
	XFileToken t = XFileToken::NextToken(s);
	if (t.m_type == XFileToken::Identifier)
	{
		m_nodes[nodeIdx].m_name = move(t.m_content);
		t = XFileToken::NextToken(s);
	}
    if (t.m_type != XFileToken::LeftBrace)
        THROW_EXCEPTION_T("Parsing error", ParsingException);
	int prevChild = -1;
	while (true)
	{
		t = XFileToken::NextToken(s);
		if (t.m_type == XFileToken::Identifier)
		{
			if (t.m_content == "Frame")
			{
				int child = ReadSceneNode(s, prevChild);
				if (prevChild == -1)
					m_nodes[nodeIdx].m_firstChild = child;
				m_nodes[child].m_parent = nodeIdx;
				prevChild = child;
			}
			else if (t.m_content == "Mesh")
				m_nodes[nodeIdx].m_mesh = ReadMesh(s);
			else if (t.m_content == "FrameTransformMatrix")
				m_nodes[nodeIdx].m_localTransform = ReadFaceTransform(s);
			else
				SkipDataObject(s);
		}
		else if (t.m_type == XFileToken::UUID)
			continue;
		else if (t.m_type == XFileToken::LeftBrace)
			SkipDataReference(s);
		else if (t.m_type == XFileToken::RightBrace)
			break;
        else
            THROW_EXCEPTION_T("Parsing error", ParsingException);
	}
	return nodeIdx;
}

template<>
void XFileLoader::ReadData<glm::mat4>(istream& s, glm::mat4& d)
{
	float elems[16];
    ReadArray(s, elems, elems + 16);
    d = make_mat4(elems);
	SkipToken(s, XFileToken::Semicolon);
}

mat4 XFileLoader::ReadFaceTransform(istream& s)
{
	XFileToken t = XFileToken::NextToken(s);
	if (t.m_type == XFileToken::Identifier)
	{
		t = XFileToken::NextToken(s);
	}
    if (t.m_type != XFileToken::LeftBrace)
        THROW_EXCEPTION_T("Parsing error", ParsingException);
    mat4 mtx;
	ReadData(s, mtx);
	SkipToken(s, XFileToken::RightBrace);
	return mtx;
}

void XFileLoader::SkipToken(istream& s, XFileToken::Type type)
{
	XFileToken t = XFileToken::NextToken(s);
    if (t.m_type != type)
        THROW_EXCEPTION_T("Parsing error", ParsingException);
}

template<>
void XFileLoader::ReadData<int>(istream& s, int& d)
{
	XFileToken t = XFileToken::NextToken(s);
    if (t.m_type != XFileToken::Integer)
        THROW_EXCEPTION_T("Parsing error", ParsingException);
	d = stoi(t.m_content);
}

template<>
void XFileLoader::ReadData<unsigned short>(istream& s, unsigned short& d)
{
	XFileToken t = XFileToken::NextToken(s);
    if (t.m_type != XFileToken::Integer)
        THROW_EXCEPTION_T("Parsing error", ParsingException);
	d = stoi(t.m_content);
}

template<>
void XFileLoader::ReadData<float>(istream& s, float& d)
{
	XFileToken t = XFileToken::NextToken(s);
    if (t.m_type != XFileToken::Float)
        THROW_EXCEPTION_T("Parsing error", ParsingException);
	d = stof(t.m_content);
}

template<>
void XFileLoader::ReadData<string>(istream& s, string& d)
{
	XFileToken t = XFileToken::NextToken(s);
    if (t.m_type != XFileToken::String)
        THROW_EXCEPTION_T("Parsing error", ParsingException);
	d = move(t.m_content);
}

template<>
void XFileLoader::ReadData<vec2>(istream& s, vec2& d)
{
	ReadMember(s, d.x);
	ReadMember(s, d.y);
}

template<>
void XFileLoader::ReadData<vec3>(istream& s, vec3& d)
{
	ReadMember(s, d.x);
	ReadMember(s, d.y);
	ReadMember(s, d.z);
}

template<>
void XFileLoader::ReadData<vec4>(istream& s, vec4& d)
{
	ReadMember(s, d.x);
	ReadMember(s, d.y);
	ReadMember(s, d.z);
	ReadMember(s, d.w);
}

template<class T>
void XFileLoader::ReadMember(istream& s, T& d)
{
	ReadData(s, d);
	SkipToken(s, XFileToken::Semicolon);
}

template<class iter>
void XFileLoader::ReadArray(istream& s, iter b, iter e)
{
	for (iter i = b; i != e; ++i)
	{
		if (i != b)
			SkipToken(s, XFileToken::Comma);
		ReadData(s, *i);
	}
	SkipToken(s, XFileToken::Semicolon);
}

void XFileLoader::ReadFaceArray(istream& s, vector<unsigned short>& indices)
{
	int nFaces;
	ReadMember(s, nFaces);
	indices.reserve(nFaces * 3);
	for (int i = 0; i < nFaces; ++i)
	{
		if (i != 0)
			SkipToken(s, XFileToken::Comma);
		int nIndices;
		ReadMember(s, nIndices);
		vector<int> faceIndices(nIndices);
		ReadArray(s, faceIndices.begin(), faceIndices.end());
		for (int i = 1; i < nIndices - 1; ++i)
		{
			indices.push_back(faceIndices[0]);
			indices.push_back(faceIndices[i]);
			indices.push_back(faceIndices[i + 1]);
		}
	}
	SkipToken(s, XFileToken::Semicolon);
}

void XFileLoader::ReadMeshNormals(istream&s, vector<vec3>& normals, vector<unsigned short>& nindices)
{
	XFileToken t = XFileToken::NextToken(s);
	if (t.m_type == XFileToken::Identifier)
	{
        //m_meshes[meshIdx].m_name = t.m_content;
		t = XFileToken::NextToken(s);
	}
    if (t.m_type != XFileToken::LeftBrace)
        THROW_EXCEPTION_T("Parsing error", ParsingException);
	int nElems;
	ReadMember(s, nElems);
	normals.resize(nElems);
	ReadArray(s, normals.begin(), normals.end());
	ReadFaceArray(s, nindices);
	SkipToken(s, XFileToken::RightBrace);
}

string XFileLoader::ReadTextureFilename(istream& s)
{
	XFileToken t = XFileToken::NextToken(s);
	if (t.m_type == XFileToken::Identifier)
		t = XFileToken::NextToken(s);
    if (t.m_type != XFileToken::LeftBrace)
        THROW_EXCEPTION_T("Parsing error", ParsingException);
	string fileName;
	ReadMember(s, fileName);
	SkipToken(s, XFileToken::RightBrace);
	return fileName;
}

int XFileLoader::CreateMaterial(const string& materialName, vec4 diffuse, vec4 specular, const string& textureFilename)
{
	if (m_materialLookup.find(materialName) != m_materialLookup.end())
		return m_materialLookup[materialName];
    int materialIdx = m_materials.size();
    m_materials.emplace_back(std::move(Material()));
	Material::MaterialData data = { diffuse, specular };
	m_materials[materialIdx].setMaterialData(data);
	m_materialLookup.insert(make_pair(move(materialName), materialIdx));
	if (!textureFilename.empty())
	{
        string fname(textureFilename.begin(), textureFilename.end());
        m_materials[materialIdx].setDiffuseTexture(move(VBGL::Texture2D::loadTexture(fname)));
		auto ext = fname.rfind(L'.');
        if (ext == fname.npos)
            THROW_EXCEPTION_T("Parsing error", ParsingException);
        string specName = fname.substr(0, ext) + "_Specular" + fname.substr(ext);
        m_materials[materialIdx].setSpecularTexture(move(VBGL::Texture2D::loadTexture(specName)));
	}
	return materialIdx;
}

int XFileLoader::ReadMaterial(istream& s)
{
	XFileToken t = XFileToken::NextToken(s);
	string materialName;
	if (t.m_type == XFileToken::Identifier)
	{
		materialName = move(t.m_content);
		t = XFileToken::NextToken(s);
	}
    if (t.m_type != XFileToken::LeftBrace)
        THROW_EXCEPTION_T("Parsing error", ParsingException);
    vec4 diffColor;
    vec3 specColor;
    vec3 emisColor;
	float pow;
	ReadMember(s, diffColor);
	ReadMember(s, pow);
	ReadMember(s, specColor);
	ReadMember(s, emisColor);
	string texFile;
	while (true)
	{
		t = XFileToken::NextToken(s);
		if (t.m_type == XFileToken::Identifier)
		{
			if ( t.m_content == "TextureFilename")
				texFile = ReadTextureFilename(s);
			else
				SkipDataObject(s);
		}
		else if (t.m_type == XFileToken::LeftBrace)
			SkipDataReference(s);
		else if (t.m_type == XFileToken::RightBrace)
			break;
        else
            THROW_EXCEPTION_T("Parsing error", ParsingException);
	}
    return CreateMaterial(materialName, diffColor, vec4(specColor.x, specColor.y, specColor.z, pow), texFile);
}

int XFileLoader::ReadMeshMaterials(istream& s)
{
	vector<int> materialIndices;
	int nMaterialIndices;
	int nMaterials;
	XFileToken t = XFileToken::NextToken(s);
	if (t.m_type == XFileToken::Identifier)
	{
		//m_meshes[meshIdx].m_name = t.m_content;
		t = XFileToken::NextToken(s);
	}
    if (t.m_type != XFileToken::LeftBrace)
        THROW_EXCEPTION_T("Parsing error", ParsingException);
	ReadMember(s, nMaterials);
	ReadMember(s, nMaterialIndices);
	materialIndices.resize(nMaterialIndices);
	ReadArray(s, materialIndices.begin(), materialIndices.end());
	SkipToken(s, XFileToken::Semicolon);
	unsigned int materialIdx = UINT_MAX;
	while (true)
	{
		t = XFileToken::NextToken(s);
		if (t.m_type == XFileToken::Identifier)
		{
			if (t.m_content == "Material" && materialIdx == UINT_MAX)
				materialIdx = ReadMaterial(s);
			else
				SkipDataObject(s);
		}
		else if (t.m_type == XFileToken::LeftBrace)
			SkipDataReference(s);
		else if (t.m_type == XFileToken::RightBrace)
			break;
        else
            THROW_EXCEPTION_T("Parsing error", ParsingException);
	}
	return materialIdx;
}

void XFileLoader::ReadTexCoords(istream& s, vector<vec2>& texCoords)
{
	XFileToken t = XFileToken::NextToken(s);
	if (t.m_type == XFileToken::Identifier)
		t = XFileToken::NextToken(s);
    if (t.m_type != XFileToken::LeftBrace)
        THROW_EXCEPTION_T("Parsing error", ParsingException);
	int nElems;
	ReadMember(s, nElems);
	texCoords.resize(nElems);
	ReadArray(s, texCoords.begin(), texCoords.end());
	SkipToken(s, XFileToken::RightBrace);
}

int XFileLoader::ReadMesh(istream& s)
{
    vector<vec3> positions;
    vector<vec2> texCoords;
	vector<unsigned short> indices;
    vector<vec3> normals;
	vector<unsigned short> nindices;
	XFileToken t = XFileToken::NextToken(s);
	if (t.m_type == XFileToken::Identifier)
	{
		//m_meshes[meshIdx].m_name = t.m_content;
		t = XFileToken::NextToken(s);
	}
    if (t.m_type != XFileToken::LeftBrace)
        THROW_EXCEPTION_T("Parsing error", ParsingException);
	int nElems;
	ReadMember(s, nElems);
	positions.resize(nElems);
	ReadArray(s, positions.begin(), positions.end());
	ReadFaceArray(s, indices);
	int materialIdx = -1;
	while (true)
	{
		t = XFileToken::NextToken(s);
		if (t.m_type == XFileToken::Identifier)
		{
			if (t.m_content == "MeshNormals")
				ReadMeshNormals(s, normals, nindices);
			else if(t.m_content == "MeshMaterialList")
				materialIdx = ReadMeshMaterials(s);
			else if(t.m_content == "MeshTextureCoords")
				ReadTexCoords(s, texCoords);
			else
				SkipDataObject(s);
		}
		else if (t.m_type == XFileToken::LeftBrace)
			SkipDataReference(s);
		else if (t.m_type == XFileToken::RightBrace)
			break;
        else
            THROW_EXCEPTION_T("Parsing error", ParsingException);
	}
    if (!texCoords.empty() && texCoords.size() != positions.size())
        THROW_EXCEPTION_T("Parsing error", ParsingException);
	return CreateMesh(positions, texCoords, indices, normals, nindices, materialIdx);
}


int XFileLoader::CreateMesh(vector<vec3>& positions,
                            vector<vec2>& texCoords,
                            vector<unsigned short>& pindices,
                            vector<vec3>& normals, vector<unsigned short>& nindices,
                            unsigned int materialIdx)
{
    if (pindices.size() != nindices.size())
        THROW_EXCEPTION_T("Parsing error", ParsingException);

    //Repack indices
    vector<vec3> _normals(nindices.size());
    for (unsigned int i=0; i < nindices.size(); ++i)
        _normals[i] = normals[nindices[i]];

    int meshIdx = m_meshes.size();
    m_meshes.push_back(Mesh(move(positions), move(texCoords), move(pindices), move(_normals), materialIdx));
	return meshIdx;
}

void XFileLoader::SkipDataObject(istream& s)
{
	XFileToken t = XFileToken::NextToken(s);
	if (t.m_type == XFileToken::Identifier)
		t = XFileToken::NextToken(s);
    if (t.m_type != XFileToken::LeftBrace)
        THROW_EXCEPTION_T("Parsing error", ParsingException);
	int leftBraceCount = 1;
	while (true)
	{
		t = XFileToken::NextToken(s);
		if (t.m_type == XFileToken::LeftBrace)
			++leftBraceCount;
		else if (t.m_type == XFileToken::RightBrace)
		{
			--leftBraceCount;
			if (leftBraceCount == 0)
				return;
		}
        else if (t.m_type == XFileToken::None)
            THROW_EXCEPTION_T("Parsing error", ParsingException);
	}
}

void XFileLoader::SkipDataReference(istream& s)
{
	XFileToken t = XFileToken::NextToken(s);
	if (t.m_type == XFileToken::Identifier)
		t = XFileToken::NextToken(s);
	if (t.m_type == XFileToken::UUID)
		t = XFileToken::NextToken(s);
	if (t.m_type != XFileToken::RightBrace)
		throw "Syntax error";
}

void XFileLoader::CheckHeader(istream& s)
{
	char header[17];
	if (!s.read(header, 16))
		throw "Read error!";
	header[16] = '\0';
	if (strcmp(header, "xof 0303txt 0032") != 0)
		throw "Unsuported file format!";
}

}
