#include "mini_mesh.h"
#include <algorithm>
using namespace std;
using namespace mini;

void Mesh::Render(VBGL::ShaderProgram* shader/*const dx_ptr<ID3D11DeviceContext>& context*/)
{
    if (m_geometry->indicesCount() == 0)
    {
        int a = 5;
    }
    if (m_geometry->indicesCount() != 0)
        m_geometry->indicedDraw(shader);
//	if (!m_indexBuffer || !m_vertexBuffers)
//		return;
//	context->IASetIndexBuffer(m_indexBuffer.get(), DXGI_FORMAT_R16_UINT, 0);
//	context->IASetVertexBuffers(0, m_buffersCount, m_vertexBuffers.get(), m_strides.get(), m_offsets.get());
//	context->DrawIndexed(m_indexCount, 0, 0);
}

Mesh::Mesh()
    : m_materialIdx(UINT_MAX)
{
    m_geometry = new VBGL::Geometry;
}

Mesh::Mesh(std::vector<glm::vec3>&& positions,
           std::vector<glm::vec2>&& texCoords,
           std::vector<unsigned short>&& pindices,
           std::vector<glm::vec3>&& normals,
           unsigned int materialIdx)
    : m_materialIdx(materialIdx)
{
    m_geometry = new VBGL::Geometry;
    m_geometry->verticesGetVector() = positions;
    m_geometry->UVsGetVector() = texCoords;
    m_geometry->indicesGetVector() = pindices;
    m_geometry->normalsGetVector() = normals;
    m_geometry->updateBuffers();
}

Mesh::~Mesh()
{
	Release();
}

void Mesh::Release()
{
    m_geometry = new VBGL::Geometry;
    m_materialIdx = UINT_MAX;
}

Mesh& Mesh::operator=(Mesh&& right)
{
	Release();
    m_geometry = move(right.m_geometry);
    m_geometry->updateBuffers();
	m_materialIdx = right.m_materialIdx;
	right.Release();
	return *this;
}

Mesh::Mesh(Mesh&& right)
    : m_geometry(move(right.m_geometry)),
      m_materialIdx(right.m_materialIdx)
{
    m_geometry->updateBuffers();
	right.Release();
}
