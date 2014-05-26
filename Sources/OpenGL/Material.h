#pragma once

#include <VTF/Pointers>
#include <OpenGL/Core>

namespace VBGL {

class Material : public VTF::RefObject
{
protected:
    VTF::RefPointer<ShaderProgram> shader;
public:
    virtual ~Material(){}

    virtual void bind() {}
    virtual void release() {}
    ShaderProgram* getShader() { return shader; }
};

}
