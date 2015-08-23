/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_GFXEVENTMANAGER_HPP
#define KKE_GFXEVENTMANAGER_HPP

#include "Events/EventManager.hpp"

namespace kke
{
    template<class EM>
    class ScopedListener;
    
	namespace render
	{
		class GfxEventManager : public EventManagerBase, public Singleton<GfxEventManager>
		{
			friend class Singleton<GfxEventManager>;
			
		private:
			GfxEventManager() : EventManagerBase() { }
			~GfxEventManager() { }
		};
        
        
        typedef ScopedListener<GfxEventManager> ScopedGEMListener;
	}
}

#endif // KKE_GFXEVENTMANAGER_HPP