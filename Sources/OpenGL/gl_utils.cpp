#include "gl_utils.h"

#include <iostream>
#include <iomanip>
using namespace glm;


std::ostream &operator<<(std::ostream &o, const dvec2 &m)
{
    for(int i=0;i<2;++i) {
        o << std::setw(15) << m[i]<<" ";
    }
    return o;
}

std::ostream &operator<<(std::ostream &o, const dvec3 &m)
{
    for(int i=0;i<3;++i) {
        o << std::setw(15) << m[i]<<" ";
    }
    return o;
}

std::ostream &operator<<(std::ostream &o, const dvec4 &m)
{
    for(int i=0;i<4;++i) {
        o << std::setw(15) << m[i]<<" ";
    }
    return o;
}

std::ostream &operator<<(std::ostream &o, const vec2 &m)
{
    for(int i=0;i<2;++i) {
        o << std::setw(15) << m[i]<<" ";
    }
    return o;
}

std::ostream &operator<<(std::ostream &o, const vec3 &m)
{
    for(int i=0;i<3;++i) {
        o << std::setw(15) << m[i]<<" ";
    }
    return o;
}

std::ostream &operator<<(std::ostream &o, const vec4 &m)
{
    for(int i=0;i<4;++i) {
        o << std::setw(15) << m[i]<<" ";
    }
    return o;
}

std::ostream &operator<<(std::ostream &o, const mat4 &m)
{
    mat4 n=glm::transpose(m);
    for(int i=0;i<4;++i) {
        o << n[i]<< "\n";
    }
    return o;
}

std::ostream &operator<<(std::ostream &o, const mat3 &m)
{
    mat3 n=glm::transpose(m);
    for(int i=0;i<3;++i) {
        o << n[i]<< "\n";
    }
    return o;
}
