/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LAYER_HPP
#define KKE_LAYER_HPP

#include "IO/Serialize_glm.hpp"
#include "Level/LevelItem.hpp"
#include "Graphics/IRenderer.hpp"

#include <vector>
#include <string>
#include <boost/archive/xml_oarchive.hpp>

namespace kke
{
	class Level;
	class LayerTexture;
	class LayerItem;
	class GameTime;
	
	typedef std::vector<LayerItem*> LayerItems;
	class Layer : public LevelItem
	{
	public:
		explicit Layer(const std::string& name = std::string(), Level* level = 0);
		virtual ~Layer();
		
		LayerItems& Items();
		
		void SetSpeed(const glm::vec2& speed);
		const glm::vec2& GetSpeed();
		
		virtual void* Copy() const;
		virtual void LoadContent();
		virtual void UnloadContent();
		virtual void Update(GameTime& gameTime);
		virtual void Draw(IRenderer& renderer);
		
	private:
		LayerItems items;
		glm::vec2 speed;
		
		friend class boost::serialization::access;
		template<class Archive> 
		void serialize(Archive& ar, const unsigned int version)
		{
			ar.register_type(static_cast<LayerItem*>(0));
			ar.register_type(static_cast<LayerTexture*>(0));
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(LevelItem);
			ar & BOOST_SERIALIZATION_NVP(items);
			ar & BOOST_SERIALIZATION_NVP(speed);
		}
	};
}

#endif // KKE_LAYER_HPP