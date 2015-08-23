/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Selectable.hpp"

using namespace kke;

Selectable::Selectable(bool selected) : selected(selected)
{

}

Selectable::~Selectable()
{

}

void Selectable::ToggleSelect()
{
	if (selected)
		Deselect();
	else
		Select();
}
