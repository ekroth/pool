/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/ShaderPart.hpp"
#include "IO/LogManager.hpp"

#include <fstream>

using namespace kke;

using std::string;
using std::ifstream;

ShaderPart::ShaderPart (GLenum type, bool generate) : 
	handle(0),
	type(type)
{
	if (generate)
		Generate();
}

ShaderPart::~ShaderPart()
{
	Unload();
}

void ShaderPart::Generate()
{
	handle = glCreateShader(type);
}

bool ShaderPart::Load(const string& path)
{
	// Create stream
	ifstream vStream(path.c_str());
	// Create string from stream, and copy file contents.		
	string source(std::istreambuf_iterator<char>(vStream), (std::istreambuf_iterator<char>()));
	
	// Make sure file is readable
	if (!vStream.good())
	{
		LOGE() << "Failed to load file: " << path;
		return false;
	}
	
	return LoadSource(source);
}

bool ShaderPart::LoadSource (string& source)
{
	if (handle == 0)
		Generate();
	
	if (!processSource(source))
	{
		LOGE() << "Failed processing source.";
	}

	const char *csource = source.c_str();
	glShaderSource(handle, 1, &csource, 0);
	glCompileShader(handle);
	
	GLint success;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
	
	if (success != GL_TRUE)
	{
		LOGE() << "Error compiling.";
		return false;
	}

	return true;
}

bool ShaderPart::Unload()
{
	if (handle != 0)
	{
		glDeleteShader(handle);
		handle = 0;
	}
	
	return true;
}

void ShaderPart::SetType (GLenum type)
{
	this->type = type;
}

GLenum ShaderPart::GetType() const
{
	return type;
}

GLuint ShaderPart::GetHandle() const
{
	return handle;
}

void ShaderPart::GetLog (string& log)
{
	int logsize;

	glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logsize);

	if (logsize == 0)
	{
		// No errors/logs.
	}
	else
	{
		char logcache[logsize];
		// Log is not empty
		glGetShaderInfoLog(handle, logsize, NULL, logcache);

		log = string(logcache);
		
		// Remove '\n'
		if (log.find("\n") != string::npos)
			log.erase(logsize - 2, logsize);
	}
}

bool ShaderPart::processSource (string& source)
{
	bool result = true;
	
	// What we search for
	const string procInclude = "#include \"";
	
	// Find position
	size_t begin = source.find(procInclude);
	if (begin != string::npos)
	{
		// Find first occurence of " after procInclude.
		size_t end = source.find('\"', begin + procInclude.length());
		
		if (end == string::npos)
		{
			LOGE() << "#include directive doesn't close \'\"\'.";
		}
		
		{
			// Extract path
			const string path = source.substr(begin + procInclude.length(), end - (begin + procInclude.length()));
			
			// Load path
			ifstream vStream(path.c_str());
			
			if (vStream.good())
			{
				// Create string from stream, and copy file contents.		
				string nSource(std::istreambuf_iterator<char>(vStream), (std::istreambuf_iterator<char>()));
				
				// Recursive, process the string.
				result = result && processSource(nSource);
				
				if (result)
				{
					// Replace text
					source.replace(begin, end - begin + 1, nSource);
				}
				else
				{
					LOGE() << "Processing source failed for next function.";
					result = false;
				}
			}
			else
			{
				LOGE() << "Error loading included file: " << '"' << path << '"';
				result = false;
			}
		}
	}
	
	return result;
}
