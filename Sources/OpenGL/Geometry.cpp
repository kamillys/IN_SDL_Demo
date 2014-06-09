#include "Geometry.h"

using namespace VBGL;

Geometry::Geometry()
{
	
#define X(type, name) \
    _##name##NeedsUpdate = false;

    GEOMETRY_VECTORED_DATA
#undef X
    _type = GL_TRIANGLES;
}

Geometry::~Geometry()
{
}

#define X(type, name) \
type Geometry::name(int i) \
{ \
    /*TODO: CHECKS*/ \
    return _##name [i]; \
} \
void Geometry::name##Append(const type& vec) \
{ \
    _##name.push_back(vec); \
    _##name##NeedsUpdate = true; \
} \
void Geometry::name##Append(const std::vector<type>& vec) \
{ \
    _##name.insert(_##name.end(), vec.begin(), vec.end());\
    _##name##NeedsUpdate = true; \
} \
size_t Geometry::name##Count() const \
{ \
    return _##name.size(); \
} \
void Geometry::name##Resize(int i) \
{ \
    _##name.resize(i); \
    _##name##NeedsUpdate = true; \
} \
void Geometry::name##Set(int i, const type &vec) \
{\
    /*TODO: CHECKS*/ \
    _##name[i] = vec; \
    _##name##NeedsUpdate = true; \
} \
void Geometry::name##Set(const std::vector<type>& vec) \
{ \
    _##name = vec; \
    _##name##NeedsUpdate = true; \
} \
void Geometry::name##Clear() \
{ \
    _##name.clear(); \
    _##name##NeedsUpdate = true; \
} \
std::vector<type>& Geometry::name##GetVector() \
{ \
    _##name##NeedsUpdate = true; \
    return _##name ; \
} \
void Geometry::name##Bind() \
{ \
    _##name##Buffer.bind();\
} \
void Geometry::name##Release() \
{\
    _##name##Buffer.release();\
}

GEOMETRY_VECTORED_DATA
#undef X

void Geometry::updateBuffers()
{
    _abo.bind();
#define X(type, name) \
    if(_##name##NeedsUpdate) { \
        if(_##name.size() == 0) { \
            _##name##Buffer.dealloc(); \
        } else { \
    /** DEBUG **/ /** fprintf(stderr, "Uploading buffer data: " #name "\n"); **/ \
            _##name##Buffer.bufferStaticData(_##name); \
        } \
        _##name##NeedsUpdate = false; \
    }
    GEOMETRY_VECTORED_DATA
#undef X
    _abo.release();
}

void Geometry::bindData(std::vector<GLint> &boundLocations, ShaderProgram* shader)
{
    updateBuffers();
    UNUSED(shader);
    UNUSED(boundLocations);
#define X(type, name) \
    if(_##name##Buffer.count()) { \
        GLint location = shader->getAttribLocation("in_" #name ); \
        /** fprintf(stderr, "Using " #name " with location %i\n", location); **/ \
        if (location != -1) { \
            _##name##Buffer.vertexAttribPointer(location, GLSizeInfo<type>::vectorLength, \
                                                GLSizeInfo<type>::glType); \
            glEnableVertexAttribArray(location); \
            boundLocations.push_back(location); \
        }\
    }
    GEOMETRY_VERTEX_DATA
#undef X
}

void Geometry::draw(ShaderProgram* shader)
{
    _abo.bind();
    std::vector<GLint> boundLocations;

    bindData(boundLocations, shader);

    glDrawArrays(_type, 0, verticesCount());
    CHECK_GL_ERRORS("Draw Arrays");

    for(unsigned i=0;i<boundLocations.size();++i)
        glDisableVertexAttribArray(boundLocations[i]);
    _abo.release();
}

void Geometry::indicedDraw(ShaderProgram *shader)
{
    _abo.bind();
    std::vector<GLint> boundLocations;

    //bind data
    bindData(boundLocations, shader);

    indicesBind();
    glDrawElements(_type, indicesCount(), GL_UNSIGNED_SHORT, NULL);
    CHECK_GL_ERRORS("Draw Indexed Elements");
    indicesRelease();

    for(unsigned i=0;i<boundLocations.size();++i)
        glDisableVertexAttribArray(boundLocations[i]);
    _abo.release();
}
