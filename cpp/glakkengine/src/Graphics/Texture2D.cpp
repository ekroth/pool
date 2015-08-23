/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/Texture2D.hpp"
#include "IO/LogManager.hpp"

#include <glm/glm.hpp>
#include <IL/ilu.h>

#include "Graphics/GLManager.hpp"

using namespace kke;

using std::string;

Texture2D::Texture2D(
	GLuint handle,
	GLuint width,
	GLuint height,
	GLenum format,
	const string& path) :
	ResourceType(),
	width(width),
	height(height),
	format(format),
	mipmaps(true)
{
	Handled::SetHandle(handle);
	this->source = path;
}

Texture2D::Texture2D (bool generate) :
	ResourceType(),
	handle(0),
	width(0),
	height(0),
	format(0),
	mipmaps(true)
{
	if (generate)
		Generate();
}

Texture2D::Texture2D (GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data) : 
	ResourceType()
{
	Generate();
	Bind();
	Allocate(level, internalFormat, width, height, border, format, type, data);
}

Texture2D::~Texture2D()
{
	Unload();
}

bool Texture2D::Load(const string& path)
{
	return Load(path, true);
}

bool Texture2D::Load(const string& path, bool flip)
{
	// Create il handle
	ILuint lhandle;
	ilGenImages(1, &lhandle);
	ilBindImage(lhandle);
	
// 	Activate(GL_TEXTURE0);
// 	TextureUnit(true);
	
	if (ilLoadImage(path.c_str()) == IL_TRUE)
	{
		// Flip if needed
		if (flip)
			iluFlipImage();
		
		glGenTextures(1, &handle);
		Bind();
		
		// Get info
		ILinfo info;
		iluGetImageInfo(&info);
		width = info.Width;
		height = info.Height;
		format = ILToGL(info.Format);
		
		// Upload
		Allocate(0, format, width, height, 0, format, GL_UNSIGNED_BYTE, ilGetData());
	}

	if (handle == 0)
	{
		// Error occured.
		LOGE() << "Error loading texture.\n";
	}

	// Set resource path
	source = path;
	
	// Delete il data
	ilDeleteImages(1, &lhandle);
	
	if (mipmaps)
		GenerateMipmaps();
	else
	{
		SetParameterf(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		SetParameterf(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	// Return if it was successful.
	return handle != 0;
}

ResourceType* Texture2D::Copy() const
{
	Texture2D *copy = new Texture2D(handle, width, height, format, source);
	return copy;
}

void Texture2D::SetHandle(
	GLuint handle,
	GLuint width,
	GLuint height,
	GLenum format,
	const string& path)
{
	// Set values
	this->handle = handle;
	this->width = width;
	this->height = height;
	this->format = format;
	this->source = path;
}

void Texture2D::Allocate (GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data)
{
	glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, border, format, type, data);
	this->width = width;
	this->height = height;
	this->format = format;
}

void Texture2D::AllocateSub (GLint level, GLsizei x, GLsizei y, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data)
{
	glTexSubImage2D(GL_TEXTURE_2D, level, x, y, width, height, format, type, data);
}

bool Texture2D::Generate()
{
	if (!Generated())	
		glGenTextures(1, &handle);
	
	return true;
}

bool Texture2D::Delete()
{
	if (Generated())
	{
		glDeleteTextures(1, &handle);
		handle = 0;
	}
	
	return true;
}

bool Texture2D::Unload()
{
	if (Generated())
	{
		GfxCache::Instance().SetTexture(GL_TEXTURE_2D, 0);
		Delete();
		width = 0;
		height = 0;
		format = 0;
	}

	return handle == 0;
}

GLuint Texture2D::GetWidth() const
{
	return width;
}

GLuint Texture2D::GetHeight() const
{
	return height;
}

GLenum Texture2D::GetFormat() const
{
	return format;
}

void Texture2D::SetParameterf (GLenum pname, float param)
{
	glTexParameterf(GL_TEXTURE_2D, pname, param);
}

void Texture2D::SetParameteri (GLenum pname, int param)
{
	glTexParameteri(GL_TEXTURE_2D, pname, param);
}

void Texture2D::GenerateMipmaps()
{
	SetParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
	SetParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
	SetParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SetParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	glGenerateMipmap(GL_TEXTURE_2D);
}

bool Texture2D::Save(const string& path) const
{
    LOGASSERT(false, "Saving not supported!");
	return false;
}

GLenum Texture2D::ILToGL (ILenum value) const
{
	switch (value)
	{
		case IL_RGBA:
			return GL_RGBA;
			break;
			
		case IL_RGB:
			return GL_RGB;
			break;
			
		case IL_BGR:
			return GL_BGR;
			break;
	}
	
	return GL_RGBA;
}