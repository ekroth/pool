/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/ShaderProgram.hpp"
#include "Graphics/VertexShader.hpp"
#include "ResourceFactory.hpp"
#include "Resource.hpp"


using namespace kke;

using std::string;

ShaderProgram::ShaderProgram (bool generate) : 
	handle(0)
{
	if (generate)
		Generate();
}

ShaderProgram::~ShaderProgram()
{
	Unload();
}

void ShaderProgram::Generate()
{
	handle = glCreateProgram();
}

void ShaderProgram::Unload()
{
	if (handle != 0)
	{
		glDetachShader(handle, vertex->GetHandle());
		glDetachShader(handle, fragment->GetHandle());
		
		delete vertex;
		delete fragment;
		vertex = 0;
		fragment = 0;
		
		glDeleteProgram(handle);
		handle = 0;
		
		uniforms.clear();
		attributes.clear();
	}
}

void ShaderProgram::Attach (tShader* shader)
{
	if (handle == 0)
	{
		LOGE() << "Can't attach shader to non-generated ShaderProgram.";
		return;
	}
	
	switch (shader->GetType())
	{
		case GL_VERTEX_SHADER:
			if (vertex != 0)
			{
				LOGW() << "Overwriting vertex shader.";
			}
			
			vertex = shader;
			break;
			
		case GL_FRAGMENT_SHADER:
			if (fragment != 0)
			{
				LOGW() << "Overwriting vertex shader.";
			}
			
			fragment = shader;
			break;
	};
	
	glAttachShader(handle, shader->GetHandle());
}

void ShaderProgram::Detach (GLenum shaderType)
{
	switch (shaderType)
	{
		case GL_VERTEX_SHADER:
			if (vertex == 0)
			{
				LOGE() << "Tried detaching non-existing vertex shader";
			}
			else
			{
				glDetachShader(handle, vertex->GetHandle());
				vertex = 0;
			}
			break;
			
		case GL_FRAGMENT_SHADER:
			if (fragment == 0)
			{
				LOGE() << "Tried detaching non-existing fragment shader";
			}
			else
			{
				glDetachShader(handle, fragment->GetHandle());
				fragment = 0;
			}
			break;
	};
}

void ShaderProgram::Bind() const
{
	glUseProgram(handle);
}

void ShaderProgram::Link()
{
	glLinkProgram(handle);
	
	GLint success;
	
	// Check for errors
	glGetProgramiv(handle, GL_LINK_STATUS, &success);
	if (success != GL_TRUE)
	{
		LOGE() << "Error linking program.";
	}
	
	uniforms.clear();
	attributes.clear();
}

void ShaderProgram::Disable() const
{
	glUseProgram(0);
}

GLuint ShaderProgram::GetHandle() const
{
	return handle;
}

