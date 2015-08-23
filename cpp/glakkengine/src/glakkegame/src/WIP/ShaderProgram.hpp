/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_SHADERPROGRAM_HPP
#define KKE_SHADERPROGRAM_HPP

#include <GL/glew.h>
#include <string>
#include <map>

namespace kke
{
	template<typename>
	class Resource;
	class ShaderPart;
	class ShaderProgram
	{
	public:
		ShaderProgram(bool generate);
		~ShaderProgram();
		
		void Generate();
		void Unload();
		void Link();
		void Bind() const;
		void Disable() const;
		
		void Attach(ShaderPart* shader);
		void Detach(GLenum shaderType);
		
		GLuint GetHandle() const;
	private:
		GLuint handle;
		Resource<ShaderPart> *vertex, *fragment;
		
		typedef std::map<std::string, GLint> ShaderVarMap;
		
		ShaderVarMap uniforms;
		ShaderVarMap attributes;
	};
}

#endif // KKE_SHADERPROGRAM_HPP