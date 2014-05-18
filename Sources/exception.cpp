#include "exception.h"

/**
 * Exception handling system.
 * Author: Kamil Łysik
 * e-Mail: kamillys@gmail.com
 *
 * Licence: GNU LESSER GENERAL PUBLIC LICENSE
 * Version 3, 29 June 2007
 *
 */


using namespace VTF;

Exception::Exception(const char *message, int exitCode)
    : m_message(message),
      m_exitCode(exitCode)
{
}

int Exception::getExitCode() const
{
    return m_exitCode;
}

std::string Exception::getMessage() const
{
    return m_message;
}
