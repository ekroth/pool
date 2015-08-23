/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef SPRITESHEET_HPP
#define SPRITESHEET_HPP

#include "ResourceType.hpp"
#include "ResourceFactory.hpp"
#include "Interface/INoncopyable.hpp"
#include "Math.hpp"

#include <map>
#include <string>

namespace kke
{
	class Texture2D;

	/**
	 * @brief SpriteSheet - Rectangles sources of a Texture2D.
	 **/
	class SpriteSheet : public ResourceType, public INoncopyable
	{
	public:
		SpriteSheet();
		explicit SpriteSheet(const std::string& source);
		~SpriteSheet();

		/**
		* @brief Load SpriteSheet with XML file + image.
		*
		* @param source <XML>|<IMAGE> .
		* @return bool Result.
		**/
		bool Load(const std::string& source);

		/**
		* @brief Load SpriteSheet with XML file and image.
		*
		* @param map XML specifying sources.
		* @param image Image containing sources.
		* @return bool Result.
		**/
		bool Load(const std::string& map, const std::string& image);
		
		/**
		* @brief Unload SpriteSheet.
		*
		* @return bool Result.
		**/
		bool Unload();

		/**
		* @brief Get Source Rectangle in float of a source.
		*
		* @param name Source name.
		* @return :Rectf& Rectangle<float> 0.0-1.0f, percentage of full Texture2D.
		**/
		const Rectf& GetSource(const std::string& name);
		
		/**
		* @brief Texture2D 
		*
		* @return :Texture2D&
		**/
		const Texture2D& GetTexture() const;

	private:
		std::map<std::string, Rectf> sheets;
		Resource<Texture2D> texture;
	};
}

#endif
