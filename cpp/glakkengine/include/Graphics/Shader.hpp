/* Copyright (c) 2011-2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_SHADER_HPP
#define KKE_SHADER_HPP

#include "ResourceType.hpp"
#include "Graphics/GfxCache.hpp"
#include "Graphics/Handled.hpp"
#include "Graphics/ShaderPart.hpp"
#include "Interface/INoncopyable.hpp"

#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include <memory>

namespace kke
{	
	/**
	 * @brief OpenGL shader program.
	 **/
	class Shader final : public ResourceType, public INoncopyable, public Handled<GLuint>
	{
        RESOURCETYPE_CATEGORY(Shader);
        
	public:
		explicit Shader(bool gen = true);
		
		/**
		 * @brief Load vertex- and fragmentfile.
		 *
		 * @param files <vertex>|<fragment>
		 **/
		explicit Shader(const std::string& files);
		Shader(const std::string& vertex, const std::string& fragment);
		~Shader();

		bool Load(const std::string& vertex, const std::string& fragment);
		
		/**
		 * @brief Load vertex- and fragmentfile.
		 *
		 * @param files <vertex>|<fragment>
		 **/
		bool Load(const std::string& files);
		bool Unload();
		inline void Bind() const { GfxCache::Instance().SetProgram(handle); }
		bool Generate();
		bool Delete();
		void Link();

		void Uniform(const std::string& name, GLfloat x);
		void Uniform(const std::string& name, GLfloat x, GLfloat y);
		void Uniform(const std::string& name, GLfloat x, GLfloat y, GLfloat z);
		void Uniform(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
		void Uniform(const std::string& name, GLint x);
		void Uniform(const std::string& name, GLint x, GLint y);
		void Uniform(const std::string& name, GLint x, GLint y, GLint z);
		void Uniform(const std::string& name, GLint x, GLint y, GLint z, GLint w);
		void UniformMat(const std::string& name, GLsizei size, bool transpose, const GLfloat* value);
		
		/**
		 * @brief Set the attribute pointer.
		 **/
		void VertexAttribPointer(const std::string& name, GLsizei size, GLenum type, bool normalized, GLsizei stride, const GLvoid* data);
		
		/**
		 * @brief Set Vertex Attrib Array status.
		 **/
		void SetVertexAttrib(const std::string& name, bool status);

		GLint GetUniformLocation(const std::string& name);
		void SetAttribLocation(const std::string& name, GLuint location);
		GLint GetAttribLocation(const std::string& name);

		void GetProgramErrors(std::string& output) const;
		void GetVertexErrors(std::string& output) const;
		void GetFragmentErrors(std::string& output) const;
		
		/**
		 * @brief Bind a frag data location. Must be done before linking.
		 * ----
		 * glDrawBuffers(2, {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT2});
		 * FragDataLocation texture 0 would then be GL_COLOR_ATTACHMENT0 (index 0), and so on.
		 **/
		void BindFragDataLocation(const std::string& name, GLint texture);
		
	private:  
		std::unique_ptr<ShaderPart> vertex, fragment;
		
		typedef std::unordered_map<std::string, GLint> ShaderVarMap;
		
		ShaderVarMap uniforms;
		ShaderVarMap attributes;
	};
}

#endif // KKE_SHADER_HPP
