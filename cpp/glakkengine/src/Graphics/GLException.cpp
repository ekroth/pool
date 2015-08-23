/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/GLException.hpp"

using namespace kke;

using std::string;

GLException::GLException(GLenum error, const string& src, const string& reason) noexcept :
    msg("OpenGL Exception: ")
{
    msg += errorToString(error) + ". Source: " + src + ". " + reason;
}

GLException::~GLException() noexcept
{

}

const char* GLException::what() const noexcept
{
    return msg.c_str();
}

string GLException::errorToString(GLenum err) noexcept
{    
    switch (err)
    {
        case GL_NO_ERROR:
            return "GL_NO_ERROR";
            
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
            
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
            
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
            
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
            
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
            
        case GL_STACK_OVERFLOW:
            return "GL_STACK_OVERFLOW";
            
        case GL_STACK_UNDERFLOW:
            return "GL_STACK_UNDERFLOW";
            
        case GL_TABLE_TOO_LARGE:
            return "GL_TABLE_TOO_LARGE";
            
        default:
            return "UNKNOWN";
    };
}

bool GLException::GetGLError(GLenum& error)
{
    error = glGetError();    
    return error != GL_NO_ERROR;
}