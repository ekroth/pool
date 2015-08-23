/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_VERTEXARRAY_HPP
#define KKE_VERTEXARRAY_HPP

#include "Graphics/Handled.hpp"
#include "Graphics/GfxCache.hpp"
#include "Interface/INoncopyable.hpp"

#include <GL/glew.h>

namespace kke
{
	class VertexArray final : public INoncopyable, public Handled<GLuint>
	{
	public:
		explicit VertexArray(bool generate = true);
		~VertexArray();
		
		bool Generate();
		bool Delete();
        
        inline void Bind() const { GfxCache::Instance().SetVAO(handle); }
        inline void Unbind() const { GfxCache::Instance().SetVAO(0); }
	};
	
	/**
	 * @brief Generate multiple array objects on heap.
	 **/
	void GenerateAOs(VertexArray** data, int amount);
	
	/**
	 * @brief Delete multiple arrayobjects on heap.
	 **/
	void DeleteAOs(VertexArray** data, int amount);
}

#endif // KKE_VERTEXARRAY_HPP