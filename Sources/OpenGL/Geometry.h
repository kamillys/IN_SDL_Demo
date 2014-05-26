#pragma once

#include "arrayBuffer.h"
#include "shader.h"
#include <vector>
#include <memory>

#define GEOMETRY_INDICES_DATA \
    X(unsigned short, indices)

#define GEOMETRY_VERTEX_DATA \
    X(glm::vec3, vertices) \
    X(glm::vec3, normals) \
    X(glm::vec2, UVs)


#define GEOMETRY_VECTORED_DATA \
    GEOMETRY_INDICES_DATA \
    GEOMETRY_VERTEX_DATA

namespace VBGL
{

class Geometry : public VTF::RefObject
{
#define X(type, name) \
    std::vector<type> _##name; \
    bool _##name##NeedsUpdate = false;
    GEOMETRY_VECTORED_DATA
#undef X
public:
    ArrayBufferObject _abo;
#define X(type, name) \
    VertexBuffer _##name##Buffer;
    GEOMETRY_VERTEX_DATA
#undef X
#define X(type, name) \
    IndexBuffer _##name##Buffer;
    GEOMETRY_INDICES_DATA
#undef X

    GLenum _type = GL_TRIANGLES;
    void bindData(std::vector<GLint>&, ShaderProgram *shader);
    //TODO: boundingBox and boundingSphere

public:
    Geometry();
    virtual ~Geometry();

#define X(type, name) \
    size_t name##Count() const; \
    type name(int i); \
    void name##Resize(int i); \
    void name##Append(const type& vec); \
    void name##Append(const std::vector<type>& vec); \
    void name##Set(int i, const type& vec); \
    void name##Set(const std::vector<type>& vec); \
    void name##Clear(); \
    std::vector<type>& name##GetVector(); \
    void name##Bind(); \
    void name##Release();
    GEOMETRY_VECTORED_DATA
#undef X

    void setGLType(GLenum type) { _type = type; }

    void updateBuffers();
    void draw(ShaderProgram *shader);
    void indicedDraw(ShaderProgram* shader);
};

}
