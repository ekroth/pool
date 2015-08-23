/* Copyright (c) 2011-2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/Shader.hpp"
#include "Graphics/GLManager.hpp"
#include "IO/LogManager.hpp"
#include "ResourceFactory.hpp"

#include <fstream>
#include <vector>
#include <array>

using namespace kke;

using std::string;
using std::ifstream;
using std::vector;
using std::unique_ptr;

Shader::Shader(bool gen)
{
	if (gen)
		Generate();
}

Shader::~Shader()
{
	Delete();
}

bool Shader::Load(const string& vertexFile, const string& fragmentFile)
{
	vertex = unique_ptr<ShaderPart>(new ShaderPart(GL_VERTEX_SHADER, false));
	fragment = unique_ptr<ShaderPart>(new ShaderPart(GL_FRAGMENT_SHADER, false));
	
	if (!vertex->Load(vertexFile))
	{
		string log;
		vertex->GetLog(log);
		LOGE() << "Log Vertex (" << vertexFile << ") : " << log;
	}
	
	if (!fragment->Load(fragmentFile))
	{
		string log;
		fragment->GetLog(log);
		LOGE() << "Log Fragment (" << fragmentFile << ") : " << log;
	}

	// Create program
	if (!Generated())
		Generate();

	// Attach shaders to program
	glAttachShader(handle, vertex->GetHandle());
	glAttachShader(handle, fragment->GetHandle());
	
	Link();
    
    return ResourceType::Load(MergeSources({ vertexFile, fragmentFile }));
}

bool Shader::Load(const string& files)
{
	vector<string> vec = ExtractSources(files);
	
	const string vertSource = vec[0].find(".vert") != string::npos ? vec[0] : vec[1];
	const string fragSource = vec[0].find(".frag") != string::npos ? vec[0] : vec[1];

	return Load(vertSource, fragSource);
}

bool Shader::Generate()
{
	if (!Generated())
	{
        handle = glCreateProgram();
	}
	
	return true;
}

bool Shader::Delete()
{
	if (Generated())
	{
		glDeleteProgram(handle);
		handle = 0;
	}
	
	return true;
}

bool Shader::Unload()
{
	if (Generated())
	{
		// Bypass Disable() warning, since this is okay.
		GfxCache::Instance().SetProgram(0);

		// Detach
		glDetachShader(handle, vertex->GetHandle());
		glDetachShader(handle, fragment->GetHandle());
		
		uniforms.clear();
		attributes.clear();
		
		Delete();
	}

	return handle == 0;
}

void Shader::Link()
{
	// Link program
	glLinkProgram(handle);
	
	GLint success;
	
	// Check for errors
	glGetProgramiv(handle, GL_LINK_STATUS, &success);
	if (success != GL_TRUE)
	{
		string logging;
		GetProgramErrors(logging);
		LOGE() << "Error linking program." << '\n' << logging;
	}
	
	uniforms.clear();
	attributes.clear();
}

void Shader::Uniform(const string& name, GLfloat x)
{
	glUniform1f(GetUniformLocation(name), x);
}

void Shader::Uniform(const string& name, GLfloat x, GLfloat y)
{
	glUniform2f(GetUniformLocation(name), x, y);
}

void Shader::Uniform(const string& name, GLfloat x, GLfloat y, GLfloat z)
{
	glUniform3f(GetUniformLocation(name), x, y, z);
}

void Shader::Uniform(const string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	glUniform4f(GetUniformLocation(name), x, y, z, w);
}

void Shader::Uniform(const string& name, GLint x)
{
	glUniform1i(GetUniformLocation(name), x);
}

void Shader::Uniform(const string& name, GLint x, GLint y)
{
	glUniform2i(GetUniformLocation(name), x, y);
}

void Shader::Uniform(const string& name, GLint x, GLint y, GLint z)
{
	glUniform3i(GetUniformLocation(name), x, y, z);
}

void Shader::Uniform(const string& name, GLint x, GLint y, GLint z, GLint w)
{
	glUniform4i(GetUniformLocation(name), x, y, z, w);
}

void Shader::UniformMat(const std::string& name, GLsizei size, bool transpose, const GLfloat* value)
{
	glUniformMatrix4fv(GetUniformLocation(name), size, transpose ? GL_TRUE : GL_FALSE, value);
}

void Shader::VertexAttribPointer(const std::string& name, GLsizei size, GLenum type, bool normalized, GLsizei stride, const GLvoid* data)
{
	glVertexAttribPointer(GetAttribLocation(name), size, type, normalized ? GL_TRUE : GL_FALSE, stride, data);
}

void Shader::SetVertexAttrib(const std::string& name, bool status)
{
	if (status)
		glEnableVertexAttribArray(GetAttribLocation(name));
	else
		glDisableVertexAttribArray(GetAttribLocation(name));
}


GLint Shader::GetUniformLocation(const string& name)
{
	ShaderVarMap::iterator it = uniforms.find(name);
	
	if (it == uniforms.end())
	{
		const int loc = glGetUniformLocation(handle, name.c_str());
		uniforms[name] = loc;
		return loc;
	}
	
	return it->second;
}

GLint Shader::GetAttribLocation(const string& name)
{
	ShaderVarMap::iterator it = attributes.find(name);
	
	if (it == attributes.end())
	{
		const int loc = glGetAttribLocation(handle, name.c_str());
		attributes[name] = loc;
		return loc;
	}
	
	return it->second;
}

void Shader::GetProgramErrors(string& output) const
{
	int logsize;

	glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logsize);

	if (logsize == 0)
	{
		// No errors/logs.
	}
	else
	{
		char log[logsize];
		// Log is not empty
		glGetProgramInfoLog(handle, logsize, NULL, log);

		output = string(log);
		// Remove '\n'
		if (output.find("\n") != string::npos)
		{
			output.erase(logsize - 2, logsize);
		}

		output = output;
	}
}

void Shader::BindFragDataLocation(const std::string& name, GLint texture)
{
	glBindFragDataLocation(handle, texture, name.c_str());
}
