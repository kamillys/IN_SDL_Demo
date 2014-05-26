#include "GLCaps.h"
#include "myGL.h"
#include <set>
#include <algorithm>

using namespace VBGL;

static std::set<std::string> getExtensions()
{
    std::set<std::string> extensions;
    const GLubyte* glex = glGetString(GL_EXTENSIONS);
    std::stringstream exstream;
    exstream << (const char*) glex;
    while(true) {
        std::string extension;
        exstream >> extension;
        if(extension.length() == 0) break;
        extensions.insert(extension);
    }
    //std::cerr << extensions.size() << " extensions" << std::endl;
    return extensions;
}

static bool checkExtension(const std::set<std::string> es, const char* e)
{
    return (std::find(es.begin(), es.end(), e) != es.end());
}

GLCaps::GLCaps()
{
    std::set<std::string> extensions = getExtensions();
    size_t count = 0;

#define X(extension) \
    m_has_##extension = checkExtension(extensions, #extension); \
    if (m_has_##extension) ++count;

    GLCAPS_EXTENSION_LIST(X);
#undef X

    /**  Detection of new extensions. For development purposes **/

    if (count != extensions.size()) {
        std::cerr << "Detected new extensions!\n";
#define X(extension) \
        extensions.erase(#extension);

        GLCAPS_EXTENSION_LIST(X);
#undef X

        for(std::string e : extensions) {
            std::cerr << "-> " << e;
        }
    }
}

void GLCaps::printExtensions()
{
    std::cout << "Extensions: \n";

#define X(extension) \
    if(m_has_##extension) \
        std::cout << "->" #extension "\n";

    GLCAPS_EXTENSION_LIST(X);
#undef X

    std::cout.flush();
}
