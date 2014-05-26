#include "util.h"
#include <stdarg.h>
#include <fstream>


//TODO: Rewrite to be usable for MS Visual Studio

void DebugLog(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
    fprintf(stderr, "\n");
}

void DebugLogV(const char *m)
{
    fprintf(stderr, "%s\n", m);
}

std::string loadFile(const char *filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in)
    {
      std::string contents;
      in.seekg(0, std::ios::end);
      contents.resize(in.tellg());
      in.seekg(0, std::ios::beg);
      in.read(&contents[0], contents.size());
      in.close();
      return(contents);
    }
    throw(errno);
}
