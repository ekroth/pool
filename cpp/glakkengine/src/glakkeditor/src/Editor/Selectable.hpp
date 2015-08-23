/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_SELECTABLE_HPP
#define KKE_SELECTABLE_HPP



#include <glm/glm.hpp>

namespace kke
{
	class Selectable
	{
	public:
		Selectable(bool selected = false);
		virtual ~Selectable();
		
		virtual bool InBounds(const glm::vec2& pos) const = 0;
		virtual void Select() = 0;
		virtual void Deselect() = 0;
		void ToggleSelect();
		
	protected:
		bool selected;
	};
}

#endif // KKE_SELECTABLE_HPP
