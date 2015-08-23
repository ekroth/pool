/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_RENDERBUFFER_HPP
#define KKE_RENDERBUFFER_HPP

#include "Graphics/GfxCache.hpp"
#include "Graphics/Handled.hpp"
#include "Graphics/GLException.hpp"
#include "Interface/INoncopyable.hpp"

#include <GL/glew.h>

namespace kke
{	
	/**
	 * @brief OpenGL RenderBuffer.
	 **/
	class RenderBuffer final : public INoncopyable, public Handled<GLuint>
	{
	public:
		explicit RenderBuffer(bool gen = false);
		~RenderBuffer();
		
		/**
		 * @brief Bind Renderbuffer.
		 **/
		inline void Bind() const { GfxCache::Instance().SetRBO(GL_RENDERBUFFER, handle); }
		
		bool Generate();
		bool Delete();
		
		/**
		 * @brief Allocate a renderbuffer.
		 *
		 * @param internalFormat ...
		 * @param width ...
		 * @param height ...
		 * @return void
		 * 
		 * @exception RenderBufferGLException If unable to allocate RBO.
		 **/
		void Allocate(GLenum internalFormat, GLsizei width, GLsizei height);
		
		GLenum GetInternalFormat() const;
		GLenum GetWidth() const;
		GLenum GetHeight() const;
		
	private:
		GLenum internalFormat;
		GLsizei width, height;
	};
	
	class RenderBufferGLException : public GLException
	{
	public:
		RenderBufferGLException(GLenum err, const std::string& reason) : GLException(err, "GL_RENDERBUFFER", reason) { }
	};
}

#endif // KKE_RENDERBUFFER_HPP