/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_FONT_HPP
#define KKE_FONT_HPP

#include "ResourceType.hpp"
#include "Math/Rect.hpp"
#include "Interface/INoncopyable.hpp"
#include "Graphics/IRenderer.hpp"

#include <glm/glm.hpp>
#include <map>
#include <GL/glew.h>
#include <string>
#include <vector>

class TiXmlDocument;
typedef struct FT_FaceRec_*  FT_Face;

namespace kke
{
	class Texture2D;
	class Glyph;
	class Shader;
	class PackNode;
	class BufferObject;

	/**
	* @brief Manages a bitmap font. Supports FreeType2 and Unicode.
	**/
	class Font : public ResourceType, public INoncopyable
	{
	public:
		Font();
		~Font();

		/**
		* @brief Load a TTF font.
		*
		* @param source Path to XML file.
		**/
		bool Load(const std::string& source);
		bool Unload();
		bool LoadCache(const TiXmlDocument& doc, const TiXmlDocument& conf, const std::string& filename, const std::string& cacheFolder);
		const std::string& GetCategory() const;

		/**
		* @brief If the font has the specific glyph loaded.
		*
		* @param glyph Unicode glyph.
		* @return bool If the glyph/char is available.
		**/
		bool HasGlyph(wchar_t glyph) const;
		
		const Glyph* GetGlyph(wchar_t glyph) const;
		
		/**
		* @brief Get size of font.
		**/
		uint GetSize() const;
		/**
		* @brief Get FreeType2 face used by font.
		**/
		const FT_Face& GetFace() const;

		/**
		* @brief Get Texture2D which contains the font bitmaps.
		**/
		const Texture2D& GetTexture() const;
		
		/**
		 * @brief Render a bitmap string, using string.
		 * 
		 * @return void
		 **/
		void Draw(IRenderer& renderer,
			const std::string& text,
			const glm::vec2& position,
			uint maxWidth);
		
		/**
		 * @brief Render a bitmap string, using wstring.
		 * 
		 * @return void
		 **/
		void Draw(IRenderer& renderer,
			const std::wstring& text,
			const glm::vec2& position,
			uint maxWidth);
		
		void DrawIndy(IRenderer& renderer,
			const std::wstring& text,
			const glm::vec2& position,
			uint maxWidth);
	private:	
		typedef std::map<wchar_t, Glyph*> GlyphMap;
		typedef std::map<wchar_t, Rectf> RectMap;
        
    private:
		GlyphMap glyphs;
		uint fontSize, dpi, lineSize;
		std::string fontPath;
		FT_Face face;

		Texture2D *texture;
		RectMap texcoords;
	};
}

#endif // KKE_FONT_HPP
