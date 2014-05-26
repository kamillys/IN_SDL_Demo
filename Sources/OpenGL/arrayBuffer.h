#pragma once

#include <VTF/Pointers>
#include "myGL.h"
#include <vector>

namespace VBGL
{

template <int _GLType>
class RawVertexBufferObject {
    GLuint _vboID;
    bool valid = false;
    size_t _count = 0;
public:
    RawVertexBufferObject() {
    }

    ~RawVertexBufferObject() {
        dealloc();
    }

    void alloc() {
        if (valid) dealloc();
        _count = 0;
        glGenBuffers(1, &_vboID);
        CHECK_GL_ERRORS("VertexBufferObject alloc");
        valid = true;
    }

    void dealloc() {
        if (!valid)
            return;
        release();
        glDeleteBuffers(1, &_vboID);
        CHECK_GL_ERRORS("VertexBufferObject dealloc");
        valid = false;
    }

    size_t count() const { return _count; }
    void bind()
    {
        if (!valid) {
            THROW_GRAPHICS_EXCEPTION("Using invalid VBO!");
        }
        glBindBuffer(_GLType, _vboID);
        CHECK_GL_ERRORS("VertexBufferObject bind");
    }

    void release()
    {
        glBindBuffer(_GLType, 0);
        CHECK_GL_ERRORS("VertexBufferObject release");
    }

    void bufferStaticData(size_t elemsize, size_t count, const void *data)
    {
        if(!valid)
            alloc();
        bind();
        _count = count;
        glBufferData(_GLType,  elemsize * count, data, GL_STATIC_DRAW);
        CHECK_GL_ERRORS("VertexBufferObject bufferStaticData");
    }

    template<typename T>
    void bufferStaticData(const std::vector<T>& data)
    {
        bufferStaticData(sizeof(T), data.size(), data.data());
    }

    void vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized = GL_FALSE, GLsizei stride = 0, const GLvoid* pointer = 0)
    {
        if(!valid)
            alloc();
        bind();
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
        CHECK_GL_ERRORS("VertexBufferObject vertexAttribPointer");
    }
};

typedef RawVertexBufferObject<GL_ARRAY_BUFFER> VertexBuffer;
typedef RawVertexBufferObject<GL_ELEMENT_ARRAY_BUFFER> IndexBuffer;

class ArrayBufferObject {
    GLuint vaID;
    bool valid = false;
public:
    ArrayBufferObject();
    ~ArrayBufferObject();
    void alloc();
    void dealloc();
    void bind();
    void release();
};

}
