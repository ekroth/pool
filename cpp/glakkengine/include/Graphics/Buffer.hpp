/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_BUFFER_HPP
#define KKE_BUFFER_HPP

#include "Graphics/GfxCache.hpp"
#include "Graphics/Handled.hpp"
#include "Graphics/GLException.hpp"
#include "Interface/INoncopyable.hpp"

#include <GL/glew.h>
#include <vector>
#include <map>

namespace kke
{

    class Buffer;
	/**
	 * @brief OpenGL vertex buffer object.
	 **/
	class Buffer final : public INoncopyable, public Handled<GLuint>
	{
	public:
		
		/**
		 * @brief Constructor.
		 *
		 * @param gen If handle should be generated. Defaults to false.
		 * @param defaultTarget Default BufferObject target. Defaults to GL_ARRAY_BUFFER.
		 **/
		explicit Buffer(bool gen = false, GLenum defaultTarget = GL_ARRAY_BUFFER);
		~Buffer();
		
		/**
		 * @brief Bind default target.
		 *
		 * @return void
		 **/
		inline void Bind() const { GfxCache::Instance().SetBuffer(defaultTarget, handle); }
		
		/**
		 * @brief Bind specific target, ignore default.
		 *
		 * @param target Specific target.
		 * @return void
		 **/
		inline void Bind(GLenum target) const { GfxCache::Instance().SetBuffer(target, handle); }
		
		/**
		 * @brief Buffer data to default target.
		 * @return void
		 **/
		void BufferData(GLsizei size, GLvoid* data, GLenum usage);
		
		/**
		 * @brief Buffer sub data to default target.
		 * @return void
		 **/
		void BufferSubData(GLint offset, GLsizei size, GLvoid* data);
		
		
		/**
		 * @brief Buffer data to specific target, ignoring default.
		 *
		 * @param target Specific target.
		 * @return void
		 **/
		void BufferData(GLsizei size, GLvoid* data, GLenum usage, GLenum target);
		
		/**
		 * @brief Buffer sub data to specific target, ignoring default.
		 *
		 * @param target Specific target.
		 * @return void
		 **/
		void BufferSubData(GLint offset, GLsizei size, GLvoid* data, GLenum target);
		
		bool Generate();
		bool Delete();
		
		/**
		 * @brief Map specific BO target to client memory. Returns pointer. Remember to Unmap.
		 *
		 * @param access Access type.
		 * @return void*
		 **/
		void* Map(GLenum access, GLenum target);
		
		/**
		 * @brief Unmap specific BO target from client memory.
		 *
		 * @return void
		 **/
		void Unmap(GLenum target);
		
		/**
		 * @brief Set default target for overloadable functions.
		 *
		 * @param target Target.
		 * @return void
		 **/
		void SetTarget(GLenum target);
		
		GLenum GetTarget() const;
		size_t GetSize() const;
		
	private:
		GLuint handle;
		size_t size;
		GLvoid* mapped;
		GLenum defaultTarget;		
	};

	/**
	 * @brief Generate multiple buffer objects on heap.
	 **/
	void GenerateBOs(Buffer** data, int amount);
	
	/**
	 * @brief Delete multiple buffer objects on heap.
	 **/
	void DeleteBOs(Buffer** data, int amount);
    
    class BufferGLException : public GLException
    {
    public:
        BufferGLException(GLenum error, GLenum source, const std::string& reason = "Unknown reason") noexcept;
        virtual ~BufferGLException() noexcept;
        
    private:
        std::string bufferToString(GLenum buffer) noexcept;
    };
}

#endif // BUFFEROBJECT_HPP
