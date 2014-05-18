#include "util.h"
#include <stdarg.h>


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
