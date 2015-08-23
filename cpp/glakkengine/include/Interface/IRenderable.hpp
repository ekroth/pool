/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_IRENDERABLE_HPP
#define KKE_IRENDERABLE_HPP

namespace kke
{
    class GameTime;
	
	/**
	 * @brief Can be drawn.
	 **/
	class IRenderable
    {
	public:       
        virtual void Render(GameTime& gt) = 0;
    };
}

#endif // KKE_IRENDERABLE_HPP
