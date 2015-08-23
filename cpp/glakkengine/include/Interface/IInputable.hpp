/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_IINPUTABLE_HPP
#define KKE_IINPUTABLE_HPP

namespace kke
{
    class InputManager;
    class GameTime;
    
	/**
	 * @brief Can handle input.
	 **/
	class IInputable
    {
	public:       
        virtual void HandleInput(InputManager& im, GameTime& gt) = 0;
    };
}

#endif // KKE_IINPUTABLE_HPP
