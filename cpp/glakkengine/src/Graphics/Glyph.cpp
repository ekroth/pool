/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/Glyph.hpp"
#include "Graphics/Texture2D.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

using kke::Glyph;
using kke::Texture2D;

Glyph::Glyph(
	wchar_t character,
// 	uint sourceIndex,
	const FT_Glyph_Metrics& metrics,
	uint fontSize,
	uint charIndex) :
// 	sourceIndex(sourceIndex),
	metrics(metrics),
	fontSize(fontSize),
	charIndex(charIndex),
	character(character)
{
}

Glyph::~Glyph()
{
}

const FT_Glyph_Metrics& Glyph::GetMetrics() const
{
	return metrics;
}

// uint Glyph::GetSourceIndex() const
// {
// 	return sourceIndex;
// }

uint Glyph::GetFontSize() const
{
	return fontSize;
}

uint Glyph::GetCharIndex() const
{
	return charIndex;
}

wchar_t Glyph::GetCharacter() const
{
	return character;
}
