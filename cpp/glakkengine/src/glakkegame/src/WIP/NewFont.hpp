/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_NEWFONT_HPP
#define KKE_NEWFONT_HPP

#include "ResourceType.hpp"
#include "Interface/INoncopyable.hpp"

#include <FTGL/ftgl.h>

namespace kke
{
	class NewFont : public ResourceType, public INoncopyable
	{
	public:
		NewFont();
		~NewFont();
		
		/**
		* @brief Load a TTF font.
		*
		* @param source Path to XML file.
		**/
		bool Load(const std::string& source);
		bool Unload();
		
		/**
		* @brief Get size of font.
		**/
		uint GetSize() const;
		
		void Draw();
	private:
		FTFont *font;
		uint fontSize;
		std::string fontPath;
	};
}

#endif // KKE_NEWFONT_HPP