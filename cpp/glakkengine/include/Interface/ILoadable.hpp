/* Copyright (c) 2011-2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_ILOADABLE_HPP
#define KKE_ILOADABLE_HPP

namespace kke
{
	/**
	 * @brief Can be loaded.
	 **/
	class ILoadable
    {
	public:
        virtual void Load() { isLoaded = true; }  
		virtual void Unload() { isLoaded = false; }
		bool IsLoaded() const { return isLoaded; }
		
	protected:
		ILoadable() : isLoaded(false) {}
		
		bool isLoaded;
    };
}

#endif // KKE_ILOADABLE_HPP
