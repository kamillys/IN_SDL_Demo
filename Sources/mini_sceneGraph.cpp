#include "mini_sceneGraph.h"

using namespace std;
using namespace mini;
using namespace glm;

SceneGraph::SceneGraph(vector<SceneNode>&& nodes, vector<Mesh>&& meshes, vector<Material>&& materials)
	: m_nodes(move(nodes)), m_meshes(move(meshes)), m_materials(move(materials))
{
    UpdateChildTransforms(0, mat4());
}

SceneGraph::SceneGraph(SceneGraph&& right)
	: m_nodes(move(right.m_nodes)), m_meshes(move(right.m_meshes)), m_materials(move(right.m_materials))
{
	right.Clear();
}

SceneGraph& SceneGraph::operator=(SceneGraph&& right)
{
	Clear();
	m_nodes = move(right.m_nodes);
	m_meshes = move(right.m_meshes);
	m_materials = move(right.m_materials);
	right.Clear();
	return *this;
}

void SceneGraph::Clear()
{
	m_nodes.clear();
	m_meshes.clear();
}

int SceneGraph::nodeByName(const std::string& name) const
{
	for (unsigned int i = 0; i < m_nodes.size(); ++i)
		if(!m_nodes[i].m_name.empty() && m_nodes[i].m_name == name)
			return i;
	return -1;
}

mat4 SceneGraph::getNodeTransform(unsigned nodeIndex) const
{
	if (nodeIndex >= m_nodes.size())
        return mat4();
	return m_nodes[nodeIndex].m_localTransform;
}

void SceneGraph::setNodeTransform(unsigned int nodeIndex, const mat4& transform)
{
	if (nodeIndex >= m_nodes.size())
		return;
	m_nodes[nodeIndex].m_localTransform = transform;
	int parentIndex = m_nodes[nodeIndex].m_parent;
    mat4 parentTransform;
	if (parentIndex == -1)
        parentTransform = mat4();
	else 
        parentTransform = m_nodes[parentIndex].m_transform;
    mat4 myTransform = parentTransform*transform;
    m_nodes[nodeIndex].m_transform = myTransform;
	int meshIdx = m_nodes[nodeIndex].m_mesh;
	if (meshIdx != -1)
		m_meshes[meshIdx].setTransform(m_nodes[nodeIndex].m_transform);
	UpdateChildTransforms(m_nodes[nodeIndex].m_firstChild, myTransform);
}

int SceneGraph::nodeFirstChild(unsigned int nodeIndex) const
{
	if (nodeIndex >= m_nodes.size())
		return -1;
	return m_nodes[nodeIndex].m_firstChild;
}

int SceneGraph::nodeNextSybling(unsigned int nodeIndex) const
{
	if (nodeIndex >= m_nodes.size())
		return -1;
	return m_nodes[nodeIndex].m_nextSybling;
}

int SceneGraph::nodeParent(unsigned int nodeIndex) const
{
	if (nodeIndex >= m_nodes.size())
		return -1;
	return m_nodes[nodeIndex].m_parent;
}

void SceneGraph::UpdateChildTransforms(unsigned int childIndex, mat4 parentTransform)
{
    while(childIndex >= 0 && childIndex < m_nodes.size())
	{
        mat4 my = m_nodes[childIndex].m_localTransform;
        my = parentTransform * my;
        m_nodes[childIndex].m_transform = my;
		int meshIdx = m_nodes[childIndex].m_mesh;
        if (meshIdx >=0 && meshIdx < m_meshes.size())
			m_meshes[meshIdx].setTransform(m_nodes[childIndex].m_transform);
		UpdateChildTransforms(m_nodes[childIndex].m_firstChild, my);
		childIndex = m_nodes[childIndex].m_nextSybling;
	}
}
