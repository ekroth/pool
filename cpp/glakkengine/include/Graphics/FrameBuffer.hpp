/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_FRAMEBUFFER_HPP
#define KKE_FRAMEBUFFER_HPP

#include "Graphics/GfxCache.hpp"
#include "Graphics/Handled.hpp"
#include "Interface/INoncopyable.hpp"

#include <unordered_map>
#include <GL/glew.h>

namespace kke
{
	class Texture2D;
	class RenderBuffer;
	class FrameBuffer final : public INoncopyable, public Handled<GLuint>
	{
	public:
		explicit FrameBuffer(bool gen = true);
		~FrameBuffer();
		
		inline void Bind(GLenum target = GL_FRAMEBUFFER) { GfxCache::Instance().SetFBO(target, handle); }
		bool Generate();
		bool Delete();
		
		/**
		 * @brief Returns status of FBO.
		 *
		 * @param output If errors should be outputted to log. Defaults to true.
		 * @return GLenum status
		 **/
		GLenum GetStatus(bool output = true) const;
		
		/**
		 * @brief Get texture, will return 0 if there's no texture attached at target/attachment.
		 **/
		Texture2D* GetTexture(GLenum target = GL_FRAMEBUFFER, GLenum attach = GL_COLOR_ATTACHMENT0);
		
		/**
		 * @brief Attach a texture to FBO. Deleting FBO will delete texture as well (as long as it is attached).
		 **/
		bool AttachTexture(Texture2D* texture, GLenum target = GL_FRAMEBUFFER, GLenum attach = GL_COLOR_ATTACHMENT0, int mipmap = 0);
		
		/**
		 * @brief Detach a renderbuffer, this will not call delete on texture.
		 **/
		bool DetachTexture(GLenum target = GL_FRAMEBUFFER, GLenum attach = GL_COLOR_ATTACHMENT0, int mipmap = 0);
		
		/**
		 * @brief Get renderbuffer, will return 0 if there's no RBO attached at target/attachment.
		 **/
		RenderBuffer* GetRenderbuffer(GLenum target = GL_FRAMEBUFFER, GLenum attach = GL_DEPTH_ATTACHMENT);
		
		/**
		 * @brief Attach a renderbuffer to FBO. Deleting FBO will delete renderbuffer as well (as long as it is attached).
		 **/
		bool AttachRenderbuffer(RenderBuffer* renderbuffer, GLenum target = GL_FRAMEBUFFER, GLenum attach = GL_DEPTH_ATTACHMENT);
		
		/**
		 * @brief Detach a renderbuffer, this will not call delete on RBO.
		 **/
		bool DetachRenderbuffer(GLenum target = GL_FRAMEBUFFER, GLenum attach = GL_DEPTH_ATTACHMENT);
		
		/**
		 * @brief Blit FBO to another FBO.
		 **/
		void Blit(int x, int y, int width, int height, FrameBuffer* targetFbo, int tx, int ty, int twidth, int theight, GLbitfield mask = GL_COLOR_BUFFER_BIT, GLenum filter = GL_LINEAR);
	
	private:
		typedef std::unordered_map<GLenum, Texture2D*> TextureMap;
		typedef std::unordered_map<GLenum, RenderBuffer*> RenderbufferMap;
		
		struct AttachPoint
		{
			TextureMap Textures;
			RenderbufferMap RenderBuffers;
		};
		
		typedef std::unordered_map<GLenum, AttachPoint> AttachMap;
        
    private:
		AttachMap attachments;
		
		TextureMap textureMap;
		RenderbufferMap renderbufferMap;
	};
}

#endif // KKE_FRAMEBUFFER_HPP