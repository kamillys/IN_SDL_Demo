#include "shader.h"
#include <exception.h>
#include <iostream>

using namespace VBGL;

static GLuint loadShader(const char* src, GLenum type);

ShaderProgram::ShaderProgram() : compiled(false)
{
}

ShaderProgram::~ShaderProgram()
{
    release();
    removeShaders();
    destroy();
    CHECK_GL_ERRORS("Shader dtor");
}

void ShaderProgram::addShader(const char *src, ShaderProgram::ShaderType type)
{
    const char* shaderName = "";
    GLenum shaderType;
    switch(type)
    {
    case ShaderProgram::VertexShader:
        shaderType = GL_VERTEX_SHADER;
        shaderName = "Vertex Shader";
        break;
    case ShaderProgram::TessellationControlShader:
        shaderType = GL_TESS_CONTROL_SHADER;
        shaderName = "Tessellation Control Shader";
        break;
    case ShaderProgram::TessellationEvaluationShader:
        shaderType = GL_TESS_EVALUATION_SHADER;
        shaderName = "Tessellation Evaluation Shader";
        break;
    case ShaderProgram::GeometryShader:
        shaderType = GL_GEOMETRY_SHADER;
        shaderName = "Geometry Shader";
        break;
    case ShaderProgram::FragmentShader:
        shaderType = GL_FRAGMENT_SHADER;
        shaderName = "Fragment Shader";
        break;
    default:
        THROW_EXCEPTION("Unknown shader type!");
    }
    std::cerr << "Building shader " << shaderName << std::endl;
    programs.push_back(loadShader(src, shaderType));
    CHECK_GL_ERRORS("Shader add");
}

void ShaderProgram::build()
{
    release();
    //Compile
    GLint result;
    int logLength;

    shader = glCreateProgram();
    for (unsigned i=0;i<programs.size();++i)
        glAttachShader(shader, programs[i]);

    glLinkProgram(shader);

    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> errorMessage(std::max(logLength, int(1)));
    glGetProgramInfoLog(shader, logLength, NULL, &errorMessage[0]);
    if (result == GL_FALSE)
        THROW_EXCEPTION_T(errorMessage.data(), VTF::ShaderBuildException);

    compiled = true;
    removeShaders();
    CHECK_GL_ERRORS("Shader build");
}

void ShaderProgram::bind()
{
    if(!compiled)
        THROW_EXCEPTION("Binding not-compiled shader!");

    //Bind shader
    glUseProgram(shader);
    CHECK_GL_ERRORS("Shader bild");
}

void ShaderProgram::release()
{
    glUseProgram(0);
    CHECK_GL_ERRORS("Shader release");
}

void ShaderProgram::destroy()
{
    if (!compiled) return;
    glDeleteProgram(shader);
    compiled = false;
    CHECK_GL_ERRORS("Shader destoy");
}

GLuint ShaderProgram::getShader() const
{
    return shader;
}

void ShaderProgram::removeShaders()
{
    while(!programs.empty())
    {
        GLuint program = programs.back();
        programs.pop_back();
        glDeleteShader(program);
        CHECK_GL_ERRORS("Shader remove shader");
    }
}

GLint ShaderProgram::getAttribLocation(const char *name)
{
    if (!compiled)
        THROW_EXCEPTION("Shader must be compiled to get attribute location!");
    GLint retval = glGetAttribLocation(shader, name);
    CHECK_GL_ERRORS("Shader getAttribLocation");
    return retval;
}

GLint ShaderProgram::getUniformLocation(const char *name)
{
    if (!compiled)
        THROW_EXCEPTION("Shader must be compiled to get uniform location!");
    GLint retval = glGetUniformLocation(shader, name);
    CHECK_GL_ERRORS("Shader getUniformLocation");
    return retval;
}

#define X(type, proc) \
void ShaderProgram::setUniform(const char *name, const type& data, GLboolean transpose) { \
    bind(); \
    if (!compiled) \
        THROW_EXCEPTION("Shader must be compiled to set variables!"); \
    proc(getUniformLocation(name), 1, transpose, glm::value_ptr(data)); \
    CHECK_GL_ERRORS("Shader setUniform " #type); \
}
_PRIVATE_SHADER_UNIFORM_TYPE_MAT
#undef X

#define X(type, proc) \
void ShaderProgram::setUniform(const char *name, const type& data) { \
    bind(); \
    if (!compiled) \
        THROW_EXCEPTION("Shader must be compiled to set variables!"); \
    proc(getUniformLocation(name), 1, glm::value_ptr(data)); \
    CHECK_GL_ERRORS("Shader setUniform " #type); \
}
_PRIVATE_SHADER_UNIFORM_TYPE_VEC
#undef X

static GLuint loadShader(const char* src, GLenum type) {
    int logLength;
    GLint result;
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

    std::vector<char> errorMessage(std::max(logLength, int(1)));
    glGetShaderInfoLog(shader, logLength, NULL, &errorMessage[0]);
    if (result == GL_FALSE)
        THROW_EXCEPTION_T(errorMessage.data(), VTF::ShaderBuildException);

    CHECK_GL_ERRORS("Shader loadShader");
    return shader;
}
