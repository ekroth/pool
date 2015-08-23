/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_IUPDATABLE_HPP
#define KKE_IUPDATABLE_HPP

namespace kke
{
    class GameTime;
	/**
	 * @brief Can be updated.
	 **/
	class IUpdatable
    {
	public:
        virtual void Update(GameTime& gt) = 0;
    };
}

#endif // KKE_IUPDATABLE_HPP
