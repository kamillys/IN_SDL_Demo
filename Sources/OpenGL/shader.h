#pragma once

#include "myGL.h"
#include <VTF/Pointers>
#include <vector>
#include <map>

namespace VBGL
{

class ShaderProgram : public VTF::RefObject
{
public:
    enum ShaderType
    {
        VertexShader,
        TessellationControlShader,
        TessellationEvaluationShader,
        GeometryShader,
        FragmentShader
    };

    GLuint getShader() const;

    ShaderProgram();
    virtual ~ShaderProgram();

    void addShader(const char* src, ShaderType type);
    void build();

    void bind();
    void release();

    void destroy();
    void removeShaders();

    GLint getAttribLocation(const char* name);
    GLint getUniformLocation(const char* name);

#define _PRIVATE_SHADER_UNIFORM_TYPE_CONST \
    X(GLint, glUniform1i) \
    X(GLfloat, glUniform1f)

#define _PRIVATE_SHADER_UNIFORM_TYPE_MAT \
    X(glm::mat2, glUniformMatrix2fv) \
    X(glm::mat3, glUniformMatrix3fv) \
    X(glm::mat4, glUniformMatrix4fv)

#define _PRIVATE_SHADER_UNIFORM_TYPE_VEC \
    X(glm::uvec2, glUniform2uiv) \
    X(glm::uvec3, glUniform3uiv) \
    X(glm::uvec4, glUniform4uiv) \
    X(glm::ivec2, glUniform2iv) \
    X(glm::ivec3, glUniform3iv) \
    X(glm::ivec4, glUniform4iv) \
    X(glm::vec2, glUniform2fv) \
    X(glm::vec3, glUniform3fv) \
    X(glm::vec4, glUniform4fv)

#define X(type, proc) \
    void setUniform(const char* name, const type& data, GLboolean transpose = GL_FALSE);
    _PRIVATE_SHADER_UNIFORM_TYPE_MAT
#undef X
#define X(type, proc) \
    void setUniform(const char* name, const type& data);
    _PRIVATE_SHADER_UNIFORM_TYPE_VEC
    _PRIVATE_SHADER_UNIFORM_TYPE_CONST
#undef X


private:
    std::vector<GLuint> programs;
    GLuint shader;
    bool compiled;
};

}
