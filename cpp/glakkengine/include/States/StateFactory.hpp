/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_STATEFACTORY_HPP
#define KKE_STATEFACTORY_HPP

#include "Interface/Singleton.hpp"

namespace kke
{
	class StateFactory : public Singleton
	{
		SINGLETON_HEAD(StateFactory);
		
	public:
		StateFactory();
		virtual ~StateFactory();
	};
}

#endif // KKE_STATEFACTORY_HPP
