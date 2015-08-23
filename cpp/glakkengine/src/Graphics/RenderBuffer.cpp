/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/RenderBuffer.hpp"

using namespace kke;

RenderBuffer::RenderBuffer (bool gen) : 
	internalFormat(0),
	width(0),
	height(0)
{
	if (gen)
		Generate();
}

RenderBuffer::~RenderBuffer()
{
	Delete();
}

bool RenderBuffer::Generate()
{
	glGenRenderbuffers(1, &handle);
	return true;
}

bool RenderBuffer::Delete()
{
	if (handle != 0)
	{
		glDeleteRenderbuffers(1, &handle);
		handle = 0;
		internalFormat = 0;
		width = 0;
		height = 0;
	}
	
	return true;
}

void RenderBuffer::Allocate (GLenum internalFormat, GLsizei width, GLsizei height)
{
	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
	this->internalFormat = internalFormat;
	this->width = width;
	this->height = height;
	
	GLenum err;
	if (GLException::GetGLError(err))
		throw new RenderBufferGLException(err, "RenderbufferStorage");
}

GLenum RenderBuffer::GetInternalFormat() const
{
	return internalFormat;
}

GLenum RenderBuffer::GetWidth() const
{
	return width;
}

GLenum RenderBuffer::GetHeight() const
{
	return height;
}