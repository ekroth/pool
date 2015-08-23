/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/FrameBuffer.hpp"
#include "Graphics/Texture2D.hpp"
#include "Graphics/RenderBuffer.hpp"
#include "Graphics/GLManager.hpp"
#include "IO/LogManager.hpp"

#include <queue>

using namespace kke;

using std::queue;

FrameBuffer::FrameBuffer (bool gen)
{
	if (gen)
		Generate();
}

FrameBuffer::~FrameBuffer()
{
	Delete();
}

bool FrameBuffer::Generate()
{
	glGenFramebuffers(1, &handle);
	return true;
}

bool FrameBuffer::Delete()
{
	if (handle != 0)
	{
		glDeleteFramebuffers(1, &handle);
		handle = 0;
		
		
		for (AttachMap::iterator it = attachments.begin(); it != attachments.end(); ++it)
		{
			for (TextureMap::iterator tex = it->second.Textures.begin(); tex != it->second.Textures.end(); ++tex)
			{
				delete tex->second;
				
				for (AttachMap::iterator itC = attachments.begin(); itC != attachments.end(); itC++)
					for (TextureMap::iterator texC = itC->second.Textures.begin(); texC != itC->second.Textures.end(); ++texC)
						if (texC->second == tex->second)
							itC->second.Textures.erase(texC);
			}
			
			for (RenderbufferMap::iterator rbo = it->second.RenderBuffers.begin(); rbo != it->second.RenderBuffers.end(); ++rbo)
			{
				delete rbo->second;
				
				for (AttachMap::iterator itC = attachments.begin(); itC != attachments.end(); itC++)
					for (RenderbufferMap::iterator rboC = itC->second.RenderBuffers.begin(); rboC != itC->second.RenderBuffers.end(); ++rboC)
						if (rboC->second == rbo->second)
							itC->second.RenderBuffers.erase(rboC);
			}
			
			it->second.Textures.clear();
			it->second.RenderBuffers.clear();
		}
		
		attachments.clear();
	}
	
	return true;
}

GLenum FrameBuffer::GetStatus(bool output) const
{
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	
	if (output)
		switch (status)
		{
			// See http://www.opengl.org/sdk/docs/man4/xhtml/glCheckFramebufferStatus.xml
			case GL_FRAMEBUFFER_COMPLETE:
				break;
				
			case GL_FRAMEBUFFER_UNDEFINED:
				LOGE() << "GL_FRAMEBUFFER_UNDEFINED";
				break;
				
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				LOGE() << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
				break;
				
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				LOGE() << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
				break;
				
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				LOGE() << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
				break;
				
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				LOGE() << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
				break;
				
			case GL_FRAMEBUFFER_UNSUPPORTED:
				LOGE() << "GL_FRAMEBUFFER_UNSUPPORTED";
				break;
				
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
				LOGE() << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
				break;
				
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
				LOGE() << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
				break;
		};
	
	return status;
}

Texture2D* FrameBuffer::GetTexture (GLenum target, GLenum attach)
{
	if (target == GL_FRAMEBUFFER)
		target = GL_DRAW_FRAMEBUFFER; // Same thing when attach/detach

	TextureMap::iterator it = attachments[target].Textures.find(attach);
	
	if (it == attachments[target].Textures.end())
	{
		return nullptr;
	}
	
	return it->second;
}

bool FrameBuffer::AttachTexture (Texture2D* texture, GLenum target, GLenum attach, int mipmap)
{
	if (target == GL_FRAMEBUFFER)
		target = GL_DRAW_FRAMEBUFFER;
	
	TextureMap::iterator it = attachments[target].Textures.find(attach);
	
	if (it != attachments[target].Textures.end())
	{
		LOGW() << "Overwritten texture on framebuffer.";
	}
	
	// Attach it
	glFramebufferTexture2D(target, attach, GL_TEXTURE_2D, texture->GetHandle(), mipmap);
	attachments[target].Textures[attach] = texture;
	
	// Bind it
	return true;
}

bool FrameBuffer::DetachTexture (GLenum target, GLenum attach, int mipmap)
{
	if (target == GL_FRAMEBUFFER)
		target = GL_DRAW_FRAMEBUFFER;
	
	if (attachments[target].Textures.find(attach) == attachments[target].Textures.end())
	{
		LOGE() << "Tried detaching texture which doesn't exist on framebuffer.";
		return false;
	}
	
	glFramebufferTexture2D(target, attach, GL_TEXTURE_2D, 0, mipmap);
	textureMap.erase(attach);
	
	return true;
}

RenderBuffer* FrameBuffer::GetRenderbuffer (GLenum target, GLenum attach)
{
	if (target == GL_FRAMEBUFFER)
		target = GL_DRAW_FRAMEBUFFER;
	
	RenderbufferMap::iterator it = attachments[target].RenderBuffers.find(attach);
	
	if (it == attachments[target].RenderBuffers.end())
	{
		return nullptr;
	}
	
	return it->second;
}

bool FrameBuffer::AttachRenderbuffer (RenderBuffer* renderbuffer, GLenum target, GLenum attach)
{
	if (target == GL_FRAMEBUFFER)
		target = GL_DRAW_FRAMEBUFFER;
	
	RenderbufferMap::iterator it = attachments[target].RenderBuffers.find(attach);
	
	if (it != attachments[target].RenderBuffers.end())
	{
		LOGW() << "Overwritten RenderBuffer on FrameBuffer.";
	}
	
	// Attach it
	glFramebufferRenderbuffer(target, attach, GL_RENDERBUFFER, renderbuffer->GetHandle());
	attachments[target].RenderBuffers[attach] = renderbuffer;
	
	// Bind it
	return true;
}

bool FrameBuffer::DetachRenderbuffer (GLenum target, GLenum attach)
{
	if (target == GL_FRAMEBUFFER)
		target = GL_DRAW_FRAMEBUFFER;
	
	if (attachments[target].RenderBuffers.find(attach) == attachments[target].RenderBuffers.end())
	{
		LOGE() << "Tried detaching renderbuffer which doesn't exist on framebuffer.";
		return false;
	}
	
	glFramebufferRenderbuffer(target, attach, GL_RENDERBUFFER, 0);
	attachments[target].RenderBuffers.erase(attach);
	
	return true;
}

void FrameBuffer::Blit (int x, int y, int width, int height, FrameBuffer* targetFbo, int tx, int ty, int twidth, int theight, GLbitfield mask, GLenum filter)
{
	GLuint targetFboHandle = targetFbo == 0 ? 0 : targetFbo->GetHandle();
	
	// Bind this one as READ
	Bind(GL_READ_FRAMEBUFFER);
	// Other as DRAW
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetFboHandle);
	glBlitFramebuffer(x, y, width, height, tx, ty, twidth, theight, mask, filter);
	
	GfxCache::Instance().SetFBO(GL_DRAW_FRAMEBUFFER, 0);
	GfxCache::Instance().SetFBO(GL_READ_FRAMEBUFFER, 0);
}