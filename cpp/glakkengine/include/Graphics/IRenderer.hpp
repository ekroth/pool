/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_IRENDERER_HPP
#define KKE_IRENDERER_HPP

namespace kke
{
	typedef unsigned int RenderMode;
	enum class RenderStatus
	{
		Idle,
		Drawing,
		PreEnded
	};
	
	class IRenderer
	{
	public:
		virtual ~IRenderer();
		
		virtual void Begin(RenderMode mode) = 0;
		virtual void PreEnd() = 0;
		
		virtual void End() = 0;
		
	protected:
		IRenderer();
	};
}

#endif // KKE_IRENDERER_HPP