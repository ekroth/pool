/* Copyright (c) 20102011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include "Interface/INoncopyable.hpp"
#include "Graphics/Handled.hpp"
#include "Graphics/GfxCache.hpp"
#include "ResourceType.hpp"

#include <GL/glew.h>
#include <string>
#include <map>

namespace kke
{
	class Texture2D : public ResourceType, public INoncopyable, public Handled<GLuint>
	{
        RESOURCETYPE_CATEGORY(Texture2D);
	public:
		/**
		* @brief Create empty Texture2D.
		*
		**/
		explicit Texture2D(const std::string& path);
		
		/**
		* @brief Create a Texture2D with a preloaded OpenGL image.
		*
		* @param handle Texture handle.
		* @param width Width.
		* @param height Height.
		* @param path Path to texture, not needed. Defaults to "".
		* @return void
		**/
		Texture2D(
			GLuint handle,
			GLuint width,
			GLuint height,
			GLuint channels,
			const std::string& path = std::string());
		
		explicit Texture2D(bool generate = false);
		
		/**
		 * @brief Generate and allocate texture. (Generate(), Bind(), Allocate())
		 **/
		Texture2D(GLint level, 
			GLint internalFormat, 
			GLsizei width, 
			GLsizei height, 
			GLint border, 
			GLenum format, 
			GLenum type, 
			const GLvoid * data = 0);

		~Texture2D();

		/**
		* @brief Load texture from file path. Using ResourceManager is prefered to avoid duplicates.
		*
		* @param path to file.
		* @return bool
		**/
		bool Load(const std::string& path) override;
		bool Load(const std::string& path, bool flip);
		ResourceType* Copy() const;
		
		/**
		* @brief Set handle
		*
		* @param handle Texture handle.
		* @param width Width.
		* @param height Height.
		* @param path Path to texture, recommended to avoid duplicates. Defaults to "Unknown path".
		* @return void
		**/
		void SetHandle(
			GLuint handle,
			GLuint width,
			GLuint height,
			GLenum format,
			const std::string& path = std::string());
		
		/**
		* @brief Allocating with glTexImage2D.
		**/
		void Allocate(
			GLint level, 
			GLint internalFormat, 
			GLsizei width, 
			GLsizei height, 
			GLint border, 
			GLenum format, 
			GLenum type, 
			const GLvoid * data = 0);
		
		/**
		 * @brief Allocating with glTexSubImage2D.
		 **/
		void AllocateSub(
			GLint level, 
			GLsizei x,
			GLsizei y,
			GLsizei width, 
			GLsizei height,
			GLenum format, 
			GLenum type, 
			const GLvoid * data);
		
		/**
		 * @brief Generate texture.
		 **/
		bool Generate();
		bool Delete();
		inline void Bind() const { GfxCache::Instance().SetTexture(GL_TEXTURE_2D, handle); }
		
		/**
		* @brief Unload all resources.
		*
		* @return void
		**/
		bool Unload();

		GLuint GetWidth() const;
		GLuint GetHeight() const;
		GLenum GetFormat() const;
		void SetParameteri(GLenum pname, GLint param);
		void SetParameterf(GLenum pname, GLfloat param);
		void GenerateMipmaps();

		/**
		* @brief Save Texture2D to a bitmap image.
		*
		* @param path Path with extension. (TGA, BMP and DDS available).
		* @return bool Result.
		**/
		bool Save(const std::string& path) const;
		
		GLenum ILToGL(uint value) const;
		
	private:
		GLuint handle, width, height;
		GLenum format;
		bool mipmaps;
	};
}

#endif // TEXTURE2D_HPP
