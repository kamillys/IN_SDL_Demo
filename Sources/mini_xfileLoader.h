#include "mini_mesh.h"
#include "mini_xfileToken.h"
#include "mini_sceneNode.h"
//#include "mini_dxDevice.h"
#include "mini_material.h"
#include <string>
#include <iostream>
#include <map>
#include <glm/glm.hpp>

namespace mini
{
	class XFileLoader
	{
	public:
        XFileLoader() { }

		std::vector<mini::Mesh> m_meshes;
		std::vector<mini::SceneNode> m_nodes;
		std::vector<mini::Material> m_materials;

		void Load(std::string filename);
	private:
		void CheckHeader(std::istream& s);
		int ReadSceneNode(std::istream& s, int prevSybling);
		int ReadMesh(std::istream& s);
        void ReadMeshNormals(std::istream&s, std::vector<glm::vec3>& normals, std::vector<unsigned short>& nindices);
        void ReadTexCoords(std::istream& s, std::vector<glm::vec2>& texCoords);
		int ReadMeshMaterials(std::istream& s);
		int ReadMaterial(std::istream& s);
		std::string ReadTextureFilename(std::istream&s);
        int CreateMesh(std::vector<glm::vec3>& positions, std::vector<glm::vec2>& texCoords,
                       std::vector<unsigned short>& indices, std::vector<glm::vec3>& normals, std::vector<unsigned short>& nindices, unsigned int materiaIdx);
        int CreateMaterial(const std::string& materialName, glm::vec4 diffuse, glm::vec4 specular, const std::string& textureFilename);
        glm::mat4 ReadFaceTransform(std::istream& s);
		void SkipDataObject(std::istream& s);
		void SkipDataReference(std::istream& s);

		template<class T>
		void ReadData(std::istream& s, T& d);

		template<class T>
		void ReadMember(std::istream& s, T& d);

		template<class iter>
		void ReadArray(std::istream& s, iter begin, iter end);

		void ReadFaceArray(std::istream& s, std::vector<unsigned short>& indices);

		void SkipToken(std::istream& s, XFileToken::Type type);

		std::map<std::string, unsigned int> m_materialLookup;
	};
}
