#include "arrayBuffer.h"

using namespace VBGL;

ArrayBufferObject::ArrayBufferObject() {
}

ArrayBufferObject::~ArrayBufferObject() {
    dealloc();
}

void ArrayBufferObject::alloc() {
    if (valid) dealloc();
    glGenVertexArrays(1, &vaID);
    CHECK_GL_ERRORS("RawArrayBufferObject alloc");
    valid = true;
}

void ArrayBufferObject::dealloc() {
    if (!valid)
        return;
    release();
    glDeleteVertexArrays(1, &vaID);
    CHECK_GL_ERRORS("RawArrayBufferObject dealloc");
    valid = false;
}

void ArrayBufferObject::bind(){
    if (!valid)
        alloc();
    glBindVertexArray(vaID);
    CHECK_GL_ERRORS("RawArrayBufferObject bind");
}

void ArrayBufferObject::release(){
    glBindVertexArray(0);
    CHECK_GL_ERRORS("RawArrayBufferObject release");
}
