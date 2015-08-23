/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/Buffer.hpp"
#include "Graphics/GLManager.hpp"
#include "IO/LogManager.hpp"

using namespace kke;

Buffer::Buffer(bool gen, GLenum defaultTarget) : 
	  handle(0),
	  size(0),
	  mapped(0),
	  defaultTarget(defaultTarget)
{
	if (gen)
	  Generate();;
}

Buffer::~Buffer()
{
	Delete();
}

GLenum Buffer::GetTarget() const
{
	return defaultTarget;
}

void Buffer::SetTarget (GLenum target)
{
	defaultTarget = target;
}

void Buffer::BufferData (GLsizei size, GLvoid* data, GLenum usage)
{
	BufferData(size, data, usage, defaultTarget);
}

void Buffer::BufferSubData (GLint offset, GLsizei size, GLvoid* data)
{
	BufferSubData(offset, size, data, defaultTarget);
}

void Buffer::BufferData(GLsizei size, GLvoid* data, GLenum usage, GLenum target)
{
	this->size = size;
	glBufferData(target, size, data, usage);
    
    GLenum err;
    if (GLException::GetGLError(err))
        throw new BufferGLException(err, target, "BufferData");
}

void Buffer::BufferSubData(GLint offset, GLsizei size, GLvoid* data, GLenum target)
{
	glBufferSubData(target, offset, size, data);
    
    GLenum err;
    if (GLException::GetGLError(err))
        throw new BufferGLException(err, target, "BufferSubData");
}

bool Buffer::Generate()
{
	if (handle == 0)
        glGenBuffers(1, &handle);
    
    return true;
}

bool Buffer::Delete()
{
	if (handle != 0)
	{
		glDeleteBuffers(1, &handle);
		size = 0;
	}
	
	return true;
}

void* Buffer::Map (GLenum access, GLenum target)
{
	if (mapped != nullptr)
	{
		LOGW() << "BufferObject map wasn't unmapped since last Map() call.";
	}
	
	mapped = glMapBuffer(target, access);
	
	if (mapped == nullptr)
		throw new BufferGLException(GL_NO_ERROR, target, "Failed mapping buffer (nullptr).");
	
	GLenum err;
    if (GLException::GetGLError(err))
        throw new BufferGLException(err, target, "BufferSubData");
	
	return mapped;
}

void Buffer::Unmap(GLenum target)
{
	mapped = 0;
	glUnmapBuffer(target);
}

size_t Buffer::GetSize() const
{
	return size;
}

// Non-member
void GenerateBOs(Buffer** data, int amount)
{
	GLuint handles[amount];
	glGenBuffers(amount, handles);
	
	for (int i = 0; i < amount; i++)
	{
		data[i] = new Buffer(false);
		data[i]->SetHandle(handles[i]);
	}
}

// Non-member
void DeleteBOs (Buffer** data, int amount)
{
	for (int i = 0; i < amount; i++)
		delete data[i];
}

BufferGLException::BufferGLException(GLenum error, GLenum source, const std::string& reason) noexcept : 
    GLException(error, bufferToString(source), reason)
{
    
}

BufferGLException::~BufferGLException() noexcept { }

std::string BufferGLException::bufferToString(GLenum buffer) noexcept
{
    switch (buffer)
    {
        case GL_ARRAY_BUFFER:
            return "GL_ARRAY_BUFFER";
            
        case GL_ELEMENT_ARRAY_BUFFER:
            return "GL_ELEMENT_ARRAY_BUFFER";
            
        case GL_PIXEL_PACK_BUFFER:
            return "GL_PIXEL_PACK_BUFFER";
            
        case GL_PIXEL_UNPACK_BUFFER:
            return "GL_PIXEL_UNPACK_BUFFER";
            
        default:
            return "UNKNOWN BUFFER";
    };
}