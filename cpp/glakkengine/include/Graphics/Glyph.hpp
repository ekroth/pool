/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_GLYPH_HPP
#define KKE_GLYPH_HPP

#include "Interface/INoncopyable.hpp"

#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

struct FT_Glyph_Metrics_;
typedef unsigned int uint;

namespace kke
{
    class Texture2D;
    /**
    * @brief Contains information about a single Unicode character. Used by class Font.
    **/
    class Glyph : public INoncopyable
    {
    public:
        Glyph(wchar_t character,
            const FT_Glyph_Metrics& metrics,
            uint fontSize,
            uint charIndex);
        ~Glyph();

        /**
        * @brief Geth glyph metrics.
        *
        * @return :GlyphMetrics& Metrics.
        **/
        const FT_Glyph_Metrics& GetMetrics() const;
		
//         /**
//         * @brief Index of character in source array.
//         *
//         * @return uint
//         **/
//         uint GetSourceIndex() const;
		
        /**
        * @brief Size of glyph/font.
        *
        * @return uint
        **/
        uint GetFontSize() const;
		
        /**
        * @brief Index of character/glyph in font.
        *
        * @return uint
        **/
        uint GetCharIndex() const;
		
        /**
        * @brief Unicode character the glyph represents.
        *
        * @return wchar_t Glyph.
        **/
        wchar_t GetCharacter() const;
    private:
//         uint sourceIndex;
        FT_Glyph_Metrics metrics;
		FT_BBox bbox;
        uint fontSize;
        uint charIndex;
        wchar_t character;
    };
}

#endif // KKE_GLYPH_HPP